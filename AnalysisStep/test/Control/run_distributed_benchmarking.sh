#!/bin/bash

# ---------------------------------------------
#  settings for this training campaign
# ---------------------------------------------

#COMP_REF_DIR="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/BenchmarkerPlotsReferenceReducedCategorySet/"
#COMP_REF_DIR="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/BenchmarkerPlotsReferenceNoBackground/"
#COMP_REF_DIR="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/BenchmarkerPlotsReferenceForOptimization/"
COMP_REF_TRAINING_DIR="/data_CMS/cms/wind/Mor18References/training/"
COMP_REF_VALIDATION_DIR="/data_CMS/cms/wind/Mor18References/validation/"
COMP_REF_TEST_DIR="/data_CMS/cms/wind/Mor18References/test/"

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
CONTROL_DIR_ORIGINAL="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/AnalysisStep/test/Control/"
BIN_DIR_ORIGINAL="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/bin/slc6_amd64_gcc630/"

# the (common) source directory for this campaign
BIN_DIR=$CAMPAIGN_DIR"bin/"

# the needed part from the C++ sources
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
cp $BIN_DIR_ORIGINAL$BENCHMARKER $BIN_DIR_ORIGINAL$ROC_PLOTTER $BIN_DIR_ORIGINAL$COMPARER $BIN_DIR
cp $CONTROL_DIR_ORIGINAL$PLOT_POSTPROCESSOR $BIN_DIR

cd $CAMPAIGN_DIR

RUN_DIRLIST=`ls -d */ | egrep -v 'bin|statistics'`

