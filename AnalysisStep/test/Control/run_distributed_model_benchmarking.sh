#!/bin/bash

CURRENT_DIR=`pwd`
CAMPAIGN_DIR=$1

JOB_SUBMITTER="/opt/exp_soft/cms/t3/t3submit_new"

# the directories where the original sources are located
PYTHON_DIR_ORIGINAL="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/AnalysisStep/test/Python/"

# the (common) source directory for this campaign
BIN_DIR=$CAMPAIGN_DIR"bin/"

# the needed part from the python sources
MODEL_BENCHMARKER="ModelCollectionEvaluator.py"

mkdir -p $BIN_DIR
cp -r $PYTHON_DIR_ORIGINAL$PYTHON_LIB $BIN_DIR
cp $PYTHON_DIR_ORIGINAL$MODEL_BENCHMARKER $BIN_DIR

cd $CAMPAIGN_DIR

RUN_DIRLIST=`ls -d */ | egrep -v 'bin|statistics'`

# for all the runs in this sweep
for RUN in $RUN_DIRLIST
do
    SETTINGS_DIR=$CAMPAIGN_DIR$RUN
    TRAINING_DIR=$CAMPAIGN_DIR$RUN"training/"
    OUT_DIR=$CAMPAIGN_DIR$RUN

    MODEL_BENCHMARKING_SETTINGS_DIR=$CAMPAIGN_DIR$RUN"settings_model_benchmark/"
    MODEL_BENCHMARKING_SCRIPT=$MODEL_BENCHMARKING_SETTINGS_DIR"run_model_benchmark.sh"
    MODEL_BENCHMARKING_LOGFILE=$MODEL_BENCHMARKING_SETTINGS_DIR"log.txt"
    mkdir -p $MODEL_BENCHMARKING_SETTINGS_DIR

    echo "#!/bin/bash" > $MODEL_BENCHMARKING_SCRIPT
    echo "python" $BIN_DIR$MODEL_BENCHMARKER $SETTINGS_DIR $TRAINING_DIR $OUT_DIR "&>" $MODEL_BENCHMARKING_LOGFILE >> $MODEL_BENCHMARKING_SCRIPT
done

sleep 1

cd $CAMPAIGN_DIR

JOBS=`find * | grep run_model_benchmark.sh$`

for JOB in $JOBS
do
    echo "launching model benchmark for " $CAMPAIGN_DIR$JOB
    $JOB_SUBMITTER "-short" $CAMPAIGN_DIR$JOB
done

cd $CURRENT_DIR

