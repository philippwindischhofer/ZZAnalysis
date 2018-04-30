#!/bin/bash

CURRENT_DIR=`pwd`

# this calibrates the discriminants that have already been augmented
BIN_DIR="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/bin/slc6_amd64_gcc630/"
CALIBRATOR="run_calibrator"

JOB_SUBMITTER="/opt/exp_soft/cms/t3/t3submit_new"

# need to pass the model again, since need access to its configuration
MODEL_ROOT=$1
AUGMENTATION_ROOT=$2

# the directory where the calibration files are going to be put
CALIBRATION_DIR=$AUGMENTATION_ROOT"calibration/"
mkdir -p $CALIBRATION_DIR

CALIBRATION_SCRIPT=$AUGMENTATION_ROOT"run_calibration.sh"
CALIBRATION_LOGFILE=$AUGMENTATION_ROOT"log_calibration.txt"

echo "#!/bin/bash" > $CALIBRATION_SCRIPT
echo $BIN_DIR$CALIBRATOR $AUGMENTATION_ROOT "ML" $CALIBRATION_DIR "0.0 1.0" "&>" $CALIBRATION_LOGFILE >> $CALIBRATION_SCRIPT

cd $AUGMENTATION_ROOT

JOBS=`find * | grep run_calibration.sh$`

for JOB in $JOBS
do
    echo "launching calibration for " $AUGMENTATION_ROOT$JOB
    #$JOB_SUBMITTER "-short" $AUGMENTATION_ROOT$JOB
done
