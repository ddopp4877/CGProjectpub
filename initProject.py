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
    
        #make averaged folders
    path1 = os.path.join("input","LV3","Avg")
    path2 = os.path.join("output","LV3","Avg")
    os.mkdir(path1)
    os.mkdir(path2)
    
    
else:
    print("invalid input")

