#!/bin/bash

# ---------------------------------------------
#  global settings
# ---------------------------------------------
CURRENT_DIR=`pwd`
CAMPAIGN_DIR=$1

JOB_SUBMITTER="/opt/exp_soft/cms/t3/t3submit_new"

# the directories where the original sources are located
PYTHON_DIR_ORIGINAL="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/AnalysisStep/test/Python/"
CONTROL_DIR_ORIGINAL="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/AnalysisStep/test/Control/"
BIN_DIR_ORIGINAL="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/bin/slc6_amd64_gcc630/"

# the (common) source directory for this campaign
BIN_DIR=$CAMPAIGN_DIR"bin/"

# the needed part from the python sources
CONFIG_FILE_GEN="ConfigFileSweeper.py"
TRAINER="SampleTraining.py"
AUGMENTER="SampleAugmentation.py"
PYTHON_LIB="trainlib"

# the needed part from the C++ sources
CALIBRATOR="run_calibrator"
BENCHMARKER="run_benchmarker"
ROC_PLOTTER="run_roc_plotter"
COMPARER="run_comp"

# the needed part from the control sources
RUN_PIPELINE="run_pipeline.sh"
PLOT_POSTPROCESSOR="plot_postprocessing.sh"

# ---------------------------------------------
#  first, copy all the executables to the campaign folder
# ---------------------------------------------
echo "preparing filesystem for training campaign"

mkdir $BIN_DIR
cp -r $PYTHON_DIR_ORIGINAL$PYTHON_LIB $BIN_DIR
cp $PYTHON_DIR_ORIGINAL$TRAINER $PYTHON_DIR_ORIGINAL$AUGMENTER $PYTHON_DIR_ORIGINAL$CONFIG_FILE_GEN $BIN_DIR
cp $BIN_DIR_ORIGINAL$CALIBRATOR $BIN_DIR_ORIGINAL$BENCHMARKER $BIN_DIR_ORIGINAL$ROC_PLOTTER $BIN_DIR_ORIGINAL$COMPARER $BIN_DIR
cp $CONTROL_DIR_ORIGINAL$RUN_PIPELINE $BIN_DIR
cp $CONTROL_DIR_ORIGINAL$PLOT_POSTPROCESSOR $BIN_DIR

# then, run the generation of the sub-config files
python $BIN_DIR$CONFIG_FILE_GEN $CAMPAIGN_DIR

# now should have plenty of prepared directories on which to run
cd $CAMPAIGN_DIR

# go through all directories, except the one containing the executables, and the one for the statistics output
DIRLIST=`ls -d */ | egrep -v 'bin|statistics'`

for DIR in $DIRLIST
do
    NEXT_LAUNCH=$CAMPAIGN_DIR${DIR%?}
    echo "launching pipeline on" $NEXT_LAUNCH
    
    LOGFILE=$NEXT_LAUNCH"/log.txt"
    JOB_SCRIPT=$NEXT_LAUNCH"/run_job.sh"

    echo "#!/bin/bash" > $JOB_SCRIPT
    echo $BIN_DIR$RUN_PIPELINE $NEXT_LAUNCH "&>>" $LOGFILE >> $JOB_SCRIPT

    #$JOB_SUBMITTER $JOB_SCRIPT
done

# wait for all jobs to complete
echo "launched all jobs"

# return to the original directory once done
cd $CURRENT_DIR