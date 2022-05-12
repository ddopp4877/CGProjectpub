from neuron import h
import numpy as np
import os
from modules.makeParams import *
from modules.RejectionProtocols import *
import sys
import time
import pandas as pd
from numpy import empty
import platform
import tracemalloc
import gc

if platform.system() == 'Linux':
    h.nrn_load_dll(os.path.join("modFiles/x86_64/.libs/libnrnmech.so"))
else:
    h.nrn_load_dll(os.path.join("modFiles","nrnmech.dll"))

if len(sys.argv) < 4:
    print("seed, voltage output filename, lv1 parameters repeated output  filename,eventTimes input filename,Control or TEA")
    quit()
#hyperparameters:
dt = 0.2
tstop = 2550#ms
maxstep = 10
vinit = -51#mV
seed = int(sys.argv[1])
voutfilename = sys.argv[2]
passparamsfilename = sys.argv[3]
eventtimesfilename = sys.argv[4]
controlorTEA = sys.argv[5]

eventTimes = np.array(pd.read_pickle(os.path.join("input","LV2",eventtimesfilename + ".pkl")))
LV1PassParams =  np.array(pd.read_pickle(os.path.join("input","LV2", passparamsfilename+ ".pkl")))
Trials = (LV1PassParams.shape)[1]
UNparamsNO = (np.unique(LV1PassParams,axis=1)).shape[1]

h.load_file('stdrun.hoc') #so you can use run command

#make a list of LargeCells then change their parameters to be random

params, LCs = makeRandomCellsLV2(UNparamsNO,seed,LV1PassParams,controlorTEA)

#read in event times, turn it into a vector, remove the zeros, play the vector as the source of the vectstim, which is the source of the netcon to the exp2syn target

#synGain = 0.16
synGain = 0.19
#recording variables:

vsAll = [h.VecStim() for i in range(0,Trials)]
ETs = [h.Vector(eventTimes[i,eventTimes[i,:] !=0]) for i in range(0,Trials)]
syns = [h.Exp2Syn(LCs[i].siz(1)) for i in range(0,Trials)]



for i in range(0,Trials):
    vsAll[i].play(ETs[i])
    syns[i].tau1,syns[i].tau2,syns[i].e   = 10,120,-15

NetCons = [h.NetCon(vsAll[i],syns[i],-10,0,synGain) for i in range(0,Trials)]    

v = [h.Vector().record(LCs[i].soma(0.5)._ref_v) for i in range(0,Trials)]

h.dt=0.2
h.finitialize(-51)
h.continuerun(2550)
#h.continuerun(2000)

V = pd.DataFrame(data = v,dtype='float32')
#V = np.array(v,dtype='float32')


V.to_pickle(os.path.join("output","LV2",voutfilename + controlorTEA + ".pkl"))
#np.save(os.path.join("output","LV2",voutfilename + controlorTEA + ".pkl"),V)

gc.collect()

#only need to save params and time once, they dont change for TEA case
if controlorTEA == "Control":
    Params = pd.DataFrame(data = params,dtype = 'float32')
    Params.to_pickle(os.path.join("output","LV2",passparamsfilename + controlorTEA + ".pkl"))
    dt = 0.2
    simTime = np.arange(0,(np.array(V).shape)[1]*dt,dt)
    np.savetxt(os.path.join("output","LV2","time.txt"),simTime)
