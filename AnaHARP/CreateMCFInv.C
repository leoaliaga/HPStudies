
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
#include <stdlib.h>
#include <iomanip>

#include "hptuple.hh"

using namespace std;

//constants:
// NA=6.022e23; // atoms/mol
// cm2_per_mb=1e-27
const double NAval = 6.022e-4;  //NAval = NA * cm2_per_mb
const double dx    = 0.7;       //cm target thickness
const double pival = 3.14159265;//approx

const double mprt    = 0.938272;
const double mcpi    = 0.139570;

//Kinematics
const int Npt = 50;
const int Nxf = 50;
const double xfmin = -1;
const double xfmax =  1;
const double ptmin =  0;
const double ptmax =  1;

const int Npar =  3;
const char* part[Npar] = {"pip","pim","prt"};

int getpartidx(int tpdg);
double getxF(double incp,int tpdg,double mx,double my,double mz,double en);
  
//Compiling: g++ -o CreateMCFInv CreateMCFInv.C `root-config --cflags --glibs` -I${G4WORKDIR}/include  -L${G4WORKDIR}/build -lg4hpDict
void CreateMCFInv();
void CreateMCFInv(const char* infiles, const char* out_histfile){

  TFile* foutput = new TFile(out_histfile,"RECREATE");
  TH2D* hmult[Npar];
  TH2D* hfinv[Npar];
  for(int i=0;i<Npar;i++){
    hmult[i] = new TH2D(Form("hmult_%s",part[i]),"",Nxf,xfmin,xfmax,Npt,ptmin,ptmax);
    hfinv[i] = new TH2D(Form("hfinv_%s",part[i]),"",Nxf,xfmin,xfmax,Npt,ptmin,ptmax);
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
  double inc_e,dens, A;
  int POT;
  setup->SetBranchAddress("energy",&inc_e); //GeV
  setup->SetBranchAddress("density",&dens); // g/cc
  setup->SetBranchAddress("A",&A); // g/mol
  setup->SetBranchAddress("nof_events",&POT); // POT
  setup->GetEntry(0);
  double dPOT = double(POT);
  dPOT *= double(counter); //adding all POT per file
  
  double sigma_factor = A/(dens * NAval * dx * dPOT); // mb
  
  std::cout<<"=> sigma_factor "<< sigma_factor <<std::endl;

  //Assuming the incident particle is a proton:
  double inc_p = sqrt(pow(inc_e,2)-pow(mprt,2));
  
  std::cout<< (evts->GetNtrees()) <<" files were added to the chain"<<std::endl;
  
  //Loop and filling:
  int nentries  = (int)evts->GetEntries();
  std::cout<<"Entries "<<nentries<<std::endl;;

  for(long int jentry=0;jentry<nentries;jentry++){
     if(jentry%(nentries/10) == 0) std::cout << "." << std::flush;
     int nb = evts->GetEntry(jentry);  
    
    int npart = int(hAinfo->prodpart.size());
    
    //looking for charged pions and nucleons:
    for(int ipart=0;ipart<npart;ipart++){
      int pdg = hAinfo->prodpart[ipart].pdg;      
      int idx = getpartidx(hAinfo->prodpart[ipart].pdg);      
      if(idx<0)continue;
      double px = hAinfo->prodpart[ipart].mom[0];//MeV/c
      double py = hAinfo->prodpart[ipart].mom[1];
      double pz = hAinfo->prodpart[ipart].mom[2];
      double ee = hAinfo->prodpart[ipart].mom[3];
      double pt = sqrt( pow(px,2)+ pow(py,2) ) / 1000.; //to GeV
      double xf = getxF(inc_p,pdg,px/1000,py/1000,pz/1000,ee/1000);
      
      hmult[idx]->Fill(xf,pt);
      hfinv[idx]->Fill(xf,pt);      
    }//part
    
  }//evts
  
  //Multiplicity:
  for(int i=0;i<Npar;i++){
    hmult[i]->Scale(1./dPOT);
  }
  
  //Invariant cross sections
  for(int h=0;h<2;h++) {
    //bin by bin
    for(int i=1;i <= hfinv[h]->GetNbinsX();i++) {
      for (int j=1;j <= hfinv[h]->GetNbinsY();j++) {
	double mult    = hfinv[h]->GetBinContent(i,j);
	double xfwidth = hfinv[h]->GetXaxis()->GetBinWidth(i);
	double ptwidth = hfinv[h]->GetYaxis()->GetBinWidth(j);
	double norm    = mult / (xfwidth * ptwidth * nentries);
	hfinv[h]->SetBinContent(i, j, norm);
      }
    }
    // Scale by invariant cross-section factors
    hfinv[h]->Scale(1.0 / (2 * pival * sigma_factor));
  }
  
  //Storing
  foutput->mkdir("mult");
  foutput->mkdir("finv");
  for(int i=0;i<Npar;i++){
    foutput->cd("mult");
    hmult[i]->Write();
    foutput->cd("finv");
    hfinv[i]->Write();
  }
  
  foutput->Close();
  std::cout<<"===>>>Running end"<<std::endl;;
}
///////////////////
int getpartidx(int tpdg){
  int tidx = -1;
  if(tpdg == 211)tidx=0;
  if(tpdg ==-211)tidx=1;
  if(tpdg ==2212)tidx=2;
  return tidx;
}
///////////////////
double getxF(double incp,int tpdg,double mx,double my,double mz,double en){

  double inc_e_lab = std::sqrt(pow(incp,2) + pow(mprt,2));
  double ecm       = std::sqrt(pow(mprt,2) + pow(mprt,2) + 2.*inc_e_lab*mprt);
  double gammacm   = (inc_e_lab + mprt)/ecm;
  double betacm    = std::sqrt(1. - 1./pow(gammacm,2));
  double gammabetacm = incp/ecm;
  
  double plcm        = gammacm * mz - gammabetacm * en;

  double appxF  = plcm*2./ecm;
  
  double max_th = -1;
  if(tpdg == 211)max_th = 1.878;
  if(tpdg ==-211)max_th = 2.016;
  
  double ecm_max  = (pow(ecm,2)+ pow(mcpi,2) - pow(max_th,2) ) / (2.*ecm);

  double plcm_max = sqrt( pow(ecm_max,2) - pow(mcpi,2) );

  double xF  = plcm/plcm_max;

  if(max_th<0)return appxF;
  
  return xF;
  
}
///////////////////
void CreateMCFInv(){
  std::cout<<"./CreateMCXSec [infiles] [out_histfile]"<<std::endl;
  }
///////////////////
# ifndef __CINT__
int main(int argc, const char* argv[]){
  if(argc==1){
    CreateMCFInv();
    exit (1);
  }  
  CreateMCFInv(argv[1],argv[2]);
  return 0;
}
# endif
