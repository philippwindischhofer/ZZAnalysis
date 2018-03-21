from trainlib.config import Config

from keras.callbacks import Callback, EarlyStopping, ModelCheckpoint, History
from keras import losses
import matplotlib.pyplot as plt
import os

from generator import generate_training_data, generate_validation_data

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

    def train(self, collection, optimizer, steps_per_epoch = 128, max_epochs = 5):
        models = collection.get_models()
        preprocessors = collection.get_preprocessors()
        settings = collection.get_settings()
        
        print "now training model collection '" + collection.name + "', containing " + str(len(models)) + " models"

        collection_outfolder = self.outfolder + collection.name + "/"
        if not os.path.exists(collection_outfolder):
            os.makedirs(collection_outfolder)

        for (cur_model, cur_preprocessor, setting) in zip(models, preprocessors, settings):
            print "now training model '" + cur_model.name + "'"
            cur_model.get_keras_model().compile(loss = "mean_squared_error", optimizer = optimizer, metrics = ["accuracy"])

            model_outfolder = collection_outfolder + cur_model.name + "/"
            if not os.path.exists(model_outfolder):
                os.makedirs(model_outfolder)

            # may need to set up the preprocessor here, given some raw data. Important: don't do any preprocessing here, but just forward everything. The preprocessor may need access to the full, raw information to set itself up!!
            train_gen = generate_training_data(collection.H1_paths, collection.H0_paths, self.branches, preprocessor = None, training_split = 0.5, chunks = 100, as_matrix = False)
            cur_preprocessor.setup(train_gen, len_setupdata = 20000)

            # recreate the generators yielding the training and validation data for the actual training procedure
            train_gen = generate_training_data(collection.H1_paths, collection.H0_paths, self.branches, preprocessor = cur_preprocessor.process, chunks = 100)
            val_gen = generate_validation_data(collection.H1_paths, collection.H0_paths, self.branches, preprocessor = cur_preprocessor.process, chunks = 100)

            # stops the training as soon as the loss starts to saturate
            early_stop = EarlyStopping(monitor = 'val_loss',
                                       patience = 10,
                                       verbose = 1,
                                       mode = 'auto')

            checkpointer = ModelCheckpoint(filepath = model_outfolder + "checkpoint.hdf5", verbose = 1, save_best_only = True)
        
            logger = Logger()
            history = History()

            cur_model.get_keras_model().fit_generator(train_gen, steps_per_epoch = setting.steps_per_epoch, epochs = setting.max_epochs, verbose = 2, 
                                                      validation_data = val_gen, validation_steps = 10, 
                                                      callbacks = [early_stop, checkpointer, logger, history])

            # save the training report as well as the final version of the trained model
            logger.report_batches(model_outfolder)
            logger.report_epochs(model_outfolder, history.history)
            cur_model.save(model_outfolder, "final.hdf5")

            # save also the preprocessor information for this model
            cur_preprocessor.save(model_outfolder, "preprocessor.pkl")
