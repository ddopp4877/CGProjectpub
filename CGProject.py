import numpy as np
import os
from modules.RejectionProtocols import *
from modules.makeParams import *
from modules.postAnalysis import *
import subprocess
import time
import pickle
from subprocess import Popen, PIPE
import sys
import matplotlib.pyplot as plt

totalStart = time.time()

seed = "222"
LV1Trials = "100"
VoltageFilename= "Vsoma"
ParamsFilename = "Params"
numprocesses = '4'
passParamsFileName = "passParams"
passParamsFileNameRepeat = "passParamsRepeat"
eventTimesFileName = "EventTimes"

#start LV1timer
start = time.time()

#LV1
#output = subprocess.run(['mpiexec', '-n', '1', 'python', 'LV1Simulation.py', LV1Trials, seed, VoltageFilename, ParamsFilename],capture_output=True)
output = subprocess.run(['python', 'LV1Simulation.py', LV1Trials, seed, VoltageFilename, ParamsFilename],capture_output=True)
print(output)
end = time.time()
print("LV1 runtime = %.2f" %(end-start))

#LV1 Rejection Protocol

Vsoma = np.array(pd.read_pickle(os.path.join("output","LV1",VoltageFilename + ".pkl")))
coded, Raw, Idxs= LV1RejectionProtocol(Vsoma)# Rin,Tau, and Vrest coded, and Rin,Tau,Vrest values

#output will always be small enough that we don't really get much from pickle
np.savetxt(os.path.join("output","LV1","LV1RejectionResults.txt"),coded)
np.savetxt(os.path.join("output","LV1","LV1RejectionRaw.txt"),Raw)


#get the passing parameters from LV1 and save
params = np.array(pd.read_pickle(os.path.join("output","LV1",ParamsFilename +".pkl")))
passingIdxs = np.where(Idxs ==1)
passingParams = params[:,passingIdxs[0]]
passParamsLV1 = pd.DataFrame(data = passingParams)
passParamsLV1.to_pickle(os.path.join("output","LV1",passParamsFileName + ".pkl"))
print("Params = %d" %((params.shape)[1]))
print("Passing Params = %d" %((passingParams.shape)[1]))

#create and save a file of the passing parameters repeated 16 times for each freq, for lv2 to use:
LV1PassParamsRepeat = np.repeat(passingParams,16,axis=1)# because testing at 16-32 hz
LV1PassParamsRepeat = pd.DataFrame(LV1PassParamsRepeat)
LV1PassParamsRepeat.to_pickle(os.path.join("input","LV2",passParamsFileNameRepeat + ".pkl"))

### create an array of event times, and save it to a file for lv2simulation to read and use for the source of the netcon that is used in each synapse.
#the event times will be from 16-32 Hz and it will be for each netcon, and for about 30 long, pad with zeros. so shape is (lv1passnumber, 30)
LV1passnumber = (passingParams.shape)[1]
#make a list of the freqs to make event times for:
if LV1passnumber <1:
   print("not enough cells passed LV1")
   quit()
eventTimes, SCfreqs = makeEventTimes(LV1passnumber,seed)
ET = pd.DataFrame(data = eventTimes)
ET.to_pickle(os.path.join("input","LV2",eventTimesFileName + ".pkl"))

#start LV2timer
start = time.time()
#LV2 control
#output = subprocess.run(['mpiexec', '-n', '4', 'python', 'LV2Simulation.py', str(LV1passnumber), seed, VoltageFilename, passParamsFileNameRepeat, eventTimesFileName,"Control"],capture_output=True)
output = subprocess.run(['python', 'LV2Simulation.py',seed, VoltageFilename, passParamsFileNameRepeat, eventTimesFileName,"Control"],capture_output=True)

print(output)
end = time.time()
print("LV2 runtime = %.2f" %(end - start))

