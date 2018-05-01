/*
  \class    modules::RochesterPATMuonCorrector
  \brief    Applies Rochester corrections to muons
  \author   Toni Sculac
*/


#include <FWCore/Framework/interface/Frameworkfwd.h>
#include <FWCore/Framework/interface/EDProducer.h>
#include <FWCore/Framework/interface/Event.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>
#include <FWCore/Framework/interface/ESHandle.h>

#include <DataFormats/PatCandidates/interface/Muon.h>

// Rochester Muon Corrections
#include <ZZAnalysis/AnalysisStep/plugins/RoccoR.h>


#include "TLorentzVector.h"
#include "TRandom3.h"


#include <vector>
#include <string>
#include <sstream>

using namespace edm;
using namespace std;
using namespace reco;


class RochesterPATMuonCorrector : public edm::EDProducer {
 public:
  /// Constructor
  explicit RochesterPATMuonCorrector(const edm::ParameterSet&);
	
  /// Destructor
  ~RochesterPATMuonCorrector(){
    delete calibrator;
  };

 private:
  virtual void beginJob(){};
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob(){};

  edm::EDGetTokenT<vector<pat::Muon> > muonToken_;
  string identifier_;
  bool isMC_;
  bool isSync_;

  RoccoR* calibrator;
  TRandom3* rgen_;

};


RochesterPATMuonCorrector::RochesterPATMuonCorrector(const edm::ParameterSet& iConfig) :
  muonToken_(consumes<vector<pat::Muon> >(iConfig.getParameter<edm::InputTag>("src"))),
  identifier_(iConfig.getParameter<string>("identifier")),
  isMC_(iConfig.getParameter<bool>("isMC")),
  isSync_(iConfig.getParameter<bool>("isSynchronization")),
  calibrator(0),
  rgen_(0)
{
  stringstream ss;
  ss << "ZZAnalysis/AnalysisStep/data/RochesterCorrections/" << identifier_ << ".txt";
  string path_string = ss.str();
  edm::FileInPath corrPath("ZZAnalysis/AnalysisStep/data/RochesterCorrections/"+identifier_+".txt");
	
  calibrator = new RoccoR(corrPath.fullPath());
  rgen_ = new TRandom3(0);
	
  produces<pat::MuonCollection>();
}


void
RochesterPATMuonCorrector::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // Input collection
  edm::Handle<vector<pat::Muon> > muonHandle;
  iEvent.getByToken(muonToken_, muonHandle);
  const vector<pat::Muon> *inputMuons = muonHandle.product();
	
  // Output collection
  auto outputMuons = std::make_unique<vector<pat::Muon> >();

  TLorentzVector p4;
	
  for (unsigned i=0; i<inputMuons->size(); ++i) {

    pat::Muon mu = inputMuons->at(i);
	 
    double oldpt=mu.pt();
    double newpt=oldpt;
    double oldpterr=mu.muonBestTrack()->ptError();
    double newpterr=oldpterr;
    int nl;
    auto gen_particle = mu.genParticle();
    double scale_factor;
    double scale_factor_error = 0.;
    double u1 = rgen_->Rndm();
    double u2 = rgen_->Rndm();
	
	 if(isSync_) {u1 = 1.; u2 = 1.;}
	 
	  

    if (calibrator != 0  && mu.muonBestTrackType() == 1 && oldpt <= 200.)
    {
		nl = mu.track()->hitPattern().trackerLayersWithMeasurement();
		
      if(isMC_ && nl > 5)//Protection against muons with low number of layers, they are not used in the analysis anyway as we apply thight muon ID
      {
			
			/// ====== ON MC (correction plus smearing) =====
			if ( gen_particle != 0)
			{
				scale_factor = calibrator->kScaleFromGenMC(mu.charge(), oldpt, mu.eta(), mu.phi(), nl, gen_particle->pt(), u1);
				scale_factor_error = calibrator->kScaleFromGenMCerror(mu.charge(), oldpt, mu.eta(), mu.phi(), nl, gen_particle->pt(), u1);
				
			}
			else
			{
				scale_factor = calibrator->kScaleAndSmearMC(mu.charge(), oldpt, mu.eta(), mu.phi(), nl, u1, u2);
				scale_factor_error = calibrator->kScaleAndSmearMCerror(mu.charge(), oldpt, mu.eta(), mu.phi(), nl, u1, u2);
				
			}
			
			newpt = oldpt*scale_factor;
			newpterr = newpt*scale_factor_error;
      }
		 
      else if(!isMC_ && nl > 5)
      {
			/// ====== ON DATA (correction only) =====
			if(mu.pt()>2.0 && fabs(mu.eta())<2.4)
			{
			  scale_factor = calibrator->kScaleDT(mu.charge(), oldpt, mu.eta(), mu.phi());
			  scale_factor_error = calibrator->kScaleDTerror(mu.charge(), oldpt, mu.eta(), mu.phi());
			}
			else
			{
			  // keep old values
			  scale_factor = 1.;
			  scale_factor_error = 0.;
			}
			
			newpt = oldpt*scale_factor;
			newpterr = newpt*scale_factor_error;
      }
    }

    p4.SetPtEtaPhiM(newpt, mu.eta(), mu.phi(), mu.mass());
    mu.setP4(reco::Particle::PolarLorentzVector(p4.Pt(), p4.Eta(), p4.Phi(), mu.mass()));
    mu.addUserFloat("correctedPtError",newpterr);
	 mu.addUserFloat("scale_unc",1. + scale_factor_error);
    mu.addUserFloat("smear_unc",0.);
	  
    outputMuons->push_back(mu);
  }

  iEvent.put(std::move(outputMuons));

}


#include <FWCore/Framework/interface/MakerMacros.h>
DEFINE_FWK_MODULE(RochesterPATMuonCorrector);
