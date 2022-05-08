#from CGProject import VoltageFilename
from modules.postAnalysis import *
import numpy as np
import os
import pandas as pd
from modules.RejectionProtocols import *
import time
from modules.makeParams import *
from scipy.stats import kurtosis
"""
VoltageFilenameTEA = "VsomaTEA"
ParamsFilenameTEA = "passParamsRepeatTEA"
passParamsFilename = "passParams"
VoltageFilename = "Vsoma"

### generate a report of the summary statistics of a particular set of parameters like mean,std, for the parameters which passed the given crit, failed high or failed low
### change the threshold value to determine which correlations to look at (default is all corrs above 0.2)

rejectionResultsRaw = np.loadtxt(os.path.join("output","LV3","LV3RejectionRaw.txt"))
rejectionResults = np.loadtxt(os.path.join("output","LV3","LV3RejectionResults.txt"))
params = np.array(pd.read_pickle(os.path.join("output","LV3","passParamsRepeat"  + ".pkl")))

[a,b] = rejectionResults.shape
passIDXs = np.where(rejectionResults[a-1,:]==1)[0]
failLowIDXs = np.where(rejectionResults[a-1,:]<1)[0]
failHighIDXs = np.where(rejectionResults[a-1,:] > 1)[0]
#concat params with rejection results data, and make a df with the columns as variables
paramsDF = pd.DataFrame(np.vstack((params,rejectionResultsRaw)).T,columns = fullParamsList() + LV3CritList())
# get only the passing parameters into a dataframe keeping columns as variables
passParamsDF = pd.DataFrame([paramsDF[key][passIDXs] for key in paramsDF]).T
flParamsDF = pd.DataFrame([paramsDF[key][failLowIDXs] for key in paramsDF]).T

maskedDFCorr = getMaskedCorr(passParamsDF,0.2)#passParams,Pearson's R threshold to consider above

pairList = getpassPairsList(maskedDFCorr)

failCriteriacoded = pd.DataFrame(rejectionResults).T
failCriteriacoded.columns = LV3CritList() + ['passorfail']

lowDict = makestatsDict(paramsDF,failCriteriacoded,pairList,'low')
passDict = makestatsDict(paramsDF,failCriteriacoded,pairList,'passing')
highDict = makestatsDict(paramsDF,failCriteriacoded,pairList,'high')
passDict.columns = ['parameter','rCriteria','AVG_passing','STD_passing','fail low p-value','fail high p-value']
lowDict.columns = ['parameter','rCriteria','AVG_Low','STD_Low']
highDict.columns = ['parameter','rCriteria','AVG_High','STD_High']
allDF = pd.merge(passDict,lowDict)
allDF = pd.merge(allDF,highDict)
allDF = allDF[['parameter','rCriteria','AVG_passing','STD_passing','AVG_Low','STD_Low','AVG_High','STD_High','fail low p-value','fail high p-value']]

allDF.to_csv('test.csv')
"""
#get the averaged networks which pass
LV3passParamsAVG = np.array(pd.read_pickle(r"C:\Users\ddopp\source\repos\CGresults\AVG\output\LV3\passParamsRepeat.pkl"))
codedAVG = np.loadtxt(r"C:\Users\ddopp\source\repos\CGresults\AVG\output\LV3\LV3RejectionResults.txt")
passIdxsAVG,failIdxsAVG,allIdxsAVG = getPassIdxs(codedAVG)
passParamsAVG = getEveryFirstNet(LV3passParamsAVG[:,passIdxsAVG])


#get the nonaveraged networks which pass
LV3passParamsOG = np.array(pd.read_pickle(r'C:\Users\ddopp\source\repos\CGresults\notAVG\output\LV3\passParamsRepeat.pkl'))#the set used to make the average params
coded = np.loadtxt(r'C:\Users\ddopp\source\repos\CGresults\notAVG\output\LV3\LV3RejectionResults.txt')
passIdxs,failIdxs,allIdxs = getPassIdxs(coded)
passParams = getEveryFirstNet(LV3passParamsOG[:,passIdxs])

#get the averaged networks which fail
failParamsAVG = getEveryFirstNet(LV3passParamsAVG[:,failIdxsAVG])
#get the nonaveraged networks which fail
failParams = getEveryFirstNet(LV3passParamsOG[:,failIdxs])
#compare the ranges of the failing networks with the nonfailing networks

allIdxsAVG#this is the 1 or 0 for every network at every frequency, 22240 recordings that form 278 networks, 1 if passed, 0 if failed
allAVGnetspassIdxs = getEveryFirstNet(allIdxsAVG)#1390 forming 278 networks
p = np.where(allAVGnetspassIdxs==1)[0]#1285 forming 257 networks
f =  np.where(allAVGnetspassIdxs!=1)[0]#105 forming 21 networks
t1 = passParams[:,p]# un averaged networks corresponding to the averaged networks that pass
t2 = passParams[:,f] # un averaged networks corresponding to the averaged networks that fail

t2Kurts = [kurtosis(t2[i,:],bias=False) for i in range((t2.shape)[0])]
t2Kurts = np.round(t2Kurts,2)
plotDistributions(t2,fullParamsList(),20,'Params corresponding to failing AVGs  ',t2Kurts)

t1Kurts = [kurtosis(t1[i,:],bias=False) for i in range((t1.shape)[0])]
t1Kurts = np.round(t1Kurts,2)
plotDistributions(t1,fullParamsList(),20,' Params corresponding to passing AVGs ',t1Kurts)

plt.show()