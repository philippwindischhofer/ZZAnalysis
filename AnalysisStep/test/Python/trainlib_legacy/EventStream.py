class EventStream:

    def __init__(self):
        self.paths = []
        self.cuts = []

    def AddEventSource(self, path, cut):
        self.paths.append(path)
        self.cuts.append(cut)

    def GetFiles(self):
        return self.paths

    def GetCuts(self):
        return self.cuts
