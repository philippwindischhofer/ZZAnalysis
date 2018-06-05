#!/bin/bash

CAMPAIGN_DIR=$1

CONTROL_DIR_ORIGINAL="/home/llr/cms/wind/cmssw/CMSSW_9_4_2/src/ZZAnalysis/AnalysisStep/test/Control/"
SUBMIT_AUGMENTATION="run_distributed_augmentation.sh"

#MC_DIR="/data_CMS/cms/wind/CJLST_NTuples_ZXonly/"
#MC_DIR="/data_CMS/cms/wind/CJLST_NTuples_ZXonly_013/"
#MC_DIR="/data_CMS/cms/wind/CJLST_NTuples_ZXonly_016/"
#MC_DIR="/data_CMS/cms/wind/CJLST_NTuples_ZXonly_022/"
#MC_DIR="/data_CMS/cms/wind/CJLST_NTuples_ZXonly_033/"
#MC_DIR="/data_CMS/cms/wind/CJLST_NTuples/"
#MC_DIR="/data_CMS/cms/wind/CJLST_NTuples_ZX_qq_013/"
#MC_DIR="/data_CMS/cms/wind/CJLST_NTuples_ZX_qq/"
#MC_DIR="/data_CMS/cms/wind/CJLST_NTuples_ZX_qq_syst/"
#MC_DIR="/data_CMS/cms/wind/CJLST_NTuples_missing_systematics/"
MC_DIR="/data_CMS/cms/wind/CJLST_NTuples_with_systematics/"
#MC_DIR="/data_CMS/cms/wind/CJLST_NTuples_ZX_qq_syst_tuneupdn/"

PARTS="training_chunks validation_chunks test_chunks"

cd $CAMPAIGN_DIR
RUN_DIRLIST=`ls -d */ | egrep -v 'bin|statistics'`

# for all the runs in this sweep
for RUN in $RUN_DIRLIST
do
    for part in $PARTS
    do
	echo "starting augmentation for: " $part
	sh $CONTROL_DIR_ORIGINAL$SUBMIT_AUGMENTATION $CAMPAIGN_DIR $MC_DIR$part"/" $CAMPAIGN_DIR$RUN"/augmentation_"$part"/" $CAMPAIGN_DIR$RUN"/settings_augmentation_"$part"/" $CAMPAIGN_DIR$RUN"/"
    done
done