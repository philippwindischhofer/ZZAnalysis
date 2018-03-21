from keras.layers import Dense, Activation
from keras.engine.topology import Input
import keras.engine.training
import os

from Model import Model

class SimpleModel(Model):

    def __init__(self, name):
        self.model = None
        self.name = name

    # actually constructs the model
    def build(self, number_inputs):
        in_layer = Input(shape = (number_inputs,))
        x = Dense(128, activation = "tanh")(in_layer)
        x = Dense(16, activation = "tanh")(x)
        out_layer = Dense(1, activation = "tanh", name = "out_layer")(x)
        self.model = keras.engine.training.Model(in_layer, out_layer, name = "simple_model")

    def get_keras_model(self):
        return self.model

    def save(self, folder, filename):
        path = os.path.join(folder, filename)
        if not os.path.exists(folder):
            os.mkdir(folder)

        self.model.save_weights(path)

    def load(self, folder, filename):
        path = os.path.join(folder, filename)
        
        self.model.load_weights(path)

