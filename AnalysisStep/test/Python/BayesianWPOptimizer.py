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
WP_VBF2j_default = 0.46386
WP_VBF1j_default = 0.37605
WP_WHh_default = 0.88384
WP_ZHh_default = 0.91315

def xi_scheduler(iteration):
    return 0.01 + 0.19 * np.exp(-0.01 * iteration)

def main():
    global evalcnt

    if len(sys.argv) != 4:
        print "Error: exactly 3 arguments are required"
        
    ref_dir = sys.argv[1]
    out_dir = sys.argv[2]
    lumi = float(sys.argv[3])

    print ref_dir
    print out_dir
    print lumi

    def punzi_target(WP_VBF2j, WP_VBF1j, WP_WHh, WP_ZHh):
        global evalcnt

        bin_dir = "/home/llr/cms/wind/cmssw/CMSSW_9_4_2/bin/slc6_amd64_gcc630/"
        cost_function_evaluator = "run_WP_evaluator"
    
        output = check_output([bin_dir + cost_function_evaluator, ref_dir, out_dir, str(lumi), str(WP_VBF2j), str(WP_VBF1j), str(WP_WHh), str(WP_ZHh)])
    
        costval = 0.0

        for line in output.split('\n'):
            if "cost = " in line:
                costval = float(line.replace("cost = ", ""))
                break
            
        if math.isnan(costval):
            costval = -8.75

        # save the sampled point such that later they can be used as exploration points (if the need occurs)
        confhandler = ConfigFileHandler()
        evaluations_path = out_dir + 'evaluations.txt'

        if os.path.exists(evaluations_path):
            confhandler.load_configuration(evaluations_path)

        print "saving evaluation for iteration " + str(evalcnt)
        
        section_name = 'evaluation_' + str(evalcnt)
        confhandler.new_section(section_name)
        confhandler.set_field(section_name, 'cost', str(costval))
        confhandler.set_field(section_name, 'WP_VBF2j', str(WP_VBF2j))
        confhandler.set_field(section_name, 'WP_VBF1j', str(WP_VBF1j))
        confhandler.set_field(section_name, 'WP_WHh', str(WP_WHh))
        confhandler.set_field(section_name, 'WP_ZHh', str(WP_ZHh))

        confhandler.save_configuration(evaluations_path)

        evalcnt += 1

        return costval
    
    eps = 1e-3
    delta = 0.2
    bo = BayesianOptimization(punzi_target, {'WP_VBF2j': (eps, 1.0 - eps), 'WP_VBF1j': (eps, 1.0 - eps),
                                             'WP_WHh': (eps, 1.0 - eps), 'WP_ZHh': (eps, 1.0 - eps)})
         
    # check if a file with previously evaluated points exists, if so, use them for initialization
    confhandler = ConfigFileHandler()
    evaluations_path = out_dir + 'evaluations.txt'

    if os.path.exists(evaluations_path):
        confhandler.load_configuration(evaluations_path)

        targets_init = []
        WP_VBF2j_init = []
        WP_VBF1j_init = []
        WP_WHh_init = []
        WP_ZHh_init = []

        for section_name in confhandler.get_sections():
            cur_section = confhandler.get_section(section_name)

            targets_init.append(float(cur_section['cost']))
            WP_VBF2j_init.append(float(cur_section['WP_VBF2j']))
            WP_VBF1j_init.append(float(cur_section['WP_VBF1j']))
            WP_WHh_init.append(float(cur_section['WP_WHh']))
            WP_ZHh_init.append(float(cur_section['WP_ZHh']))
            
        init_dict = {'target': targets_init, 'WP_VBF2j': WP_VBF2j_init, 'WP_VBF1j': WP_VBF1j_init,
                     'WP_WHh': WP_WHh_init, 'WP_ZHh': WP_ZHh_init}
        
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
        cur_xi = xi_scheduler(cur_iteration)
        cur_iteration += 1
        print "using xi = " + str(cur_xi)
        
        bo.maximize(init_points = 6, n_iter = 1, acq = 'poi', kappa = 3, xi = cur_xi, **gp_params)
    
        # evaluate the current maximum
        curval = bo.res['max']
        cost = curval['max_val']
        WPs = curval['max_params']
    
        confhandler = ConfigFileHandler()
        confhandler.config.optionxform = str
        confhandler.new_section('WPs')
        confhandler.set_field('WPs', 'cost', str(cost))
        
        for key, val in WPs.iteritems():
            confhandler.set_field('WPs', key, str(val))
        
        confhandler.save_configuration(out_dir + 'WPs.txt')

if __name__ == "__main__":
    main()
