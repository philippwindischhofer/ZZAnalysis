#!/bin/bash

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

# the directories where the original sources are located
PYTHON_DIR_ORIGINAL="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/AnalysisStep/test/Python/"

# the (common) source directory for this campaign
BIN_DIR=$CAMPAIGN_DIR"bin/"

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

# ---------------------------------------------
#  first, copy all the executables to the campaign folder
# ---------------------------------------------
echo "preparing filesystem for augmentation campaign"

mkdir -p $BIN_DIR
cp -r $PYTHON_DIR_ORIGINAL$PYTHON_LIB $BIN_DIR
cp $PYTHON_DIR_ORIGINAL$AUGMENTER $BIN_DIR

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
    echo "python" $BIN_DIR$AUGMENTER $MC_DIR $datafile $GLOBAL_SETTINGS_DIR $TRAINING_DIR $AUGMENTATION_DIR "&>" $AUGMENTATION_LOGFILE >> $AUGMENTATION_SCRIPT
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