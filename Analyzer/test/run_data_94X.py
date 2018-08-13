import FWCore.ParameterSet.Config as cms

process = cms.Process('analyser')

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.options = cms.untracked.PSet( allowUnscheduled = cms.untracked.bool(True) )

process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
#process.load("Configuration.Geometry.GeometryRecoDB_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
from Configuration.AlCa.GlobalTag_condDBv2 import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:run2_mc')
process.GlobalTag = GlobalTag(process.GlobalTag, '94X_dataRun2_ReReco_EOY17_v6')
process.load("Configuration.StandardSequences.MagneticField_cff")

#process.Tracer = cms.Service("Tracer")
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(100) )
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = 1000

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
        #'file:/data4/cmkuo/testfiles/DYJetsToLL_M-50_RunIIFall17.root'        
        #'/store/mc/RunIIFall17MiniAOD/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/MINIAODSIM/94X_mc2017_realistic_v10-v1/00000/005DC030-D3F4-E711-889A-02163E01A62D.root'
        #'/store/mc/RunIISummer16MiniAODv2/GJets_Pt-20To100_13TeV-sherpa/MINIAODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/60000/ECCEE377-42CD-E611-80DA-002590494FEA.root'
        #'file:sherpa_13TeV_aNTGC_ZZg_h3z0_h4z0_ptmin10_ptmax100_MASTER_cff_py_GEN.root'
        #'file:sherpa_13TeV_aNTGC_ZZg_h3z0_h4z0_ptmin10_ptmax100_MASTER_cff_py_GEN_SIM_DIGI_L1_DIGI2RAW_RAW2DIGI_L1Reco_RECO.root'
        #'file:tryminiaod.root'
        '/store/data/Run2017A/SinglePhoton/MINIAOD/PromptReco-v1/000/295/834/00000/9000F8FD-6548-E711-84D2-02163E0133F1.root'
        ))

#process.load("PhysicsTools.PatAlgos.patSequences_cff")

process.load( "PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff" )
process.load( "PhysicsTools.PatAlgos.selectionLayer1.selectedPatCandidates_cff" )

### EGM scale and smearing correction         
#process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
#                                                  calibratedPatElectrons = cms.PSet(
#    initialSeed = cms.untracked.uint32(12345),
#    engineName = cms.untracked.string('TRandom3')
#    ),
#                                                  calibratedPatPhotons = cms.PSet(
#    initialSeed = cms.untracked.uint32(12345),
#    engineName = cms.untracked.string('TRandom3')
#    ),
#                                                   ggNtuplizer  = cms.PSet(
#    initialSeed = cms.untracked.uint32(67890),
#    engineName = cms.untracked.string('TRandom3')
#    )
#                                                   )

#process.load('EgammaAnalysis.ElectronTools.calibratedPatElectronsRun2_cfi')
#process.load('EgammaAnalysis.ElectronTools.calibratedPatPhotonsRun2_cfi')
#process.calibratedPatElectrons.isMC = cms.bool(True)
#process.calibratedPatPhotons.isMC = cms.bool(True)

process.load("Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff")

process.TFileService = cms.Service("TFileService", fileName = cms.string('ggtree_data.root'))

# MET correction and uncertainties
from PhysicsTools.PatUtils.tools.runMETCorrectionsAndUncertainties import runMetCorAndUncFromMiniAOD
runMetCorAndUncFromMiniAOD(process,
                           isData=False
                           )


runAOD=False

print "runing on AOD:", runAOD

if runAOD:
    process.load("ATGC-Analysis.Analyzer.analyzer_cfi")

if not runAOD:
    process.load("ATGC-Analysis.Analyzer.analyzer_miniAOD_cfi")
    print "Loaded analyzer_miniAOD_cfi"


process.load("ATGC-Analysis.Analyzer.ggPhotonIso_CITK_PUPPI_cff")
#process.analyzer.dumpSoftDrop= cms.bool(True)
#process.analyzer.isAOD=cms.bool(False)
#process.analyzer.doGenParticles=cms.bool(True)
#process.analyzer.dumpSubJets=cms.bool(False)
#process.analyzer.dumpJets=cms.bool(True)
process.analyzer.dumpSoftDrop= cms.bool(True)
#process.analyzer.jecAK8PayloadNames=cms.vstring(jecLevels)
process.analyzer.isAOD=cms.bool(runAOD)
process.analyzer.doGenParticles=cms.bool(False)
process.analyzer.dumpSubJets=cms.bool(True)
process.analyzer.dumpJets=cms.bool(True)
process.analyzer.dumpPhotons=cms.bool(True)
process.analyzer.dumpElectrons=cms.bool(True)
process.analyzer.dumpMuons=cms.bool(True)
process.analyzer.runOnVtx=cms.bool(True)     
process.analyzer.doGenJets=cms.bool(False)       
process.analyzer.runOnReco=cms.bool(True)                 
process.analyzer.runphoIDVID=cms.bool(True)
process.analyzer.runeleIDVID=cms.bool(True)
process.analyzer.runphoMVAID=cms.bool(True)
process.analyzer.runeleMVAID=cms.bool(True)
process.analyzer.addFilterInfoAOD=cms.bool(False)
process.analyzer.doCalib=cms.bool(False)
process.analyzer.runOnSherpa          = cms.bool(True)

#####VID framework####################
from PhysicsTools.SelectorUtils.tools.vid_id_tools import *
dataFormat = DataFormat.MiniAOD
if runAOD: 
    dataFormat = DataFormat.AOD
switchOnVIDElectronIdProducer(process, dataFormat)
switchOnVIDPhotonIdProducer(process, dataFormat)

# define which IDs we want to produce
my_id_modules = ['RecoEgamma.ElectronIdentification.Identification.cutBasedElectronID_Fall17_94X_V1_cff',
                 'RecoEgamma.ElectronIdentification.Identification.heepElectronID_HEEPV70_cff',
                 'RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_noIso_V1_cff',
                 'RecoEgamma.ElectronIdentification.Identification.mvaElectronID_Fall17_iso_V1_cff']

#add them to the VID producer
for idmod in my_id_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDElectronSelection)
    
my_phoid_modules = ['RecoEgamma.PhotonIdentification.Identification.cutBasedPhotonID_Fall17_94X_V1_TrueVtx_cff',
                    'RecoEgamma.PhotonIdentification.Identification.mvaPhotonID_Fall17_94X_V1_cff']

process.load("RecoEgamma.ElectronIdentification.ElectronIDValueMapProducer_cfi")
process.electronIDValueMapProducer.srcMiniAOD = cms.InputTag('slimmedElectrons')
process.electronMVAValueMapProducer.srcMiniAOD = cms.InputTag('slimmedElectrons')
process.photonIDValueMapProducer.srcMiniAOD = cms.InputTag('slimmedPhotons')
process.photonMVAValueMapProducer.srcMiniAOD = cms.InputTag('slimmedPhotons')

#add them to the VID producer
for idmod in my_phoid_modules:
    setupAllVIDIdsInModule(process,idmod,setupVIDPhotonSelection)

process.p = cms.Path(
#    process.regressionApplication*
 #   process.calibratedPatElectrons*
 #   process.calibratedPatPhotons* 

    process.egmGsfElectronIDSequence*
    process.egmPhotonIDSequence*
    process.analyzer
    )

#print process.dumpPython()
