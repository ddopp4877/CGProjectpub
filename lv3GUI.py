#LV3GUI
import platform
from tkinter.font import BOLD
from neuron import h
import numpy as np
import os
from modules.makeParams import *
import pandas as pd
from itertools import combinations
from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg, NavigationToolbar2Tk)



if platform.system() == 'Linux':
    h.nrn_load_dll(os.path.join("modFiles/x86_64/.libs/libnrnmech.so"))
else:
    h.nrn_load_dll(os.path.join("modFiles","nrnmech.dll"))




#hyperparameters:
dt = 0.2
tstop = 2550#ms
maxstep = 10
vinit = -51#mV
seed = 222
voutfilename = "Vsoma"
passparamsfilename = "passParamsRepeat"
eventtimesfilename = "EventTimes"
boxColControlLabel = 0
boxColControl = 1
boxColControlLabelRange = 2
plotCols = 3
teaBoxLabelRangeCol = 4
teaBoxCol = 5
teaBoxLabelCol = 6

os.chdir("modFiles")
os.system("nrnivmodl")
os.chdir("..")

#archivedPath = os.path.join("..","CGResults","fixed_Gsyn","MedwithLV2")
eventTimes = np.array(pd.read_pickle(os.path.join("LV3","input",eventtimesfilename + ".pkl")))
LV2PassParams =  np.array(pd.read_pickle(os.path.join("LV3","output", passparamsfilename+ ".pkl")))

#eventTimes = np.array(pd.read_pickle(r'C:\Users\ddopp\source\repos\CGresults\fixed_Gsyn\Low\LV3\EventTimesControl.pkl'))
#LV2PassParams =  np.array(pd.read_pickle(r'C:\Users\ddopp\source\repos\CGresults\fixed_Gsyn\Low\LV3\passParamsRepeat.pkl'))


Trials = (LV2PassParams.shape)[1]
print(LV2PassParams.shape)
subTrials = 5
#eventTimes = eventTimes[:,:subTrials]
#LV2PassParams = LV2PassParams[:,:Trials]
ETs = [h.Vector(eventTimes[i,eventTimes[i,:] !=0]) for i in range(0,Trials)]

h.load_file('stdrun.hoc') #so you can use run command

comboList = list(combinations([0,1,2,3,4],2))

