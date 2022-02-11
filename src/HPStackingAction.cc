
#include "HPStackingAction.hh"
#include "HPAnalysis.hh"
#include "HPTrackInfo.hh"
#include "G4Track.hh"
#include "G4HadronicProcessStore.hh"
#include "G4NistManager.hh"
#include "HPEventAction.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include <cassert>
#include <iostream>

HPStackingAction::HPStackingAction()
{
  killSecondary  = false;
  pname          = ""; 
  elm            = 0;
}

HPStackingAction::~HPStackingAction()
{}

G4ClassificationOfNewTrack
HPStackingAction::ClassifyNewTrack(const G4Track* aTrack)
{
  // when is this called with respect to UserTrackingAction
 G4ClassificationOfNewTrack status = fUrgent;
 EvtManager = G4EventManager::GetEventManager();
 HPEvtAct = (HPEventAction*)(EvtManager -> GetUserEventAction());
 // const G4ParticleDefinition* pd = aTrack->GetDefinition();
 if (aTrack->GetTrackStatus() == fAlive) {
   HPTrackInfo* info = 
     dynamic_cast<HPTrackInfo*>(aTrack->GetUserInformation());
   //   assert(info);
   if(!info &&  aTrack->GetParentID()==0) { // do nothing 
   }
   else if(!info && aTrack->GetParentID()!=0){
     // this is a problem
     std::cout<<"!info TrackID:ParentID:Name = "<<
       aTrack->GetTrackID()<<":"<<
       aTrack->GetParentID()<<":"<<
       aTrack->GetDefinition()->GetParticleName()<<std::endl;
     abort();
   }
   else{
     // OK, we want to save any track that has primary_chain and !fast_decay
     // the track info is filled in HPTrackingAction
     if(info->primary_chain==true && info->fast_decay==false){
       HPEvtAct->AddTrack(aTrack);
#ifdef DEBUG
       std::cout<<"Storing "<< aTrack->GetDefinition()->GetParticleName()
		<<"("<<aTrack->GetTrackID()<<")"<<std::endl;
#endif
     }
   }
 }

  //stack or delete secondaries
 if (killSecondary)      status = fKill;

  return status;
}
