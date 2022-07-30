#save output figures
import numpy as np
from modules.postAnalysis import printNetVoltages,printLV2Voltages
import os
import pandas as pd



#don't forget to uncomment matplotlib.use('Agg') and matplotlib in postAnalysis if printing a large dataset

endIDX = 2*16#20 per page so this is 16 pages
#print all voltages to a pdf

#RejectionResults = np.loadtxt(os.path.join("output","LV2","LV2RejectionResults.txt"))
#Voltages = np.array(np.load(os.path.join("output","LV2","VSIZ.pkl.npy"))).T
#printLV2Voltages(Voltages,RejectionResults,"LV2 Voltages - Control")

#RejectionResults = np.loadtxt(os.path.join("output","LV2","LV2RejectionResults.txt"))
#Voltages = np.array(np.load(os.path.join("output","LV2","VSIZTEA.pkl.npy"))).T
#printLV2Voltages(Voltages,RejectionResults,"LV2 Voltages - TEA")

archivedPath = os.path.join("..","CGresults","fixed_Gsyn","MedwithLV2")
#archivedPath = os.path.join("output")
RejectionResults = np.loadtxt(os.path.join(archivedPath,"LV3","LV3RejectionResults.txt"))
Voltages = np.array(pd.read_pickle(os.path.join(archivedPath,"LV3","VSIZControl.pkl"))).T
printNetVoltages(Voltages,RejectionResults,"LV3 Voltages - Control")

RejectionResults = np.loadtxt(os.path.join(archivedPath,"LV3","LV3RejectionResults.txt"))
Voltages = np.array(pd.read_pickle(os.path.join(archivedPath,"LV3","VSIZTEA.pkl"))).T
printNetVoltages(Voltages,RejectionResults,"LV3 Voltages - TEA")

