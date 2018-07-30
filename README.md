Bayes4Leptons
=============

This is a modified version of the original [CJLST/ZZAnalysis](https://github.com/CJLST/ZZAnalysis), using Bayesian event categorization instead.

The comments below are for the installation of the original CMSSW environment. For instructions of how to use Bayesian classification, see the [wiki](https://github.com/philippwindischhofer/ZZAnalysis/wiki) instead.

------------------------------

To install the necessary environment from scratch:
------------------------------
mkdir cmssw
cd cmssw
scram p CMSSW CMSSW_9_4_2

wget -O ${TMPDIR}/checkout_9X_bayes.csh https://raw.githubusercontent.com/philippwindischhofer/ZZAnalysis/experimental/checkout_9X_bayes.csh 
cd $CMSSW_BASE/src
cmsenv
chmod u+x ${TMPDIR}/checkout_9X_bayes.csh
${TMPDIR}/checkout_9X_bayes.csh 

cd ZZAnalysis/AnalysisStep/bin
scram b
