import sys
sys.path.insert(0, '/home/llr/cms/wind/.local/lib/python2.7/site-packages/')

from bayes_opt import BayesianOptimization
from sklearn.gaussian_process.kernels import Matern, ConstantKernel
from subprocess import check_output

from scipy.optimize import minimize

import numpy as np
import matplotlib.pyplot as plt
from matplotlib import gridspec
import os
import math
import re

from trainlib.ConfigFileHandler import ConfigFileHandler

eps = 1e-4
delta = 0.1

# prior values that are going to be optimized
priors_best = {}
priors_best["ggh_prior"] = 1.0
priors_best["vbf_prior"] = 1.0
priors_best["whhadr_prior"] = 0.7
priors_best["zhhadr_prior"] = 0.7
priors_best["whlept_prior"] = 1.0
priors_best["zhlept_prior"] = 1.0
priors_best["tthhadr_prior"] = 1.0
priors_best["tthlept_prior"] = 1.0
priors_best["zhmet_prior"] = 1.0
priors_best["target"] = -7.0

# set the ranges in which to optimize the individual priors
priors_min = {}
priors_min["ggh_prior"] = 1.0
priors_min["tthhadr_prior"] = eps
priors_min["tthlept_prior"] = eps
priors_min["zhlept_prior"] = eps
priors_min["whlept_prior"] = eps
priors_min["zhhadr_prior"] = 0.3
priors_min["whhadr_prior"] = 0.3
priors_min["zhmet_prior"] = eps

priors_max = {}
priors_max["ggh_prior"] = 3.0
priors_max["tthhadr_prior"] = 0.4
priors_max["tthlept_prior"] = 0.3
priors_max["zhlept_prior"] = 0.3
priors_max["whlept_prior"] = 0.3
priors_max["zhhadr_prior"] = 0.8
priors_max["whhadr_prior"] = 0.8
priors_max["zhmet_prior"] = 0.3

# global evaluation counter
evalcnt = 0

# other global settings
run_dir = ""
out_dir = ""
engine = ""
punzi_file = "Mor18_punzi_S_comp.conf"

def load_file(path, keys):
    confhandler = ConfigFileHandler()
    confhandler.load_configuration(path)
    
    retval = {}
    
    for section_name in confhandler.get_sections():
        cur_section = confhandler.get_section(section_name)
        
        for key in keys:
            if not key in retval:
                retval[key] = []
                
            retval[key].append(float(cur_section[key]))
            
    return retval

def gp_fitted(priors, bo):
    priors_list = [priors[key] for key in bo.keys] # this takes care to pass them in the correct order!
    return -bo.gp.predict(np.array(priors_list).reshape(1, -1), return_std = False)[0]

def gp_fitted_wrapper(priors_list, bo):
    # this receives the arguments (priors) as a list, to make it compatible with sklearn's minimization routine
    priors = to_prior_dict(priors_list)
    
    print priors
    
    return gp_fitted(priors, bo)

def to_prior_dict(priors_list):
    priors = {}
    priors["ggh_prior"] = priors_list[0]
    priors["tthhadr_prior"] = priors_list[1]
    priors["tthlept_prior"] = priors_list[2]
    priors["whhadr_prior"] = priors_list[3]
    priors["whlept_prior"] = priors_list[4]
    priors["zhhadr_prior"] = priors_list[5]
    priors["zhlept_prior"] = priors_list[6]
    priors["zhmet_prior"] = priors_list[7]
    
    return priors

def to_prior_list(prior_dict):
    priors = []
    priors.append(prior_dict["ggh_prior"])
    priors.append(prior_dict["tthhadr_prior"])
    priors.append(prior_dict["tthlept_prior"])
    priors.append(prior_dict["whhadr_prior"])
    priors.append(prior_dict["whlept_prior"])
    priors.append(prior_dict["zhhadr_prior"])
    priors.append(prior_dict["zhlept_prior"])
    priors.append(prior_dict["zhmet_prior"])
    
    return priors

def weight_func(delta_p, alpha):
    retval = 0.0
    
    if(delta_p > 0.0):
        retval = 1.0 / alpha * math.tanh(alpha * delta_p)
    else:
        retval = delta_p
        
    retval -= 1.0 / alpha
    
    return retval

