from abc import ABCMeta, abstractmethod

class Preprocessor:
    __metaclass__ = ABCMeta

    @abstractmethod
    def __init__(self, processed_columns, cuts):
        pass

    # pass the entire data generator here, to have the preprocessor get as much data as it might desire
    @abstractmethod
    def setup(self, datagen, len_setupdata):
        pass

    @abstractmethod
    def process(self, data):
        pass

    @abstractmethod
    def save(self, folder, filename):
        pass

    @abstractmethod
    def load(self, folder, filename):
        pass
