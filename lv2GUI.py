#LV2GUI
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


eventTimes = np.array(pd.read_pickle(os.path.join("input","LV2",eventtimesfilename + ".pkl")))
LV1PassParams =  np.array(pd.read_pickle(os.path.join("input","LV2", passparamsfilename+ ".pkl")))
Trials = (LV1PassParams.shape)[1]

h.load_file('stdrun.hoc') #so you can use run command

#make a list of LargeCells then change their parameters to be random
#read in event times, turn it into a vector, remove the zeros, play the vector as the source of the vectstim, which is the source of the netcon to the exp2syn target

#recording variables:
ETs = [h.Vector(eventTimes[i,eventTimes[i,:] !=0]) for i in range(0,Trials)]
    
class LV2Cell:
    def __init__(self, startNo,controlorTEA):
        
        params, LCs = makeRandomCellsLV2(100,222,LV1PassParams,controlorTEA)
        self.params = params[:,startNo]
        self.LCs = LCs[startNo]

        self.vsAll = h.VecStim()
        
        self.syns = self.createSyns()
        self.setSyns()

        self.synGain = 0.29
        self.startNo = startNo

        self.setEventTimes()
        self.NetCons = self.createNetCons()
        
    def createNetCons(self):
        return h.NetCon(self.vsAll,self.syns,-10,0,self.synGain)

    def setEventTimes(self):
        [self.vsAll.play(ETs[self.startNo])]
        
    def setSyns(self):
        self.syns.tau1,self.syns.tau2,self.syns.e   = 10,120,-15

    def createSyns(self):
        return h.Exp2Syn(self.LCs.siz(1))
        
    def run(self):
        stopTime = 2550
        self.v =  h.Vector().record(self.LCs.soma(0.5)._ref_v)
        h.dt = 0.2
        h.finitialize(-51)
        h.continuerun(tstop)
        return np.array(self.v).T[:int(stopTime/dt)]


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
        
        self.usedParams = myCell.LV2CellControl.params.reshape(-1,1)
        
        ar1 = np.array([myCell.LV2CellControl.LCs.siz.g_leak,myCell.LV2CellControl.LCs.siz.g_nasiz,myCell.LV2CellControl.LCs.siz.g_kdsiz]).reshape(-1,1)
        self.usedParams = np.vstack((self.usedParams,ar1))


        for i in range(0,len(self.myVars)):
            print(self.usedParams[i,0])
            print(self.myVarsValues[i][1])
            self.allSpins.append(MySpinBox(master,self.usedParams[i,0],self.myVarsValues[i][1],1e-5,self.myVars[i],i+1,boxColControl,0,2,"LV2CellControl"))
 
            self.controlVarText = '({0:>.5f}   -   {1:>.5f})'.format(self.myVarsValues[i][0],self.myVarsValues[i][1])
           
            self.rangeLabel = tk.Label(master, text = self.controlVarText).grid(row = i+1,column = boxColControlLabelRange,padx=5)

           
            self.allSpinsTEA.append(MySpinBox(master,self.usedParams[i,0],self.myVarsValuesTEA[i][1],1e-5,self.myVarsTEA[i],i+1,teaBoxCol,0,10,"LV2CellTEA"))
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

        if self.controlorTEA == "LV2CellControl":  
            
            self.label.grid(row=self.gridRow,column=boxColControlLabel,pady=self.gridPady,padx=self.gridPadx,sticky = 'W')
        else:
            self.label.grid(row=self.gridRow,column=teaBoxLabelCol,pady=self.gridPady,padx=self.gridPadx,sticky = 'W')

        self.bindings()

    def display(self,event):
        self.value = self.spinbox.get()
        
        exec("%s = %f" %("myCell." + self.controlorTEA+ ".LCs." + self.boxLabel,float(self.value)))
        
        if self.controlorTEA == "LV2CellControl":
            
            
            myCell.LV2CellControl.setSyns()
            myCell.LV2CellControl.NetCons = myCell.LV2CellControl.createNetCons()
            
            self.plotArray = myCell.LV2CellControl.run()
            myCell.controlPlot.axPlot[0].set_ydata(self.plotArray)
            myCell.controlPlot.fig.canvas.draw()
        else:
            
            myCell.LV2CellTEA.setSyns()
            myCell.LV2CellTEA.NetCons = myCell.LV2CellTEA.createNetCons()
            
            self.plotArray = myCell.LV2CellTEA.run()
            myCell.TEAPlot.axPlot[0].set_ydata(self.plotArray)
            myCell.TEAPlot.fig.canvas.draw()

    def bindings(self):
        self.spinbox.bind('<Return>', self.display)


