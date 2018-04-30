#!/bin/bash

CAMPAIGN_DIR=$1
shift

CONTROL_DIR_ORIGINAL="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/AnalysisStep/test/Control/"
SUBMIT_AUGMENTATION="run_distributed_augmentation.sh"

cd $CAMPAIGN_DIR
MASS_POINTS=`ls -d */`

for MASS_POINT in $MASS_POINTS
do
    echo "starting augmentation for " $CAMPAIGN_DIR$MASS_POINT
    sh $CONTROL_DIR_ORIGINAL$SUBMIT_AUGMENTATION $CAMPAIGN_DIR$MASS_POINT "$@"
done