class Network:
    def __init__(self, netNo,FreqNo,controlorTEA):
        self.netNo = netNo
        self.FreqNo = FreqNo
        self.startNo = getNetIDX(self.netNo,self.FreqNo)
        netParams = LV2PassParams[:,self.startNo:self.startNo+5]
        self.params, self.LCs = makeCellsLV3(netParams,controlorTEA)
        self.ETs = ETs[self.startNo:self.startNo+5]

        self.vsAll = [h.VecStim() for i in range(0,subTrials)]

        self.syns = self.createSyns()
        self.setSyns()

        self.synGain = 0.09
        self.RSOMA= 1.54
        self.RSIZ =200

        self.setEventTimes()
        self.NetCons = self.createNetCons()
    # connect all the sizs in a network, and LC1 and LC2 Somas and LC4 and LC5

        self.g,self.gSIZ = self.createSomaGaps(),self.createSIZGaps()
        self.v =  [h.Vector().record(self.LCs[i].soma(0.5)._ref_v) for i in range(0,subTrials)]

    def createSIZGaps(self):
        #join all siz's, referencing the combinations list (global) for all the connections.
        self.gSIZ = []
        [self.setGapSIZ(self.LCs[comboList[i][0]],self.LCs[comboList[i][1]],self.gSIZ) for i in range(len(comboList))]
        
        return self.gSIZ

    def createSomaGaps(self):
        self.g = []
        self.setGapSoma(self.LCs[0],self.LCs[1],self.g)
        self.setGapSoma(self.LCs[3],self.LCs[4],self.g)
        return self.g

    def createNetCons(self):
        return [h.NetCon(self.vsAll[i],self.syns[i],-10,0,self.synGain) for i in range(0,subTrials)]

    def setEventTimes(self):
        [self.vsAll[i].play(self.ETs[i]) for i in range(0,subTrials)]
        
    def setSyns(self):
        for i in range(0,subTrials):
            self.syns[i].tau1,self.syns[i].tau2,self.syns[i].e   = 10,120,-15

    def createSyns(self):
        return [h.Exp2Syn(self.LCs[i].siz(1)) for i in range(0,subTrials)]

    def setGapSoma(self,LCA,LCB,g):#join gap junction between a and b and b and a
        newGap = h.GAP(LCA.soma(0.5))
        g.append(newGap)
        i = len(g) - 1
        g[i].r = self.RSOMA
        h.setpointer(LCB.soma(0.5)._ref_v,'vgap',g[i])

        newGap = h.GAP(LCB.soma(0.5))
        g.append(newGap)
        i = len(g)-1
        g[i].r = self.RSOMA
        h.setpointer(LCA.soma(0.5)._ref_v,'vgap',g[i])

    
    def setGapSIZ(self,LCA,LCB,gSIZ):# join gap junction between a and b and b and a
        newGap = h.GAP(LCA.siz(0.5))
        gSIZ.append(newGap)
        i = len(gSIZ) - 1
        gSIZ[i].r = self.RSIZ
        h.setpointer(LCB.siz(0.5)._ref_v,'vgap',gSIZ[i])
        
        newGap = h.GAP(LCB.siz(0.5))
        gSIZ.append(newGap)
        i = len(gSIZ) - 1
        gSIZ[i].r = self.RSIZ
        h.setpointer(LCA.siz(0.5)._ref_v,'vgap',gSIZ[i])
    
    
    
        
    def run(self):
        stopTime = 1800
        self.vloc = self.v
        #self.v =  [h.Vector().record(self.LCs[i].soma(0.5)._ref_v) for i in range(0,subTrials)]
        #self.vSIZ = [h.Vector().record(self.LCs[i].siz(0.5)._ref_v) for i in range(0,subTrials)] 
        h.dt = 0.2
        h.finitialize(-51)
        h.continuerun(tstop)
        return np.array(self.vloc).T[:int(stopTime/dt),:]
        #return np.array(self.vSIZ).T[:int(stopTime/dt),:]



import matplotlib.pyplot as plt
from matplotlib.widgets import Slider
import tkinter as tk

