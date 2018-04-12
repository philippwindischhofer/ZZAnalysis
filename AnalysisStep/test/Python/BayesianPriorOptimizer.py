import sys
sys.path.insert(0, '/home/llr/cms/wind/.local/lib/python2.7/site-packages/')

from bayes_opt import BayesianOptimization
import numpy as np

import matplotlib.pyplot as plt
from matplotlib import gridspec
from subprocess import check_output
from trainlib.ConfigFileHandler import ConfigFileHandler

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
        bin_dir = run_dir + "../bin/"
        cost_function_evaluator = "run_prior_evaluator"
    
        output = check_output([bin_dir + cost_function_evaluator, run_dir, out_dir, engine, str(ggH_prior), str(WHhadr_prior), 
                               str(ZHhadr_prior), str(WHlept_prior), str(ZHlept_prior), str(ZHMET_prior), str(ttHhadr_prior), str(ttHlept_prior)])
    
        costval = 0.0

        for line in output.split('\n'):
            if "cost = " in line:
                costval = -float(line.replace("cost = ", ""))
                break
     
        return costval

    eps = 1e-3

    bo = BayesianOptimization(punzi_target, {'ggH_prior': (1.0, 4.0), 'WHhadr_prior': (eps, 1.0), 
                                   'ZHhadr_prior': (eps, 1.0), 'WHlept_prior': (eps, 1.0),
                                   'ZHlept_prior': (eps, 1.0), 'ZHMET_prior': (eps, 1.0),
                                   'ttHhadr_prior': (eps, 1.0), 'ttHlept_prior': (eps, 1.0)})

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
