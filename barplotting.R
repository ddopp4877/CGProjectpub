
setwd("C:/Users/ddopp/source/repos/CGresults/plottingScripts")

# barplot
library(tidyverse)
library(RColorBrewer)
library(corrr)
library(dplyr)
library(tidyr)
library(ggplot2)
library(svglite)
library(corrplot)
library("rhdf5")
library("matrixStats")
library(purrr)
library(Kendall)
library(ggcorrplot)
library(gridExtra)
library(grid)
# R works better with a lot of rows than a lot of columns, hence the upfront 
# transposition.
lvl1out = t(h5read(file = "./../fixed_Gsyn/MedwithLV2/output/LV1/Params.hdf5",
                   name = "default"))

lvl2out = t(h5read(file = "./../fixed_Gsyn/MedwithLV2/output/LV2/passParamsRepeatControl.hdf5",
                   name = "default"))#repeat means each param set tested at all 16 frequencies

lvl3out = t(h5read(file = "./../fixed_Gsyn/MedwithLV2/output/LV3/passParamsRepeat.hdf5",
                   name = "default"))

###########
lvl1PassCrit = h5read(file = "./../fixed_Gsyn/MedwithLV2/output/LV1/LV1RejectionResults.hdf5",
                      name = "default")#coded 1 for passing,2 for failing for being too high, 0 for failing for being too low

lvl2PassCrit = h5read(file = "./../fixed_Gsyn/MedwithLV2/output/LV2/LV2RejectionResults.hdf5",
                      name = "default")

lvl3PassCrit = h5read(file = "./../fixed_Gsyn/MedwithLV2/output/LV3/LV3RejectionResults.hdf5",
                      name = "default")
###RejectionResultsRaw.hdf5 has the actual values calculated for the given parameter set

lvl1PassIdxs = which(rowProds(lvl1PassCrit)==1)# should be 15161
lvl2PassIdxs = which(rowProds(lvl2PassCrit)==1)# should be 6587



#append SCfrequency
#lvl2SCfreq = h5read(file = "output/LV2/SCfreqs.hdf5",
#name = "default")
#lvl3SCfreq = h5read(file = "output/LV3/SCfreqs.hdf5",
#name = "default")

#lvl2ParamsWithSCF = cbind(lvl2out,lvl2SCfreq)
#lvl3ParamsWithSCF = cbind(lvl3out,lvl3SCfreq)
#get the passing or nonpassing parameters
lvl1Pass = lvl1out[lvl1PassIdxs,]
lvl2Pass = lvl2out[lvl2PassIdxs,]


#make them unique if desired
lvl1PassUn = unique(lvl1Pass) 
lvl2PassUn = unique(lvl2Pass)
lvl3PassUn = h5read(file = "./../fixed_Gsyn/MedwithLV2/output/LV3/LV3PassParams.hdf5",
                    name = "default")

# Vectors for plotting ---------------------------------------------------------
# Set up desired dependent variable vector
lvl1Cols <- c(
  "gleakSoma", "gASoma", "gBKKcaSoma", "gSKKcaSoma","gKd1Soma", "gKd2Soma", "gCalSoma","gCatSoma","gCANSoma",
  "gNapSoma","gleakNeu"
)

lvl2Cols <- c(
  "gleakSoma", "gASoma", "gBKKcaSoma", "gSKKcaSoma","gKd1Soma", "gKd2Soma", "gCalSoma","gCatSoma","gCANSoma",
  "gNapSoma","gleakNeu", "gCatNeu", "gCaLNeu","gNapNeu", 
  "BkkcaNeu"
)


lvl3Cols <- c(
  "gleakSoma", "gASoma", "gBKKcaSoma", "gSKKcaSoma","gKd1Soma", "gKd2Soma", "gCalSoma","gCatSoma","gCANSoma",
  "gNapSoma","gleakNeu", "gCatNeu", "gCaLNeu","gNapNeu", 
  "BkkcaNeu"
)


lvl0 = data.frame(unique(lvl1out))#what was tested in lvl1
colnames(lvl0) = lvl1Cols

lvl1 = data.frame(lvl1PassUn)#unique passing lvl1,what was tested in lvl2
colnames(lvl1) = lvl1Cols

lvl2 = data.frame(lvl2PassUn)#what passed lv2, and goes into lvl3
colnames(lvl2) = lvl2Cols

lvl3 = data.frame(lvl3PassUn)#what networks passed lvl3
colnames(lvl3) = lvl3Cols

lvl0$lvl = 0
lvl1$lvl = 1
lvl2$lvl = 2
lvl3$lvl = 3