class Window:
    def __init__(self,master):
        self.frame = tk.Frame(master,width = 200,height=10)


        ### loop through the params and make a spinbox for each one with its specified range from the dictionary
        self.myVars = list(rangeVarNames().keys())
        self.myVarsValues = list(rangeVarNames().values())
        self.allSpins = []

        self.myVarsTEA = list(rangeVarNames().keys())
        self.myVarsValuesTEA = list(rangeVarNames().values())
        self.allSpinsTEA = []
        
        #self.stringVar = tk.StringVar(master,"Control")
        self.bold25 =tk.font.Font(master, size=15, weight=BOLD)
        self.ControlBoxesLabel = tk.Label(master, text="Control",font = self.bold25)
        self.ControlBoxesLabel.grid(row = 0,column = boxColControl, pady = 0)
        self.TEABoxesLabel = tk.Label(master, text = "TEA" ,font = self.bold25).grid(row = 0,column = teaBoxCol, pady = 10)
        
        self.usedParams = myNet.myNetControl.params
        self.usedParamsTEA = myNet.myNetTEA.params
        val1 = myNet.myNetControl.LCs[0].siz.g_leak
        val2 = myNet.myNetControl.LCs[0].siz.g_nasiz
        val3 = myNet.myNetControl.LCs[0].siz.g_kdsiz
        
        ar1 = np.repeat(np.array([val1,val2,val3]),5).reshape(3,5)
        ar2 = np.ones((3,5))
        self.nextParams = np.multiply(ar1,ar2)
        self.usedParams = np.vstack((self.usedParams,self.nextParams))
        
        self.usedParamsTEA = np.vstack((self.usedParamsTEA,self.nextParams))
                

        self.NetBox = netSelect(master,"Network Number",30,2)
        self.FreqBox = netSelect(master,"SCfrequency",32,2)

        self.vecSelect = vecSelect(master,31,1)
        
        
    
        for i in range(0,len(self.myVars)):
            
            self.allSpins.append(MySpinBox(master,self.usedParams[i,0],self.myVarsValues[i][1],1e-6,self.myVars[i],i+1,boxColControl,0,2,"myNetControl"))
            self.controlVarText = '({0:>.5f}   -   {1:>.5f})'.format(self.myVarsValues[i][0],self.myVarsValues[i][1])
            self.rangeLabel = tk.Label(master, text = self.controlVarText).grid(row = i+1,column = boxColControlLabelRange,padx=5)

            
            self.allSpinsTEA.append(MySpinBox(master,self.usedParamsTEA[i,0],self.myVarsValuesTEA[i][1],1e-6,self.myVarsTEA[i],i+1,teaBoxCol,0,8,"myNetTEA"))
            self.teaVarText = '({0:>.5f}   -   {1:>.5f})'.format(self.myVarsValuesTEA[i][0],self.myVarsValuesTEA[i][1])
            self.rangeLabel = tk.Label(master, text = self.teaVarText).grid(row = i+1,column = teaBoxLabelRangeCol,padx = 5)
        
        self.cellSelect = cellSelect(master,31,0)
        self.resetButton = resetButton(master,31,4)

class resetButton():
    def __init__(self,master,rownum,columnum):
        self.labelname = "Reset"
        self.button = tk.Button(master,text=self.labelname,command=self.run)
        self.button.grid(row = rownum,column = columnum)
        self.bindings()
    def run(self,*args):
        myNet.__init__(1,16)
        #myNet.myNetControl.__init__(1,16,"Control")
    def bindings(self):
        self.button.bind('<Return>',self.run)
    


class cellSelect():
    def __init__(self,master,rownum,columnum):
        self.master = master
        self.labelname = "Select Cell #"
        self.string_var = tk.StringVar()
        self.string_var.set("0")
        self.enterbox = tk.Entry(master,textvariable=self.string_var)
        self.enterbox.grid(row = rownum,column = columnum)
        
        self.boxLabel = tk.Label(master,text = self.labelname).grid(row = rownum+1,column = columnum)

        self.bindings()

    def display(self,*args):
        self.value = self.string_var.get()
        print(self.value)
        self.myVarsValues = list(rangeVarNames().values())
        self.myVars = list(rangeVarNames().keys())
        #window.cellNo = self.value
        for i in range(len(window.allSpins)):
            window.allSpins[i].cellNo = self.value
            
        for i in range(len(window.allSpinsTEA)):
            window.allSpinsTEA[i].cellNo = self.value
            
        for i in range(myNet.myNetControl.params.shape[0]):

            window.allSpins[i].__init__(self.master,myNet.myNetControl.params[i,int(self.value)],self.myVarsValues[i][1],1e-6,self.myVars[i],i+1,boxColControl,0,2,"myNetControl")
            window.allSpins[i].cellNo = self.value
            
            window.allSpinsTEA[i].__init__(self.master,myNet.myNetTEA.params[i,int(self.value)],self.myVarsValues[i][1],1e-6,self.myVars[i],i+1,teaBoxCol,0,8,"myNetTEA")
            window.allSpinsTEA[i].cellNo = self.value
            
        #exec("%s = %f" %("myNet." + window.allSpins[0].controlorTEA+ ".LCs["+str(window.allSpins[int(self.value)].cellNo)+"]." + window.allSpins[int(self.value)].boxLabel,float(window.allSpins[int(self.value)].spinbox.getvalue)))
        #self.value = self.spinbox.get()
        #exec("%s = %f" %("myNet." + self.controlorTEA+ ".LCs["+str(self.cellNo)+"]." + self.boxLabel,float(self.value)))
        #myNet.myNetControl.setSyns()
        #myNet.myNetControl.NetCons = myNet.myNetControl.createNetCons()
        #myNet.myNetControl.g,myNet.myNetControl.gSIZ = myNet.myNetControl.createSomaGaps(),myNet.myNetControl.createSIZGaps()
        #myNet.update()
        #print(window.allSpins[int(self.value)].display())
        #for i in range(0,len(window.myVars)):
            
            #window.allSpins[i].cellNo = window.usedParams[i,int(self.value)]#MySpinBox(master,window.usedParams[i,int(self.value)],window.myVarsValues[i][1],1e-6,window.myVars[i],i+1,boxColControl,0,2,"myNetControl")
            #print(window.usedParams[i,int(self.value)])
            #append(MySpinBox(master,window.usedParams[i,int(self.value)],window.myVarsValues[i][1],1e-6,window.myVars[i],i+1,boxColControl,0,2,"myNetControl"))
            
            #window.allSpinsTEA.append(MySpinBox(master,self.usedParamsTEA[i,int(self.value)],self.myVarsValuesTEA[i][1],1e-6,self.myVarsTEA[i],i+1,teaBoxCol,0,8,"myNetTEA"))

        myNet.update()
        myNet.updateTEA()
        
        
    def bindings(self):
        self.enterbox.bind('<Return>',self.display)


