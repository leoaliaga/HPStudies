
#ifndef HPANALYSIS_HH
#define HPANALYSIS_HH

#include "globals.hh"

//root
#include "TSystem.h"

//G4
#include "G4ios.hh"
#include "G4TrajectoryContainer.hh"
#include "G4ParticleDefinition.hh"
#include "G4Element.hh"

#include <map>
#include <vector>

#include "TrackInfo_t.hh"
#include "HPConfig.hh"

class G4ParticleDefinition;
class G4Step;
class TFile;
class TTree;
class G4Track;
class G4VTrajectory;

class ProdPart;
class HPTuple;

class HPAnalysis
{
public:

  HPAnalysis();
  ~HPAnalysis();
  
  void book(const HPConfig &config);
  void FillNtuple(std::vector<TrackInfo_t>);
  
  void WriteNtuple();
  void GetPrimGenInfo(Double_t enerPrim,G4ParticleDefinition* Part);
  
  static HPAnalysis* getInstance();
  void finish();
  
  
private:
   static HPAnalysis* instance;
   
  char NtupleFileName[50];
  TFile* FileNtuple;  
  TTree* EvtTree;
  TTree* HPSetup;
  
  Double_t enerPrimGen;
  G4ParticleDefinition* particle;
  
  HPTuple* t_hptuple;
  
};

#endif 
