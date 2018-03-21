from Preprocessor import Preprocessor

class SimplePreprocessor(Preprocessor):
    def __init__(self):
        pass

    def setup(self, datagen):
        print "this is the setup of the simple preprocessor"

    def process(self, data):
        return data.loc[data["nCleanedJetsPt30"] >= 2]
