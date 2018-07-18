import sys
import os
import subprocess as sp

# this takes all the files needed for the evaluation of systematics (hard-coded below) and splits
# them up into several file, where each one takes into account either JEC or BTag variations
def main():
    if len(sys.argv) != 2:
        print "Error: exactly 1 argument is required"

    root_dir = sys.argv[1]

    # global settings
    root_file_name = "ZZ4lAnalysis.root"
    zzroot = os.environ["CMSSW_BASE"]
    bin_dir = os.path.join(zzroot, "bin/slc6_amd64_gcc630/")
    systematic_preparer = os.path.join(bin_dir, "run_systematic_preparer")

    # list of files that will be processed by this script
    input_folders = ["ggH125ext", "VBFH125ext", "WplusH125ext", "WminusH125ext", "ZH125ext", "ttH125ext", "bbH125", "tqH125", "ZZTo4l", "ggTo4e_Contin_MCFM701", "ggTo4mu_Contin_MCFM701", "ggTo4tau_Contin_MCFM701", "ggTo2e2mu_Contin_MCFM701", "ggTo2e2tau_Contin_MCFM701", "ggTo2mu2tau_Contin_MCFM701"]

    modes = ["JEC_UP", "JEC_DN", "BTag_UP", "BTag_DN", "LEC_UP", "LEC_DN"]
    
    for input_folder in input_folders:
        for mode in modes:
            
            # for this file and this mode, run the systematic extractor
            output_folder = os.path.join(root_dir, input_folder + "_" + mode)
            
            if not os.path.exists(output_folder):
                os.makedirs(output_folder)

            input_file = os.path.join(root_dir, input_folder, root_file_name)
            output_file = os.path.join(output_folder, root_file_name)

            print input_file + " --> " + output_file

            output = sp.check_output([systematic_preparer, input_file, output_file, mode])
            print output

if __name__ == "__main__":
    main()
