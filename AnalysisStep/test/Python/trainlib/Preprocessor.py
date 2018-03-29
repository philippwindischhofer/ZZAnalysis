from abc import ABCMeta, abstractmethod

class Preprocessor:
    __metaclass__ = ABCMeta

    @abstractmethod
    def __init__(self, name, processed_columns, cuts):
        pass

    # pass the entire data generator here, to have the preprocessor get as much data as it might desire
    @abstractmethod
    def setup_generator(self, datagen, len_setupdata):
        pass

    # set itself up on a given chunk of data (passed as a Pandas frame)
    @abstractmethod
    def setup(self, data):
        pass

    @abstractmethod
    def process(self, data):
        pass

    # returns the indices of the Pandas frame that was the last one that got processed
    @abstractmethod
    def get_last_indices(self):
        pass

    @abstractmethod
    def save(self, folder, filename):
        pass

    @abstractmethod
    def load(self, folder, filename):
        pass
