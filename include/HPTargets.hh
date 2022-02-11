//store predefined targets for g4hp

#ifndef TARGETS_HH
#define TARGETS_HH

#include "HPConfig.hh"
#include <map>

map <G4String, Target> createTargets () //to avoid using c++11
{
  map <G4String, Target> myTargets;

  //name, mass number, atomic number, density [g/cm3], radius (opt, def = 0.3cm)
  myTargets ["Be"] = Target ("Be",   9.01,  4,  1.85);
  myTargets  ["C"] = Target ( "C",  12.01,  6,  1.78);
  myTargets ["Al"] = Target ("Al",  26.98, 13,  2.70);
  myTargets ["Cu"] = Target ("Cu",  63.55, 29,  8.96);
  myTargets ["Ag"] = Target ("Ag", 106.87, 47, 10.49);
  myTargets ["Pb"] = Target ("Pb", 207.21, 82, 11.34);

  return myTargets;
}

map <G4String, Target> HPTargets = createTargets ();

#endif //TARGETS_HH
