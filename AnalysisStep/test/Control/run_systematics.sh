#!/bin/bash

if [[ -z "$CMSSW_BASE" ]]; then
    echo "ERROR: need to have CMSSW_BASE set! Did you forget to run 'cmsenv'?"
    exit
fi

BIN_DIR=$CMSSW_BASE"/bin/slc6_amd64_gcc630/"
PYTHON_DIR=$CMSSW_BASE"/src/ZZAnalysis/AnalysisStep/test/Python/"

SYSTEMATICS_PREPARER="run_systematics"
SYSTEMATICS_CONVERTER="Systematics2YAML.py"

# ---------------------------------------------
#  parse the given arguments
# ---------------------------------------------
POSARG=()

while [[ $# -gt 0 ]]
do
key=$1

case $key in
    --engine)
    ENGINE="$2"
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

RUN_DIR=$1"/"

# create output directory
SYSTEMATICS_DIR=$RUN_DIR"/systematic_uncertainties_"$ENGINE"/"
mkdir -p $SYSTEMATICS_DIR

# run the determination of the systematics
$BIN_DIR$SYSTEMATICS_PREPARER $RUN_DIR $ENGINE $SYSTEMATICS_DIR"/systematics_raw.txt"

# convert them into the proper YAML files
python $PYTHON_DIR$SYSTEMATICS_CONVERTER $SYSTEMATICS_DIR"/systematics_raw.txt" $SYSTEMATICS_DIR
