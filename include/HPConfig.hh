//HPConf handles settings (beam, target, #events) from command line;

#ifndef HP_CONFIG_HH
#define HP_CONFIG_HH

#include <getopt.h>

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "Randomize.hh"
#include <TTree.h>

using namespace std;

//store information about target
struct Target
{
  G4String name;      //e.g. C, Al
  G4double A;         //mass number
  G4int    Z;         //atomic number
  G4double density;   //in g/cm3
  G4double radius;    //in cm

  Target (G4String name_ = "", G4double A_ = 0, G4int Z_ = 0,
          G4double density_ = 0, G4double radius_ = 0.3) :
          name(name_), A(A_), Z(Z_), density(density_), radius(radius_) {};
  void print () const;
};

//store information about beam
struct Beam
{
  G4String particle;  //e.g. proton
  G4String energy;	  //in GeV

  Beam (G4String particle_ = "", G4String energy_ = "") :
        particle(particle_), energy(energy_) {};
  void print () const;
};

//read and store configuration for g4na49 defined by command line arguments
class HPConfig
{
  public:

    HPConfig (int argc, char **argv);
    inline Target getTarget() const {return target;};
    inline Beam getBeam() const {return beam;};
    inline G4String getphysicsList() const {return physicsList;};
    inline G4String getNevents() const {return nEvents;};
    inline G4String getRunNumber() const {return runNumber;};
    inline G4String getOutputFile() const {return outputFile;};
    inline G4String getOutputDir() const {return outputDir;};
    TTree* createTree() const; //create ROOT TTree with sim config

  private:

    void usage() const;
    //set predefined target (from HPTargets.hh) if exists
    void setTarget (const G4String &targetName);
    //display chosen configuration and wait for user confirmation
    void checkConf() const;			

    Target target;
    Beam beam;
    G4String physicsList;      //e.g. QGSP_BERT
    G4String nEvents;
    G4String runNumber;       //used in random seed and auto outputFile
    G4String outputFile;      //output root file
    G4String outputDir;       //output root directory

    bool isConfirmed;         //checkConf will be called if false
};
    
#endif //HP_CONFIG_HH
