#!/bin/bash

if [[ -z "$CMSSW_BASE" ]]; then
    echo "ERROR: need to have CMSSW_BASE set! Did you forget to run 'cmsenv'?"
    exit
fi

PYTHON_DIR=$CMSSW_BASE"/src/ZZAnalysis/AnalysisStep/test/Python/"
INPUT_VARIABLE_SELECTOR="input_feature_selection.py"

USEMELA="n"

# ---------------------------------------------
#  parse the given arguments
# ---------------------------------------------
POSARG=()

while [[ $# -gt 0 ]]
do
key=$1

case $key in
    --threshold)
    THRESHOLD="$2"
    shift
    shift
    ;;	
    --dest)
    DEST_ROOT="$2/"
    shift
    shift
    ;;
    --name)
    CAMPAIGN_NAME="$2"
    shift
    shift
    ;;
    --workdir)
    MC_PATH="$2/"
    shift
    shift
    ;;
    --mela)
    USEMELA="y"
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

# create the output director, if it doesn't already exist
mkdir -p $DEST_ROOT

# run the input variable selection
python $PYTHON_DIR$INPUT_VARIABLE_SELECTOR $DEST_ROOT $CAMPAIGN_NAME $MC_PATH"/trainval/" $USEMELA $THRESHOLD