import pandas as pd
import os

# describes an ensemble of classifiers for a specific pair of categories. Each model in the collection targets a different region in the event space (as filtered out by the corresponding preprocessors)
class ModelCollection:

    def __init__(self, name = "noname", H1_stream = None, H0_stream = None, default_value = 0.0):
        self.model_dict = {}
        self.preprocessor_dict = {}
        self.settings_dict = {}
        self.name = name

        self.H1_stream = H1_stream
        self.H0_stream = H0_stream

        self.default_value = default_value

    def get_models(self):
        return self.model_dict.values()

    def get_preprocessors(self):
        return self.preprocessor_dict.values()

    def get_settings(self):
        return self.settings_dict.values()

    def add_model(self, preprocessor, model, settings):
        self.model_dict[model.name] = model
        self.preprocessor_dict[model.name] = preprocessor
        self.settings_dict[model.name] = settings

    def load_weights(self, path):
        if path is None:
            return

        # look at the directory structure starting at path, and attempt to load the weights of all the models that are contained in this collection

        # remove the last slash, if there is one (to ensure os.path.basename can operate correctly)
        if path.endswith("/"):
            path = path[:-1]

        inferred_collection_name = os.path.basename(path)

        if inferred_collection_name != self.name:
            raise IOError("Error: are you sure this is the right directory for this collection? [" + inferred_collection_name + " != " + self.name + "]")

        possible_model_dirs = next(os.walk(path))[1]
        model_dirs = []
        print "found the following models belonging to this collection:"
        for model_dir in possible_model_dirs:
            if model_dir in self.model_dict:
                model_dirs.append(model_dir)
                print model_dir

        for model_dir in model_dirs:
            model_path = path + "/" + model_dir + "/"
            # in loading back the model, prefer to use the checkpoint file. Only if this is not available, go for the final training output
            try:
                self.model_dict[model_dir].load(model_path, "checkpoint.hdf5")
            except IOError:
                self.model_dict[model_dir].load(model_path, "final.hdf5")

            # also read back the preprocessor information here from its own file!
            self.preprocessor_dict[model_dir].load(model_path, "preprocessor.pkl")

    # contrary to its low-level equivalent model.predict, this one works directly on the Pandas dataframe, and not on the numpy array itself (the pandas dataframe is assumed to contain a unique index column)
    def predict(self, dataframe):
        # now need to sift through the dataframe, and apply each model on the slice that it is designed to treat.
        predictions = []

        for (model, preprocessor) in zip(self.get_models(), self.get_preprocessors()):
            print "now evaluating model '" + model.name + "'"
            
            # have each model give predictions on the relevant pieces of the full data
            cur_data = preprocessor.process(dataframe)

            if len(preprocessor.get_last_indices()) != 0:
                cur_prediction = model.get_keras_model().predict(x = cur_data, verbose = 2, batch_size = len(cur_data)).flatten()
                # then, keep track of the indices (i.e. the positions of the individual chunks in the main datastream)
                cur_series = pd.Series(cur_prediction, index = preprocessor.get_last_indices())

                predictions.append(cur_series)

        if len(predictions) != 0:
            # merge the individual chunks of predictions ...
            retval = pd.concat(predictions).sort_index()
        else:
            retval = pd.DataFrame(columns = dataframe.columns)

        # now, need to fill in any open spots that no preprocessor took (e.g. because there hasn't been a model defined (yet) for this region in event space)
        missed_indices = [ind for ind in dataframe.index if ind not in retval.index]
        missed_retval = pd.Series(self.default_value, index = missed_indices)

        print "had no preprocessor available for " + str(len(missed_indices)) + " entries"

        retval = pd.concat([retval, missed_retval]).sort_index()

        #print retval

        # ... and return them as numpy array
        return retval.as_matrix()
            
