print "starting imports..."

import tensorflow as tf
from keras import backend as K
from trainlib.SimpleModel import SimpleModel
from trainlib.ModelCollection import ModelCollection
from trainlib.FileCollection import FileCollection
from trainlib.Trainer import Trainer
from trainlib.ModelCollectionConfigFileHandler import ModelCollectionConfigFileHandler
from trainlib.config import Config
import trainlib.utils as utils
import trainlib.cuts as cuts
import root_numpy
import numpy as np
from rootpy.io import root_open
from shutil import copyfile
import os
import sys

print "imports done"

# configuring Tensorflow as Keras backend
config = tf.ConfigProto(intra_op_parallelism_threads=10, inter_op_parallelism_threads=10, allow_soft_placement=True, device_count = {'CPU': 10})
session = tf.Session(config = config)
K.set_session(session)

#def augment_file(data_inpath, data_outpath, data_file, mcolls):
def augment_file(in_folder, out_folder, tree_name, mcolls):
    # first, copy the original ROOT file to its destination, keeping the directory structure the same
    #data_outdir = data_outpath + data_file
    #if not os.path.exists(data_outdir):
    #    os.makedirs(data_outdir)
    if not os.path.exists(out_folder):
        os.makedirs(out_folder)

    data_outfile = os.path.join(out_folder, Config.MC_filename)
    data_infile = os.path.join(in_folder, Config.MC_filename)

    copyfile(data_infile, data_outfile)

    #tree_name = "ClassTree"

    # now, can read the file from its new location and change it
    fcoll = FileCollection({data_outfile: cuts.no_cut}, 0.0, 1.0, tree_name = tree_name)
    length = fcoll.get_length()

    indata = utils.read_data(fcoll, start = 0, stop = length, branches = Config.branches, tree_name = tree_name)

    # loop over ModelCollections here to get the prediction from each
    out_branches = []
    prepared_dtype = []
    branch_names = []

    for mcoll in mcolls:
        out_branches.append(mcoll.predict(indata))
        branch_names.append(mcoll.name)
        prepared_dtype.append((mcoll.name.encode("ascii"), 'f4'))

    print prepared_dtype

    # make it into the correct type and shape
    new_branches = np.array(np.zeros(length), dtype = prepared_dtype)

    for out_data, branch_name in zip(out_branches, branch_names):
        new_branches[branch_name] = out_data

    # now re-open the output file in append mode
    outfile = root_open(data_outfile, mode = "a");
    outtree = outfile.Get(tree_name + "/candTree")

    root_numpy.array2tree(new_branches, tree = outtree)
    outfile.write()
    outfile.close()


def main():

    if len(sys.argv) != 5:
        print "Error: exactly 4 arguments are required"

    in_folder = sys.argv[1]
    out_folder = sys.argv[2]
    tree_name = sys.argv[3]
    run_dir = sys.argv[4]

    confhandler = ModelCollectionConfigFileHandler()
    confhandler.load_configuration(os.path.join(run_dir, "settings.conf"))
    mcolls = confhandler.GetModelCollection(weightpath = os.path.join(run_dir, "training/"))

    augment_file(in_folder, out_folder, tree_name, mcolls)

    # MC_dir = sys.argv[1]
    # data_file = sys.argv[2]
    # setting_dir = sys.argv[3]
    # training_path = sys.argv[4]
    # data_outpath = sys.argv[5]

    # confhandler = ModelCollectionConfigFileHandler()
    # confhandler.load_configuration(setting_dir + "settings.conf")
    # mcolls = confhandler.GetModelCollection(weightpath = training_path)

    # augment_file(MC_dir, data_outpath, data_file, mcolls)

if __name__ == "__main__":
    main()
