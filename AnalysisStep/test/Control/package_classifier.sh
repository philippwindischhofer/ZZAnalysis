#!/bin/bash

TRAINING_MASS_POINT="125"
MC_FILE_NAME="ZZ4lAnalysis.root"
BIN_DIR="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/bin/slc6_amd64_gcc630/"
CHUNK_EXTRACTOR="run_chunk_extractor"

# this prepares a fully trained classifier to be used as input for the calculation of the yield parameterization
SOURCE_ROOT=$1
DEST_ROOT=$2

CONFIG_FILE="settings.conf"

# first, create the destination directory
mkdir -p $DEST_ROOT

# copy the calibration
CALIBRATION_SOURCE=$SOURCE_ROOT"calibration_validation/"
CALIBRATION_DEST=$DEST_ROOT"calibration/"
cp -r $CALIBRATION_SOURCE $CALIBRATION_DEST

# copy the configuration file
cp $SOURCE_ROOT$CONFIG_FILE $DEST_ROOT$CONFIG_FILE

# copy the priors
cp -r $SOURCE_ROOT"priors*" $DEST_ROOT

# now, copy the augmentation, but for the mass point on which the classifier was trained, get only the (reweighted) test portion
AUGMENTATION_SOURCE=$SOURCE_ROOT"augmentation/"
AUGMENTATION_DEST=$DEST_ROOT"CJLST_NTuples/"
mkdir -p $AUGMENTATION_DEST

cd $AUGMENTATION_SOURCE
SOURCE_FOLDERS=`ls -d */`

for SOURCE_FOLDER in $SOURCE_FOLDERS
do
    DEST_FOLDER=$AUGMENTATION_DEST$SOURCE_FOLDER
    mkdir -p $DEST_FOLDER

    if [[ "$SOURCE_FOLDER" =~ "$TRAINING_MASS_POINT" ]]
    then
	# this file has been used for training, need to pass it through the chunk extractor that gives back only the test piece
	$BIN_DIR$CHUNK_EXTRACTOR $AUGMENTATION_SOURCE$SOURCE_FOLDER$MC_FILE_NAME $DEST_FOLDER$MC_FILE_NAME "0.75" "1.0"
    else
	# this file has not been used for training, can just copy it through
	cp $AUGMENTATION_SOURCE$SOURCE_FOLDER$MC_FILE_NAME $DEST_FOLDER$MC_FILE_NAME 
    fi
done