
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
const Int_t    Nhistos = 201; 
const Int_t    pT_bins = 80;
const Double_t fpT     = 0.0125;
const Double_t lpT     = 2.0125;
const Int_t    Nmax    = 150;
const Double_t fxF     = -0.1025;
const Double_t lxF     = 0.9025;
const Double_t DxF     = (lxF-fxF)/Double_t(Nhistos);

const Int_t Npart = 8;
std::string spart[Npart] = {"pip","pim","kap","kam","klong","kshort","prt","neu"};

Int_t getHistoID(Double_t xFval);
Double_t getxF(Int_t id);

void CreateYields(Int_t Mom, const char* infiles, const char* out_histfile, const char* out_QEfile, bool include_ff){

  TFile* foutput = new TFile(out_histfile,"RECREATE");

  std::ofstream qeinfo;
  qeinfo.open(out_QEfile);

  //Histograms:
  TH2D*   hxFpT[Npart];
  
  TChain* evts = new TChain("hAinfoTree");
  HPTuple*  hAinfo  = new HPTuple;
  std::ifstream ifs;	
  ifs.open(infiles);
  std::string line;
  while (ifs.good()) {
    getline(ifs,line);
    if(line.find(".root")>10000)continue;
    evts->Add(line.c_str());
    std::cout<<"Adding ntuple at : "<<line<<std::endl;
  }	
  ifs.close();  
  evts->SetBranchAddress("hAinfo",&hAinfo);
  
  std::cout<< (evts->GetNtrees()) <<" files were added to the chain"<<std::endl;
  
  qeinfo<<"Processing "<< (evts->GetNtrees()) <<" trees in the chain"<<std::endl;;
  qeinfo<<"#Nentries Entries el_like qe_like frag_like prod_entries"<<std::endl;;  
  //Counting quasielastics:
  Int_t  Nqe_pp        = 0;
  Int_t  Nqe_tot       = 0;
  Int_t  prod_tot      = 0;
  Int_t  qe_tot        = 0;
  Int_t  frag_tot      = 0;
  Int_t  el_tot        = 0;
  Double_t sec_p_ener  = 0.;

  foutput->cd(0);
  for(Int_t ii=0;ii<6;ii++){
    hxFpT[ii] = new TH2D(Form("xFpT_%s",spart[ii].c_str()),";x_{F}; p_{T} (GeV/c)", 
			 201,-0.1025,0.9025,80,0.0125,2.0125);
  }
  // protons and neutrons need a different binning in xF
  for(Int_t ii=6;ii<=7;ii++){
    hxFpT[ii] = new TH2D(Form("xFpT_%s",spart[ii].c_str()),";x_{F}; p_{T} (GeV/c)", 
			 351,-0.8025,0.9525,80,0.0125,2.0125);
  }
  // special histogram for neutron yields
  TH1D* dndxf_neu          = new TH1D("dndxf_neu"         ,"; x_{F}; dn/dx_{F} for neutrons",100,0.0,1.0);
  TH1D* dndxf_neu_cut      = new TH1D("dndxf_neu_cut"     ,"; x_{F}; dn/dx_{F} for neutrons",100,0.0,1.0);
  TH1D* dndxf_neu_prod     = new TH1D("dndxf_neu_prod"    ,"; x_{F}; dn/dx_{F} for neutrons",100,0.0,1.0);
  TH1D* dndxf_neu_prod_cut = new TH1D("dndxf_neu_prod_cut","; x_{F}; dn/dx_{F} for neutrons",100,0.0,1.0);

  //GetEntries and Loop:
  int TEntries = 0;
  int nentries  = (int)evts->GetEntries();

  std::cout<<"Entries "<<nentries<<std::endl;;

  Long64_t pip_yield=0;
  Long64_t pim_yield=0;
  
  for(long int jentry=0;jentry<nentries;jentry++) {
    TEntries++;
    Int_t countpiK      = 0;
    Int_t countNucleons = 0;
    Int_t countFragments= 0;

    if(jentry%100000==0)std::cout<<"Entry "<<jentry/1000<<" k"<<std::endl;;
    int nb = evts->GetEntry(jentry);  
    
    int npart = int(hAinfo->prodpart.size());
    
    //looking for new particles and nucleons:
    for(int ipart=0;ipart<npart;ipart++){
      int pdg = hAinfo->prodpart[ipart].pdg;
      if(abs(pdg)==211 || abs(pdg)==321)countpiK++;
      if(pdg==111)countpiK++;    
      if(pdg==130 || pdg==310 )countpiK++;
      if(pdg==2212||pdg==2112)countNucleons++;
      if(pdg>1000000000) countFragments++;
    }
    
    //we look for characterize the event:
    bool qe_event=false;
    // sometimes QEL-like events have a nuclear fragment, like C11 in the Final State
    // sometimes not
    if(countpiK == 0 && countNucleons==2 && countFragments<=1){ qe_tot++; qe_event=true;}
    
    bool frag_event=false;
    // events with multiple nuclear fragments in the FS
    // or events with more than 2 nucleons and one or more fragments
    if((countpiK ==0 && countFragments>1 && countNucleons>0)
       || (countpiK ==0 && countFragments>0 && countNucleons>2)){
      frag_tot++; 
      frag_event=true;
    }
    
    bool el_event=false;
    if(countpiK==0 && countFragments==1 && countNucleons==1){el_tot++; el_event=true;}
    
    //production count:
    bool prod_event=false;
    if(countpiK>0){ prod_tot++; prod_event=true;}  

    //if(!prod_event && !qe_event && !frag_event && !el_event) evts->Show(jentry);
    
    ////Filling histograms:
    for(int ipart=0;ipart<npart;ipart++){
      int pdg = hAinfo->prodpart[ipart].pdg;
      Double_t xF = hAinfo->prodpart[ipart].xf;
      int xfh = getHistoID(xF); //to localize the xF histo
      Double_t pT = hAinfo->prodpart[ipart].pt/1000.; //in GeV
      if(!include_ff && hAinfo->prodpart[ipart].ff==kTRUE) continue; // do not histogram particles from fast decays (eta,eta')

      if(pdg== 211) { hxFpT[0]->Fill(xF,pT); }
      
      else if(pdg==-211) { hxFpT[1]->Fill(xF,pT); }
      else if(pdg== 321) { hxFpT[2]->Fill(xF,pT); }
      else if(pdg==-321) { hxFpT[3]->Fill(xF,pT); }
      else if(pdg==130) { hxFpT[4]->Fill(xF,pT); }
      else if(pdg==310) { hxFpT[5]->Fill(xF,pT); }
      else if(pdg== 2212 && prod_event){ hxFpT[6]->Fill(xF,pT); }
      else if(pdg== 2112 && prod_event){ hxFpT[7]->Fill(xF,pT); }

      if(xF>-0.1 && xF<0.5){
	if(pdg==211) pip_yield++;
	else if(pdg==-211) pim_yield++;
      }

      // fill neutron yields histograms
      if(pdg==2112){
	dndxf_neu->Fill(xF);
	if(prod_event) 	dndxf_neu_prod->Fill(xF);
	double A=0.398; double B=4.315; // pt< A+B*xF for NA49 neutron acceptance
	if(pT/1000.0<A+B*xF){
	  dndxf_neu_cut->Fill(xF);
	  if(prod_event) 	dndxf_neu_prod_cut->Fill(xF);
	}
      }

    }
  }
  
  qeinfo<<nentries<<"    "<<TEntries<<"    "<<el_tot<<"     "<<qe_tot<<"    "<<frag_tot<<"     "<<prod_tot<<std::endl;;
  qeinfo<<"average pi+ multiplicity per production event: "<<double(pip_yield)/double(prod_tot)<<std::endl;;
  qeinfo<<"average pi- multiplicity per production event: "<<double(pim_yield)/double(prod_tot)<<std::endl;;
  
  qeinfo.close();
  foutput->Write();
  foutput->Close();
  std::cout<<"===>>>Running end"<<std::endl;;
}

//Get Histo ID:
Int_t getHistoID(Double_t xFval){
  
  if(xFval<-0.1025 || xFval>0.9025)return -1;

 //formula to get the binID:
  return Int_t((xFval-(fxF))/DxF);
}

//Get xF value from xF histo number:
Double_t getxF(Int_t id){
  return fxF + (Double_t(id)+0.5)*DxF; 
}

# ifndef __CINT__
int main(int argc, const char* argv[]){
  int tmom = atoi(argv[1]); 
  bool doff = true;
  if(argc==5)CreateYields(tmom,argv[2],argv[3],argv[4],doff);
  if(argc==6){
    if(std::string(argv[5])=="false")doff = false;
    CreateYields(tmom,argv[2],argv[3],argv[4],doff);
  }
  return 0;
}
# endif
