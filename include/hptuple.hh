/**
 * \class HPTuple
 * \file  hptuple.h
 *
 * \brief A class that defines the "HPTuple" object used as the 
 *        branch for a TTree for the output of events of g4hp
 */

#ifndef HPTUPLE_H
#define HPTUPLE_H

#include "TROOT.h"
#include "TObject.h"
#include "Rtypes.h"

#include <vector>
#include <string>

#define HPTUPLEVER 1   // KEEP THIS UP-TO-DATE!  increment for each change

/**
 *============================================================================
 *  Produced particle
 */
class ProdPart{

 public:
  Int_t pdg;
  Double_t vtx[3]; //Initial position of the track
  Double_t mom[4]; //4-momentum. P[maxPart][3] is the energy
  Double_t xf; 
  Double_t pt; 
  Bool_t ff;
  
 public:
  ProdPart();
  ProdPart(int pdg_, double vtx_[], double mom_[], double xf_, double pt_, bool ff_);
  virtual     ~ProdPart();
  
 private:
  ClassDef(ProdPart,HPTUPLEVER)
    };

class HPTuple{

 public:
    HPTuple();
    virtual     ~HPTuple();
    
    std::vector<ProdPart> prodpart;
  
  private:
    ClassDef(HPTuple,HPTUPLEVER)
    
      }; 
#endif   // HPTUPLE_H
