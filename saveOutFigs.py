#save output figures
import numpy as np
from modules.postAnalysis import printNetVoltages,printLV2Voltages
import os
import pandas as pd
import h5py

endIDX = 100*16#20 per page so this is 16 pages

archivedPath = os.path.join("..","CGresults","fixed_Gsyn","MedwithLV2")

"""
def readDat(f,endIDX):
    
    firstdatasetName = list(f.keys())[0]
    number_of_rows = f[firstdatasetName].shape[0]

    myiter = f['default'].__iter__()# __iter__ is a method of the dataset subclass that returns an iterator. when called () this is actually a generator,which moves by next()

    savedDat = []
    #now read in only the data between rows 30 and 40
    for i in range(0,number_of_rows):
        newDat = next(myiter)
        if  i < endIDX:
            savedDat.append(newDat)
        else:
            pass
    f.close()
    return np.array(savedDat)

archivedPath = os.path.join("..","CGresults","fixed_Gsyn","MedwithLV2")

RejectionResults = np.loadtxt(os.path.join(archivedPath,"output","LV2","LV2RejectionResults.txt"))[:,:endIDX]
f = h5py.File(os.path.join(archivedPath,"output","LV2","VsomaControl.h5"), 'r')
Voltages = readDat(f,endIDX)
printLV2Voltages(Voltages.T,RejectionResults,"LV2 Voltages - Control")


f = h5py.File(os.path.join(archivedPath,"output","LV2","VsomaTEA.h5"), 'r')
Voltages = readDat(f,endIDX)
printLV2Voltages(Voltages.T,RejectionResults,"LV2 Voltages - TEA")

#don't forget to uncomment matplotlib.use('Agg') and matplotlib in postAnalysis if printing a large dataset
"""

#print all voltages to a pdf
archivedPath = os.path.join("..","CGresults","fixed_Gsyn","MedwithLV2")
RejectionResults = np.loadtxt(os.path.join(archivedPath,"output","LV2","LV2RejectionResults.txt"))
Voltages = np.array(np.load(os.path.join(archivedPath,"output","LV2","VsomaControl.pkl.npy"))).T
printLV2Voltages(Voltages[:,:endIDX],RejectionResults[:,:endIDX],"LV2 Voltages - Control")

#RejectionResults = np.loadtxt(os.path.join(archivedPath,"LV2","LV2RejectionResults.txt"))
#Voltages = np.array(np.load(os.path.join(archivedPath,"LV2","VsomaTEA.pkl.npy"))).T
#printLV2Voltages(Voltages[:,:endIDX],RejectionResults[:,:endIDX],"LV2 Voltages - TEA")
"""
archivedPath = os.path.join("..","CGresults","fixed_Gsyn","MedwithLV2")
#archivedPath = os.path.join("output")
RejectionResults = np.loadtxt(os.path.join(archivedPath,"LV3","LV3RejectionResults.txt"))
Voltages = np.array(pd.read_pickle(os.path.join(archivedPath,"LV3","VSIZControl.pkl"))).T
printNetVoltages(Voltages,RejectionResults,"LV3 Voltages - Control")

RejectionResults = np.loadtxt(os.path.join(archivedPath,"LV3","LV3RejectionResults.txt"))
Voltages = np.array(pd.read_pickle(os.path.join(archivedPath,"LV3","VSIZTEA.pkl"))).T
printNetVoltages(Voltages,RejectionResults,"LV3 Voltages - TEA")
"""
