Bayes4Leptons
=============

This is a modified version of the original [CJLST/ZZAnalysis](https://github.com/CJLST/ZZAnalysis), using Bayesian event categorization instead.

See below for how to get the analysis environment installed. For instructions of how to train and use Bayesian classification, see the [wiki](https://github.com/philippwindischhofer/ZZAnalysis/wiki) instead.

------------------------------

To install the necessary environment from scratch:
------------------------------
```
# get a CMSSW environment
mkdir cmssw
cd cmssw
scram p CMSSW CMSSW_9_4_2

# run the checkout script
wget -O ${TMPDIR}/checkout_9X_bayes.csh https://raw.githubusercontent.com/philippwindischhofer/ZZAnalysis/experimental/checkout_9X_bayes.csh 
cd $CMSSW_BASE/src
cmsenv
chmod u+x ${TMPDIR}/checkout_9X_bayes.csh
${TMPDIR}/checkout_9X_bayes.csh 

# initial build
cd $CMSSW_BASE/src/ZZAnalysis/AnalysisStep/bin
scram b

# install Python requirements
cd $CMSSW_BASE/src/ZZAnalysis/AnalysisStep/test/Python/
pip install --user -r requirements.txt

```

To set up Tensorflow as default backend for Keras, create (or modify) the file `~/.keras/keras.json` with the following content
```
{
    "epsilon": 1e-07, 
    "floatx": "float32", 
    "image_data_format": "channels_last", 
    "backend": "tensorflow"
}
```