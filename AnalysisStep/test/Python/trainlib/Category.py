from ConfigFileUtils import ConfigFileUtils
import trainlib.cuts
import os

class Category:
    
    def __init__(self, name, datastream, datastream_unmixed = None):
        self.name = name
        self.datastream = datastream

        if datastream_unmixed is not None:
            self.datastream_unmixed = datastream_unmixed
        else:
            self.datastream_unmixed = self.datastream

        self.endpieces = []

    def add_endpiece(self, endpiece):
        self.endpieces.append(endpiece)

    def has_matching_endpiece(self, endpiece):
        for ep in self.endpieces:
            if ep.name == endpiece.name:
                return ep

        return None

    @classmethod
    def from_config(cls, config_section, category_name, MC_path):
        # read in the location of the MC files for training ...
        training_data = ConfigFileUtils.parse_dict(config_section["training_data"], lambda x: eval(x))
        
        # ... and prepend them with the absolute path to the training file directory
        training_data_stream = {}
        for key, val in training_data.iteritems():
            training_data_stream[os.path.join(MC_path, key, "ZZ4lAnalysis.root")] = val
            
        # check if there is an "unmixed" training data stream available as well
        training_data_stream_unmixed = None
            
        if "calibration_data" in config_section:
            print "have dedicated calibration data available"
            calibration_data = ConfigFileUtils.parse_dict(config_section["calibration_data"], lambda x: eval(x))
            training_data_stream_unmixed = {}
            for key, val in calibration_data.iteritems():
                training_data_stream_unmixed[os.path.join(MC_path, key, "ZZ4lAnalysis.root")] = val

        # create category object
        return cls(category_name, training_data_stream, training_data_stream_unmixed)

    # match two categories, i.e. look at the DiscriminantEndpieces they contain and return the preferred way of matching them
    @staticmethod
    def match(cat_a, cat_b):

        # need to check for a match while respecting the preference order!
        for ep_a in cat_a.endpieces:
            ep_b = cat_b.has_matching_endpiece(ep_a)

            if ep_b:
                print "matched " + cat_a.name + " with " + cat_b.name + ", using endpieces " + ep_a.name + " and " + ep_b.name
            
                return cat_a, ep_a, cat_b, ep_b
        
        print "Error: these categories are mutually incompatible!"
        return None
