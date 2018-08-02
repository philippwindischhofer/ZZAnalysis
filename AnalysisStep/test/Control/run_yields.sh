#!/bin/bash

if [[ -z "$CMSSW_BASE" ]]; then
    echo "ERROR: need to have CMSSW_BASE set! Did you forget to run 'cmsenv'?"
    exit
fi

BIN_DIR=$CMSSW_BASE"/bin/slc6_amd64_gcc630/"
CONTROL_DIR=$CMSSW_BASE"/src/ZZAnalysis/AnalysisStep/test/Control/"
YIELD_PREPARER="run_yields"

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

# run the yield parameterization
$BIN_DIR$YIELD_PREPARER $RUN_DIR $ENGINE

sleep 3

# move the resulting output into the training directory
YIELDS_DIR=$RUN_DIR"/yield_parameterization_"$ENGINE"/"
mkdir -p $YIELDS_DIR
mv $CONTROL_DIR"/Fits/" $YIELDS_DIR
mv $CONTROL_DIR"/ROOT_files/" $YIELDS_DIR
mv $CONTROL_DIR"/YAML_files/" $YIELDS_DIR