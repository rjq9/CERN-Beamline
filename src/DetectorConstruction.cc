//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file DetectorConstruction.cc
/// \brief Implementation of the B1::DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Trd.hh"
// needed to modify geometry
#include "G4RunManager.hh"

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Setup
  G4NistManager* nist = G4NistManager::Instance();
  G4bool checkOverlaps = true;
  G4Material* targetMaterial = nist->FindOrBuildMaterial("G4_C");
  nist->FindOrBuildMaterial("G4_POLYETHYLENE");
  nist->FindOrBuildMaterial("G4_Al");
  nist->FindOrBuildMaterial("G4_Pb");
  nist->FindOrBuildMaterial("G4_Fe");
  auto messenger = new G4GenericMessenger(this,
                                     "/material/",
                                     "Material control");

  messenger->DeclareMethod("setTarget",
                          &DetectorConstruction::settarget,
                          "Set target material");
  // Parameters

  //G4double env_sizeXY = 100 * cm, env_sizeZ = 300 * cm;
  //G4Material* env_mat = nist->FindOrBuildMaterial("G4_AIR");

  G4double world_sizeXY = 100 * cm;
  G4double world_sizeZ = 300 * cm;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  // Building stage
  auto solidWorld =
    new G4Box("World",  // its name
              0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);  // its size
  auto logicWorld = new G4LogicalVolume(solidWorld,  // its solid
                                        world_mat,  // its material
                                        "World");  // its name

  auto physWorld = new G4PVPlacement(nullptr,  // no rotation
                                     G4ThreeVector(),  // at (0,0,0)
                                     logicWorld,  // its logical volume
                                     "World",  // its name
                                     nullptr,  // its mother  volume
                                     false,  // no boolean operation
                                     0,  // copy number
                                     checkOverlaps);  // overlaps checking
  

  // relative nuclear densities
  // Pb : 0.0547; C : 0.175; H in CH2 : 0.026; C in : 0.013; Al : 0.100; Fe : 0.141
  
  G4double targetXY = 10 * cm;
  G4double targetThickness = 10 * cm;
  G4double targetDistance = 30 * cm;
  

  
  auto solidTarget = new G4Box("Target", targetXY, targetXY, targetThickness);
  logicTarget = new G4LogicalVolume(solidTarget, targetMaterial, "Target");
  new G4PVPlacement(nullptr, G4ThreeVector(0,0,targetDistance),logicTarget, "Target", logicWorld, false, 0, checkOverlaps);

  G4double detectorXY = 10 * cm;
  G4double detectorThickness = 10 * cm;
  G4double detectorDistance = 50 * cm;
  // ~ lead glass
  G4Material* detectorMaterial = nist->FindOrBuildMaterial("G4_GLASS_LEAD");
  auto solidDetector = new G4Box("Det", detectorXY, detectorXY, detectorThickness);
  auto logicDetector = new G4LogicalVolume(solidDetector, detectorMaterial, "Det");
  new G4PVPlacement(nullptr, G4ThreeVector(0,0,detectorDistance),logicDetector, "Det", logicWorld, false, 0, checkOverlaps);
  
  // this is what's actually yk detecting stuff
  fScoringVolume = logicDetector;

  /*autosolidEnv = new G4Box("Envelope",  // its name
                            0.5 * env_sizeXY, 0.5 * env_sizeXY, 0.5 * env_sizeZ);  // its size
  auto logicEnv = new G4LogicalVolume(solidEnv,  // its solid
                                      env_mat,  // its material
                                      "Envelope");  // its name
  new G4PVPlacement(nullptr,  // no rotation
                    G4ThreeVector(),  // at (0,0,0)
                    logicEnv,  // its logical volume
                    "Envelope",  // its name
                    logicWorld,  // its mother  volume
                    false,  // no boolean operation
                    0,  // copy number
                    checkOverlaps);  // overlaps checking
  G4Material* shape1_mat = nist->FindOrBuildMaterial("G4_C");
  G4ThreeVector pos1 = G4ThreeVector(0, 2 * cm, -7 * cm);
  */
  // Conical section shape
  /*G4double shape1_rmina = 0. * cm, shape1_rmaxa = 2. * cm;
  G4double shape1_rminb = 0. * cm, shape1_rmaxb = 4. * cm;
  G4double shape1_hz = 3. * cm;
  G4double shape1_phimin = 0. * deg, shape1_phimax = 360. * deg;
  auto solidShape1 = new G4Cons("Shape1", shape1_rmina, shape1_rmaxa, shape1_rminb, shape1_rmaxb,
                                shape1_hz, shape1_phimin, shape1_phimax);

  auto logicShape1 = new G4LogicalVolume(solidShape1,  // its solid
                                         shape1_mat,  // its material
                                         "Shape1");  // its name

  new G4PVPlacement(nullptr,  // no rotation
                    pos1,  // at position
                    logicShape1,  // its logical volume
                    "Shape1",  // its name
                    logicEnv,  // its mother  volume
                    false,  // no boolean operation
                    0,  // copy number
                    checkOverlaps);  // overlaps checking

  //
  // Shape 2
  //
  G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_C");
  G4ThreeVector pos2 = G4ThreeVector(0, -1 * cm, 7 * cm);

  // Trapezoid shape
  G4double shape2_dxa = 12 * cm, shape2_dxb = 12 * cm;
  G4double shape2_dya = 10 * cm, shape2_dyb = 16 * cm;
  G4double shape2_dz = 6 * cm;
  auto solidShape2 =
    new G4Trd("Shape2",  // its name
              0.5 * shape2_dxa, 0.5 * shape2_dxb, 0.5 * shape2_dya, 0.5 * shape2_dyb,
              0.5 * shape2_dz);  // its size

  auto logicShape2 = new G4LogicalVolume(solidShape2,  // its solid
                                         shape2_mat,  // its material
                                         "Shape2");  // its name

  new G4PVPlacement(nullptr,  // no rotation
                    pos2,  // at position
                    logicShape2,  // its logical volume
                    "Shape2",  // its name
                    logicEnv,  // its mother  volume
                    false,  // no boolean operation
                    0,  // copy number
                    checkOverlaps);  // overlaps checking

  // Set Shape2 as scoring volume
  //
  */
  
  

  //
  // always return the physical World
  //
  return physWorld;
}

// updates target material. can be configured using /material/setTarget "target"

void DetectorConstruction:: settarget(const G4String &name) {
    auto nist = G4NistManager::Instance();
    G4Material* newmaterial = G4Material::GetMaterial(name);
    
    if (newmaterial == NULL) {
      G4Material* newmaterial = nist->FindOrBuildMaterial(name);
    }
    if (newmaterial == NULL) {
      G4cout << "No material found with name: " << name << G4endl;
    }
    else {
      if (logicTarget) {
        G4cout << "Swapped target material to " << name << G4endl;
        logicTarget->SetMaterial(newmaterial);
        targetMaterial = newmaterial;
        G4RunManager::GetRunManager()->GeometryHasBeenModified();
      }
      else {
        G4cout << "No logical target found." << G4endl;
      }
    }
    
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B1
