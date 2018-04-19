import sys
sys.path.insert(0, '/home/llr/cms/wind/.local/lib/python2.7/site-packages/')

from bayes_opt import BayesianOptimization
from sklearn.gaussian_process.kernels import Matern
import numpy as np

import matplotlib.pyplot as plt
from matplotlib import gridspec
from subprocess import check_output
from trainlib.ConfigFileHandler import ConfigFileHandler
import re
import os
import math

evalcnt = 0

# default values
VBF_prior_default = 1.0
ggH_prior_default = 1.4665
WHhadr_prior_default = 0.582676
ZHhadr_prior_default = 0.707539
WHlept_prior_default = 0.172772
ZHlept_prior_default = 0.0633419
ZHMET_prior_default = 0.0666923
ttHhadr_prior_default = 0.145215
ttHlept_prior_default = 0.1954

def xi_scheduler(iteration):
    return 0.01 + 0.19 * np.exp(-0.02 * iteration)

def main():
    global evalcnt

    if len(sys.argv) != 4:
        print "Error: exactly 3 arguments are required"
        
    run_dir = sys.argv[1]
    out_dir = sys.argv[2]
    engine = sys.argv[3]

    print run_dir
    print out_dir
    print engine

    # punzi_target_2d = lambda WHlept_prior, ZHlept_prior: punzi_target(ggH_prior_default, WHhadr_prior_default, ZHhadr_prior_default,
    #                                                                       WHlept_prior, ZHlept_prior, ZHMET_prior_default, 
    #                                                                       ttHhadr_prior_default, ttHlept_prior_default)

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
            costval = -7.0

        # save the sampled point such that later they can be used as exploration points (if the need occurs)
        confhandler = ConfigFileHandler()
        evaluations_path = out_dir + 'evaluations.txt'

        if os.path.exists(evaluations_path):
            confhandler.load_configuration(evaluations_path)

        print "saving evaluation for iteration " + str(evalcnt)
        
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
    delta = 0.2
    bo = BayesianOptimization(punzi_target, {'ggH_prior': (1.1, 1.8), 'WHhadr_prior': (0.2, 0.8), 
                                   'ZHhadr_prior': (0.2, 0.8), 'WHlept_prior': (0.2, 0.8),
                                   'ZHlept_prior': (eps, 0.3), 'ZHMET_prior': (eps, 0.3),
                                   'ttHhadr_prior': (0.05, 0.25), 'ttHlept_prior': (0.05, 0.25)})

    # bo = BayesianOptimization(punzi_target_2d, {'WHlept_prior': (eps, WHlept_prior_default + delta),
    #                                                  'ZHlept_prior': (eps, ZHlept_prior_default + delta)})
         
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
        
        evalcnt = int(re.sub('evaluation_', '', confhandler.get_sections()[-1])) + 1

        print "resuming at evaluation " + str(evalcnt)
        
        bo.initialize(init_dict)
        initialized = True
    else:
        initialized = False
            
        
    # change the kernel to have a length scale more appropriate to this function
    gp_params = {'kernel': 1.0 * Matern(length_scale = 0.05, length_scale_bounds = (1e-5, 1e5), nu = 1.5),
                 'alpha': 1e-5}

    # perform the standard initialization and setup
    if initialized:
        bo.maximize(init_points = 0, n_iter = 0, acq = 'poi', kappa = 3, xi = xi_scheduler(0.0), **gp_params)
    else:
        bo.maximize(init_points = 6, n_iter = 0, acq = 'poi', kappa = 3, xi = xi_scheduler(0.0), **gp_params)

    cur_iteration = 1
    for it in range(1000):
        cur_iteration += 1
        
        cur_xi = xi_scheduler(cur_iteration)
        print "using xi = " + str(cur_xi)
        
        bo.maximize(init_points = 6, n_iter = 1, acq = 'poi', kappa = 3, xi = cur_xi, **gp_params)
    
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
