#include "utadk2nu.h"

#include <iostream>
#include <iomanip>
#include <sstream>

#include "TMath.h"

//-----------------------------------------------------------------------------
ClassImp(uta::NuRay)
uta::NuRay::NuRay() { clear(); }
uta::NuRay::~NuRay() { ; }
uta::NuRay::NuRay(double pxi, double pyi, double pzi, double Ei, double wgti)
  : px(pxi), py(pyi), pz(pzi), E(Ei), wgt(wgti) { ; }
void uta::NuRay::clear(const std::string &){ 
  px = py = pz = E = wgt = 0;
}

//-----------------------------------------------------------------------------
ClassImp(uta::Decay)
uta::Decay::Decay() { clear(); }
uta::Decay::~Decay() { ; }
void uta::Decay::clear(const std::string &)
{ 
  norig = ndecay = ntype = -9999999;
  vx = vy = vz = pdpx = pdpy = pdpz = -9999999.;
  ppdxdz = ppdydz = pppz = ppenergy = -9999999.;
  ppmedium = -9999999.; 
  ptype = -9999999;
  muparpx = muparpy = muparpz = mupare = -9999999.;
  necm = nimpwt = -9999999.;

}

//-----------------------------------------------------------------------------
ClassImp(uta::Ancestor)
uta::Ancestor::Ancestor() { clear(); }
uta::Ancestor::~Ancestor() { ; }
void uta::Ancestor::clear(const std::string &){
  pdg = 0;  // 0 is not a legal PDG code
  startx = starty = startz = -9999999.;
  startt = 0;  // in case user doesn't set it
  startpx = startpy = startpz = -9999999.;
  stoppx = stoppy = stoppz = -9999999.;
  polx = poly = polz = -9999999.;
#ifdef KEEP_ANCESTOR_PPRODPXYZ
  pprodpx = pprodpy = pprodpz = -9999999.;
#endif
  nucleus = 0;  // not a legal PDG code
  parIndex = -1; // not legal, should be a positive integer
  proc = "<<no-process>>";
  ivol = "<<no-volume>>";
}

void uta::Ancestor::SetStartXYZT(Double_t x, Double_t y, Double_t z, Double_t t)
{ startx = x; starty = y; startz = z; startt = t; }
void uta::Ancestor::SetStartP(Double_t px, Double_t py, Double_t pz)
{ startpx = px; startpy = py; startpz = pz; }
void uta::Ancestor::SetStopP(Double_t px, Double_t py, Double_t pz)
{ stoppx = px; stoppy = py; stoppz = pz; }

#ifdef KEEP_ANCESTOR_PPRODPXYZ
void uta::Ancestor::SetPProdP(Double_t px, Double_t py, Double_t pz)
{ pprodpx = px; pprodpy = py; pprodpz = pz; }
#endif

Double_t uta::Ancestor::r() const 
{ return TMath::Sqrt(startx*startx+starty*starty); }
Double_t uta::Ancestor::startpt() const
{ return TMath::Sqrt(startpx*startpx+startpy*startpy); }
Double_t uta::Ancestor::startp() const
{ return TMath::Sqrt(startpx*startpx+startpy*startpy+startpz*startpz); }
Double_t uta::Ancestor::stoppt() const
{ return TMath::Sqrt(stoppx*stoppx+stoppy*stoppy); }
Double_t uta::Ancestor::stopp() const
{ return TMath::Sqrt(stoppx*stoppx+stoppy*stoppy+stoppz*stoppz); }

