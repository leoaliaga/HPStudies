
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "Randomize.hh"

#include "HPDetectorConstruction.hh"

#include "G4PhysListFactory.hh"

#include "HPPrimaryGeneratorAction.hh"

#include "HPRunAction.hh"
#include "HPEventAction.hh"
#include "HPStackingAction.hh"
#include "HPTrackingAction.hh"
#include "HPConfig.hh"

#include <iostream>
#include <fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv) {
  HPConfig config (argc, argv);
  
  //choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
  
  //Construct the default run manager
  G4RunManager * runManager = new G4RunManager();
  
  //set mandatory initialization classes
  runManager->SetUserInitialization(new HPDetectorConstruction(config.getTarget()));
  
  G4PhysListFactory factory;
  G4VModularPhysicsList *phys = 0;
  if(factory.IsReferencePhysList(config.getphysicsList())){
    phys = factory.GetReferencePhysList(config.getphysicsList());
  }
  else {
    G4cout << "Couldn't find physics list name " << config.getphysicsList() << G4endl;
    G4cout << "Exiting" << G4endl;
    exit(0);
  }
  runManager->SetUserInitialization(phys); 
  
  runManager->SetUserAction(new HPPrimaryGeneratorAction());
  
  //set user action classes
  runManager->SetUserAction(new HPRunAction(config));
  runManager->SetUserAction(new HPEventAction());
  runManager->SetUserAction(new HPStackingAction());
  runManager->SetUserAction(new HPTrackingAction());
  
  // get the pointer to the UI manager and set verbosities
  G4UImanager* UI = G4UImanager::GetUIpointer();
  
  UI->ApplyCommand("/control/verbose 0");
  UI->ApplyCommand("/run/verbose 0");
  UI->ApplyCommand("/tracking/verbose 0");
  UI->ApplyCommand("/testhadr/Update");
  UI->ApplyCommand("/run/initialize");
  
  UI->ApplyCommand("/random/setSeeds " + config.getBeam().energy + " "
                   + config.getRunNumber());
  UI->ApplyCommand("/gun/particle " + config.getBeam().particle);
  UI->ApplyCommand("/gun/energy " + config.getBeam().energy + " GeV");
  UI->ApplyCommand("/run/beamOn " + config.getNevents());
  
  UI->ApplyCommand("/control/execute");
  
  delete runManager;
  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
