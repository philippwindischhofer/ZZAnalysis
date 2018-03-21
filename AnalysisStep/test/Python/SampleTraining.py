print "starting imports..."

import tensorflow as tf
from keras import backend as K
from keras import optimizers
from trainlib.SimpleModel import SimpleModel
from trainlib.ModelCollection import ModelCollection
from trainlib.Trainer import Trainer
from trainlib.SimplePreprocessor import SimplePreprocessor
from trainlib.ModelFactory import ModelFactory

print "imports done"

# configuring Tensorflow as Keras backend
config = tf.ConfigProto(intra_op_parallelism_threads = 10, inter_op_parallelism_threads = 10, allow_soft_placement = True, device_count = {'CPU': 10})
session = tf.Session(config = config)
K.set_session(session)

# #input training files for training this discriminant
# inpath = "/data_CMS/cms/wind/CJLST_NTuples/"
# filename = "/ZZ4lAnalysis.root"
# H1_files = ["VBFH125"]
# H0_files = ["ggH125"]

# H1_paths = [inpath + H1_file + filename for H1_file in H1_files]
# H0_paths = [inpath + H0_file + filename for H0_file in H0_files]

# # def no_preprocessor(frame):
# #     return frame.loc[frame["nCleanedJetsPt30"] >= 2]

# mod = SimpleModel("simplemodel")
# mod.build()

# coll = ModelCollection("simplecoll", H1_paths, H0_paths)
# pre = SimplePreprocessor()
# coll.add_model(pre, mod)

mcolls = ModelFactory.GenerateModelCollections(SimpleModel)

train = Trainer("/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/AnalysisStep/test/Python/training_area/")
sgd = optimizers.SGD(lr = 0.01)

for mcoll in mcolls:
    train.train(mcoll, optimizer = sgd)

