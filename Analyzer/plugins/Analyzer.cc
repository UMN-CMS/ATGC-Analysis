// -*- C++ -*-
//
// Package:    Analyser/Analyzer
// Class:      Analyzer
// 
/**\class Analyzer Analyzer.cc Analyser/Analyzer/plugins/Analyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Shilpi Jain
//         Created:  Mon, 29 Jan 2018 00:08:57 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/EcalDigi/interface/EBSrFlag.h"
#include "DataFormats/EcalDigi/interface/EESrFlag.h"

#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"

#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFLayer.h"
//#include "CalibCalorimetry/EcalTPGTools/interface/EcalReadoutTools.h"

#include "CondFormats/DataRecord/interface/GBRDWrapperRcd.h"

#include "Geometry/CaloTopology/interface/EcalTrigTowerConstituentsMap.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"


#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateEGammaExtra.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateEGammaExtraFwd.h"

#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"

#include "DataFormats/PatCandidates/interface/Photon.h"

#include "CommonTools/CandAlgos/interface/ModifyObjectValueBase.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "ATGC-Analysis/Analyzer/interface/GenParticleParentage.h"


//
// class declaration
//

#include "TTree.h"
#include <iostream>
#include <vector>

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class Analyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit Analyzer(const edm::ParameterSet&);
      ~Analyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
  //edm::EDGetTokenT<reco::PhotonCollection> recophotonCollection_;
  edm::EDGetTokenT<edm::View<pat::Electron> > recoelectronCollection_;
  edm::EDGetTokenT<std::vector<reco::GenParticle> > genParticlesCollection_;
  
  bool doGenParticles_;
  bool doRecoEle_;
  
  std::shared_ptr<ModifyObjectValueBase> regress_;

  TTree *tree_;
  std::vector<float> egmeleEcalE;
  std::vector<float> egmeleSCEta;
  std::vector<float> egmeleSCPhi;
  std::vector<float> egmeleSCE;
  std::vector<float> egmeleEta;
  std::vector<float> egmelePhi;
  std::vector<float> egmelerawSCE;
  std::vector<float> egmeleGenE;
  std::vector<float> egmeleGenEta;
  std::vector<float> egmeleGenPhi;
  std::vector<float> egmeleGenPt;


  Int_t            nMC_;
  std::vector<int>      mcPID;
  std::vector<float>    mcVtx;
  std::vector<float>    mcVty;
  std::vector<float>    mcVtz;
  std::vector<float>    mcPt;
  std::vector<float>    mcMass;
  std::vector<float>    mcEta;
  std::vector<float>    mcPhi;
  std::vector<float>    mcE;
  std::vector<float>    mcEt;
  std::vector<int>      mcGMomPID;
  std::vector<int>      mcMomPID;
  std::vector<float>    mcMomPt;
  std::vector<float>    mcMomMass;
  std::vector<float>    mcMomEta;
  std::vector<float> mcMomPhi;

  int run;
  int lumi;
  int event;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
Analyzer::Analyzer(const edm::ParameterSet& iConfig):
  regress_(0)
{
  //now do what ever initialization is needed
  usesResource("TFileService");
  edm::Service<TFileService> fs;
  tree_ = fs->make<TTree>("EventTree", "EGM tree");
  
  //recophotonCollection_ = consumes<reco::ElectronCollection> (iConfig.getParameter<edm::InputTag>("recoElectronSrc"));
  recoelectronCollection_ = consumes<edm::View<pat::Electron> > (iConfig.getParameter<edm::InputTag>("recoElectronSrc"));

  doGenParticles_ = iConfig.getParameter<bool>("doGenParticles");
  doRecoEle_ = iConfig.getParameter<bool>("doRecoEle");
  genParticlesCollection_ = consumes<std::vector<reco::GenParticle> > (iConfig.getParameter<edm::InputTag>("genParticleSrc"));
  
  edm::ConsumesCollector sumes(consumesCollector());
  
  regress_.reset(ModifyObjectValueFactory::get()->create( "EGExtraInfoModifierFromDB", iConfig.getParameter<edm::ParameterSet>("regressionConfig") )); 
  regress_->setConsumes(sumes);

  tree_->Branch("run",&run);
  tree_->Branch("lumi",&lumi);
  tree_->Branch("event",&event);

  if(doRecoEle_){
    tree_->Branch("egmeleEcalE",&egmeleEcalE);
    tree_->Branch("egmeleSCEta",&egmeleSCEta);   
    tree_->Branch("egmeleSCPhi",&egmeleSCPhi);   
    tree_->Branch("egmeleSCE",&egmeleSCE);   
  tree_->Branch("egmelerawSCE",&egmelerawSCE); 
  tree_->Branch("egmeleEta",&egmeleEta);   
  tree_->Branch("egmelePhi",&egmelePhi);   


  tree_->Branch("egmeleGenE",&egmeleGenE);   
  tree_->Branch("egmeleGenPt",&egmeleGenPt);   
  tree_->Branch("egmeleGenPhi",&egmeleGenPhi);   
  tree_->Branch("egmeleGenEta",&egmeleGenEta);   
  }

  tree_->Branch("nMC",          &nMC_);
  tree_->Branch("mcPID",        &mcPID);
  tree_->Branch("mcVtx",        &mcVtx);
  tree_->Branch("mcVty",        &mcVty);
  tree_->Branch("mcVtz",        &mcVtz);
  tree_->Branch("mcPt",         &mcPt);
  tree_->Branch("mcMass",       &mcMass);
  tree_->Branch("mcEta",        &mcEta);
  tree_->Branch("mcPhi",        &mcPhi);
  tree_->Branch("mcE",          &mcE);
  tree_->Branch("mcEt",         &mcEt);
  tree_->Branch("mcMomPID",     &mcMomPID);
  tree_->Branch("mcMomPt",      &mcMomPt);
  tree_->Branch("mcMomMass",    &mcMomMass);
  tree_->Branch("mcMomEta",     &mcMomEta);
  tree_->Branch("mcMomPhi", &mcMomPhi);
  
}


Analyzer::~Analyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
Analyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  
   using namespace edm;
   using namespace std;


   //std::cout<<"Inside analyse"<<std::endl;

   egmeleEcalE.clear();
   egmeleSCE.clear();
   egmelerawSCE.clear();
   egmeleSCEta.clear();
   egmeleSCPhi.clear();
   egmelePhi.clear();
   egmeleEta.clear();
   egmeleGenE.clear();
   egmeleGenEta.clear();
   egmeleGenPhi.clear();
   egmeleGenPt.clear();

   mcPID       .clear();
   mcVtx       .clear();
   mcVty       .clear();
   mcVtz       .clear();
   mcPt        .clear();
   mcMass      .clear();
   mcEta       .clear();
   mcPhi       .clear();
   mcE         .clear();
   mcEt        .clear();
   mcGMomPID   .clear();
   mcMomPID    .clear();
   mcMomPt     .clear();
   mcMomMass   .clear();
   mcMomEta    .clear();
   mcMomPhi    .clear();
   //mcIndex     .clear();
   nMC_ = 0;
   
   run = iEvent.id().run();
   event = iEvent.id().event();
   lumi = iEvent.luminosityBlock();

   if(doRecoEle_){
     edm::Handle<edm::View<pat::Electron> > recoElectronHandle;
     iEvent.getByToken(recoelectronCollection_, recoElectronHandle);
     
     //std::cout<<"Taking handle"<<std::endl;
     
     if(!recoElectronHandle.isValid()){
       
       std::cout<<"electron handle is not valid"<<endl;
       return;
     }


     regress_->setEvent(iEvent);
     //cout<<"Set event"<<endl;
     regress_->setEventContent(iSetup);
     //cout<<"Set eventContent"<<endl;
     
     if (recoElectronHandle.isValid() && recoElectronHandle->size()>1) {
       
       //std::cout<<"no. of electrons "<<recoElectronHandle->size()<<std::endl;
       
       for (edm::View<pat::Electron>::const_iterator iEle = recoElectronHandle->begin(); iEle != recoElectronHandle->end(); ++iEle) {
	 
	 
	 const pat::Electron& electron=(*iEle);
	 
	 //reco::Electron electronRef(electron);
	 
	 double E = iEle->energy();
	 double scRaw = iEle->superCluster()->rawEnergy();
	 double corrE = iEle->correctedEcalEnergy();
	 double scE = iEle->superCluster()->energy();
	 double scEta = iEle->superCluster()->eta();
	 double scPhi = iEle->superCluster()->phi();
	 
	 egmeleEcalE.push_back(corrE);
	 egmelerawSCE.push_back(scRaw);
	 egmeleSCE.push_back(scE);
	 egmeleSCEta.push_back(scEta);
	 egmeleSCPhi.push_back(scPhi);
	 egmelePhi.push_back(iEle->phi());
	 egmeleEta.push_back(iEle->eta());
	 
	 if(iEle->genLepton() != 0) {
	   egmeleGenE.push_back(iEle->genLepton()->energy());
	   egmeleGenEta.push_back(iEle->genLepton()->eta());
	   egmeleGenPhi.push_back(iEle->genLepton()->phi());
	 egmeleGenPt.push_back(iEle->genLepton()->pt());
	 }
	 
	 else{
	   egmeleGenE.push_back(-999);
	   egmeleGenEta.push_back(-999);
	   egmeleGenPhi.push_back(-999);
	   egmeleGenPt.push_back(-999);
	 }
	 
	 
       }//for (edm::View<pat::Electron>::const_iterator iEle = recoElectronHandle->begin(); iEl...)
       
     }//if (recoElectronHandle.isValid() && recoElectronHandle->size()>1)
   }// if(doRecoEle_)

   
   ///store gen info only when there are atleast 2 electrons in the event
   
   std::cout<<"================================="<<std::endl;

     if(doGenParticles_){
       edm::Handle<std::vector<reco::GenParticle> > genParticlesHandle;
       iEvent.getByToken(genParticlesCollection_, genParticlesHandle);
       
       for (std::vector<reco::GenParticle>::const_iterator ip = genParticlesHandle->begin(); ip != genParticlesHandle->end(); ++ip) {
	 //genIndex++;
	 
	 int status = ip->status();
	 
       // keep non-FSR photons with pT > 5.0 and all leptons with pT > 3.0;
	 bool photonOrLepton =
	   (ip->pdgId() == 22 && (ip->isPromptFinalState() || ip->isLastCopy())) ||
	   (status == 1 && abs(ip->pdgId()) == 11 && (ip->isPromptFinalState() || ip->isLastCopy()));
	 //|| (status == 1 && abs(ip->pdgId()) == 13 && (ip->isPromptFinalState() || ip->isLastCopy())) ||
	 //(status == 1 && (abs(ip->pdgId()) == 12 || abs(ip->pdgId()) == 14 || abs(ip->pdgId()) == 16)) ||
	 //(status == 1 && ( abs(ip->pdgId()) >= 11 && abs(ip->pdgId()) <= 16 ) && ip->pt() > 3.0)  ||
	 //(status < 10 && abs(ip->pdgId()) == 15 && ip->pt() > 3.0);
	 


	 
	 
	 int mcGMomPID_ = -999;
	 int mcMomPID_  = -999;
	 float mcMomPt_    = -999.;
	 float mcMomMass_  = -999.;
	 float mcMomEta_   = -999.;
	 float mcMomPhi_   = -999.;
	 
	 if (photonOrLepton) {
	   
	   const reco::Candidate *p = (const reco::Candidate*)&(*ip);
	   if (!p->mother()) continue;
	   
	 mcPID    .push_back(p->pdgId());
	 mcVtx    .push_back(p->vx());
	 mcVty    .push_back(p->vy());
	 mcVtz    .push_back(p->vz());
	 mcPt     .push_back(p->pt());
	 mcMass   .push_back(p->mass());
	 mcEta    .push_back(p->eta());
	 mcPhi    .push_back(p->phi());
	 mcE      .push_back(p->energy());
	 mcEt     .push_back(p->et());
	 //mcStatus .push_back(p->status());


	 
	 reco::GenParticleRef partRef = reco::GenParticleRef(genParticlesHandle,
							     ip-genParticlesHandle->begin());
	 genpartparentage::GenParticleParentage particleHistory(partRef);
	 
	 if ( particleHistory.hasRealParent() ) {
	   reco::GenParticleRef momRef = particleHistory.parent();
	   if ( momRef.isNonnull() && momRef.isAvailable() ) {
	     mcMomPID_  = momRef->pdgId();
	     mcMomPt_   = momRef->pt();
	     mcMomMass_ = momRef->mass();
	     mcMomEta_  = momRef->eta();
	     mcMomPhi_ = momRef->phi();
	     mcMomPID.push_back(mcMomPID_);
	     mcMomPt.push_back(mcMomPt_);
	     mcMomMass.push_back(mcMomMass_);
	     mcMomEta.push_back(mcMomEta_);
	     mcMomPhi.push_back(mcMomPhi_);
	 

	   }//if ( momRef.isNonnull() && momRef.isAvailable() )
	 }//if ( particleHistory.hasRealParent() )


	 nMC_++;
	 
       }//if (photonOrLepton)
	 std::cout<<"ID : status : isPromptFinalState: isFinalCopy : mothID "<<ip->pdgId()<<" "<<status<<" "<<ip->isPromptFinalState()<<" "<<ip->isLastCopy()<<" "<<mcMomPID_<<std::endl;  
     }//for (vector<reco::GenParticle>::const_iterator ip = genParticlesHandle->begin(); ip != genParticlesHandl
     }//if(doGenParticles_)

     tree_->Fill();

}

// ------------ method called once each job just before starting event loop  ------------
void 
Analyzer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
Analyzer::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
Analyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(Analyzer);
