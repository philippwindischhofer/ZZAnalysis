from utils import get_size, read_data
import numpy as np

# this simulates a single ROOT file that is actually distributed over multiple "physical" ROOT trees. From each file in the list,
# only the portion between start_fraction and end_fraction is considered
class FileCollection:
    def __init__(self, files_cuts, start_fraction, end_fraction):
        self.files_cuts = files_cuts
        self.files = self.files_cuts.keys()
        self.cuts = self.files_cuts.values()

        self.start_fraction = start_fraction
        self.end_fraction = end_fraction

        # the number of entries in each file, and the local start- and endpositions
        self.lengths = [get_size(file_path) for file_path in self.files]
        self.minpos = [int(length * start_fraction) for length in self.lengths]
        self.maxpos = [int(length * end_fraction) for length in self.lengths]
        
        self.used_lengths = [end - beginning for (end, beginning) in zip(self.maxpos, self.minpos)]
                
        self.total_length = sum(self.lengths)
        self.used_length = sum(self.used_lengths)
        
        print "collection set up: " + str(len(self.files)) + " files, " + str(self.total_length) + " entries in total, " + str(self.used_length) + " of which will be used"
        
    def get_length(self):
        return self.used_length
    
    # returns some data from this file collection
    def get_data(self, branches, start, end):
        # now need to translate between a global index, and a filepath and its corresponding local index
        if isinstance(start, float):
            start_index = int(start * self.get_length())
        else:
            start_index = start

        if isinstance(end, float):
            end_index = int(end * self.get_length())
        else:
            end_index = end

        return read_data(self, start_index, end_index, branches)
        
    def transform_index(self, global_index):
        if global_index > self.get_length():
            raise IndexError("global index out of range: requested entry " + str(global_index) + ", but max. index is " + str(self.get_length() - 1))
        
        # first determine which file in the list is needed to read this index
        max_local_indices = np.array(self.used_lengths) - 1  # all works by 0-indexing
        cum_lengths = np.cumsum(self.used_lengths)
        
        needed_file = 0
        while global_index > cum_lengths[needed_file] - 1:
            needed_file += 1
            
        # then determine the corresponding local index within this file
        local_minpos = np.append(0, cum_lengths)
                
        local_index = global_index - local_minpos[needed_file]
        
        # up to now, all these indices are relative w.r.t. the used slice in each file. the beginning of these slices can be shifted w.r.t. the beginning of the file itself
        local_index += self.minpos[needed_file]
            
        return self.files[needed_file], local_index
    
    def transform_index_range(self, global_start_index, global_end_index):
        if global_start_index > self.get_length() or global_end_index > self.get_length():
            raise IndexError("global index out of range: requested entries " + str(global_start_index) + " through " + str(global_end_index) + ", but max. index is " + str(self.get_length() - 1))
        if global_end_index < global_start_index:
            raise IndexError("end ought to come after beginning")
            
        local_coords = [self.transform_index(global_index) for global_index in range(global_start_index, global_end_index)]
        needed_files = set([local_coord[0] for local_coord in local_coords])
        
        retval = []
        # now look at each needed file in turn and determine the relevant index range in this local file
        for needed_file in needed_files:
            needed_local_indices = [local_coord[1] for local_coord in local_coords if local_coord[0] == needed_file ]
            needed_min_index = min(needed_local_indices)
            needed_max_index = max(needed_local_indices)
            
            retval.append([needed_file, self.files_cuts[needed_file], needed_min_index, needed_max_index])
        
        return retval
