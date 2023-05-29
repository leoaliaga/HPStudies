
#include "utadkmeta.h"
#include "utadk2nu.h"

#include "TSystem.h"
#include "TFile.h"
#include "TH1D.h"
#include "TChain.h"

#include <iostream>
#include <fstream>
#include <vector>

const int wanted_det = 4; //This index corresponds to the SBND detector

//To generate the library: 
//rootcint -f utadk2nuCint.cc -c utadk2nu.h utadkmeta.h LinkDef.h 
//g++ -shared -fPIC `root-config --cflags` -o libutadk2nuTree.so utadk2nu.cc utadkmeta.cc utadk2nuCint.cc

//To compile this code:
//g++ -o SBNFluxAna_dk2nu SBNFluxAna_dk2nu.C `root-config --cflags --glibs` lib/libutadk2nuTree.so

//To run:
// ./SBNFluxAna_dk2nu [dk2nu_input_file_name] [output_file_name]

// 4 neutrino types in this analysis:
const int Nnu             = 4;
const int pdgnu[Nnu]      = {12,-12,14,-14};
const char* namenu[Nnu]   = {"nue","nuebar","numu","numubar"};

const int Nint            = 7;
const char* nameint[Nint] = {"tot","pBepi_HARP","pBepi_NoHARP","pBeK","pBenuc","incmes","other"};

//Bin parameters for the neutrino energy:
const int Nbins      = 120;
const double mine    = 0.0;
const double maxe    = 6.0;

//get the index of the neutrino type
int get_idxnu(int tpdg);
//get the index of the hadronic interaction type 
int get_idxhad(int incpdg, std::string incproc, std::string tmat, int prodpdg, double tmom, double tang );

double get_angle(double tpxi, double tpyi, double tpzi, double tpxf, double tpyf, double tpzf); 
 
