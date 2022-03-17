from modules.postAnalysis import *
import numpy as np
import os
import pandas as pd
from modules.RejectionProtocols import *
import time
from modules.makeParams import *

VoltageFilenameTEA = "VsomaTEA"
ParamsFilenameTEA = "passParamsRepeatTEA"
passParamsFilename = "passParams"

#rejectionResults = np.loadtxt(os.path.join("output","LV2","LV2RejectionResults.txt"))
#rejectionResultsRaw = np.loadtxt(os.path.join("output","LV2","LV2RejectionRaw.txt"))
#critList = ["Areas", "Peaks", "SPB", "AreasTEA", "PeaksTEA", "SPBTEA"]

#plot1 = plotFailCrit(rejectionResults,critList)#rejection results need to be an array with coded values in the columns and rejection criteria in the rows

"""
rejectionResults = np.loadtxt(os.path.join("output","LV3","LV3RejectionResults.txt"))
critList = ["Areas", "Peaks", "SPB", "AreasTEA", "PeaksTEA", "SPBTEA", "Synchrony","SynchronyTEA"]



###########LV2 plots #########
Vsoma = np.array(pd.read_pickle(os.path.join("output","LV2","VsomaControl" + ".pkl"))).T
plotNet(Vsoma,0,25)#network , scFreq



VsomaTEA = np.array(pd.read_pickle(os.path.join("output","LV2",VoltageFilenameTEA + ".pkl"))).T
Time = np.loadtxt(os.path.join("output","LV2","time.txt"))
#plot4 = plotNet(VsomaTEA,0,16)

Vsoma = np.array(pd.read_pickle(os.path.join("output","LV3","VsomaControl" + ".pkl"))).T
#plot5 = plotNet(Vsoma,0,16)


Vsoma = np.array(pd.read_pickle(os.path.join("output","LV3","VsomaTEA" + ".pkl"))).T
plot6= plotNet(Vsoma,0,25)


lv3ParamsList = list(LV1ParamsDict().keys()) + list(LV2ParamsDict().keys())
LV3passParams = np.array(pd.read_pickle(os.path.join("output","LV3",passParamsFilename  + ".pkl")))
plotCorrelogram(LV3passParams,lv3ParamsList)


plt.show()

IDX = getLV2CellIDX(67,24)

print(critList)
print(rejectionResults[:,IDX])
print(rejectionResultsRaw[:, IDX])


VsomaTEA = np.array(pd.read_pickle(os.path.join("output","LV2", "Vsoma" + "TEA" + ".pkl"))).T

timeArray = getSimTime(Vsoma,0.2)
coded, Raw, Idxs,critList = LV2RejectionProtocol(timeArray, Vsoma[:,IDX:IDX+1],VsomaTEA[:,IDX:IDX+1] )


rejectionResults = np.loadtxt(os.path.join("output","LV3","LV3RejectionResults.txt"))
critList = ["Areas", "Peaks", "SPB", "AreasTEA", "PeaksTEA", "SPBTEA", "Synchrony","SynchronyTEA"]
plotFailCrit(rejectionResults,critList)
plt.show()

avg = np.mean(Vsoma[np.where(t<=100),IDX])
peak = max(v[:,IDX]-avg)
peaks, _ = find_peaks(Vsoma[:,IDX],distance =90,  height = -40)
print(peaks)
print(_)
Vsoma = np.array(pd.read_pickle(os.path.join("output","LV2","VsomaTEA" + ".pkl"))).T
peaks, _ = find_peaks(Vsoma[:,IDX],distance =90,  height = -40)
print(peaks)
print(_)
"""
fullParams = fullParamsList()
LV3passParams = np.array(pd.read_pickle(os.path.join("output","LV3",passParamsFilename  + ".pkl")))
Params = np.array(pd.read_pickle(os.path.join("output","LV2","passParams" + ".pkl"))).T

plotDistributions(Params,fullParams)


plotCorrelogram(LV3passParams,fullParams)
plt.show()