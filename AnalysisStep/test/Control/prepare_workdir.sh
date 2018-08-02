#!/bin/bash

if [[ -z "$CMSSW_BASE" ]]; then
    echo "ERROR: need to have CMSSW_BASE set! Did you forget to run 'cmsenv'?"
    exit
fi

export JOB_SUBMITTER="/opt/exp_soft/cms/t3/t3submit_new"

# ---------------------------------------------
#  parse the given arguments
# ---------------------------------------------
POSARG=()

while [[ $# -gt 0 ]]
do
key=$1

case $key in
    --source)
    SOURCE_ROOT="$2/"
    shift
    shift
    ;;
    --dest)
    DEST_ROOT="$2/"
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

# call the python script that actually takes care of all the work
python $CMSSW_BASE"/src/ZZAnalysis/AnalysisStep/test/Python/prepare_workdir.py" $SOURCE_ROOT $DEST_ROOT