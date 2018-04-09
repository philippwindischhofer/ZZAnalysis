#!/bin/bash

# ---------------------------------------------
#  global settings
# ---------------------------------------------
CURRENT_DIR=`pwd`
CAMPAIGN_DIR=$1
ENGINE=$2

if [ -z $ENGINE ]
then
    echo "no engine name provided, using default: rand_KL"
    ENGINE="rand_KL"
fi

JOB_SUBMITTER="/opt/exp_soft/cms/t3/t3submit_new"

# the directories where the original sources are located
BIN_DIR_ORIGINAL="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/bin/slc6_amd64_gcc630/"

# the (common) source directory for this campaign
BIN_DIR=$CAMPAIGN_DIR"bin/"

# the needed part from the C++ sources
PRIOR_OPTIMIZER="run_prior_optimizer"

# ---------------------------------------------
#  first, copy all the executables to the campaign folder
# ---------------------------------------------
echo "preparing filesystem for training campaign"

mkdir -p $BIN_DIR
cp $BIN_DIR_ORIGINAL$PRIOR_OPTIMIZER $BIN_DIR

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
    echo $BIN_DIR$PRIOR_OPTIMIZER $CAMPAIGN_DIR$RUN $PRIOR_DIR $ENGINE "&>" $PRIOR_LOGFILE >> $PRIOR_SCRIPT
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