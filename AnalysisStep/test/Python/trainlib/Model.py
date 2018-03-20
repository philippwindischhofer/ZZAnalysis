from abc import ABCMeta, abstractmethod

class Model:
    __metaclass__ = ABCMeta

    @abstractmethod
    def __init__(self, name):
        pass

    @abstractmethod
    def build(self):
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
