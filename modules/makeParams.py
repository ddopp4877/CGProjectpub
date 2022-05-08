import numpy as np
import os
from neuron import h
from numpy.lib.function_base import select
from pandas.core.algorithms import SelectNSeries
import math
import shutil
import pandas as pd
import neuron
import h5py

h.load_file('stdrun.hoc')
#load mechanisms from folder with modfiles. If this doesn't work then just put all the mod files in the same folder as the script that uses them. also this is for windows not linux
import platform

if platform.system() == 'Linux':
    h.nrn_load_dll(os.path.join("..","modFiles","x86_64",".libs/libnrnmech.so"))
else:
    h.nrn_load_dll(os.path.join("..","modFiles","nrnmech.dll"))
#neuron.load_mechanisms(os.path.join("..","modFiles"))





################################################################            FUNCTIONS               ##########################################################################

##GENERAL

def folderList():
    someList  = ['input','output']
    return someList

def subfolderList():
    someList = ['LV1','LV2','LV3']
    return someList 

def convertfilestoH5():
    fileList = os.listdir()
    for file in fileList:
        if file.endswith(".txt"):
            data = np.loadtxt(file)
        if file.endswith(".pkl"):
            data  = np.array(pd.read_pickle(file),dtype = 'float32').T

        filename  = file.split(".")[0]#get the filename without the extension  
        if not file.endswith(".hdf5"):
            with h5py.File(filename +'.hdf5', 'w') as f:
                print("writing...")
                dset = f.create_dataset('default',data=data)

def convertResultstoH5(folderList,subfolderList):# run in the project directory

    for folder in folderList:
        for subfolder in subfolderList:
            os.chdir(os.path.join(os.getcwd(),folder,subfolder))     
            convertfilestoH5()
            os.chdir("..")
            os.chdir("..")

#each folder will have all the subfolders. should be able to make a list of lists for more generalization later
def mkDirandSubdir(folderList,subfolderList):
    dirList = os.listdir()
    for folder in folderList:
        path = os.path.join(os.getcwd(),folder)
        os.mkdir(path)
        for subfolder in subfolderList:
            path = os.path.join(os.getcwd(),folder,subfolder)
            os.mkdir(path)
            
def rmDirs(folderList):#takes a list of the folder names to remove in the cwd
    dirList = os.listdir()
    for folder in folderList:
        if (folder in dirList):
            path = os.path.join(os.getcwd(),folder)
            shutil.rmtree(path)


def makeRandomParams(*args):
    if len(args) < 3:

        print("Enter #Trials, seed, paramDict")
    
    Trials = args[0]
    seed = args[1]
    params = args[2]
    AllParams = np.ones((1,Trials))
    np.random.seed(seed)
    
    for key in params:
        newparam = np.random.uniform(params[key][0], params[key][1],Trials).reshape(-1,Trials)
        AllParams = np.vstack((AllParams,newparam))
    AllParams = AllParams[1:,:]

    return AllParams

def makeCells(Trials,CellType):
    LCAll = []
    for i in range(0,Trials):
        LCAll.append(CellType(i))
    return LCAll


def makeEventTimes(Trials,seed):
    SCfreqs = []
    bufferSize = 50
    np.random.seed(int(seed))
    for j in range(0,Trials):
        for i in range(0,16):
            SCfreqs.append(np.random.uniform(16+i,17+i))
    All = np.zeros((bufferSize))
    for j in range(0,Trials):
        for i in range(0,16):
            freq = SCfreqs[i]*0.6
            spikeNo = math.ceil(freq*.300)
            firstPart = np.linspace(300,600,spikeNo)

            freq = SCfreqs[i]*0.9
            spikeNo = math.ceil(freq*0.6)
            firstPartLastSpikeTime = firstPart[len(firstPart)-1]
            secondPart = np.linspace(600,1200,spikeNo)
            secondSpace = math.floor(secondPart[1]-secondPart[0])
            secondPart = np.linspace(600+secondSpace,1200,spikeNo)

            freq = SCfreqs[i]*0.4
            spikeNo = math.ceil(freq*.100)
            firstSpace = math.floor(firstPart[1]-firstPart[0])
            thirdPart = np.linspace(1200+secondSpace,1300,spikeNo)
        

            spikeTrain = np.hstack((firstPart,secondPart,thirdPart))
            spikeTrain = np.hstack((spikeTrain,np.zeros((bufferSize-len(spikeTrain)))))
            All = np.vstack((All,spikeTrain))
    All = All[1:len(All),:]
    return All, SCfreqs


