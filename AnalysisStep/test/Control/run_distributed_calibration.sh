#!/bin/bash

if [[ -z "$CMSSW_BASE" ]]; then
    echo "ERROR: need to have CMSSW_BASE set! Did you forget to run 'cmsenv'?"
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
    --workdir)
    MC_TRAINING_DIR="$2/"
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

# ---------------------------------------------
#  global settings
# ---------------------------------------------
CURRENT_DIR=`pwd`

CAMPAIGN_DIR="${1:-$CAMPAIGN_DIR}"
CAMPAIGN_DIR=$CAMPAIGN_DIR"/"

AUGMENTATION_SETTINGS=$MC_TRAINING_DIR"/settings.conf"

# the directories where the original sources are located
BIN_DIR=$CMSSW_BASE"/bin/slc6_amd64_gcc630/"
BUILDFILE_DIR=$CMSSW_BASE"/src/ZZAnalysis/AnalysisStep/bin/"
PYTHON_DIR=$CMSSW_BASE"/src/ZZAnalysis/AnalysisStep/test/Python/"

# the needed part from the C++ sources
CALIBRATOR="run_calibrator"
CALIBRATION_PREPARER="prepare_calibration.py"
HEADER_GENERATOR="CreateHeaderFiles.py"

# from the calibration step onwards, C++ takes over. The number and type of defined categories is visible from C++ in the branch names in the augmented trees that hold the evaluated neural networks.
# for the configuration found in categories.conf, make the list of branches and recompile the C++ codebase with them
python $PYTHON_DIR$HEADER_GENERATOR
cd $BUILDFILE_DIR
scram b clean
scram b

cd $CAMPAIGN_DIR

RUN_DIRLIST=`ls -d */ | egrep -v 'bin|statistics'`

for RUN in $RUN_DIRLIST
do

    AUGMENTATION_TRAINING_DIR=$CAMPAIGN_DIR$RUN"augmentation_training/"
    AUGMENTATION_VALIDATION_DIR=$CAMPAIGN_DIR$RUN"augmentation_validation/"
    AUGMENTATION_TEST_DIR=$CAMPAIGN_DIR$RUN"augmentation_test/"

    CALIBRATION_SETTINGS_DIR=$CAMPAIGN_DIR$RUN"settings_calibration/"
    CALIBRATION_TRAINING_DIR=$CAMPAIGN_DIR$RUN"calibration_training/"
    CALIBRATION_VALIDATION_DIR=$CAMPAIGN_DIR$RUN"calibration_validation/"

    CALIBRATION_TEST_DIR=$CAMPAIGN_DIR$RUN"calibration_test/"

    mkdir -p $CALIBRATION_SETTINGS_DIR
    mkdir -p $CALIBRATION_TRAINING_DIR
    mkdir -p $CALIBRATION_VALIDATION_DIR
    mkdir -p $CALIBRATION_TEST_DIR

    # ----------------

    if [ -d "$AUGMENTATION_VALIDATION_DIR" ]
    then
	echo "chunks already merged!"
    else
        # before can start the calibration, also need to prepare the combined *augmented* training, validation and test dataset for the background(s)
	python $PYTHON_DIR$CALIBRATION_PREPARER $AUGMENTATION_SETTINGS $CAMPAIGN_DIR$RUN
    fi
        
    # -----------------

    CALIBRATION_TRAINING_SCRIPT=$CALIBRATION_SETTINGS_DIR"run_calibration_training.sh"
    CALIBRATION_TRAINING_LOGFILE=$CALIBRATION_SETTINGS_DIR"log_calibration_training.txt"

    CALIBRATION_VALIDATION_SCRIPT=$CALIBRATION_SETTINGS_DIR"run_calibration_validation.sh"
    CALIBRATION_VALIDATION_LOGFILE=$CALIBRATION_SETTINGS_DIR"log_calibration_validation.txt"

    CALIBRATION_TEST_SCRIPT=$CALIBRATION_SETTINGS_DIR"run_calibration_test.sh"
    CALIBRATION_TEST_LOGFILE=$CALIBRATION_SETTINGS_DIR"log_calibration_test.txt"

    echo "#!/bin/bash" > $CALIBRATION_TRAINING_SCRIPT
    echo $BIN_DIR$CALIBRATOR $AUGMENTATION_TRAINING_DIR "ML" $CALIBRATION_TRAINING_DIR "0.0" "1.0" "&>" $CALIBRATION_TRAINING_LOGFILE >> $CALIBRATION_TRAINING_SCRIPT

    echo "#!/bin/bash" > $CALIBRATION_VALIDATION_SCRIPT
    echo $BIN_DIR$CALIBRATOR $AUGMENTATION_VALIDATION_DIR "ML" $CALIBRATION_VALIDATION_DIR "0.0" "1.0" "&>" $CALIBRATION_VALIDATION_LOGFILE >> $CALIBRATION_VALIDATION_SCRIPT

    echo "#!/bin/bash" > $CALIBRATION_TEST_SCRIPT
    echo $BIN_DIR$CALIBRATOR $AUGMENTATION_TEST_DIR "ML" $CALIBRATION_TEST_DIR "0.0" "1.0" "&>" $CALIBRATION_TEST_LOGFILE >> $CALIBRATION_TEST_SCRIPT
done

sleep 1

# all scripts prepared, going forward to launch them
cd $CAMPAIGN_DIR

JOBS=`find * | grep run_calibration_.*.sh$`

for JOB in $JOBS
do
    echo "launching calibration for " $CAMPAIGN_DIR$JOB
    $JOB_SUBMITTER "-short" $CAMPAIGN_DIR$JOB
done

cd $CURRENT_DIR