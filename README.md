# CGProjectpub

Getting started:
download python 3.9 (>= 3.10 will not work with neuron 8.0)

windows:  
download Neuron 8.0  
pip install -r requirementsWindows.txt

Unix:  
pip install -r requirements.txt  
  
then run:  
python initProject.py  
python CGProject.py


to run CGrun.sh, enter :  
yes | bash CGRun.sh

---------------------------------------------------------------------
Notes

change the number of trials in CGProject.py to change how many cells are generated

edit and run AnalysisScript.py to look at parameter distributions, parameter correlations,
and pass/fail criteria ratios

run saveOutFigs.py to save the voltage traces of the cells or networks to a pdf. change
the index parameter to determine how many cells/networks to save. passing cells are marked
with **, so just search this in the pdf to find passing cells or networks. Note that if the  
data is too large, you will have to uncomment import matplotlib matplotlib.use('Agg')  
in the postAnalysis.py file. The reason this is commented out is because matplotlib will not  
work in jupyterlab with this backend, but without it you can't print large numbers of networks into the pdf

run lv2GUI.py or lv3GUI.py to tune a cell or network. you can change maximal conductance
as well as synaptic gain, and for lv3 you can also change gap condutance for large cell or siz



note that RAM usage seems rather high, probably pandas and pickle are responsible. with 16GB of RAM
and 10000 cells starting, the resulting data for all 3 runs together is about 20 GB of ROM and at peak runtime (should be during LV2),
 about 8 GB of RAM is used. Total runtime on intel i5 with 16 GB RAM was ~ 12 hours total, ~ 6 hours for LV3
 
this could probably be worked around if the simulation scripts were turned into generator
functions, and results aggregated for each round. The most gain could be gotten from doing
this with LV2 since it usually uses the most RAM because of the number of cells (16X the number passing LV1)
and not many pass it by comparison.

CGenv is also included in the github, but is probably best to set up your own environment if so desired


the output files are a mix of txt and pkl. If you want them all to be converted to hdf5 instead, call the function:
convertResultstoH5(folderList(),subfolderList()) while in the AnalysisScript.py file. (can't use with Avg folder in lv3 though so have to remove it first)

to verify everything worked, it worth using 'Checkruns.ipynb' to make sure the input and output files produce the number of passing cells/networks that is expected
then, print all the  networks and verify that ones marked passing look like they should have passed
then select a passing cell or network and explore it with the gui
