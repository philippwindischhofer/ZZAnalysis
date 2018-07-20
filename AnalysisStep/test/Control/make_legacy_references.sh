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
    --lumi)
    LUMI="$2"
    shift
    shift
    ;;
    --source)
    SOURCE_ROOT="$2"
    shift
    shift
    ;;
    --dest)
    DEST_ROOT="$2"
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

BENCHMARKER=$CMSSW_BASE"/bin/slc6_amd64_gcc630/run_benchmarker_legacy"
JOB_SUBMITTER="/opt/exp_soft/cms/t3/t3submit_new"

mkdir -p $DEST_ROOT

# mass points for which references (for training, validation and test) are required
MASS_POINTS="120 125 130"
TRAINING_MASS_POINT="125"

for MASS_POINT in $MASS_POINTS
do
    DEST_DIR=$DEST_ROOT$MASS_POINT
    mkdir -p $DEST_DIR

    cd $DEST_DIR

    if [ "$MASS_POINT" = "$TRAINING_MASS_POINT" ]
    then
	# this is the mass point for which the training was done; need to split it into training, validation and test dataset
	VALIDATION_DIR=$DEST_DIR"/validation/"
	TEST_DIR=$DEST_DIR"/test/"

	mkdir -p $VALIDATION_DIR
	mkdir -p $TEST_DIR

	BENCHMARK_VALIDATION_SCRIPT=$VALIDATION_DIR"run_reference.sh"
	BENCHMARK_TEST_SCRIPT=$TEST_DIR"run_reference.sh"

	BENCHMARK_VALIDATION_LOGFILE=$VALIDATION_DIR"log.txt"
	BENCHMARK_TEST_LOGFILE=$TEST_DIR"log.txt"

	echo "#!/bin/bash" > $BENCHMARK_VALIDATION_SCRIPT
	echo $BENCHMARKER $SOURCE_ROOT"/validation/" $VALIDATION_DIR "0.0" "1.0" $LUMI $MASS_POINT "&>" $BENCHMARK_VALIDATION_LOGFILE >> $BENCHMARK_VALIDATION_SCRIPT

	echo "#!/bin/bash" > $BENCHMARK_TEST_SCRIPT
	echo $BENCHMARKER $SOURCE_ROOT"/test/" $TEST_DIR "0.0" "1.0" $LUMI $MASS_POINT "&>" $BENCHMARK_TEST_LOGFILE >> $BENCHMARK_TEST_SCRIPT
    else
	# this is a mass point that was not used for training, do not have to respect any splitting
	BENCHMARK_SCRIPT=$DEST_DIR"/run_reference.sh"
	BENCHMARK_LOGFILE=$DEST_DIR"/log.txt"

	echo "#!/bin/bash" > $BENCHMARK_SCRIPT
	echo $BENCHMARKER $SOURCE_ROOT"/test/" $DEST_DIR"/" "0.0" "1.0" $LUMI $MASS_POINT "&>" $BENCHMARK_LOGFILE >> $BENCHMARK_SCRIPT
    fi
done

cd $DEST_ROOT

JOBS=`find * | grep run_reference.sh$`

for JOB in $JOBS
do
    echo "lauching benchmarking for " $DEST_ROOT$JOB
    until $JOB_SUBMITTER "-short" $DEST_ROOT$JOB
    do
    	echo "----------------------------------------------------------------"
    	echo " error submitting job, retrying ..."
    	echo "----------------------------------------------------------------"
    	sleep 1
    done
done