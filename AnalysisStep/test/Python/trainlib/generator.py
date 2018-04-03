from FileCollection import FileCollection
from utils import read_data
import pandas as pd
import numpy as np

class Generator:
    
    def __init__(self, H1_stream, H0_stream, branches, preprocessor, training_split = 0.5, chunks = 1000):
        self.H1_stream = H1_stream
        self.H0_stream = H0_stream

        self.branches = branches
        self.preprocessor = preprocessor

        self.training_split = training_split
        self.chunks = chunks

        self.H1_collection = None
        self.H0_collection = None

    def setup_training_data(self):
        self.H1_collection = FileCollection(self.H1_stream, start_fraction = 0.0, end_fraction = self.training_split)
        self.H0_collection = FileCollection(self.H0_stream, start_fraction = 0.0, end_fraction = self.training_split)
        return self.H1_collection.get_length() + self.H0_collection.get_length()
        
    def setup_validation_data(self):
        self.H1_collection = FileCollection(self.H1_stream, start_fraction = self.training_split, end_fraction = 1.0)
        self.H0_collection = FileCollection(self.H0_stream, start_fraction = self.training_split, end_fraction = 1.0)
        return self.H1_collection.get_length() + self.H0_collection.get_length()

    def get_H1_length(self):
        return self.H1_collection.get_length()

    def get_H0_length(self):
        return self.H0_collection.get_length()
        
    def raw_generator(self):
        H1_curpos = 0
        H0_curpos = 0

        H1_maxpos = self.H1_collection.get_length()
        H0_maxpos = self.H0_collection.get_length()

        print "H1 contains " + str(H1_maxpos) + " entries"
        print "H0 contains " + str(H0_maxpos) + " entries"
            
        H1_chunksize = int(H1_maxpos / self.chunks)
        H0_chunksize = int(H0_maxpos / self.chunks)
            
        print "using the following chunk sizes: " + "(" + str(H1_chunksize) + " / " + str(H0_chunksize) + ")"
    
        # as a generator, deliver data forever
        while True:
                
            if H1_curpos + H1_chunksize > H1_maxpos:
                H1_curpos = 0
                
            if H0_curpos + H0_chunksize > H0_maxpos:
                H0_curpos = 0
        
            # prepare next training data chunk set by drawing events randomly from the two files
            H1_data = read_data(self.H1_collection, branches = self.branches, start = H1_curpos, stop = H1_curpos + H1_chunksize)
            H0_data = read_data(self.H0_collection, branches = self.branches, start = H0_curpos, stop = H0_curpos + H0_chunksize)

            # fix the indexing (indices can occur multiple times, if H1_data comes (say) from two different files, during the overlap)
            H1_data = H1_data.reset_index(drop = True)
            H0_data = H0_data.reset_index(drop = True)
        
            # update the starting position for the next chunk
            H1_curpos += H1_chunksize
            H0_curpos += H0_chunksize

            yield H1_data, H0_data
        
    def raw_generator_scrambled(self):
        for H1_data, H0_data in self.raw_generator():
            data_chunk = pd.concat([H1_data, H0_data])
            
            # scramble it
            scrambled_data = data_chunk.sample(frac = 1)

            yield scrambled_data

    def preprocessed_generator(self):
        for H1_data, H0_data in self.raw_generator():
            # run the preprocessing: this will in general change the number of rows (if there are cuts hidden inside the preprocessor) AND the number of columns (if not all loaded columns are meant as input to the classifier)
            H1_processed = self.preprocessor(H1_data)
            H0_processed = self.preprocessor(H0_data)

            # len() automatically maps to the first dimension of a numpy array!
            # implicit assumption: all dictionaries delivered by a preprocessor must have the same length
            H1_samples = len(H1_processed.values()[0])
            H0_samples = len(H0_processed.values()[0])

            target_data = np.concatenate([np.ones(H1_samples), np.zeros(H0_samples)], axis = 0)
            perm = np.random.permutation(len(target_data))

            # now concatenate all input array found in H1_processed & H0_processed
            input_data = {}
            for key in H1_processed:
                temp = np.concatenate([H1_processed[key], H0_processed[key]], axis = 0)
                input_data[key] = temp[perm]

            target_data = {"target": target_data[perm]}

            yield input_data, target_data
