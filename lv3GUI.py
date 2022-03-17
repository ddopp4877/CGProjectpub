#LV3GUI
import platform
from tkinter.constants import BOTH, W
from tkinter.font import BOLD
from neuron import h
import numpy as np
import os
from pandas.core.algorithms import value_counts
from modules.makeParams import *
from modules.RejectionProtocols import *
import sys
import pandas as pd
from itertools import combinations
from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg, 
NavigationToolbar2Tk)

if platform.system() == 'Linux':
    h.nrn_load_dll(os.path.join("modFiles/x86_64/.libs/libnrnmech.so"))
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



eventTimes = np.array(pd.read_pickle(os.path.join("input","LV3",eventtimesfilename + ".pkl")))
LV2PassParams =  np.array(pd.read_pickle(os.path.join("input","LV3", passparamsfilename+ ".pkl")))

Trials = (LV2PassParams.shape)[1]
subTrials = 5
eventTimes = eventTimes[:,:Trials]
LV2PassParams = LV2PassParams[:,:Trials]
ETs = [h.Vector(eventTimes[i,eventTimes[i,:] !=0]) for i in range(0,Trials)]

h.load_file('stdrun.hoc') #so you can use run command

comboList = list(combinations([0,1,2,3,4],2))

class Network:
    def __init__(self, startNo,controlorTEA):
        netParams = LV2PassParams[:,startNo:startNo+5]
        params, LCs = makeCellsLV3(netParams,controlorTEA)
        self.params = params
        self.LCs = LCs

        self.vsAll = [h.VecStim() for i in range(0,subTrials)]
        
        self.syns = self.createSyns()
        self.setSyns()

        self.synGain = 0.13
        self.RSOMA= 1.54
        self.RSIZ =200

        self.setEventTimes()
        self.NetCons = self.createNetCons()
    # connect all the sizs in a network, and LC1 and LC2 Somas and LC4 and LC5

        self.g,self.gSIZ = self.createSomaGaps(),self.createSIZGaps()


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
        [self.vsAll[i].play(ETs[i]) for i in range(0,subTrials)]
        
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
        self.v =  [h.Vector().record(self.LCs[i].soma(0.5)._ref_v) for i in range(0,subTrials)] 
        h.dt = 0.2
        h.finitialize(-51)
        h.continuerun(tstop)
        return np.array(self.v).T[:int(stopTime/dt),:]




import matplotlib.pyplot as plt
from matplotlib.widgets import *
import tkinter as tk

class Window:
    def __init__(self,master):
        self.frame = tk.Frame(master,width = 200,height=10)

        #self.spinbox1 = MySpinBox(master,6.2e-5,97e-5,10e-5,"g_leak",0,1,10,5)
        #self.spinbox2 = MySpinBox(master,1,10,1,"g_nap2",1,1,10,5)
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
        val1 = myNet.myNetControl.LCs[0].siz.g_leak
        val2 = myNet.myNetControl.LCs[0].siz.g_nasiz
        val3 = myNet.myNetControl.LCs[0].siz.g_kdsiz
        ar1 = np.repeat(np.array([val1,val2,val3]),5).reshape(3,5)
        ar2 = np.ones((3,5))
        self.nextParams = np.multiply(ar1,ar2)
        self.usedParams = np.vstack((self.usedParams,self.nextParams))
        for i in range(0,len(self.myVars)):

            self.allSpins.append(MySpinBox(master,self.usedParams[i,0],self.myVarsValues[i][1],1e-5,self.myVars[i],i+1,boxColControl,0,2,"myNetControl"))
            #self.controlVarText = tk.StringVar()
            self.controlVarText = '({0:>.5f}   -   {1:>.5f})'.format(self.myVarsValues[i][0],self.myVarsValues[i][1])
           
            self.rangeLabel = tk.Label(master, text = self.controlVarText).grid(row = i+1,column = boxColControlLabelRange,padx=5)

           
            self.allSpinsTEA.append(MySpinBox(master,self.usedParams[i,0],self.myVarsValuesTEA[i][1],1e-5,self.myVarsTEA[i],i+1,teaBoxCol,0,10,"myNetTEA"))
            self.teaVarText = '({0:>.5f}   -   {1:>.5f})'.format(self.myVarsValuesTEA[i][0],self.myVarsValuesTEA[i][1])
            self.rangeLabel = tk.Label(master, text = self.teaVarText).grid(row = i+1,column = teaBoxLabelRangeCol,padx = 5)
            



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
        #self.label.configure(text = self.boxLabel )
        
        
        self.bindings()

    def display(self,event):
        self.value = self.spinbox.get()
        
        for i in range(5):
            exec("%s = %f" %("myNet." + self.controlorTEA+ ".LCs["+str(i)+"]." + self.boxLabel,float(self.value)))
        
        if self.controlorTEA == "myNetControl":
            
            
            myNet.myNetControl.setSyns()
            myNet.myNetControl.NetCons = myNet.myNetControl.createNetCons()
            myNet.myNetControl.g,myNet.myNetControl.gSIZ = myNet.myNetControl.createSomaGaps(),myNet.myNetControl.createSIZGaps()
            #myNet.myNetControl.run()
            self.plotArray = myNet.myNetControl.run()
            [myNet.controlPlot.axList[i][0].set_ydata(self.plotArray[:,i]) for i in range(0,(self.plotArray.shape)[1])]
            myNet.controlPlot.fig.canvas.draw()
        else:
            
            myNet.myNetTEA.setSyns()
            myNet.myNetTEA.NetCons = myNet.myNetTEA.createNetCons()
            myNet.myNetTEA.g,myNet.myNetTEA.gSIZ = myNet.myNetTEA.createSomaGaps(),myNet.myNetTEA.createSIZGaps()
            #myNet.myNetTEA.run()
            self.plotArray = myNet.myNetTEA.run()
            [myNet.TEAPlot.axList[i][0].set_ydata(self.plotArray[:,i]) for i in range(0,(self.plotArray.shape)[1])]
            myNet.TEAPlot.fig.canvas.draw()

    def bindings(self):
        self.spinbox.bind('<Return>', self.display)


