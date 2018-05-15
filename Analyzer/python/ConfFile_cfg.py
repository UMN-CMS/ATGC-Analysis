import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.Geometry.GeometryRecoDB_cff')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use                                                                                                                     
                            fileNames = cms.untracked.vstring(
        #'/store/mc/RunIISummer16MiniAODv2/DYJetsToLL_M-50_TuneCUETP8M1_13TeV-madgraphMLM-pythia8/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6_ext1-v2/60000/CEAB3688-1CC7-E611-8BC3-C4346BBCB6A8.root'
        #'/store/mc/RunIIFall17MiniAOD/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/94X_mc2017_realistic_v10-v1/00000/6E2D3AD4-9EF5-E711-8AD0-02163E019BA1.root'
        'file:/eos/cms/store/user/rchatter/aTGC/Signal/Sherpa/sherpa_13TeV_aNTGC_Zgg_h3g0_h4g0_MASTER_cff_py_GEN.root'
        )
                            )

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '94X_mc2017_realistic_v10', '') ###94X sample
#process.GlobalTag = GlobalTag(process.GlobalTag, '80X_dataRun2_2016LegacyRepro_v4', '') 

#process.GlobalTag = GlobalTag(process.GlobalTag, '80X_mcRun2_asymptotic_2016_miniAODv2_v0', '')  ###80X sample


from RecoEgamma.EgammaTools.regressionModifier_cfi import regressionModifier 

process.TFileService = cms.Service("TFileService", fileName = cms.string('egmRegression.root'))

process.demo = cms.EDAnalyzer('Analyzer',
                              #recoPhotonSrc = cms.InputTag("gedPhotons")  ###AOD
                              #recoPhotonSrc = cms.InputTag("slimmedPhotons"),  ###MINIAOD
                              recoElectronSrc = cms.InputTag("slimmedElectrons"),  ###MINIAOD
                              doGenParticles  = cms.bool(True),
                              doRecoEle       = cms.bool(False),
                              #genParticleSrc = cms.InputTag("prunedGenParticles"),
                              genParticleSrc = cms.InputTag("genParticles"),
                              regressionConfig = regressionModifier.clone(manualBunchSpacing = cms.int32(25),              
                                                                          rhoCollection = cms.InputTag("fixedGridRhoFastjetAll"),
                                                                          vertexCollection = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                                                          autoDetectBunchSpacing = cms.bool(False),                                                                                              
                                                                          photon_config   = cms.PSet( # EB, EE
            regressionKey_ecalonly  = cms.vstring('photon_eb_ECALonly_lowpt', 'photon_eb_ECALonly', 'photon_ee_ECALonly_lowpt', 'photon_ee_ECALonly'),
            uncertaintyKey_ecalonly = cms.vstring('photon_eb_ECALonly_lowpt_var', 'photon_eb_ECALonly_var', 'photon_ee_ECALonly_lowpt_var', 'photon_ee_ECALonly_var'),
            
            #regressionKey_25ns  = cms.vstring('gedphoton_EBCorrection_25ns', 'gedphoton_EECorrection_25ns'),
            #uncertaintyKey_25ns = cms.vstring('gedphoton_EBUncertainty_25ns', 'gedphoton_EEUncertainty_25ns'),
            
            #regressionKey_50ns  = cms.vstring('gedphoton_EBCorrection_50ns', 'gedphoton_EECorrection_50ns'),
            #uncertaintyKey_50ns = cms.vstring('gedphoton_EBUncertainty_50ns', 'gedphoton_EEUncertainty_50ns'),
            ),
                                                                          )
                              
                              )



process.p = cms.Path(process.demo)

#print process.dumpPython()
