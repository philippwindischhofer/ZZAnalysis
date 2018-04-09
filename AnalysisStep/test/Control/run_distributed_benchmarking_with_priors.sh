#!/bin/bash

# ---------------------------------------------
#  settings for this training campaign
# ---------------------------------------------

#COMP_REF_DIR="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/BenchmarkerPlotsReferenceReducedCategorySet/"
#COMP_REF_DIR="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/BenchmarkerPlotsReferenceNoBackground/"
COMP_REF_DIR="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/BenchmarkerPlotsReferenceForOptimization/"

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
BENCHMARKER="run_benchmarker"
COMPARER="run_comp"

# ---------------------------------------------
#  first, copy all the executables to the campaign folder
# ---------------------------------------------
echo "preparing filesystem for training campaign"

mkdir -p $BIN_DIR
cp $BIN_DIR_ORIGINAL$BENCHMARKER $BIN_DIR_ORIGINAL$COMPARER $BIN_DIR

cd $CAMPAIGN_DIR

RUN_DIRLIST=`ls -d */ | egrep -v 'bin|statistics'`

# for all the runs in this sweep
for RUN in $RUN_DIRLIST
do
    # make all the necessary directories now
    CALIBRATION_DIR=$CAMPAIGN_DIR$RUN"calibration/"
    AUGMENTATION_DIR=$CAMPAIGN_DIR$RUN"augmentation/"
    BENCHMARK_DIR=$CAMPAIGN_DIR$RUN"benchmark_priors_"$ENGINE"/"
    COMP_DIR=$CAMPAIGN_DIR$RUN"comp_priors_"$ENGINE"/"
    BENCHMARK_SETTINGS_DIR=$CAMPAIGN_DIR$RUN"settings_benchmark_priors_"$ENGINE"/"
    PRIOR_DIR=$CAMPAIGN_DIR$RUN"priors_"$ENGINE"/"

    mkdir -p $BENCHMARK_SETTINGS_DIR
    mkdir -p $BENCHMARK_DIR
    mkdir -p $COMP_DIR

    BENCHMARK_SCRIPT=$BENCHMARK_SETTINGS_DIR"run_benchmark_priors_"$ENGINE".sh"
    BENCHMARK_LOGFILE=$BENCHMARK_SETTINGS_DIR"log_benchmark_priors_"$ENGINE".txt"

    echo "#!/bin/bash" > $BENCHMARK_SCRIPT

    # launch the benchmarking
    echo $BIN_DIR$BENCHMARKER $CALIBRATION_DIR $AUGMENTATION_DIR $BENCHMARK_DIR $ENGINE $PRIOR_DIR "&>" $BENCHMARK_LOGFILE >> $BENCHMARK_SCRIPT
    echo "sleep 5" >> $BENCHMARK_SCRIPT

    # launch the comparison to the reference
    echo $BIN_DIR$COMPARER $BENCHMARK_DIR $COMP_REF_DIR $COMP_DIR "&>>" $BENCHMARK_LOGFILE >> $BENCHMARK_SCRIPT
done

sleep 1

# now go back and launch all the jobs that have been prepared
cd $CAMPAIGN_DIR

JOBS=`find * | grep run_benchmark_priors_$ENGINE.sh$`

for JOB in $JOBS
do
    echo "lauching benchmarking for " $CAMPAIGN_DIR$JOB
    $JOB_SUBMITTER $CAMPAIGN_DIR$JOB
done

cd $CURRENT_DIR