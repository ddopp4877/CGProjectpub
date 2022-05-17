#from CGProject import VoltageFilename
from modules.postAnalysis import *
import numpy as np
import os
import pandas as pd
import time
from scipy.stats import kurtosis

print('test') 

archivedPath = os.path.join("..","CGresults","may13_22")#folder is outside of CGProjectpub
coded = np.loadtxt(os.path.join(archivedPath,"output","LV3","LV3RejectionResults.txt"))
SCfreqsnonavg = np.loadtxt(os.path.join(archivedPath,"output","LV3","SCfreqs"))
mappedIDxs,freqsnonavgpassing = getRasterData(coded,SCfreqsnonavg)
plotRaster(mappedIDxs,freqsnonavgpassing)

#plt.savefig('non average passing')

plt.show()