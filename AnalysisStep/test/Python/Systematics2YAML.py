from trainlib.ConfigFileHandler import ConfigFileHandler
from trainlib.ConfigFileUtils import ConfigFileUtils
import yaml
import os
import sys

class Systematics2YAML:
    def __init__(this):
        this.yaml_def = {}
        
    def add_systematics(this, systematics_name):
        this.yaml_def[systematics_name] = {}
        
    def add_category(this, systematics_name, category_name):
        this.yaml_def[systematics_name][category_name] = {}
        
    def add_category_entry(this, systematics_name, category_name, entry_dict):
        this.yaml_def[systematics_name][category_name].update(entry_dict)
        
    def save(this, file_path):
        # delete recursively all categories (and subcategories) that do not contain any values
        for systematics_key in this.yaml_def.keys():
            for category_key in this.yaml_def[systematics_key].keys():
                # remove empty categories
                if this.yaml_def[systematics_key][category_key].keys() == ["type"]:
                    del this.yaml_def[systematics_key][category_key]

            # remove empty systematics
            if this.yaml_def[systematics_key].keys() == []:
                del this.yaml_def[systematics_key]

        with open(file_path, 'w') as yaml_file:
            yaml.dump(this.yaml_def, yaml_file, default_flow_style = False)

def main():

    if len(sys.argv) != 3:
        print "Error: exactly 2 arguments are required"
    
    # global settings
    categories = ["UnTagged", "VBF1jTagged", "VBF2jTagged", "VHLeptTagged", "VHHadrTagged", "ttHLeptTagged", "ttHHadrTagged", "VHMETTagged"]
    processes = {'ggH': 'ggH_hzz',
                 'qqH': 'qqH_hzz',
                 'WH_lep': 'WH_lep_hzz',
                 'WH_had': 'WH_had_hzz',
                 'ZH_lep': 'ZH_lep_hzz',
                 'ZH_had': 'ZH_had_hzz',
                 'ttH_lep': 'ttH_lep_hzz',
                 'ttH_had': 'ttH_had_hzz',
                 'tqH': 'tqH_hzz',
                 'bbH': 'bbH_hzz',
                 'qqZZ': 'qqZZ_hzz',
                 'ggZZ': 'ggZZ_hzz'}

    input_file = sys.argv[1]
    output_path = sys.argv[2]
    # output_path = os.path.dirname(input_file)
    
    output_file_expt = os.path.join(output_path, "systematics_expt_13TeV.yaml")
    output_file_th = os.path.join(output_path, "systematics_theory_13TeV.yaml")

    confhandler = ConfigFileHandler()
    confhandler.load_configuration(input_file)    

    # prepare the YAML file for the experimental uncertainties
    syst_expt = Systematics2YAML()

    # --------------------------------
    # PileUp
    # --------------------------------
    # the global, inclusive one
    source_syst_name = 'PileUp'
    YAML_syst_name = 'CMS_PileUp'
    syst_type = 'lnN'
    syst_expt.add_systematics(YAML_syst_name)

    for category in categories:
        syst_expt.add_category(YAML_syst_name, category)
        cur_sect = confhandler.get_section(source_syst_name + ": " + "inclusive")

        syst_expt.add_category_entry(YAML_syst_name, category, {'type': syst_type})

        for process in processes.keys():
            val_list = ConfigFileUtils.parse_list(cur_sect[process], lambda x: float(x))
            val_string = str(val_list[0]) + "/" + str(val_list[1])
            
            if not(val_list[0] == 1.0 and val_list[1] == 1.0):
                syst_expt.add_category_entry(YAML_syst_name, category, {processes[process]: val_string})            


    # for each category
    source_syst_name = 'PileUp'
    YAML_syst_name = 'CMS_PileUp_cat'
    syst_type = 'lnN'
    syst_expt.add_systematics(YAML_syst_name)

    for category in categories:
        syst_expt.add_category(YAML_syst_name, category)
        cur_sect = confhandler.get_section(source_syst_name + ": " + category)

        syst_expt.add_category_entry(YAML_syst_name, category, {'type': syst_type})

        for process in processes.keys():
            val_list = ConfigFileUtils.parse_list(cur_sect[process], lambda x: float(x))
            val_string = str(val_list[0]) + "/" + str(val_list[1])
            
            if not(val_list[0] == 1.0 and val_list[1] == 1.0):
                syst_expt.add_category_entry(YAML_syst_name, category, {processes[process]: val_string})            

    # --------------------------------
    # Jet Energy Scale
    # --------------------------------
    source_syst_name = 'JEC'
    YAML_syst_name = 'CMS_scale_j_13TeV'
    syst_type = 'lnN'
    syst_expt.add_systematics(YAML_syst_name)

    for category in categories:
        syst_expt.add_category(YAML_syst_name, category)
        cur_sect = confhandler.get_section(source_syst_name + ": " + category)

        syst_expt.add_category_entry(YAML_syst_name, category, {'type': syst_type})

        for process in processes.keys():
            val_list = ConfigFileUtils.parse_list(cur_sect[process], lambda x: float(x))
            val_string = str(val_list[0]) + "/" + str(val_list[1])

            if not(val_list[0] == 1.0 and val_list[1] == 1.0):
                syst_expt.add_category_entry(YAML_syst_name, category, {processes[process]: val_string})            

    # --------------------------------
    # Lepton Energy Scale
    # --------------------------------
    source_syst_name = 'LEC'
    YAML_syst_name = 'CMS_scale_l_13TeV'
    syst_type = 'lnN'
    syst_expt.add_systematics(YAML_syst_name)

    for category in categories:
        syst_expt.add_category(YAML_syst_name, category)
        cur_sect = confhandler.get_section(source_syst_name + ": " + category)

        syst_expt.add_category_entry(YAML_syst_name, category, {'type': syst_type})

        for process in processes.keys():
            val_list = ConfigFileUtils.parse_list(cur_sect[process], lambda x: float(x))
            val_string = str(val_list[0]) + "/" + str(val_list[1])

            if not(val_list[0] == 1.0 and val_list[1] == 1.0):
                syst_expt.add_category_entry(YAML_syst_name, category, {processes[process]: val_string})            

    # --------------------------------
    # BTagging
    # --------------------------------
    source_syst_name = 'BTag'
    YAML_syst_name = 'CMS_eff_b'
    syst_type = 'lnN'
    syst_expt.add_systematics(YAML_syst_name)

    for category in categories:
        syst_expt.add_category(YAML_syst_name, category)
        cur_sect = confhandler.get_section(source_syst_name + ": " + category)

        syst_expt.add_category_entry(YAML_syst_name, category, {'type': syst_type})

        for process in processes.keys():
            val_list = ConfigFileUtils.parse_list(cur_sect[process], lambda x: float(x))
            val_string = str(val_list[0]) + "/" + str(val_list[1])

            if not(val_list[0] == 1.0 and val_list[1] == 1.0):
                syst_expt.add_category_entry(YAML_syst_name, category, {processes[process]: val_string})            

    syst_expt.save(output_file_expt)

    # ---------------------------------------------------------------------------------
    # ---------------------------------------------------------------------------------

    # prepare the YAML file for the theoretical uncertainties
    syst_th = Systematics2YAML()

    # --------------------------------
    # PDF scale
    # --------------------------------
    source_syst_name = 'PDF_scale'
    YAML_syst_name = 'pdf_Higgs_gg_cat'
    syst_type = 'lnN'
    syst_th.add_systematics(YAML_syst_name)
    
    for category in categories:
        syst_th.add_category(YAML_syst_name, category)
        cur_sect = confhandler.get_section(source_syst_name + ": " + category)

        syst_th.add_category_entry(YAML_syst_name, category, {'type': syst_type})

        for process in ["ggH", "ggZZ"]:
            val_list = ConfigFileUtils.parse_list(cur_sect[process], lambda x: float(x))
            val_string = str(val_list[0]) + "/" + str(val_list[1])

            if not(val_list[0] == 1.0 and val_list[1] == 1.0):
                syst_th.add_category_entry(YAML_syst_name, category, {processes[process]: val_string})            

    source_syst_name = 'PDF_scale'
    YAML_syst_name = 'pdf_Higgs_qqbar_cat'
    syst_type = 'lnN'
    syst_th.add_systematics(YAML_syst_name)
    
    for category in categories:
        syst_th.add_category(YAML_syst_name, category)
        cur_sect = confhandler.get_section(source_syst_name + ": " + category)

        syst_th.add_category_entry(YAML_syst_name, category, {'type': syst_type})

        for process in ["qqH", "WH_lep", "WH_had", "ZH_lep", "ZH_had"]:
            val_list = ConfigFileUtils.parse_list(cur_sect[process], lambda x: float(x))
            val_string = str(val_list[0]) + "/" + str(val_list[1])

            if not(val_list[0] == 1.0 and val_list[1] == 1.0):
                syst_th.add_category_entry(YAML_syst_name, category, {processes[process]: val_string})            

    source_syst_name = 'PDF_scale'
    YAML_syst_name = 'pdf_Higgs_ttH_cat'
    syst_type = 'lnN'
    syst_th.add_systematics(YAML_syst_name)
    
    for category in categories:
        syst_th.add_category(YAML_syst_name, category)
        cur_sect = confhandler.get_section(source_syst_name + ": " + category)

        syst_th.add_category_entry(YAML_syst_name, category, {'type': syst_type})

        for process in ["ttH_lep", "ttH_had"]:
            val_list = ConfigFileUtils.parse_list(cur_sect[process], lambda x: float(x))
            val_string = str(val_list[0]) + "/" + str(val_list[1])

            if not(val_list[0] == 1.0 and val_list[1] == 1.0):
                syst_th.add_category_entry(YAML_syst_name, category, {processes[process]: val_string})            

    source_syst_name = 'PDF_scale'
    YAML_syst_name = 'pdf_qqbar_cat'
    syst_type = 'lnN'
    syst_th.add_systematics(YAML_syst_name)
    
    for category in categories:
        syst_th.add_category(YAML_syst_name, category)
        cur_sect = confhandler.get_section(source_syst_name + ": " + category)

        syst_th.add_category_entry(YAML_syst_name, category, {'type': syst_type})

        for process in ["qqZZ"]:
            val_list = ConfigFileUtils.parse_list(cur_sect[process], lambda x: float(x))
            val_string = str(val_list[0]) + "/" + str(val_list[1])

            if not(val_list[0] == 1.0 and val_list[1] == 1.0):
                syst_th.add_category_entry(YAML_syst_name, category, {processes[process]: val_string})            

    # --------------------------------
    # QCD scale
    # --------------------------------
         
    source_syst_name = 'QCD_scale'
    YAML_syst_name = 'QCDscale_qqH_cat'
    syst_type = 'lnN'
    syst_th.add_systematics(YAML_syst_name)
    
    for category in categories:
        syst_th.add_category(YAML_syst_name, category)
        cur_sect = confhandler.get_section(source_syst_name + ": " + category)

        syst_th.add_category_entry(YAML_syst_name, category, {'type': syst_type})

        for process in ["qqH"]:
            val_list = ConfigFileUtils.parse_list(cur_sect[process], lambda x: float(x))
            val_string = str(val_list[0]) + "/" + str(val_list[1])

            if not(val_list[0] == 1.0 and val_list[1] == 1.0):
                syst_th.add_category_entry(YAML_syst_name, category, {processes[process]: val_string})            

    source_syst_name = 'QCD_scale'
    YAML_syst_name = 'QCDscale_VH_cat'
    syst_type = 'lnN'
    syst_th.add_systematics(YAML_syst_name)
    
    for category in categories:
        syst_th.add_category(YAML_syst_name, category)
        cur_sect = confhandler.get_section(source_syst_name + ": " + category)

        syst_th.add_category_entry(YAML_syst_name, category, {'type': syst_type})

        for process in ["WH_lep", "WH_had"]:
            val_list = ConfigFileUtils.parse_list(cur_sect[process], lambda x: float(x))
            val_string = str(val_list[0]) + "/" + str(val_list[1])

            if not(val_list[0] == 1.0 and val_list[1] == 1.0):
                syst_th.add_category_entry(YAML_syst_name, category, {processes[process]: val_string})            

                # here, since did not have the values for ZH_had and ZH_lep available, copy them from the corresponding WH processes
                if process == "WH_lep":
                    syst_th.add_category_entry(YAML_syst_name, category, {processes["ZH_lep"]: val_string})            
                if process == "WH_had":
                    syst_th.add_category_entry(YAML_syst_name, category, {processes["ZH_had"]: val_string})            

    source_syst_name = 'QCD_scale'
    YAML_syst_name = 'QCDscale_ttH_cat'
    syst_type = 'lnN'
    syst_th.add_systematics(YAML_syst_name)
    
    for category in categories:
        syst_th.add_category(YAML_syst_name, category)
        cur_sect = confhandler.get_section(source_syst_name + ": " + category)

        syst_th.add_category_entry(YAML_syst_name, category, {'type': syst_type})

        for process in ["ttH_lep", "ttH_had", "tqH", "bbH"]:
            val_list = ConfigFileUtils.parse_list(cur_sect[process], lambda x: float(x))
            val_string = str(val_list[0]) + "/" + str(val_list[1])

            if not(val_list[0] == 1.0 and val_list[1] == 1.0):
                syst_th.add_category_entry(YAML_syst_name, category, {processes[process]: val_string})            

    source_syst_name = 'QCD_scale'
    YAML_syst_name = 'QCDscale_VV_cat'
    syst_type = 'lnN'
    syst_th.add_systematics(YAML_syst_name)
    
    for category in categories:
        syst_th.add_category(YAML_syst_name, category)
        cur_sect = confhandler.get_section(source_syst_name + ": " + category)

        syst_th.add_category_entry(YAML_syst_name, category, {'type': syst_type})

        for process in ["qqZZ"]:
            val_list = ConfigFileUtils.parse_list(cur_sect[process], lambda x: float(x))
            val_string = str(val_list[0]) + "/" + str(val_list[1])

            if not(val_list[0] == 1.0 and val_list[1] == 1.0):
                syst_th.add_category_entry(YAML_syst_name, category, {processes[process]: val_string})            

    # --------------------------------
    # EW corrections
    # --------------------------------

    source_syst_name = 'EWCorr'
    YAML_syst_name = 'EWcorr_VV_cat'
    syst_type = 'lnN'
    syst_th.add_systematics(YAML_syst_name)
    
    for category in categories:
        syst_th.add_category(YAML_syst_name, category)
        cur_sect = confhandler.get_section(source_syst_name + ": " + category)

        syst_th.add_category_entry(YAML_syst_name, category, {'type': syst_type})

        for process in ["qqZZ"]:
            val_list = ConfigFileUtils.parse_list(cur_sect[process], lambda x: float(x))
            val_string = str(val_list[0]) + "/" + str(val_list[1])

            if not(val_list[0] == 1.0 and val_list[1] == 1.0):
                syst_th.add_category_entry(YAML_syst_name, category, {processes[process]: val_string})            

    # --------------------------------
    # ggH uncertainties
    # --------------------------------
                
    source_syst_name = 'THU_ggH_Mu'
    YAML_syst_name = 'THU_ggH_Mu'
    syst_type = 'lnN'
    syst_th.add_systematics(YAML_syst_name)

    for category in categories:
        syst_th.add_category(YAML_syst_name, category)
        cur_sect = confhandler.get_section(source_syst_name + ": " + category)

        syst_th.add_category_entry(YAML_syst_name, category, {'type': syst_type})

        process = "ggH"
        val = float(cur_sect[process])

        if not(val == 1.0):
            syst_th.add_category_entry(YAML_syst_name, category, {processes[process]: val})            

        # can use the same value as well for the ggZZ background! (here and in the following)
        if not(val == 1.0):
            syst_th.add_category_entry(YAML_syst_name, category, {processes["ggZZ"]: val})            

    source_syst_name = 'THU_ggH_Res'
    YAML_syst_name = 'THU_ggH_Res'
    syst_type = 'lnN'
    syst_th.add_systematics(YAML_syst_name)

    for category in categories:
        syst_th.add_category(YAML_syst_name, category)
        cur_sect = confhandler.get_section(source_syst_name + ": " + category)

        syst_th.add_category_entry(YAML_syst_name, category, {'type': syst_type})

        process = "ggH"
        val = float(cur_sect[process])

        if not(val == 1.0):
            syst_th.add_category_entry(YAML_syst_name, category, {processes[process]: val})            

        if not(val == 1.0):
            syst_th.add_category_entry(YAML_syst_name, category, {processes["ggZZ"]: val})            

    source_syst_name = 'THU_ggH_Mig01'
    YAML_syst_name = 'THU_ggH_Mig01'
    syst_type = 'lnN'
    syst_th.add_systematics(YAML_syst_name)

    for category in categories:
        syst_th.add_category(YAML_syst_name, category)
        cur_sect = confhandler.get_section(source_syst_name + ": " + category)

        syst_th.add_category_entry(YAML_syst_name, category, {'type': syst_type})

        process = "ggH"
        val = float(cur_sect[process])

        if not(val == 1.0):
            syst_th.add_category_entry(YAML_syst_name, category, {processes[process]: val})            

        if not(val == 1.0):
            syst_th.add_category_entry(YAML_syst_name, category, {processes["ggZZ"]: val})            

    source_syst_name = 'THU_ggH_Mig12'
    YAML_syst_name = 'THU_ggH_Mig12'
    syst_type = 'lnN'
    syst_th.add_systematics(YAML_syst_name)

    for category in categories:
        syst_th.add_category(YAML_syst_name, category)
        cur_sect = confhandler.get_section(source_syst_name + ": " + category)

        syst_th.add_category_entry(YAML_syst_name, category, {'type': syst_type})

        process = "ggH"
        val = float(cur_sect[process])

        if not(val == 1.0):
            syst_th.add_category_entry(YAML_syst_name, category, {processes[process]: val})            

        if not(val == 1.0):
            syst_th.add_category_entry(YAML_syst_name, category, {processes["ggZZ"]: val})            

    source_syst_name = 'THU_ggH_VBF2j'
    YAML_syst_name = 'THU_ggH_VBF2j'
    syst_type = 'lnN'
    syst_th.add_systematics(YAML_syst_name)

    for category in categories:
        syst_th.add_category(YAML_syst_name, category)
        cur_sect = confhandler.get_section(source_syst_name + ": " + category)

        syst_th.add_category_entry(YAML_syst_name, category, {'type': syst_type})

        process = "ggH"
        val = float(cur_sect[process])

        if not(val == 1.0):
            syst_th.add_category_entry(YAML_syst_name, category, {processes[process]: val})            

        if not(val == 1.0):
            syst_th.add_category_entry(YAML_syst_name, category, {processes["ggZZ"]: val})            

    source_syst_name = 'THU_ggH_VBF3j'
    YAML_syst_name = 'THU_ggH_VBF3j'
    syst_type = 'lnN'
    syst_th.add_systematics(YAML_syst_name)

    for category in categories:
        syst_th.add_category(YAML_syst_name, category)
        cur_sect = confhandler.get_section(source_syst_name + ": " + category)

        syst_th.add_category_entry(YAML_syst_name, category, {'type': syst_type})

        process = "ggH"
        val = float(cur_sect[process])

        if not(val == 1.0):
            syst_th.add_category_entry(YAML_syst_name, category, {processes[process]: val})            

        if not(val == 1.0):
            syst_th.add_category_entry(YAML_syst_name, category, {processes["ggZZ"]: val})            

    source_syst_name = 'THU_ggH_PT60'
    YAML_syst_name = 'THU_ggH_PT60'
    syst_type = 'lnN'
    syst_th.add_systematics(YAML_syst_name)

    for category in categories:
        syst_th.add_category(YAML_syst_name, category)
        cur_sect = confhandler.get_section(source_syst_name + ": " + category)

        syst_th.add_category_entry(YAML_syst_name, category, {'type': syst_type})

        process = "ggH"
        val = float(cur_sect[process])

        if not(val == 1.0):
            syst_th.add_category_entry(YAML_syst_name, category, {processes[process]: val})            

        if not(val == 1.0):
            syst_th.add_category_entry(YAML_syst_name, category, {processes["ggZZ"]: val})            

    source_syst_name = 'THU_ggH_PT120'
    YAML_syst_name = 'THU_ggH_PT120'
    syst_type = 'lnN'
    syst_th.add_systematics(YAML_syst_name)

    for category in categories:
        syst_th.add_category(YAML_syst_name, category)
        cur_sect = confhandler.get_section(source_syst_name + ": " + category)

        syst_th.add_category_entry(YAML_syst_name, category, {'type': syst_type})

        process = "ggH"
        val = float(cur_sect[process])

        if not(val == 1.0):
            syst_th.add_category_entry(YAML_syst_name, category, {processes[process]: val})            

        if not(val == 1.0):
            syst_th.add_category_entry(YAML_syst_name, category, {processes["ggZZ"]: val})            

    source_syst_name = 'THU_ggH_qmtop'
    YAML_syst_name = 'THU_ggH_qmtop'
    syst_type = 'lnN'
    syst_th.add_systematics(YAML_syst_name)

    for category in categories:
        syst_th.add_category(YAML_syst_name, category)
        cur_sect = confhandler.get_section(source_syst_name + ": " + category)

        syst_th.add_category_entry(YAML_syst_name, category, {'type': syst_type})

        process = "ggH"
        val = float(cur_sect[process])

        if not(val == 1.0):
            syst_th.add_category_entry(YAML_syst_name, category, {processes[process]: val})            

        if not(val == 1.0):
            syst_th.add_category_entry(YAML_syst_name, category, {processes["ggZZ"]: val})            

    syst_th.save(output_file_th)

if __name__ == "__main__":
    main()
