import sys
sys.path.insert(0, '/home/llr/cms/wind/.local/lib/python2.7/site-packages/')

from bayes_opt import BayesianOptimization
import numpy as np

import matplotlib.pyplot as plt
from matplotlib import gridspec
from subprocess import check_output
from trainlib.ConfigFileHandler import ConfigFileHandler

import os
import math

evalcnt = 0

def main():

    if len(sys.argv) != 4:
        print "Error: exactly 3 arguments are required"
        
    run_dir = sys.argv[1]
    out_dir = sys.argv[2]
    engine = sys.argv[3]

    print run_dir
    print out_dir
    print engine

    def punzi_target(ggH_prior, WHhadr_prior, ZHhadr_prior, WHlept_prior, ZHlept_prior, ZHMET_prior, ttHhadr_prior, ttHlept_prior):
        global evalcnt

        bin_dir = "/home/llr/cms/wind/cmssw/CMSSW_9_4_2/bin/slc6_amd64_gcc630/"
        cost_function_evaluator = "run_prior_evaluator"
    
        output = check_output([bin_dir + cost_function_evaluator, run_dir, out_dir, engine, str(ggH_prior), str(WHhadr_prior), 
                               str(ZHhadr_prior), str(WHlept_prior), str(ZHlept_prior), str(ZHMET_prior), str(ttHhadr_prior), str(ttHlept_prior)])
    
        costval = 0.0

        for line in output.split('\n'):
            if "cost = " in line:
                costval = -float(line.replace("cost = ", ""))
                break
            
        if math.isnan(costval):
            costval = -10.0

        # save the sampled point such that later they can be used as exploration points (if the need occurs)
        confhandler = ConfigFileHandler()
        evaluations_path = out_dir + 'evaluations.txt'

        if os.path.exists(evaluations_path):
            confhandler.load_configuration(evaluations_path)

        section_name = 'evaluation_' + str(evalcnt)
        confhandler.new_section(section_name)
        confhandler.set_field(section_name, 'cost', str(costval))
        confhandler.set_field(section_name, 'ggH_prior', str(ggH_prior))
        confhandler.set_field(section_name, 'WHhadr_prior', str(WHhadr_prior))
        confhandler.set_field(section_name, 'ZHhadr_prior', str(ZHhadr_prior))
        confhandler.set_field(section_name, 'WHlept_prior', str(WHlept_prior))
        confhandler.set_field(section_name, 'ZHlept_prior', str(ZHlept_prior))
        confhandler.set_field(section_name, 'ZHMET_prior', str(ZHMET_prior))
        confhandler.set_field(section_name, 'ttHhadr_prior', str(ttHhadr_prior))
        confhandler.set_field(section_name, 'ttHlept_prior', str(ttHlept_prior))

        confhandler.save_configuration(evaluations_path)

        evalcnt += 1

        return costval
    
    eps = 1e-3
    bo = BayesianOptimization(punzi_target, {'ggH_prior': (1.0 + eps, 4.0), 'WHhadr_prior': (eps, 1.0 - eps), 
                                   'ZHhadr_prior': (eps, 1.0 - eps), 'WHlept_prior': (eps, 1.0 - eps),
                                   'ZHlept_prior': (eps, 1.0 - eps), 'ZHMET_prior': (eps, 1.0 - eps),
                                   'ttHhadr_prior': (eps, 1.0 - eps), 'ttHlept_prior': (eps, 1.0 - eps)})
         
    # check if a file with previously evaluated points exists, if so, use them for initialization
    confhandler = ConfigFileHandler()
    evaluations_path = out_dir + 'evaluations.txt'

    if os.path.exists(evaluations_path):
        confhandler.load_configuration(evaluations_path)
        
        ggH_priors_init = []
        WHhadr_priors_init = []
        ZHhadr_priors_init = []
        WHlept_priors_init = []
        ZHlept_priors_init = []
        ZHMET_priors_init = []
        ttHhadr_priors_init = []
        ttHlept_priors_init = []
        targets_init = []

        for section_name in confhandler.get_sections():
            cur_section = confhandler.get_section(section_name)

            targets_init.append(float(cur_section['cost']))
            ggH_priors_init.append(float(cur_section['ggH_prior']))
            WHhadr_priors_init.append(float(cur_section['WHhadr_prior']))
            ZHhadr_priors_init.append(float(cur_section['ZHhadr_prior']))
            WHlept_priors_init.append(float(cur_section['WHlept_prior']))
            ZHlept_priors_init.append(float(cur_section['ZHlept_prior']))
            ZHMET_priors_init.append(float(cur_section['ZHMET_prior']))
            ttHhadr_priors_init.append(float(cur_section['ttHhadr_prior']))
            ttHlept_priors_init.append(float(cur_section['ttHlept_prior']))
            
        init_dict = {'target': targets_init, 'ggH_prior': ggH_priors_init, 'WHhadr_prior': WHhadr_priors_init,
            'ZHhadr_prior': ZHhadr_priors_init, 'WHlept_prior': WHlept_priors_init, 'ZHlept_prior': ZHlept_priors_init,
            'ZHMET_prior': ZHMET_priors_init, 'ttHhadr_prior': ttHhadr_priors_init, 'ttHlept_prior': ttHlept_priors_init}
        
        bo.initialize(init_dict)

    # perform the standard initialization and setup
    bo.maximize(init_points = 6, n_iter = 0, acq = 'ucb', kappa = 3)

    for it in range(500):
        bo.maximize(init_points = 6, n_iter = 1, acq = 'ucb', kappa = 3)
    
        # evaluate the current maximum
        curval = bo.res['max']
        cost = curval['max_val']
        priors = curval['max_params']
    
        confhandler = ConfigFileHandler()
        confhandler.config.optionxform = str
        confhandler.new_section('Priors')
        confhandler.set_field('Priors', 'cost', str(cost))
        confhandler.set_field('Priors', 'VBF_prior', str(1.0))
        
        for key, val in priors.iteritems():
            confhandler.set_field('Priors', key, str(val))
        
        confhandler.save_configuration(out_dir + 'priors.txt')

if __name__ == "__main__":
    main()
