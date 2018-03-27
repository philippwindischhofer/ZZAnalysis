#!/bin/bash

# ---------------------------------------------
#  global settings
# ---------------------------------------------
PYTHON_DIR="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/AnalysisStep/test/Python/"
CONTROL_DIR="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/AnalysisStep/test/Control/"
CONFIG_FILE_GEN="ConfigFileSweeper.py"
STATISTICS_GEN="SampleStatistics.py"
RUN_PIPELINE="run_pipeline.sh"
CURRENT_DIR=`pwd`
CAMPAIGN_DIR=$1

MAX_PYTHONS=10

echo "preparing filesystem for training campaign"

# first, run the generation of the sub-config files
python $PYTHON_DIR$CONFIG_FILE_GEN $CAMPAIGN_DIR

# now should have plenty of prepared directories on which to run
cd $CAMPAIGN_DIR

DIRLIST=`ls -d */`

for DIR in $DIRLIST
do
    while : ; do
	CUR_PYTHONS=`ps -u wind | grep python | wc -l`
	ALLOWED_PYTHONS=`expr $MAX_PYTHONS - $CUR_PYTHONS`
	sleep 1
	[[ $ALLOWED_PYTHONS -gt 0 ]] && break
    done
    echo "have "$CUR_PYTHONS" python instances running and resources for "$ALLOWED_PYTHONS" more"

    NEXT_LAUNCH=$CAMPAIGN_DIR${DIR%?}
    echo "launching pipeline on" $NEXT_LAUNCH
    
    LOGFILE=$NEXT_LAUNCH"/log.txt"
    sh $CONTROL_DIR$RUN_PIPELINE $NEXT_LAUNCH &> $LOGFILE &
    sleep 1
done

# wait for all jobs to complete
echo "launched all jobs, waiting for them to finish"
wait

echo "all jobs finished"

# now launch statistics on the campaign directory
# python $PYTHON_DIR$STATISTICS_GEN $CAMPAIGN_DIR

# return to the original directory once done
cd $CURRENT_DIR