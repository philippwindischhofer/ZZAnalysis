#!/bin/bash

if [[ -z "$CMSSW_BASE" ]]; then
    echo "ERROR: need to have CMSSW_BASE set! Did you forget to run 'cmsenv'?"
    exit
fi

MC_PREPARED_DIR="${1:-$MC_PREPARED_DIR}"

MC_PREPARED_DIR=$MC_PREPARED_DIR"/"

# call the python script that handles the rest
python $CMSSW_BASE"/src/ZZAnalysis/AnalysisStep/test/Python/prepare_systematics.py" $MC_PREPARED_DIR