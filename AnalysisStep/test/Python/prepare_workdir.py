import sys
import os
import subprocess as sp

from trainlib.ConfigFileHandler import ConfigFileHandler
from trainlib.ConfigFileUtils import ConfigFileUtils

def main():
    if len(sys.argv) != 2:
        print "Error: exactly 1 argument is required!"

    dest_path = sys.argv[1]

    # global settings:
    source_path = "/data_CMS/cms/wind/CJLST_NTuples_prepared/"
    bin_dir = "/home/llr/cms/wind/cmssw/CMSSW_9_4_2/bin/slc6_amd64_gcc630/"
    scrambler = os.path.join(bin_dir, "run_scrambler")
    chunk_extractor = os.path.join(bin_dir, "run_chunk_extractor")

    # create the needed folders:
    train_dir = os.path.join(dest_path, "training/")
    validation_dir = os.path.join(dest_path, "validation/")
    test_dir = os.path.join(dest_path, "test/")
    trainval_dir = os.path.join(dest_path, "trainval/")
    temp_dir = os.path.join(dest_path, "temp/")

    # create these directories
    if not os.path.exists(train_dir):
        os.makedirs(train_dir)
    
    if not os.path.exists(validation_dir):
        os.makedirs(validation_dir)
    
    if not os.path.exists(test_dir):
        os.makedirs(test_dir)
    
    if not os.path.exists(trainval_dir):
        os.makedirs(trainval_dir)
    
    if not os.path.exists(temp_dir):
        os.makedirs(temp_dir)

    settings_path = os.path.join(dest_path, "settings.conf")

    confhandler = ConfigFileHandler()
    confhandler.load_configuration(settings_path)

    # load global settings from the configuration file
    root_file_name = confhandler.get_field("Global", "root_file_name")
    source_dir = confhandler.get_field("Global", "source_dir")

    training_files = [cur_file for cur_file in confhandler.get_sections() if "Global" not in cur_file]
    available_files = next(os.walk(source_path))[1]
    used_files = []
    
    for training_file in training_files:
        sect = confhandler.get_section(training_file)
    
        print "--------------------------------------------------"
        print "currently splitting: " + training_file
    
        source_files = ConfigFileUtils.parse_list(sect["source"], lambda x: x)
        train_val_splits = ConfigFileUtils.parse_list(sect["train_val_split"], lambda x: float(x))
        val_test_splits = ConfigFileUtils.parse_list(sect["val_test_split"], lambda x: float(x))
    
        # first split the needed files into 3 pieces, as dictated by the splits read from the config file
        for source_file, train_val_split, val_test_split in zip(source_files, train_val_splits, val_test_splits):
        
            print "extracting 0.0 - " + str(train_val_split) + " from " + source_file
        
            dest_dir = os.path.join(train_dir, source_file)
            if not os.path.exists(dest_dir):
                os.makedirs(dest_dir)
    
            output = sp.check_output([chunk_extractor, os.path.join(source_path, source_file, root_file_name),
                                      os.path.join(dest_dir, root_file_name), str(0.0), str(train_val_split)])      
            print output
        
            print "-- -- -- -- -- -- -- -- -- -- -- --"
        
            print "extracting " + str(train_val_split) + " - " + str(val_test_split) + " from " + source_file
        
            dest_dir = os.path.join(validation_dir, source_file)
            if not os.path.exists(dest_dir):
                os.makedirs(dest_dir)
    
            output = sp.check_output([chunk_extractor, os.path.join(source_path, source_file, root_file_name),
                                      os.path.join(dest_dir, root_file_name), str(train_val_split), str(val_test_split)])      
            print output
        
            print "-- -- -- -- -- -- -- -- -- -- -- --"
        
            print "extracting " + str(val_test_split) + " - 1.0 from " + source_file
        
            dest_dir = os.path.join(test_dir, source_file)
            if not os.path.exists(dest_dir):
                os.makedirs(dest_dir)
    
            output = sp.check_output([chunk_extractor, os.path.join(source_path, source_file, root_file_name),
                                      os.path.join(dest_dir, root_file_name), str(val_test_split), str(1.0)])      
            print output
        
            used_files.append(source_file)
    
        print "--------------------------------------------------"

    unused_files = [cur_file for cur_file in available_files if cur_file not in used_files]

    # copy all unused files (i.e. those that will never end up in some combined training dataset) directly into the test folder
    for unused_file in unused_files:
        dest_dir = test_dir
        source_dir = os.path.join(source_path, unused_file)
    
        print "cp -r " + source_dir + " " + dest_dir
        output = sp.check_output(["cp", "-r", source_dir, dest_dir])      
        print output

    # now have all the needed files split apart, can now proceed to combine them into the training 
    # datasets that will end up in trainval
    for training_file in training_files:
        print "now building training dataset: " + training_file
        sect = confhandler.get_section(training_file)
        source_folders = ConfigFileUtils.parse_list(sect["source"], lambda x: x)
    
        for mode in ["training", "validation"]:

            temp_dest_folder = os.path.join(dest_path, temp_dir, training_file, mode)
            temp_dest_file = os.path.join(temp_dest_folder, root_file_name)

            if not os.path.exists(temp_dest_folder):
                os.makedirs(temp_dest_folder)

            source_files = [os.path.join(dest_path, mode, cur_file, root_file_name) for cur_file in source_folders]

            print "hadd " + temp_dest_file + " " + " ".join(source_files)
            output = sp.check_output(["hadd", temp_dest_file] + source_files)      
            print output
    
            temp_scrambled_folder = os.path.join(dest_path, temp_dir, "scrambled", training_file, mode)
            if not os.path.exists(temp_scrambled_folder):
                os.makedirs(temp_scrambled_folder)
            
            temp_scrambled_file = os.path.join(temp_scrambled_folder, root_file_name)
        
            print scrambler + " " + temp_dest_file + " " + temp_scrambled_file
            output = sp.check_output([scrambler, temp_dest_file, temp_scrambled_file])      
            print output
        
        trainval_dest_folder = os.path.join(trainval_dir, training_file)
        if not os.path.exists(trainval_dest_folder):
            os.makedirs(trainval_dest_folder)
        
        print "hadd " + os.path.join(trainval_dest_folder, root_file_name) + " " + os.path.join(dest_path, temp_dir, "scrambled", training_file, "training", root_file_name) + " " + os.path.join(dest_path, temp_dir, "scrambled", training_file, "validation", root_file_name)
        
        output = sp.check_output(["hadd", os.path.join(trainval_dest_folder, root_file_name),
                                 os.path.join(dest_path, temp_dir, "scrambled", training_file, "training", root_file_name),
                                 os.path.join(dest_path, temp_dir, "scrambled", training_file, "validation", root_file_name)])
        print output

        print "done."
        
    
if __name__ == "__main__":
    main()