#this can be used generally, but here I intend to give it a network, and it copies that network 16 times for the LV3 simulation input
def repeatSubarray(array, subSize,repeatNo):#could not figure out how to get np.tile to do this, thus:
    assert len(array.shape) == 2 #this function is only for 2d arrays, with rows for trials and columns for variables
    All = []
    for i in range(0,(array.shape)[1], subSize):
        netArray = array[:, i:i+5 ]#grab the subarray
        netArrayRepeat = np.tile(netArray, (1,repeatNo))#repeat subarray for specified number of times
        All.append(netArrayRepeat)
    All = np.concatenate(All, axis=1)
    return All


def getSimTime(simData, dt=0.2):#2d array, rows are time steps columns are variables; expects numpy
    return np.arange(0,(simData.shape)[0]*dt,dt)

def getNetIDX(netNo,SCfreq):
    return (netNo-1)*(16*5) + ((SCfreq - 16) *5)

def getLV2CellIDX(cellNo,SCfreq):
    return ((cellNo)*16) + (SCfreq-16)

def getParamSet(filepath,netNo,SCfreq):
    paramFile = np.array(pd.read_pickle(filepath))
    return paramFile[:,getNetIDX(netNo,SCfreq)]

def getParamNames(LV):#takes a string noting which LVL
    if LV == 'LV1':
        return list(LV1ParamsDict().keys())
    if LV == 'LV2' or  LV == 'LV3':
        return list(LV1ParamsDict().keys()) + list(LV2ParamsDict().keys())

def rangeVarNames():
    params = {                          
          'soma.g_leak':         [6.2e-5,97e-5],#SOMA
          'soma.g_a2':           [17.2e-5,190e-5],
          'soma.g_bkkca':        [7.9e-4,61e-4],
          'soma.gbar_skkca':     [88e-5,200e-5],
          'soma.g1_kd2':         [16.5e-5,127e-5],
          'soma.g2_kd2':         [91e-6, 500e-6],
          'soma.g_cal':          [6.5e-5,13e-5],
          'soma.g_cat':          [16e-5,31e-5],
          'soma.gbar_Nn':        [7e-5,15e-5],
          'soma.g_nap2':         [7.6e-5,35e-5],
          'neurite.g_leak':      [6.2e-5,97e-5],#NEURITE
          'neurite.g_cat':       [16e-5,31e-5], 
          'neurite.g_cal':       [6.5e-5,13e-5],
          'neurite.g_nap2':      [7.6e-5,35e-5],
          'neurite.g_bkkca':     [7.9e-4,61e-4],
          'siz.g_leak':          [0,1],#SIZ
          'siz.g_nasiz':         [0, 1],
          'siz.g_kdsiz':         [0, 1]
          }
    return params
        

##For LV1
def LV1ParamsDict():
        params = { 
          'soma_leak':      [6.2e-5,97e-5],
          'soma_a2':        [17.2e-5,190e-5],
          'soma_bkkca':     [7.9e-4,61e-4],
          'soma_skkca':     [88e-5,200e-5],
          'soma_kd1':       [16.5e-5,127e-5],
          'soma_kd2':       [91e-6, 500e-6],
          'soma_cal':       [6.5e-5,13e-5],
          'soma_cat':       [16e-5,31e-5],
          'soma_caN':       [7e-5,15e-5],
          'soma_nap2':      [7.6e-5,35e-5],
          'neurite_leak':   [6.2e-5,97e-5]
        }
        return params

