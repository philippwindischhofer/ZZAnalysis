#!/bin/bash

# mass point on which the classifier was trained
TRAINING_MASS_POINT="125"

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
    --mass)
    MASS_POINT="$2"
    shift
    shift
    ;;
    --ref)
    COMP_REF_DIR="$2/"
    shift
    shift
    ;;
    --lumi)
    LUMI="$2"
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
CAMPAIGN_DIR=$1"/"

if [ -z $ENGINE ]
then
    echo "no engine name provided, using default: tree"
    ENGINE="tree"
fi

if [ -z $MASS_POINT ]
then
    echo "no mass point provided, using default: "$TRAINING_MASS_POINT"GeV"
    MASS_POINT=$TRAINING_MASS_POINT
fi

if [ -z $COMP_REF_DIR ]
then
    echo "ERROR: no reference provided - can not produce Punzi plots!"
fi

# ---------------------------------------------
#  settings for this training campaign
# ---------------------------------------------
COMP_REF_TRAINING_DIR=$COMP_REF_DIR$MASS_POINT"/training/"
COMP_REF_VALIDATION_DIR=$COMP_REF_DIR$MASS_POINT"/validation/"
COMP_REF_TEST_DIR=$COMP_REF_DIR$MASS_POINT"/test/"
COMP_REF_DIR=$COMP_REF_DIR$MASS_POINT"/"

# the directories where the original sources are located
CONTROL_DIR=$CMSSW_BASE"/src/ZZAnalysis/AnalysisStep/test/Control/"
BIN_DIR=$CMSSW_BASE"/bin/slc6_amd64_gcc630/"

# the needed part from the C++ sources
BENCHMARKER="run_benchmarker"
ROC_PLOTTER="run_roc_plotter"
COMPARER="run_comp"

cd $CAMPAIGN_DIR

RUN_DIRLIST=`ls -d */ | egrep -v 'bin|statistics'`

