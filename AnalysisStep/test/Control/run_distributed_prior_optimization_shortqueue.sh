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
CLUSTER_STATUS="/opt/exp_soft/cms/t3/t3stat"

# the directories where the original sources are located
BIN_DIR_ORIGINAL="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/bin/slc6_amd64_gcc630/"
PYTHON_DIR_ORIGINAL="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/AnalysisStep/test/Python/"

# the (common) source directory for this campaign
BIN_DIR=$CAMPAIGN_DIR"bin/"

# the needed part from the C++ sources
PRIOR_OPTIMIZER="BayesianPriorOptimizer.py"
PRIOR_EVALUATOR="run_prior_evaluator"

# ---------------------------------------------
#  first, copy all the executables to the campaign folder
# ---------------------------------------------
echo "preparing filesystem for training campaign"

mkdir -p $BIN_DIR
cp $PYTHON_DIR_ORIGINAL$PRIOR_OPTIMIZER $BIN_DIR
cp $BIN_DIR_ORIGINAL$PRIOR_EVALUATOR $BIN_DIR

cd $CAMPAIGN_DIR

RUN_DIRLIST=`ls -d */ | egrep -v 'bin|statistics'`

# for all the runs in this sweep
for RUN in $RUN_DIRLIST
do
    # make all the necessary directories now
    PRIOR_DIR=$CAMPAIGN_DIR$RUN"priors_"$ENGINE"_short/"
    PRIOR_SETTINGS_DIR=$CAMPAIGN_DIR$RUN"settings_priors_"$ENGINE"_short/"

    mkdir -p $PRIOR_DIR
    mkdir -p $PRIOR_SETTINGS_DIR

    PRIOR_SCRIPT=$PRIOR_SETTINGS_DIR"run_prior_optimizer_"$ENGINE"_short.sh"
    PRIOR_LOGFILE=$PRIOR_SETTINGS_DIR"log_prior_optimizer_"$ENGINE"_short.txt"

    # ------------------------------------------------------------
    # this is the actually application specific part

    echo "#!/bin/bash" > $PRIOR_SCRIPT

    # launch the optimization
    echo "python" $BIN_DIR$PRIOR_OPTIMIZER $CAMPAIGN_DIR$RUN $PRIOR_DIR $ENGINE "&>" $PRIOR_LOGFILE >> $PRIOR_SCRIPT

    # ------------------------------------------------------------
done

# now go back and launch all the jobs that have been prepared
cd $CAMPAIGN_DIR

JOBS=`find * | grep run_prior_optimizer_${ENGINE}_short.sh$`

echo $JOBS

for JOB in $JOBS
do
    echo "lauching prior optimization for " $CAMPAIGN_DIR$JOB
    RESP=`$JOB_SUBMITTER "-short" $CAMPAIGN_DIR$JOB`
    JOBID=`echo $RESP | sed s:.*cluster\ :: | sed s:[.]::`

    RUNNING_JOBS=$RUNNING_JOBS" "`echo $JOBID`

done

echo "running jobs: "$RUNNING_JOBS

while true
do
    RUNNING_JOBS_UPDATED=$RUNNING_JOBS

    for RUNNING_JOB in $RUNNING_JOBS
    do
	echo "checking now for "$RUNNING_JOB

	# check if it is still running
	STATUS=`$CLUSTER_STATUS | grep $RUNNING_JOB | wc -l`

	if [ "$STATUS" -eq "0" ]; then
	    echo "job "$RUNNING_JOB "finished, restarting it"
    
            # look for the job file
	    JOB_DIR=`find * | grep o$RUNNING_JOB`
	    JOB_DIR=`echo $JOB_DIR | sed s:run_prior_optimizer_${ENGINE}_short.sh.*::`
	
	    echo "found job settings in "$JOB_DIR"run_prior_optimizer_${ENGINE}_short.sh"

	    RESP=`$JOB_SUBMITTER "-short" $JOB_DIR"run_prior_optimizer_${ENGINE}_short.sh"`
	    JOBID=`echo $RESP | sed s:.*cluster\ :: | sed s:[.]::`

	    # update the list of running jobs
	    RUNNING_JOBS_UPDATED=`echo $RUNNING_JOBS_UPDATED | sed s:$RUNNING_JOB::`
	    RUNNING_JOBS_UPDATED=$RUNNING_JOBS_UPDATED" "`echo $JOBID`

	    echo "running jobs: "$RUNNING_JOBS_UPDATED	    
	fi
    done

    RUNNING_JOBS=$RUNNING_JOBS_UPDATED

    sleep 1m
done

cd $CURRENT_DIR