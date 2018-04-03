#!/bin/bash

# ---------------------------------------------
#  settings for this training campaign
# ---------------------------------------------

MC_DIR="/data_CMS/cms/wind/CJLST_NTuples/"
#COMP_REF_DIR="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/BenchmarkerPlotsReferenceReducedCategorySet/"
COMP_REF_DIR="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/BenchmarkerPlotsReferenceNoBackground/"
#COMP_REF_DIR="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/BenchmarkerPlotsReferenceForOptimization/"

# ---------------------------------------------

CAMPAIGN_DIR=$1
SETTINGS_DIR=$CAMPAIGN_DIR"/"
TRAINING_DIR=${CAMPAIGN_DIR}"/training/"
AUGMENTATION_DIR=${CAMPAIGN_DIR}"/augmentation/"
CALIBRATION_DIR=${CAMPAIGN_DIR}"/calibration/"
ROC_DIR=${CAMPAIGN_DIR}"/ROCs/"
BENCHMARK_DIR=${CAMPAIGN_DIR}"/benchmark/"
COMP_DIR=${CAMPAIGN_DIR}"/comp/"
BIN_DIR=${CAMPAIGN_DIR}"/../bin/"

# ---------------------------------------------
#  global settings
# ---------------------------------------------
TRAINER="SampleTraining.py"
AUGMENTER="SampleAugmentation.py"

CALIBRATOR="run_calibrator"
BENCHMARKER="run_benchmarker"
ROC_PLOTTER="run_roc_plotter"
COMPARER="run_comp"

PLOT_POSTPROCESSOR="plot_postprocessing.sh"

# create the folder structure that is needed for this step
mkdir $TRAINING_DIR
mkdir $AUGMENTATION_DIR
mkdir $CALIBRATION_DIR
mkdir $ROC_DIR
mkdir $BENCHMARK_DIR
mkdir $COMP_DIR

# put the plot postprocessors into the directories with many plots
cp $BIN_DIR$PLOT_POSTPROCESSOR $CALIBRATION_DIR
cp $BIN_DIR$PLOT_POSTPROCESSOR $ROC_DIR

# launch the training
python $BIN_DIR$TRAINER $SETTINGS_DIR $TRAINING_DIR

# launch the augmentation
python $BIN_DIR$AUGMENTER $MC_DIR $SETTINGS_DIR $TRAINING_DIR $AUGMENTATION_DIR

# launch the calibration
$BIN_DIR$CALIBRATOR $AUGMENTATION_DIR "ML" $CALIBRATION_DIR

# launch the benchmarking
$BIN_DIR$BENCHMARKER $CALIBRATION_DIR $AUGMENTATION_DIR $BENCHMARK_DIR

# launch the comparison to the reference
$BIN_DIR$COMPARER $BENCHMARK_DIR $COMP_REF_DIR $COMP_DIR

# launch the plotting of ROCs
$BIN_DIR$ROC_PLOTTER $AUGMENTATION_DIR $ROC_DIR

# run the plot postprocessing
cd $CALIBRATION_DIR
sh $PLOT_POSTPROCESSOR
cd $ROC_DIR
sh $PLOT_POSTPROCESSOR

# delete again the large augmented ROOT files (can always be regenerated, if necessary)
#echo "removing "$AUGMENTATION_DIR
#rm -r $AUGMENTATION_DIR