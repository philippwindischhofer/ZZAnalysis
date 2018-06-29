import trainlib.cuts as cuts
from trainlib.FileCollection import FileCollection
from trainlib.config import Config

import copy
import pandas as pd
import numpy as np
import xgboost as xgb
from sklearn.metrics import mean_squared_error

class BDTscorer:

    def __init__(self, MC_path, list_input_length = 2, xgboost_params = 
                 {'eta': 0.01, 'silent': 1, 'gamma': 0.5, 'objective': 'binary:logistic', 'nthread': 20, 'eval_metric': 'rmse'}
):
        self.MC_path = MC_path

        self.collections = {"VBF2j": {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.j2cut},
                    "VBF1j": {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.j1cut},
                    "VBF0j": {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.j0cut},
                    "VBF": {MC_path + "VBFH125/ZZ4lAnalysis.root": cuts.no_cut},
                    "ggH2j": {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.j2cut},
                    "ggH1j": {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.j1cut},
                    "ggH0j": {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.j0cut},
                    "ggH" : {MC_path + "ggH125/ZZ4lAnalysis.root": cuts.no_cut},
                    "WHh": {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut, 
                            MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHhadr_cut},
                    "WHl": {MC_path + "WplusH125/ZZ4lAnalysis.root": cuts.WHlept_cut, 
                            MC_path + "WminusH125/ZZ4lAnalysis.root": cuts.WHlept_cut},
                    "ZHh": {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHhadr_cut},
                    "ZHl": {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHlept_cut},
                    "ttHh": {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHhadr_cut},
                    "ttHl": {MC_path + "ttH125/ZZ4lAnalysis.root": cuts.ttHlept_cut},
                    "ZHMET": {MC_path + "ZH125/ZZ4lAnalysis.root": cuts.ZHMET_cut},
                    "ZX": {MC_path + "AllData/ZZ4lAnalysis.root": cuts.mZZ_cut},
                    "qq": {MC_path + "ZZTo4l/ZZ4lAnalysis.root": cuts.mZZ_cut}
                  }
        
        self.list_input_length = list_input_length
        self.xgboost_params = xgboost_params

    def extract_order(self, df, col_basename, sorted_column, columns, order):
        def get_index(row, order, col_basename, sorted_column):
            sorted_column = row[col_basename + sorted_column]
            if order >= len(sorted_column):
                return -1
            else:
                return np.flipud(np.argsort(sorted_column))[order]

        index_column = pd.DataFrame(df.transform(lambda row: get_index(row, order, col_basename, sorted_column), axis = 1, raw = True))
        index_column.columns = ["index"]
        df_temp = pd.concat([index_column, df], axis = 1)

        def get_element(row, column_name):
            if row["index"] == -1:
                return 0
            else:
                return row[column_name][row["index"]]

        extracted_cols = pd.DataFrame()
        for column in columns:
            extracted_col = pd.DataFrame(df_temp.transform(lambda row: get_element(row, col_basename + column), axis = 1, raw = True))
            
            # modify the notation for extracted list columns from using [] to (), due to compatibility issues with xgBoost
            extracted_col.columns = [col_basename + column + "(" + col_basename + "Pt|" + str(order) + ")"]
            extracted_cols = pd.concat([extracted_cols, extracted_col], axis = 1)

        return extracted_cols

    def prepare_data(self, df, col_basenames, sorted_column, columns, orders, pt_limits):
        all_extracted = pd.DataFrame()
        for col_basename, pt_limit in zip(col_basenames, pt_limits):
            for order in orders:
                extracted = self.extract_order(df, col_basename, sorted_column, columns, order)
                
                # zero all columns that are incompatible with the imposed pt limit
                mask = extracted[col_basename + "Pt(" + col_basename + "Pt|" + str(order) + ")"] < pt_limit
                extracted[mask] = 0.0

                all_extracted = pd.concat([all_extracted, extracted], axis = 1)

        return all_extracted

    def get_data(self, H1_coll, H0_coll, read_branches, input_branches, list_branches, pt_limits):
        H1_df = H1_coll.get_data(read_branches, 0.0, 1.0)
        H0_df = H0_coll.get_data(read_branches, 0.0, 1.0)

        H1_list_df = self.prepare_data(H1_df, list_branches, "Pt", ["Pt", "Eta", "Phi"], range(self.list_input_length), pt_limits)
        H0_list_df = self.prepare_data(H0_df, list_branches, "Pt", ["Pt", "Eta", "Phi"], range(self.list_input_length), pt_limits)

        list_branches_unrolled = H1_list_df.columns

        H1_df = pd.concat([H1_df, H1_list_df], axis = 1)
        H0_df = pd.concat([H0_df, H0_list_df], axis = 1)        

        complete_input_branches = np.concatenate([input_branches, list_branches_unrolled])

        H1_df = H1_df[complete_input_branches]
        H0_df = H0_df[complete_input_branches]

        return H1_df, H0_df

    def get_data_dmatrix(self, H1_coll, H0_coll, read_branches, input_branches, list_branches, pt_limits):
        H1_df, H0_df = self.get_data(H1_coll, H0_coll, read_branches, input_branches, list_branches, pt_limits)

        complete_input_branches = H1_df.columns
        print "number of input variables: " + str(len(complete_input_branches))
        print "final list of inputs: " + str(complete_input_branches)

        # try with the same weights as used later in the neural network training, to balance out some (very)
        # unbalanced datasets
        H1_class_weight = 1.0 + float(len(H0_df)) / float(len(H1_df))
        H0_class_weight = 1.0 + float(len(H1_df)) / float(len(H0_df))

        print "using class weights: " + str(H1_class_weight) + " (H1), " + str(H0_class_weight) + " (H0)"

        H1_weights = np.full(len(H1_df), H1_class_weight)
        H0_weights = np.full(len(H0_df), H0_class_weight)

        H1_data = H1_df.as_matrix()
        H0_data = H0_df.as_matrix()
        H1_target = np.ones(np.shape(H1_data)[0])
        H0_target = np.zeros(np.shape(H0_data)[0])

        target = np.concatenate([H1_target, H0_target])
        data = np.concatenate([H1_data, H0_data])
        weights = np.concatenate([H1_weights, H0_weights])

        dmatrix = xgb.DMatrix(data, label = target, feature_names = complete_input_branches, weight = weights)

        return dmatrix

    def get_feature_importance_list_BDT(self, disc_pair, scalar_branches, list_branches, pt_limits):
        H1_name = disc_pair[0]
        H0_name = disc_pair[1]

        # get the training data for the BDT ...
        H1_coll_train = FileCollection(self.collections[H1_name], 0.0, 0.5)
        H0_coll_train = FileCollection(self.collections[H0_name], 0.0, 0.5)
        dtrain = self.get_data_dmatrix(H1_coll_train, H0_coll_train, Config.branches, scalar_branches, list_branches, pt_limits)

        # ... and the validation data as well
        H1_coll_val = FileCollection(self.collections[H1_name], 0.5, 1.0)
        H0_coll_val = FileCollection(self.collections[H0_name], 0.5, 1.0)
        dval = self.get_data_dmatrix(H1_coll_val, H0_coll_val, Config.branches, scalar_branches, list_branches, pt_limits)

        evallist = [(dtrain, 'train'), (dval, 'eval')]

        # perform the training
        # try different tree depths and choose the one that gives the best RMSE (i.e. avoid too deep trees to start with)
        max_num_rounds = 2000

        best_loss = 1e6
        best_imp = None
        best_params = None
        
        # perform a crude hyperparameter optimization by training trees of different depth and trusting the one that gives
        # the best loss on the *validation* dataset
        for tree_depth in range(1,8):
            self.xgboost_params['max_depth'] = tree_depth

            bst = xgb.train(self.xgboost_params, dtrain, max_num_rounds, evals = evallist, early_stopping_rounds = 10, verbose_eval = False)

            pred = bst.predict(dval)
            cur_loss = np.sqrt(mean_squared_error(pred, dval.get_label()))
            cur_imp = bst.get_fscore()

            print "for max_depth = " + str(self.xgboost_params['max_depth']) + ": loss = " + str(cur_loss)

            if cur_loss < best_loss:
                best_loss = cur_loss
                best_imp = copy.copy(cur_imp)
                best_params = copy.copy(self.xgboost_params)

        # normalize the usage score w.r.t. the total score (i.e. sum of all individuals)
        score_sum = sum([val for key, val in best_imp.iteritems()])
        used_variables = {key: val / float(score_sum) for key, val in sorted(best_imp.iteritems(), key = lambda x: x[1], reverse = True)}           
        return best_params, dtrain.feature_names, used_variables

    def get_sorted_feature_importance_list(self, disc, scalar_branches, list_branches, pt_limits):

        _, _, implist = self.get_feature_importance_list_BDT(disc, scalar_branches, list_branches, pt_limits)

        # sort the used variables according to their importance
        sorted_implist = []
        for key, val in sorted(implist.iteritems(), key = lambda x: x[1], reverse = True):
            sorted_implist.append((key, val))

        print "number of selected input variables = " + str(len(sorted_implist))
            
        return sorted_implist
