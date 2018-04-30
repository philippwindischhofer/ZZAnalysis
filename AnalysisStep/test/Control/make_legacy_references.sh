#!/bin/bash

# the output root directory
DEST_ROOT=$1
LUMI=$2

mkdir -p $DEST_ROOT

# this runs the legacy classifier on the (raw) MC files, to have a reference of the classification quality
BENCHMARKER="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/bin/slc6_amd64_gcc630/run_benchmarker_legacy"
JOB_SUBMITTER="/opt/exp_soft/cms/t3/t3submit_new"

# mass points for which references (for training, validation and test) are required
MASS_POINTS="120 124 125 126 130"

for MASS_POINT in $MASS_POINTS
do
    DEST_DIR=$DEST_ROOT$MASS_POINT
    mkdir -p $DEST_DIR

    cd $DEST_DIR
    TRAINING_DIR=$DEST_DIR"/training/"
    VALIDATION_DIR=$DEST_DIR"/validation/"
    TEST_DIR=$DEST_DIR"/test/"

    mkdir -p $TRAINING_DIR
    mkdir -p $VALIDATION_DIR
    mkdir -p $TEST_DIR

    BENCHMARK_TRAINING_SCRIPT=$TRAINING_DIR"run_reference.sh"
    BENCHMARK_VALIDATION_SCRIPT=$VALIDATION_DIR"run_reference.sh"
    BENCHMARK_TEST_SCRIPT=$TEST_DIR"run_reference.sh"

    BENCHMARK_TRAINING_LOGFILE=$TRAINING_DIR"log.txt"
    BENCHMARK_VALIDATION_LOGFILE=$VALIDATION_DIR"log.txt"
    BENCHMARK_TEST_LOGFILE=$TEST_DIR"log.txt"

    echo "#!/bin/bash" > $BENCHMARK_TRAINING_SCRIPT
    echo $BENCHMARKER $TRAINING_DIR "0.0 0.5" $LUMI $MASS_POINT "&>" $BENCHMARK_TRAINING_LOGFILE >> $BENCHMARK_TRAINING_SCRIPT

    echo "#!/bin/bash" > $BENCHMARK_VALIDATION_SCRIPT
    echo $BENCHMARKER $VALIDATION_DIR "0.5 0.75" $LUMI $MASS_POINT "&>" $BENCHMARK_VALIDATION_LOGFILE >> $BENCHMARK_VALIDATION_SCRIPT

    echo "#!/bin/bash" > $BENCHMARK_TEST_SCRIPT
    echo $BENCHMARKER $TEST_DIR "0.75 1.0" $LUMI $MASS_POINT "&>" $BENCHMARK_TEST_LOGFILE >> $BENCHMARK_TEST_SCRIPT
done

cd $DEST_ROOT

JOBS=`find * | grep run_reference.sh$`

for JOB in $JOBS
do
    echo "lauching benchmarking for " $DEST_ROOT$JOB
    $JOB_SUBMITTER "-short" $DEST_ROOT$JOB
done