#!/bin/bash

# ---------------------------------------------
#  settings for this training campaign
# ---------------------------------------------

#COMP_REF_DIR="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/BenchmarkerPlotsReferenceReducedCategorySet/"
COMP_REF_DIR="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/BenchmarkerPlotsReferenceNoBackground/"
#COMP_REF_DIR="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/BenchmarkerPlotsReferenceForOptimization/"

# ---------------------------------------------
#  global settings
# ---------------------------------------------
CURRENT_DIR=`pwd`
CAMPAIGN_DIR=$1

JOB_SUBMITTER="/opt/exp_soft/cms/t3/t3submit_new"

# the directories where the original sources are located
CONTROL_DIR_ORIGINAL="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/AnalysisStep/test/Control/"
BIN_DIR_ORIGINAL="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/bin/slc6_amd64_gcc630/"

# the (common) source directory for this campaign
BIN_DIR=$CAMPAIGN_DIR"bin/"

# the needed part from the C++ sources
CALIBRATOR="run_calibrator"
BENCHMARKER="run_benchmarker"
ROC_PLOTTER="run_roc_plotter"
COMPARER="run_comp"

# the needed part from the control sources
RUN_PIPELINE="run_pipeline.sh"
PLOT_POSTPROCESSOR="plot_postprocessing.sh"

# ---------------------------------------------
#  first, copy all the executables to the campaign folder
# ---------------------------------------------
echo "preparing filesystem for training campaign"

mkdir -p $BIN_DIR
cp $BIN_DIR_ORIGINAL$CALIBRATOR $BIN_DIR_ORIGINAL$BENCHMARKER $BIN_DIR_ORIGINAL$ROC_PLOTTER $BIN_DIR_ORIGINAL$COMPARER $BIN_DIR
cp $CONTROL_DIR_ORIGINAL$PLOT_POSTPROCESSOR $BIN_DIR

cd $CAMPAIGN_DIR

RUN_DIRLIST=`ls -d */ | egrep -v 'bin|statistics'`

# for all the runs in this sweep
for RUN in $RUN_DIRLIST
do
    # make all the necessary directories now
    CALIBRATION_DIR=$CAMPAIGN_DIR$RUN"calibration/"
    BENCHMARK_DIR=$CAMPAIGN_DIR$RUN"benchmark/"
    COMP_DIR=$CAMPAIGN_DIR$RUN"comp/"
    ROC_DIR=$CAMPAIGN_DIR$RUN"ROCs/"
    AUGMENTATION_DIR=$CAMPAIGN_DIR$RUN"augmentation/"
    BENCHMARK_SETTINGS_DIR=$CAMPAIGN_DIR$RUN"settings_benchmark/"

    mkdir -p $BENCHMARK_SETTINGS_DIR
    mkdir -p $CALIBRATION_DIR
    mkdir -p $BENCHMARK_DIR
    mkdir -p $COMP_DIR
    mkdir -p $ROC_DIR

    BENCHMARK_SCRIPT=$BENCHMARK_SETTINGS_DIR"run_benchmark.sh"
    BENCHMARK_LOGFILE=$BENCHMARK_SETTINGS_DIR"log_benchmark.txt"

    echo "#!/bin/bash" > $BENCHMARK_SCRIPT

    # launch the calibration
    echo $BIN_DIR$CALIBRATOR $AUGMENTATION_DIR "ML" $CALIBRATION_DIR "&>" $BENCHMARK_LOGFILE >> $BENCHMARK_SCRIPT

    # launch the benchmarking
    echo $BIN_DIR$BENCHMARKER $CALIBRATION_DIR $AUGMENTATION_DIR $BENCHMARK_DIR "&>>" $BENCHMARK_LOGFILE >> $BENCHMARK_SCRIPT

    # launch the comparison to the reference
    echo $BIN_DIR$COMPARER $BENCHMARK_DIR $COMP_REF_DIR $COMP_DIR "&>>" $BENCHMARK_LOGFILE >> $BENCHMARK_SCRIPT

    # launch the plotting of ROCs
    echo $BIN_DIR$ROC_PLOTTER $AUGMENTATION_DIR $ROC_DIR "&>>" $BENCHMARK_LOGFILE >> $BENCHMARK_SCRIPT
done

# now go back and launch all the jobs that have been prepared
cd $CAMPAIGN_DIR

JOBS=`find * | grep run_benchmark.sh`

for JOB in $JOBS
do
    echo "lauching benchmarking for " $CAMPAIGN_DIR$JOB
    $JOB_SUBMITTER $CAMPAIGN_DIR$JOB
done

cd $CURRENT_DIR