class vecSelect():
    
    def __init__(self,master,rownum,columnum):
        
        self.labelname = 'Change to Soma or SIZ'
        self.str1 = tk.StringVar(master)
        self.str1.set("Soma")
        self.enterbox = tk.Button(master,text=self.labelname,command=self.display)
        self.enterbox.grid(row = rownum,column = columnum)
        self.vecLabel = tk.Label(master,text = self.str1.get())
        self.vecLabel.grid(row = rownum+1,column = columnum)
        self.bindings()
        

    def display(self):
        
        if self.str1.get() == 'Soma':
            self.str1.set("SIZ")
            self.vecLabel['text'] = self.str1.get()
            myNet.myNetControl.v = [h.Vector().record(myNet.myNetControl.LCs[i].siz(0.5)._ref_v) for i in range(0,subTrials)] 
            myNet.myNetTEA.v = [h.Vector().record(myNet.myNetTEA.LCs[i].siz(0.5)._ref_v) for i in range(0,subTrials)]
            
        else:
            self.str1.set("Soma")
            self.vecLabel['text'] = self.str1.get()
            myNet.myNetControl.v =  [h.Vector().record(myNet.myNetControl.LCs[i].soma(0.5)._ref_v) for i in range(0,subTrials)]
            myNet.myNetTEA.v =  [h.Vector().record(myNet.myNetTEA.LCs[i].soma(0.5)._ref_v) for i in range(0,subTrials)]
            
        print(self.str1.get())

        myNet.update()
        myNet.updateTEA()
    
    def bindings(self):
        self.enterbox.bind('<Return>',self.display)
        

