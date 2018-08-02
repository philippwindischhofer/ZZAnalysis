print "starting imports..."

from trainlib.ModelCollection import ModelCollection
from trainlib.ModelCollectionConfigFileHandler import ModelCollectionConfigFileHandler
import tensorflow as tf
from keras import backend as K
from keras import optimizers
from trainlib.Trainer import Trainer
import sys

# configuring Tensorflow as Keras backend
config = tf.ConfigProto(intra_op_parallelism_threads = 10, inter_op_parallelism_threads = 10, allow_soft_placement = True, device_count = {'CPU': 10})
session = tf.Session(config = config)
K.set_session(session)

print "imports done"

def main():
    
    if len(sys.argv) != 3:
        print "Error: exactly 2 arguments are required"

    setting_dir = sys.argv[1]
    training_dir = sys.argv[2]

    confhandler = ModelCollectionConfigFileHandler()
    confhandler.load_configuration(setting_dir + "settings.conf")
    mcolls = confhandler.GetModelCollection()

    train = Trainer(training_dir)
    #opt = optimizers.SGD(lr = 0.01, momentum = 0.95, decay = 1e-6)
    opt = optimizers.Adam(lr = 0.001, beta_1 = 0.9, beta_2 = 0.999, epsilon = K.epsilon(), decay = 0.0)
    #opt = optimizers.RMSprop(lr = 0.001, rho = 0.9, epsilon = K.epsilon(), decay = 0.0)

    for mcoll in mcolls:
        train.train(mcoll, optimizer = opt, MC_weighting = False)

if __name__ == "__main__":
    main()
