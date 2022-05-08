import os
from neuron import h
import numpy as np
import platform
from modules.makeParams import *
from modules.RejectionProtocols import *
import matplotlib.pyplot as plt
import pickle
import sys
import pandas as pd

if platform.system() == 'Linux':
    h.nrn_load_dll(os.path.join("modFiles/x86_64/.libs/libnrnmech.so"))
else:
    h.nrn_load_dll(os.path.join("modFiles","nrnmech.dll"))
h.load_file('stdrun.hoc') #so you can use run command

if len(sys.argv) < 4:
    print("trials, seed, voltage filename, parameters filename")
    quit()

#hyperparameters:
dt = 0.2
tstop = 2550#ms
maxstep = 10
vinit = -51#mV
Trials = int(sys.argv[1])
seed  = int(sys.argv[2])


#make a list of LargeCells then change their parameters to be random

params, LCs = makeRandomCellsLV1(Trials,seed)

#make stims for each LC :

iclamps = []
for i in range(0,Trials):

    iclamp = h.IClamp(LCs[i].soma(0.5))
    iclamp.delay = 300
    iclamp.dur = 500
    iclamp.amp = -1
    iclamps.append(iclamp)


#recording variables:
v = []

for i in range(0,Trials):
    v.append(h.Vector().record(LCs[i].soma(0.5)._ref_v))             # Membrane potential vector

h.dt=0.2
h.finitialize(-51)
h.continuerun(2550)
print("done")
V = pd.DataFrame(data = v,dtype='float32')
Params = pd.DataFrame(data = params,dtype = 'float32')
V.to_pickle(os.path.join("output","LV1",sys.argv[3] +  ".pkl"))
Params.to_pickle(os.path.join("output","LV1",sys.argv[4] +  ".pkl"))

