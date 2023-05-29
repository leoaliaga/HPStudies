
#ifndef UTA_DKMETA_H
#define UTA_DKMETA_H

//Copy of the dkmeta file just for temporary use 
//until access to gpvms (Leo, April 17 2023) 

#include "TROOT.h"
#include "TObject.h"

#include <vector>
#include <string>

#define UTADKMETAVER 1   // KEEP THIS UP-TO-DATE!  increment for each change

namespace uta {
 
  class Location{
  public:
    Double_t x;        ///< x position of location
    Double_t y;        ///< y position of location
    Double_t z;        ///< z position of location
    std::string name;  ///< identifying name
    
  public:
    Location();
    Location(double x, double y, double z, std::string name);
    virtual     ~Location();
    void        clear(const std::string &opt = "");    ///< reset everything
    
  private:
    ClassDef(uta::Location,UTADKMETAVER)
  };  // end-of-class uta::Location

  ///---------------------------------------------------------------------------
  /**
   *============================================================================
   *  Metadata for the flux file
   */
  class DkMeta{
  public:

    /**
     *============================================================================
     *  General information
     */
    Int_t    job;           ///< identifying job # (keep files distinct)
    Double_t pots;          ///< protons-on-target
    
    /**
     * DKMETA:  
     * formatted strings are most flexible ...
     * but not necessarily convenient to use
     * ??? Should parts of these be standardized ??? 
     */
    std::string beamsim;    ///< e.g. "flugg" or "g4numi/<tag>"
    std::string physics;    ///< e.g. "fluka08", "g4.9.3p01"
    std::string physcuts;   ///< tracking cuts    e.g. "threshold=0.1GeV"
    std::string tgtcfg;     ///< target config    e.g. "minos/epoch3/-10cm"
    std::string horncfg;    ///< horn config      e.g. "FHC/185A/LE/h1xoff=1mm"
    std::string dkvolcfg;   ///< decay vol config e.g. "helium" or "vacuum"
    
    /**
     *============================================================================
     *  Beam Info:
     */
    Double_t beam0x;       ///< x of beam center at start
    Double_t beam0y;       ///< y of beam center at start
    Double_t beam0z;       ///< z of beam start
    Double_t beamhwidth;   ///< horizontal width of beam
    Double_t beamvwidth;   ///< vertical width of beam
    Double_t beamdxdz;     ///< beam slope dx/dz
    Double_t beamdydz;     ///< beam slope dy/dz
    
    /**
     *============================================================================
     *  Detector Position Info:
     *  Values are in beam coordinate system w/ units of "cm"
     */
    std::vector<uta::Location> location;   ///< locations
    
    /**
     *============================================================================
     *  Special Info:
     *  Document extensibility enhancements 
     */
    std::vector<std::string> vintnames;    ///< names of elements for user defined vector of integers
    std::vector<std::string> vdblnames;    ///< names of elements for user defined vector of doubles
    
  public:
    /**
     *   Public methods for constructing/destruction and resetting the data
     */
    DkMeta();
    virtual     ~DkMeta();
    void        clear(const std::string &opt = "");    ///< reset everything
    
  private:
    ClassDef(uta::DkMeta,UTADKMETAVER)
    
  }; // end-of-class uta::DkMeta

} // end-of-namespace "uta"


#endif   // uta_DKMETA_H