void SBNFluxAna_dk2nu(const char* cfIn, const char* cfOut){  
  std::cout<< "starting()"<<std::endl;    
  
  //Declaring histograms:
  TH1D* hflux[Nnu];
  TH1D* hhad[Nnu][Nint];
  for(int ii=0;ii<Nnu;ii++){
    hflux[ii] = new TH1D(Form("hflux_%s",namenu[ii]),"",Nbins,mine,maxe);
    for(int jj=0;jj<Nint;jj++){
      hhad[ii][jj]  = new TH1D(Form("hhad_%s_%s" ,namenu[ii],nameint[jj]),"",Nbins,mine,maxe);
    }
  }
  
  //Declaring Dk2Nu class:   
  TChain* eIn         = new TChain("dk2nuTree");  
  uta::Dk2Nu*  dk2nu  = new uta::Dk2Nu;  
  
  //Input trees from dsta files
  std::ifstream fIn(cfIn);
  std::string line;
  while(std::getline(fIn, line)){
    if( line.find(".root")>10000) continue;
    std::cout<<"=> Input file: "<<line<<std::endl;
    eIn->Add(line.c_str());
  }

  //Setting braches:    
  eIn->SetBranchAddress("dk2nu",&dk2nu);
  int nentries  = eIn->GetEntries();
  std::cout<<"Using "<<nentries<<" events"<<std::endl;  
         
  //Loop over entries:
        
  for(int ii=0;ii<nentries;ii++){  
    if(ii%100000==0)std::cout<<ii/1000<<" K evts"<<std::endl;
    eIn->GetEntry(ii);
    
    //Weight for the SBND position
    double wgt  = dk2nu->decay.nimpwt * (dk2nu->nuray)[wanted_det].wgt;
    //Neutrino energy at SBND:
    double ener = (dk2nu->nuray)[wanted_det].E;
    
    int idxnu = get_idxnu(dk2nu->decay.ntype);
    if(idxnu<0)continue;
      
    hflux[idxnu]->Fill(ener, wgt);
    
    //Check of ancestry:
    unsigned int nanc = (dk2nu->ancestor).size();
    if( (nanc<3) || (dk2nu->ancestor[0].proc)!="Primary" || (dk2nu->ancestor[nanc-1].proc)!="Decay:"){
      std::cout<<"=> Something wrong with this entry"<<std::endl;
      continue;
    }
    
    //Loop on neutrino ancestors (excluding first and last ancestors)  
    for(unsigned int jj=1;jj<(nanc-1);jj++){

      //is this pocess inelastic?
      bool is_inel = (dk2nu->ancestor[jj].proc).find("Inelastic") < 100; 
      if( !is_inel) continue;
      
      //Filling the total
      hhad[idxnu][0]->Fill(ener, wgt);  
      
      //Getting particle momentum:
      double partmom = sqrt(pow(dk2nu->ancestor[jj].startpx,2)+pow(dk2nu->ancestor[jj].startpy,2)+pow(dk2nu->ancestor[jj].startpz,2));
      
      //Geeting particle angle:
      double partang = get_angle(dk2nu->ancestor[jj-1].startpx, dk2nu->ancestor[jj-1].startpy, dk2nu->ancestor[jj-1].startpz, 
                                 dk2nu->ancestor[jj  ].startpx, dk2nu->ancestor[jj  ].startpy, dk2nu->ancestor[jj  ].startpz);
                            
      //Filling per type of hadronic interaction
      //int incpdg, std::string incproc, std::string tmat, int prodpdg
      
      int idxhad = get_idxhad(dk2nu->ancestor[jj-1].pdg, dk2nu->ancestor[jj-1].proc, dk2nu->ancestor[jj].imat, 
                              dk2nu->ancestor[jj].pdg, partmom, partang); 
           
      if(idxhad>=0)hhad[idxnu][idxhad]->Fill(ener, wgt);
    }
    
  } //end evt loop
  
  TFile* fOut = new TFile(cfOut,"recreate");
  for(int ii=0;ii<Nnu;ii++)fOut->mkdir(namenu[ii]);
  
  for(int ii=0;ii<Nnu;ii++){  
    fOut->cd(namenu[ii]);  
    hflux[ii]->Write();
    for(int jj=0;jj<Nint;jj++)hhad[ii][jj]->Write(); 
  }   
  fOut->Close();
  
  std::cout<< "ending()"<<std::endl;
}  
/////////////////////
int get_idxnu(int tpdg){
  int tidx = -1;
  if(tpdg == 12)tidx = 0;
  if(tpdg ==-12)tidx = 1;
  if(tpdg == 14)tidx = 2;
  if(tpdg ==-14)tidx = 3;
  return tidx;
}
/////////////////////
double get_angle(double tpxi, double tpyi, double tpzi, double tpxf, double tpyf, double tpzf){
  // theta = arccos( (A.B)/|A||B| )
  //A.B:
  double tnum = tpxi*tpxf + tpyi*tpyf + tpzi*tpzf;
  //|A|:
  double tA = sqrt( pow(tpxi,2) + pow(tpyi,2) + pow(tpzi,2) );
  //|B|:
  double tB = sqrt( pow(tpxf,2) + pow(tpyf,2) + pow(tpzf,2) );
  
  if(tA==0 || tA==0) return -1;
  
  return 1000.*acos( tnum/(tA*tB) ); //mrad
}

int get_idxhad(int incpdg, std::string incproc, std::string tmat, int prodpdg, double tmom, double tang){
  int tidx = -1;
  
  //is primary proton on Be interaction?
  bool is_primary_be = (incpdg==2212 && tmat=="BERILLIUM" && incproc=="Primary");
  
  //is pion plus produced in the HARP phase space?
  //HARP proton on Berylium at 8.9 GeV
  //https://arxiv.org/pdf/hep-ex/0702024.pdf
  //Table 5
  //angle(mrad)  mom(GeV/c) 
  // 30-60       0.75-6.5
  // 60-90       0.75-6.5
  // 90-120      0.75-6.5 
  //120-150      0.75-6.5 
  //150-180      0.75-6.5 
  //180-210      0.75-6.5 
  bool is_HARP_kin = (tmom>= 0.5 && tmom<6.5 && tang>= 30. && tang<210);
      
  if( is_primary_be && prodpdg== 211 && is_HARP_kin){
    tidx = 1;
  }
  else if( is_primary_be && abs(prodpdg)== 211){
    tidx = 2;
  }
  else if( is_primary_be && abs(prodpdg)== 321 ){
    tidx = 3;
  }
  else if( is_primary_be && abs(prodpdg)==2212 || abs(prodpdg)==2112){
    tidx = 4;    
  }   
  else if( abs(incpdg)  == 211 ){
    tidx = 5;
  }
  else{
   tidx = 6; 
  }
  
  return tidx; 
  
  //const char* nameint[Nint] = {"tot","pBepi_HARP","pBepi_NoHARP","pBeK","pBenuc","incmes","other"};
  
}
#ifndef __CINT__
int main(int argc, const char* argv[]){

  SBNFluxAna_dk2nu(argv[1],argv[2]);
  return 0;
}
#endif               
