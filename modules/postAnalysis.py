# a set of functions for analyzing the results:

import numpy as np
from neuron import h
import seaborn as sns
import math
from modules.makeParams import getNetIDX; sns.set_theme()
from matplotlib.ticker import FuncFormatter

import matplotlib
matplotlib.use('Agg')# get some memory error with printing to pdf if this backend isn't used.
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
from scipy import stats
import pandas as pd
from modules.makeParams import *

def plotRaster(mappedIdxs,freqsnonavgpassing,filename = 'cellIdsfreq.png'):#get the arguments from getRasterData()
    
    allIds = []
    for i in range(0,len(mappedIdxs),5):
        if np.all(mappedIdxs[i:i+10] == mappedIdxs[i]):
            allIds.append(mappedIdxs[i])
    allIds = np.array(allIds)
    allIds = np.unique(allIds)


    fig, ax = plt.subplots(figsize=(30,6))
    plt.scatter(mappedIdxs,freqsnonavgpassing,s=2)


    ax.set_yticks(np.arange(16,32))
    ax.set_xticks(allIds,allIds,fontsize = 8,rotation = 90)


    axin1 = ax.inset_axes([0.2,-0.012,0,0.95])
    axin1.set_yticks(np.arange(2,34,2),np.arange(16,32),fontsize =9)
    axin1.set_xticks([])


    axin2 = ax.inset_axes([0.4,-0.012,0,0.95])
    axin2.set_yticks(np.arange(2,34,2),np.arange(16,32),fontsize =9)
    axin2.set_xticks([])

    axin3 = ax.inset_axes([0.6,-0.012,0,0.95])
    axin3.set_yticks(np.arange(2,34,2),np.arange(16,32),fontsize =9)
    axin3.set_xticks([])


    axin4 = ax.inset_axes([0.8,-0.012,0,0.95])
    axin4.set_yticks(np.arange(2,34,2),np.arange(16,32),fontsize =9)
    axin4.set_xticks([])

    plt.vlines(allIds,ymax=31,ymin=16,color = 'red',linewidths=0.3)
    plt.xlabel('network ID')
    plt.ylabel('SCfreq')
    plt.savefig(filename,dpi = 500)
    print('file saved as %s' %(filename))
    plt.show()


def getRasterData(coded,SCfreqs):#coded is rejection results coded, second argument is SCfreqs file in the same output folder
    #get the passing network indices
    passnetIDs,passnetIDXs = getpassingNetIDXs(coded)#
    #get the frequencies at the passing network indicies
    [a,b] = coded.shape
    netPassI = np.array([1 if(np.all(coded[:,i:i+5]==1)) else 0 for i in range(0,b,5)])# mark 1 if all cells in a net passed#11200
    netPassIdxs = np.repeat(netPassI,5)#repeat so that each index is the same for all the cells in a single network, back to 56000
    netPassIdxs = np.where(netPassIdxs==1)[0]#find where in the array of 56000 passing+nonpassing where the whole network passes
    freqsnonavgpassing = SCfreqs[netPassIdxs]
    
    #change them to be indexed for the raster
    mappedIDxs,IDxs = mapping(passnetIDXs)
    return mappedIDxs,freqsnonavgpassing



def getpassingNetIDXs(coded):#each network id is created for each cell in that network that passed when in a network
    [a,b] = coded.shape
    cellids = np.arange(0,int((coded.shape)[1]/5/16))
    netids = np.repeat(cellids,5*16)
    codedwNets = np.vstack((coded,netids))
    passnetIDs =np.array([int(codedwNets[a,i]) if (np.all(coded[:,i:i+5]==1)) else -1 for i in range(0,b,5)])#list of cellids of passing nets, 5x less than all traces 56000
    passnetIDs = np.repeat(passnetIDs,5)
    passnetIDXs = passnetIDs[np.where(passnetIDs == 1)[0]]#just the passing idxs from the original 56000
    return passnetIDs,passnetIDXs


