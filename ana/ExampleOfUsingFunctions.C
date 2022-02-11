
#include "functions.cxx"

using namespace std;

//Examples on how to use the pre-definied functions
void ExampleOfUsingFunctions(){
   
  //Calculate the average invariant cross section 
  //and xF and pT for the center of the bin

  //Given the integrated cross section divided by the bin momentum width
  // p + C -> pi- X at 31 GeV, Phys.Rev.C84:034604, 2011.
  //Data: 4.91 mb/(GeV/c) for momentum in [3.0,3.2]GeV/c and angle in [40,60]mrad
  std::cout<<""<<std::endl;std::cout<<"Example for p + C -> pi- X at 31 GeV"<<std::endl;
  std::cout<<"<f>: "<<get_avef("XS/DP",4.91,31,"cpi",3,3.2, 0.04,0.06)<<std::endl;
  std::cout<<"xF : "<<get_xf("prt",31,"cpi",3.1,0.05)<<std::endl;
  std::cout<<"pT : "<<get_pt(3.1,0.05)<<std::endl;
  
  //Given the integrated cross section divided by the bin momentum and angle widths
  // p + C -> K+ X at 31 GeV, Eur.Phys. J. C76(2), 1-49 (2016).
  //Data: 12.9 mb/rad/(GeV/c) for momentum in [4.8,5.6]GeV/c and angle in [20,40]mrad
  std::cout<<""<<std::endl;std::cout<<"Example for p + C -> K+ X at 31 GeV"<<std::endl;
  std::cout<<"<f>: "<<get_avef("XS/DPDA",12.9,31,"cka",4.8,5.6, 0.02,0.04)<<std::endl;
  std::cout<<"xF "<<get_xf("prt",31,"cka",5.2,0.03)<<std::endl;
  std::cout<<"pT "<<get_pt(5.2,0.03)<<std::endl;
  
  //Given the integrated multiplcity divided by the bin momentum and angle widths
  // pi+ + C -> pi+ X at 60 GeV, Phis. Rev. D 100, 112004 (2019).
  //Data: 0.483 /rad/(GeV/c) for momentum in [17,19]GeV/c and angle in [20,40]mrad
  std::cout<<""<<std::endl;std::cout<<"Example for pi+ + C -> pi+ X at 60 GeV"<<std::endl;
  std::cout<<"<n>: "<<get_avef("N/DPDA",0.483,60,"cpi",17,19, 0.02,0.04)<<std::endl;
  std::cout<<"xF "<<get_xf("cpi",60,"cpi",18,0.03)<<std::endl;
  std::cout<<"pT "<<get_pt(18,0.03)<<std::endl;
  
  //Given the integrated multiplcity divided by the bin momentum and solid angle widths
  // pi+ + C -> pi+ X at 12 GeV, Nuclear Physics, Section A, Volume 821, Issue 1, p. 118-192.
  //Data: 0.27 barn/GeV/sr for momentum in [3.5,4]GeV/c and angle in [50,100]mrad
  std::cout<<""<<std::endl;std::cout<<"Example for pi+ + C -> pi+ X at 12 GeV"<<std::endl;
  std::cout<<"<f>: "<<get_avef("XS/DPDO",270,12,"cpi",3.5,4, 0.05,0.10)<<std::endl;
  std::cout<<"xF "<<get_xf("cpi",12,"cpi",3.75,0.075)<<std::endl;
  std::cout<<"pT "<<get_pt(3.75,0.075)<<std::endl;

}



# ifndef __CINT__
int main(int argc, const char* argv[]){
  ExampleOfUsingFunctions();
  return 0;
}
# endif
