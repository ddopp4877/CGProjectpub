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
"""
rejectionResults = np.loadtxt(os.path.join("output","LV2","LV2RejectionResults.txt"))
#rejectionResultsRaw = np.loadtxt(os.path.join("output","LV2","LV2RejectionRaw.txt"))
critList = ["Areas", "Peaks", "SPB", "AreasTEA", "PeaksTEA", "SPBTEA"]

plot1 = plotFailCrit(rejectionResults,critList)#rejection results need to be an array with coded values in the columns and rejection criteria in the rows


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

fullParams = fullParamsList()
#LV3passParams = np.array(pd.read_pickle(os.path.join("output","LV3",passParamsFilename  + ".pkl")))
Params = np.array(pd.read_pickle(os.path.join("input","LV2","passParamsRepeat" + ".pkl"))).T

plotDistributions(Params,list(LV1ParamsDict().keys()))
print(Params.shape)

lv3ParamsList = fullParams
LV3passParams = np.array(pd.read_pickle(os.path.join("output","LV3",passParamsFilename  + ".pkl")))
plotCorrelogram(LV3passParams,lv3ParamsList)
"""
fullParams = fullParamsList()
critList = ["Areas", "Peaks", "SPB", "AreasTEA", "PeaksTEA", "SPBTEA", "Synchrony","SynchronyTEA"]
#lv3ParamsList = fullParams +critList

#LV3passParams = np.array(pd.read_pickle(os.path.join("output","LV3","LV3passParamsandCrit.pkl"))).T
#plotCorrelogram(LV3passParams,lv3ParamsList)

#print(LV3passParams.shape)
#plotDistributions(LV3passParams,lv3ParamsList)

LV3passParams = np.array(pd.read_pickle(os.path.join("output","LV3",passParamsFilename  + ".pkl")))
#get the average of each network:
"""
[a,b] = LV3passParams.shape

avgNets = np.ones((a,1))

for i in range(0,b,5):
    avgNets = np.hstack((avgNets,np.mean(LV3passParams[:,i:i+5],axis=1).reshape(-1,1)))
avgNets = avgNets[:,1:]

avgNets = np.repeat(avgNets,5,axis=1)
print(avgNets.shape)
np.savetxt("avgNets.txt",avgNets)

"""
#convertResultstoH5(folderList(),subfolderList())


critList = ["Areas", "Peaks", "SPB", "AreasTEA", "PeaksTEA", "SPBTEA", "Synchrony","SynchronyTEA"]
lv3ParamsList = fullParamsList() + critList
LV3passCrit = np.loadtxt(os.path.join("output","LV3","LV3RejectionResults"  + ".txt"))

[a,b] = LV3passCrit.shape

netPass = np.array([1 if(np.all(LV3passCrit[a-1,i:i+5]==1)) else 0 for i in range(0,b,5)])# mark 1 if all cells in a net passed
uniqueNetPass = [1 if (np.any(netPass[i:i+16] == 1)) else 0 for i in range(0,len(netPass),16)]# mark 1 if any networks in a set of 16 passed (because it's actually the same net)

uNetPassIdxs = np.repeat(netPass,5)


params = np.array(pd.read_pickle(os.path.join("output","LV3","passParamsRepeat"  + ".pkl")))

Trials = int((params.shape)[1]/16)
SCfreqs = []
bufferSize = 50
np.random.seed(int(222))
for j in range(0,Trials):
    for i in range(0,16):
        SCfreqs.append(np.random.uniform(16+i,17+i))
SCfreqs = np.array(SCfreqs)

passingParams = params[:,(np.where(np.array(uNetPassIdxs)==1))[0]]
passingFreqs = SCfreqs[(np.where(np.array(uNetPassIdxs)==1))[0]]
passingParams = np.vstack((passingParams,passingFreqs))
paramList = fullParamsList() + ['SCfreq']
print(paramList)

LV3passRaw = np.loadtxt(os.path.join("output","LV3","LV3RejectionRaw"  + ".txt"))
totalParams = np.vstack((passingParams,LV3passRaw[:,(np.where(np.array(uNetPassIdxs)==1))[0]]))

#passingParams = np.unique(passingParams,axis=1)


plotCorrelogram(passingParams,paramList,' passing network parameter sets with repetition')




#the rejection criteria can only be used with the repeated parameter set, since the unique passing sets are just sets that pass at any point
#ppnoRepeat = np.array(pd.read_pickle(os.path.join("output","LV3","passParams"  + ".pkl")))
#plottitle = 'passing Networks unique parameters'
#plotCorrelogram(ppnoRepeat,fullParamsList(),plottitle)


plt.show()