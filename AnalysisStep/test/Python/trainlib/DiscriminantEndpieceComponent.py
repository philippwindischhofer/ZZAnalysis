class DiscriminantEndpieceComponent:

    def __init__(self, name, public_name, component_cut, nonperiodic_columns, periodic_columns, model_basetype, model_hyperparams, preprocessor_basetype):
        self.name = name
        self.public_name = public_name
        self.component_cut = component_cut
        self.nonperiodic_columns = nonperiodic_columns
        self.periodic_columns = periodic_columns
        self.model_basetype = model_basetype
        self.model_hyperparams = model_hyperparams
        self.preprocessor_basetype = preprocessor_basetype
