
#include "HPDetectorConstruction.hh"

#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4RunManager.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4UnitsTable.hh"
#include "G4ios.hh"

#include "G4NistManager.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPDetectorConstruction::HPDetectorConstruction(const Target &t)
{
  logicTarget = 0;
  logicWorld  = 0;

  //Default is carbon
  TargetZ = t.Z;
  TargetA = t.A*CLHEP::g/CLHEP::mole;
  TargetDensity = t.density*CLHEP::g/CLHEP::cm3;

  radius = t.radius*CLHEP::cm;

  targetMaterial = new G4Material(t.name,TargetZ, TargetA, TargetDensity);
  worldMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPDetectorConstruction::~HPDetectorConstruction()
{ 
}

G4VPhysicalVolume* HPDetectorConstruction::Construct()
{
  // Cleanup old geometry

  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // Sizes
  G4double worldR       = radius + CLHEP::cm;
  G4double targetLenght = 0.7*CLHEP::cm*0.5; 
  G4double worldZ       = targetLenght + CLHEP::cm;

  // World
  G4Tubs* solidW           = new G4Tubs("World",0.,worldR,worldZ,0.,CLHEP::twopi);
  logicWorld               = new G4LogicalVolume( solidW,worldMaterial,"World");
  G4VPhysicalVolume* world = new G4PVPlacement(0,G4ThreeVector(),
					       logicWorld,"World",0,false,0);
  // Target volume
  G4Tubs* solidT = new G4Tubs("Target",0.,radius,targetLenght,0.,CLHEP::twopi);
  logicTarget    = new G4LogicalVolume( solidT,targetMaterial,"Target");
  new G4PVPlacement(0,G4ThreeVector(),logicTarget,"Target",logicWorld,false,0);
  
  //See this (Leo)??
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  return world;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HPDetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

