from CGProject import VoltageFilename
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