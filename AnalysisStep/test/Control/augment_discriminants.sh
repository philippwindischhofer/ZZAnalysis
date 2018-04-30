#!/bin/bash

CURRENT_DIR=`pwd`

# this computes and augments a completely trained collection of discriminants to external ROOT files (that may or may not be identical with the training data)

JOB_SUBMITTER="/opt/exp_soft/cms/t3/t3submit_new"
PYTHON_DIR="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/AnalysisStep/test/Python/"
AUGMENTER="SingleAugmentation.py"

# apply MODEL to SOURCE and save it to DEST
MODEL_ROOT=$1
SOURCE_ROOT=$2
DEST_ROOT=$3

# the directory holding the settings of the trained models (i.e. which networks, their hyperparameters etc.)
SETTINGS_DIR=$MODEL_ROOT

# the directory holding the trained models (i.e. their weights)
MODEL_DIR=$MODEL_ROOT"training/"

cd $SOURCE_ROOT
SOURCE_FOLDERS=`ls -d */`

# create the destination directory
mkdir -p $DEST_ROOT

for SOURCE_FOLDER in $SOURCE_FOLDERS
do
    mkdir -p $DEST_ROOT$SOURCE_FOLDER

    # prepare the job augmenting this specific datafile
    AUGMENTATION_SCRIPT=$DEST_ROOT"run_augmentation_"${SOURCE_FOLDER%/}".sh"
    AUGMENTATION_LOGFILE=$DEST_ROOT"log_augmentation_"${SOURCE_FOLDER%/}".txt"

    echo "#!/bin/bash" > $AUGMENTATION_SCRIPT
    echo "python" $PYTHON_DIR$AUGMENTER $SOURCE_ROOT ${SOURCE_FOLDER%/} $SETTINGS_DIR $MODEL_DIR $DEST_ROOT "&>" $AUGMENTATION_LOGFILE >> $AUGMENTATION_SCRIPT
done

cd $DEST_ROOT

JOBS=`find * | grep run_augmentation.*.sh$`

for JOB in $JOBS
do
    echo "launching augmentation for " $DEST_ROOT$JOB
    #$JOB_SUBMITTER $DEST_ROOT$JOB
done

cd $CURRENT_DIR