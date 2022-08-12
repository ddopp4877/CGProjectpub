#from CGProject import VoltageFilename
from modules.postAnalysis import *
import numpy as np
import os
import pandas as pd
import time
from scipy.stats import kurtosis

os.chdir("..")
os.chdir('CGResults')
os.chdir("fixed_Gsyn")
os.chdir("MedwithLV2")
convertResultstoH5()
plt.show()