#rerun with TEA :
start = time.time()
#output = subprocess.run(['mpiexec', '-n', '1', 'python', 'LV2Simulation.py', str(LV1passnumber), seed, VoltageFilenameTEA, passParamsFileNameRepeat, eventTimesFileName,"TEA"],capture_output=True)
output = subprocess.run(['python', 'LV2Simulation.py', seed, VoltageFilename, passParamsFileNameRepeat, eventTimesFileName,"TEA"],capture_output=True)

print(output)
end = time.time()
print("LV2 TEA runtime = %.2f" %(end - start))

#LV2RejectionProtocol:
timeArray = np.loadtxt(os.path.join("output","LV2","time.txt"))
Vsoma =np.array(pd.read_pickle(os.path.join("output","LV2",VoltageFilename +"Control" +  ".pkl"))).T
VsomaTEA = np.array(pd.read_pickle(os.path.join("output","LV2", VoltageFilename + "TEA" + ".pkl"))).T


coded, Raw, Idxs,critList = LV2RejectionProtocol(timeArray, Vsoma,VsomaTEA )# coded, values, and indexes
np.savetxt(os.path.join("output","LV2","LV2RejectionResults.txt"),coded)
np.savetxt(os.path.join("output","LV2","LV2RejectionRaw.txt"),Raw)


#get the passing parameters from LV2 and save
params = np.array(pd.read_pickle(os.path.join("output","LV2",passParamsFileNameRepeat + "Control" + ".pkl")))
passingIdxs = np.where(Idxs ==1)
passingParams = np.unique(params[:,passingIdxs[0]],axis=1)
passParamsLV2 = pd.DataFrame(data = passingParams)
passParamsLV2.to_pickle(os.path.join("output","LV2",passParamsFileName + ".pkl"))
print("Params = %d" %((params.shape)[1]))
print("LV2 Passing Params = %d" %((passingParams.shape)[1]))


# LV3: 
# put the passing cells of LV2 into sets of 5. then repeat the params 16 times, give to LV3 code. LV3 code will connect all the sizs to each other in a network, and lc12 and lc45 then run.

#make passing params of lv2 a multiple of 5, then repeat each network 16 times, one for each SCfreq
if (passingParams.shape)[1] < 5:
    print("not enough cells to make a network")
else:

    passingParams = passingParams[:,0:int(((passingParams.shape)[1]) - ((passingParams.shape)[1] % 5))]
    passingParamsRepeated = repeatSubarray(passingParams, 5,16)
    passingParamsRepeated = pd.DataFrame(data = passingParamsRepeated)
    passingParamsRepeated.to_pickle(os.path.join("input","LV3",passParamsFileNameRepeat + ".pkl"))

    ### create an array of event times, and save it to a file for lv2simulation to read and use for the source of the netcon that is used in each synapse.
    #the event times will be from 16-32 Hz and it will be for each netcon, and for about 30 long, pad with zeros. so shape is (lv1passnumber, bufferSize)
    LV2passnumber = (passingParams.shape)[1]
    #make a list of the freqs to make event times for, and the event times:
    eventTimes, SCfreqs = makeEventTimes(LV2passnumber,seed)# takes the number of unique cells. return 16 synaptic inputs (spike times) for each passing cell. there are N*16 number of freqs then.



    #since each set of 5 parameters is a network, each unique synaptic input must repeat 5 times, that is, each row repeat 5. 
    #the array given to neuron must have the rows as trials and columns as event times, but repeatSubarray() repeats the subarray horizontally
    eventTimes = np.repeat(eventTimes.T, 5, axis=1 )#take the event times for 1 freq, and repeat it 5 times.
    eventTimes = eventTimes.T


    ET = pd.DataFrame(data = eventTimes)
    ET.to_pickle(os.path.join("input","LV3",eventTimesFileName + ".pkl"))


    startLV3 = time.time()
    output = subprocess.run(['python', 'LV3Simulation.py',seed, VoltageFilename,passParamsFileNameRepeat, eventTimesFileName,"Control"],capture_output=True)
    print(output)
    endLV3 = time.time()
    print("LV3time = %.2f" %(endLV3 - startLV3))

    #rerun with TEA

    startLV3 = time.time()
    output = subprocess.run(['python', 'LV3Simulation.py',seed, VoltageFilename,passParamsFileNameRepeat, eventTimesFileName,"TEA"],capture_output=True)
    print(output)
    endLV3 = time.time()
    print("LV3 TEA time = %.2f" %(endLV3 - startLV3))
  
    #LV3RejectionProtocol:
  
