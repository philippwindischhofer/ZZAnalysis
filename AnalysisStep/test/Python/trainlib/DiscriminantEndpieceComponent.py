from ConfigFileUtils import ConfigFileUtils

class DiscriminantEndpieceComponent:

    def __init__(self, name, public_name, component_cut, nonperiodic_columns, periodic_columns, model_basetype, model_hyperparams, preprocessor_basetype, component_cut_s = None):
        self.name = name
        self.public_name = public_name
        self.component_cut = component_cut
        self.component_cut_s = component_cut_s
        self.nonperiodic_columns = nonperiodic_columns
        self.periodic_columns = periodic_columns
        self.model_basetype = model_basetype
        self.model_hyperparams = model_hyperparams
        self.preprocessor_basetype = preprocessor_basetype

    @classmethod
    def from_config(cls, config_section, variable_section, model_basetype, preprocessor_basetype):
        name = config_section["name"]
        public_name = config_section["public_name"]
        component_cut = ConfigFileUtils.parse_lambda(config_section["cut"])
        component_cut_s = config_section["cut"]
        nonperiodic_columns = ConfigFileUtils.parse_list(variable_section[config_section["default_variables_nonperiodic"]], lambda x: x)
        periodic_columns = ConfigFileUtils.parse_list(variable_section[config_section["default_variables_periodic"]], lambda x: x)
        model_hyperparams = ConfigFileUtils.parse_dict(variable_section["hyperparameters"], lambda x: float(x))

        return cls(name, public_name, component_cut, nonperiodic_columns, periodic_columns, model_basetype, model_hyperparams, preprocessor_basetype, component_cut_s = component_cut_s)
