from abc import ABCMeta, abstractmethod

class Model:
    __metaclass__ = ABCMeta

    @abstractmethod
    def __init__(self, name, input_columns):
        pass

    @abstractmethod
    def build(self, hyperparameters = None):
        pass

    @abstractmethod
    def get_keras_model(self):
        pass

    @abstractmethod
    def save(self, folder, filename):
        pass

    @abstractmethod
    def load(self, folder, filename):
        pass
