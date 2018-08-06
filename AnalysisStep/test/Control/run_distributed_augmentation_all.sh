#!/bin/bash

if [[ -z "$CMSSW_BASE" ]]; then
    echo "ERROR: need to have CMSSW_BASE set! Did you forget to run 'cmsenv'?"
    exit
fi

CONTROL_DIR_ORIGINAL=$CMSSW_BASE"/src/ZZAnalysis/AnalysisStep/test/Control/"
SUBMIT_AUGMENTATION="/run_distributed_augmentation.sh"

PARTS="training_chunks validation_chunks test_chunks"

# ---------------------------------------------
#  parse the given arguments
# ---------------------------------------------
POSARG=()

while [[ $# -gt 0 ]]
do
key=$1

case $key in
    --workdir)
    MC_TRAINING_DIR="$2/"
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

cd $CAMPAIGN_DIR
RUN_DIRLIST=`ls -d */ | egrep -v 'bin|statistics'`

cd $CONTROL_DIR_ORIGINAL

# for all the runs in this sweep
for RUN in $RUN_DIRLIST
do
    for part in $PARTS
    do
	echo "starting augmentation for: " $part
	sh $CONTROL_DIR_ORIGINAL$SUBMIT_AUGMENTATION $CAMPAIGN_DIR $MC_TRAINING_DIR$part"/" $CAMPAIGN_DIR$RUN"/augmentation_"$part"/" $CAMPAIGN_DIR$RUN"/settings_augmentation_"$part"/" $CAMPAIGN_DIR$RUN"/"
    done
done