#include "ATGC-Analysis/Analyzer/interface/Analyzer.h"

using namespace std;
using namespace edm;

void setbit(UShort_t& x, UShort_t bit) {
  UShort_t a = 1;
  x |= (a << bit);
}

Analyzer::Analyzer(const edm::ParameterSet& ps) :
  hltPrescaleProvider_(ps, consumesCollector(), *this)
{

  development_               = ps.getParameter<bool>("development");
  addFilterInfoAOD_          = ps.getParameter<bool>("addFilterInfoAOD");
  addFilterInfoMINIAOD_      = ps.getParameter<bool>("addFilterInfoMINIAOD");

  doGenJets_            = ps.getParameter<bool>("doGenJets");

  doGenParticles_            = ps.getParameter<bool>("doGenParticles");
  doCalib_            = ps.getParameter<bool>("doCalib");
  runOnParticleGun_          = ps.getParameter<bool>("runOnParticleGun");
  runOnSherpa_               = ps.getParameter<bool>("runOnSherpa");
  runOnVtx_               = ps.getParameter<bool>("runOnVtx");
  runOnReco_               = ps.getParameter<bool>("runOnReco");
  dumpPhotons_               = ps.getParameter<bool>("dumpPhotons");
  dumpElectrons_               = ps.getParameter<bool>("dumpElectrons");
  dumpMuons_               = ps.getParameter<bool>("dumpMuons");
  dumpJets_                  = ps.getParameter<bool>("dumpJets");
  dumpSubJets_               = ps.getParameter<bool>("dumpSubJets");
  dumpSoftDrop_              = ps.getParameter<bool>("dumpSoftDrop");
  dumpPDFSystWeight_         = ps.getParameter<bool>("dumpPDFSystWeight");
  dumpGenScaleSystWeights_   = ps.getParameter<bool>("dumpGenScaleSystWeights");
  isAOD_                     = ps.getParameter<bool>("isAOD");
  

  vtxLabel_                  = consumes<reco::VertexCollection>        (ps.getParameter<InputTag>("VtxLabel"));
  vtxBSLabel_                = consumes<reco::VertexCollection>        (ps.getParameter<InputTag>("VtxBSLabel"));
  rhoLabel_                  = consumes<double>                        (ps.getParameter<InputTag>("rhoLabel"));
  rhoCentralLabel_           = consumes<double>                        (ps.getParameter<InputTag>("rhoCentralLabel"));
  trgResultsLabel_           = consumes<edm::TriggerResults>           (ps.getParameter<InputTag>("triggerResults"));
  trgResultsProcess_         =                                          ps.getParameter<InputTag>("triggerResults").process();
  generatorLabel_            = consumes<GenEventInfoProduct>           (ps.getParameter<InputTag>("generatorLabel"));
  lheEventLabel_             = consumes<LHEEventProduct>               (ps.getParameter<InputTag>("LHEEventLabel"));
  puCollection_              = consumes<vector<PileupSummaryInfo> >    (ps.getParameter<InputTag>("pileupCollection"));
  genParticlesCollection_    = consumes<vector<reco::GenParticle> >    (ps.getParameter<InputTag>("genParticleSrc"));
  electronCollection_        = consumes<View<pat::Electron> >          (ps.getParameter<InputTag>("electronSrc"));
  calibelectronCollection_   = consumes<View<pat::Electron> >          (ps.getParameter<InputTag>("calibelectronSrc"));
  gsfTracks_                 = consumes<View<reco::GsfTrack>>          (ps.getParameter<InputTag>("gsfTrackSrc"));

  photonCollection_          = consumes<View<pat::Photon> >            (ps.getParameter<InputTag>("photonSrc"));
  calibphotonCollection_     = consumes<View<pat::Photon> >            (ps.getParameter<InputTag>("calibphotonSrc"));
  muonCollection_            = consumes<View<pat::Muon> >              (ps.getParameter<InputTag>("muonSrc"));
  ebReducedRecHitCollection_ = consumes<EcalRecHitCollection>          (ps.getParameter<InputTag>("ebReducedRecHitCollection"));
  eeReducedRecHitCollection_ = consumes<EcalRecHitCollection>          (ps.getParameter<InputTag>("eeReducedRecHitCollection"));
  esReducedRecHitCollection_ = consumes<EcalRecHitCollection>          (ps.getParameter<InputTag>("esReducedRecHitCollection")); 
  recophotonCollection_      = consumes<reco::PhotonCollection>        (ps.getParameter<InputTag>("recoPhotonSrc"));
  tracklabel_                = consumes<reco::TrackCollection>         (ps.getParameter<InputTag>("TrackLabel"));
  gsfElectronlabel_          = consumes<reco::GsfElectronCollection>   (ps.getParameter<InputTag>("gsfElectronLabel"));
  pfAllParticles_            = consumes<reco::PFCandidateCollection>   (ps.getParameter<InputTag>("PFAllCandidates"));
  ///pckPFCandidateCollection_  = consumes<pat::PackedCandidateCollection>(ps.getParameter<InputTag>("packedPFCands"));
  //pckPFCdsLabel_             = consumes<vector<pat::PackedCandidate>>  (ps.getParameter<InputTag>("packedPFCands"));
  recoCdsLabel_              = consumes<View<reco::Candidate>>         (ps.getParameter<InputTag>("packedPFCands"));

  jetsAK4Label_              = consumes<View<pat::Jet> >               (ps.getParameter<InputTag>("ak4JetSrc"));
  jetsAK8Label_              = consumes<View<pat::Jet> >               (ps.getParameter<InputTag>("ak8JetSrc"));


  GenJetLabel_              = consumes<std::vector<reco::GenJet> >               (ps.getParameter<InputTag>("GenJetLabel"));

  //boostedDoubleSVLabel_      = consumes<reco::JetTagCollection>        (ps.getParameter<InputTag>("boostedDoubleSVLabel"));
  newparticles_              =                                          ps.getParameter< vector<int > >("newParticles");
  //jecAK8PayloadNames_        =                                          ps.getParameter<std::vector<std::string> >("jecAK8PayloadNames"); 

  //pfLooseId_                 = ps.getParameter<ParameterSet>("pfLooseId");

  //egmScaler_   = new EnergyScaleCorrection_class("EgammaAnalysis/ElectronTools/data/ScalesSmearings/Moriond17_23Jan_ele");

  // electron ID 
  eleVetoIdMapToken_       = consumes<edm::ValueMap<bool> >(ps.getParameter<edm::InputTag>("eleVetoIdMap"));
  eleLooseIdMapToken_      = consumes<edm::ValueMap<bool> >(ps.getParameter<edm::InputTag>("eleLooseIdMap"));
  eleMediumIdMapToken_     = consumes<edm::ValueMap<bool> >(ps.getParameter<edm::InputTag>("eleMediumIdMap"));
  eleTightIdMapToken_      = consumes<edm::ValueMap<bool> >(ps.getParameter<edm::InputTag>("eleTightIdMap"));
  //eleHLTIdMapToken_        = consumes<edm::ValueMap<bool> >(ps.getParameter<edm::InputTag>("eleHLTIdMap"));
  eleHEEPIdMapToken_       = consumes<edm::ValueMap<bool> >(ps.getParameter<edm::InputTag>("eleHEEPIdMap"));
  //eleMVAValuesMapToken_    = consumes<edm::ValueMap<float> >(ps.getParameter<edm::InputTag>("eleMVAValuesMap"));
  //eleMVAHZZValuesMapToken_ = consumes<edm::ValueMap<float> >(ps.getParameter<edm::InputTag>("eleMVAHZZValuesMap"));
  elePFClusEcalIsoToken_   = mayConsume<edm::ValueMap<float> >(ps.getParameter<edm::InputTag>("elePFClusEcalIsoProducer"));
  elePFClusHcalIsoToken_   = mayConsume<edm::ValueMap<float> >(ps.getParameter<edm::InputTag>("elePFClusHcalIsoProducer"));

  // Photon ID in VID framwork 
  phoLooseIdMapToken_             = consumes<edm::ValueMap<bool> >(ps.getParameter<edm::InputTag>("phoLooseIdMap"));
  phoMediumIdMapToken_            = consumes<edm::ValueMap<bool> >(ps.getParameter<edm::InputTag>("phoMediumIdMap"));
  phoTightIdMapToken_             = consumes<edm::ValueMap<bool> >(ps.getParameter<edm::InputTag>("phoTightIdMap"));
  phoMVAValuesMapToken_           = consumes<edm::ValueMap<float> >(ps.getParameter<edm::InputTag>("phoMVAValuesMap")); 
  phoChargedIsolationToken_       = consumes <edm::ValueMap<float> >(ps.getParameter<edm::InputTag>("phoChargedIsolation"));
  phoNeutralHadronIsolationToken_ = consumes <edm::ValueMap<float> >(ps.getParameter<edm::InputTag>("phoNeutralHadronIsolation"));
  phoPhotonIsolationToken_        = consumes <edm::ValueMap<float> >(ps.getParameter<edm::InputTag>("phoPhotonIsolation"));
  phoWorstChargedIsolationToken_  = consumes <edm::ValueMap<float> >(ps.getParameter<edm::InputTag>("phoWorstChargedIsolation"));

  //phoChargedIsolationToken_CITK_       = consumes <edm::ValueMap<float> >(ps.getParameter<edm::InputTag>("phoChargedIsolation_CITK"));
  //phoPhotonIsolationToken_CITK_        = consumes <edm::ValueMap<float> >(ps.getParameter<edm::InputTag>("phoPhotonIsolation_CITK"));
  //phoNeutralHadronIsolationToken_CITK_ = consumes <edm::ValueMap<float> >(ps.getParameter<edm::InputTag>("phoNeutralHadronIsolation_CITK"));
  
  //phoChargedIsolationToken_PUPPI_       = consumes <edm::ValueMap<float> >(ps.getParameter<edm::InputTag>("phoChargedIsolation_PUPPI"));
  //phoNeutralHadronIsolationToken_PUPPI_ = consumes <edm::ValueMap<float> >(ps.getParameter<edm::InputTag>("phoNeutralHadronIsolation_PUPPI"));
  //phoPhotonIsolationToken_PUPPI_        = consumes <edm::ValueMap<float> >(ps.getParameter<edm::InputTag>("phoPhotonIsolation_PUPPI"));

  Service<TFileService> fs;
  tree_    = fs->make<TTree>("EventTree", "Event data (tag V08_00_26_06)");
  hEvents_ = fs->make<TH1F>("hEvents",    "total processed and skimmed events",   2,  0,   2);

  if(runOnReco_) branchesGlobalEvent(tree_);

  if (doGenParticles_) {
    branchesGenInfo(tree_, fs);
    branchesGenPart(tree_);
  }

  if( doGenJets_)
    branchesGenJetPart(tree_);


  if(dumpPhotons_) branchesPhotons(tree_);
  if(dumpElectrons_) branchesElectrons(tree_);
  if(dumpMuons_) branchesMuons(tree_);
  if (dumpJets_)       branchesJets(tree_);
}

