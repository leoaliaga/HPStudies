
#include <iostream>
#include <fstream>
#include "TTree.h"
#include "TChain.h"
#include "TBranch.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iomanip>
#include <algorithm>

#include "hptuple.hh"

using namespace std;

//constants:
// NA=6.022e23; // atoms/mol
// cm2_per_mb=1e-27
const double NAval = 6.022e-4;  //NAval = NA * cm2_per_mb
const double dx    = 0.7;       //cm target thickness
const double pival = 3.14159265;//approx

inline Bool_t isMeson (const int &pdg){
  //mesons have 3 digits in the PDG code
  return abs(pdg)>=100 && abs(pdg)<1000;
}
// check if particle == nucleon
inline Bool_t isNucleon (const int &pdg){
  return pdg == 2212 || pdg == 2112;
}

//4 cases of QE for studies
const int Ntyp   = 4;
const char* ntyp[Ntyp] = {"ela_like","qel_like","frag_like","disnuc_like"};
const int Nbins = 1000;
const double minp = 0.;
const double maxp = 32.;

//Compiling: g++ -o QElEvents QElEvents.C `root-config --cflags --glibs` -I${G4WORKDIR}/include  -L${G4WORKDIR}/build -lg4hpDict
void QElEvents();
void QElEvents(const char* infiles, const char* out_histfile){

  // counters
  int nElaEvts  = 0; // 1 nucleon + 1 fragment + 0 mesons
  int nQElEvts  = 0; // 2 nucleons + <= 1 fragment + 0 mesons
  int nProdEvts = 0; // >0 mesons
  int nFragEvts = 0; // >0 nucleons + >1 fragments + 0 mesons
                     // or >2 nucleons + >0 frgaments + 0 mesons
  int nDisNucEvts = 0; // >0 nucleons + 0 fragments + 0 mesons
  
  TFile* foutput = new TFile(out_histfile,"RECREATE");
  TH1D* hlead_nuc[Ntyp];
  TH1D* hoth_nuc[Ntyp];
  for(int i=0;i<Ntyp;i++){
    hlead_nuc[i] = new TH1D(Form("hlead_nuc_%s",ntyp[i]),"",Nbins,minp,maxp);
    hoth_nuc[i]  = new TH1D(Form("hoth_nuc_%s" ,ntyp[i]),"",Nbins,minp,maxp);
  }
  
  TChain* evts  = new TChain("hAinfoTree");
  TChain* setup = new TChain("setup");
  HPTuple*  hAinfo  = new HPTuple;
  std::ifstream ifs;	
  ifs.open(infiles);
  std::string line;
  int counter = 0;
  while (ifs.good()) {
    getline(ifs,line);
    if(line.find(".root")>10000)continue;

    evts->Add(line.c_str());
    std::cout<<"Adding ntuple at : "<<line<<std::endl;

    if(counter==0)setup->Add(line.c_str());
    counter++;
  }
  ifs.close();  
  evts->SetBranchAddress("hAinfo",&hAinfo);
  //Setup:
  double dens, A;
  int POT;
  setup->SetBranchAddress("density",&dens); // g/cc
  setup->SetBranchAddress("A",&A); // g/mol
  setup->SetBranchAddress("nof_events",&POT); // POT
  setup->GetEntry(0);
  double dPOT = double(POT);
  dPOT *= double(counter); //adding all POT per file
  
  double sigma_factor = A/(dens * NAval * dx * dPOT); // mb
  
  std::cout<<"=> sigma_factor "<< sigma_factor <<std::endl;
  
  std::cout<< (evts->GetNtrees()) <<" files were added to the chain"<<std::endl;
  
  //Loop and filling:
  //vector of momenta per events
  std::vector<double> vnucmom;
    
  int nentries  = (int)evts->GetEntries();
  std::cout<<"Entries "<<nentries<<std::endl;;

  for(long int jentry=0;jentry<nentries;jentry++){
     if(jentry%(nentries/10) == 0) std::cout << "." << std::flush;
     int nb = evts->GetEntry(jentry);  
    
    int npart = int(hAinfo->prodpart.size());

    int nMes  = 0; // no. of pions, kaons, etc in FS
    int nNuc  = 0; // no. of nucleons in FS
    int nFrag = 0; // no. of fragments in FS
    //
    bool isEla    = false; // flag: elastic
    bool isQEl    = false; // flag: quasi-elastic
    bool isProd   = false; // flag: production 
    bool isFrag   = false; // flag: fragments
    bool isDisNuc = false; // flag: nuclear dissociation in nucleons
    
    //looking for charged pions and nucleons:
    vnucmom.clear();
    for(int ipart=0;ipart<npart;ipart++){
      int tpdg = hAinfo->prodpart[ipart].pdg;
      if      (isMeson (tpdg))    nMes++;
      else if (isNucleon (tpdg))  nNuc++;
      else if (tpdg > 1000000000) nFrag++;
      //
      if(isNucleon (tpdg)){
	double px = hAinfo->prodpart[ipart].mom[0] / 1000.; //MeV/c -> GeV/c
	double py = hAinfo->prodpart[ipart].mom[1] / 1000.;
	double pz = hAinfo->prodpart[ipart].mom[2] / 1000.;
	double pp = sqrt(pow(px,2)+ pow(py,2)+ pow(pz,2) );
	vnucmom.push_back(pp);
      }
    }
    
    // flag the event

    // meson production
    if (nMes > 0){ 
      nProdEvts++;
      isProd = true;
    }

    // elastic-like?
    else if(nFrag == 1 && nNuc == 1){ 
      nElaEvts++;
      isEla = true;
    }

    // quasi-elastic
    else if(nNuc == 2 && nFrag <= 1){ 
      // sometimes QEL-like events have a nuclear fragment, like C11 in the FS
      // sometimes not
      nQElEvts++;
      isQEl = true;
    }

    //fragment events
    else if((nFrag > 1 && nNuc > 0) || (nFrag > 0 && nNuc > 2)) {
      // events with multiple nuclear fragments in the FS
      // or events with more than 2 nucleons and one or more fragments
      nFragEvts++; 
      isFrag = true;
    }

    //only nucleobs in the event
    else if(nFrag == 0 && nNuc > 0) {
      //this events are like target dissosiation but in many nucleons
      //,i.e/, not nuclear fragments
      nDisNucEvts++;
      isDisNuc = true;
    }   
    else{
      std::cout<<"This should never happen"<<std::endl;
      evts->Show (jentry);
    }    

    //Filling histograms
    if(isEla || isQEl || isFrag || isDisNuc){
      // Elastic-like has only 1 nucleon but it is incuded here
      // for consistency structure. 
      int idx = -1;
      if(isEla)   idx = 0;
      if(isQEl)   idx = 1;
      if(isFrag)  idx = 2;
      if(isDisNuc)idx = 3;
      
      //Normally G4 track the hoghest energetic particle first
      //But just in case, we use the stl functions:
      sort(vnucmom.begin()   , vnucmom.end());
      reverse(vnucmom.begin(), vnucmom.end());

      if(vnucmom.size()==0){
	std::cout<<"This should not happened"<<std::endl;
	exit (1);
      }

      hlead_nuc[idx]->Fill(vnucmom[0]);
      for(unsigned int i=1;i<vnucmom.size();i++){
	hoth_nuc[idx]->Fill(vnucmom[i]);
      }
      
    }
    
    }//evts

  // open a txt tile to save some stats
  ofstream qeInfo ((string (foutput->GetName()) + ".QEinfo").c_str());

  qeInfo << "Processing " << evts->GetNtrees() << " trees in the chain\n\n";

  qeInfo << setw (15) << "#Nentries" << setw (15) << "el_like" << setw (15)
         << "qe_like" << setw (15) << "frag_like" << setw (15) << "dis-nuc_like" << setw (15)
         << "prod_entries\n";

  qeInfo << setw (15) << nentries << setw (15) << nElaEvts << setw (15)
         << nQElEvts << setw (15) << nFragEvts << setw (15) << nDisNucEvts << setw (15) 
         << nProdEvts << "\n\n";
         
  qeInfo.close();
  
  foutput->cd();
  for(int i=0;i<Ntyp;i++){
    hlead_nuc[i]->Write();
    hoth_nuc[i]->Write();
  }
  foutput->Close();

  std::cout<<"===>>>Running end"<<std::endl;;
}
///////////////////
void QElEvents(){
  std::cout<<"./QElEvents [infiles] [out_histfile]"<<std::endl;
  }
///////////////////
# ifndef __CINT__
int main(int argc, const char* argv[]){
  if(argc==1){
    QElEvents();
    exit (1);
  }  
  QElEvents(argv[1],argv[2]);
  return 0;
}
# endif
