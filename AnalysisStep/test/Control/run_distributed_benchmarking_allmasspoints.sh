#!/bin/bash

MASS_POINTS="120 125 130"

CAMPAIGN_DIR=$1
ENGINE=$2

if [ -z $ENGINE ]
then
    echo "no engine name provided, using default: tree"
    ENGINE="tree"
fi

CONTROL_DIR="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/AnalysisStep/test/Control/"
BENCHMARKER="run_distributed_benchmarking.sh"

for MASS_POINT in $MASS_POINTS
do
    sh $CONTROL_DIR$BENCHMARKER $CAMPAIGN_DIR $ENGINE $MASS_POINT
done