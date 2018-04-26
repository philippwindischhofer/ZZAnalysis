#!/bin/bash

# ---------------------------------------------
#  global settings
# ---------------------------------------------
CURRENT_DIR=`pwd`
CAMPAIGN_DIR=$1
INPUT_CONFIG_FILE=$2

JOB_SUBMITTER="/opt/exp_soft/cms/t3/t3submit_new"

# the directories where the original sources are located
PYTHON_DIR_ORIGINAL="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/AnalysisStep/test/Python/"

# the (common) source directory for this campaign
BIN_DIR=$CAMPAIGN_DIR"bin/"

# the needed part from the python sources
CONFIG_FILE_GEN="ConfigFileSweeper.py"
TRAINING_CONFIG_FILE_GEN="DistributeTrainingSettings.py"
TRAINER="SampleTraining.py"
PYTHON_LIB="trainlib"

# ---------------------------------------------
#  first, copy all the executables to the campaign folder
# ---------------------------------------------
echo "preparing filesystem for training campaign"

mkdir -p $BIN_DIR
cp -r $PYTHON_DIR_ORIGINAL$PYTHON_LIB $BIN_DIR
cp $PYTHON_DIR_ORIGINAL$TRAINER $PYTHON_DIR_ORIGINAL$CONFIG_FILE_GEN $PYTHON_DIR_ORIGINAL$TRAINING_CONFIG_FILE_GEN $BIN_DIR

# then, run the generation of the sub-config files
python $BIN_DIR$CONFIG_FILE_GEN $CAMPAIGN_DIR $INPUT_CONFIG_FILE

cd $CAMPAIGN_DIR

# at this stage, have directories for each run in the sweep. go through all of them and prepare the config files for the individual trainings
RUN_DIRLIST=`ls -d */ | egrep -v 'bin|statistics'`

# for all the runs in this sweep
for RUN in $RUN_DIRLIST
do
    # create the directory that will hold the training output
    TRAINING_DIR=$CAMPAIGN_DIR$RUN"training/"
    mkdir -p $TRAINING_DIR

    # for each run, first make all the config files for the individual trainings
    python $BIN_DIR$TRAINING_CONFIG_FILE_GEN $CAMPAIGN_DIR$RUN

    TRAINING_SETTINGS_DIR=$CAMPAIGN_DIR$RUN"settings_training/"
    cd $TRAINING_SETTINGS_DIR

    TRAININGS=`ls -d */`

    for TRAINING in $TRAININGS
    do
	# for each training, generate the appropriate shell script
	TRAINING_SCRIPT=$TRAINING_SETTINGS_DIR$TRAINING"run_training.sh"
	TRAINING_LOGFILE=$TRAINING_SETTINGS_DIR$TRAINING"log.txt"
	echo "#!/bin/bash" > $TRAINING_SCRIPT
	echo "python" $BIN_DIR$TRAINER $TRAINING_SETTINGS_DIR$TRAINING $TRAINING_DIR "&>" $TRAINING_LOGFILE >> $TRAINING_SCRIPT
    done
done

sleep 1

# now go back and launch all the jobs that have been prepared
cd $CAMPAIGN_DIR

JOBS=`find * | grep run_training.sh$`

for JOB in $JOBS
do
    echo "launching training for " $CAMPAIGN_DIR$JOB
    $JOB_SUBMITTER $CAMPAIGN_DIR$JOB
done

cd $CURRENT_DIR