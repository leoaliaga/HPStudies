
#ifndef HPEventAction_h
#define HPEventAction_h 1
 
#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector>
#include "TrackInfo_t.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class G4Event;
class G4Track;
class G4UImanager;

class HPEventAction : public G4UserEventAction
{
public: // Without description

  HPEventAction();
  virtual ~HPEventAction();

  void BeginOfEventAction(const G4Event*);
  void   EndOfEventAction(const G4Event*);
  void AddTrack(const G4Track* aTrack);


private:

  G4UImanager*          UI;
  std::vector<TrackInfo_t> TrackInfoVec;
  
};

#endif