def mapping(passnetIDXs):#run getpassingNetIDXs to get passnetIDXs
    mappedIdxs = np.array([0])
    IDxs = []
    j=0
    for i in range(len(passnetIDXs)-1):
        
        if passnetIDXs[i] == passnetIDXs[i+1]:
            mappedIdxs = np.append(mappedIdxs,j)
            IDxs.append(passnetIDXs[i])
        else:
            mappedIdxs = np.append(mappedIdxs,j)
            IDxs.append(passnetIDXs[i])
            j+=1
    mappedIdxs = mappedIdxs[1:]#remove the first item since it is just to get the array started
    mappedIdxs = np.append(mappedIdxs,mappedIdxs[len(mappedIdxs)-1])#repeat the last item since we have to stop early
    IDxs.append(IDxs[len(IDxs)-1])
    return mappedIdxs,IDxs

#go from the passidxs, to the index when we have all cells including nonpassing:
#passnetIDXs are the indices of the passnetIDs that passed, but /80, so
def unMap(mappedIdxs,IDxs,passnetIDs,netID):#mapped is the result of mapping(), IDxs is the second list returned from mapping(), netID is the network of interest in the raster
    passnetIndex = np.where(mappedIdxs == netID)[0]#index of this network in the mapped indices
    passnetIndex = np.array(IDxs)[passnetIndex][0] #network id of the network which passed in the passnetIDs list
    netID = passnetIndex *80# the index of the cell in the passnetIDs
    neworkID = passnetIDs[netID] #the id of the network in the original data. for example, network 2 in the mapped data is network 6 in the original
    return neworkID


def getPassIdxs(codedArray,lvl):
    if lvl == 'LV2':
        print('using LV2')
        passIdxs = np.array([1 if(np.all(codedArray[:,i]==1)) else 0 for i in range(0,codedArray.shape[1])])# mark 1 if all cells in a net passed
        passCellIdsLV2 = np.array([1 if (np.any(passIdxs[i:i+16] == 1)) else 0 for i in range(0,len(passIdxs),16)])
        passIdxs = np.where(passIdxs==1)[0]
        failIdxs = np.where(passIdxs!=1)[0]
        codedPassexpanded = np.repeat(passCellIdsLV2,16)# since each set of 16 is actually 1
    
    else:
        print('using LV3')
        [a,b] = codedArray.shape
        netPass = np.array([1 if(np.all(codedArray[:,i:i+5]==1)) else 0 for i in range(0,b,5)])# mark 1 if all cells in a net passed
        singleNetPass = [1 if (np.any(netPass[i:i+16] == 1)) else 0 for i in range(0,len(netPass),16)]
        codedPassexpanded = np.repeat(singleNetPass,5*16)#since each set of 80 is actually just for 1 network
        passIdxs =  np.where(codedPassexpanded ==1)[0]
        failIdxs = np.where(codedPassexpanded !=1)[0]
    return passIdxs,failIdxs,codedPassexpanded

def getEveryFirstNet(array):
    if np.ndim(array) == 1:
        dummyarray = np.ones((len(array)))
        array = np.vstack((array,dummyarray))
        check=1
    else:
        check=0
    [a,b] = array.shape
    allNets = np.ones((a,1))
    for i in range(0,b,5*16):
        allNets = np.hstack((allNets,array[:,i:i+5]))
    if check ==1:
        return allNets[0,1:]
    return allNets[:,1:]

def getpassPairsList(maskedDFCorr):
    passpairs = []
    for param1 in maskedDFCorr:
        for i in range(len(maskedDFCorr[param1])):
            if not np.isnan(maskedDFCorr[param1][i]):
                passpairs.append((param1,(maskedDFCorr.keys())[i]))
    passpairs = list(set(passpairs))
    pairList = []
    for pair in passpairs:
        if pair[0] in fullParamsList():       
            pairList.append(pair)
    return pairList

