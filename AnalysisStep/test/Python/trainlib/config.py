class Config:
    # the branches that are always going to be loaded and on which any preprocessor will act
    branches = ["PFMET", "nCleanedJetsPt30", "nCleanedJetsPt30BTagged_bTagSF", "nExtraLep", "D_VBF2j_ggH_ME"]
    MC_filename = "/ZZ4lAnalysis.root"

class TrainingConfig:
    def __init__(self, steps_per_epoch = 128, max_epochs = 100):
        self.steps_per_epoch = steps_per_epoch
        self.max_epochs = max_epochs