def cost_func(delta_pi, alpha, m):
    cost = 0.0
        
    for delta_p in delta_pi:
        cost += math.pow(1.0 / alpha, m) - math.pow(weight_func(delta_p, alpha), m)

    # make it an *average*, since the number of evaluated classes can actually fluctuate
    cost = cost / np.sqrt(len(delta_pi))
        
    return cost

def punzi_target(priors, relevant_classes, params):
    bin_dir = "/home/llr/cms/wind/cmssw/CMSSW_9_4_2/bin/slc6_amd64_gcc630/"
    cost_function_evaluator = "run_prior_evaluator"
    
    output = check_output([bin_dir + cost_function_evaluator, run_dir, out_dir, engine, str(params["min_iterations"]), str(params["max_iterations"]), str(priors["ggh_prior"]), str(priors["whhadr_prior"]), 
              str(priors["zhhadr_prior"]), str(priors["whlept_prior"]), str(priors["zhlept_prior"]), str(priors["zhmet_prior"]), 
              str(priors["tthhadr_prior"]), str(priors["tthlept_prior"])])
    
    # read directly the configuration file containing the relative Punzi improvements w.r.t. the reference 
    # (the one with flat priors)
    punzihandler = ConfigFileHandler()
    punzihandler.load_configuration(os.path.join(out_dir, punzi_file))
    
    costval = 0.0
    
    # compute the utility function over the categories that were specified
    #for relevant_class in relevant_classes:
        #costval += (float(punzihandler.get_field('Punzi', relevant_class)) - 1.0)
     
    # use the weighted cost function
    delta_pi = []
    for relevant_class in relevant_classes:
        delta_pi.append(float(punzihandler.get_field('Punzi', relevant_class)) - 1.0)
    
    costval = cost_func(delta_pi, 8.0, 2)

    if math.isnan(costval):
        print "caught NaN!"
        costval = -7.0
 
    return costval

def save_priors(out_path, priors):
    # combine all the results into the final prior and save it again
    confhandler = ConfigFileHandler()
    confhandler.config.optionxform = str
    confhandler.new_section('Priors')
    confhandler.set_field('Priors', 'VBF_prior', str(1.0))
    confhandler.set_field('Priors', 'ggH_prior', str(priors["ggh_prior"]))
    confhandler.set_field('Priors', 'ttHlept_prior', str(priors["tthlept_prior"]))
    confhandler.set_field('Priors', 'ttHhadr_prior', str(priors["tthhadr_prior"]))
    confhandler.set_field('Priors', 'ZHlept_prior', str(priors["zhlept_prior"]))
    confhandler.set_field('Priors', 'WHlept_prior', str(priors["whlept_prior"]))
    confhandler.set_field('Priors', 'ZHhadr_prior', str(priors["zhhadr_prior"]))
    confhandler.set_field('Priors', 'WHhadr_prior', str(priors["whhadr_prior"]))
    confhandler.set_field('Priors', 'ZHMET_prior', str(priors["zhmet_prior"]))    
    confhandler.save_configuration(out_path)

def save_params(out_path, params, evalcnt):
    confhandler = ConfigFileHandler()
    
    if os.path.exists(out_path):
        confhandler.load_configuration(out_path)
    
    section_name = 'evaluation_' + str(evalcnt)
    confhandler.new_section(section_name)
    
    for key, value in params.iteritems():
        confhandler.set_field(section_name, key, str(value))
     
    confhandler.save_configuration(out_path)
    
