
#ifndef HPRunAction_h
#define HPRunAction_h 1

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "globals.hh"
#include "HPConfig.hh"

class HPRunAction : public G4UserRunAction
{
public:
  
  HPRunAction(const HPConfig &config_) : config(config_) {};
  ~HPRunAction();
  
public: 
  
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);

private:

  HPConfig config;
  
};

#endif

