#ifndef MuonIdentification_MuonArbitrationMethods_h
#define MuonIdentification_MuonArbitrationMethods_h

#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"
#include "DataFormats/MuonReco/interface/MuonChamberMatch.h"

#include <utility>

// Author: Jake Ribnik (UCSB)

/// functor predicate for standard library sort algorithm
struct SortMuonSegmentMatches {
  /// constructor takes arbitration type
  SortMuonSegmentMatches(unsigned int flag) { flag_ = flag; }
  /// sorts vector of pairs of chamber and segment pointers
  bool operator()(std::pair<reco::MuonChamberMatch*, reco::MuonSegmentMatch*> p1,
                  std::pair<reco::MuonChamberMatch*, reco::MuonSegmentMatch*> p2) {
    reco::MuonChamberMatch* cm1 = p1.first;
    reco::MuonSegmentMatch* sm1 = p1.second;
    reco::MuonChamberMatch* cm2 = p2.first;
    reco::MuonSegmentMatch* sm2 = p2.second;
    if (flag_ == reco::MuonSegmentMatch::BestInChamberByDX || flag_ == reco::MuonSegmentMatch::BestInStationByDX ||
        flag_ == reco::MuonSegmentMatch::BelongsToTrackByDX){
        if (fabs(sm1->y - cm1->y)>7.5){
          std::cout << "Bad segment: Dy too large" << std::endl;
          return false;
        }
        /*
        if ((sm1->dXdZ - cm1->dXdZ)>0){
        if (fabs(sm1->dXdZ - cm1->dXdZ)>0.05){
          std::cout << "Bad segment: D(dX/dZ) too large" << std::endl;
          return false;
        }
      }
        else if ((sm1->dXdZ - cm1->dXdZ)<0){
        if (fabs(sm1->dXdZ - cm1->dXdZ)>0.1){
          return false;
        }
      }
          return fabs(sm1->x - cm1->x) < fabs(sm2->x - cm2->x);
          //*/
          // pesi normalizzati
      else{
          const double wx  = 0.5;   // esempio: 70% su Δx
          const double wdx = 1-wx;   // 30% su Δ(dX/dZ)

          // differenze (pull)
          double dx1      = sm1->x    - cm1->x;
          double ddxdz1   = sm1->dXdZ - cm1->dXdZ;
          double dx2      = sm2->x    - cm2->x;
          double ddxdz2   = sm2->dXdZ - cm2->dXdZ;

          // incertezze
          double sigma_x    = 1.5;
          //double sigma_dxdz = 0.04;

          // default: risoluzione migliore
          double sigma_dxdz = 0.0005;

          // pull
          double pull_x1    = std::abs(dx1    / sigma_x);
          double pull_dxdz1 = std::abs(ddxdz1 / sigma_dxdz);
          double pull_x2    = std::abs(dx2    / sigma_x);
          double pull_dxdz2 = std::abs(ddxdz2 / sigma_dxdz);

          // discriminanti
          double D1 = std::sqrt(wx * pull_x1 * pull_x1 + wdx * pull_dxdz1 * pull_dxdz1);
          double D2 = std::sqrt(wx * pull_x2 * pull_x2 + wdx * pull_dxdz2 * pull_dxdz2);

          return D1 < D2;
          //return 0.5*fabs(sm1->x - cm1->x)/fabs(sm1->x)+0.5*fabs(sm1->dXdZ - cm1->dXdZ)/fabs(sm1->dXdZ)<0.5*fabs(sm2->x - cm2->x)/fabs(sm2->x)+0.5*fabs(sm2->dXdZ - cm2->dXdZ)/fabs(sm2->dXdZ)
          //return (fabs(sm1->x - cm1->x) < fabs(sm2->x - cm2->x))&&(fabs(sm1->dXdZ - cm1->dXdZ) < fabs(sm2->dXdZ - cm2->dXdZ));
        //return sqrt(pow((sm1->x - cm1->x)/0.1, 2) + pow((sm1->y - cm1->y)/10, 2)) <
        //     sqrt(pow((sm2->x - cm2->x)/0.1, 2) + pow((sm2->y - cm2->y)/10, 2));
        //*/
        //else return fabs(sm1->x - cm1->x) < fabs(sm2->x - cm2->x);
       }
    }
    if (flag_ == reco::MuonSegmentMatch::BestInChamberByDR || flag_ == reco::MuonSegmentMatch::BestInStationByDR ||
        flag_ == reco::MuonSegmentMatch::BelongsToTrackByDR) {
      if ((!sm1->hasZed()) || (!sm2->hasZed()))  // no y information so return dx
        return fabs(sm1->x - cm1->x) < fabs(sm2->x - cm2->x);
      return sqrt(pow(sm1->x - cm1->x, 2) + pow(sm1->y - cm1->y, 2)) <
             sqrt(pow(sm2->x - cm2->x, 2) + pow(sm2->y - cm2->y, 2));
    }
    if (flag_ == reco::MuonSegmentMatch::BestInChamberByDXSlope ||
        flag_ == reco::MuonSegmentMatch::BestInStationByDXSlope ||
        flag_ == reco::MuonSegmentMatch::BelongsToTrackByDXSlope)
      return fabs(sm1->dXdZ - cm1->dXdZ) < fabs(sm2->dXdZ - cm2->dXdZ);
    if (flag_ == reco::MuonSegmentMatch::BestInChamberByDRSlope ||
        flag_ == reco::MuonSegmentMatch::BestInStationByDRSlope ||
        flag_ == reco::MuonSegmentMatch::BelongsToTrackByDRSlope) {
      if ((!sm1->hasZed()) || (!sm2->hasZed()))  // no y information so return dx
        return fabs(sm1->dXdZ - cm1->dXdZ) < fabs(sm2->dXdZ - cm2->dXdZ);
      return sqrt(pow(sm1->dXdZ - cm1->dXdZ, 2) + pow(sm1->dYdZ - cm1->dYdZ, 2)) <
             sqrt(pow(sm2->dXdZ - cm2->dXdZ, 2) + pow(sm2->dYdZ - cm2->dYdZ, 2));
    }

    return false;  // is this appropriate? fix this
  }

  unsigned int flag_;
};

#endif
