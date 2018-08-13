from CRABClient.UserUtilities import config, getUsernameFromSiteDB
import sys
config = config()

submitVersion = "V0"
#submitVersion = "V1"

mainOutputDir = '/store/user/shilpi/aNTGC/%s' % submitVersion
#mainOutputDir = '/store/caf/user/shilpi/aNTGC/%s' % submitVersion 

config.General.transferLogs = False

config.JobType.pluginName  = 'Analysis'

# Name of the CMSSW configuration file
config.JobType.psetName  = 'run_mc_94X.py'
#config.Data.allowNonValidInputDataset = False
config.JobType.sendExternalFolder     = True

config.Data.inputDBS = 'global'
config.Data.publication = False
config.Data.allowNonValidInputDataset = True
config.Site.storageSite = 'T2_CH_CERN'



if __name__ == '__main__':

    from CRABAPI.RawCommand import crabCommand
    from CRABClient.ClientExceptions import ClientException
    from httplib import HTTPException

    # We want to put all the CRAB project directories from the tasks we submit here into one common directory.
    # That's why we need to set this parameter (here or above in the configuration file, it does not matter, we will not overwrite it).
    config.General.workArea = 'crab_%s' % submitVersion

    def submit(config):
        try:
            crabCommand('submit', config = config)
        except HTTPException as hte:
            print "Failed submitting task: %s" % (hte.headers)
        except ClientException as cle:
            print "Failed submitting task: %s" % (cle)


    ##### submit MC
    config.Data.outLFNDirBase = '%s/%s/' % (mainOutputDir,'mc')
    config.Data.splitting     = 'FileBased'
    config.Data.unitsPerJob   = 4
    config.JobType.psetName  = 'run_mc_94X.py'


#####Gjet
    config.General.requestName  = 'GJetsHT40To100_ext1'
    config.Data.inputDataset    = '/GJets_HT-40To100_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_1core_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM'
#    submit(config)

    config.General.requestName  = 'GJetsHT100To200'
    config.Data.inputDataset    = '/GJets_HT-100To200_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
#    submit(config)


    config.General.requestName  = 'GJetsHT200To400'
    config.Data.inputDataset    = '/GJets_HT-200To400_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
#    submit(config)

    config.General.requestName  = 'GJetsHT400To600'
    config.Data.inputDataset    = '/GJets_HT-400To600_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAOD-PU2017_94X_mc2017_realistic_v11-v1/MINIAODSIM'
#    submit(config)

 
    config.General.requestName  = 'GJetsHT600ToInf'
    config.Data.inputDataset    = '/GJets_HT-600ToInf_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
#    submit(config)


#####QCD

    config.General.requestName  = 'QCDPt30To50'
    config.Data.inputDataset    = '/QCD_Pt_30to50_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
#    submit(config)

    config.General.requestName  = 'QCDPt50To80'
    config.Data.inputDataset    = '/QCD_Pt_50to80_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
#    submit(config)

    config.General.requestName  = 'QCDPt80To120'
    config.Data.inputDataset    = '/QCD_Pt_80to120_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
#    submit(config)

    config.General.requestName  = 'QCDPt120To170'
    config.Data.inputDataset    = '/QCD_Pt_120to170_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
#    submit(config)

    config.General.requestName  = 'QCDPt170To300'
    config.Data.inputDataset    = '/QCD_Pt_170to300_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
#    submit(config)

    config.General.requestName  = 'QCDPt300To470'
    config.Data.inputDataset    = '/QCD_Pt_300to470_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
#    submit(config)

    config.General.requestName  = 'QCDPt470To600'
    config.Data.inputDataset    = '/QCD_Pt_470to600_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
#    submit(config)

    config.General.requestName  = 'QCDPt600To800'
    config.Data.inputDataset    = '/QCD_Pt_600to800_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
#    submit(config)

    config.General.requestName  = 'QCDPt800To1000'
    config.Data.inputDataset    = '/QCD_Pt_800to1000_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
#    submit(config)

    config.General.requestName  = 'QCDPt1000To1400'
    config.Data.inputDataset    = '/QCD_Pt_1000to1400_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
#    submit(config)

    config.General.requestName  = 'QCDPt1400To1800'
    config.Data.inputDataset    = '/QCD_Pt_1400to1800_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
##    submit(config)

    config.General.requestName  = 'QCDPt1800To2400'
    config.Data.inputDataset    = '/QCD_Pt_1800to2400_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
##    submit(config)

    config.General.requestName  = 'QCDPt2400To3200'
    config.Data.inputDataset    = '/QCD_Pt_2400to3200_TuneCP5_13TeV_pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
##    submit(config)



#### TTjets
    config.General.requestName  = 'TTJets'
    config.Data.inputDataset    = '/TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
#    submit(config)
    
    config.General.requestName  = 'TTGJets'
    config.Data.inputDataset    = '/TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
#    submit(config)

    config.General.requestName  = 'TTGJets_ext1'
    config.Data.inputDataset    = '/TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8/RunIIFall17MiniAODv2-PU2017_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM'
##    submit(config)


    config.General.requestName  = 'DYJetsToll'
    config.Data.inputDataset    = '/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017RECOSIMstep_12Apr2018_94X_mc2017_realistic_v14-v1/MINIAODSIM'
#    submit(config)

    config.General.requestName  = 'DYJetsToll_ext1'
    config.Data.inputDataset    = '/DYJetsToLL_M-50_TuneCP5_13TeV-madgraphMLM-pythia8/RunIIFall17MiniAODv2-PU2017RECOSIMstep_12Apr2018_94X_mc2017_realistic_v14_ext1-v1/MINIAODSIM'
#    submit(config)


#######################DATA
    ##### submit MC
    config.Data.outLFNDirBase = '%s/%s/' % (mainOutputDir,'data')
    config.Data.splitting     = 'LumiBased'
    config.JobType.psetName  = 'run_data_94X.py'
    #config.Data.lumiMask      = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/PromptReco/Cert_294927-306462_13TeV_PromptReco_Collisions17_JSON.txt'
    config.Data.lumiMask      = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions17/13TeV/ReReco/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt'
    config.Data.unitsPerJob   = 50

    config.General.requestName  = 'SinglePhotonB'
    config.Data.inputDataset    = '/SinglePhoton/Run2017B-31Mar2018-v1/MINIAOD'
#    submit(config)

    config.General.requestName  = 'SinglePhotonC'
    config.Data.inputDataset    = '/SinglePhoton/Run2017C-31Mar2018-v1/MINIAOD'
#    submit(config)

    config.General.requestName  = 'SinglePhotonD'
    config.Data.inputDataset    = '/SinglePhoton/Run2017D-31Mar2018-v1/MINIAOD'
    submit(config)

    config.General.requestName  = 'SinglePhotonE'
    config.Data.inputDataset    = '/SinglePhoton/Run2017E-31Mar2018-v1/MINIAOD'
    submit(config)

    config.General.requestName  = 'SinglePhotonF'
    config.Data.inputDataset    = '/SinglePhoton/Run2017F-31Mar2018-v1/MINIAOD'
#    submit(config)
