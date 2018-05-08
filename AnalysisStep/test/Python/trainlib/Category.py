class Category:
    
    def __init__(self, name, datastream, datastream_unmixed = None):
        self.name = name
        self.datastream = datastream

        if datastream_unmixed is not None:
            self.datastream_unmixed = datastream_unmixed
        else:
            self.datastream_unmixed = self.datastream

        self.endpieces = []

    def add_endpiece(self, endpiece):
        self.endpieces.append(endpiece)

    def has_matching_endpiece(self, endpiece):
        for ep in self.endpieces:
            if ep.name == endpiece.name:
                return ep

        return None
        
    # match two categories, i.e. look at the DiscriminantEndpieces they contain and return the preferred way of matching them
    @staticmethod
    def match(cat_a, cat_b):

        # need to check for a match while respecting the preference order!
        for ep_a in cat_a.endpieces:
            ep_b = cat_b.has_matching_endpiece(ep_a)

            if ep_b:
                print "matched " + cat_a.name + " with " + cat_b.name + ", using endpieces " + ep_a.name + " and " + ep_b.name
            
                return cat_a, ep_a, cat_b, ep_b
        
        print "Error: these categories are mutually incompatible!"
        return None
