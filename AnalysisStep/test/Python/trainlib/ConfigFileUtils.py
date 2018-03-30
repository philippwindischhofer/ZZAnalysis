import inspect

class ConfigFileUtils:

    @staticmethod
    def parse_list(rawstring, processor):
        retlist = []
        for entry in rawstring.split(';'):
            retlist.append(processor(entry.strip()))
            
        return retlist

    @staticmethod
    def is_dict(rawstring):
        if ":" in rawstring:
            return True
        else:
            return False

    @staticmethod
    def parse_dict(rawstring, processor):
        retdict = {}
        if rawstring:
            for source in rawstring.split(','):
                key, cmd = source.split(':')
                retdict[key.strip()] = processor(cmd.strip())
        
        return retdict    

    @staticmethod
    def parse_lambda(rawstring):
        return eval(rawstring)

    @staticmethod
    def serialize_list(in_list, processor):
        out_string = ""
    
        for entry in in_list:
            out_string += processor(entry) + "; "
        
        return out_string[:-2]

    @staticmethod
    def serialize_dict(in_dict, processor):            
        out_string = ""
    
        for key, val in in_dict.iteritems():
            out_string += key + ": "
            out_string += processor(val) + ", "
    
        # cut away the last two (superfluous) characters
        return out_string[:-2]

    @staticmethod
    def serialize_lambda(func):
        func_string = str(inspect.getsourcelines(func)[0])
        func_string = func_string.strip("['\\n']").split(" = ")[1]

        return func_string

    @staticmethod
    def inverse_dict_processor(inval, translation_dict):
        inverse_translation = {val: key for key, val in translation_dict.iteritems()}
        return inverse_translation.get(inval)

    @staticmethod
    def dict_processor(inval, translation_dict):
        return translation_dict[inval]