## Great a single dataframe with one set of each level's entries ===============
# From the level 2 input file retain only passing cells, level one is 
# already represented in the level 1 file. Same with level 3.


M = full_join(lvl0, lvl1) %>% full_join(lvl2) %>% full_join(lvl3)






soma_cols = c("gleakSoma", "gASoma", "gBKKcaSoma", "gSKKcaSoma","gKd1Soma", "gKd2Soma", "gCalSoma","gCatSoma","gCANSoma",
              "gNapSoma")
neurite_cols = c("gleakNeu", "gCatNeu", "gCaLNeu","gNapNeu", 
                 "BkkcaNeu")

all_cols = c(soma_cols,neurite_cols)
M = cbind(M[,1:10],M$lvl,M[,11],M[,13:16])
colnames(M) = c(soma_cols,"lvl",neurite_cols)

find_corrs = function(LVL,type){
  
  if(type == "soma"){
    start = 1
    end = (length(soma_cols)+1)

  }
  if(type=="neurite"){
    start = length(soma_cols)
    end = length(all_cols)

  }
  if(type=="all"){
    start = 1
    end = 16
  }
  
  M_local = M[,start:end]
  corrs = M_local %>% filter(lvl == LVL)
  corrs = correlate(corrs,method='spearman',use='everything')
  corrs[is.na(corrs)] = 0
  corrs = corrs[corrs$term != "lvl",]
  corrs = subset(corrs, select = -c(lvl))
  return(corrs)
}

selectionVal = "soma"
corrs0 = find_corrs(0,selectionVal)
corrs1 = find_corrs(1,selectionVal)
corrs2 = find_corrs(2,selectionVal)
corrs3 = find_corrs(3,selectionVal)

#### single case ##########
a = 1
b = 11
df = rbind(corrs0[a,b],corrs1[a,b],corrs2[a,b],corrs3[a,b])
df = cbind(df,c(0:3))
colnames(df) = c("vals","levels")
df$polarity = df$vals < 0
ggplot(data=df,aes(x =levels,y=vals )) +
  geom_bar(stat='identity',aes(x=levels,y=vals,fill=polarity))+
  scale_y_continuous(limits = c(-0.25,0.25))+
  scale_fill_manual(values = c(RColorBrewer::brewer.pal(3, "PuOr")[c(1, 3)]))+
  theme(legend.position = "none",strip.text.x = element_blank())

##############################

polarityf = function(df){
  d = NULL
  for(i in c(1:length(df$vals))){
    if( df$vals[i] < 0 & df$vals[i] > -1){
      d = c(d, -1)
    }
    else if(df$vals[i] > 0 ){
      d = c(d,1 )
    }
    else{
      d = c(d,0)
    }
  }
  return(as.character(d))
}

    

#add a black line color for small data, and labels then photoshop to be triangular

p = data.frame()
for(i in c(1:dim(corrs0)[1])){
  for(j in c(2:length(corrs0))){
    df = rbind(corrs0[i,j],corrs1[i,j],corrs2[i,j],corrs3[i,j])
    df = cbind(df,c(0:3))
    colnames(df) = c("vals","levels")
    #df$polarity = df$vals < 0
    df$polarity = polarityf(df)
    df$row = i
    df$col = j-1
    df$name = colnames(corrs0)[j]
    p = rbind(p,df)
    print(colnames(corrs0[i,j]))
    print(df$vals)
    print(df$plot)
    
    
  }
}


getName = function(string){
  return(neurite_cols)
}

getName = function(string){
  return(soma_cols)
}

getName = function(string){
  return(all_cols)
}

colorScale = c(RColorBrewer::brewer.pal(3, "PuOr")[c(3)])
colorScale[2] = "#000000"
colorScale[3] = c(RColorBrewer::brewer.pal(3, "PuOr")[c(1)])
  



ggplot(data=p,aes(x =levels,y=vals,fill=polarity )) +
  geom_bar(stat='identity',aes(x=levels,y=vals))+
  geom_hline(yintercept = 0, color = "black",size=0.5,alpha=0.5)+
  scale_y_continuous(limits = c(-0.2,0.2))+
  scale_fill_manual(values = colorScale)+
  theme(legend.position = "none",strip.background = element_blank(),
        panel.grid.minor =element_blank(),
        strip.text.x = element_text(angle = 45, hjust = 0.5),
        strip.text.y = element_text(angle = 0),
        axis.title = element_text(size = 12,face="bold")
        )+
  facet_grid(rows = vars(row),cols = vars(col),
             labeller = labeller(col = getName,row=getName)  )+
  labs(x="selection level", y = "correlation coefficient (rho-value)")


