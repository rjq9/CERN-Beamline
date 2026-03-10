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
/// \file SteppingAction.cc
/// \brief Implementation of the B1::SteppingAction class

#include "SteppingAction.hh"

#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "TrackingAction.hh"
#include "G4Event.hh"
#include "G4LogicalVolume.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction) : fEventAction(eventAction) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  // stupid boilerplate
  if (!fScoringVolume) {
    const auto detConstruction = static_cast<const DetectorConstruction*>(
      G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detConstruction->GetScoringVolume();
  }

  // get volume of the current step
  G4LogicalVolume* volume =
    step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();


  
  // check if we are in scoring volume
  if (volume != fScoringVolume) return;

  // collect energy deposited in this step
  G4double measuredem = step->GetTotalEnergyDeposit();
  // this is just leftover from the old code, but its kinda convennient so whatever
  fEventAction->AddEdep(measuredem);

  //auto particlename = step->GetTrack()->GetDefinition()->GetParticleName();
  //auto tag = (Pi0Tag* )step->GetTrack() ->GetUserInformation();
  
  auto particlename = step->GetTrack()->GetDefinition()->GetParticleName();
  auto tag = (Pi0Tag* )step->GetTrack() ->GetUserInformation();
  
  //if (tag && tag->IsPi0) {
  //  fEventAction->addPion(1);
  // }

  if (particlename == "gamma" || particlename == "e+" || particlename == "e-") {
    fEventAction->addEnergy(measuredem);
    
    if (tag && !tag->hasHitDetector) {
      fEventAction->addGamma(1);
      tag->hasHitDetector=true;
      step->GetTrack()->SetUserInformation(tag);
    }
    if (tag && tag->IsPi0Descendant) {
      fEventAction->addPi0Energy(measuredem);

      // have we included this specific pi0?
      int m = tag->ID;
      //G4cout << std::to_string(m)<<  "<--" << G4endl;
      if (m!= -1) {
        auto result = (fEventAction->Pi0IDs).insert(m);
        if (result.second) {
          fEventAction->addPion(1);
        }
      }
    }
  }
  // adds charged pions idk why i called it judas i think i was kinda pissed 
  if ((particlename=="pi+" || particlename=="pi-")) {
    if (tag && !tag->hasHitDetector) {
      fEventAction->judas(1);
      tag->hasHitDetector = true;
      step->GetTrack()->SetUserInformation(tag);
    }
    // G4cout << "WOAH" << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B1
