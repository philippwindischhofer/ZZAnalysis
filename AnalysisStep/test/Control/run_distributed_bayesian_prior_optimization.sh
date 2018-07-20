#!/bin/bash

if [[ -z "$CMSSW_BASE" ]]; then
    echo "ERROR: need to have CMSSW_BASE set! Did you forget to run 'cmsenv'?"
    exit
fi

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
    REF_DIR="$2"
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

# ---------------------------------------------
#  global settings
# ---------------------------------------------
CURRENT_DIR=`pwd`
CAMPAIGN_DIR=$1

if [ -z $ENGINE ]
then
    echo "no engine name provided, using default: rand_KL"
    ENGINE="rand_KL"
fi

JOB_SUBMITTER="/opt/exp_soft/cms/t3/t3submit_new"

BIN_DIR=$CMSSW_BASE"/bin/slc6_amd64_gcc630/"
PYTHON_DIR=$CMSSW_BASE"/src/ZZAnalysis/AnalysisStep/test/Python/"

# the (common) source directory for this campaign
BIN_DIR=$CAMPAIGN_DIR"bin/"

# the needed part from the C++ sources
#PRIOR_OPTIMIZER="BayesianPriorOptimizer.py"
PRIOR_OPTIMIZER="SequentialBayesianPriorOptimizer.py"
PYTHON_LIB="trainlib"
PRIOR_EVALUATOR="run_prior_evaluator"

cd $CAMPAIGN_DIR

RUN_DIRLIST=`ls -d */ | egrep -v 'bin|statistics'`

# for all the runs in this sweep
for RUN in $RUN_DIRLIST
do
    # make all the necessary directories now
    PRIOR_DIR=$CAMPAIGN_DIR$RUN"priors_"$ENGINE"/"
    PRIOR_SETTINGS_DIR=$CAMPAIGN_DIR$RUN"settings_priors_"$ENGINE"/"

    mkdir -p $PRIOR_DIR
    mkdir -p $PRIOR_SETTINGS_DIR

    PRIOR_SCRIPT=$PRIOR_SETTINGS_DIR"run_prior_optimizer_"$ENGINE".sh"
    PRIOR_LOGFILE=$PRIOR_SETTINGS_DIR"log_prior_optimizer_"$ENGINE".txt"

    echo "#!/bin/bash" > $PRIOR_SCRIPT

    # launch the optimization
    echo "python" $PYTHON_DIR$PRIOR_OPTIMIZER $CAMPAIGN_DIR$RUN $PRIOR_DIR $ENGINE $REF_DIR "&>" $PRIOR_LOGFILE >> $PRIOR_SCRIPT
done

# now go back and launch all the jobs that have been prepared
cd $CAMPAIGN_DIR

JOBS=`find * | grep run_prior_optimizer_$ENGINE.sh$`

for JOB in $JOBS
do
    echo "lauching prior optimization for " $CAMPAIGN_DIR$JOB
    $JOB_SUBMITTER $CAMPAIGN_DIR$JOB
done

cd $CURRENT_DIR