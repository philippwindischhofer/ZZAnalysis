from ROOT import TFile
from root_numpy import root2array, root2rec, tree2array
import array
import pandas as pd
import tensorflow as tf
from keras.models import Sequential
from keras.layers import Dense, Activation
from keras.layers import LSTM
from keras.callbacks import EarlyStopping
from keras import losses, optimizers
from keras import backend as K
import math

config = tf.ConfigProto(intra_op_parallelism_threads=10, inter_op_parallelism_threads=10,                         
                        allow_soft_placement=True, device_count = {'CPU': 10})
session = tf.Session(config = config)
K.set_session(session)

inpath = "/data_CMS/cms/tsculac/CJLST_NTuples/"
filename = "/ZZ4lAnalysis.root"
H1_files = ["VBFH125"]
H0_files = ["ggH125"]

def get_size(filepath):
    print "trying to open " + filepath
    f = TFile.Open(filepath)
    size = f.Get("ZZTree/candTree").GetEntries()
    f.Close()

    return size

# this simulates a single ROOT file that is actually distributed over multiple "physical" ROOT trees. From each file in the list,
# only the portion between start_fraction and end_fraction is considered
class FileCollection:
    def __init__(self, files, start_fraction, end_fraction):
        self.files = files
        self.start_fraction = start_fraction
        self.end_fraction = end_fraction

        self.lengths = [get_size(file_path) for file_path in files]
        

def generate_training_data(H1_files, H0_files, branches, training_split = 0.5, chunks = 100):
    return datagen(H1_files, H0_files, branches, start_fraction = 0.0, end_fraction = training_split, chunks = chunks)

def generate_validation_data(H1_files, H0_files, branches, training_split = 0.5, chunks = 100):
    return datagen(H1_files, H0_files, branches, start_fraction = training_split, end_fraction = 1.0, chunks = chunks)

# builds a generator that returns data 
def datagen(H1_files, H0_files, branches, start_fraction, end_fraction, chunks = 100):
    H1_curpos = 0
    H0_curpos = 0
    
    # build arrays holding the number of entries for each file
    H1_lengths = [get_size(inpath + H1_file + filename) for H1_file in H1_files]
    H0_lengths = [get_size(inpath + H0_file + filename) for H0_file in H0_files]
    
    H1_minpos = [int(H1_length * start_fraction) for H1_length in H1_lengths]
    H0_minpos = [int(H0_length * start_fraction) for H0_length in H1_lengths]
    
    H1_maxpos = [int(H1_length * end_fraction) for H1_length in H1_lengths]
    H0_maxpos = [int(H0_length * end_fraction) for H0_length in H0_lengths]

    H1_total_length = sum(H1_lengths)
    H0_total_length = sum(H0_lengths)
    
    H1_used_length = sum(H1_maxpos)
    H0_used_length = sum(H0_maxpos)
    
    print "H1 contains " + str(H1_total_length) + " entries, " + str(H1_used_length) + " of which will be used"
    print "H0 contains " + str(H0_total_length) + " entries, " + str(H0_used_length) + " of which will be used"

    # the chunksize is global, i.e. spans the entire list of files!
    H1_chunksize = int(H1_used_length / chunks)
    H0_chunksize = int(H0_used_length / chunks)
    
    print "using the following chunk sizes: " + "(" + str(H1_chunksize) + " / " + str(H0_chunksize) + ")"
    
    # deliver data forever, it will be the job of the early_stopping to actually terminate the training
    while True:
        # prepare next training data chunk set by drawing events randomly from the two files
        H1_data = pd.DataFrame(root2array(inpath + H0_file + filename, treename = "ZZTree/candTree", 
                    branches = branches, start = H1_curpos, stop = H1_curpos + H1_chunksize))

        H0_data = pd.DataFrame(root2array(inpath + H1_file + filename, treename = "ZZTree/candTree",
                    branches = branches, start = H0_curpos, stop = H0_curpos + H0_chunksize))
                        
        # update the starting position for the next chunk
        H1_curpos += H1_chunksize
        H0_curpos += H0_chunksize
        
        if H1_curpos > H1_maxpos:
            H1_curpos = H1_minpos
            
        if H0_curpos > H0_maxpos:
            H0_maxpos = H0_minpos

        # add the truth information
        H1_data["target"] = 1.0
        H0_data["target"] = 0.0

        data_chunk = pd.concat([H1_data, H0_data])

        # return a randomized signal + background sample
        training_data = data_chunk.sample(frac = 1)
        input_data = training_data[branches].as_matrix()
        target_data = training_data["target"].as_matrix()
        
        yield input_data, target_data

branches = ["PFMET", "nCleanedJetsPt30"]

train_gen = generate_training_data(H0_files, H1_files, branches, chunks = 100)

for training_data, target_data in train_gen:
    print training_data

# # make a simple Keras model, just for Hello World purposes
# model = Sequential()

# model.add(Dense(32, input_shape=(2,)))
# model.add(Dense(32))
# model.add(Dense(32))
# model.add(Dense(1))
# model.add(Activation("relu"))

# sgd = optimizers.SGD(lr = 0.1)
# model.compile(loss = "mean_squared_error", optimizer = sgd, metrics = ["accuracy"])

# train_gen = generate_training_data(H0_files, H1_files, branches, chunks = 100)
# val_gen = generate_validation_data(H0_files, H1_files, branches, chunks = 100)

# early_stop = EarlyStopping(monitor = 'val_loss',
#                           patience = 10,
#                           verbose = 1,
#                           mode = 'auto')

# ret = model.fit_generator(train_gen, steps_per_epoch = 128, epochs = 50, verbose = 2, validation_data = val_gen, validation_steps = 10, callbacks = [early_stop])