class netSelect():
    def __init__(self,master,labelname,rownum,columnum):
        self.labelname = labelname
        self.string_var = tk.StringVar()
        self.enterbox = tk.Entry(master,textvariable=self.string_var)
        self.enterbox.grid(row = rownum,column = columnum)
        self.NetNoLabel = tk.Label(master,text = labelname).grid(row = rownum+1,column = columnum)

        self.bindings()

    def display(self,master,*args):
        self.value = self.string_var.get()
        if self.labelname == "Network Number":
            myNet.myNetControl.netNo = int(self.value)
            myNet.myNetTEA.netNo = int(self.value)
            print("net change")
        if self.labelname == "SCfrequency":
            myNet.myNetControl.FreqNo = int(self.value)
            myNet.myNetTEA.FreqNo = int(self.value)
            print("freq change")

        
        self.startNo = getNetIDX(myNet.myNetControl.netNo,myNet.myNetTEA.FreqNo)
        netParams = LV2PassParams[:,self.startNo:self.startNo+5]
        self.params, self.LCs = makeCellsLV3(netParams,"Control")
        
        myNet.myNetControl.params = self.params
        myNet.myNetControl.LCs = self.LCs
        myNet.myNetControl.ETs = ETs[self.startNo:self.startNo+5]
        myNet.myNetControl.vsAll = [h.VecStim() for i in range(0,subTrials)]
        myNet.myNetControl.syns = myNet.myNetControl.createSyns()
        myNet.myNetControl.setSyns()
        myNet.myNetControl.setEventTimes()
        myNet.myNetControl.NetCons = myNet.myNetControl.createNetCons()
        myNet.myNetControl.g,myNet.myNetControl.gSIZ = myNet.myNetControl.createSomaGaps(),myNet.myNetControl.createSIZGaps()
        myNet.myNetControl.v =  [h.Vector().record(myNet.myNetControl.LCs[i].soma(0.5)._ref_v) for i in range(0,subTrials)]
        #myNet.myNetControl.v =  [h.Vector().record(myNet.myNetControl.LCs[i].soma(0.5)._ref_v) for i in range(0,subTrials)]
        myNet.controlPlot.fig.suptitle('%s - Network %d at %d Hz' %("Control",myNet.myNetControl.netNo, myNet.myNetControl.FreqNo))
        myNet.update()
  
        myNet.myNetTEA.params = self.params
        self.params, self.LCs = makeCellsLV3(netParams,"TEA")
        myNet.myNetTEA.LCs = self.LCs
        myNet.myNetTEA.ETs = ETs[self.startNo:self.startNo+5]
        myNet.myNetTEA.vsAll = [h.VecStim() for i in range(0,subTrials)]
        myNet.myNetTEA.syns = myNet.myNetTEA.createSyns()
        myNet.myNetTEA.setSyns()
        myNet.myNetTEA.setEventTimes()
        myNet.myNetTEA.NetCons = myNet.myNetTEA.createNetCons()
        myNet.myNetTEA.g,myNet.myNetTEA.gSIZ = myNet.myNetTEA.createSomaGaps(),myNet.myNetTEA.createSIZGaps()
        myNet.myNetTEA.v =  [h.Vector().record(myNet.myNetTEA.LCs[i].soma(0.5)._ref_v) for i in range(0,subTrials)]
        myNet.TEAPlot.fig.suptitle('%s - Network %d at %d Hz' %("TEA",myNet.myNetTEA.netNo, myNet.myNetTEA.FreqNo))
        myNet.updateTEA()
        
        
    def bindings(self):
        self.enterbox.bind('<Return>',self.display)