def LV1ParamsDictRestricted():
        params = { 
          'soma_leak':      [6.2e-5,20e-5],
          'soma_a2':        [17.2e-5,190e-5],
          'soma_bkkca':     [7.9e-4,61e-4],
          'soma_skkca':     [88e-5,200e-5],
          'soma_kd1':       [16.5e-5,127e-5],
          'soma_kd2':       [91e-6, 500e-6],
          'soma_cal':       [6.5e-5,13e-5],
          'soma_cat':       [16e-5,31e-5],
          'soma_caN':       [7e-5,15e-5],
          'soma_nap2':      [7.6e-5,35e-5],
          'neurite_leak':   [6.2e-5,97e-5]
        }
        return params

def makeRandomCellsLV1(*args):
    if(len(args)!=2):
        print("enter makeRandomCells(#Trials, seed)")
    else:
        Trials = args[0]
        seed = args[1]
        #make the random parameter array
        paramsDict = LV1ParamsDictRestricted()
        #paramsDict = LV1ParamsDict()
        params = makeRandomParams(Trials,seed,paramsDict)

        #make the same number of cells, with default initialization (not written or needed)
        LCs = makeCells(Trials,LargeCellLV1)
    
        #assign the param(:,i) to LC1(i)
        for i in range(0,Trials):

            LCs[i].soma.g_leak =        params[0,i]
            LCs[i].soma.g_a2 =          params[1,i]
            LCs[i].soma.g_bkkca =       params[2,i]
            LCs[i].soma.gbar_skkca =    params[3,i]
            LCs[i].soma.g1_kd2 =        params[4,i]
            LCs[i].soma.g2_kd2 =        params[5,i]
            LCs[i].soma.g_cal =         params[6,i]
            LCs[i].soma.g_cat =         params[7,i]
            LCs[i].soma.gbar_Nn =       params[8,i]
            LCs[i].soma.g_nap2 =        params[9,i]
            LCs[i].neurite.g_leak =     params[10,i]

        return params, LCs


#For LV2
def LV2ParamsDict():
    params = { 
          'neurite_leak': [6.2e-5,97e-5],
          'neurite_cat': [16e-5,31e-5],
          'neurite_cal': [6.5e-5,13e-5],
          'neurite_nap2': [7.6e-5,35e-5],
          'neurite_bkkca': [7.9e-4,61e-4],
          
         }
    return params

def LV2ParamsDictRestricted():
    params = { 
          'neurite_leak': [6.2e-5,40e-5],
          'neurite_cat': [16e-5,31e-5],
          'neurite_cal': [6.5e-5,13e-5],
          'neurite_nap2': [10e-5,35e-5],
          'neurite_bkkca': [7.9e-4,61e-4],
          
         }
    return params

def LV3CritList():
    critList = ['AUC_Control','Peaks_Control','SPB_Control','AUC_TEA','Peaks_TEA','SPB_TEA','Synchrony','Synchrony_TEA']
    return critList

def LV2CritList():
    critList = ['AUC_Control','Peaks_Control','SPB_Control','AUC_TEA','Peaks_TEA','SPB_TEA']
    return critList
    

def fullParamsList():
    lv3ParamsList = list(LV1ParamsDict().keys()) + list(LV2ParamsDict().keys())
    lv3ParamsList.remove('neurite_leak')
    return lv3ParamsList


