#!/bin/bash

# mass point on which the classifier was trained
TRAINING_MASS_POINT="125"

# ---------------------------------------------
#  global settings
# ---------------------------------------------
CURRENT_DIR=`pwd`
CAMPAIGN_DIR=$1
ENGINE=$2
MASS_POINT=$3
PRIOR_FILE=$4

if [ -z $ENGINE ]
then
    echo "no engine name provided, using default: tree"
    ENGINE="tree"
fi

if [ -z $PRIOR_FILE ]
then
    echo "no prior file provided, using default: priors.txt"
    PRIOR_FILE="priors.txt"
fi

if [ -z $MASS_POINT ]
then
    echo "no mass point provided, using default: "$TRAINING_MASS_POINT"GeV"
    MASS_POINT=$TRAINING_MASS_POINT
fi

# ---------------------------------------------
#  settings for this training campaign
# ---------------------------------------------
COMP_REF_TRAINING_DIR="/data_CMS/cms/wind/Mor18References/"$MASS_POINT"/training/"
COMP_REF_VALIDATION_DIR="/data_CMS/cms/wind/Mor18References/"$MASS_POINT"/validation/"
COMP_REF_TEST_DIR="/data_CMS/cms/wind/Mor18References/"$MASS_POINT"/test/"
COMP_REF_DIR="/data_CMS/cms/wind/Mor18References/"$MASS_POINT"/"

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
    CALIBRATION_TRAINING_DIR=$CAMPAIGN_DIR$RUN"calibration_training/"
    CALIBRATION_VALIDATION_DIR=$CAMPAIGN_DIR$RUN"calibration_validation/"
    CALIBRATION_TEST_DIR=$CAMPAIGN_DIR$RUN"calibration_test/"

    if [ "$MASS_POINT" = "$TRAINING_MASS_POINT" ]
    then

	BENCHMARK_VALIDATION_DIR=$CAMPAIGN_DIR$RUN"benchmark_priors_"$ENGINE"/"$MASS_POINT"/validation/"
	BENCHMARK_TEST_DIR=$CAMPAIGN_DIR$RUN"benchmark_priors_"$ENGINE"/"$MASS_POINT"/test/"

	COMP_VALIDATION_DIR=$CAMPAIGN_DIR$RUN"comp_priors_"$ENGINE"/"$MASS_POINT"/validation/"
	COMP_TEST_DIR=$CAMPAIGN_DIR$RUN"comp_priors_"$ENGINE"/"$MASS_POINT"/test/"

	PRIOR_DIR=$CAMPAIGN_DIR$RUN"priors_"$ENGINE"/"

	AUGMENTATION_VALIDATION_DIR=$CAMPAIGN_DIR$RUN"augmentation_validation/"
	AUGMENTATION_TEST_DIR=$CAMPAIGN_DIR$RUN"augmentation_test/"

	BENCHMARK_SETTINGS_DIR=$CAMPAIGN_DIR$RUN"settings_benchmark_priors_"$ENGINE"/"$MASS_POINT"/"

	mkdir -p $BENCHMARK_SETTINGS_DIR

	mkdir -p $BENCHMARK_VALIDATION_DIR
	mkdir -p $BENCHMARK_TEST_DIR

	mkdir -p $COMP_VALIDATION_DIR
	mkdir -p $COMP_TEST_DIR

	BENCHMARK_VALIDATION_SCRIPT=$BENCHMARK_SETTINGS_DIR"run_benchmark_priors_"$ENGINE"_validation.sh"
	BENCHMARK_VALIDATION_LOGFILE=$BENCHMARK_SETTINGS_DIR"log_benchmark_priors_"$ENGINE"_validation.txt"

	BENCHMARK_TEST_SCRIPT=$BENCHMARK_SETTINGS_DIR"run_benchmark_priors_"$ENGINE"_test.sh"
	BENCHMARK_TEST_LOGFILE=$BENCHMARK_SETTINGS_DIR"log_benchmark_priors_"$ENGINE"_test.txt"

	echo "#!/bin/bash" > $BENCHMARK_VALIDATION_SCRIPT

        # launch the benchmarking
	echo $BIN_DIR$BENCHMARKER $CALIBRATION_VALIDATION_DIR $AUGMENTATION_VALIDATION_DIR $MASS_POINT $BENCHMARK_VALIDATION_DIR "0.0 1.0" $ENGINE $PRIOR_DIR$PRIOR_FILE "&>" $BENCHMARK_VALIDATION_LOGFILE >> $BENCHMARK_VALIDATION_SCRIPT
        echo "sleep 5" >> $BENCHMARK_VALIDATION_SCRIPT

        # launch the comparison to the reference
	echo $BIN_DIR$COMPARER $BENCHMARK_VALIDATION_DIR $COMP_REF_VALIDATION_DIR $COMP_VALIDATION_DIR "&>>" $BENCHMARK_VALIDATION_LOGFILE >> $BENCHMARK_VALIDATION_SCRIPT

        # ------------------------------------------------------------

	echo "#!/bin/bash" > $BENCHMARK_TEST_SCRIPT

        # this also uses the validation calibration for the evaluation on the test set! (since pretend to not know the truth on the test set, can not do any calibration...)
	echo $BIN_DIR$BENCHMARKER $CALIBRATION_VALIDATION_DIR $AUGMENTATION_TEST_DIR $MASS_POINT $BENCHMARK_TEST_DIR "0.0 1.0" $ENGINE $PRIOR_DIR$PRIOR_FILE "&>" $BENCHMARK_TEST_LOGFILE >> $BENCHMARK_TEST_SCRIPT
        echo "sleep 5" >> $BENCHMARK_TEST_SCRIPT

        # launch the comparison to the reference
	echo $BIN_DIR$COMPARER $BENCHMARK_TEST_DIR $COMP_REF_TEST_DIR $COMP_TEST_DIR "&>>" $BENCHMARK_TEST_LOGFILE >> $BENCHMARK_TEST_SCRIPT
    else
	# do not need to respect the training/validation/test split in any way now
	BENCHMARK_DIR=$CAMPAIGN_DIR$RUN"benchmark_priors_"$ENGINE"/"$MASS_POINT"/"	
	COMP_DIR=$CAMPAIGN_DIR$RUN"comp_priors_"$ENGINE"/"$MASS_POINT"/"

	PRIOR_DIR=$CAMPAIGN_DIR$RUN"priors_"$ENGINE"/"

	AUGMENTATION_DIR=$CAMPAIGN_DIR$RUN"augmentation_test/"
	BENCHMARK_SETTINGS_DIR=$CAMPAIGN_DIR$RUN"settings_benchmark_priors_"$ENGINE"/"$MASS_POINT"/"

	mkdir -p $BENCHMARK_SETTINGS_DIR
	mkdir -p $BENCHMARK_DIR
	mkdir -p $COMP_DIR

	BENCHMARK_SCRIPT=$BENCHMARK_SETTINGS_DIR"run_benchmark_priors_"$ENGINE"_validation.sh"
	BENCHMARK_LOGFILE=$BENCHMARK_SETTINGS_DIR"log_benchmark_priors_"$ENGINE"_validation.txt"

	echo "#!/bin/bash" > $BENCHMARK_SCRIPT

        # launch the benchmarking
	echo $BIN_DIR$BENCHMARKER $CALIBRATION_VALIDATION_DIR $AUGMENTATION_DIR $MASS_POINT $BENCHMARK_DIR "0.0 1.0" $ENGINE $PRIOR_DIR$PRIOR_FILE "&>" $BENCHMARK_LOGFILE >> $BENCHMARK_SCRIPT
	echo "sleep 5" >> $BENCHMARK_SCRIPT

        # launch the comparison to the reference
	echo $BIN_DIR$COMPARER $BENCHMARK_DIR $COMP_REF_DIR $COMP_DIR "&>>" $BENCHMARK_LOGFILE >> $BENCHMARK_SCRIPT	
    fi
done

sleep 1

# now go back and launch all the jobs that have been prepared
cd $CAMPAIGN_DIR

JOBS=`find * | grep $MASS_POINT.*run_benchmark_priors_$ENGINE.*.sh$`

for JOB in $JOBS
do
    echo "lauching benchmarking for " $CAMPAIGN_DIR$JOB
    $JOB_SUBMITTER "-short" $CAMPAIGN_DIR$JOB
    #sh $CAMPAIGN_DIR$JOB &
done

cd $CURRENT_DIR