# for all the runs in this sweep
for RUN in $RUN_DIRLIST
do
    # make all the necessary directories now
    CALIBRATION_TRAINING_DIR=$CAMPAIGN_DIR$RUN"calibration_training/"
    CALIBRATION_VALIDATION_DIR=$CAMPAIGN_DIR$RUN"calibration_validation/"
    CALIBRATION_TEST_DIR=$CAMPAIGN_DIR$RUN"calibration_test/"

    BENCHMARK_TRAINING_DIR=$CAMPAIGN_DIR$RUN"benchmark_"$ENGINE"_training/"
    BENCHMARK_VALIDATION_DIR=$CAMPAIGN_DIR$RUN"benchmark_"$ENGINE"_validation/"
    BENCHMARK_TEST_DIR=$CAMPAIGN_DIR$RUN"benchmark_"$ENGINE"_test/"

    COMP_TRAINING_DIR=$CAMPAIGN_DIR$RUN"comp_"$ENGINE"_training/"
    COMP_VALIDATION_DIR=$CAMPAIGN_DIR$RUN"comp_"$ENGINE"_validation/"
    COMP_TEST_DIR=$CAMPAIGN_DIR$RUN"comp_"$ENGINE"_test/"

    ROC_DIR=$CAMPAIGN_DIR$RUN"ROCs/"
    AUGMENTATION_DIR=$CAMPAIGN_DIR$RUN"augmentation/"
    BENCHMARK_SETTINGS_DIR=$CAMPAIGN_DIR$RUN"settings_benchmark_"$ENGINE"/"

    mkdir -p $BENCHMARK_SETTINGS_DIR

    mkdir -p $BENCHMARK_TRAINING_DIR
    mkdir -p $BENCHMARK_VALIDATION_DIR
    mkdir -p $BENCHMARK_TEST_DIR

    mkdir -p $COMP_TRAINING_DIR
    mkdir -p $COMP_VALIDATION_DIR
    mkdir -p $COMP_TEST_DIR

    mkdir -p $ROC_DIR

    BENCHMARK_TRAINING_SCRIPT=$BENCHMARK_SETTINGS_DIR"run_benchmark_"$ENGINE"_training.sh"
    BENCHMARK_TRAINING_LOGFILE=$BENCHMARK_SETTINGS_DIR"log_benchmark_"$ENGINE"_training.txt"

    BENCHMARK_VALIDATION_SCRIPT=$BENCHMARK_SETTINGS_DIR"run_benchmark_"$ENGINE"_validation.sh"
    BENCHMARK_VALIDATION_LOGFILE=$BENCHMARK_SETTINGS_DIR"log_benchmark_"$ENGINE"_validation.txt"

    BENCHMARK_TEST_SCRIPT=$BENCHMARK_SETTINGS_DIR"run_benchmark_"$ENGINE"_test.sh"
    BENCHMARK_TEST_LOGFILE=$BENCHMARK_SETTINGS_DIR"log_benchmark_"$ENGINE"_test.txt"
    
    # -------------------------------------------------------
    
    echo "#!/bin/bash" > $BENCHMARK_TRAINING_SCRIPT

    # launch the benchmarking
    echo $BIN_DIR$BENCHMARKER $CALIBRATION_TRAINING_DIR $AUGMENTATION_DIR $BENCHMARK_TRAINING_DIR "0.0 0.5" $ENGINE "&>>" $BENCHMARK_TRAINING_LOGFILE >> $BENCHMARK_TRAINING_SCRIPT
    echo "sleep 5" >> $BENCHMARK_TRAINING_SCRIPT

    # launch the comparison to the reference
    echo $BIN_DIR$COMPARER $BENCHMARK_TRAINING_DIR $COMP_REF_TRAINING_DIR $COMP_TRAINING_DIR "&>>" $BENCHMARK_TRAINING_LOGFILE >> $BENCHMARK_TRAINING_SCRIPT

    # -------------------------------------------------------

    echo "#!/bin/bash" > $BENCHMARK_VALIDATION_SCRIPT

    # launch the benchmarking
    echo $BIN_DIR$BENCHMARKER $CALIBRATION_VALIDATION_DIR $AUGMENTATION_DIR $BENCHMARK_VALIDATION_DIR "0.5 0.75" $ENGINE "&>>" $BENCHMARK_VALIDATION_LOGFILE >> $BENCHMARK_VALIDATION_SCRIPT
    echo "sleep 5" >> $BENCHMARK_VALIDATION_SCRIPT

    # launch the comparison to the reference
    echo $BIN_DIR$COMPARER $BENCHMARK_VALIDATION_DIR $COMP_REF_VALIDATION_DIR $COMP_VALIDATION_DIR "&>>" $BENCHMARK_VALIDATION_LOGFILE >> $BENCHMARK_VALIDATION_SCRIPT

    # -------------------------------------------------------

    echo "#!/bin/bash" > $BENCHMARK_TEST_SCRIPT

    # launch the benchmarking
    echo $BIN_DIR$BENCHMARKER $CALIBRATION_TEST_DIR $AUGMENTATION_DIR $BENCHMARK_TEST_DIR "0.75 1.0" $ENGINE "&>>" $BENCHMARK_TEST_LOGFILE >> $BENCHMARK_TEST_SCRIPT
    echo "sleep 5" >> $BENCHMARK_VALIDATION_SCRIPT

    # launch the comparison to the reference
    echo $BIN_DIR$COMPARER $BENCHMARK_TEST_DIR $COMP_REF_TEST_DIR $COMP_TEST_DIR "&>>" $BENCHMARK_TEST_LOGFILE >> $BENCHMARK_TEST_SCRIPT

    if [ "$(ls -A $ROC_DIR)" ]
    then
	echo "ROCs already computed, skipping this step now"
    else
        # launch the plotting of ROCs
	echo $BIN_DIR$ROC_PLOTTER $AUGMENTATION_DIR $ROC_DIR "&>>" $BENCHMARK_TEST_LOGFILE >> $BENCHMARK_TEST_SCRIPT

	cp $CONTROL_DIR_ORIGINAL$PLOT_POSTPROCESSOR $ROC_DIR
    fi

    # put the plot aggregation scripts into the folders where they are going to be needed
    cp $CONTROL_DIR_ORIGINAL$PLOT_POSTPROCESSOR $CALIBRATION_TRAINING_DIR
    cp $CONTROL_DIR_ORIGINAL$PLOT_POSTPROCESSOR $CALIBRATION_VALIDATION_DIR
    cp $CONTROL_DIR_ORIGINAL$PLOT_POSTPROCESSOR $CALIBRATION_TEST_DIR

done

sleep 1

# now go back and launch all the jobs that have been prepared
cd $CAMPAIGN_DIR

JOBS=`find * | grep run_benchmark_$ENGINE.*.sh$`

for JOB in $JOBS
do
    echo "lauching benchmarking for " $CAMPAIGN_DIR$JOB
    $JOB_SUBMITTER "-short" $CAMPAIGN_DIR$JOB
done

cd $CURRENT_DIR