class sliderVars:
    def __init__(self,start,end,step):
        self.start = start
        self.end = end
        self.step = step


class CellPlot:
    def __init__(self,startNo):
    #start by running the control and tea sims and setting plotting constants
        self.startNo = startNo
        self.LV2CellControl = LV2Cell(startNo,"Control")
        self.LV2CellTEA = LV2Cell(startNo,"TEA")
        self.plotArrayC = self.LV2CellControl.run()
        self.plotArrayTEA = self.LV2CellTEA.run()
        #plotting constants
        self.simTime = getSimTime(self.plotArrayC,dt)
        
        #create plot objects and run
        self.controlPlot = self.plotClass("Control",self.simTime,self.plotArrayC)
        

        #create Slider objects
        self.synGainSlider = self.mySliders(0.16,2,0.01,0.09,"synGain")#start, end, step, yaxis location, labelname
        
        #set a callback for the slider params to rerun and print on change
        self.synGainSlider.mySlider.on_changed( self.updateSyn)
        
        self.TEAPlot = self.plotClass("TEA",self.simTime,self.plotArrayTEA)
                #create Slider objects
        self.synGainSliderTEA = self.mySliders(0.16,2,0.01,0.09,"synGain")#start, end, step, yaxis location, labelname

        #set a callback for the slider params to rerun and print on change
        self.synGainSliderTEA.mySlider.on_changed( self.updateSynTEA)

    class mySliders:
        def __init__(self,start,end,step,yaxis,mylabel):
            self.slidernums = sliderVars(start,end,step)
            self.ax_slide1 = plt.axes([0.15, yaxis, 0.65, 0.03])
            self.mySlider = Slider(self.ax_slide1,label = mylabel,valmin = self.slidernums.start,valmax = self.slidernums.end,valinit=self.slidernums.start,valstep=self.slidernums.step,orientation='horizontal')
            
    

    def updateSynTEA(self,value):
        self.LV2CellTEA.synGain = value
        self.LV2CellTEA.NetCons = self.LV2CellTEA.createNetCons()
        self.updateTEA()

    def updateSyn(self,value):
        
        self.LV2CellControl.synGain = value
        self.LV2CellControl.NetCons = self.LV2CellControl.createNetCons()
        self.update()

    def updateTEA(self):
        #rerun simulation
        self.plotArray = self.LV2CellTEA.run()
        self.TEAPlot.axPlot[0].set_ydata(self.plotArray)
        self.TEAPlot.fig.canvas.draw()

    def update(self): 
        #rerun simulation
        self.plotArray = self.LV2CellControl.run()
        self.controlPlot.axPlot[0].set_ydata(self.plotArray)
        self.controlPlot.fig.canvas.draw()

    class plotClass:
        def __init__(self,controlorTEA,simTime,plotArrayl):#pass it the type of run and the data to plot
            self.fig,self.axs = plt.subplots(figsize=(5,4.5))
            #self.fig = plt.Figure(figsize=(8,4.5))
            self.fig.subplots_adjust(left=0.1, bottom=0.2, right=.9, top=.92, wspace=0, hspace=0)
            #self.axs = self.fig.add_subplot(111)
            self.axs.set_ylim([-60,-10])
            self.axPlot = self.axs.plot(simTime,plotArrayl)
            self.canvas = FigureCanvasTkAgg(self.fig,
                               master = root)
            self.fig.suptitle('%s - Cell %d at %d Hz' %(controlorTEA,cellNo, FreqNo))
            if(controlorTEA == "Control"):
                self.canvas.get_tk_widget().grid(column = plotCols, row=0,rowspan=19,padx = 180,pady=15)
            elif(controlorTEA== "TEA"):
                self.canvas.get_tk_widget().grid(column = plotCols, row=19,rowspan=19,padx = 180,pady=15)

cellNo,FreqNo = 19,24
#network number ?, tested at ? Hz
startNo = getLV2CellIDX(cellNo,FreqNo)


root = tk.Tk()
myCell = CellPlot(startNo)
window = Window(root)


root.mainloop()





 