def punzi_target_global(ggh_prior, whhadr_prior, zhhadr_prior, whlept_prior, zhlept_prior, zhmet_prior, tthhadr_prior, tthlept_prior):
    global evalcnt
    
    priors = {}
    priors["ggh_prior"] = ggh_prior
    priors["whhadr_prior"] = whhadr_prior
    priors["zhhadr_prior"] = zhhadr_prior
    priors["whlept_prior"] = whlept_prior
    priors["zhlept_prior"] = zhlept_prior
    priors["zhmet_prior"] = zhmet_prior
    priors["tthhadr_prior"] = tthhadr_prior
    priors["tthlept_prior"] = tthlept_prior

    # go for higher quality for the final round of optimization: have three times as many rounds, i.e. the variance on the function output will naively be 3 times smaller
    params = {}
    params["min_iterations"] = 75
    params["max_iterations"] = 150
    
    costval = punzi_target(priors, ["VBF1j", "VBF2j", "ttHlept", "ttHhadr", "VHlept", "VHhadr", "VHMET"], params)
    priors["target"] = costval
    
    save_params(os.path.join(out_dir, 'evaluations_global.txt'), priors, evalcnt)

    # only in this last step have a continuous output of the best prior (since potentially it can be very long)
    if costval > priors_best["target"]:
        priors_best["target"] = costval
        priors_best["ggh_prior"] = priors["ggh_prior"]
        priors_best["tthhadr_prior"] = priors["tthhadr_prior"]
        priors_best["tthlept_prior"] = priors["tthlept_prior"]
        priors_best["zhlept_prior"] = priors["zhlept_prior"]
        priors_best["whlept_prior"] = priors["whlept_prior"]
        priors_best["zhhadr_prior"] = priors["zhhadr_prior"]
        priors_best["whhadr_prior"] = priors["whhadr_prior"]
        priors_best["zhmet_prior"] = priors["zhmet_prior"]

        save_priors(os.path.join(out_dir, 'priors.txt'), priors)

    # also keep updating the fitted maximum of the gaussian process
    postfit_priors = gp_maximum(os.path.join(out_dir, 'evaluations_global.txt'), priors_min, priors_max)

    save_priors(os.path.join(out_dir, 'priors_postfit.txt'), postfit_priors)
    
    evalcnt += 1
    return costval                
    
def punzi_target_ggH(ggh_prior):
    global evalcnt

    priors = {}
    priors["ggh_prior"] = ggh_prior
    priors["whhadr_prior"] = priors_best["whhadr_prior"]
    priors["zhhadr_prior"] = priors_best["zhhadr_prior"]
    priors["whlept_prior"] = priors_best["whlept_prior"]
    priors["zhlept_prior"] = priors_best["zhlept_prior"]
    priors["zhmet_prior"] = priors_best["zhmet_prior"]
    priors["tthhadr_prior"] = priors_best["tthhadr_prior"]
    priors["tthlept_prior"] = priors_best["tthlept_prior"]
    
    # default settings for faster, but noisy, evaluation
    params = {}
    params["min_iterations"] = 25
    params["max_iterations"] = 100

    costval = punzi_target(priors, ["VBF1j", "VBF2j"], params)
    priors["target"] = costval
    
    save_params(os.path.join(out_dir, 'evaluations_ggH.txt'), priors, evalcnt)

    if costval > priors_best["target"]:
        priors_best["target"] = costval
         
    evalcnt += 1
    return costval

def punzi_target_ttH(tthhadr_prior, tthlept_prior):
    global evalcnt
    
    priors = {}
    priors["ggh_prior"] = priors_best["ggh_prior"]
    priors["whhadr_prior"] = priors_best["whhadr_prior"]
    priors["zhhadr_prior"] = priors_best["zhhadr_prior"]
    priors["whlept_prior"] = priors_best["whlept_prior"]
    priors["zhlept_prior"] = priors_best["zhlept_prior"]
    priors["zhmet_prior"] = priors_best["zhmet_prior"]
    priors["tthhadr_prior"] = tthhadr_prior
    priors["tthlept_prior"] = tthlept_prior

    # default settings for faster, but noisy, evaluation
    params = {}
    params["min_iterations"] = 25
    params["max_iterations"] = 100
    
    costval = punzi_target(priors, ["ttHlept", "ttHhadr"], params)
    priors["target"] = costval
    
    save_params(os.path.join(out_dir, 'evaluations_ttH.txt'), priors, evalcnt)

    if costval > priors_best["target"]:
        priors_best["target"] = costval
             
    evalcnt += 1
    return costval

def punzi_target_VHlept(zhlept_prior, whlept_prior):
    global evalcnt
        
    priors = {}
    priors["ggh_prior"] = priors_best["ggh_prior"]
    priors["whhadr_prior"] = priors_best["whhadr_prior"]
    priors["zhhadr_prior"] = priors_best["zhhadr_prior"]
    priors["whlept_prior"] = whlept_prior
    priors["zhlept_prior"] = zhlept_prior
    priors["zhmet_prior"] = priors_best["zhmet_prior"]
    priors["tthhadr_prior"] = priors_best["tthhadr_prior"]
    priors["tthlept_prior"] = priors_best["tthlept_prior"]

    # go for higher quality for the final round of optimization
    params = {}
    params["min_iterations"] = 25
    params["max_iterations"] = 100

    costval = punzi_target(priors, ["VHlept"], params)
    priors["target"] = costval
    
    save_params(os.path.join(out_dir, 'evaluations_VHlept.txt'), priors, evalcnt)

    if costval > priors_best["target"]:
        priors_best["target"] = costval
         
    evalcnt += 1
    return costval

