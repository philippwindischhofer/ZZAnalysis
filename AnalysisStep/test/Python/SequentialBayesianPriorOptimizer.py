import sys
sys.path.insert(0, '/home/llr/cms/wind/.local/lib/python2.7/site-packages/')

from bayes_opt import BayesianOptimization
from sklearn.gaussian_process.kernels import Matern
from subprocess import check_output

import numpy as np
import matplotlib.pyplot as plt
from matplotlib import gridspec
import os
import math
import re

from trainlib.ConfigFileHandler import ConfigFileHandler

# prior values that are going to be optimized
ggh_prior_best = 1.0
vbf_prior_best = 1.0
whhadr_prior_best = 1.0
zhhadr_prior_best = 1.0
whlept_prior_best = 1.0
zhlept_prior_best = 1.0
tthhadr_prior_best = 1.0
tthlept_prior_best = 1.0
zhmet_prior_best = 1.0

# global evaluation counter
evalcnt = 0

# other global settings
run_dir = ""
out_dir = ""
engine = ""
punzi_file = "Mor18_punzi_S_comp.conf"

def punzi_target(priors, relevant_classes):
    bin_dir = "/home/llr/cms/wind/cmssw/CMSSW_9_4_2/bin/slc6_amd64_gcc630/"
    cost_function_evaluator = "run_prior_evaluator"
    
    output = check_output([bin_dir + cost_function_evaluator, run_dir, out_dir, engine, str(priors["ggh_prior"]), str(priors["whhadr_prior"]), 
              str(priors["zhhadr_prior"]), str(priors["whlept_prior"]), str(priors["zhlept_prior"]), str(priors["zhmet_prior"]), 
              str(priors["tthhadr_prior"]), str(priors["tthlept_prior"])])
    
    # read directly the configuration file containing the relative Punzi improvements w.r.t. the reference 
    # (the one with flat priors)
    punzihandler = ConfigFileHandler()
    punzihandler.load_configuration(os.path.join(out_dir, punzi_file))
    
    costval = 0.0
    
    # compute the utility function over the categories that were specified
    for relevant_class in relevant_classes:
        costval += (float(punzihandler.get_field('Punzi', relevant_class)) - 1.0)
 
    return costval

def save_params(out_path, params, evalcnt):
    confhandler = ConfigFileHandler()
    
    if os.path.exists(out_path):
        confhandler.load_configuration(out_path)
    
    section_name = 'evaluation_' + str(evalcnt)
    confhandler.new_section(section_name)
    
    for key, value in params.iteritems():
        confhandler.set_field(section_name, key, str(value))
     
    confhandler.save_configuration(out_path)
    
def punzi_target_ggH(ggh_prior):
    global evalcnt

    priors = {}
    priors["ggh_prior"] = ggh_prior
    priors["whhadr_prior"] = whhadr_prior_best
    priors["zhhadr_prior"] = zhhadr_prior_best
    priors["whlept_prior"] = whlept_prior_best
    priors["zhlept_prior"] = zhlept_prior_best
    priors["zhmet_prior"] = zhmet_prior_best
    priors["tthhadr_prior"] = tthhadr_prior_best
    priors["tthlept_prior"] = tthlept_prior_best
    
    costval = punzi_target(priors, ["VBF1j", "VBF2j"])
    priors["target"] = costval
    
    save_params(os.path.join(out_dir, 'evaluations_ggH.txt'), priors, evalcnt)
         
    evalcnt += 1
    return costval

def punzi_target_ttH(tthhadr_prior, tthlept_prior):
    global evalcnt
    
    priors = {}
    priors["ggh_prior"] = ggh_prior_best
    priors["whhadr_prior"] = whhadr_prior_best
    priors["zhhadr_prior"] = zhhadr_prior_best
    priors["whlept_prior"] = whlept_prior_best
    priors["zhlept_prior"] = zhlept_prior_best
    priors["zhmet_prior"] = zhmet_prior_best
    priors["tthhadr_prior"] = tthhadr_prior
    priors["tthlept_prior"] = tthlept_prior
    
    costval = punzi_target(priors, ["ttHlept", "ttHhadr"])
    priors["target"] = costval
    
    save_params(os.path.join(out_dir, 'evaluations_ttH.txt'), priors, evalcnt)
             
    evalcnt += 1
    return costval

