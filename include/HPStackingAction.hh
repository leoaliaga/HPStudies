
#ifndef HPStackingAction_h
#define HPStackingAction_h 1

#include "G4UserStackingAction.hh"
#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "HPAnalysis.hh"

class G4Track;
class G4EventManager;
class HPEventAction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HPStackingAction : public G4UserStackingAction
{
public:

  HPStackingAction();
  virtual ~HPStackingAction();
   
  void SetKillStatus(G4bool value)    {killSecondary = value;};
  void SetKill(const G4String& name)  {pname = name;};
     
  G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);
    
private:

  G4String                    pname;
  G4bool                      killSecondary;
  const G4ParticleDefinition* primaryDef;
  G4double                    primaryTotalEnergy;
  G4Element*                  elm;
  G4EventManager *EvtManager;
  HPEventAction *HPEvtAct;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

