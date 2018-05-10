#!/bin/bash

TRAINING_MASS_POINT="125"
MC_FILE_NAME="ZZ4lAnalysis.root"
BIN_DIR="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/bin/slc6_amd64_gcc630/"
CHUNK_EXTRACTOR="run_chunk_extractor"

# this prepares a fully trained classifier to be used as input for the calculation of the yield parameterization
SOURCE_ROOT=$1
DEST_ROOT=$2
ENGINE=$3

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
cp -r $SOURCE_ROOT"priors_"$ENGINE $DEST_ROOT

# now, copy the augmentation
AUGMENTATION_SOURCE=$SOURCE_ROOT"augmentation_test/"
AUGMENTATION_DEST=$DEST_ROOT"CJLST_NTuples/"

cp -r $AUGMENTATION_SOURCE $AUGMENTATION_DEST
