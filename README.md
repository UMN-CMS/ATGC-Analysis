# ATGC-Analysis
Anomalous Triple Gauge Coupling Analysis
<br> cmsrel CMSSW_9_4_0_patch1 
<br> cd CMSSW_9_4_0_patch1/src 
<br> cmsenv 
<br> git cms-init
<br> git cms-merge-topic lsoffi:CMSSW_9_4_0_pre3_TnP
<br> scram b -j 9


<br># Add the area containing the MVA weights (from cms-data, to appear in “external”).
<br> # Note: the “external” area appears after “scram build” is run at least once, as above
<br> #
<br> cd $CMSSW_BASE/external
<br> # below, you may have a different architecture, this is just one example from lxplus
<br> cd slc6_amd64_gcc630/
<br> git clone https://github.com/lsoffi/RecoEgamma-PhotonIdentification.git data/RecoEgamma/PhotonIdentification/data
<br> cd data/RecoEgamma/PhotonIdentification/data
<br> git checkout CMSSW_9_4_0_pre3_TnP
<br> cd $CMSSW_BASE/external
<br> cd slc6_amd64_gcc630/
<br> git clone https://github.com/lsoffi/RecoEgamma-ElectronIdentification.git data/RecoEgamma/ElectronIdentification/data
<br> cd data/RecoEgamma/ElectronIdentification/data
<br> git checkout CMSSW_9_4_0_pre3_TnP
<br> # Go back to the src/
<br> cd $CMSSW_BASE/src

<br> git clone https://github.com/UMN-CMS/ATGC-Analysis.git 
<br> cd ATGC-Analysis/Analyzer 
<br> scram b -j10 
