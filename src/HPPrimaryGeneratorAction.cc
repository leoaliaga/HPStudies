
#include "HPPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "Randomize.hh"
#include "HPAnalysis.hh"

HPPrimaryGeneratorAction::HPPrimaryGeneratorAction()
{
  particleGun  = new G4ParticleGun(1);
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
}

HPPrimaryGeneratorAction::~HPPrimaryGeneratorAction()
{
  delete particleGun;
}

void HPPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4double zVertex = -6.5*CLHEP::mm;
  particleGun->SetParticlePosition(G4ThreeVector(0.,0.,zVertex));
  particleGun->GeneratePrimaryVertex(anEvent);
  
  HPAnalysis* analysis = HPAnalysis::getInstance();
  Double_t ener = particleGun->GetParticleEnergy();
  G4ParticleDefinition* part  = particleGun->GetParticleDefinition();
  analysis->GetPrimGenInfo(ener,part);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
