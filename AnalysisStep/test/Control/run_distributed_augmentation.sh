#!/bin/bash

if [[ -z "$CMSSW_BASE" ]]; then
    echo "ERROR: need to have CMSSW_BASE set! Did you forget to run 'cmsenv'?"
    exit
fi

# ---------------------------------------------
#  global settings
# ---------------------------------------------
CURRENT_DIR=`pwd`
CAMPAIGN_DIR=$1
SOURCE_ROOT=$2
DEST_ROOT=$3
AUGMENTATION_SETTINGS_ROOT=$4
GLOBAL_SETTINGS_DIR=$5

JOB_SUBMITTER="/opt/exp_soft/cms/t3/t3submit_new"

# Python source directory
BIN_DIR=$CMSSW_BASE"/src/ZZAnalysis/AnalysisStep/test/Python/"

# the needed part from the python sources
AUGMENTER="SingleAugmentation.py"
PYTHON_LIB="trainlib"

MC_DIR=$SOURCE_ROOT

cd $MC_DIR
dirs=`ls -d */`
datafiles=""

for dir in $dirs
do
    datafiles=$datafiles" ${dir%/}"
done 

echo $datafiles

cd $CAMPAIGN_DIR

# create the directory that will hold the augmented MC files
AUGMENTATION_DIR=$DEST_ROOT"/"
TRAINING_DIR=$GLOBAL_SETTINGS_DIR"/training/"
AUGMENTATION_SETTINGS_DIR=$AUGMENTATION_SETTINGS_ROOT"/"
mkdir -p $AUGMENTATION_DIR
mkdir -p $AUGMENTATION_SETTINGS_DIR

sleep 5

for datafile in $datafiles
do
    mkdir -p $AUGMENTATION_SETTINGS_DIR$datafile

    # prepare the job augmenting this specific datafile
    AUGMENTATION_SCRIPT=$AUGMENTATION_SETTINGS_DIR$datafile"/run_augmentation.sh"
    AUGMENTATION_LOGFILE=$AUGMENTATION_SETTINGS_DIR$datafile"/log_augmentation.txt"
    
    echo "#!/bin/bash" > $AUGMENTATION_SCRIPT
    echo "python" $BIN_DIR$AUGMENTER $MC_DIR$datafile $AUGMENTATION_DIR$datafile "ClassTree" $GLOBAL_SETTINGS_DIR "&>" $AUGMENTATION_LOGFILE >> $AUGMENTATION_SCRIPT
done

# now go back and launch all the jobs that have been prepared
cd $AUGMENTATION_SETTINGS_DIR

JOBS=`find * | grep run_augmentation.sh$`

for JOB in $JOBS
do
    echo "launching augmentation for " $AUGMENTATION_SETTINGS_DIR$JOB
    until $JOB_SUBMITTER "-short" $AUGMENTATION_SETTINGS_DIR$JOB
    do
    	echo "----------------------------------------------------------------"
    	echo " error submitting job, retrying ..."
    	echo "----------------------------------------------------------------"
    	sleep 1
    done
done

cd $CURRENT_DIR