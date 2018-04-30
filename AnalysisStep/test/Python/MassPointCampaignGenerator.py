import numpy as np
import os
import re
import copy
import sys

from trainlib.ConfigFileHandler import ConfigFileHandler

def main():
    if len(sys.argv) != 4:
        print "Error: exactly 3 arguments are required"

    config_file_in = sys.argv[1]
    config_file_out = sys.argv[2]
    masspoint = float(sys.argv[3])

    confhandler = ConfigFileHandler()
    confhandler.load_configuration(config_file_in)
    confhandler.set_field('global', 'mass_point', str(masspoint))
    confhandler.save_configuration(config_file_out)

if __name__ == "__main__":
    main()
