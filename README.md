Bayes4Leptons
=============

This is a modified version of the original [CJLST/ZZAnalysis](https://github.com/CJLST/ZZAnalysis), using Bayesian event categorization instead.

See below for how to get the analysis environment installed. For instructions of how to train and use Bayesian classification, see the [wiki](https://github.com/philippwindischhofer/ZZAnalysis/wiki) instead.

------------------------------

To install the necessary environment from scratch:
------------------------------
```
mkdir cmssw
cd cmssw
scram p CMSSW CMSSW_9_4_2

wget -O ${TMPDIR}/checkout_9X_bayes.csh https://raw.githubusercontent.com/philippwindischhofer/ZZAnalysis/experimental/checkout_9X_bayes.csh 
cd $CMSSW_BASE/src
cmsenv
<<<<<<< HEAD
chmod u+x ${TMPDIR}/checkout_9X_bayes.csh
${TMPDIR}/checkout_9X_bayes.csh 
=======
chmod u+x ${TMPDIR}/checkout_9X.csh
${TMPDIR}/checkout_9X.csh
```

To install a complete CMSSW 10X area (including this package)
------------------------------
Please use CMSSW_10_1_6.

Download and execute the setup script:
```
wget -O ${TMPDIR}/checkout_10X.csh https://raw.githubusercontent.com/CJLST/ZZAnalysis/miniAOD_80X/checkout_10X.csh
cd $CMSSW_BASE/src
cmsenv
chmod u+x ${TMPDIR}/checkout_10X.csh
${TMPDIR}/checkout_10X.csh
```

To update this package from the release
------------------------------------------
In the package directory, simply issue
```
git pull
```

To commit and push new changes
------------------------------
To commit directly (you need write access to the repository):
```
git pull
[edit files]
```
Once you are ready to commit
```
git pull
git add [files to be added]
git commit -m ["commit message"] [files to be added]
git push origin miniAOD_80X
```
>>>>>>> miniAOD_80X

cd ZZAnalysis/AnalysisStep/bin
scram b

pip install --user -r ./ZZAnalysis/AnalysisStep/test/Python/requirements.txt
```