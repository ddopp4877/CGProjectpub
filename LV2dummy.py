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

UNparamsNO = (np.unique(LV1PassParams,axis=1)).shape[1]

h.load_file('stdrun.hoc') #so you can use run command

#make a list of LargeCells then change their parameters to be random

params, LCs = makeRandomCellsLV2(UNparamsNO,seed,LV1PassParams,controlorTEA)
Trials = len(LCs)



gc.collect()

#only need to save params and time once, they dont change for TEA case
if controlorTEA == "Control":
    Params = pd.DataFrame(data = params,dtype = 'float32')
    Params.to_pickle(os.path.join("output","LV2",passparamsfilename + "Repeat" + controlorTEA + ".pkl"))

