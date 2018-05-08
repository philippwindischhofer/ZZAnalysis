from config import TrainingConfig
from ConfigFileHandler import ConfigFileHandler
from ConfigFileUtils import ConfigFileUtils
import pandas as pd
import os

# describes an ensemble of classifiers for a specific pair of categories. Each model in the collection targets a different region in the event space (as filtered out by the corresponding preprocessors)
class ModelCollection:

    def __init__(self, name = "noname", H1_stream = None, H0_stream = None, default_value = 0.0, H1_name = None, H0_name = None, H1_stream_unmixed = None, H0_stream_unmixed = None):
        self.model_dict = {}
        self.preprocessor_dict = {}
        self.settings_dict = {}
        self.name = name

        self.H1_stream = H1_stream
        self.H0_stream = H0_stream

        if H1_stream_unmixed is not None:
            self.H1_stream_unmixed = H1_stream_unmixed
        else:
            self.H1_stream_unmixed = H1_stream

        if H0_stream_unmixed is not None:
            self.H0_stream_unmixed = H0_stream_unmixed
        else:
            self.H0_stream_unmixed = H0_stream

        # names of the two categories this discriminant seeks to separate
        self.H1_name = H1_name
        self.H0_name = H0_name

        self.default_value = default_value

    @classmethod
    def from_discriminant_endpieces(cls, cat_a, ep_a, cat_b, ep_b, input_config_file = None):
        # open the input config file, if specified
        if input_config_file is not None:
            confhandler = ConfigFileHandler()
            confhandler.load_configuration(input_config_file)
        else:
            confhandler = None

        mcoll_name = "D_" + cat_a.name + "_" + cat_b.name + "_ML"
        H1_stream = cat_a.datastream
        H0_stream = cat_b.datastream

        H1_stream_unmixed = cat_a.datastream_unmixed
        H0_stream_unmixed = cat_b.datastream_unmixed

        H1_name = cat_a.name
        H0_name = cat_b.name
        
        obj = cls(name = mcoll_name, H1_stream = H1_stream, H0_stream = H0_stream, H1_name = H1_name, H0_name = H0_name, H1_stream_unmixed = H1_stream_unmixed, H0_stream_unmixed = H0_stream_unmixed)

        # now construct and add the models specified by the two matching endpieces
        for cur_comp in ep_a.endpiece_components.keys():
            
            # as soon as two endpieces have the same name, they will have compatible cuts and compatible model and preprocessor basetypes <-> otherwise they cannot be matched!
            if ep_a[cur_comp].public_name:
                model_name = "D_" + cat_a.name + "_" + cat_b.name + "_" + ep_a[cur_comp].public_name + "_ML"
            else:
                model_name = mcoll_name

            cuts = ep_a[cur_comp].component_cut
            model_basetype = ep_a[cur_comp].model_basetype
            hyperparams = ep_a[cur_comp].model_hyperparams
            preprocessor_basetype = ep_a[cur_comp].preprocessor_basetype
            
            # only need to join the lists of inputs, such that every endpiece has the ones it needs
            periodic_columns = list(set(ep_a[cur_comp].periodic_columns + ep_b[cur_comp].periodic_columns))
            nonperiodic_columns = list(set(ep_a[cur_comp].nonperiodic_columns + ep_b[cur_comp].nonperiodic_columns))

            # if an input configuration file is specified, try to look if it contains an entry for this model; if so, use the variables specified therein instead of the default above
            if confhandler is not None:
                if model_name in confhandler.get_sections():
                    print "found input configuration entry for model " + str(model_name)
                    periodic_columns = filter(None, ConfigFileUtils.parse_list(confhandler.get_field(model_name, "periodic_columns"), lambda x: x))
                    nonperiodic_columns = filter(None, ConfigFileUtils.parse_list(confhandler.get_field(model_name, "nonperiodic_columns"), lambda x: x))
                else:
                    print "no input configuration entry found for model " + str(model_name)
            
            # now can construct all model components and add them to the collection
            pre = preprocessor_basetype(name = model_name + "_input", nonperiodic_columns = nonperiodic_columns, periodic_columns = periodic_columns, cuts = cuts)
            mod = model_basetype(model_name, pre.number_processed_columns(), hyperparams)
            sett = TrainingConfig()

            obj.add_model(pre, mod, sett)
        
        return obj

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
            # in loading back the model, prefer to use the checkpoint file. Only if this is not available, go for the final training output (which might be overfitted)
            try:
                self.model_dict[model_dir].load(model_path, "final.hdf5")
                print "read weights from " + model_path + "final.hdf5"
            except IOError:
                self.model_dict[model_dir].load(model_path, "checkpoint.hdf5")
                print "read weights from " + model_path + "checkpoint.hdf5"

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
            
