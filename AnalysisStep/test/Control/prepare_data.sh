#!/bin/bash

if [[ -z "$CMSSW_BASE" ]]; then
    echo "ERROR: need to have CMSSW_BASE set! Did you forget to run 'cmsenv'?"
    exit
fi

# ---------------------------------------------
#  parse the given arguments
# ---------------------------------------------
POSARG=()

while [[ $# -gt 0 ]]
do
key=$1

case $key in
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

JOB_SUBMITTER="/opt/exp_soft/cms/t3/t3submit_new"
FILE_PREPARER=$CMSSW_BASE"/bin/slc6_amd64_gcc630/run_augment_single_tree"

SOURCE_FILE_NAME="ZZ4lAnalysis.root"

cd $SOURCE_ROOT
SOURCE_FOLDERS=`ls -d */`

mkdir -p $DEST_ROOT

for SOURCE_FOLDER in $SOURCE_FOLDERS
do
    SOURCE_PATH=$SOURCE_ROOT$SOURCE_FOLDER$SOURCE_FILE_NAME
    DEST_PATH=$DEST_ROOT$SOURCE_FOLDER$SOURCE_FILE_NAME
    
    # create the destination directory tree
    mkdir -p $DEST_ROOT$SOURCE_FOLDER

    PREPARATION_SCRIPT=$DEST_ROOT"run_preparation_"${SOURCE_FOLDER%/}".sh"
    PREPARATION_LOGFILE=$DEST_ROOT"log_preparation_"${SOURCE_FOLDER%/}".txt"

    echo "#!/bin/bash" > $PREPARATION_SCRIPT
    echo $FILE_PREPARER $SOURCE_PATH $DEST_PATH "1" "&>" $PREPARATION_LOGFILE >> $PREPARATION_SCRIPT
done

cd $DEST_ROOT
JOBS=`find * | grep run_preparation.*.sh$`

for JOB in $JOBS
do
    echo "launching " $DEST_ROOT$JOB
    until $JOB_SUBMITTER "-short" $DEST_ROOT$JOB
    do
	echo "----------------------------------------------------------------"
    	echo " error submitting job, retrying ..."
    	echo "----------------------------------------------------------------"
    	sleep 1
    done
done
