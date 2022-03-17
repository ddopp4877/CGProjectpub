# a set of functions for analyzing the results:
import matplotlib.pyplot as plt
import numpy as np
from neuron import h
import seaborn as sns

from modules.makeParams import getNetIDX; sns.set_theme()
from matplotlib.backends.backend_pdf import PdfPages



def plotFailCrit(rejectionResults,critList):# plot a bar graph of all the rejection criteria, and blue for fail below, green for pass, red for fail above

    def findPassNum(array):
        A = np.where(array == 0)
        A = len(A[0][:])
        B = np.where(array == 1)
        B = len(B[0][:])
        C = np.where(array == 2)
        C = len(C[0][:])
        return A,B,C

    allLow,allPass,allHigh = [],[],[]
    for i in range(0,len(critList)):
        failLow,Pass,failHigh = findPassNum(rejectionResults[i,:])
        allLow.append(failLow)
        allPass.append(Pass)
        allHigh.append(failHigh)

    barWidth = 0.2
    plot1 = plt.figure(figsize=(10,5))
    xcoor = np.arange(0,len(allLow))
    plt.bar(xcoor-barWidth,allLow,barWidth, color='blue',label = 'low')
    plt.bar(xcoor,allPass,barWidth,color='green',label='pass')
    plt.bar(xcoor+barWidth,allHigh,barWidth,color = 'red',label='high')
    plt.xticks(xcoor,critList,fontsize=8)
    plt.legend()
    return plot1

def plotNet(array,NetNo,SCfreq):
    
    fig,axs = plt.subplots(1,5,figsize=(7,4))
    startNo=getNetIDX(NetNo,SCfreq)
    [axs[i].plot(array[:,i]) for i in range(0,len(axs))]
    
    return fig

def plotCorrelogram(params,paramsList):
    plot = plt.figure(figsize=(10,5))
    Corrs = np.corrcoef(params)
    ax = sns.heatmap(Corrs,xticklabels=paramsList,yticklabels = paramsList,vmin=-0.5,vmax=0.5,annot=True)
    plt.title('LV3 Passing Net Correlogram',fontsize=15)
    return plot


def printNetVoltages(Voltages, RejectionResults,outfile):
    
    [a,b] = RejectionResults.shape
    netPass = np.array([1 if(np.all(RejectionResults[a-1,i:i+5]==1)) else 0 for i in range(0,b,5)])# mark 1 if all cells in a net passed
    netPass = np.repeat(netPass,5)
    SCfreq = np.arange(16,32)
    with PdfPages(outfile+'.pdf') as pdf:
            j=0
            rows,columns = 5,4# 25 networks on one page, one column is one network

            ## make a list of all the indices in the matrix such that the 2nd plotted element is the 5th cell, and so on
            a = []
            for j in range(0,b,int((rows*columns))):#for all the rows for the number of pages, skipping the number of cells in a page,
                for i in range(0,rows):#for one row
                    a.append(np.arange(i+j,i+j+int((rows*columns)),rows))#the array's indices will be the cell number + column number to that number + 1 pg, for how ever many are on a page
            a = np.concatenate(a,axis=0)

            for j in range(0,b,rows*columns):#for all the plots, skipping the number in a page,
                plt.figure(figsize=(15,10))
                for i in range(1,rows*columns+1):#for the subplot number, go from 1 to the number of cells in a page+1
                    arrayIdx = j+i-1
                    plt.subplot(rows,columns,i)#plot the next cell, right to left up to down
                    plt.plot(Voltages[:,a[arrayIdx]])# using the array's next index, which is the cell number of the first cell on the page+ the number in the page
                    if((a[arrayIdx]%5) == 0):
                        if netPass[int(a[arrayIdx])] == 1:
                            string = "**"
                        else:
                            string = ""
                        plt.title('SCfreq =  %d%s' %(SCfreq[arrayIdx%16], string))
                    plt.suptitle("Network %d" %(arrayIdx/80 +1),fontsize=14)

                pdf.savefig()
                plt.close()

def printLV2Voltages(Voltages, RejectionResults,outfile):
    [a,b] = RejectionResults.shape
    passNos = [1 if np.all(RejectionResults[:,i]==1) else 0 for i in range((RejectionResults.shape[1]))]
    SCfreq = np.arange(16,32)
    with PdfPages(outfile + '.pdf') as pdf:
            
            rows,columns = 4,4# 16 cells on one page,r-l t-b increasing in frequency

            for j in range(0,b,rows*columns):#for all the plots, skipping the number in a page,
                plt.figure(figsize=(15,10))
                for i in range(1,rows*columns+1):#for the subplot number, go from 1 to the number of cells in a page+1
                
                    plt.subplot(rows,columns,i)#plot the next cell, left to right up to down
                    plt.subplots_adjust(left=0.2, bottom=0.2, right=.9, top=.92, wspace=0.2, hspace=0.4)
                    plt.plot(Voltages[:,(j+i -1)])# using the array's next index, which is the cell number of the first cell on the page+ the number in the page
                    if passNos[j+i-1] == 1:
                            string = "**"
                    else:
                        string = ""
                    plt.title('SCfreq =  %d%s' %(SCfreq[(i-1)], string))
                plt.suptitle("Cell %d" %((j+i-1)/16),fontsize=14)

                pdf.savefig()
                plt.close() 
 
                
def plotDistributions(Params,paramsList):
    xlen,ylen = 4,4
    fig,axs = plt.subplots(xlen,ylen,figsize=(15,10))
    plt.subplots_adjust(left=0.2, bottom=0.2, right=.9, top=.92, wspace=0.2, hspace=0.4)
    for i in range(xlen):
        for j in range(xlen):
            if (i%xlen*xlen+j) == len(paramsList):
                break
            axs[i][j].hist(Params[:,i%xlen*xlen+j],bins = 50)
            axs[i][j].set(title = paramsList[i%xlen*xlen+j])

def plotCellStruct(LC):# untested
    Cell = LC
    h.topology()
    s = h.Shape()
"""
def mergeOnevoltageOutput(array,cellNo):
    firstIndex = cellNo*16
    lastIndex = cellNo*16 + 16
    plotArray = np.empty((0,0))
    for i in range(0,16):
        plotArray = np.append(plotArray,array[:,firstIndex+i])

    return plotArray





"""