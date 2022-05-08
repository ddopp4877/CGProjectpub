import numpy as np
import pandas as pd
import os
import math
from sklearn import metrics
from scipy.signal import find_peaks

from modules.makeParams import getSimTime

#general
def tag(array,upper,lower):
    failHigh = array > upper
    failLow = array < lower
    passing = (array <= upper) & (array >= lower)
    array[failHigh] = 2
    array[failLow] = 0
    array[passing] = 1
    return array.ravel()

#calculations
def CalcPassiveProperties(v):
    Vrest = []
    Rin = []

    for i in range(0,len(v)):
        vrest = v[i,250] # 50 ms before the current pulse
        midpulsetime = (300/0.2) + (500/0.2)/2 # current injection delay/dt + (duration/dt) /2
        rin = vrest - v[i,int(math.floor(midpulsetime))]# mv/nA = Mohms
        Vrest.append(vrest)
        Rin.append(rin)
        
    return np.array(Vrest),np.array(Rin)

def CalcBaseline(t,v):
    baselines = []
    for i in range(0,(v.shape)[1]):
        avg = np.mean(v[np.where(t<=100),i])
        baselines.append(avg)
    return np.array(baselines)

def CalcAreas(t,v,Baselines):
    Areas = []
    for i in range(0,(v.shape)[1]):
        area = abs(metrics.auc(t,v[:,i] - Baselines[i]))
        Areas.append(area)
    return np.array(Areas)

def CalcPeaks(t,v,Baselines):
    Peaks = []
    for i in range(0,(v.shape)[1]):
        peak = max(v[:,i]-Baselines[i])
        
        Peaks.append(peak)
        
    return np.array(Peaks)

def CalcSPB(v):
    SPB = [] 
    for i in range(0, (v.shape)[1]):
        peaks, _ = find_peaks(v[:,i],distance=90, height = -35)
        SPB.append(len(peaks))
    return np.array(SPB)

    
#Protocols
def LV1RejectionProtocol(v):
    Vrest, Rin= CalcPassiveProperties(v)
    Raw = np.concatenate((Vrest.reshape(1,-1),Rin.reshape(1,-1)),axis=0)
    Vresttagged = tag(Vrest, -50.6, -67.1)
    Rintagged = tag(Rin, 5.03 + 2.4, 5.03 - 2.4)# from Ransdell 2013
    array = np.vstack((Vresttagged,Rintagged))
    passingIdxs = np.equal(Vresttagged,Rintagged)

    return array, Raw, passingIdxs

def LV2RejectionCalculations(t,v):
     
     Baselines = CalcBaseline(t,v)
     Areas = CalcAreas(t,v,Baselines)
     Peaks = CalcPeaks(t,v,Baselines)
     SPBs = CalcSPB(v)
     return Areas,Peaks,SPBs

def LV2RejectionProtocol(v,vTEA):
    #since tag() rewrites the array, save the raw data first
    t = getSimTime(v)
    Areas,Peaks,SPB = LV2RejectionCalculations(t,v)
    AreasTEA,PeaksTEA,SPBTEA = LV2RejectionCalculations(t,vTEA)
    TEAAreaLim = Areas*1.2
    TEAPeaksLim = Peaks*1.3
    TEASPBLim = SPB*1.5

        # join the raw data for return
    rawArgs = [Areas,Peaks,SPB,AreasTEA,PeaksTEA,SPBTEA]
    for i in range(0,len(rawArgs)):
        rawArgs[i] = rawArgs[i].reshape(1,-1)
    rawArray = np.concatenate(rawArgs, axis=0)

    #Now code the arrays as 0,1,2 for fail low, pass, and fail high
    #control
    Areastagged = tag(Areas,18373,4640)
    Peakstagged = tag(Peaks,35,10)
    SPBtagged = tag(SPB,9, 3)
   
    #TEA
    AreasTEAtagged = tag(AreasTEA,36853,TEAAreaLim)
    PeaksTEAtagged = tag(PeaksTEA,100, TEAPeaksLim)#no upper limit
    SPBTEAtagged = tag(SPBTEA, 100, TEASPBLim) # no upper limit
    


    #reshape to use for concatenate. didn't want to change the shape of things in tag()
    
    args = [Areastagged,Peakstagged,SPBtagged,AreasTEAtagged,PeaksTEAtagged, SPBTEAtagged]
    critList = ["Areas", "Peaks", "SPB", "AreasTEA", "PeaksTEA", "SPBTEA"]
    for i in range(0,len(args)):
        args[i] = args[i].reshape(1,-1)
    taggedArray = np.concatenate(args, axis = 0)

    #find the IDXs that pass everything
    passingIdxs = PeaksTEAtagged*AreasTEAtagged*SPBtagged*Peakstagged*Areastagged
    
    return taggedArray, rawArray, passingIdxs, critList

def LV3RejectionProtocol(v,vTEA):
    t = getSimTime(v)
    coded, Raw, passingIdxs,critList = LV2RejectionProtocol( v,vTEA )# coded, values, and indices
  
    
    xcorrs,xcorrsTEA = [],[]
    for i in range(2, (v.shape)[1],5):
        xcorr = abs(np.corrcoef(v[int(300/0.2):int(1400/0.2),i],v[int(300/0.2):int(1400/0.2),i+2])[0][1])
        xcorrTEA = abs(np.corrcoef(vTEA[int(300/0.2):int(1400/0.2),i],v[int(300/0.2):int(1400/0.2),i+2])[0][1])
        xcorrs.append(xcorr)
        xcorrsTEA.append(xcorrTEA)

    
    
    xcorrsRepeated = np.repeat(np.array(xcorrs).reshape(1,-1),5,axis=1)
    xcorrsTEARepeated = np.repeat(np.array(xcorrsTEA).reshape(1,-1),5,axis=1)
    rawArgs = [xcorrsRepeated,xcorrsTEARepeated]
    rawArray = np.concatenate(rawArgs,axis=0)
    rawArray = np.vstack((Raw, rawArray))


    xcorrstagged = tag(np.array(xcorrsRepeated),2 ,0.9425)#no upper limit, expects array

    xcorrsTEAtagged = tag(np.array(xcorrsTEARepeated), 0.8967, 0) # no lower limit

    passingIdxs =  passingIdxs*xcorrstagged*xcorrsTEAtagged

    args = [coded, xcorrstagged.reshape(1,-1), xcorrsTEAtagged.reshape(1,-1),passingIdxs.reshape(1,-1)]
    taggedArray = np.concatenate(args,axis=0)
    critList.append(["xcorrs","xcorrsTEA","AllPass"])
    
    return taggedArray, rawArray, passingIdxs,critList
#returns 1 for pass, 0 for fail, 2 for fail control or tea (can't tell which one but not really important for now), 4 for fail both.

