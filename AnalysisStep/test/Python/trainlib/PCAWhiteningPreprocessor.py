import pandas as pd
import numpy as np
from trainlib.Preprocessor import Preprocessor
from trainlib.config import Config
from sklearn.decomposition import PCA
import pickle

class PCAWhiteningPreprocessor(Preprocessor):
    
    # requires the name of the data columns that should be whitened in the end
    def __init__(self, processed_columns, cuts):
        self.processed_columns = processed_columns
        self.outcol_names = ["PCA_w_" + str(i) for i in range(len(self.processed_columns))]
        self.cuts = cuts
        n_inputs = len(processed_columns)
        self.pca = PCA(n_components = n_inputs, svd_solver = 'auto', whiten = True)

    def setup(self, datagen, len_setupdata):
        self.len_setupdata = len_setupdata

        # draw some data from the generator to find the eigenvectors
        extracted_data = []
        extracted_rows = 0

        # data gives a tuple [raw columns, target data], don't take the target information
        for data in datagen:
            extracted_data.append(data[0])
            extracted_rows += len(data[0])

            if extracted_rows > self.len_setupdata:
                break

        print "setting up PCA whitening on " + str(extracted_rows) + " events"

        input_data = pd.concat(extracted_data)

        input_data = self._rowcol_cut(input_data)

        # determine the PCA parameters (eigenvectors and -values) on this set of data
        input_data = input_data.as_matrix()
        self.pca.fit(input_data)

    def process(self, data):
        processed_data = self._rowcol_cut(data)
        processed_data = self.pca.transform(processed_data)

        # it is expected to return the processed data in form of a pandas dataframe again
        retval = pd.DataFrame(processed_data, columns = self.outcol_names)
        return retval

    def save(self, folder, filename):
        outfile = open(folder + filename, "wb")
        pickle.dump(self.pca, outfile)
        outfile.close()

    def load(self, folder, filename):
        infile = open(folder + filename, "rb")
        self.pca = pickle.load(infile)
        infile.close()

    def _rowcol_cut(self, data):
        # apply the row selection
        data = data.loc[data.apply(self.cuts, axis = 1)]

        # apply the column selection
        output_data = data.loc[:, self.processed_columns]
                
        return output_data
