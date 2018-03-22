print "starting imports..."

import tensorflow as tf
from keras import backend as K
from trainlib.SimpleModel import SimpleModel
from trainlib.ModelCollection import ModelCollection
from trainlib.ModelFactory import ModelFactory
from trainlib.FileCollection import FileCollection
from trainlib.Trainer import Trainer
from trainlib.config import Config
import trainlib.utils as utils
import trainlib.cuts as cuts
import root_numpy
import numpy as np
from rootpy.io import root_open
from shutil import copyfile
import os

print "imports done"

# configuring Tensorflow as Keras backend
config = tf.ConfigProto(intra_op_parallelism_threads=10, inter_op_parallelism_threads=10, allow_soft_placement=True, device_count = {'CPU': 10})
session = tf.Session(config = config)
K.set_session(session)

def augment_file(data_inpath, data_outpath, data_file, mcolls):
    # first, copy the original ROOT file to its destination, keeping the directory structure the same
    data_outdir = data_outpath + data_file
    if not os.path.exists(data_outdir):
        os.makedirs(data_outdir)

    data_outfile = data_outdir + Config.MC_filename
    copyfile(data_inpath + data_file + Config.MC_filename, data_outfile)

    # now, can read the file from its new location and change it
    fcoll = FileCollection({data_outfile: cuts.no_cut}, 0.0, 1.0)
    length = fcoll.get_length()

    indata = utils.read_data(fcoll, start = 0, stop = length, branches = Config.branches)

    # loop over ModelCollections here to get the prediction from each
    out_branches = []
    prepared_dtype = []
    branch_names = []

    for mcoll in mcolls:
        out_branches.append(mcoll.predict(indata))
        branch_names.append(mcoll.name)
        prepared_dtype.append((mcoll.name, 'f4'))

    # make it into the correct type and shape
    new_branches = np.array(np.zeros(length), dtype = prepared_dtype)

    for out_data, branch_name in zip(out_branches, branch_names):
        new_branches[branch_name] = out_data

    # now re-open the output file in append mode
    outfile = root_open(data_outfile, mode = "a");
    outtree = outfile.Get("ZZTree/candTree")

    root_numpy.array2tree(new_branches, tree = outtree)
    outfile.write()
    outfile.close()


def main():
    # --------------------------------------------------------------

    # # later, the construction of the ModelCollection etc. is all going to be handled by a factory (which will look up the settings in some dictionary, based on the passed name of the discriminant)
    # def trivial_preprocessor(frame):
    #     return frame

    # # input model file for augmenting the trained discriminant to the tree
    # disc_inpath = "/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/AnalysisStep/test/Python/training_logs/"
    # disc_name = "simplecoll"


    # mcoll1 = ModelCollection("simplecoll1")
    # mcoll2 = ModelCollection("simplecoll2")
    # mcolls = []

    # mod1 = SimpleModel("simplemodel")
    # mod1.build()
    # mcoll1.add_model(trivial_preprocessor, mod1)
    # mcoll1.load_weights(disc_inpath + "simplecoll1")
    # mcolls.append(mcoll1)

    # mod2 = SimpleModel("simplemodel")
    # mod2.build()
    # mcoll2.add_model(trivial_preprocessor, mod2)
    # mcoll2.load_weights(disc_inpath + "simplecoll2")
    # mcolls.append(mcoll2)

    # # ------------------------------------------------------------

    data_inpath = "/data_CMS/cms/wind/CJLST_NTuples/"
    data_outpath = "/data_CMS/cms/wind/processed/"

    # files to which this discriminant should be augmented
    data_files = ["ggH125", "VBFH125", "ZH125", "WplusH125", "WminusH125"]

    mcolls = ModelFactory.GenerateModelCollections(SimpleModel, weight_path = "/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/AnalysisStep/test/Python/training_area/")

    for data_file in data_files:
        augment_file(data_inpath, data_outpath, data_file, mcolls)

if __name__ == "__main__":
    main()
