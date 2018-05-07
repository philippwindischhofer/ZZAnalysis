#!/bin/bash

# ---------------------------------------------
#  global settings
# ---------------------------------------------
CURRENT_DIR=`pwd`
CAMPAIGN_DIR=$1

JOB_SUBMITTER="/opt/exp_soft/cms/t3/t3submit_new"

# the directories where the original sources are located
BIN_DIR_ORIGINAL="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/bin/slc6_amd64_gcc630/"

TRAINING_FILES_BKG="ggTo2e2mu_Contin_MCFM701 ggTo2e2tau_Contin_MCFM701 ggTo2mu2tau_Contin_MCFM701 ggTo4e_Contin_MCFM701 ggTo4mu_Contin_MCFM701 ggTo4tau_Contin_MCFM701 ZZTo4l AllData"
ROOT_FILE_NAME="/ZZ4lAnalysis.root"

# the (common) source directory for this campaign
BIN_DIR=$CAMPAIGN_DIR"bin/"

# the needed part from the C++ sources
CALIBRATOR="run_calibrator"

# ---------------------------------------------
#  first, copy all the needed executables to the campaign folder
# ---------------------------------------------
mkdir -p $BIN_DIR
cp $BIN_DIR_ORIGINAL$CALIBRATOR $BIN_DIR

cd $CAMPAIGN_DIR

RUN_DIRLIST=`ls -d */ | egrep -v 'bin|statistics'`

for RUN in $RUN_DIRLIST
do

    AUGMENTATION_TRAINING_DIR=$CAMPAIGN_DIR$RUN"augmentation_training/"
    AUGMENTATION_VALIDATION_DIR=$CAMPAIGN_DIR$RUN"augmentation_validation/"
    AUGMENTATION_TEST_DIR=$CAMPAIGN_DIR$RUN"augmentation_test/"

    # before can start the calibration, also need to prepare the combined *augmented* training, validation and test dataset for the background(s)
    # BKG_TRAINING_SRC=""
    # for file in $TRAINING_FILES_BKG
    # do
    # 	BKG_TRAINING_SRC="${BKG_TRAINING_SRC} ${AUGMENTATION_TRAINING_DIR}${file}${ROOT_FILE_NAME}"
    # done

    # BKG_COMBINED_TRAINING=$AUGMENTATION_TRAINING_DIR"/bkg/"
    # mkdir -p $BKG_COMBINED_TRAINING
    # hadd $BKG_COMBINED_TRAINING$ROOT_FILE_NAME $BKG_TRAINING_SRC

    BKG_VALIDATION_SRC=""
    for file in $TRAINING_FILES_BKG
    do
	BKG_VALIDATION_SRC="${BKG_VALIDATION_SRC} ${AUGMENTATION_VALIDATION_DIR}${file}${ROOT_FILE_NAME}"
    done

    BKG_COMBINED_VALIDATION=$AUGMENTATION_VALIDATION_DIR"/bkg/"
    mkdir -p $BKG_COMBINED_VALIDATION
    hadd $BKG_COMBINED_VALIDATION$ROOT_FILE_NAME $BKG_VALIDATION_SRC

    BKG_TEST_SRC=""
    for file in $TRAINING_FILES_BKG
    do
	BKG_TEST_SRC="${BKG_TEST_SRC} ${AUGMENTATION_TEST_DIR}${file}${ROOT_FILE_NAME}"
    done

    BKG_COMBINED_TEST=$AUGMENTATION_TEST_DIR"/bkg/"
    mkdir -p $BKG_COMBINED_TEST
    hadd $BKG_COMBINED_TEST$ROOT_FILE_NAME $BKG_TEST_SRC

    CALIBRATION_SETTINGS_DIR=$CAMPAIGN_DIR$RUN"settings_calibration/"
    CALIBRATION_TRAINING_DIR=$CAMPAIGN_DIR$RUN"calibration_training/"
    CALIBRATION_VALIDATION_DIR=$CAMPAIGN_DIR$RUN"calibration_validation/"

    CALIBRATION_TEST_DIR=$CAMPAIGN_DIR$RUN"calibration_test/"

    mkdir -p $CALIBRATION_SETTINGS_DIR
    mkdir -p $CALIBRATION_TRAINING_DIR
    mkdir -p $CALIBRATION_VALIDATION_DIR
    mkdir -p $CALIBRATION_TEST_DIR

    CALIBRATION_TRAINING_SCRIPT=$CALIBRATION_SETTINGS_DIR"run_calibration_training.sh"
    CALIBRATION_TRAINING_LOGFILE=$CALIBRATION_SETTINGS_DIR"log_calibration_training.txt"

    CALIBRATION_VALIDATION_SCRIPT=$CALIBRATION_SETTINGS_DIR"run_calibration_validation.sh"
    CALIBRATION_VALIDATION_LOGFILE=$CALIBRATION_SETTINGS_DIR"log_calibration_validation.txt"

    CALIBRATION_TEST_SCRIPT=$CALIBRATION_SETTINGS_DIR"run_calibration_test.sh"
    CALIBRATION_TEST_LOGFILE=$CALIBRATION_SETTINGS_DIR"log_calibration_test.txt"

    #echo "#!/bin/bash" > $CALIBRATION_TRAINING_SCRIPT
    #echo $BIN_DIR$CALIBRATOR $AUGMENTATION_TRAINING_DIR "ML" $CALIBRATION_TRAINING_DIR "0.0" "1.0" "&>" $CALIBRATION_TRAINING_LOGFILE >> $CALIBRATION_TRAINING_SCRIPT

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