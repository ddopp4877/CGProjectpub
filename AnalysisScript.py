#from CGProject import VoltageFilename
from modules.postAnalysis import *
import numpy as np
import os
import pandas as pd
from modules.RejectionProtocols import *
import time
from modules.makeParams import *
from scipy.stats import kurtosis

from saveOutFigs import RejectionResults


#merge parameters, scfreqs,and rejection results raw
params = np.array(pd.read_pickle(os.path.join("output","LV3","passParamsRepeat.pkl")))
scfreqs = np.loadtxt(os.path.join("output","LV3","SCfreqs"))
RejectionResults = np.loadtxt(os.path.join("output","LV3","LV3RejectionRaw.txt"))

args = [params,scfreqs.reshape(1,-1),RejectionResults]
Params = np.concatenate(args,axis=0)

myList = fullParamsList() + ['SCfreqs'] + LV3CritList()


plotCorrelogram(Params,myList,'not avg')




plt.show()