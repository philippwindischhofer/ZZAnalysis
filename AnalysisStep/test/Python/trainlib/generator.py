from FileCollection import FileCollection
from utils import read_data
import pandas as pd

def generate_training_data(H1_files, H0_files, branches, preprocessor, training_split = 0.5, chunks = 1000, as_matrix = True):
    H1_collection = FileCollection(H1_files, start_fraction = 0.0, end_fraction = training_split)
    H0_collection = FileCollection(H0_files, start_fraction = 0.0, end_fraction = training_split)
    return datagen(H1_collection, H0_collection, branches, chunks = chunks, preprocessor = preprocessor, as_matrix = as_matrix)


def generate_validation_data(H1_files, H0_files, branches, preprocessor, training_split = 0.5, chunks = 1000, as_matrix = True):
    H1_collection = FileCollection(H1_files, start_fraction = training_split, end_fraction = 1.0)
    H0_collection = FileCollection(H0_files, start_fraction = training_split, end_fraction = 1.0)
    return datagen(H1_collection, H0_collection, branches, chunks = chunks, preprocessor = preprocessor, as_matrix = as_matrix)


def datagen(H1_collection, H0_collection, branches, preprocessor, chunks = 100, as_matrix = True):
    H1_curpos = 0
    H0_curpos = 0

    H1_maxpos = H1_collection.get_length()
    H0_maxpos = H0_collection.get_length()

    print "H1 contains " + str(H1_maxpos) + " entries"
    print "H0 contains " + str(H0_maxpos) + " entries"

    H1_chunksize = int(H1_maxpos / chunks)
    H0_chunksize = int(H0_maxpos / chunks)
    
    print "using the following chunk sizes: " + "(" + str(H1_chunksize) + " / " + str(H0_chunksize) + ")"
    
    # as a generator, deliver data forever
    while True:
               
        if H1_curpos + H1_chunksize > H1_maxpos:
            H1_curpos = 0
            
        if H0_curpos + H0_chunksize > H0_maxpos:
            H0_curpos = 0
        
        # prepare next training data chunk set by drawing events randomly from the two files
        H1_data = read_data(H1_collection, branches = branches, start = H1_curpos, stop = H1_curpos + H1_chunksize)
        H0_data = read_data(H0_collection, branches = branches, start = H0_curpos, stop = H0_curpos + H0_chunksize)
                        
        # update the starting position for the next chunk
        H1_curpos += H1_chunksize
        H0_curpos += H0_chunksize

        # run the preprocessing: this will in general change the number of rows (if there are cuts hidden inside the preprocessor) AND the number of columns (if not all loaded columns are meant as input to the classifier)
        if preprocessor is not None:
            H1_data = preprocessor(H1_data)
            H0_data = preprocessor(H0_data)

        # add the truth information
        H1_data["target"] = 1.0
        H0_data["target"] = 0.0

        data_chunk = pd.concat([H1_data, H0_data])

        # return a randomized signal + background sample
        training_data = data_chunk.sample(frac = 1)

        # the target data for the training is the one that has just been added...
        target_data = training_data["target"]
        if as_matrix:
            target_data = target_data.as_matrix()
    
        # ... and the input data for the training is everything else
        input_data = training_data.loc[:, training_data.columns != "target"]
        if as_matrix:
            input_data = input_data.as_matrix()

        yield input_data, target_data

