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

        self.H1_weight = 1.0
        self.H0_weight = 1.0

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

    def set_H1_weight(self, weight):
        self.H1_weight = weight

    def set_H0_weight(self, weight):
        self.H0_weight = weight

    def preprocessed_generator(self):
        # build the class weights
        # class_weight = {0: float(self.H1_weight) / float(self.H0_weight),
        #                 1: 1.0}

        # print "using class_weights = " + str(class_weight)

        for H1_data, H0_data in self.raw_generator():
            # run the preprocessing: this will in general change the number of rows (if there are cuts hidden inside the preprocessor) AND the number of columns (if not all loaded columns are meant as input to the classifier)
            H1_processed = self.preprocessor.process(H1_data)
            H1_indices = self.preprocessor.get_last_indices()
            H0_processed = self.preprocessor.process(H0_data)
            H0_indices = self.preprocessor.get_last_indices()

            # print "H1_indices"
            # print H1_indices

            # print "H0_indices"
            # print H0_indices

            # len() automatically maps to the first dimension of a numpy array!
            # implicit assumption: all dictionary entries delivered by a preprocessor must have the same length
            H1_samples = len(H1_processed.values()[0])
            H0_samples = len(H0_processed.values()[0])

            # prepare the weights for each sample
            #sample_weights = np.concatenate([np.full(H1_samples, class_weight[1]), np.full(H0_samples, class_weight[0])], axis = 0)
            H1_sample_weights = np.array(H1_data["training_weight"][H1_indices])
            H0_sample_weights = np.array(H0_data["training_weight"][H0_indices])

            #print H1_samples
            #print len(H1_sample_weights)

            #print H0_samples
            #print len(H0_sample_weights)

            # multiply with the per-class weights
            H1_sample_weights *= self.H1_weight
            H0_sample_weights *= self.H0_weight

            sample_weights = np.concatenate([H1_sample_weights, H0_sample_weights], axis = 0)

            # print "sample_weights"
            # print sample_weights

            target_data = np.concatenate([np.ones(H1_samples), np.zeros(H0_samples)], axis = 0)
            
            #print len(target_data)
            #print H1_samples + H0_samples

            perm = np.random.permutation(len(target_data))

            #print len(perm)

            # now concatenate all input array found in H1_processed & H0_processed
            input_data = {}
            for key in H1_processed:
                temp = np.concatenate([H1_processed[key], H0_processed[key]], axis = 0)
                input_data[key] = temp[perm]

            target_data = {"target": target_data[perm]}
            sample_weights = sample_weights[perm]

            yield input_data, target_data, sample_weights
