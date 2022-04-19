#initialization: empty the input and output folders
from modules.makeParams import *
import neuron
import os

print("this will empty the folders 'input' and 'output' - proceed anyway - ? [y\\n]")
result = input()
result = result.lower()
if(result == 'n' ):
    quit()
elif (result == 'y'):
    os.chdir("modFiles")
    os.system("nrnivmodl")
    os.chdir("..")

    folderList = ["input","output"]
    subfolderList = ["LV1","LV2","LV3"]
    rmDirs(folderList)
    mkDirandSubdir(folderList,subfolderList)
    
else:
    print("invalid input")

