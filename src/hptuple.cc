
#include "hptuple.hh"
#include "iostream"

ClassImp(ProdPart)
ClassImp(HPTuple)

ProdPart::ProdPart(){
  
}
ProdPart::ProdPart(int pdg_, double vtx_[], double mom_[], double xf_, double pt_, bool ff_){
  pdg = pdg_;
  vtx[0] = vtx_[0];
  vtx[1] = vtx_[1];
  vtx[2] = vtx_[2];
  mom[0] = mom_[0];
  mom[1] = mom_[1];
  mom[2] = mom_[2];
  mom[3] = mom_[3];
  xf = xf_;
  pt = pt_;
  ff = ff_;  
}
ProdPart::~ProdPart(){
}
//////////
HPTuple::HPTuple(){
}
HPTuple::~HPTuple(){
}
