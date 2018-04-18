from trainlib.ModelCollectionConfigFileHandler import ModelCollectionConfigFileHandler
from trainlib.ConfigFileHandler import ConfigFileHandler
from trainlib.generator import Generator
from trainlib.config import Config
from keras import optimizers
import sys

def main():
    
    def _compute_class_weights_lengths(gen, preprocessor, MC_weighting = False):
        # determine the actual size of the available dataset and adjust the sample weights correspondingly
        H1_data = gen.H1_collection.get_data(Config.branches, 0.0, 1.0)
        H0_data = gen.H0_collection.get_data(Config.branches, 0.0, 1.0)
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

        return H1_class_weight, H0_class_weight, H1_length, H0_length

    # this computes low-level performance metrics for a model collection, i.e. the mean-quare error 
    # computed on the validation dataset for each discriminant. Since the validation datasets will be held constant,
    # this is an easy way to directly compare different models
    
    setting_dir = sys.argv[1]
    training_dir = sys.argv[2]
    out_dir = sys.argv[3]
    
    # first, need to read in the trained ModelCollection:
    mconfhandler = ModelCollectionConfigFileHandler()
    mconfhandler.load_configuration(setting_dir + "settings.conf")
    mcolls = mconfhandler.GetModelCollection(weightpath = training_dir)
    
    confhandler = ConfigFileHandler()
    out_path = out_dir + "model_benchmark.txt"
    
    # for the evaluation, need to proceed in the same way as for training, but evaluate the models on the validation
    # data instead of training them on the training data
    
    for mcoll in mcolls:
        models, preprocessors, settings = mcoll.get_models(), mcoll.get_preprocessors(), mcoll.get_settings()
    
        for cur_model, cur_preprocessor, cur_settings in zip(models, preprocessors, settings):
            val_gen = Generator(mcoll.H1_stream, mcoll.H0_stream, Config.branches, preprocessor = cur_preprocessor, chunks = 1, MC_weighting = False)
            val_gen.setup_validation_data()
            val_H1_classweight, val_H0_classweight, H1_length, H0_length = _compute_class_weights_lengths(val_gen, cur_preprocessor, False)
            print val_H1_classweight
            print val_H0_classweight
            print H1_length
            print H0_length
            val_gen.set_H1_weight(val_H1_classweight)
            val_gen.set_H0_weight(val_H0_classweight)
            val_gen.set_minimum_length(0)
            cur_model.get_keras_model().compile(optimizer = optimizers.Adam(), loss = "mean_squared_error", metrics = ["binary_accuracy"])
            res = cur_model.get_keras_model().evaluate_generator(val_gen.preprocessed_generator(), steps = 1)
            print "statistics for model " + cur_model.name
            print res
            print cur_model.get_keras_model().metrics_names

            confhandler.new_section(cur_model.name)
            confhandler.set_field(cur_model.name, 'H0_val_length', str(H0_length))
            confhandler.set_field(cur_model.name, 'H1_val_length', str(H1_length))
            confhandler.set_field(cur_model.name, 'val_loss', str(res[0]))
        
    confhandler.save_configuration(out_path)


if __name__ == "__main__":
    main()
