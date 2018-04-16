from keras.layers import Dense, Activation, BatchNormalization, Dropout
from keras.engine.topology import Input
import keras.engine.training
import os
import re

from Model import Model
from ConfigFileUtils import ConfigFileUtils

class SimpleModel(Model):

    def __init__(self, name, number_input_columns, hyperparameters = None):
        self.model = None
        self.number_input_columns = number_input_columns
        self.name = name
        self.hyperparameters = hyperparameters

        self.build()

    @classmethod
    def from_config(cls, config_section):
        model_name = re.sub('[\[\]]', '', config_section.name) # remove any stray brackets from the model's name
        number_input_columns = int(config_section['number_input_columns'])
        hyperparameters = ConfigFileUtils.parse_dict(config_section['hyperparameters'], lambda x: float(x))
        obj = cls(name = model_name, number_input_columns = number_input_columns, hyperparameters = hyperparameters)
        
        return obj

    def to_config(self, confhandler):
        section_name = '[' + self.name
        confhandler.new_section(section_name)

        confhandler.set_field(section_name, 'model_type', 'SimpleModel')
        confhandler.set_field(section_name, 'number_input_columns', str(self.number_input_columns))
        confhandler.set_field(section_name, 'hyperparameters', ConfigFileUtils.serialize_dict(self.hyperparameters, lambda x: str(x)))

        return section_name # return handler to the newly added section

    # actually constructs the model
    def build(self):
        self.default_neurons = 128
        self.default_dropout_rate = 0.2

        if self.hyperparameters is not None:
            print "got the following list of hyperparams: " + str(self.hyperparameters)

        print "building network with " + str(self.number_input_columns) + " inputs"

        in_layer = Input(shape = (self.number_input_columns,), name = self.name + '_input')
        x = Dense(self.default_neurons)(in_layer)
        x = BatchNormalization()(x)
        x = Activation('relu')(x)
        x = Dropout(self.default_dropout_rate)(x)

        if self.hyperparameters is not None:
            for layer in range(int(self.hyperparameters.get('number_layers', 0))):
                x = Dense(int(self.hyperparameters.get('number_neurons', self.default_neurons)))(x)
                x = BatchNormalization()(x)
                x = Activation('relu')(x)
                x = Dropout(self.default_dropout_rate)(x)

        out_layer = Dense(1, activation = "sigmoid", name = "target")(x)
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
        print "now attempting to load model from file " + path
        self.model.load_weights(path)