def getMaskedCorr(passParamsDF,threshold):
    dfCorr = passParamsDF.corr(method='pearson')
    maskedDFCorr = np.array(dfCorr)
    mask = (abs(dfCorr) > threshold) & (abs(dfCorr < 1))
    mask = np.array(mask)
    for i in range(len(mask)):
        for j in range(len(mask)):
            if (i < len(fullParamsList())) & (j <len(fullParamsList())):
                maskedDFCorr[i][j] = np.nan

    maskedDFCorr = pd.DataFrame(maskedDFCorr)
    maskedDFCorr = maskedDFCorr[pd.DataFrame(mask)]
    maskedDFCorr.columns = fullParamsList() + LV3CritList()
    maskedDFCorr.index = fullParamsList() + LV3CritList()
    return maskedDFCorr

def summaryStats(selecteddf,pair):   
    
    AVG = np.mean(selecteddf[pair[0]])
    STD = np.std(selecteddf[pair[0]])
    #Skew = stats.skew(selecteddf)
    #Kurt = stats.kurtosis(selecteddf, fisher=True,bias=False)
    retDict = {'parameter' : pair[0],
               'rCriteria': pair[1],
               'AVG' : AVG,
               'STD': STD,
               #'Skew' : Skew,
               #'Kurt' : Kurt
              }
    return retDict

def tTest(passDf,failDf,pair):
    eqVar = True
    if abs(np.std(passDf[pair[0]]) - np.std(passDf[pair[1]]) ) < abs(np.std(passDf[pair[0]]))/ 2:
        eqVar = False
    return stats.ttest_ind(passDf[pair[0]], failDf[pair[0]], equal_var=eqVar)

def makestatsDict(paramsDF,failCriteriacoded,pairList,passing):
    
    allStats = []
    for i in range(len(pairList)):
        passIDXs = failCriteriacoded[pairList[i][1]] == 1
        lowIDXs = failCriteriacoded[pairList[i][1]] < 1
        highIDXs = failCriteriacoded[pairList[i][1]] > 1

        passdf = pd.DataFrame([paramsDF[key][passIDXs] for key in paramsDF]).T
        lowdf = pd.DataFrame([paramsDF[key][lowIDXs] for key in paramsDF]).T
        highdf = pd.DataFrame([paramsDF[key][highIDXs] for key in paramsDF]).T
        

        if passing == 'passing':
            statsDict = summaryStats(passdf,pairList[i])
            lowtest = tTest(passdf,lowdf,pairList[i])
            hightest = tTest(passdf,highdf,pairList[i])
            statsDict.update({'fail low p-value':lowtest[1]})
            statsDict.update({'fail high p-value': hightest[1]})
        elif passing == 'low':
            statsDict = summaryStats(lowdf,pairList[i])
        elif passing == 'high':
            statsDict = summaryStats(highdf,pairList[i])
        allStats.append(statsDict)
    allStats = pd.DataFrame(allStats)
    return allStats



def plotCellOverFreqs(array,cellNo,NetworkNo = -100):
    
    [a,b] = array.shape
    traces = []
    
    if NetworkNo == -100:
        print("printing LV2")
        title = 'LV2 Cell ' + str(cellNo)
        
        cellNo = cellNo-1
        for i in range(cellNo*16,cellNo*16+16):
            trace = array[:,i]
            traces.append(trace)

    else:
        print("printing LV3")
        title = 'LV3 Network ' + str(NetworkNo) + 'Cell ' + str(cellNo)
        
        NetworkNo = NetworkNo -1     
        cellNo = cellNo - 1   
        for i in range(NetworkNo*80 + cellNo,(NetworkNo*80 + cellNo) + 80,5):
            trace = array[:,i]
            traces.append(trace)
    print(len(traces))#double check that 16 traces were obtained
    
    
    allTraces = np.concatenate(traces)
    tickLengthStart = int((trace.shape)[0]/2)
    plt.figure(figsize=(20,5))
    plt.title(title)
    plt.xticks(np.arange(tickLengthStart,tickLengthStart*32,step=tickLengthStart*2))
    ax = plt.gca()
    ax.set_xticklabels(np.arange(16,32))
    ax.set_xlabel('SCfreq')
    plt.plot(allTraces)

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
    
    fig,axs = plt.subplots(1,5,figsize=(25,4))
    startNo=getNetIDX(NetNo,SCfreq)
    [axs[i].plot(array[:,startNo+i]) for i in range(0,len(axs))]
    plt.suptitle('network %d at freq %d' %(NetNo,SCfreq))
    return fig

