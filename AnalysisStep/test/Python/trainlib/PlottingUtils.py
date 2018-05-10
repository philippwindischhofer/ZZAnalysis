import pandas as pd
import numpy as np
import re

def extract_columns(df, colstrings):
    dfs = []
    
    for colstring in colstrings:
        dfs.append(extract_column(df, colstring))
        
    df = pd.concat(dfs, axis = 1)
        
    return df

def extract_column(df, colstring):
    if '[' in colstring and ']' in colstring:
        col, sorted_col_order, _ = re.split('[\[\]]', colstring)
        sorted_col, order = re.split('\|', sorted_col_order)
        cf = extract_order_filtered(df, sorted_col, [col], int(order))
    else:
        cf = df[[colstring]]
    return cf

def extract_order_filtered(df, sorted_column, columns, order):
    extracted_raw = extract_order(df, sorted_column, columns, order)

    if any("Jet" in col for col in extracted_raw.columns):
        # apply the jet-pt cut
        mask_column = extract_order(df, "JetPt", ["JetPt"], order)
        mask = mask_column < 30.0
        extracted_raw[mask.as_matrix()] = 0.0

    return extracted_raw

def extract_order(df, sorted_column, columns, order):
    def get_index(row, order, sorted_column):
        sorted_column = row[sorted_column]
        if order >= len(sorted_column):
            return -1
        else:
            return np.flipud(np.argsort(sorted_column))[order]

    index_column = pd.DataFrame(df.transform(lambda row: get_index(row, order, sorted_column), axis = 1, raw = True))
    index_column.columns = ["index"]
    df_temp = pd.concat([index_column, df], axis = 1)

    def get_element(row, column_name):
        if row["index"] == -1:
            return 0
        else:
            return row[column_name][row["index"]]

    extracted_cols = pd.DataFrame()
    for column in columns:
        extracted_col = pd.DataFrame(df_temp.transform(lambda row: get_element(row, column), axis = 1, raw = True))
        extracted_col.columns = [column + "_" + str(order)]
        extracted_cols = pd.concat([extracted_cols, extracted_col], axis = 1)

    return extracted_cols