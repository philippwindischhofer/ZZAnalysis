#!/bin/bash

MASS_POINTS="120 125 130"

if [[ -z "$CMSSW_BASE" ]]; then
    echo "ERROR: need to have CMSSW_BASE set! Did you forget to run 'cmsenv'?"
    exit
fi

# ---------------------------------------------
#  parse the given arguments
# ---------------------------------------------
#  default values
PRIOR_FILE="priors_bkg.txt"

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
    --ref)
    LEGACY_REF_DIR="$2/"
    shift
    shift
    ;;
    --priors)
    PRIOR_FILE="$2"
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

CAMPAIGN_DIR="${1:-$CAMPAIGN_DIR}"
CAMPAIGN_DIR=$CAMPAIGN_DIR"/"

if [ -z $ENGINE ]
then
    echo "no engine name provided, using default: tree"
    ENGINE="tree"
fi

if [ -z $PRIOR_FILE ]
then
    echo "no prior file provided, using default: priors.txt"
    PRIOR_FILE="priors.txt"
fi

CONTROL_DIR=$CMSSW_BASE"/src/ZZAnalysis/AnalysisStep/test/Control/"
BENCHMARKER="run_distributed_benchmarking_with_priors.sh"

for MASS_POINT in $MASS_POINTS
do
    sh $CONTROL_DIR$BENCHMARKER $CAMPAIGN_DIR "--engine" $ENGINE "--mass" $MASS_POINT "--ref" $LEGACY_REF_DIR "--priors" $PRIOR_FILE
done