# for all the runs in this sweep
for RUN in $RUN_DIRLIST
do
    # make all the necessary directories now
    CALIBRATION_TRAINING_DIR=$CAMPAIGN_DIR$RUN"calibration_training/"
    CALIBRATION_VALIDATION_DIR=$CAMPAIGN_DIR$RUN"calibration_validation/"
    CALIBRATION_TEST_DIR=$CAMPAIGN_DIR$RUN"calibration_test/"

    if [ "$MASS_POINT" = "$TRAINING_MASS_POINT" ]
    then

	# need to take extra care: split everything into training, validation and test dataset!
	BENCHMARK_VALIDATION_DIR=$CAMPAIGN_DIR$RUN"benchmark_"$ENGINE"/"$MASS_POINT"/validation/"
	BENCHMARK_TEST_DIR=$CAMPAIGN_DIR$RUN"benchmark_"$ENGINE"/"$MASS_POINT"/test/"

	COMP_VALIDATION_DIR=$CAMPAIGN_DIR$RUN"comp_"$ENGINE"/"$MASS_POINT"/validation/"
	COMP_TEST_DIR=$CAMPAIGN_DIR$RUN"comp_"$ENGINE"/"$MASS_POINT"/test/"

	AUGMENTATION_VALIDATION_DIR=$CAMPAIGN_DIR$RUN"augmentation_validation/"
	AUGMENTATION_TEST_DIR=$CAMPAIGN_DIR$RUN"augmentation_test/"

	BENCHMARK_SETTINGS_DIR=$CAMPAIGN_DIR$RUN"settings_benchmark_"$ENGINE"/"$MASS_POINT"/"

	mkdir -p $BENCHMARK_SETTINGS_DIR

	mkdir -p $BENCHMARK_VALIDATION_DIR
	mkdir -p $BENCHMARK_TEST_DIR

	mkdir -p $COMP_VALIDATION_DIR
	mkdir -p $COMP_TEST_DIR

	BENCHMARK_VALIDATION_SCRIPT=$BENCHMARK_SETTINGS_DIR"run_benchmark_"$ENGINE"_validation.sh"
	BENCHMARK_VALIDATION_LOGFILE=$BENCHMARK_SETTINGS_DIR"log_benchmark_"$ENGINE"_validation.txt"

	BENCHMARK_TEST_SCRIPT=$BENCHMARK_SETTINGS_DIR"run_benchmark_"$ENGINE"_test.sh"
	BENCHMARK_TEST_LOGFILE=$BENCHMARK_SETTINGS_DIR"log_benchmark_"$ENGINE"_test.txt"
	
        # -------------------------------------------------------

	echo "#!/bin/bash" > $BENCHMARK_VALIDATION_SCRIPT

        # launch the benchmarking
	echo $BIN_DIR$BENCHMARKER $CALIBRATION_VALIDATION_DIR $AUGMENTATION_VALIDATION_DIR $MASS_POINT $BENCHMARK_VALIDATION_DIR "0.0" "1.0" $LUMI $ENGINE "&>" $BENCHMARK_VALIDATION_LOGFILE >> $BENCHMARK_VALIDATION_SCRIPT

	echo "sleep 5" >> $BENCHMARK_VALIDATION_SCRIPT

        # launch the comparison to the reference
	echo $BIN_DIR$COMPARER $BENCHMARK_VALIDATION_DIR $COMP_REF_VALIDATION_DIR $COMP_VALIDATION_DIR $LUMI "&>>" $BENCHMARK_VALIDATION_LOGFILE >> $BENCHMARK_VALIDATION_SCRIPT

        # -------------------------------------------------------

	echo "#!/bin/bash" > $BENCHMARK_TEST_SCRIPT

        # launch the benchmarking
	echo $BIN_DIR$BENCHMARKER $CALIBRATION_VALIDATION_DIR $AUGMENTATION_TEST_DIR $MASS_POINT $BENCHMARK_TEST_DIR "0.0" "1.0" $LUMI $ENGINE "&>" $BENCHMARK_TEST_LOGFILE >> $BENCHMARK_TEST_SCRIPT
	echo "sleep 5" >> $BENCHMARK_TEST_SCRIPT

        # launch the comparison to the reference
	echo $BIN_DIR$COMPARER $BENCHMARK_TEST_DIR $COMP_REF_TEST_DIR $COMP_TEST_DIR $LUMI "&>>" $BENCHMARK_TEST_LOGFILE >> $BENCHMARK_TEST_SCRIPT
    else
	# are benchmarking on a mass point that is different from where the classifier was trained
	BENCHMARK_DIR=$CAMPAIGN_DIR$RUN"benchmark_"$ENGINE"/"$MASS_POINT"/"

	COMP_DIR=$CAMPAIGN_DIR$RUN"comp_"$ENGINE"/"$MASS_POINT"/"

	AUGMENTATION_DIR=$CAMPAIGN_DIR$RUN"augmentation_test/"
	BENCHMARK_SETTINGS_DIR=$CAMPAIGN_DIR$RUN"settings_benchmark_"$ENGINE"/"$MASS_POINT"/"

	mkdir -p $BENCHMARK_SETTINGS_DIR
	mkdir -p $BENCHMARK_DIR
	mkdir -p $COMP_DIR

	BENCHMARK_SCRIPT=$BENCHMARK_SETTINGS_DIR"run_benchmark_"$ENGINE".sh"
	BENCHMARK_LOGFILE=$BENCHMARK_SETTINGS_DIR"log_benchmark_"$ENGINE".txt"

	echo "#!/bin/bash" > $BENCHMARK_SCRIPT

        # launch the benchmarking
	echo $BIN_DIR$BENCHMARKER $CALIBRATION_VALIDATION_DIR $AUGMENTATION_DIR $MASS_POINT $BENCHMARK_DIR "0.0 1.0" $LUMI $ENGINE "&>" $BENCHMARK_LOGFILE >> $BENCHMARK_SCRIPT
	echo "sleep 5" >> $BENCHMARK_SCRIPT

        # launch the comparison to the reference
	echo $BIN_DIR$COMPARER $BENCHMARK_DIR $COMP_REF_DIR $COMP_DIR $LUMI "&>>" $BENCHMARK_LOGFILE >> $BENCHMARK_SCRIPT	
    fi
done

sleep 1

# now go back and launch all the jobs that have been prepared
cd $CAMPAIGN_DIR

JOBS=`find * | grep $MASS_POINT.*run_benchmark_$ENGINE.*.sh$`

for JOB in $JOBS
do
    echo "lauching benchmarking for " $CAMPAIGN_DIR$JOB
    $JOB_SUBMITTER "-short" $CAMPAIGN_DIR$JOB
done

cd $CURRENT_DIR