
import h5py
import shutil
import os
import numpy as np
import pandas as pd
import pickle
import matplotlib.pyplot as plt
from scipy import stats
from itertools import combinations
from scipy.stats import kurtosis
from scipy.spatial import distance

from modules.makeParams import *
from modules.RejectionProtocols import *
from modules.postAnalysis import *

archivedPath = os.path.join("..","CGResults","fixed_gSyn")
file = "MedwithLV2"

LV3Params = np.array(pd.read_pickle(os.path.join(archivedPath,file,"output","LV3","passParamsRepeat.pkl")))
coded = np.loadtxt(os.path.join(archivedPath,file,"output","LV3","LV3RejectionResults.txt"))
passIdxs,failIdxs,allIdxs = getPassIdxs(coded,"LVL3")
passParams = LV3Params[:,passIdxs]

h.nrn_load_dll(os.path.join("modFiles","nrnmech.dll"))
h.load_file('stdrun.hoc') #so you can use run command
eventTimes = np.array(pd.read_pickle(os.path.join(archivedPath,file,"input","LV3", "EventTimes.pkl"))).T
passingEventTimes = eventTimes[:,passIdxs]

params = passParams[:,0]
ETs = passingEventTimes[:,0]

cell = LargeCellLV2(0)

varNames = list(rangeVarNames().keys())[:len(params)]

#assign the param(:,i) to LC2(i)
for j in range(len(varNames)):
    exec("%s = %f" %("cell."+varNames[j],params[j]))

dt = 0.2
tstop = 2550#ms
maxstep = 10
vinit = -51#mV
Trials = 1
seed  = 32165156


synGain = 0.09

vsAll = h.VecStim()
ETs = h.Vector(ETs[ETs !=0])

syns = h.Exp2Syn(cell.siz(1))

vsAll.play(ETs)
syns.tau1,syns.tau2,syns.e   = 10,120,-15

h.NetCon(vsAll,syns,-10,0,synGain)

v = h.Vector().record(cell.soma(0.5)._ref_v)
vNeurite = h.Vector().record(cell.neurite(0.5)._ref_v)
vSIZ = h.Vector().record(cell.siz(0.5)._ref_v)

h.dt=0.2
h.finitialize(-51)
h.continuerun(2550)

