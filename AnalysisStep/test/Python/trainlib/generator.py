from FileCollection import FileCollection
from utils import read_data
import pandas as pd

class Generator:
    
    def __init__(self, H1_stream, H0_stream, branches, preprocessor, training_split = 0.5, chunks = 1000, as_matrix = True):
        self.H1_stream = H1_stream
        self.H0_stream = H0_stream

        self.branches = branches
        self.preprocessor = preprocessor

        self.training_split = training_split
        self.chunks = chunks
        self.as_matrix = as_matrix

        self.H1_collection = None
        self.H0_collection = None

        def setup_training_data(self):
            self.H1_collection = FileCollection(self.H1_stream, start_fraction = 0.0, end_fraction = self.training_split)
            self.H0_collection = FileCollection(self.H0_stream, start_fraction = 0.0, end_fraction = self.training_split)
            return self.H1_collection.get_length() + self.H0_collection.get_length()
            #return self._datagen(H1_collection, H0_collection, self.branches, chunks = self.chunks, preprocessor = self.preprocessor, as_matrix = self.as_matrix)
        
        def setup_validation_data(self):
            self.H1_collection = FileCollection(self.H1_stream, start_fraction = self.training_split, end_fraction = 1.0)
            self.H0_collection = FileCollection(self.H0_stream, start_fraction = self.training_split, end_fraction = 1.0)
            return self.H1_collection.get_length() + self.H0_collection.get_length()
            #return self._datagen(H1_collection, H0_collection, self.branches, chunks = self.chunks, preprocessor = self.preprocessor, as_matrix = self.as_matrix)
        
        def generator(self):
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
        
                # update the starting position for the next chunk
                H1_curpos += H1_chunksize
                H0_curpos += H0_chunksize

                # run the preprocessing: this will in general change the number of rows (if there are cuts hidden inside the preprocessor) AND the number of columns (if not all loaded columns are meant as input to the classifier)
                if self.preprocessor is not None:
                    H1_data = self.preprocessor(H1_data)
                    H0_data = self.preprocessor(H0_data)

                # add the truth information
                H1_data["target"] = 1.0
                H0_data["target"] = 0.0

                data_chunk = pd.concat([H1_data, H0_data])

                # return a randomized signal + background sample
                training_data = data_chunk.sample(frac = 1)

                # the target data for the training is the one that has just been added...
                target_data = training_data["target"]
                if self.as_matrix:
                    target_data = target_data.as_matrix()
    
                # ... and the input data for the training is everything else
                input_data = training_data.loc[:, training_data.columns != "target"]
                if self.as_matrix:
                    input_data = input_data.as_matrix()

                yield input_data, target_data

