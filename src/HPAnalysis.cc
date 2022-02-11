
#include "HPAnalysis.hh"

#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <stdlib.h>

//Root 
#include <TFile.h>          
#include <TTree.h>

//GEANT4 
#include "globals.hh"
#include "G4ios.hh"
#include "G4Track.hh"
#include "G4SteppingManager.hh"
#include "G4ThreeVector.hh"
#include "G4TrajectoryContainer.hh"
#include "G4RunManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "G4Run.hh"
#include "G4Proton.hh"

#include "hptuple.hh"

using namespace std;

HPAnalysis* HPAnalysis::instance = 0;

HPAnalysis::HPAnalysis() :t_hptuple(0)
{}

HPAnalysis::~HPAnalysis()
{}

HPAnalysis* HPAnalysis::getInstance()
{
  if (instance == 0) instance = new HPAnalysis;
  return instance;
}

void HPAnalysis::book(const HPConfig &config)
{
  FileNtuple = new TFile((config.getOutputDir() + "/" + config.getOutputFile()).c_str(), 
			 "RECREATE",
			 "hadron from h+C ntuple");   

  EvtTree = new TTree("hAinfoTree","g4hp info from h+A");
  
  EvtTree->Branch("hAinfo","HPTuple",&t_hptuple,32000,99); 
  HPSetup = config.createTree();
  
}

void HPAnalysis::finish()
{
    FileNtuple->cd(); 
    
    EvtTree->Write();
    HPSetup->Write();

    FileNtuple->Close();
    delete FileNtuple;
}

void HPAnalysis::FillNtuple(std::vector<TrackInfo_t> trackInfoVec){

  std::vector<ProdPart> tvec_prodpart;
  tvec_prodpart.clear();
  
  Double_t XF,PT,Ecm,PL,beta,gamma,Pxx,Pyy,Pzz,PartE;
  Double_t BeamEnergy = enerPrimGen; //got from Primary Generator
  
  Double_t massProton    = CLHEP::proton_mass_c2;
  Double_t massPionPlus  = G4PionPlus::PionPlus()->GetPDGMass();
  Double_t massPionMinus = G4PionMinus::PionMinus()->GetPDGMass();

  Double_t massInc = 0;  
  G4String pname   = particle->GetParticleName();
  if( pname == "proton" ) massInc = massProton;
  else if( pname == "pi+"    ) massInc = massPionPlus;
  else if( pname == "pi-"    ) massInc = massPionMinus;
  else exit (1);

  std::vector<TrackInfo_t>::iterator iteTrackInfo = trackInfoVec.begin();
  for(;iteTrackInfo != trackInfoVec.end();iteTrackInfo++){  
    PartE = (*iteTrackInfo).Mom.E();
    Pxx = (*iteTrackInfo).Mom.X();
    Pyy = (*iteTrackInfo).Mom.Y();
    Pzz = (*iteTrackInfo).Mom.Z();
    //
    PT    = sqrt(pow(Pxx,2.0)+pow(Pyy,2.0));
    Ecm   = sqrt(pow(massInc,2.0) + pow(massProton,2.0) + 2.*BeamEnergy*massProton);
    gamma = (BeamEnergy + massProton) / Ecm;
    beta  = sqrt(pow(gamma,2)-1) / gamma;
    PL    = gamma*(Pzz-beta*PartE);    
    XF    = 2.*PL/Ecm;
    //
    double tpos[3] = {(*iteTrackInfo).Pos.X(), (*iteTrackInfo).Pos.Y(), (*iteTrackInfo).Pos.Z()};
    double tmom[4] = {Pxx,Pyy,Pzz,PartE};
    
    ProdPart t_prodpart((*iteTrackInfo).PDGcode, tpos,tmom, XF,PT,(*iteTrackInfo).FromFast);

    tvec_prodpart.push_back(t_prodpart);
  }
  t_hptuple->prodpart = tvec_prodpart;
  
  if (tvec_prodpart.size() > 0) WriteNtuple();
}
void HPAnalysis::WriteNtuple(){    
  EvtTree->Fill();   
}
void HPAnalysis::GetPrimGenInfo(Double_t enerPrim,G4ParticleDefinition* Part){
  enerPrimGen = enerPrim; 
  particle    = Part;
}
