
#include <iostream>
#include <fstream>
#include <vector>
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

//bins form pip and piom production
const int Naran          = 12;
const double mina[Naran] = {0 , 3,10,20,40, 60,100,140,180,240,300,360};
const double maxa[Naran] = {10,10,20,40,60,100,140,180,240,300,360,420};
const int Npart         = 2;
const char* part[Npart] = {"pip","pim"};
const int Nbin    = 600;
const double minx = 0;
const double maxx = 60;

void CreateMult(const char* infiles, const char* out_histfile, bool include_ff){
  
  TFile* foutput = new TFile(out_histfile,"RECREATE");
  TH1D* hMul[Npart][Naran];
  for(int i=0;i<Npart;i++){
    for(int j=0;j<Naran;j++){
      hMul[i][j] = new TH1D(Form("h%s_%d-%dmrad",part[i],int(mina[j]),int(maxa[j])),"",Nbin,minx,maxx); 
    }
  }
  int prod_tot = 0;

  //Loading ntuples:
  TChain* evts   = new TChain("hAinfoTree");  
  HPTuple*  hAinfo  = new HPTuple;  
  std::ifstream ifs;	
  ifs.open(infiles);
  std::string line;
  while (ifs.good()){
    getline(ifs,line);
    if(line.find(".root")>10000)continue;
    evts->Add(line.c_str());
  }	
  ifs.close();
  
  evts->SetBranchAddress("hAinfo",&hAinfo);
  int nentries  = evts->GetEntries();
  std::cout<<"nentries "<<nentries<<std::endl;
  for(int ii=0;ii<nentries;ii++){  
    if(ii%10000==0)std::cout<<ii/1000<<" k evts"<<std::endl;
    evts->GetEntry(ii); 
    
    int npart = int(hAinfo->prodpart.size());

    //looking for new particles:
    int countpiK      = 0;    
    for(int ipart=0;ipart<npart;ipart++){
      int pdg = hAinfo->prodpart[ipart].pdg;
      if(abs(pdg)==211 || abs(pdg)==321)countpiK++;
      if(pdg==111)countpiK++;    
      if(pdg==130 || pdg==310 )countpiK++;
    }
    
    //Filling
    for(int ipart=0;ipart<npart;ipart++){
      int pdg = hAinfo->prodpart[ipart].pdg;
      int idx = -1;
      if(pdg == 211)idx = 0;
      if(pdg ==-211)idx = 1;
      if(idx<0)continue;
      
      double PP = sqrt( pow(hAinfo->prodpart[ipart].mom[0],2) + pow(hAinfo->prodpart[ipart].mom[1],2) + pow(hAinfo->prodpart[ipart].mom[2],2) );    
      PP /= 1000;
      double aa = 1000.*atan( hAinfo->prodpart[ipart].pt / hAinfo->prodpart[ipart].mom[2] );
      for(int jj=0;jj<Naran;jj++){
	if(aa>=mina[jj] && aa<maxa[jj])hMul[idx][jj]->Fill(PP);
      }
    }
    
    //production count:
    bool prod_event=false;
    if(countpiK>0){ prod_tot++; prod_event=true;} 
  } //end of evts
  
  foutput->cd();
  //Scaling by the number of prods, angle bin width
  for(int i=0;i<Npart;i++){
    for(int j=0;j<Naran;j++){
      hMul[i][j]->Scale(1./double(prod_tot));
      double binaw = 0.001*(maxa[j] - mina[j]);
      hMul[i][j]->Scale(1./binaw);
      double binpw  = hMul[0][0]->GetXaxis()->GetBinWidth(1);
      hMul[i][j]->Scale(1./binpw);
    }
  }

  /////////
  for(int i=0;i<Npart;i++){
    for(int j=0;j<Naran;j++){
      hMul[i][j]->SetTitle(";Momentum (GeV/c);d^{2} / (dpd#theta)[rad.(GeV/c)^{-1}])");
      hMul[i][j]->Write();
    }
  }
  
  foutput->Close();
  std::cout<<"===>>>Running end"<<std::endl;
}

# ifndef __CINT__
int main(int argc, const char* argv[]){
  if(argc==3)CreateMult(argv[1],argv[2],true);
  else if(argc==4)CreateMult(argv[1],argv[2],argv[3]);
  else exit (1);
  return 0;
}
# endif