def makeRandomCellsLV2(*args):

    if(len(args)!=4):
        print("enter makeRandomCells(#Trials, seed,LV1Passarray, ""Control"" or ""TEA""")
    else:
        if args[3] == "Control":
            teaEffect = 1
        if args[3] == "TEA":
            teaEffect = 0.03
        Trials = args[0]# because testing at 16-32 hz
        seed = args[1]
        #make the random parameter arrays
        LV1Params = np.repeat(args[2],16,axis=1)# because testing at 16-32 hz
        LV2Params = makeRandomParams(args[0],seed, LV2ParamsDictRestricted())#expects the number of unique parameters
        LV2Params = np.repeat(LV2Params,16,axis=1)

        #make the same number of cells, with default initialization
        LCs = makeCells(Trials*16, LargeCellLV2)
        [a,b] = LV2Params.shape
        LV1Params = LV1Params[:,:b]#in case they are not the same size
        params = np.vstack((LV1Params,LV2Params))
        
        for i in range(0,Trials*16):

            LCs[i].soma.g_leak =        params[0,i]
            LCs[i].soma.g_a2 =          params[1,i]*teaEffect
            LCs[i].soma.g_bkkca =       params[2,i]*teaEffect
            LCs[i].soma.gbar_skkca =    params[3,i]
            LCs[i].soma.g1_kd2 =        params[4,i]*teaEffect
            LCs[i].soma.g2_kd2 =        params[5,i]*teaEffect
            LCs[i].soma.g_cal =         params[6,i]
            LCs[i].soma.g_cat =         params[7,i]
            LCs[i].soma.gbar_Nn =       params[8,i]
            LCs[i].soma.g_nap2 =        params[9,i]
            
            LCs[i].neurite.g_leak =     params[11,i]#skip 10, since it is gleak from lv1 and not for lv2
            LCs[i].neurite.g_cat =      params[12,i]
            LCs[i].neurite.g_cal =      params[13,i]
            LCs[i].neurite.g_nap2 =     params[14,i]
            LCs[i].neurite.g_bkkca =    params[15,i]*teaEffect

            LCs[i].siz.g_leak =         0.006#params[0,i]#should remain consistent?
            LCs[i].siz.g_nasiz =        0.6
            LCs[i].siz.g_kdsiz =        0.02
            #siz conductances are all the same, and defined in the class
        
        params = np.delete(params,10,axis=0)# the 10th param is from lv1 not lv2
        return params, LCs



## for LV3
def makeCellsLV3(LV2PassParams,controlorTEA):#the passparams file should be each network repeated 16 times, so this only assigns the params to the channels
        if controlorTEA == "Control":
            teaEffect = 1
        if controlorTEA == "TEA":
            teaEffect = 0.03
        Trials = (LV2PassParams.shape)[1]
        params = LV2PassParams
        LCs = makeCells(Trials, LargeCellLV2)
        for i in range(0,Trials):

            LCs[i].soma.g_leak =        params[0,i]
            LCs[i].soma.g_a2 =          params[1,i]*teaEffect
            LCs[i].soma.g_bkkca =       params[2,i]*teaEffect
            LCs[i].soma.gbar_skkca =    params[3,i]
            LCs[i].soma.g1_kd2 =        params[4,i]*teaEffect
            LCs[i].soma.g2_kd2 =        params[5,i]*teaEffect
            LCs[i].soma.g_cal =         params[6,i]
            LCs[i].soma.g_cat =         params[7,i]
            LCs[i].soma.gbar_Nn =       params[8,i]
            LCs[i].soma.g_nap2 =        params[9,i]
            
            LCs[i].neurite.g_leak =     params[10,i]
            LCs[i].neurite.g_cat =      params[11,i]
            LCs[i].neurite.g_cal =      params[12,i]
            LCs[i].neurite.g_nap2 =     params[13,i]
            LCs[i].neurite.g_bkkca =    params[14,i]*teaEffect

            LCs[i].siz.g_leak =         0.002#params[0,i]#should remain consistent?
            LCs[i].siz.g_nasiz =        0.5
            LCs[i].siz.g_kdsiz =        0.05
            #siz conductances are all the same, and defined in the class

        return params, LCs

def makeAvgNets(LV3passParams):#LV3passParams should be the unique networks which pass lv3
    [a,b] = LV3passParams.shape

    avgNets = np.ones((a,1))

    for i in range(0,b,5):
        avgNets = np.hstack((avgNets,np.mean(LV3passParams[:,i:i+5],axis=1).reshape(-1,1)))
    avgNets = avgNets[:,1:]

    avgNets = np.repeat(avgNets,5,axis=1)

    return avgNets
################################################################            CLASSES               ##########################################################################

