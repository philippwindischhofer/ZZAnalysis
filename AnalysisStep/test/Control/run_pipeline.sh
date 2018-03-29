#!/bin/bash

# ---------------------------------------------
#  global settings
# ---------------------------------------------
PYTHON_DIR="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/AnalysisStep/test/Python/"
TRAINER="SampleTraining.py"
AUGMENTER="SampleAugmentation.py"

BIN_DIR="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/bin/slc6_amd64_gcc630/"
CALIBRATOR="./run_calibrator"
BENCHMARKER="./run_benchmarker"
ROC_PLOTTER="./run_roc_plotter"
COMPARER="./run_comp"

# directory where all the outputs will be stored
#OUT_DIR="/data_CMS/cms/wind/"

# ---------------------------------------------
#  settings for this training campaign
# ---------------------------------------------

#CAMPAIGN_NAME="test_training_config_file"
#CAMPAIGN_NAME=$1
MC_DIR="/data_CMS/cms/wind/CJLST_NTuples/"
COMP_REF_DIR="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/BenchmarkerPlotsReferenceReducedCategorySet/"

# ---------------------------------------------

#$OUT_DIR$CAMPAIGN_NAME
CAMPAIGN_DIR=$1
SETTINGS_DIR=$CAMPAIGN_DIR"/"
TRAINING_DIR=${CAMPAIGN_DIR}"/training/"
AUGMENTATION_DIR=${CAMPAIGN_DIR}"/augmentation/"
CALIBRATION_DIR=${CAMPAIGN_DIR}"/calibration/"
ROC_DIR=${CAMPAIGN_DIR}"/ROCs/"
BENCHMARK_DIR=${CAMPAIGN_DIR}"/benchmark/"
COMP_DIR=${CAMPAIGN_DIR}"/comp/"

#rm -r $CAMPAIGN_DIR
#mkdir $CAMPAIGN_DIR

mkdir $TRAINING_DIR
mkdir $AUGMENTATION_DIR
mkdir $CALIBRATION_DIR
mkdir $ROC_DIR
mkdir $BENCHMARK_DIR
mkdir $COMP_DIR

# launch the training
python $PYTHON_DIR$TRAINER $SETTINGS_DIR $TRAINING_DIR

# launch the augmentation
#python $PYTHON_DIR$AUGMENTER $MC_DIR $SETTINGS_DIR $TRAINING_DIR $AUGMENTATION_DIR

# launch the calibration
#$BIN_DIR$CALIBRATOR $AUGMENTATION_DIR "ML" $CALIBRATION_DIR

# launch the benchmarking
#$BIN_DIR$BENCHMARKER $CALIBRATION_DIR $AUGMENTATION_DIR $BENCHMARK_DIR

# launch the comparison to the reference
#$BIN_DIR$COMPARER $BENCHMARK_DIR $COMP_REF_DIR $COMP_DIR

# launch the plotting of ROCs
#$BIN_DIR$ROC_PLOTTER $AUGMENTATION_DIR $ROC_DIR

# delete again the large augmented ROOT files (can always be regenerated, if necessary)
#echo "removing "$AUGMENTATION_DIR
#rm -r $AUGMENTATION_DIR