def punzi_target_VHlept(zhlept_prior, whlept_prior):
    global evalcnt
        
    priors = {}
    priors["ggh_prior"] = ggh_prior_best
    priors["whhadr_prior"] = whhadr_prior_best
    priors["zhhadr_prior"] = zhhadr_prior_best
    priors["whlept_prior"] = whlept_prior
    priors["zhlept_prior"] = zhlept_prior
    priors["zhmet_prior"] = zhmet_prior_best
    priors["tthhadr_prior"] = tthhadr_prior_best
    priors["tthlept_prior"] = tthlept_prior_best
    
    costval = punzi_target(priors, ["VHlept"])
    priors["target"] = costval
    
    save_params(os.path.join(out_dir, 'evaluations_VHlept.txt'), priors, evalcnt)
         
    evalcnt += 1
    return costval

def punzi_target_VHhadr(zhhadr_prior, whhadr_prior):
    global evalcnt
 
    priors = {}
    priors["ggh_prior"] = ggh_prior_best
    priors["whhadr_prior"] = whhadr_prior
    priors["zhhadr_prior"] = zhhadr_prior
    priors["whlept_prior"] = whlept_prior_best
    priors["zhlept_prior"] = zhlept_prior_best
    priors["zhmet_prior"] = zhmet_prior_best
    priors["tthhadr_prior"] = tthhadr_prior_best
    priors["tthlept_prior"] = tthlept_prior_best

    costval = punzi_target(priors, ["VHhadr"])
    priors["target"] = costval
    
    save_params(os.path.join(out_dir, 'evaluations_VHhadr.txt'), priors, evalcnt)
         
    evalcnt += 1
    return costval

def punzi_target_ZHMET(zhmet_prior):
    global evalcnt
    
    priors = {}
    priors["ggh_prior"] = ggh_prior_best
    priors["whhadr_prior"] = whhadr_prior_best
    priors["zhhadr_prior"] = zhhadr_prior_best
    priors["whlept_prior"] = whlept_prior_best
    priors["zhlept_prior"] = zhlept_prior_best
    priors["zhmet_prior"] = zhmet_prior
    priors["tthhadr_prior"] = tthhadr_prior_best
    priors["tthlept_prior"] = tthlept_prior_best

    costval = punzi_target(priors, ["VHMET"])  
    priors["target"] = costval
    
    save_params(os.path.join(out_dir, 'evaluations_VHMET.txt'), priors, evalcnt)
         
    evalcnt += 1
    return costval

def run_bayesian_optimization(name, eval_file, target, var_ranges, init_points, max_iterations, patience):
    global evalcnt
    evalcnt = 0
    
    # change the kernel to have a length scale more appropriate to this function
    # alpha ... corresponds to the value added to the diagonal elements of the covariance matrix <-> the approximate noise level in the observations
    gp_params = {'kernel': 1.0 * Matern(length_scale = 0.01, length_scale_bounds = (1e-5, 1e5), nu = 1.5),
                 'alpha': 1e-4}

    bo = BayesianOptimization(target, var_ranges)
    
    # check if a file with previous evaluations of this utility function already exists, if so, use it for initialization
    evaluations_path = os.path.join(out_dir, eval_file)
    
    if os.path.exists(evaluations_path):
        confhandler = ConfigFileHandler()
        confhandler.load_configuration(evaluations_path)
        
        init_dict = {}
        
        for section_name in confhandler.get_sections():
            cur_section = confhandler.get_section(section_name)
            
            for key, value in cur_section.iteritems():
                if key not in init_dict:
                    init_dict[key] = []
                    
                init_dict[key].append(float(value))
                
        evalcnt = int(re.sub('evaluation_', '', confhandler.get_sections()[-1])) + 1
        print "resuming " + name + " at evaluation " + str(evalcnt)
        
        print init_dict
        
        bo.initialize(init_dict)
        bo.maximize(init_points = init_points, n_iter = 0, acq = 'poi', kappa = 3, xi = xi_scheduler(0.0, max_iterations), **gp_params)
    else:
        bo.maximize(init_points = init_points, n_iter = 0, acq = 'poi', kappa = 3, xi = xi_scheduler(0.0, max_iterations), **gp_params)
    
    cur_iteration = 1
    patience_cnt = 0
    best_cost = -7.0
    
    for it in range(max_iterations):
        cur_iteration += 1
        
        cur_xi = xi_scheduler(cur_iteration, max_iterations)
        print "using xi = " + str(cur_xi)
        
        bo.maximize(init_points = 2, n_iter = 1, acq = 'poi', kappa = 3, xi = cur_xi, **gp_params)

        # evaluate the current maximum
        curval = bo.res['max']
        cost = curval['max_val']
        curparams = curval['max_params']
    
        confhandler = ConfigFileHandler()
        confhandler.config.optionxform = str
        confhandler.new_section(name)
        confhandler.set_field(name, 'target', str(cost))
        
        for key, val in curparams.iteritems():
            confhandler.set_field(name, key, str(val))
        
        confhandler.save_configuration(os.path.join(out_dir, name + '.txt'))
        
        # check if it is time to stop this optimization
        if(cost > best_cost):
            best_cost = cost
            patience_cnt = 0
            
        patience_cnt += 1
        
        if(patience_cnt > patience):
            break
            
    return curparams

