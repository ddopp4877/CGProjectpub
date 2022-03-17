#initialization: empty the input and output folders
from modules.makeParams import *
import neuron
import os


os.chdir("modFiles")
os.system("nrnivmodl")
os.chdir("..")

folderList = ["input","output"]
subfolderList = ["LV1","LV2","LV3"]
rmDirs(folderList)
mkDirandSubdir(folderList,subfolderList)