def punzi_target_VHhadr(zhhadr_prior, whhadr_prior):
    global evalcnt
 
    priors = {}
    priors["ggh_prior"] = priors_best["ggh_prior"]
    priors["whhadr_prior"] = whhadr_prior
    priors["zhhadr_prior"] = zhhadr_prior
    priors["whlept_prior"] = priors_best["whlept_prior"]
    priors["zhlept_prior"] = priors_best["zhlept_prior"]
    priors["zhmet_prior"] = priors_best["zhmet_prior"]
    priors["tthhadr_prior"] = priors_best["tthhadr_prior"]
    priors["tthlept_prior"] = priors_best["tthlept_prior"]

    # go for higher quality for the final round of optimization
    params = {}
    params["min_iterations"] = 25
    params["max_iterations"] = 100

    costval = punzi_target(priors, ["VHhadr"], params)
    priors["target"] = costval
    
    save_params(os.path.join(out_dir, 'evaluations_VHhadr.txt'), priors, evalcnt)

    if costval > priors_best["target"]:
        priors_best["target"] = costval
         
    evalcnt += 1
    return costval

def punzi_target_ZHMET(zhmet_prior):
    global evalcnt
    
    priors = {}
    priors["ggh_prior"] = priors_best["ggh_prior"]
    priors["whhadr_prior"] = priors_best["whhadr_prior"]
    priors["zhhadr_prior"] = priors_best["zhhadr_prior"]
    priors["whlept_prior"] = priors_best["whlept_prior"]
    priors["zhlept_prior"] = priors_best["zhlept_prior"]
    priors["zhmet_prior"] = zhmet_prior
    priors["tthhadr_prior"] = priors_best["tthhadr_prior"]
    priors["tthlept_prior"] = priors_best["tthlept_prior"]

    # go for higher quality for the final round of optimization
    params = {}
    params["min_iterations"] = 25
    params["max_iterations"] = 100

    costval = punzi_target(priors, ["VHMET"], params)  
    priors["target"] = costval
    
    save_params(os.path.join(out_dir, 'evaluations_VHMET.txt'), priors, evalcnt)

    if costval > priors_best["target"]:
        priors_best["target"] = costval
         
    evalcnt += 1
    return costval

def run_bayesian_optimization(name, eval_file, target, var_ranges, init_points, max_iterations, patience, alpha):
    global evalcnt
    evalcnt = 0
    
    print "now optimizing the following variables: " + str(var_ranges)
    print "alpha = " + str(alpha)

    # change the kernel to have a length scale more appropriate to this function
    # alpha ... corresponds to the value added to the diagonal elements of the covariance matrix <-> the approximate noise level in the observations
    gp_params = {'kernel': ConstantKernel(1.0, (1e-8, 1e2)) * Matern(length_scale = 0.01, length_scale_bounds = (1e-5, 1e5), nu = 1.5),
                 'alpha': alpha}

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
                # only take those variables that are actually relevant
                if key in var_ranges or key == "target":
                    if key not in init_dict:
                        init_dict[key] = []
                    
                    init_dict[key].append(float(value))
                
        evalcnt = int(re.sub('evaluation_', '', confhandler.get_sections()[-1])) + 1
        print "resuming " + name + " at evaluation " + str(evalcnt)
        
        init_points_loaded = len(init_dict["target"])
        print "found " + str(init_points_loaded) + " initialization points: " + str(init_dict)
        
        bo.initialize(init_dict)
        bo.maximize(init_points = max(0, init_points - init_points_loaded), n_iter = 0, acq = 'poi', kappa = 3, xi = xi_scheduler(0.0, max_iterations), **gp_params)
        print "initialization done"
    else:
        bo.maximize(init_points = init_points, n_iter = 0, acq = 'poi', kappa = 3, xi = xi_scheduler(0.0, max_iterations), **gp_params)
    
    cur_iteration = 1
    patience_cnt = 0
    best_cost = -7.0
    
    for it in range(max_iterations): 
        cur_xi = xi_scheduler(cur_iteration, max_iterations)
        print "cur_iteration = " + str(cur_iteration) + ", using xi = " + str(cur_xi)

        cur_iteration += 1
        
        bo.maximize(init_points = 0, n_iter = 1, acq = 'poi', kappa = 3, xi = cur_xi, **gp_params)

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

