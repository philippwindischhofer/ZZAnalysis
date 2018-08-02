#!/bin/bash

if [[ -z "$CMSSW_BASE" ]]; then
    echo "ERROR: need to have CMSSW_BASE set! Did you forget to run 'cmsenv'?"
    exit
fi

PYTHON_DIR=$CMSSW_BASE"/src/ZZAnalysis/AnalysisStep/test/Python/PlottingScripts/"
INPUT_VARIABLE_PLOTTER="plot_input_variables.py"

# ---------------------------------------------
#  parse the given arguments
# ---------------------------------------------
POSARG=()

while [[ $# -gt 0 ]]
do
key=$1

case $key in
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
    *)
    POSARG+=("$1")
    shift
    ;;
esac
done

# set back the positional arguments in case they will be needed later
set -- "${POSARG[@]}"

python $PYTHON_DIR$INPUT_VARIABLE_PLOTTER $DEST_ROOT $CAMPAIGN_NAME