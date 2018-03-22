print "starting imports..."

import tensorflow as tf
from keras import backend as K
from keras import optimizers
from trainlib.SimpleModel import SimpleModel
from trainlib.ModelCollection import ModelCollection
from trainlib.Trainer import Trainer
from trainlib.SimplePreprocessor import SimplePreprocessor
from trainlib.ModelFactory import ModelFactory
import sys

print "imports done"

# configuring Tensorflow as Keras backend
config = tf.ConfigProto(intra_op_parallelism_threads = 10, inter_op_parallelism_threads = 10, allow_soft_placement = True, device_count = {'CPU': 10})
session = tf.Session(config = config)
K.set_session(session)

def main():
    
    if len(sys.argv) != 3:
        print "Error: exactly 2 arguments are required"

    #/data_CMS/cms/wind/CJLST_NTuples/
    MC_dir = sys.argv[1]
    training_dir = sys.argv[2]

    mcolls = ModelFactory.GenerateModelCollections(SimpleModel, MC_dir)

    train = Trainer(training_dir)
    sgd = optimizers.SGD(lr = 0.01, momentum = 0.9)

    for mcoll in mcolls:
        train.train(mcoll, optimizer = sgd)

if __name__ == "__main__":
    main()