def gp_maximum(path, priors_min, priors_max):
    print "computing fitted GP maximum"

    keys = priors_min.keys() + ["target"]

    print keys

    # now, as the very last step, also find and save the maximum of the fitted Gaussian process
    init_dict = load_file(path, keys) 

    # dummy function
    target = lambda x: x

    gp_params = {'kernel': 1.0 * Matern(length_scale = 0.01, length_scale_bounds = (1e-5, 1e5), nu = 1.5),
                 'alpha':5e-7}

    prior_dict = {key: (priors_min[key], priors_max[key]) for key in priors_min.keys()}

    bo = BayesianOptimization(target, prior_dict)
    
    # fit the gaussian process to the data just read back
    bo.initialize(init_dict)
    bo.maximize(init_points=0, n_iter=0, acq='poi', kappa=5, xi = 0.1, **gp_params)
    bo.gp.fit(bo.X, bo.Y)
    
    # start in the center of the final optimization volume
    p0 = to_prior_list({key: 0.5 * (priors_min[key] + priors_max[key]) for key in priors_min.keys()})
    
    # find the maximum of the fitted gaussian process as a more robust estimate for the true prior
    res = minimize(gp_fitted_wrapper, p0, args = (bo), method = 'Nelder-Mead', tol = 0.001)

    postfit_priors = to_prior_dict(res["x"])

    return postfit_priors


def xi_scheduler(iteration, max_iterations):
    return 0.001 + 0.19 * np.exp(-2.0 * float(iteration) / float(max_iterations))