class MySpinBox():
    def __init__(self,master,start,end,inc,boxLabel,gridRow,gridCol,gridPady,gridPadx,controlorTEA):
        #constants
        self.boxLabel = boxLabel
        self.gridRow = gridRow
        self.gridPady = gridPady
        self.gridPadx = gridPadx
        self.controlorTEA = controlorTEA
        #widgets
        self.spinbox = tk.Spinbox(master, from_ = start, to = end,
                                  increment = inc, 
                                  command = lambda: self.display(self.display))
        self.spinbox.grid(row=gridRow,column=gridCol,pady=gridPady,padx=gridPadx)

        self.label = tk.Label(master, text = self.boxLabel )
        self.label.configure(text = self.boxLabel )

        if self.controlorTEA == "myNetControl":  
            
            self.label.grid(row=self.gridRow,column=boxColControlLabel,pady=self.gridPady,padx=self.gridPadx,sticky = 'W')
        else:
            self.label.grid(row=self.gridRow,column=teaBoxLabelCol,pady=self.gridPady,padx=self.gridPadx,sticky = 'W')
        
        
        self.cellNo = 0
        self.bindings()
        


    def display(self,*args):
        self.value = self.spinbox.get()
        
        exec("%s = %f" %("myNet." + self.controlorTEA+ ".LCs["+str(self.cellNo)+"]." + self.boxLabel,float(self.value)))
        
        if self.controlorTEA == "myNetControl":
            
            
            myNet.myNetControl.setSyns()
            myNet.myNetControl.NetCons = myNet.myNetControl.createNetCons()
            myNet.myNetControl.g,myNet.myNetControl.gSIZ = myNet.myNetControl.createSomaGaps(),myNet.myNetControl.createSIZGaps()
            myNet.update()

        else:
            
            myNet.myNetTEA.setSyns()
            myNet.myNetTEA.NetCons = myNet.myNetTEA.createNetCons()
            myNet.myNetTEA.g,myNet.myNetTEA.gSIZ = myNet.myNetTEA.createSomaGaps(),myNet.myNetTEA.createSIZGaps()
            myNet.updateTEA()


    def bindings(self):
        self.spinbox.bind('<Return>', self.display)
        

class sliderVars:
    def __init__(self,start,end,step,initval):
        self.start = start
        self.end = end
        self.step = step
        self.initval = initval


