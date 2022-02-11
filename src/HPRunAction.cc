
#include "HPRunAction.hh"

#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4NistManager.hh"
#include "G4Element.hh"

#include "HPAnalysis.hh"
#include "Randomize.hh"

HPRunAction::~HPRunAction()
{}

void HPRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4int id = aRun->GetRunID();
  G4cout << "### Run " << id << " start" << G4endl;

  const G4long* table_entry;
  table_entry = CLHEP::HepRandom::getTheSeeds();
  G4long id0 = table_entry[0];
  G4long id1 = table_entry[1];
  G4cout << "### Random numbers check: " << id0 << " "<< id1 << G4endl;
 
#ifdef G4VIS_USE
  G4UImanager* UI = G4UImanager::GetUIpointer();

  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();

  if(pVVisManager){
    UI->ApplyCommand("/vis/scene/notifyHandlers");
  }
#endif

  HPAnalysis* analysis = HPAnalysis::getInstance();
  analysis->book(config);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HPRunAction::EndOfRunAction(const G4Run*)
{
  G4cout << "RunAction: End of run actions are started" << G4endl;

#ifdef G4VIS_USE
  if (G4VVisManager::GetConcreteInstance())
    G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");
#endif

  HPAnalysis* analysis = HPAnalysis::getInstance();
  analysis->finish();

}
