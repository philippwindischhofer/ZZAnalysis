import configparser
import inspect
from config import ConfigFileConfig

class ConfigFileHandler:

    def __init__(self):
        self.config = configparser.ConfigParser()

    def _idict_translator(self, inval, translation_dict):
        inverse_translation = {val: key for key, val in translation_dict.iteritems()}
        return inverse_translation.get(inval)

    def _dict_translator(self, inval, translation_dict):
        return translation_dict[inval]

    def _process_dict(self, rawstring, processor):
        retdict = {}
        if rawstring:
            for source in rawstring.split(','):
                key, cmd = source.split(':')
                retdict[key.strip()] = processor(cmd.strip())
        
        return retdict    

    def _process_list(self, rawstring, processor):
        retlist = []
        for entry in rawstring.split(','):
            retlist.append(processor(entry.strip()))
            
        return retlist

    def _assemble_lambda_string(self, func):
        func_string = str(inspect.getsourcelines(func)[0])
        func_string = func_string.strip("['\\n']").split(" = ")[1]

        return func_string

    def _assemble_dict(self, in_dict, processor):
    
        out_string = ""
    
        for key, val in in_dict.iteritems():
            out_string += key + ": "
            out_string += processor(val) + ", "
    
        # cut away the last two (superfluous) characters
        return out_string[:-2]

    def _assemble_list(self, in_list, processor):
        out_string = ""
    
        for entry in in_list:
            out_string += processor(entry) + ", "
        
        return out_string[:-2]

    def _get_sections(self):
        return self.config.sections()

    def _get_section(self,section):
        return self.config[section]

    def _get_field(self, section, field):
        return self.config[section][field]
        
    def _set_field(self, section, field, value):
        self.config[section][field] = value

    def _new_section(self, new_section):
        self.config[new_section] = {}

    def LoadConfiguration(self, configpath):
        print "attempting to load configuration file from " + configpath
        self.config.read(configpath)

    def SaveConfiguration(self, configpath):
        with open(configpath, 'w') as configfile:
            self.config.write(configfile)
