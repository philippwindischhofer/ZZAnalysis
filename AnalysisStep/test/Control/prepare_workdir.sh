#!/bin/bash

if [[ -z "$CMSSW_BASE" ]]; then
    echo "ERROR: need to have CMSSW_BASE set! Did you forget to run 'cmsenv'?"
    exit
fi

# ---------------------------------------------
#  parse the given arguments
# ---------------------------------------------
POSARG=()

while [[ $# -gt 0 ]]
do
key=$1

case $key in
    --source)
    MC_PREPARED_DIR="$2"
    shift
    shift
    ;;
    --dest)
    MC_TRAINING_DIR="$2"
    shift
    shift
    ;;
    *)
    POSARG+=("$1")
    shift
    ;;
esac
done

# set back the positional arguments in case they will be needed later
set -- "${POSARG[@]}"

MC_PREPARED_DIR=$MC_PREPARED_DIR"/"
MC_TRAINING_DIR=$MC_TRAINING_DIR"/"

# call the python script that actually takes care of all the work
python $CMSSW_BASE"/src/ZZAnalysis/AnalysisStep/test/Python/prepare_workdir.py" $MC_PREPARED_DIR $MC_TRAINING_DIR