#save output figures
import numpy as np
from modules.postAnalysis import printNetVoltages,printLV2Voltages
import os
import pandas as pd

endIDX = 16*20
#print all voltages to a pdf

"""
RejectionResults = np.loadtxt(os.path.join("output","LV2","LV2RejectionResults.txt"))
Voltages = np.array(np.load(os.path.join("output","LV2","VsomaControl.pkl.npy"),allow_pickle=True)).T
printLV2Voltages(Voltages[:,:endIDX],RejectionResults[:,:endIDX],"LV2 Voltages - Control")

RejectionResults = np.loadtxt(os.path.join("output","LV2","LV2RejectionResults.txt"))
Voltages = np.array(np.load(os.path.join("output","LV2","VsomaTEA.pkl.npy"),allow_pickle=True)).T
printLV2Voltages(Voltages[:,:endIDX],RejectionResults[:,:endIDX],"LV2 Voltages - TEA")

"""
RejectionResults = np.loadtxt(os.path.join("output","LV3","LV3RejectionResults.txt"))
Voltages = np.array(pd.read_pickle(os.path.join("output","LV3","VsomaControl.pkl"))).T
#printNetVoltages(Voltages[:,:endIDX],RejectionResults[:,:endIDX],"Network Voltages - Control")
printNetVoltages(Voltages,RejectionResults,"Network Voltages - Control")


Voltages = np.array(pd.read_pickle(os.path.join("output","LV3","VsomaTEA.pkl"))).T
#printNetVoltages(Voltages[:,:endIDX],RejectionResults[:,:endIDX],"Network Voltages - TEA")
printNetVoltages(Voltages,RejectionResults,"Network Voltages - TEA")

