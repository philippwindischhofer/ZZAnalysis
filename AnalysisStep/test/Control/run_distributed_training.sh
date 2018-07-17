#!/bin/bash

if [[ -z "$ZZROOT" ]]; then
    echo "ERROR: need to have ZZROOT set! Did you forget to source 'prepare_env.sh'?"
    exit
fi

# ---------------------------------------------
#  parse the given arguments
# ---------------------------------------------
POSARG=()

while [[ $# -gt 0 ]]
do
key=$1

case $key in
    --hyperpar)
    HYPERPARAM_CONFIG_FILE="$2"
    shift
    shift
    ;;
    --input)
    INPUT_CONFIG_FILE="$2"
    shift
    shift
    ;;
    *)
    POSARG+=("$1")
    shift
    ;;
esac
done

# set back the positional arguments in case they will be needed later
set -- "${POSARG[@]}"

CAMPAIGN_DIR=$1

echo "input_config_file = "$INPUT_CONFIG_FILE
echo "hyperparam_config_file = "$HYPERPARAM_CONFIG_FILE

# ---------------------------------------------
#  global settings
# ---------------------------------------------
CURRENT_DIR=`pwd`
JOB_SUBMITTER="/opt/exp_soft/cms/t3/t3submit_new"

# the (common) source directory for this campaign
BIN_DIR=$ZZROOT"/src/ZZAnalysis/AnalysisStep/test/Python/"

# the needed part from the python sources
CONFIG_FILE_GEN="ConfigFileSweeper.py"
CONFIG_FILE_GEN_NOSWEEP="ConfigFileCreator.py"
TRAINING_CONFIG_FILE_GEN="DistributeTrainingSettings.py"
TRAINER="SampleTraining.py"

CAMPAIGN_CONFIG_FILE=$CAMPAIGN_DIR"/campaign.conf"

# ---------------------------------------------
#  first, copy all the executables to the campaign folder
# ---------------------------------------------
echo "preparing filesystem for training campaign"

# then, run the generation of the sub-config files
if [[ -z "$HYPERPARAM_CONFIG_FILE" ]]; then
    if [[ -e "$CAMPAIGN_CONFIG_FILE" ]]; then
        # no hyperparameter configs given -> use the campaign config file to sweep them
	python $BIN_DIR$CONFIG_FILE_GEN $CAMPAIGN_DIR $INPUT_CONFIG_FILE	
    else
	echo "ERROR: no hyperparameter configuration file given, and no file 'campaign.conf' in the campaign directory found as fallback! Please specify either the one or the other."
	exit
    fi
else
    # have fully specified hyperparameters; ignore any campaign config file that might presribe to sweep them
    python $BIN_DIR$CONFIG_FILE_GEN_NOSWEEP $CAMPAIGN_DIR $INPUT_CONFIG_FILE $HYPERPARAM_CONFIG_FILE
fi

cd $CAMPAIGN_DIR

# at this stage, have directories for each run in the sweep. go through all of them and prepare the config files for the individual trainings
RUN_DIRLIST=`ls -d */ | egrep -v 'bin|statistics'`

# for all the runs in this sweep
for RUN in $RUN_DIRLIST
do
    # create the directory that will hold the training output
    TRAINING_DIR=$CAMPAIGN_DIR$RUN"/training/"
    mkdir -p $TRAINING_DIR

    # for each run, first make all the config files for the individual trainings
    python $BIN_DIR$TRAINING_CONFIG_FILE_GEN $CAMPAIGN_DIR$RUN

    TRAINING_SETTINGS_DIR=$CAMPAIGN_DIR$RUN"/settings_training/"
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
    #$JOB_SUBMITTER $CAMPAIGN_DIR$JOB
done

cd $CURRENT_DIR