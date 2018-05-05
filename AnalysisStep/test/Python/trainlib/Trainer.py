from trainlib.config import Config

import matplotlib as mpl
mpl.use('Agg')
import matplotlib.pyplot as plt

from keras.callbacks import Callback, EarlyStopping, ModelCheckpoint, History
from keras import losses
import numpy as np
import os
import copy

from generator import Generator

# records the training history and makes nice plots at the end
class Logger(Callback):
    def on_train_begin(self, logs = {}):
        self.losses = []

    def on_batch_end(self, batch, logs = {}):
        self.losses.append(logs.get('loss'))

    def report_batches(self, outfolder):
        batches = range(1, len(self.losses) + 1) # will form the x axis of the loss plot
        plt.figure()
        plt.plot(batches, self.losses, label = "training loss")
        plt.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3,
           ncol=2, mode="expand", borderaxespad=0.)
        plt.xlabel("batch")
        plt.savefig(outfolder + "batch_losses.pdf")

    def report_epochs(self, outfolder, history):
        epoch_losses = history.get('loss')
        epoch_val_losses = history.get('val_loss')
        epochs = range(1, len(epoch_losses) + 1)

        plt.figure()
        plt.plot(epochs, epoch_losses, label = "training loss")
        plt.plot(epochs, epoch_val_losses, label = "validation loss")
        plt.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3,
           ncol=2, mode="expand", borderaxespad=0.)
        plt.xlabel("epoch")
        plt.savefig(outfolder + "epoch_losses.pdf")
    