Analyzer::~Analyzer() {
  cleanupPhotons();
}

void Analyzer::analyze(const edm::Event& e, const edm::EventSetup& es) {

  hEvents_->Fill(0.5);

  if (doGenParticles_) {
    jetResolution_   = JME::JetResolution::get(es, "AK4PFchs_pt");
    jetResolutionSF_ = JME::JetResolutionScaleFactor::get(es, "AK4PFchs");
    AK8jetResolution_   = JME::JetResolution::get(es, "AK8PFchs_pt");
    AK8jetResolutionSF_ = JME::JetResolutionScaleFactor::get(es, "AK8PFchs");
  }




  //std::cout<<"taking vertex collection"<<std::endl;
    edm::Handle<reco::VertexCollection> vtxHandle;
    math::XYZPoint pv(0, 0, 0);    
    reco::Vertex vtx;
  if( runOnVtx_ || dumpElectrons_ || dumpMuons_ ){
    e.getByToken(vtxLabel_, vtxHandle);
    

    ///std::cout<<"took vertex collection"<<std::endl;
    
    // best-known primary vertex coordinates

    for (vector<reco::Vertex>::const_iterator v = vtxHandle->begin(); v != vtxHandle->end(); ++v) {
      // replace isFake() for miniAOD since it requires tracks while miniAOD vertices don't have tracks:
      // Vertex.h: bool isFake() const {return (chi2_==0 && ndof_==0 && tracks_.empty());}
      bool isFake = isAOD_ ? v->isFake() : (v->chi2() == 0 && v->ndof() == 0);
      
      if (!isFake) {
	pv.SetXYZ(v->x(), v->y(), v->z());
	vtx = *v;
	break;
      }
    }
  }//if( runOnVtx_ )

  //initTriggerFilters(e);

  if(runOnReco_) fillGlobalEvent(e, es);
  if (!e.isRealData()) {
    fillGenInfo(e);
    if (doGenParticles_)
      fillGenPart(e);
  }

  if(dumpPhotons_) fillPhotons(e, es); // FIXME: photons have different vertex (not pv)
  if(dumpElectrons_) fillElectrons(e, es, pv);
  if(dumpMuons_) fillMuons(e, pv, vtx);
  
  if (!e.isRealData()) 
    if(doGenJets_) fillGenJetInfo(e);


  if (dumpJets_)        fillJets(e,es);

  hEvents_->Fill(1.5);

  //std::cout<<"Filling tree"<<std::endl;
  tree_->Fill();

}


// void Analyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
// {
//   //The following says we do not know what parameters are allowed so do no validation
//   // Please change this to state exactly what you do use, even if it is no parameters
//   edm::ParameterSetDescription desc;
//   desc.setUnknown();
//   descriptions.addDefault(desc);
// }

DEFINE_FWK_MODULE(Analyzer);