def plotCorrelogram(params,paramsList,title = 'no title'):
    plot = plt.figure(figsize=(20,10))
    Corrs = np.corrcoef(params)
    ax = sns.heatmap(Corrs,xticklabels=paramsList,yticklabels = paramsList,vmin=-0.5,vmax=0.5,annot=True)
    
    plt.title(title,fontsize=15)
    return plot


def printNetVoltages(Voltages, RejectionResults,outfile):
    
    [a,b] = RejectionResults.shape
    netPass = np.array([1 if(np.all(RejectionResults[:a-1,i:i+5]==1)) else 0 for i in range(0,b,5)])# mark 1 if all cells in a net passed passed all rejection criteria
    netPass = np.repeat(netPass,5)
  
    SCfreq = np.arange(16,32)
    with PdfPages(outfile+'.pdf') as pdf:
            j=0
            rows,columns = 5,4# 25 networks on one page, one column is one network

            ## make a list of all the indices in the matrix such that the 2nd plotted element is the 5th cell, and so on
            arr = []
            for j in range(0,b,int((rows*columns))):#for all the rows for the number of pages, skipping the number of cells in a page,
                for i in range(0,rows):#for one row
                    arr.append(np.arange(i+j,i+j+int((rows*columns)),rows))#the array's indices will be the cell number + column number to that number + 1 pg, for how ever many are on a page
            arr = np.concatenate(arr,axis=0)

            for j in range(0,b,rows*columns):#for all the plots, skipping the number in a page,
                plt.figure(figsize=(15,10))
                for i in range(1,rows*columns+1):#for the subplot number, go from 1 to the number of cells in a page+1
                    arrayIdx = j+i-1
                    plt.subplot(rows,columns,i)#plot the next cell, right to left up to down
                    plt.plot(Voltages[:,arr[arrayIdx]])# using the array's next index, which is the cell number of the first cell on the page+ the number in the page
                    if((arr[arrayIdx]%5) == 0):
                        if netPass[int(arr[arrayIdx])] == 1:
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
 
def format_tick_labels(x,y):
    return '{0:.2f}'.format(x)
                
def plotDistributions(Params,paramsList,binSize,title = 'no title',xlabels='none',figuresize=(11,7)):
    
    if type(xlabels) != list:
        xlabels=[xlabels]
    rowLen = 5
    title= title + '1e-4 S'
    Params = Params*10**4
    xlen,ylen = math.ceil(int((len(paramsList) - (len(paramsList) % rowLen) )/rowLen)), rowLen
    if (len(xlabels)) != len(paramsList):
        xlabels *= (xlen*ylen)
    fig,axs = plt.subplots(xlen,ylen,figsize=figuresize)
    plt.subplots_adjust(left=0.2, bottom=0.2, right=.9, top=0.9, wspace=0.5, hspace=0.9)
    for i in range(xlen):
        for j in range(ylen):           
            if (i%xlen*ylen+j) >= len(paramsList):
                break
            axs[i][j].hist(Params[i%xlen*ylen+j,:],bins=binSize)
            axs[i][j].set(title = paramsList[i%xlen*ylen+j])
            xticks = [min(Params[i%xlen*ylen+j,:]),max(Params[i%xlen*ylen+j,:])]
            axs[i][j].set_xticks(xticks)
            axs[i][j].xaxis.set_major_formatter(FuncFormatter(format_tick_labels))
            axs[i][j].set_xlabel(xlabels[i%xlen*ylen+j])
            
    plt.suptitle(title)
    
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