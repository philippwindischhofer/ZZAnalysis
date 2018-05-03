#!/bin/bash

DEST_NAME=$1

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

scp -r $DEST_ROOT_local $DEST_ROOT

cd $CURRENT_DIR