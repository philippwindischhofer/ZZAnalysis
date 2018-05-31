#!/bin/bash

DEST_NAME=$1
SYSTEMATICS_FILE=$2

PYTHON_DIR_ORIGINAL="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/AnalysisStep/test/Python/"
SYSTEMATICS_TO_YAML="Systematics2YAML.py"

# transfer the produced yield output files to LXPLUS (where they are going to be needed for the combined result)
CURRENT_DIR=`pwd`

SOURCE_ROOT="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/bin/slc6_amd64_gcc630/"
DIRS="Fits ROOT_files YAML_files"

DEST_ROOT="phwindis@lxplus.cern.ch:~/"$DEST_NAME"/"
DEST_ROOT_local="/data_CMS/cms/wind/"$DEST_NAME"/"

# also copy them to /data_CMS/
mkdir -p $DEST_ROOT_local

cd $SOURCE_ROOT
cp -r $DIRS $DEST_ROOT_local

# convert the systematics file into YAML
python $PYTHON_DIR_ORIGINAL$SYSTEMATICS_TO_YAML $SYSTEMATICS_FILE $DEST_ROOT_local

# copy the systematics file itself as well
cp $SYSTEMATICS_FILE $DEST_ROOT_local

scp -r $DEST_ROOT_local $DEST_ROOT

cd $CURRENT_DIR