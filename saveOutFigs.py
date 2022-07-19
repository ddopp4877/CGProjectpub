#save output figures
import numpy as np
from modules.postAnalysis import printNetVoltages,printLV2Voltages
import os
import pandas as pd



#don't forget to uncomment matplotlib.use('Agg') and matplotlib in postAnalysis if printing a large dataset

endIDX = 2*16#20 per page so this is 16 pages
#print all voltages to a pdf

"""
RejectionResults = np.loadtxt(os.path.join("output","LV2","LV2RejectionResults.txt"))
Voltages = np.array(pd.read_pickle(os.path.join("output","LV2","VsomaControl.pkl"))).T
printLV2Voltages(Voltages[:,:endIDX],RejectionResults[:,:endIDX],"LV2 Voltages - Control")

RejectionResults = np.loadtxt(os.path.join("output","LV2","LV2RejectionResults.txt"))
Voltages = np.array(pd.read_pickle(os.path.join("output","LV2","VsomaTEA.pkl"))).T
printLV2Voltages(Voltages[:,:endIDX],RejectionResults[:,:endIDX],"LV2 Voltages - TEA")
"""
RejectionResults = np.loadtxt(os.path.join("output","LV3","LV3RejectionResults.txt"))
Voltages = np.array(pd.read_pickle(os.path.join("output","LV3","VsomaControl.pkl"))).T
printNetVoltages(Voltages,RejectionResults,"LV3 Voltages - Control")

RejectionResults = np.loadtxt(os.path.join("output","LV3","LV3RejectionResults.txt"))
Voltages = np.array(pd.read_pickle(os.path.join("output","LV3","VsomaTEA.pkl"))).T
printNetVoltages(Voltages,RejectionResults,"LV3 Voltages - TEA")