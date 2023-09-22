#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TChain.h"
#include <TRandom.h>
#include <TRandom3.h>
#include <TMatrixD.h>
#include <TDecompChol.h>

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "hptuple.hh"

//constants
const int Nbpp     = 105;
const double ppmin = 0.0;
const double ppmax = 31.;
const int Nban     = 400;
const double anmin = 0.0;
const double anmax = 2000.;

const int Nhad         = 2;
const char* nhad[Nhad] = {"pip","kap"};

int hadidx(int hadpdg);

using namespace std;

//Compiling: g++ -o kaonStudy kaonStudy.C `root-config --cflags --glibs` -I${G4WORKDIR}/include  -L${G4WORKDIR}/build -lg4hpDict 
void kaonStudy(const char* cfIn, const char* cfOut){

  //Inputs
  std::cout<< "kaonStudy() starts"   <<std::endl;
  std::cout<< "cfIn : "<< cfIn  <<std::endl;
  std::cout<< "cfOut: "<< cfOut <<std::endl;
  
  //Analysis histograms
  TH2D* hhad[Nhad];
  for(int i=0;i<Nhad;i++){
    hhad[i] = new TH2D(Form("h%s",nhad[i]),"",Nbpp,ppmin,ppmax,Nban,anmin,anmax);
  }
  
  //Setting branches
   TChain* evts = new TChain("hAinfoTree");
   HPTuple*  hAinfo  = new HPTuple;
   
   std::ifstream list_files(cfIn);
   std::string line;
   while (getline(list_files, line)){
     if(line.find(".root")>1000000)continue;    
     evts->Add(line.c_str());    
   }
   list_files.close();  
   evts->SetBranchAddress("hAinfo",&hAinfo);
  
   int nentries = evts->GetEntries();
   std::cout<<"=> Entries: "<< nentries <<std::endl;
   
   for(int k=0;k<nentries;k++){
     if(k%(nentries/8) == 0) std::cout << "." << std::flush;
     evts->GetEntry(k);

     unsigned int npart = hAinfo->prodpart.size();
     for(unsigned int p=0;p<npart;p++){
       double px = (hAinfo->prodpart[p].mom[0]) / 1000.; //MeV to GeV
       double py = (hAinfo->prodpart[p].mom[1]) / 1000.;
       double pz = (hAinfo->prodpart[p].mom[2]) / 1000.;
       
       double pmom  = sqrt(pow(px,2)+pow(py,2)+pow(pz,2)); 
       double pang  = -10000;
       if(pmom>0){
	 pang = 1000. * acos(pz/pmom); //to mrad
       }
       
       //std::cout<<"=> "<<(hAinfo->prodpart[p].pdg)<<" "<<pmom<<" "<<pang<<std::endl;
       
       int ihad = hadidx(hAinfo->prodpart[p].pdg);
       if(ihad>=0) hhad[ihad]->Fill(pmom,pang);
     } //end loop on particles
     
   } //end loop events
   
   //Storing:
   TFile* fOut = new TFile(cfOut, "RECREATE");
   fOut->cd();
   for(int i=0;i<Nhad;i++){
     hhad[i]->Write();
   }
   
   fOut->Close();  
   std::cout<< "fluxWeights() ends"   <<std::endl;
}
int hadidx(int hadpdg){
  int tidx = -1;
  if(hadpdg == 211)tidx = 0;
  if(hadpdg == 321)tidx = 1;
  return tidx;
}
////////////////////////////////
#ifndef __CINT__
int main(int argc, const char* argv[]){  
  kaonStudy(argv[1],argv[2]);
  return 0;
}
#endif
