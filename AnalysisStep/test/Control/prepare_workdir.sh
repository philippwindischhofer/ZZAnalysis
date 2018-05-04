#!/bin/bash

# -------------------------------
# global settings
# -------------------------------
TRAINING_FILES_SIG="ggH125 ttH125 VBFH125 WminusH125 WplusH125 ZH125"
TRAINING_FILES_BCK="ggTo2e2mu_Contin_MCFM701 ggTo2e2tau_Contin_MCFM701 ggTo2mu2tau_Contin_MCFM701 ggTo4e_Contin_MCFM701 ggTo4mu_Contin_MCFM701 ggTo4tau_Contin_MCFM701 ZZTo4l AllData"

ROOT_FILE_NAME="/ZZ4lAnalysis.root"

BIN_DIR="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/bin/slc6_amd64_gcc630/"
CHUNK_EXTRACTOR=$BIN_DIR"/run_chunk_extractor"
SCRAMBLER=$BIN_DIR"/run_scrambler"

SOURCE_ROOT=$1
DEST_ROOT=$2

mkdir -p $DEST_ROOT

TRAIN_DIR=$DEST_ROOT"/training/"
VALIDATION_DIR=$DEST_ROOT"/validation/"
TEST_DIR=$DEST_ROOT"/test/"
TEMP_DIR=$DEST_ROOT"/temp/"

cd $SOURCE_ROOT
ALL_FILES=`ls -d */`

# this folder contains the actual training dataset on which to train the discriminants and which to use for early-stopping regularization
TRAINVAL_DIR=$DEST_ROOT"/trainval/"

mkdir -p $TRAIN_DIR
mkdir -p $VALIDATION_DIR
mkdir -p $TEST_DIR
mkdir -p $TRAINVAL_DIR

mkdir -p $TEMP_DIR

# perform first the splitting into training, validation and test dataset
for file in $ALL_FILES
do
    if [[ $(echo $TRAINING_FILES_SIG$TRAINING_FILES_BCK | grep ${file%/}) ]]
    then
	mkdir -p $TRAIN_DIR$file
	$CHUNK_EXTRACTOR $SOURCE_ROOT$file$ROOT_FILE_NAME $TRAIN_DIR$file$ROOT_FILE_NAME "0.0" "0.5"

	mkdir -p $VALIDATION_DIR$file
	$CHUNK_EXTRACTOR $SOURCE_ROOT$file$ROOT_FILE_NAME $VALIDATION_DIR$file$ROOT_FILE_NAME "0.5" "0.75"

	mkdir -p $TEST_DIR$file
	$CHUNK_EXTRACTOR $SOURCE_ROOT$file$ROOT_FILE_NAME $TEST_DIR$file$ROOT_FILE_NAME "0.75" "1.0"
    fi
done

# now, need to generate the training dataset for the backgrounds:

# the files for signal-only training don't need any more treatment
for file in $TRAINING_FILES_SIG
do
    cp -r $TRAIN_DIR$file $TRAINVAL_DIR
done

# first, merge separately the training- and validation portions for the background, ...
mkdir -p $TEMP_DIR"/bck_training/"
mkdir -p $TEMP_DIR"/bck_validation/"

BCK_TRAINING_SRC=""
for file in $TRAINING_FILES_BCK
do
    BCK_TRAINING_SRC="${BCK_TRAINING_SRC} ${TRAIN_DIR}${file}${ROOT_FILE_NAME}"
done

BCK_VALIDATION_SRC=""
for file in $VALIDATION_FILES_BCK
do
    BCK_VALIDATION_SRC="${BCK_VALIDATION_SRC} ${TRAIN_DIR}${file}${ROOT_FILE_NAME}"
done

echo "now merging training files for background:"
for file in $BCK_TRAINING_SRC
do
    echo $file
done

hadd $TEMP_DIR"/bck_training/"$ROOT_FILE_NAME $BCK_TRAINING_SRC 

echo "now merging validation files for background:"
for file in $BCK_TRAINING_SRC
do
    echo $file
done

hadd $TEMP_DIR"/bck_validation/"$ROOT_FILE_NAME $BCK_VALIDATION_SRC 

# ... then, scramble each of them individually, ...
mkdir -p $TEMP_DIR"/bck_training_scrambled/"
mkdir -p $TEMP_DIR"/bck_validation_scrambled/"

$SCRAMBLER $TEMP_DIR"/bck_training/"$ROOT_FILE_NAME $TEMP_DIR"/bck_training_scrambled/"$ROOT_FILE_NAME
$SCRAMBLER $TEMP_DIR"/bck_validation/"$ROOT_FILE_NAME $TEMP_DIR"/bck_validation_scrambled/"$ROOT_FILE_NAME

# ... and finally add them together to form the final training/validation dataset
mkdir $TRAINVAL_DIR"/bck/"
hadd $TRAINVAL_DIR"/bck/"$ROOT_FILE_NAME $TEMP_DIR"/bck_training_scrambled/"$ROOT_FILE_NAME $TEMP_DIR"/bck_validation_scrambled/"$ROOT_FILE_NAME