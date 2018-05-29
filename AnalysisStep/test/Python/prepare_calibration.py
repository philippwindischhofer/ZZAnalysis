import sys
import os
import subprocess as sp
import re

from trainlib.ConfigFileHandler import ConfigFileHandler
from trainlib.ConfigFileUtils import ConfigFileUtils

def main():
    if len(sys.argv) != 3:
        print "Error: exactly 2 arguments are required!"

    settings_path = sys.argv[1]
    run_dir = sys.argv[2]

    confhandler = ConfigFileHandler()
    confhandler.load_configuration(settings_path)
    root_file_name = confhandler.get_field("Global", "root_file_name")

    # need to merge the many individual chunks coming from the augmentation. keep care that the weights are updated correctly!
    augmentation_training_chunks_dir = os.path.join(run_dir, "augmentation_training_chunks")
    augmentation_validation_chunks_dir = os.path.join(run_dir, "augmentation_validation_chunks")
    augmentation_test_chunks_dir = os.path.join(run_dir, "augmentation_test_chunks")

    augmentation_training_dir = os.path.join(run_dir, "augmentation_training")
    augmentation_validation_dir = os.path.join(run_dir, "augmentation_validation")
    augmentation_test_dir = os.path.join(run_dir, "augmentation_test")

    if not os.path.exists(augmentation_training_dir):
        os.makedirs(augmentation_training_dir)

    if not os.path.exists(augmentation_validation_dir):
        os.makedirs(augmentation_validation_dir)

    if not os.path.exists(augmentation_test_dir):
        os.makedirs(augmentation_test_dir)

    def merge_chunks(source_dir, dest_dir):
        if not os.path.exists(dest_dir):
            os.makedirs(dest_dir)
        
        available_dirs = next(os.walk(source_dir))[1]
        merged_dirs = list(set(map(lambda x: re.sub('_chunk_.*$', '', x), available_dirs)))
    
        for merged_dir in merged_dirs:
            chunks = sorted([cur_dir for cur_dir in available_dirs if merged_dir + "_chunk_" in cur_dir])
    
            dest_folder = os.path.join(dest_dir, merged_dir)
            if not os.path.exists(dest_folder):
                os.makedirs(dest_folder)
    
            dest_file = os.path.join(dest_folder, root_file_name)
        
            source_files = [os.path.join(source_dir, chunk, root_file_name) for chunk in chunks]
        
            print "merging " + " ".join(chunks) + " into " + merged_dir

            # do the raw merging
            output = sp.check_output(["hadd", dest_file] + source_files)
            print output
    
            # now, need to ensure that the metadata is corrected (as hadd also modfies it in a way that is incorrect here)
            command = ["rootcp", "--replace", source_files[0] + ":ClassTree/Counters", dest_file + ":/ClassTree/Counters"]
            print " ".join(command)
            output = sp.check_output(command)
            print output

    merge_chunks(augmentation_training_chunks_dir, augmentation_training_dir)
    merge_chunks(augmentation_validation_chunks_dir, augmentation_validation_dir)
    merge_chunks(augmentation_test_chunks_dir, augmentation_test_dir)

if __name__ == "__main__":
    main()
