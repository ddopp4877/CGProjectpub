from modules.postAnalysis import *
import numpy as np
import os
import pandas as pd
from modules.RejectionProtocols import *
import time
from modules.makeParams import *



#get the averaged networks which pass
LV3passParamsAVG = np.array(pd.read_pickle(os.path.join("output","LV3","passParamsRepeat.pkl")))
codedAVG = np.loadtxt(os.path.join("output","LV3","LV3RejectionResults.txt"))
passIdxsAVG,failIdxsAVG,allIdxsAVG = getPassIdxs(codedAVG)
passParamsAVG = getEveryFirstNet(LV3passParamsAVG[:,passIdxsAVG])


#get the nonaveraged networks which pass
LV3passParams = np.array(pd.read_pickle(r'C:\Users\ddopp\source\repos\CGresults\notAVG\output\LV3\passParamsRepeat.pkl'))#the set used to make the average params
coded = np.loadtxt(r'C:\Users\ddopp\source\repos\CGresults\notAVG\output\LV3\LV3RejectionResults.txt')
passIdxs,failIdxs,allIdxs = getPassIdxs(coded)
passParams = getEveryFirstNet(LV3passParams[:,passIdxs])

#get the averaged networks which fail
failParamsAVG = getEveryFirstNet(LV3passParamsAVG[:,failIdxsAVG])
#get the nonaveraged networks which fail
failParams = getEveryFirstNet(LV3passParams[:,failIdxs])
#compare the ranges of the failing networks with the nonfailing networks

allIdxsAVG#this is the 1 or 0 for every network at every frequency, 22240 recordings that form 278 networks, 1 if passed, 0 if failed
allAVGnetspassIdxs = getEveryFirstNet(allIdxsAVG)#1390 forming 278 networks
p = np.where(allAVGnetspassIdxs==1)[0]#1285 forming 257 networks
f =  np.where(allAVGnetspassIdxs!=1)[0]#105 forming 21 networks
t1 = passParams[:,p]# un averaged networks corresponding to the averaged networks that pass#because passParams are the params input into the averaged trials
t2 = passParams[:,f] # un averaged networks corresponding to the averaged networks that fail

plotCorrelogram(failParamsAVG,fullParamsList(),' averaged networks which fail')
plt.show()