class NetPlot:
    def __init__(self,netNo,freqNo):
    #start by running the control and tea sims and setting plotting constants
        
        self.myNetControl = Network(netNo,freqNo,"Control")
        self.myNetTEA = Network(netNo,freqNo,"TEA")
        self.plotArrayC = self.myNetControl.run()
        self.plotArrayTEA = self.myNetTEA.run()
        #plotting constants
        self.simTime = getSimTime(self.plotArrayC,dt)



        #create plot objects and run
        self.controlPlot = self.plotClass("Control",self.simTime,self.plotArrayC,'%s - Network %d at %d Hz' %("Control",self.myNetControl.netNo, self.myNetControl.FreqNo))
        

        #create Slider objects
        self.synGainSlider = self.mySliders(0.00001,2,0.005,self.myNetControl.synGain,0.09,"synGain")#start, end, step,initval, yaxis location, labelname
        self.somaRSlider = self.mySliders(1.25,2.5,0.01,self.myNetControl.RSOMA,0.05,"somaR")
        self.sizRSlider = self.mySliders(0.1,300,1,self.myNetControl.RSIZ,0.01,"sizR")

        #set a callback for the slider params to rerun and print on change
        self.synGainSlider.mySlider.on_changed( self.updateSyn)
        self.somaRSlider.mySlider.on_changed(self.updatesomaR)
        self.sizRSlider.mySlider.on_changed(self.updatesizR)
        
        self.TEAPlot = self.plotClass("TEA",self.simTime,self.plotArrayTEA,'%s - Network %d at %d Hz' %("TEA",self.myNetControl.netNo, self.myNetControl.FreqNo))
                #create Slider objects
        self.synGainSliderTEA = self.mySliders(0.00001,2,0.005,self.myNetTEA.synGain,0.09,"synGain")#start, end, step,initval, yaxis location, labelname
        self.somaRSliderTEA = self.mySliders(1.25,2.5,0.01,self.myNetTEA.RSOMA,0.05,"somaR")
        self.sizRSliderTEA = self.mySliders(0.1,300,1,self.myNetTEA.RSIZ,0.01,"sizR")

        #set a callback for the slider params to rerun and print on change
        self.synGainSliderTEA.mySlider.on_changed( self.updateSynTEA)
        self.somaRSliderTEA.mySlider.on_changed(self.updatesomaRTEA)
        self.sizRSliderTEA.mySlider.on_changed(self.updatesizRTEA)
        


    class mySliders:
        def __init__(self,start,end,step,initval,yaxis,mylabel):
            self.slidernums = sliderVars(start,end,step,initval)
            self.ax_slide1 = plt.axes([0.15, yaxis, 0.65, 0.03])
            self.mySlider = Slider(self.ax_slide1,label = mylabel,valmin = self.slidernums.start,valmax = self.slidernums.end,valinit=self.slidernums.initval,valstep=self.slidernums.step,orientation='horizontal')

    

    def updateSynTEA(self,value):
        self.myNetTEA.synGain = value
        self.myNetTEA.NetCons = self.myNetTEA.createNetCons()
        self.updateTEA()
    def updatesomaRTEA(self,value):
        self.myNetTEA.RSOMA = value
        self.myNetTEA.g = self.myNetTEA.createSomaGaps()
        self.updateTEA()

    def updatesizRTEA(self,value):
        self.myNetTEA.RSIZ = value
        self.myNetTEA.gSIZ = self.myNetTEA.createSIZGaps()
        self.updateTEA()

    def updateSyn(self,value):
        self.myNetControl.synGain = value
        self.myNetControl.NetCons = self.myNetControl.createNetCons()
        self.update()

    def updatesomaR(self,value):
        self.myNetControl.RSOMA = value
        self.myNetControl.g = self.myNetControl.createSomaGaps()
        self.update()

    def updatesizR(self,value):
        self.myNetControl.RSIZ = value
        self.myNetControl.gSIZ = self.myNetControl.createSIZGaps()
        self.update()

    def updateTEA(self):
        #rerun simulation
        self.plotArray = self.myNetTEA.run()
        [self.TEAPlot.axList[i][0].set_ydata(self.plotArray[:,i]) for i in range(0,(self.plotArray.shape)[1])]
        [self.TEAPlot.axs[i].set_ylim((-60,np.max(self.plotArray))) for i in range(0,len(self.controlPlot.axList))]
        self.TEAPlot.fig.canvas.draw()

    def update(self): 
        #rerun simulation
        self.plotArray = self.myNetControl.run()
        [self.controlPlot.axList[i][0].set_ydata(self.plotArray[:,i]) for i in range(0,(self.plotArray.shape)[1])]
        [self.controlPlot.axs[i].set_ylim((-60,np.max(self.plotArray))) for i in range(0,len(self.controlPlot.axList))]
        self.controlPlot.fig.canvas.draw()

    class plotClass:
        def __init__(self,controlorTEA,simTime,plotArrayl,title):#pass it the type of run and the data to plot
            self.title = title
            self.fig,self.axs = plt.subplots(1,5,figsize=(10,4.5))
            self.fig.subplots_adjust(left=0.1, bottom=0.2, right=.9, top=.92, wspace=0, hspace=0)
            self.axList = [self.axs[i].plot(simTime, plotArrayl[:,i]) for i in range(0,(plotArrayl.shape)[1])]
            
            [self.axs[i].set_ylim((-60,np.max(plotArrayl))) for i in range(0,len(self.axList))]
            self.fig.suptitle(self.title)

            self.canvas = FigureCanvasTkAgg(self.fig,
                               master = root)
            if(controlorTEA == "Control"):
                self.canvas.get_tk_widget().grid(column = plotCols, row=0,rowspan=19,padx = 70,pady=20)
            elif(controlorTEA== "TEA"):
                self.canvas.get_tk_widget().grid(column = plotCols, row=19,rowspan=19,padx = 70,pady=20)




def callback():
    if tk.messagebox.askokcancel("Quit", "Do you really wish to quit?"):
        root.destroy()
        root.quit()
    
############ currently using SIZ to plot. uncomment the line in display and comment out the one using SIZ. This is the only change needed to plot soma instead of SIZ
netNo,FreqNo = 1,16 #network number ?, tested at ? Hz
root = tk.Tk()
myNet = NetPlot(netNo,FreqNo)
window = Window(root)

root.protocol("WM_DELETE_WINDOW", callback)
root.mainloop()












 
