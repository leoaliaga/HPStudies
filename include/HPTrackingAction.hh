#ifndef HPTrackingAction_h
#define HPTrackingAction_h

#include "globals.hh"
#include "G4UserTrackingAction.hh"

class G4Track;

class HPTrackingAction : public G4UserTrackingAction {
public:
  HPTrackingAction();
  virtual ~HPTrackingAction();

  virtual void PostUserTrackingAction(const G4Track*);

};

#endif
