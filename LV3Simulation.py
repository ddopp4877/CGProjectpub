from neuron import h
import numpy as np
import os
from modules.makeParams import *
from modules.RejectionProtocols import *
import sys
import pandas as pd
import platform


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
averageorNo = sys.argv[6]
averageorNo = averageorNo.lower()
if averageorNo == 'avg':
    inpath = os.path.join("input","LV3","Avg")
    outpath = os.path.join("output","LV3","Avg")
else:
    inpath = os.path.join("input","LV3")
    outpath = os.path.join("output","LV3")

eventTimes = np.array(pd.read_pickle(os.path.join(inpath,eventtimesfilename + ".pkl")))
LV2PassParams =  np.array(pd.read_pickle(os.path.join(inpath, passparamsfilename+ ".pkl")))

Trials = (LV2PassParams.shape)[1]

UNparamsNO = (np.unique(LV2PassParams,axis=1)).shape[1]

h.load_file('stdrun.hoc') #so you can use run command

#make a list of LargeCells, and assign the parameters from LV2 to these cells.

params, LCs = makeCellsLV3(LV2PassParams,controlorTEA)

#read in event times, turn it into a vector, remove the zeros, play the vector as the source of the vectstim, which is the source of the netcon to the exp2syn target

synGain = 0.13

#recording variables:

vsAll = [h.VecStim() for i in range(0,Trials)]
ETs = [h.Vector(eventTimes[i,eventTimes[i,:] !=0]) for i in range(0,Trials)]
syns = [h.Exp2Syn(LCs[i].siz(1)) for i in range(0,Trials)]

for i in range(0,Trials):
    vsAll[i].play(ETs[i])
    syns[i].tau1,syns[i].tau2,syns[i].e   = 10,120,-15


NetCons = [h.NetCon(vsAll[i],syns[i],-10,0,synGain) for i in range(0,Trials)]  
# connect all the sizs in a network, and LC1 and LC2 in a network

LC1s,LC2s,LC3s,LC4s,LC5s = list(np.arange(0,Trials,5)),list(np.arange(1 ,Trials,5)),list(np.arange(2 ,Trials,5)),list(np.arange(3,Trials,5)),list(np.arange(4 ,Trials,5))

RSOMA=1.6#1.54
RSIZ=1
g,gSIZ1,gSIZ2,gSIZ3,gSIZ4 = [],[],[],[],[]

def setGapSoma(LCA,LCB,g):
    
    newGap = h.GAP(LCs[LCA].soma(0.5))
    g.append(newGap)
    i = len(g) - 1
    g[i].r = RSOMA
    h.setpointer(LCs[LCB].soma(0.5)._ref_v,'vgap',g[i])

def setGapSIZ(LCA,LCB,gSIZ):
    newGap = h.GAP(LCs[LCA].siz(0.5))
    gSIZ.append(newGap)
    i = len(gSIZ) - 1
    gSIZ[i].r = RSIZ
    h.setpointer(LCs[LCB].siz(0.5)._ref_v,'vgap',gSIZ[i])


###########    add the gap junctions. Probably better to do this with a connectivity matrix but anyways:
connNo = len(LC1s)
[setGapSoma(LC1s[i],LC2s[i],g) for i in range(connNo)]
[setGapSoma(LC2s[i],LC1s[i],g) for i in range(connNo)]
[setGapSoma(LC4s[i],LC5s[i],g) for i in range(connNo)]
[setGapSoma(LC5s[i],LC4s[i],g) for i in range(connNo)]

[setGapSIZ(LC1s[i],LC2s[i],gSIZ1) for i in range(connNo)]
[setGapSIZ(LC2s[i],LC1s[i],gSIZ1) for i in range(connNo)]
[setGapSIZ(LC1s[i],LC3s[i],gSIZ1) for i in range(connNo)]
[setGapSIZ(LC3s[i],LC1s[i],gSIZ1) for i in range(connNo)]
[setGapSIZ(LC1s[i],LC4s[i],gSIZ1) for i in range(connNo)]
[setGapSIZ(LC4s[i],LC1s[i],gSIZ1) for i in range(connNo)]
[setGapSIZ(LC1s[i],LC5s[i],gSIZ1) for i in range(connNo)]
[setGapSIZ(LC5s[i],LC1s[i],gSIZ1) for i in range(connNo)]

[setGapSIZ(LC2s[i],LC3s[i],gSIZ1) for i in range(connNo)]
[setGapSIZ(LC3s[i],LC2s[i],gSIZ1) for i in range(connNo)]
[setGapSIZ(LC2s[i],LC4s[i],gSIZ1) for i in range(connNo)]
[setGapSIZ(LC4s[i],LC2s[i],gSIZ1) for i in range(connNo)]
[setGapSIZ(LC2s[i],LC5s[i],gSIZ1) for i in range(connNo)]
[setGapSIZ(LC5s[i],LC2s[i],gSIZ1) for i in range(connNo)]

[setGapSIZ(LC3s[i],LC4s[i],gSIZ1) for i in range(connNo)]
[setGapSIZ(LC4s[i],LC3s[i],gSIZ1) for i in range(connNo)]
[setGapSIZ(LC3s[i],LC5s[i],gSIZ1) for i in range(connNo)]
[setGapSIZ(LC5s[i],LC3s[i],gSIZ1) for i in range(connNo)]

[setGapSIZ(LC4s[i],LC5s[i],gSIZ1) for i in range(connNo)]
[setGapSIZ(LC5s[i],LC4s[i],gSIZ1) for i in range(connNo)]

v = [h.Vector().record(LCs[i].soma(0.5)._ref_v) for i in range(0,Trials)]

h.dt=0.2
h.finitialize(-51)
h.continuerun(2550)
 

V = pd.DataFrame(data = v,dtype='float32')
V.to_pickle(os.path.join(outpath,voutfilename + controlorTEA + ".pkl"))
if controlorTEA == "Control":
    Params = pd.DataFrame(data = params,dtype = 'float32')
    Params.to_pickle(os.path.join(outpath,passparamsfilename + ".pkl"))


