#include "utadkmeta.h"

#include <iostream>
#include <iomanip>
#include <sstream>

//-----------------------------------------------------------------------------
ClassImp(uta::Location)
uta::Location::Location() { clear(); }
uta::Location::~Location() { ; }
uta::Location::Location(double xi, double yi, double zi, std::string namei)
  : x(xi), y(yi), z(zi), name(namei) { ; }
void uta::Location::clear(const std::string &){
  x = y = z = 0;
  name = "<<no-location>>";
}

//-----------------------------------------------------------------------------
ClassImp(uta::DkMeta)
uta::DkMeta::DkMeta() { clear(); }
uta::DkMeta::~DkMeta() { ; }
void uta::DkMeta::clear(const std::string &){
  const std::string kUnsetString = "<<unset-string>>";
  const int         kDfltInt     =           -9999999;
  const double      kDfltDouble =           -9999999.;

  job  = kDfltInt;
  pots = 0;  // initialize to sensible value

  beamsim     = kUnsetString;
  physics     = kUnsetString;
  physcuts    = kUnsetString;
  tgtcfg      = kUnsetString;
  horncfg     = kUnsetString;
  dkvolcfg    = kUnsetString;

  beam0x      = kDfltDouble;
  beam0y      = kDfltDouble;
  beam0z      = kDfltDouble;
  beamhwidth  = kDfltDouble;
  beamvwidth  = kDfltDouble;
  beamdxdz    = kDfltDouble;
  beamdydz    = kDfltDouble;
  
  location.clear();

  vintnames.clear();
  vdblnames.clear();
}

