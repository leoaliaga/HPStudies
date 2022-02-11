#include "HPEventAction.hh"
#include "G4Event.hh"
#include "HPAnalysis.hh"
#include "HPTrackInfo.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "G4Track.hh"

HPEventAction::HPEventAction()
{
  UI = G4UImanager::GetUIpointer();
}

HPEventAction::~HPEventAction(){}

void HPEventAction::BeginOfEventAction(const G4Event* evt)
{
  G4int nEvt = evt->GetEventID();
  if(nEvt%1000==0)G4cout<<"EventID " <<nEvt<<G4endl;
  
}

void HPEventAction::AddTrack(const G4Track* aTrack)
{ 
  TrackInfo_t aTrackInfo;
  aTrackInfo.PDGcode = (aTrack->GetDefinition())->GetPDGEncoding();
  aTrackInfo.Pos.SetX(aTrack->GetPosition().x());
  aTrackInfo.Pos.SetY(aTrack->GetPosition().y());
  aTrackInfo.Pos.SetZ(aTrack->GetPosition().z());
  aTrackInfo.Mom.SetPx(aTrack->GetMomentum().x());
  aTrackInfo.Mom.SetPy(aTrack->GetMomentum().y());
  aTrackInfo.Mom.SetPz(aTrack->GetMomentum().z());
  aTrackInfo.Mom.SetE(aTrack->GetTotalEnergy());
  const HPTrackInfo* info = dynamic_cast<HPTrackInfo*>(aTrack->GetUserInformation());
  aTrackInfo.FromFast = info->fast_decay_progeny;

  TrackInfoVec.push_back(aTrackInfo);
}

void HPEventAction::EndOfEventAction(const G4Event* evt)
{

  HPAnalysis* analysis = HPAnalysis::getInstance();
  if(TrackInfoVec.size()<150)analysis->FillNtuple(TrackInfoVec);
  TrackInfoVec.clear();
}
