
//List of functions to handle HP kinematics
//all energy units are in GeV

#include <iostream>
#include <math.h>
#include <vector>

const double mm_prt = 0.938272;
const double mm_neu = 0.939565;
const double mm_cpi = 0.139570;
const double mm_pi0 = 0.134977;
const double mm_cka = 0.493677;
const double mm_ka0 = 0.497648;

const double pival  = 3.141592;

const double xsprod_na61_incpi = 166.7;

//Get Ecm between two particles. 
//The incident particle has all the kinetic energy 
//The target particle is a proton at rest
//2 incident particle options: pi+-: cpi and proton: prt
double get_ecm(std::string inc_part, double inc_ener){

  double mass_inc_part = -1;
  if(inc_part=="prt")     mass_inc_part = mm_prt;
  else if(inc_part=="cpi")mass_inc_part = mm_cpi;
  else{
    std::cout<<"Ecm not definied for "<<inc_part<<std::endl;
    return -1.;
  }
  
  return sqrt(pow(mass_inc_part,2) + pow(mm_prt,2) + 2*inc_ener*mm_prt);
}

//Get xF value
//Inputs: - incident energy and particle type
//        - secondary particle type, momentum and angle
//Incident: prt or cpi. Secondary: prt, neu, cpi, pi0, cka, ka0
double get_xf(std::string inc_part, double inc_ener,std::string sec_part, double sec_mom,double sec_ang){
  
  double mass_inc_part = -1;
  if(inc_part=="prt")     mass_inc_part = mm_prt;
  else if(inc_part=="cpi")mass_inc_part = mm_cpi;
  else{
    std::cout<<"xF not definied for incident "<<inc_part<<std::endl;
    return -1000.;
  }  
  double mass_sec_part = 0;
  if(sec_part=="prt")     mass_sec_part = mm_prt;
  else if(sec_part=="neu")mass_sec_part = mm_neu;
  else if(sec_part=="cpi")mass_sec_part = mm_cpi;
  else if(sec_part=="pi0")mass_sec_part = mm_pi0;
  else if(sec_part=="cka")mass_sec_part = mm_cka;
  else if(sec_part=="ka0")mass_sec_part = mm_ka0;
  else{
    std::cout<<"xF not definied for secondary "<<sec_part<<std::endl;
    return -1000.;
  }
  
  double ener_cm  = get_ecm(inc_part,inc_ener);  
  double gamma_cm = (inc_ener+mm_prt)/ener_cm;
  double beta_cm  = sqrt(1. - 1./pow(gamma_cm,2));
  
  double ener_sec = sqrt( pow(sec_mom,2)+pow(mass_sec_part,2) );

  double pZ     = sec_mom*cos(sec_ang);
  return ( 2.*gamma_cm*(pZ - beta_cm*ener_sec)/ener_cm ); 
}

double get_pt(double sec_mom,double sec_ang){
  return sec_mom*sin(sec_ang);
}

//This function calculates the denominator of the 
//average invariant cross section. It is the integral for a given mom:
//Integral[ p^{2}/sqrt(p^2+mass) ]dp
double p_func(std::string sec_part, double sec_mom){
  
  double mass_sec_part = 0;
  if(sec_part=="prt")     mass_sec_part = mm_prt;
  else if(sec_part=="neu")mass_sec_part = mm_neu;
  else if(sec_part=="cpi")mass_sec_part = mm_cpi;
  else if(sec_part=="pi0")mass_sec_part = mm_pi0;
  else if(sec_part=="cka")mass_sec_part = mm_cka;
  else if(sec_part=="ka0")mass_sec_part = mm_ka0;
  else{
    std::cout<<"p_func not definied for secondary "<<sec_part<<std::endl;
    return -1000.;
  }
  
  double fact1 = (sec_mom/2.) * sqrt(pow(sec_mom,2)+pow(mass_sec_part,2));
  
  double fact2 = pow(mass_sec_part,2)/2. * log(sqrt(pow(sec_mom,2)+pow(mass_sec_part,2)) + sec_mom) / log(10);
  
  return fact1 - fact2;
    
}

//Initalization of kinematic values
//Inputs: secondary particle type and the 
//bin limits of its momentum and angle
std::vector<double> get_initialValues(std::string sec_part,double initial_mom, double final_mom, double initial_ang, double final_ang){
  std::vector<double> vec;
  vec.push_back(final_mom - initial_mom);                //0: dP
  vec.push_back(0.5*(final_mom + initial_mom));          //1: cP
  vec.push_back(final_ang - initial_ang);                //2: dT
  vec.push_back(0.5*(final_ang + initial_ang));          //3: cT
  vec.push_back(p_func(sec_part,final_mom)-p_func(sec_part,initial_mom));  //4: dFP
  vec.push_back(cos(initial_ang)-cos(final_ang));        //5: dCT
  return vec;
  
}

//Calculates average invariant cross section from
//integrated cross section divided by the bin momentum width
double avef1(double hh, double inc_ener, std::string sec_part, double initial_mom, double final_mom, double initial_ang, double final_ang){
  
  std::vector<double> ival = get_initialValues(sec_part,initial_mom,final_mom,initial_ang,final_ang);  
  return  (hh * ival[0]) / (2. * pival * ival[4] * ival[5]);
}

//Calculates average invariant cross section from
//integrated cross section divided by bin momentum and angle widths
double avef2(double hh, double inc_ener, std::string sec_part, double initial_mom, double final_mom, double initial_ang, double final_ang){
  
  std::vector<double> ival = get_initialValues(sec_part,initial_mom,final_mom,initial_ang,final_ang);  
  return (hh * ival[0] * ival[2]) / (2. * pival * ival[4] * ival[5]);
}

//Calculates average invariant cross section from
//integrated multiplicty divided by bin momentum and angle widths
double avef3(double hh, double inc_ener, std::string sec_part, double initial_mom, double final_mom, double initial_ang, double final_ang){
  
  std::vector<double> ival = get_initialValues(sec_part,initial_mom,final_mom,initial_ang,final_ang);  
  return (hh * ival[0] * ival[2] * xsprod_na61_incpi)/ (2. * pival * ival[4] * ival[5] );
  
}
//Calculates average invariant cross section from
//integrated cross section divided by bin momentum and solid angle widths
double avef4(double hh, double inc_ener, std::string sec_part, double initial_mom, double final_mom, double initial_ang, double final_ang){
  
  std::vector<double> ival = get_initialValues(sec_part,initial_mom,final_mom,initial_ang,final_ang);  
  return (hh * ival[0]) / (ival[4]);
  
}

//Get average invariant cross section given 
//Inputs: - Mode: XS/DP, XS/DPDA or N/DPDA
//        - hh: data value
//        - incident energy
//        - secondary part type
//        - initial and final momentum and angle
double get_avef(std::string dtmode, double hh, double inc_ener, std::string sec_part, 
		double initial_mom, double final_mom, double initial_ang, double final_ang){  
  if(dtmode=="XS/DP"  ) return avef1(hh,inc_ener,sec_part,initial_mom,final_mom,initial_ang,final_ang);
  if(dtmode=="XS/DPDA") return avef2(hh,inc_ener,sec_part,initial_mom,final_mom,initial_ang,final_ang);
  if(dtmode=="N/DPDA" ) return avef3(hh,inc_ener,sec_part,initial_mom,final_mom,initial_ang,final_ang);
  if(dtmode=="XS/DPDO") return avef4(hh,inc_ener,sec_part,initial_mom,final_mom,initial_ang,final_ang);
  return -1;
}
