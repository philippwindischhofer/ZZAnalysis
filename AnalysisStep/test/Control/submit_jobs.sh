#!/bin/bash

CURRENT_DIR=`pwd`
CAMPAIGN_DIR=$1
JOB_NAME=$2
OPTIONS=$3

JOB_SUBMITTER="/opt/exp_soft/cms/t3/t3submit_new"

cd $CAMPAIGN_DIR

JOBS=`find * | grep $JOB_NAME$`

for JOB in $JOBS
do
    echo "launching model benchmark for " $CAMPAIGN_DIR$JOB
    echo $JOB_SUBMITTER"XXX" "$OPTIONS" $CAMPAIGN_DIR$JOB
    #$JOB_SUBMITTER "$OPTIONS" $CAMPAIGN_DIR$JOB
done

cd $CURRENT_DIR
