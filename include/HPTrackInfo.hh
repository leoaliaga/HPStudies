#ifndef HPTrackInfo_h
#define HPTrackInfo_h

#include "globals.hh"
//#include "G4ParticleDefinition.hh"
//#include "HPAnalysis.hh"

#include "G4VUserTrackInformation.hh"

class HPTrackInfo : public G4VUserTrackInformation {
public:
  HPTrackInfo();
  virtual ~HPTrackInfo();
  virtual void Print() const;
  bool primary_chain; // did the primary particle make this one?
  bool fast_decay; // does this particle decay quickly?
  bool fast_decay_progeny; // did this particle have a quickly decaying anscestor?

};


#endif