timeArray = np.loadtxt(os.path.join("output","LV3","time.txt"))
Vsoma =np.array(pd.read_pickle(os.path.join("output","LV3",VoltageFilename +"Control" +  ".pkl"))).T
VsomaTEA = np.array(pd.read_pickle(os.path.join("output","LV3", VoltageFilename + "TEA" + ".pkl"))).T

coded, Raw, Idxs,critList = LV3RejectionProtocol(timeArray, Vsoma,VsomaTEA )# coded, values, and indexes
np.savetxt(os.path.join("output","LV3","LV3RejectionResults.txt"),coded)
np.savetxt(os.path.join("output","LV3","LV3RejectionRaw.txt"),Raw)
np.savetxt("passingIdxs.txt", Idxs)

   
    
#get unique passing idxs
    
[a,b] = coded.shape
netPass = np.array([1 if(np.all(coded[a-1,i:i+5]==1)) else 0 for i in range(0,b,5)])# mark 1 if all cells in a net passed
uniqueNetPass = [1 if (np.any(netPass[i:i+16] == 1)) else 0 for i in range(0,len(netPass),16)]# mark 1 if any networks in a set of 16 passed (because it's actually the same net)
netPassNo = uniqueNetPass.count(1)
print("#networks tested = %d\n#trials passed = %d\n#networks passed = %d" %(b/16/5,(list(netPass)).count(1),netPassNo))

uNetPassIdxs = np.repeat(uniqueNetPass,5)

#save passing params and voltages
params = np.array(pd.read_pickle(os.path.join("output","LV3",passParamsFileNameRepeat  + ".pkl")))
params = np.unique(params,axis=1)
passingParams = params[:,(np.where(np.array(uNetPassIdxs)==1))[0]]
passParamsLV3 = pd.DataFrame(data = passingParams)
passParamsLV3.to_pickle(os.path.join("output","LV3",passParamsFileName + ".pkl"))

nonpassingParams = params[:,(np.where(np.array(uNetPassIdxs)!=1))[0]]
nonpassingParams = pd.DataFrame(data = nonpassingParams)
nonpassingParams.to_pickle(os.path.join("output","LV3","nonpassingParams" + ".pkl"))


uNetPassIdxs = np.where(netPass == 1)[0]


totalEnd = time.time()
print("Totaltime = %.2f" %(totalEnd - totalStart))

RejectionResults = np.loadtxt(os.path.join("output","LV2","LV2RejectionResults.txt"))
Voltages = np.array(pd.read_pickle(os.path.join("output","LV2","VsomaControl.pkl"))).T
printLV2Voltages(Voltages,RejectionResults,"LV2 Voltages - Control")

RejectionResults = np.loadtxt(os.path.join("output","LV2","LV2RejectionResults.txt"))
Voltages = np.array(pd.read_pickle(os.path.join("output","LV2","VsomaTEA.pkl"))).T
printLV2Voltages(Voltages,RejectionResults,"LV2 Voltages - TEA")

RejectionResults = np.loadtxt(os.path.join("output","LV3","LV3RejectionResults.txt"))
Voltages = np.array(pd.read_pickle(os.path.join("output","LV3","VsomaControl.pkl"))).T
printNetVoltages(Voltages,RejectionResults,"Network Voltages - Control")

Voltages = np.array(pd.read_pickle(os.path.join("output","LV3","VsomaTEA.pkl"))).T
printNetVoltages(Voltages,RejectionResults,"Network Voltages - TEA")