class sliderVars:
    def __init__(self,start,end,step):
        self.start = start
        self.end = end
        self.step = step


class NetPlot:
    def __init__(self,startNo):
    #start by running the control and tea sims and setting plotting constants
        self.startNo = startNo
        self.myNetControl = Network(startNo,"Control")
        self.myNetTEA = Network(startNo,"TEA")
        self.plotArrayC = self.myNetControl.run()
        self.plotArrayTEA = self.myNetTEA.run()
        #plotting constants
        self.simTime = getSimTime(self.plotArrayC,dt)



        #create plot objects and run
        self.controlPlot = self.plotClass("Control",self.simTime,self.plotArrayC)
        

        #create Slider objects
        self.synGainSlider = self.mySliders(0.05,2,0.01,0.09,"synGain")#start, end, step, yaxis location, labelname
        self.somaRSlider = self.mySliders(1.56,6,0.01,0.05,"somaR")
        self.sizRSlider = self.mySliders(1,300,1,0.01,"sizR")

        #set a callback for the slider params to rerun and print on change
        self.synGainSlider.mySlider.on_changed( self.updateSyn)
        self.somaRSlider.mySlider.on_changed(self.updatesomaR)
        self.sizRSlider.mySlider.on_changed(self.updatesizR)
        
        self.TEAPlot = self.plotClass("TEA",self.simTime,self.plotArrayTEA)
                #create Slider objects
        self.synGainSliderTEA = self.mySliders(0.05,2,0.01,0.09,"synGain")#start, end, step, yaxis location, labelname
        self.somaRSliderTEA = self.mySliders(1.56,6,0.01,0.05,"somaR")
        self.sizRSliderTEA = self.mySliders(1,300,1,0.01,"sizR")

        #set a callback for the slider params to rerun and print on change
        self.synGainSliderTEA.mySlider.on_changed( self.updateSynTEA)
        self.somaRSliderTEA.mySlider.on_changed(self.updatesomaRTEA)
        self.sizRSliderTEA.mySlider.on_changed(self.updatesizRTEA)
        


    class mySliders:
        def __init__(self,start,end,step,yaxis,mylabel):
            self.slidernums = sliderVars(start,end,step)
            self.ax_slide1 = plt.axes([0.15, yaxis, 0.65, 0.03])
            self.mySlider = Slider(self.ax_slide1,label = mylabel,valmin = self.slidernums.start,valmax = self.slidernums.end,valinit=self.slidernums.start,valstep=self.slidernums.step,orientation='horizontal')

    

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
        self.TEAPlot.fig.canvas.draw()

    def update(self): 
        #rerun simulation
        self.plotArray = self.myNetControl.run()
        [self.controlPlot.axList[i][0].set_ydata(self.plotArray[:,i]) for i in range(0,(self.plotArray.shape)[1])]
        self.controlPlot.fig.canvas.draw()

    class plotClass:
        def __init__(self,controlorTEA,simTime,plotArrayl):#pass it the type of run and the data to plot
            self.fig,self.axs = plt.subplots(1,5,figsize=(8,4.5))
            self.fig.subplots_adjust(left=0.1, bottom=0.2, right=.9, top=.92, wspace=0, hspace=0)
            self.axList = [self.axs[i].plot(simTime, plotArrayl[:,i]) for i in range(0,(plotArrayl.shape)[1])]
            [self.axs[i].set_ylim((-60,10)) for i in range(0,len(self.axList))]
            self.fig.suptitle('%s - Network %d at %d Hz' %(controlorTEA,netNo, FreqNo))
            #self.root = tk.Tk()
            #self.window = Window(self.root)
            self.canvas = FigureCanvasTkAgg(self.fig,
                               master = root)
            if(controlorTEA == "Control"):
                self.canvas.get_tk_widget().grid(column = plotCols, row=0,rowspan=19,padx = 180,pady=10)
            elif(controlorTEA== "TEA"):
                self.canvas.get_tk_widget().grid(column = plotCols, row=19,rowspan=19,padx = 180,pady=10)

netNo,FreqNo = 2,22 #network number ?, tested at ? Hz
startNo = getNetIDX(netNo,FreqNo)


root = tk.Tk()
myNet = NetPlot(startNo)
window = Window(root)


root.mainloop()





 
