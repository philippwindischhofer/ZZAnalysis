import configparser

class ConfigFileHandler:

    def __init__(self):
        self.config = configparser.ConfigParser()

    def get_sections(self):
        return self.config.sections()

    def has_field(self, section, field):
        if field in self.get_section(section):
            return True
        else:
            return False

    def get_section(self, section):
        return self.config[section]

    def get_field(self, section, field):
        return self.config[section][field]
        
    def set_field(self, section, field, value):
        self.config[section][field] = value

    def new_section(self, new_section):
        self.config[new_section] = {}

    def load_configuration(self, configpath):
        print "attempting to load configuration file from " + configpath
        self.config.read(configpath)

    def save_configuration(self, configpath):
        with open(configpath, 'w') as configfile:
            self.config.write(configfile)
