from ROOT import TFile
from root_numpy import root2array, root2rec, tree2array
import numpy as np
import pandas as pd

def get_size(filepath, tree_name = "ClassTree"):
    print "skimming " + filepath
    f = TFile.Open(filepath)

    #tree_name = "ClassTree"

    size = f.Get(tree_name + "/candTree").GetEntries()
    f.Close()

    return size

def read_data(collection, start, stop, branches, tree_name = "ClassTree"):
    #print "requesting data in range (" + str(start) + ", " + str(stop) + ")"
    filetuple = collection.transform_index_range(start, stop)
        
    files = [entry[0] for entry in filetuple]
    cuts = [entry[1] for entry in filetuple]
    start_indices = [entry[2] for entry in filetuple]
    stop_indices = [entry[3] for entry in filetuple]
    
    read_list = []
    for (cur_file, cur_cut, cur_start_index, cur_stop_index) in zip(files, cuts, start_indices, stop_indices):
        #print "reading from " + cur_file + ": (" + str(cur_start_index) + ", " + str(cur_stop_index) + ")" 

        read_data = pd.DataFrame(root2array(cur_file, treename = tree_name + "/candTree", branches = branches, start = cur_start_index, stop = cur_stop_index + 1))
        cut_data = read_data.loc[read_data.apply(cur_cut, axis = 1)]
        read_list.append(cut_data)

        #print "read successful"
        
    return pd.concat(read_list)
