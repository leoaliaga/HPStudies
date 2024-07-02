
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

using namespace std;

//constants:
// NA=6.022e23; // atoms/mol
// cm2_per_mb=1e-27
const double NAval = 6.022e-4;  //NAval = NA * cm2_per_mb
const double dx    = 0.7;       //cm target thickness
const double pival = 3.14159265;//approx

const int Nang =  7;
const int Nmom = 15;
const int Npar =  3;
const double ang[Nang] = {30,60,90,120,150,180,210};
const double mom[Nmom] = {0.75,1.00,1.25,1.50,1.75,2.00,2.25,2.50,
			  2.75,3.00,3.25,4.00,5.00,6.50,8.00};
const char* part[Npar] = {"pip","pim","prt"};

const char* cang[Nang-1] = {"ang030-060","ang060-090","ang090-120","ang120-150","ang150-180","ang180-210"};

static constexpr int kMaxprodpart = 1000;

int getpartidx(int tpdg);
std::vector<TH1D*> getanghist(TH2D* hh2In,std::string name);

//Compiling: g++ -o CreateMCXSec2 CreateMCXSec2.C `root-config --cflags --glibs`
//Running: ./CreateMCXSec2 [file with the list of input files] [output root file]

void CreateMCXSec2(const char* infiles, const char* out_histfile){

  TFile* foutput = new TFile(out_histfile,"RECREATE");

  TH2D* h2mult[Npar];
  TH2D* h2xsec[Npar];
  for(int i=0;i<Npar;i++){
    h2mult[i] = new TH2D(Form("h2mult_%s",part[i]),"",Nmom-1,mom,Nang-1,ang);
    h2xsec[i] = new TH2D(Form("h2xsec_%s",part[i]),"",Nmom-1,mom,Nang-1,ang);
  }
  
  TChain* evts  = new TChain("hAinfoTree");
  TChain* setup = new TChain("setup");

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

  int Npart;
  int pdg[kMaxprodpart];   
  double mom[kMaxprodpart][4];
  evts->SetBranchAddress("prodpart",&Npart); 
  evts->SetBranchAddress("prodpart.pdg",pdg);
  evts->SetBranchAddress("prodpart.mom[4]",mom); 
  
   //Setup:
  double dens, A;
  int POT;
  setup->SetBranchAddress("density",&dens); // g/cc
  setup->SetBranchAddress("A",&A); // g/mol
  setup->SetBranchAddress("nof_events",&POT); // POT
  setup->GetEntry(0);

  POT *= counter; //adding all POT per file
  
  double sigma_factor = A/(dens * NAval * dx * POT); // mb
  
  std::cout<<"=> sigma_factor "<< sigma_factor <<std::endl;
  
  std::cout<< (evts->GetNtrees()) <<" files were added to the chain"<<std::endl;
  
  //Loop and filling:
  int nentries  = (int)evts->GetEntries();
  std::cout<<"Entries "<<nentries<<std::endl;;

  for(long int jentry=0;jentry<nentries;jentry++){
    if(jentry%10000==0)std::cout<<"Entry "<<jentry/1000<<" k"<<std::endl;
    int nb = evts->GetEntry(jentry);  
    int npart = Npart;
    //looking for new particles and nucleons:
    for(int ipart=0;ipart<npart;ipart++){
      int idx = getpartidx(pdg[ipart]);
      if(idx<0)continue;
      double px = mom[ipart][0] / 1000.; //MeV/c -> GeV/c
      double py = mom[ipart][1] / 1000.; 
      double pz = mom[ipart][2] / 1000.; 

      //std::cout<<"=> "<<mom[ipart][2]<<std::endl;
       
      double pp = sqrt(pow(px,2)+ pow(py,2)+ pow(pz,2) );
      double aa = 1000.*acos(pz/pp);      
      h2mult[idx]->Fill(pp,aa);
      h2xsec[idx]->Fill(pp,aa);
    }
  }
  
  //Multiplicity:
  for(int i=0;i<Npar;i++){
    h2mult[i]->Scale(1./double(POT));
  }

  //Cross Sections:
  for(int i=1;i<=(Nmom-1);i++){
    double pmin   = h2xsec[0]->GetXaxis()->GetBinLowEdge(i);
    double pmax   = h2xsec[0]->GetXaxis()->GetBinUpEdge(i);
    double deltap = pmax - pmin;
    
    for(int j=1;j<=(Nang-1);j++){
      double amin   = ( h2xsec[0]->GetYaxis()->GetBinLowEdge(j) ) / 1000.; //mrad->rad
      double amax   = ( h2xsec[0]->GetYaxis()->GetBinUpEdge(j)  ) / 1000.;
      double deltao = 2.*pival * (cos(amin) - cos(amax) );

      for(int k=0;k<Npar;k++){
	double xsecval = h2xsec[k]->GetBinContent(i,j);
	xsecval *= (sigma_factor/ (deltap*deltao) );
	h2xsec[k]->SetBinContent(i,j,xsecval);
      }

    }    
  }

  //2D:
  foutput->mkdir("h2mult");
  foutput->mkdir("h2xsec");
  for(int i=0;i<Npar;i++){
    foutput->cd("h2mult");
    h2mult[i]->Write();
    foutput->cd("h2xsec");
    h2xsec[i]->Write();
  }

  //1D:
  foutput->cd();
  std::vector<TH1D*> vh1xsec[Npar];
  for(int i=0;i<Npar;i++){
    vh1xsec[i] = getanghist(h2xsec[i],Form("h1xsec_%s",part[i]));
    for(unsigned int j=0; j<vh1xsec[i].size(); j++){
      vh1xsec[i][j]->Write();
    }
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
std::vector<TH1D*> getanghist(TH2D* hh2In,std::string name){
  std::vector<TH1D*> vvout;

  for(int ii=0;ii<(Nang-1);ii++){
    vvout.push_back( new TH1D(Form("%s_%s",name.c_str(),cang[ii]),"",Nmom-1,mom) );

    for(int jj=1;jj<=(Nmom-1);jj++){
      vvout[ii]->SetBinContent(jj,hh2In->GetBinContent(jj,ii+1));
    }
    
  }
  
  return vvout;
}
///////////////////
# ifndef __CINT__
int main(int argc, const char* argv[]){
  CreateMCXSec2(argv[1],argv[2]);
  return 0;
}
# endif