def xi_scheduler(iteration, max_iterations):
    return 0.001 + 0.19 * np.exp(-2.0 * float(iteration) / float(max_iterations))

def main():
    global ggh_prior_best
    global tthhadr_prior_best
    global tthlept_prior_best
    global zhlept_prior_best
    global whlept_prior_best
    global zhhadr_prior_best
    global whhadr_prior_best
    global zhmet_prior_best
    
    global run_dir
    global out_dir
    global engine
    
    eps = 1e-2
    delta = 0.2
    
    if len(sys.argv) != 4:
        print "Error: exactly 3 arguments are required"
        
    run_dir = sys.argv[1]
    out_dir = sys.argv[2]
    engine = sys.argv[3]

    print run_dir
    print out_dir
    print engine
    
    # start by fixing the ggH_prior by optimizing Punzi in the two VBF categories
    res = run_bayesian_optimization("ggH", "evaluations_ggH.txt", punzi_target_ggH, {'ggh_prior': (1.0, 1.7)}, 
                                init_points = 2, max_iterations = 30, patience = 30)
    ggh_prior_best = res["ggh_prior"]
    
    # continue with the next ones to be fixed: ttHh and ttHl
    res = run_bayesian_optimization("ttH", "evaluations_ttH.txt", punzi_target_ttH, {'tthhadr_prior': (eps, 0.3), 
                                                                                     'tthlept_prior': (eps, 0.3)}, 
                                 init_points = 20, max_iterations = 40, patience = 40)
    tthhadr_prior_best = res["tthhadr_prior"]
    tthlept_prior_best = res["tthlept_prior"]
    
    # then proceed with ZHlept and WHlept
    res = run_bayesian_optimization("VHlept", "evaluations_VHlept.txt", punzi_target_VHlept, {'zhlept_prior': (eps, 0.3),
                                                                                              'whlept_prior': (eps, 0.3)}, 
                                 init_points = 20, max_iterations = 40, patience = 40)
    zhlept_prior_best = res["zhlept_prior"]
    whlept_prior_best = res["whlept_prior"]
    
    # then proceed with ZHhadr and WHhadr
    res = run_bayesian_optimization("VHhadr", "evaluations_VHhadr.txt", punzi_target_VHhadr, {'zhhadr_prior': (eps, 0.3),
                                                                                              'whhadr_prior': (eps, 0.3)}, 
                                  init_points = 20, max_iterations = 40, patience = 40)
    zhhadr_prior_best = res["zhhadr_prior"]
    whhadr_prior_best = res["whhadr_prior"]
    
    # then finish off with ZHMET
    res = run_bayesian_optimization("VHMET", "evaluations_VHMET.txt", punzi_target_ZHMET, {'zhmet_prior': (eps, 0.3)}, 
                                  init_points = 2, max_iterations = 30, patience = 30)
    zhmet_prior_best = res["zhmet_prior"]
    
    # combine all the results into the final prior
    confhandler = ConfigFileHandler()
    confhandler.config.optionxform = str
    confhandler.new_section('Priors')
    confhandler.set_field('Priors', 'VBF_prior', str(1.0))
    confhandler.set_field('Priors', 'ggH_prior', str(ggh_prior_best))
    confhandler.set_field('Priors', 'ttHlept_prior', str(tthlept_prior_best))
    confhandler.set_field('Priors', 'ttHhadr_prior', str(tthhadr_prior_best))
    confhandler.set_field('Priors', 'ZHlept_prior', str(zhlept_prior_best))
    confhandler.set_field('Priors', 'WHlept_prior', str(whlept_prior_best))
    confhandler.set_field('Priors', 'ZHhadr_prior', str(zhhadr_prior_best))
    confhandler.set_field('Priors', 'WHhadr_prior', str(whhadr_prior_best))
    confhandler.set_field('Priors', 'ZHMET_prior', str(zhmet_prior_best))
    
    confhandler.save_configuration(os.path.join(out_dir, 'priors.txt'))

if __name__ == "__main__":
    main()