#make a Large Cell for LV1 (truncated, passive soma)
class LargeCellLV1:

    def __init__(self,gid):
        self._gid = gid
        self.soma = h.Section(name='soma',cell=self)
        self.neurite = h.Section(name = 'neurite',cell=self)
    #define soma morphology and passive properties
        self.soma.L = 120
        self.soma.diam = 90
        self.soma.Ra = 50
        self.soma.cm = 1.5

    #insert soma channels:
        self.soma.insert('leak')
        self.soma.insert('a2')
        self.soma.insert('bkkca')
        self.soma.insert('skkca')
        self.soma.insert('kd2')
        self.soma.insert('cal')
        self.soma.insert('cat')
        self.soma.insert('Nn')
        self.soma.insert('nap2')
        self.soma.insert('pool')

    #initialize soma reversal potentials and fpool

        self.soma.ek = -80
        self.soma.e_leak = -51
        self.soma.ena = 55
        self.soma.eca = 45
        self.soma.enn = -30
        self.soma.f_pool = 20e6 #unclear what fcac stands for, originally f_pool was set to this but it is a parameter not range
 
    #define neurite morphology and passive properties
    
        self.neurite.L = 600
        self.neurite.diam = 12
        self.neurite.cm = 1.5
        self.neurite.Ra = 50


    #insert neurite channels:
        self.neurite.insert('leak')

    #initialize neurite reversal potentials and conductances (S/cm^2)
        self.neurite.e_leak = -51

    #connect the child section (neurite) to the parent section (soma)
        self.neurite.connect(self.soma,1,0)
    def __repr__(self):
        return 'LargeCell[{}]'.format(self._gid)


class LargeCellLV2:
    def __init__(self,gid):
        self._gid = gid
        self.soma = h.Section(name='soma',cell=self)
        self.neurite = h.Section(name = 'neurite',cell=self)
        self.siz = h.Section(name = 'siz', cell = self)

    #define soma morphology and passive properties
        self.soma.L = 120
        self.soma.diam = 90
        self.soma.Ra = 50
        self.soma.cm = 1.5
       

    #insert soma channels:
        self.soma.insert('leak')
        self.soma.insert('a2')
        self.soma.insert('bkkca')
        self.soma.insert('skkca')
        self.soma.insert('kd2')
        self.soma.insert('cal')
        self.soma.insert('cat')
        self.soma.insert('Nn')
        self.soma.insert('nap2')
        self.soma.insert('pool')

    #initialize soma reversal potentials and conductances

        self.soma.ek = -80
        self.soma.e_leak = -51
        self.soma.ena = 55
        self.soma.eca = 45
        self.soma.enn = -30
        self.soma.f_pool = 20e6 #unclear what fcac stands for, originally f_pool was set to this but it is a parameter not range
 
    #define neurite morphology and passive properties
    
        self.neurite.L = 1380
        self.neurite.diam = 12
        self.neurite.cm = 1.5
        self.neurite.Ra = 50


    #insert neurite channels:
        self.neurite.insert('leak')
        self.neurite.insert('cat')
        self.neurite.insert('cal')
        self.neurite.insert('nap2')
        self.neurite.insert('bkkca')

    #initialize neurite reversal potentials and conductances (S/cm^2)
        self.neurite.e_leak = -51
        self.neurite.ek = -80
        self.neurite.ena = 55
        self.neurite.eca = 45
 

    #define SIZ morphology and passive properties
        self.siz.L = 108
        self.siz.diam = 20
        self.siz.cm = 1.5
        self.siz.Ra = 50
    #insert siz channels:
        self.siz.insert('leak')
        self.siz.insert('nasiz')
        self.siz.insert('kdsiz')
    #initialize size reversal potentials and conductances
        self.siz.e_leak = -51
        self.siz.ek = -80
        self.siz.ena = 55
        self.siz.g_nasiz = 0.6
        self.siz.g_kdsiz = 0.1
    #connect the child section (neurite) to the parent section (soma)
        self.neurite.connect(self.soma(1),0)
        self.siz.connect(self.neurite(1),0)
    def __repr__(self):
        return 'LargeCell[{}]'.format(self._gid)

