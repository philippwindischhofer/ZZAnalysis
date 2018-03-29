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

JOB_SUBMITTER="/opt/exp_soft/cms/t3/t3submit_new"

echo "preparing filesystem for training campaign"

# first, run the generation of the sub-config files
python $PYTHON_DIR$CONFIG_FILE_GEN $CAMPAIGN_DIR

# now should have plenty of prepared directories on which to run
cd $CAMPAIGN_DIR

DIRLIST=`ls -d */`

for DIR in $DIRLIST
do
    NEXT_LAUNCH=$CAMPAIGN_DIR${DIR%?}
    echo "launching pipeline on" $NEXT_LAUNCH
    
    LOGFILE=$NEXT_LAUNCH"/log.txt"
    JOB_SCRIPT=$NEXT_LAUNCH"/run_job.sh"

    echo "#!/bin/bash" >> $JOB_SCRIPT
    echo $CONTROL_DIR$RUN_PIPELINE $NEXT_LAUNCH "&>" $LOGFILE >> $JOB_SCRIPT

    $JOB_SUBMITTER $JOB_SCRIPT
done

# wait for all jobs to complete
echo "launched all jobs"

# return to the original directory once done
cd $CURRENT_DIR