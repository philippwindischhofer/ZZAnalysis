import sys
import os
import subprocess as sp
import numpy as np
import uuid

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

    settings_path = os.path.join(dest_path, "settings.conf")

    confhandler = ConfigFileHandler()
    confhandler.load_configuration(settings_path)

    # load global settings from the configuration file
    root_file_name = confhandler.get_field("Global", "root_file_name")
    source_dir = confhandler.get_field("Global", "source_dir")
    chunk_size = int(confhandler.get_field("Global", "chunk_size"))

    def submit_job(cmd_dir, command):
        job_submitter = "/opt/exp_soft/cms/t3/t3submit_new"

        filename = str(uuid.uuid4()) + ".sh"
        file_path = os.path.join(cmd_dir, filename)
        with open(file_path, "w") as cmd_file:
            cmd_file.write("#!/bin/bash\n")
            cmd_file.write(command)

        output = sp.check_output([job_submitter, "-short", file_path])
        print output

    def chunk_file(in_dir, out_root, base_name, number_chunks, cmd_dir):
        splits = np.linspace(0, 1, number_chunks)
        in_file = os.path.join(in_dir, root_file_name)

        if number_chunks == 1:
            out_folder = os.path.join(out_root, base_name + "_chunk_0/")

            if not os.path.exists(out_folder):
                os.makedirs(out_folder)

            out_file = os.path.join(out_folder, root_file_name)

            command = " ".join([chunk_extractor, in_file, out_file, str(0.0), str(1.0), str(0)])
            submit_job(cmd_dir, command)
            print command

        else:
            for i in range(len(splits) - 1):
                start_split = splits[i]
                end_split = splits[i + 1]
            
                out_folder = os.path.join(out_root, base_name + "_chunk_" + str(i) + "/")
                if not os.path.exists(out_folder):
                    os.makedirs(out_folder)

                out_file = os.path.join(out_folder, root_file_name)
                
                command = " ".join([chunk_extractor, in_file, out_file, str(start_split), str(end_split), str(0)])
                submit_job(cmd_dir, command)
                print command

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

    # for all files that are not used for training, split them 50:50 into validation and test ...
    for unused_file in unused_files:
        source_dir = os.path.join(source_path, unused_file)

        # ... unless they are only needed to assess systematics, i.e. are not going to be used at all during the validation step
        if "ext" in unused_file or "tuneup" in unused_file or "tunedown" in unused_file:
            print "extracting 0.0 - 1.0 from " + unused_file

            dest_dir = os.path.join(test_dir, unused_file)
            if not os.path.exists(dest_dir):
                os.makedirs(dest_dir)
                
            output = sp.check_output([chunk_extractor, os.path.join(source_dir, root_file_name),
                                      os.path.join(dest_dir, root_file_name), str(0.0), str(1.0)])      
            print output

        else:
            print "extracting 0.0 - 0.5 from " + unused_file
            
            dest_dir = os.path.join(validation_dir, unused_file)
            if not os.path.exists(dest_dir):
                os.makedirs(dest_dir)
    
            output = sp.check_output([chunk_extractor, os.path.join(source_dir, root_file_name),
                                      os.path.join(dest_dir, root_file_name), str(0.0), str(0.5)])      
            print output

            print "-- -- -- -- -- -- -- -- -- -- -- --"

            print "extracting 0.5 - 1.0 from " + unused_file
            
            dest_dir = os.path.join(test_dir, unused_file)
            if not os.path.exists(dest_dir):
                os.makedirs(dest_dir)
    
            output = sp.check_output([chunk_extractor, os.path.join(source_dir, root_file_name),
                                      os.path.join(dest_dir, root_file_name), str(0.5), str(1.0)])      
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

    # call here the splitting of the files for systematics into multiple components
    

    # at the end, chunk the ROOT files into many smaller ones, to keep the augmentation time short
    train_chunks_dir = os.path.join(dest_path, "training_chunks/")
    validation_chunks_dir = os.path.join(dest_path, "validation_chunks/")
    test_chunks_dir = os.path.join(dest_path, "test_chunks/")

    # create these directories
    if not os.path.exists(train_chunks_dir):
        os.makedirs(train_chunks_dir)
    
    if not os.path.exists(validation_chunks_dir):
        os.makedirs(validation_chunks_dir)
    
    if not os.path.exists(test_chunks_dir):
        os.makedirs(test_chunks_dir)

    for mode in ["training", "validation", "test"]:
        # look at each file individually and put it into chunks
        cur_dir = os.path.join(dest_path, mode)
        available_folders = next(os.walk(cur_dir))[1]

        for available_folder in available_folders:
            available_file = os.path.join(cur_dir, available_folder, root_file_name)

            number_chunks = max(1, os.path.getsize(available_file) / chunk_size)

            print "now splitting file " + available_file + " into " + str(number_chunks) + " chunks"

            out_root = os.path.join(dest_path, mode + "_chunks")
            
            chunk_file(os.path.join(dest_path, mode, available_folder), out_root, available_folder, number_chunks, temp_dir)
        
    print "done."        
    
if __name__ == "__main__":
    main()
