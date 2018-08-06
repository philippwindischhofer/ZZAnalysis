#!/bin/bash

if [[ -z "$CMSSW_BASE" ]]; then
    echo "ERROR: need to have CMSSW_BASE set! Did you forget to run 'cmsenv'?"
    exit
fi

BIN_DIR=$CMSSW_BASE"/bin/slc6_amd64_gcc630/"
PLOTTER="run_migration_matrix_plotter"

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

CAMPAIGN_DIR="${1:-$CAMPAIGN_DIR}"
CAMPAIGN_DIR=$CAMPAIGN_DIR"/"

cd $CAMPAIGN_DIR
RUN_DIRLIST=`ls -d */ | egrep -v 'bin|statistics'`

for RUN in $RUN_DIRLIST
do
    RUN_DIR=$CAMPAIGN_DIR$RUN

    # run the migration matrix plotting
    $BIN_DIR$PLOTTER $RUN_DIR $ENGINE
done