def main():
    global priors_best
    
    global run_dir
    global out_dir
    global engine
    
    if len(sys.argv) != 4:
        print "Error: exactly 3 arguments are required"
        
    run_dir = sys.argv[1]
    out_dir = sys.argv[2]
    engine = sys.argv[3]

    print run_dir
    print out_dir
    print engine

    # start by fixing the ggH_prior by optimizing Punzi in the two VBF categories
    res = run_bayesian_optimization("ggH", "evaluations_ggH.txt", punzi_target_ggH, {'ggh_prior': (priors_min["ggh_prior"], priors_max["ggh_prior"])}, 
                                init_points = 2, max_iterations = 30, patience = 40, alpha = 1.5e-6)
    priors_best["ggh_prior"] = res["ggh_prior"]
    
    # continue with the next ones to be fixed: ttHh and ttHl
    res = run_bayesian_optimization("ttH", "evaluations_ttH.txt", punzi_target_ttH, {'tthhadr_prior': (priors_min["tthhadr_prior"], priors_max["tthhadr_prior"]), 
                                                                                     'tthlept_prior': (priors_min["tthlept_prior"], priors_max["tthlept_prior"])}, 
                                 init_points = 10, max_iterations = 40, patience = 40, alpha = 1.5e-6)
    priors_best["tthhadr_prior"] = res["tthhadr_prior"]
    priors_best["tthlept_prior"] = res["tthlept_prior"]
    
    # then proceed with ZHlept and WHlept
    res = run_bayesian_optimization("VHlept", "evaluations_VHlept.txt", punzi_target_VHlept, {'zhlept_prior': (priors_min["zhlept_prior"], priors_max["zhlept_prior"]),
                                                                                              'whlept_prior': (priors_min["whlept_prior"], priors_max["whlept_prior"])}, 
                                 init_points = 10, max_iterations = 40, patience = 40, alpha = 1.5e-6)
    priors_best["zhlept_prior"] = res["zhlept_prior"]
    priors_best["whlept_prior"] = res["whlept_prior"]
    
    # then proceed with ZHhadr and WHhadr
    res = run_bayesian_optimization("VHhadr", "evaluations_VHhadr.txt", punzi_target_VHhadr, {'zhhadr_prior': (priors_min["zhhadr_prior"], priors_max["zhhadr_prior"]),
                                                                                              'whhadr_prior': (priors_min["whhadr_prior"], priors_max["whhadr_prior"])}, 
                                  init_points = 10, max_iterations = 40, patience = 40, alpha = 1.5e-6)
    priors_best["zhhadr_prior"] = res["zhhadr_prior"]
    priors_best["whhadr_prior"] = res["whhadr_prior"]
    
    # then finish off with ZHMET
    res = run_bayesian_optimization("VHMET", "evaluations_VHMET.txt", punzi_target_ZHMET, {'zhmet_prior': (priors_min["zhmet_prior"], priors_max["zhmet_prior"])}, 
                                  init_points = 2, max_iterations = 30, patience = 30, alpha = 1.5e-6)
    priors_best["zhmet_prior"] = res["zhmet_prior"]
    
    save_priors(os.path.join(out_dir, 'priors_sequential.txt'), priors_best)

    # priors_best["ggh_prior"] = 1.3
    # priors_best["tthhadr_prior"] = 0.2
    # priors_best["tthlept_prior"] = 0.04
    # priors_best["zhlept_prior"] = 0.03
    # priors_best["zhhadr_prior"] = 0.81
    # priors_best["whlept_prior"] = 0.19
    # priors_best["whhadr_prior"] = 0.73
    # priors_best["zhmet_prior"] = 0.15

    # at the end, optimize globally again, tweaking all priors simultaneously, but in a very small region set by the current best guess from the sequential algorithm
    res = run_bayesian_optimization("global", "evaluations_global.txt", punzi_target_global, 
                                    {'ggh_prior': (max(priors_min["ggh_prior"], priors_best["ggh_prior"] - delta), min(priors_max["ggh_prior"], priors_best["ggh_prior"] + delta)),
                                     'tthhadr_prior': (max(priors_min["tthhadr_prior"], priors_best["tthhadr_prior"] - delta), min(priors_max["tthhadr_prior"], priors_best["tthhadr_prior"] + delta)),
                                     'tthlept_prior': (max(priors_min["tthlept_prior"], priors_best["tthlept_prior"] - delta), min(priors_max["tthlept_prior"], priors_best["tthlept_prior"] + delta)),
                                     'zhlept_prior': (max(priors_min["zhlept_prior"], priors_best["zhlept_prior"] - delta), min(priors_max["zhlept_prior"], priors_best["zhlept_prior"] + delta)),
                                     'whlept_prior': (max(priors_min["whlept_prior"], priors_best["whlept_prior"] - delta), min(priors_max["whlept_prior"], priors_best["whlept_prior"] + delta)),
                                     'zhhadr_prior': (max(priors_min["zhhadr_prior"], priors_best["zhhadr_prior"] - delta), min(priors_max["zhhadr_prior"], priors_best["zhhadr_prior"] + delta)),
                                     'whhadr_prior': (max(priors_min["whhadr_prior"], priors_best["whhadr_prior"] - delta), min(priors_max["whhadr_prior"], priors_best["whhadr_prior"] + delta)),
                                     'zhmet_prior': (max(priors_min["zhmet_prior"], priors_best["zhmet_prior"] - delta), min(priors_max["zhmet_prior"], priors_best["zhmet_prior"] + delta))},
                                    init_points = 20, max_iterations = 80, patience = 70, alpha = 5e-7)

    priors_best["ggh_prior"] = res["ggh_prior"]
    priors_best["tthhadr_prior"] = res["tthhadr_prior"]
    priors_best["tthlept_prior"] = res["tthlept_prior"]
    priors_best["zhlept_prior"] = res["zhlept_prior"]
    priors_best["whlept_prior"] = res["whlept_prior"]
    priors_best["zhhadr_prior"] = res["zhhadr_prior"]
    priors_best["whhadr_prior"] = res["whhadr_prior"]
    priors_best["zhmet_prior"] = res["zhmet_prior"]
    
    save_priors(os.path.join(out_dir, 'priors.txt'), priors_best)

    postfit_priors = gp_maximum(os.path.join(out_dir, 'evaluations_global.txt'), priors_min, priors_max)

    save_priors(os.path.join(out_dir, 'priors_postfit.txt'), postfit_priors)

if __name__ == "__main__":
    main()
