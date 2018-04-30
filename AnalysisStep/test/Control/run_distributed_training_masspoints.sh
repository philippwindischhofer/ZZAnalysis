#!/bin/bash

# ---------------------------------------------
#  global settings
# ---------------------------------------------
CURRENT_DIR=`pwd`
CAMPAIGN_DIR=$1
INPUT_CONFIG_FILE=$2

PYTHON_DIR_ORIGINAL="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/AnalysisStep/test/Python/"
CONTROL_DIR_ORIGINAL="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/AnalysisStep/test/Control/"

CAMPAIGN_GEN="MassPointCampaignGenerator.py"
SUBMIT_TRAINING="run_distributed_training.sh"

MASS_POINTS="120 124 125 126 130"

cd $CAMPAIGN_DIR

for MASS_POINT in $MASS_POINTS
do
    mkdir -p $CAMPAIGN_DIR$MASS_POINT
    python $PYTHON_DIR_ORIGINAL$CAMPAIGN_GEN $CAMPAIGN_DIR"campaign.conf" $CAMPAIGN_DIR$MASS_POINT"/campaign.conf" $MASS_POINT
    sh $CONTROL_DIR_ORIGINAL$SUBMIT_TRAINING $CAMPAIGN_DIR$MASS_POINT"/" $INPUT_CONFIG_FILE
done