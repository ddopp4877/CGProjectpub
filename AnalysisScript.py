#from CGProject import VoltageFilename
from modules.postAnalysis import *
import numpy as np
import os
import pandas as pd
import time
from scipy.stats import kurtosis

print('test') 
RejectionResults = np.loadtxt(os.path.join("output","LV3","LV3RejectionResults.txt"))
critList = LV3CritList()
plotFailCrit(RejectionResults,critList)

plt.show()