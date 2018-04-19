from keras.layers import Dense, Activation, LSTM, BatchNormalization, Dropout
from keras.engine.topology import Input
import keras.engine.training
import os
import re

from Model import Model
from ConfigFileUtils import ConfigFileUtils

class CombinedModel(Model):

    # note: list_inputs have the format {"list_input_name": ["a", ...], ...}
    def __init__(self, name, scalar_inputs, list_inputs, hyperparameters):
        self.model = None
        self.name = name
        self.scalar_inputs = scalar_inputs
        self.list_inputs = list_inputs
        self.hyperparameters = hyperparameters

        self.build()

    @classmethod
    def from_config(cls, config_section):
        model_name = re.sub('[\[\]]', '', config_section.name)

        scalar_inputs = ConfigFileUtils.parse_list(config_section['scalar_input_columns'], lambda x: x)
        list_inputs = ConfigFileUtils.parse_dict(config_section['list_input_columns'], 
                                                 lambda x: ConfigFileUtils.parse_list(x, lambda x: x))
        hyperparameters = ConfigFileUtils.parse_dict(config_section['hyperparameters'], lambda y: float(y))

        obj = cls(name = model_name, scalar_inputs = scalar_inputs, list_inputs = list_inputs, hyperparameters = hyperparameters)

        return obj

    def to_config(self, confhandler):
        section_name = '[' + self.name
        confhandler.new_section(section_name)

        confhandler.set_field(section_name, 'model_type', 'CombinedModel')
        confhandler.set_field(section_name, 'scalar_input_columns', ConfigFileUtils.serialize_list(self.scalar_inputs, lambda x: x))
        confhandler.set_field(section_name, 'list_input_columns', ConfigFileUtils.serialize_dict(self.list_inputs, lambda x: ConfigFileUtils.serialize_list(x, lambda y: y)))
        confhandler.set_field(section_name, 'hyperparameters', ConfigFileUtils.serialize_dict(self.hyperparameters, lambda x: str(x)))
        
        return section_name

    def build(self):
        self.default_neurons = 128
        self.default_dropout_rate = 0.2

        # build a separate LSTM for every list input that got passed
        input_layers_lstm = []
        output_layers_lstm = []
        for input_name, input_list in self.list_inputs.iteritems():
            print input_name
            print input_list

            # TODO: check dynamically the size of the actual input list (the encoding of the periodic angle columns increases the size!)
            input_layer = Input(shape = (None, 1 + len(input_list)), name = self.name + "_" + input_name)

            # number units = dimensionality of the output space
            lstm = LSTM(units = int(self.hyperparameters.get('LSTM_units', 16)), return_sequences = False)(input_layer)
            x = Dense(int(self.hyperparameters.get('LSTM_output_size', 4)))(lstm)
            x = BatchNormalization()(x)
            x = Activation('relu')(x)
            output_layer = Dropout(self.default_dropout_rate)(x)

            input_layers_lstm.append(input_layer)
            output_layers_lstm.append(output_layer)
        
        input_layer_scalar = Input(shape = (len(self.scalar_inputs),), name = self.name + "_" + 'scalar_inputs')
        
        x = keras.layers.concatenate(output_layers_lstm + [input_layer_scalar])

        number_dense_layers = int(self.hyperparameters.get('number_dense_layers', 2))
        number_dense_neurons = int(self.hyperparameters.get('number_dense_neurons', self.default_neurons))
        for layer in range(number_dense_layers):
            x = Dense(number_dense_neurons)(x)
            x = BatchNormalization()(x)
            x = Activation('relu')(x)
            x = Dropout(self.default_dropout_rate)(x)

        output_layer = Dense(1, activation = 'sigmoid', name = 'target')(x)
        
        self.model = keras.engine.training.Model(inputs = input_layers_lstm + [input_layer_scalar], outputs = [output_layer], name = self.name)

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

