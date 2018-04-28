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

run_dir = ""
out_dir = ""
engine = ""
punzi_file = "Mor18_punzi_S_comp.conf"

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
         
    # use the weighted cost function
    delta_pi = []
    for relevant_class in relevant_classes:
        delta_pi.append(float(punzihandler.get_field('Punzi', relevant_class)) - 1.0)
    
    costval = cost_func(delta_pi, 8.0, 2)

    if math.isnan(costval):
        print "caught NaN!"
        costval = -7.0
 
    return costval


def main():
    global run_dir
    global out_dir
    global engine

    if len(sys.argv) != 5:
        print "Error: exactly 3 arguments are required"
        
    run_dir = sys.argv[1]
    out_dir = sys.argv[2]
    engine = sys.argv[3]
    number_runs = int(sys.argv[4])

    evaluations = []

    priors = {}
    priors["ggh_prior"] = 1.0
    priors["whhadr_prior"] = 1.0
    priors["zhhadr_prior"] = 1.0
    priors["whlept_prior"] = 1.0
    priors["zhlept_prior"] = 1.0
    priors["zhmet_prior"] = 1.0
    priors["tthhadr_prior"] = 1.0
    priors["tthlept_prior"] = 1.0

    params = {}
    params["min_iterations"] = 25
    params["max_iterations"] = 100

    for i in range(number_runs):
        cur_eval = punzi_target(priors, ["VBF1j", "VBF2j"], params)
        evaluations.append(cur_eval)
        print "evaluation " + str(i) + ": " + str(cur_eval)

    print "mean = " + str(np.mean(evaluations))
    print "var = " + str(np.var(evaluations))

if __name__ == "__main__":
    main()
