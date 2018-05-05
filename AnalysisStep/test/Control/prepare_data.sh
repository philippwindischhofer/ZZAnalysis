#!/bin/bash

# the source and destination directories
SOURCE_ROOT=$1
DEST_ROOT=$2

JOB_SUBMITTER="/opt/exp_soft/cms/t3/t3submit_new"
FILE_PREPARER="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/bin/slc6_amd64_gcc630/run_augment_single_tree"

SOURCE_FILE_NAME="ZZ4lAnalysis.root"

cd $SOURCE_ROOT
SOURCE_FOLDERS=`ls -d */`

for SOURCE_FOLDER in $SOURCE_FOLDERS
do
    SOURCE_PATH=$SOURCE_ROOT$SOURCE_FOLDER$SOURCE_FILE_NAME
    DEST_PATH=$DEST_ROOT$SOURCE_FOLDER$SOURCE_FILE_NAME
    
    # create the destination directory tree
    mkdir -p $DEST_ROOT$SOURCE_FOLDER

    PREPARATION_SCRIPT=$DEST_ROOT"run_preparation_"${SOURCE_FOLDER%/}".sh"
    PREPARATION_LOGFILE=$DEST_ROOT"log_preparation_"${SOURCE_FOLDER%/}".txt"

    echo "#!/bin/bash" > $PREPARATION_SCRIPT
    echo "source /home/llr/cms/wind/se.sh" >> $PREPARATION_SCRIPT
    echo $FILE_PREPARER $SOURCE_PATH $DEST_PATH "1" "&>" $PREPARATION_LOGFILE >> $PREPARATION_SCRIPT
done

cd $DEST_ROOT
JOBS=`find * | grep run_preparation.*.sh$`

for JOB in $JOBS
do
    echo "launching " $DEST_ROOT$JOB
    $JOB_SUBMITTER "-short" $DEST_ROOT$JOB
done
