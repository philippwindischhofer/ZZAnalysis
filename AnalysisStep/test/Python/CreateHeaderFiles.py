from trainlib.SimpleModelFactoryDynamic import SimpleModelFactoryDynamic
import os
import datetime

def main():
    # obtain a list of all model names:
    mcolls = SimpleModelFactoryDynamic.GenerateSimpleModelCollections("", input_config_file = None, hyperparam_config_file = None)

    model_names = []

    # iterate over all models that are contained in the list of model collections
    for mcoll in mcolls:
        model_names.append(mcoll.name)

    # generate the four header files that will be included into Tree.h
    header_path = os.path.join(os.environ["CMSSW_BASE"], "src/ZZAnalysis/AnalysisStep/test/classlib/include/")
    
    header_1_path = os.path.join(header_path, "MLBranches_1.h")
    header_2_path = os.path.join(header_path, "MLBranches_2.h")
    header_3_path = os.path.join(header_path, "MLBranches_3.h")
    header_4_path = os.path.join(header_path, "MLBranches_4.h")

    datetime_string = datetime.datetime.now().strftime("%Y-%m-%d %H:%M")
    start_line = "/*   This file was auto generated on " + datetime_string + ". Do NOT edit manually!    */\n\n\n"
    
    # the first header file contains the declarations of the numerical values
    print "generating " + header_1_path
    with open(header_1_path, "w") as header:
        header.write(start_line)

        for model_name in model_names:
            header.write("float " + model_name + ";\n")

        header.write("\n\nstd::map<TString, float*> ML_discriminants = {\n")

        for model_name in model_names[:-1]:
            header.write("\t{\"" + model_name + "\", &" + model_name + "},\n")
        
        header.write("\t{\"" + model_names[-1] + "\", &" + model_names[-1] + "}};\n")

    # the second header file contains the declarations of the branches themselves
    print "generating " + header_2_path
    with open(header_2_path, "w") as header:
        header.write(start_line)

        for model_name in model_names:
            header.write("TBranch* b_" + model_name + ";\n")

    # the third header file initializes the values
    print "generating " + header_3_path
    with open(header_3_path, "w") as header:
        header.write(start_line)

        for model_name in model_names:
            header.write(model_name + " = 0;\n")
    
    # and the third header file links them to the branches that were declared before
    print "generating " + header_4_path
    with open(header_4_path, "w") as header:
        header.write(start_line)

        for model_name in model_names:
            header.write("fChain->SetBranchAddress(\"" + model_name + "\", &" + model_name + ", &b_" + model_name + ");\n")

if __name__ == "__main__":
    main()