class Trainer:
    def __init__(self, outfolder):
        self.outfolder = outfolder

        if not os.path.exists(outfolder):
            os.makedirs(outfolder)

        # branches of the ROOT trees that are going to be loaded, and on which the preprocessor is going to operate
        self.branches = Config.branches

    def _compute_class_weights(self, gen, preprocessor, MC_weighting = False):
        # determine the actual size of the available dataset and adjust the sample weights correspondingly
        H1_data = gen.H1_collection.get_data(self.branches, 0.0, 1.0)
        H0_data = gen.H0_collection.get_data(self.branches, 0.0, 1.0)
        H1_length = len(preprocessor.process(H1_data).values()[0])
        H1_indices = preprocessor.get_last_indices()
        H0_length = len(preprocessor.process(H0_data).values()[0])
        H0_indices = preprocessor.get_last_indices()

        print "H1_length = " + str(H1_length)
        print "H0_length = " + str(H0_length)

        # if per-sample weighting is enabled, also set up the normalization of the event weights
        if MC_weighting:
            H1_weight_sum = np.sum(np.maximum(np.array(H1_data["training_weight"][H1_indices]), 0.0))
            H0_weight_sum = np.sum(np.maximum(np.array(H0_data["training_weight"][H0_indices]), 0.0))
        
            H1_class_weight = float(H0_length) / H1_weight_sum
            H0_class_weight = float(H1_length) / H0_weight_sum
        else:
            # H1_class_weight = 1.0
            # H0_class_weight = float(H1_length) / float(H0_length)
            H1_class_weight = 1.0 + float(H0_length) / float(H1_length)
            H0_class_weight = 1.0 + float(H1_length) / float(H0_length)

        return H1_class_weight, H0_class_weight

    def _dataset_lengths(self, gen, preprocessor):
        H1_data = gen.H1_collection.get_data(self.branches, 0.0, 1.0)
        H0_data = gen.H0_collection.get_data(self.branches, 0.0, 1.0)

        H1_length = len(preprocessor.process(H1_data).values()[0])
        H0_length = len(preprocessor.process(H0_data).values()[0])

        return H1_length, H0_length        

    def train(self, collection, optimizer, steps_per_epoch = 128, max_epochs = 5, MC_weighting = False):
        if MC_weighting:
            print "using MC weights in training"
        
        models = collection.get_models()
        preprocessors = collection.get_preprocessors()
        settings = collection.get_settings()
        
        print "now training model collection '" + collection.name + "', containing " + str(len(models)) + " models"

        collection_outfolder = self.outfolder + collection.name + "/"
        if not os.path.exists(collection_outfolder):
            os.makedirs(collection_outfolder)

        for (cur_model, cur_preprocessor, setting) in zip(models, preprocessors, settings):
            print "now training model '" + cur_model.name + "'"
            cur_model.get_keras_model().compile(loss = "mean_squared_error", optimizer = optimizer, metrics = ["binary_accuracy"])

            model_outfolder = collection_outfolder + cur_model.name + "/"
            if not os.path.exists(model_outfolder):
                os.makedirs(model_outfolder)

            # may need to set up the preprocessor here, given some raw data. Important: don't do any preprocessing here, but just forward everything. The preprocessor may need access to the full, raw information to set itself up!!
            setup_gen = Generator(collection.H1_stream, collection.H0_stream, self.branches, preprocessor = None, training_split = 0.66, chunks = 100)
            len_setup_data = setup_gen.setup_training_data()
            cur_preprocessor.setup_generator(setup_gen.raw_generator_scrambled(), len_setupdata = len_setup_data)

            # now have a fully set up preprocessor
            # recreate the generators yielding the training and validation data for the actual training procedure
            train_gen = Generator(collection.H1_stream, collection.H0_stream, self.branches, preprocessor = cur_preprocessor, chunks = setting.steps_per_epoch, MC_weighting = MC_weighting)            
            train_gen.setup_training_data()

            train_H1_classweight, train_H0_classweight = self._compute_class_weights(train_gen, cur_preprocessor, MC_weighting)
            train_H1_length, train_H0_length = self._dataset_lengths(train_gen, cur_preprocessor)
            train_chunks_per_epoch = float(train_H1_length + train_H0_length) / setting.batch_size
            print "using " + str(train_chunks_per_epoch) + " chunks for training"

            train_gen.set_number_chunks(train_chunks_per_epoch)
            train_gen.set_minimum_length(setting.batch_size)
            train_gen.set_H1_weight(train_H1_classweight)
            train_gen.set_H0_weight(train_H0_classweight)

            # make a copy of the preprocessor for the validation data generator (important, as these will be running concurrently)
            val_preprocess = copy.deepcopy(cur_preprocessor)
            val_gen = Generator(collection.H1_stream, collection.H0_stream, self.branches, preprocessor = val_preprocess, chunks = setting.steps_per_epoch, MC_weighting = MC_weighting)
            val_gen.setup_validation_data()

            val_H1_classweight, val_H0_classweight = self._compute_class_weights(val_gen, val_preprocess, MC_weighting)
            val_H1_length, val_H0_length = self._dataset_lengths(val_gen, val_preprocess)
            val_chunks_per_epoch = float(val_H1_length + val_H0_length) / setting.batch_size
            print "using " + str(val_chunks_per_epoch) + " chunks for validation"

            val_gen.set_number_chunks(val_chunks_per_epoch)
            val_gen.set_minimum_length(setting.batch_size)
            val_gen.set_H1_weight(val_H1_classweight)
            val_gen.set_H0_weight(val_H0_classweight)

            # stops the training as soon as the loss starts to saturate
            early_stop = EarlyStopping(monitor = 'val_loss',
                                       patience = 7,
                                       verbose = 1,
                                       mode = 'auto')

            checkpointer = ModelCheckpoint(filepath = model_outfolder + "checkpoint.hdf5", verbose = 1, save_best_only = True)
        
            logger = Logger()
            history = History()

            cur_model.get_keras_model().fit_generator(train_gen.preprocessed_generator(), steps_per_epoch = train_chunks_per_epoch, epochs = setting.max_epochs, verbose = 2, 
                                                      validation_data = val_gen.preprocessed_generator(), validation_steps = val_chunks_per_epoch, 
                                                      callbacks = [early_stop, checkpointer, logger, history])

            # save the training report as well as the final version of the trained model
            logger.report_batches(model_outfolder)
            logger.report_epochs(model_outfolder, history.history)

            # however: the final model is the one at the *end* of the training campaign, and may therefore contain (significant) overtraining 
            cur_model.save(model_outfolder, "final.hdf5")

            # save also the preprocessor information for this model
            cur_preprocessor.save(model_outfolder, "preprocessor.pkl")
