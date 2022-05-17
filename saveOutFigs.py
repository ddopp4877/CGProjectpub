#save output figures
import numpy as np
from modules.postAnalysis import printNetVoltages,printLV2Voltages
import os
import pandas as pd

endIDX = 5*20#20 per page so this is 16 pages
#print all voltages to a pdf

"""
RejectionResults = np.loadtxt(os.path.join("output","LV2","LV2RejectionResults.txt"))
Voltages = np.array(np.load(os.path.join("output","LV2","VsomaControl.pkl.npy"),allow_pickle=True)).T
printLV2Voltages(Voltages[:,:endIDX],RejectionResults[:,:endIDX],"LV2 Voltages - Control")

RejectionResults = np.loadtxt(os.path.join("output","LV2","LV2RejectionResults.txt"))
Voltages = np.array(np.load(os.path.join("output","LV2","VsomaTEA.pkl.npy"),allow_pickle=True)).T
printLV2Voltages(Voltages[:,:endIDX],RejectionResults[:,:endIDX],"LV2 Voltages - TEA")

"""
#RejectionResults = np.loadtxt(r'C:\Users\ddopp\source\repos\CGresults\notAVG\output\LV3\LV3RejectionResults.txt')
#Voltages = np.array(pd.read_pickle(r'C:\Users\ddopp\source\repos\CGresults\notAVG\output\LV3\VsomaControl.pkl')).T
#printNetVoltages(Voltages[:,:endIDX],RejectionResults[:,:endIDX],"Network Voltages - Control")
#np.array(pd.read_pickle(os.path.join("output","LV3", "passParamsRepeat"+ ".pkl")))
Voltages = np.array(pd.read_pickle(os.path.join("output","LV3","AVG","VsomaControl.pkl"))).T
RejectionResults = np.loadtxt(os.path.join("output","LV3","AVG","LV3RejectionResults.txt"))
printNetVoltages(Voltages,RejectionResults,"Network Voltages - Control - AVG")


#Voltages = np.array(pd.read_pickle(r'C:\Users\ddopp\source\repos\CGresults\notAVG\output\LV3\VsomaTEA.pkl')).T
#printNetVoltages(Voltages[:,:endIDX],RejectionResults[:,:endIDX],"Network Voltages - TEA")
Voltages = np.array(pd.read_pickle(os.path.join("output","LV3","AVG","VsomaTEA.pkl"))).T

printNetVoltages(Voltages,RejectionResults,"Network Voltages - TEA - AVG")

