from Config import Config

from ROOT import TFile
import array

class Augmenter:

    def __init__(self, model, preprocessor):
        self.model = model
        self.preprocessor = preprocessor

    def augment(self, inpath, outpath):

        f = TFile.Open(inpath)
        counters = f.Get("ZZTree/Counters") # get the metadata
        tree = f.Get("ZZTree/candTree") # get the tree

        newfile = TFile(outpath, "RECREATE")

        newfile.mkdir("ZZTree")
        newfile.cd("ZZTree")
    
        newtree = tree.CloneTree(0)
    
        # add definition of computation result here
        leaf = self.model.name + "/D"
        leafValues = array.array("f", [0.0])
    
        newBranch = newtree.Branch(self.model.name, leafValues, leaf)
    
        for i in range(tree.GetEntries()):
            tree.GetEntry(i)
            
            # add own computation here
            leafValues[0] = 2.0 * tree.PFMET
    
            newtree.Fill()
        
        newtree.Write()
        counters.Write()
        newfile.Close()
        
