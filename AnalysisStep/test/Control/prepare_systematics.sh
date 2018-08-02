#!/bin/bash

if [[ -z "$CMSSW_BASE" ]]; then
    echo "ERROR: need to have CMSSW_BASE set! Did you forget to run 'cmsenv'?"
    exit
fi

if [[ "$#" -ne 1 ]]; then
    echo "Usage: $0 DIR"
    exit
fi

DIR=$1"/"

# call the python script that handles the rest
python $CMSSW_BASE"/src/ZZAnalysis/AnalysisStep/test/Python/prepare_systematics.py" $DIR