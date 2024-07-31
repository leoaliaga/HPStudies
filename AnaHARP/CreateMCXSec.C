
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

//Kinematics
const int Nang1 = 6;
const int Nmom1 = 14;
const int Nang2 = 1000;
const int Nmom2 = 900;
//
const double vang[Nang1+1] = {30,60,90,120,150,180,210};
const double vmom[Nmom1+1] = {0.75,1.00,1.25,1.50,1.75,2.00,2.25,2.50,
			      2.75,3.00,3.25,4.00,5.00,6.50,8.00};
const double angmin = 0.0;
const double angmax = 1000.; 
const double mommin = 0.0;
const double mommax = 9.0;

const int Npar =  3;
const char* part[Npar] = {"pip","pim","prt"};

const char* cang[Nang1] = {"ang030_060","ang060_090","ang090_120","ang120_150","ang150_180","ang180_210"};

int getpartidx(int tpdg);
std::vector<TH1D*> getanghist(TH2D* hh2In,std::string name);

//Compiling: g++ -o CreateMCXSec CreateMCXSec.C `root-config --cflags --glibs` -I${G4WORKDIR}/include  -L${G4WORKDIR}/build -lg4hpDict
void CreateMCXSec();
void CreateMCXSec(const char* infiles, const char* out_histfile){

  TFile* foutput = new TFile(out_histfile,"RECREATE");
  //Type
  //A: HARP angles and momenta
  //B: HARP angles and fine momenta
  //C: fine angles and momenta
  const int Ntyp = 3;
  const char* ctyp[Ntyp] = {"A","B","C"};  
  TH2D* hmult[Npar][Ntyp];
  TH2D* hxsec[Npar][Ntyp];
  for(int i=0;i<Npar;i++){
    hmult[i][0] = new TH2D(Form("hmultA_%s",part[i]),"",Nmom1,vmom,Nang1,vang);
    hxsec[i][0] = new TH2D(Form("hxsecA_%s",part[i]),"",Nmom1,vmom,Nang1,vang);
    //
    hmult[i][1] = new TH2D(Form("hmultB_%s",part[i]),"",Nmom2,mommin,mommax,Nang1,vang);
    hxsec[i][1] = new TH2D(Form("hxsecB_%s",part[i]),"",Nmom2,mommin,mommax,Nang1,vang);
    //
    hmult[i][2] = new TH2D(Form("hmultC_%s",part[i]),"",Nmom2,mommin,mommax,Nang2,angmin,angmax);
    hxsec[i][2] = new TH2D(Form("hxsecC_%s",part[i]),"",Nmom2,mommin,mommax,Nang2,angmin,angmax);
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
  POT *= counter; //adding all POT per file
  
  double sigma_factor = A/(dens * NAval * dx * POT); // mb
  
  std::cout<<"=> sigma_factor "<< sigma_factor <<std::endl;
  
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
      int idx = getpartidx(hAinfo->prodpart[ipart].pdg);      
      if(idx<0)continue;
      //std::cout<<idx<<" "<< (hAinfo->prodpart[ipart].pdg) <<std::endl;
      double px = hAinfo->prodpart[ipart].mom[0] / 1000.; //MeV/c -> GeV/c
      double py = hAinfo->prodpart[ipart].mom[1] / 1000.;
      double pz = hAinfo->prodpart[ipart].mom[2] / 1000.;
      double pp = sqrt(pow(px,2)+ pow(py,2)+ pow(pz,2) );
      double aa = 1000.*acos(pz/pp); //mrad
      for(int i=0;i<Ntyp;i++){
	hmult[idx][i]->Fill(pp,aa);
	hxsec[idx][i]->Fill(pp,aa);
      }//typ
    }//part
    
  }//evts
  
  //Multiplicity:
  for(int i=0;i<Npar;i++){
    for(int j=0;j<Ntyp;j++){
      hmult[i][j]->Scale(1./double(POT));
    }
  }
  
  //Cross Sections:
  for(int p=0;p<Ntyp;p++){ //typ

    int Nmom = hxsec[0][p]->GetXaxis()->GetNbins();
    int Nang = hxsec[0][p]->GetYaxis()->GetNbins();

    //////////////////////
    //loop on mom
    for(int i=1;i<=Nmom;i++){
      double pmin   = hxsec[0][p]->GetXaxis()->GetBinLowEdge(i);
      double pmax   = hxsec[0][p]->GetXaxis()->GetBinUpEdge(i);
      double deltap = pmax - pmin;
      
      for(int j=1;j<=Nang;j++){
	double amin   = ( hxsec[0][p]->GetYaxis()->GetBinLowEdge(j) ) / 1000.; //mrad->rad
	double amax   = ( hxsec[0][p]->GetYaxis()->GetBinUpEdge(j)  ) / 1000.;
	double deltao = 2.*pival * (cos(amin) - cos(amax) );
	
	for(int k=0;k<Npar;k++){
	  double xsecval = hxsec[k][p]->GetBinContent(i,j);
	  xsecval *= (sigma_factor/ (deltap*deltao) );
	  hxsec[k][p]->SetBinContent(i,j,xsecval);
	}//particle
	
      } //ang
      
    } //mom
    //////////////////////
        
  } //typ

  //2D:
  foutput->mkdir("mult");
  foutput->mkdir("xsec");
  for(int i=0;i<Npar;i++){
    for(int j=0;j<Ntyp;j++){
      foutput->cd("mult");
      hmult[i][j]->Write();
      foutput->cd("xsec");
      hxsec[i][j]->Write();
    }
  }

   //HARP 1D:
  foutput->mkdir("xsec1D");
  foutput->cd("xsec1D");
  std::vector<TH1D*> vh1xsec[Npar];
  for(int i=0;i<Npar;i++){
    vh1xsec[i] = getanghist(hxsec[i][0],Form("h1xsec_%s",part[i]));
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
///////////////////
std::vector<TH1D*> getanghist(TH2D* hh2In,std::string name){
  std::vector<TH1D*> vvout;

  for(int ii=0;ii<Nang1;ii++){
    vvout.push_back( new TH1D(Form("%s_%s",name.c_str(),cang[ii]),"",Nmom1,vmom) );

    for(int jj=1;jj<=Nmom1;jj++){
      vvout[ii]->SetBinContent(jj,hh2In->GetBinContent(jj,ii+1));
    }
    
  }
  
  return vvout;
}
///////////////////
void CreateMCXSec(){
  std::cout<<"./CreateMCXSec [infiles] [out_histfile]"<<std::endl;
  }
///////////////////
# ifndef __CINT__
int main(int argc, const char* argv[]){
  if(argc==1){
    CreateMCXSec();
    exit (1);
  }  
  CreateMCXSec(argv[1],argv[2]);
  return 0;
}
# endif
