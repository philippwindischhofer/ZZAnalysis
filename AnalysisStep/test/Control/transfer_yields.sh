#!/bin/bash

# transfer the produced yield output files to LXPLUS (where they are going to be needed for the combined result)
CURRENT_DIR=`pwd`

SOURCE_ROOT="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/bin/slc6_amd64_gcc630/"
DIRS="Fits ROOT_files YAML_files"

DEST_ROOT="phwindis@lxplus.cern.ch:~/Yields/"

cd $SOURCE_ROOT
scp -r $DIRS $DEST_ROOT

cd $CURRENT_DIR