// helper function added by Marco
bool uta::Ancestor::IsInTarget(){
  TString volumeString = ivol;
  if(volumeString.CompareTo("TargetM")==0) return true;
  if(volumeString.CompareTo("Budal_H")==0) return true;
  if(volumeString.CompareTo("Budal_V")==0) return true;
  if(volumeString.CompareTo("TGT1")==0)    return true;
  if(volumeString.CompareTo("TGT1001")==0) return true;
  if(volumeString.CompareTo("TGT1002")==0) return true;
  if(volumeString.CompareTo("TGT1003")==0) return true;
  if(volumeString.CompareTo("TGT1004")==0) return true;
  if(volumeString.CompareTo("TGT1005")==0) return true;
  if(volumeString.CompareTo("TGT1006")==0) return true;
  if(volumeString.CompareTo("TGT1007")==0) return true;
  if(volumeString.CompareTo("TGT1008")==0) return true;
  if(volumeString.CompareTo("TGT1009")==0) return true;
  if(volumeString.CompareTo("TGT1010")==0) return true;
  if(volumeString.CompareTo("TGT1011")==0) return true;
  if(volumeString.CompareTo("TGT1012")==0) return true;
  if(volumeString.CompareTo("TGT1013")==0) return true;
  if(volumeString.CompareTo("TGT1014")==0) return true;
  if(volumeString.CompareTo("TGT1015")==0) return true;
  if(volumeString.CompareTo("TGT1016")==0) return true;
  if(volumeString.CompareTo("TGT1017")==0) return true;
  if(volumeString.CompareTo("TGT1018")==0) return true;
  if(volumeString.CompareTo("TGT1019")==0) return true;
  if(volumeString.CompareTo("TGT1020")==0) return true;
  if(volumeString.CompareTo("TGT1021")==0) return true;
  if(volumeString.CompareTo("TGT1022")==0) return true;
  if(volumeString.CompareTo("TGT1023")==0) return true;
  if(volumeString.CompareTo("TGT1024")==0) return true;
  if(volumeString.CompareTo("TGT1025")==0) return true;
  if(volumeString.CompareTo("TGT1026")==0) return true;
  if(volumeString.CompareTo("TGT1027")==0) return true;
  if(volumeString.CompareTo("TGT1028")==0) return true;
  if(volumeString.CompareTo("TGT1029")==0) return true;
  if(volumeString.CompareTo("TGT1030")==0) return true;
  if(volumeString.CompareTo("TGT1031")==0) return true;
  if(volumeString.CompareTo("TGT1032")==0) return true;
  if(volumeString.CompareTo("TGT1033")==0) return true;
  if(volumeString.CompareTo("TGT1034")==0) return true;
  if(volumeString.CompareTo("TGT1035")==0) return true;
  if(volumeString.CompareTo("TGT1036")==0) return true;
  if(volumeString.CompareTo("TGT1037")==0) return true;
  if(volumeString.CompareTo("TGT1038")==0) return true;
  if(volumeString.CompareTo("TGT1039")==0) return true;
  if(volumeString.CompareTo("TGT1040")==0) return true;
  if(volumeString.CompareTo("TGT1041")==0) return true;
  if(volumeString.CompareTo("TGT1042")==0) return true;
  if(volumeString.CompareTo("TGT1043")==0) return true;
  if(volumeString.CompareTo("TGT1044")==0) return true;
  if(volumeString.CompareTo("TGT1045")==0) return true;
  if(volumeString.CompareTo("TGT1046")==0) return true;
  if(volumeString.CompareTo("TGT1047")==0) return true;
  if(volumeString.CompareTo("TGT1048")==0) return true;
  if(volumeString.CompareTo("TargetB")==0) return true;
  if(volumeString.CompareTo("Outside")==0) return true;
  if(volumeString.CompareTo("CasingW")==0) return true;
  if(volumeString.CompareTo("InsideC")==0) return true;
  if(volumeString.CompareTo("TargetD")==0) return true;
  if(volumeString.CompareTo("Target001")==0) return true;
  if(volumeString.CompareTo("TargetU")==0) return true;
  if(volumeString.CompareTo("Target002")==0) return true;
  if(volumeString.CompareTo("Target003")==0) return true;
  if(volumeString.CompareTo("Pressin")==0) return true;
  if(volumeString.CompareTo("Cooling")==0) return true;
  if(volumeString.CompareTo("Cooli001")==0) return true;
  if(volumeString.CompareTo("Cooli002")==0) return true;
  return false;
}

#ifdef KEEP_ANCESTOR_PPRODPXYZ
Double_t uta::Ancestor::pprodpt() const
{ return TMath::Sqrt(pprodpx*pprodpx+pprodpy*pprodpy); }
Double_t uta::Ancestor::pprodp() const
{ return TMath::Sqrt(pprodpx*pprodpx+pprodpy*pprodpy+pprodpz*pprodpz); }
#endif


//-----------------------------------------------------------------------------
ClassImp(uta::TgtExit)
uta::TgtExit::TgtExit() { clear(); }
uta::TgtExit::~TgtExit() { ; }
void uta::TgtExit::clear(const std::string &){
  tvx = tvy = tvz = tpx = tpy = tpz = -9999999.;
  tptype = tgen = -9999999;
}

//-----------------------------------------------------------------------------
ClassImp(uta::Traj)
uta::Traj::Traj() { clear(); }
uta::Traj::~Traj() { ; }
void uta::Traj::clear(const std::string &){
  trkx = trky = trkz = trkpx = trkpy = trkpz = -9999999.;
}

//-----------------------------------------------------------------------------
ClassImp(uta::Dk2Nu)
uta::Dk2Nu::Dk2Nu() { clear(); }
uta::Dk2Nu::~Dk2Nu() { ; }
void uta::Dk2Nu::clear(const std::string &){ 
  job    = -9999999;
  potnum = 0;
  nuray.clear();     /// clear the vector
  decay.clear();     /// clear the object
  ancestor.clear();  /// clear the vector

  ppvx  = -9999999.;
  ppvy  = -9999999.;
  ppvz  = -9999999.;

  tgtexit.clear();  /// clear the object
  traj.clear();     /// clear the vector

  flagbits = 0;
  vint.clear();     /// clear the vector
  vdbl.clear();     /// clear the vector

}

size_t uta::Dk2Nu::indxnu() const { return ancestor.size()-1; }
size_t uta::Dk2Nu::indxp() const { return ancestor.size()-2; }
size_t uta::Dk2Nu::indxgp() const { return ancestor.size()-3; }

