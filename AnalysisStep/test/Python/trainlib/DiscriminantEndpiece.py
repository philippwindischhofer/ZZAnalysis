class DiscriminantEndpiece:

    def __init__(self, name):
        self.name = name
        self.endpiece_components = {}

    def add_component(self, component):
        self.endpiece_components[component.name] = component

    def __getitem__(self, key):
        return self.endpiece_components[key]
    
