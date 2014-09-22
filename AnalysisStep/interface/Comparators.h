#ifndef COMPARATORS_H
#define COMPARATORS_H

/** \class Comparators
 *
 *  No description available.
 *
 *  $Date: 2012/06/06 00:27:43 $
 *  $Revision: 1.3 $
 *  \author S. Casasso - TORINO
 *  \author N. Amapane - TORINO

 */

#include <DataFormats/PatCandidates/interface/CompositeCandidate.h>

#include <algorithm>

namespace Comparators {

  const float ZmassValue = 91.1876;

  enum ComparatorTypes {byBestZ1bestZ2 = 0, byBestKD=1};

  // Abstract base class
  struct BestCandComparator {
    const pat::CompositeCandidateCollection& candCollection;
    
    BestCandComparator(const pat::CompositeCandidateCollection& candCollection, ComparatorTypes type):
      candCollection(candCollection),
      theType(type)
    {}


    virtual bool operator() (int i, int j){

      const pat::CompositeCandidate& cand_i = candCollection[i];
      const pat::CompositeCandidate& cand_j = candCollection[j];

      if (theType==byBestZ1bestZ2) { // "Legacy" best cand logic	

	float mZ1_i = cand_i.daughter("Z1")->mass();
	float mZ1_j = cand_j.daughter("Z1")->mass();
	if ( fabs(mZ1_i-mZ1_j) < 1e-04 ){ // same Z1: choose the candidate with highest-pT Z2 leptons
	  const reco::Candidate* Z2_i = cand_i.daughter("Z2");
	  const reco::Candidate* Z2_j = cand_j.daughter("Z2");
	  double ptSumZ2_i = Z2_i->daughter(0)->pt() + Z2_i->daughter(1)->pt();
	  double ptSumZ2_j = Z2_j->daughter(0)->pt() + Z2_j->daughter(1)->pt();

	  // cout <<  "Comparator: compare Z2 pTs: " << ptSumZ2_i << " " << ptSumZ2_j << endl;
	  return ( ptSumZ2_i > ptSumZ2_j );
	}
	else { // choose the candidate with Z1 closest to nominal mass 
	  // cout << "Comparator: compare Z1 masses: " << mZ1_i << " " << mZ1_j << endl;
	  return ( fabs(mZ1_i-ZmassValue)<fabs(mZ1_j-ZmassValue) );
	}      

      } //end of byBestZ1bestZ2
  
      else if (theType==byBestKD) {	
	double KD_i = cand_i.userFloat("p0plus_VAJHU")/( cand_i.userFloat("p0plus_VAJHU") + cand_i.userFloat("bkg_VAMCFM") );
	double KD_j = cand_j.userFloat("p0plus_VAJHU")/( cand_j.userFloat("p0plus_VAJHU") + cand_j.userFloat("bkg_VAMCFM") );
      
	return (KD_i>KD_j);
      } // end of byBestKD

      else {
	abort();
      }

    } // end of operator()    
    
    ComparatorTypes theType;
    
  }; // BestCandComparator


  
}// end namespace Comparators
  

#endif