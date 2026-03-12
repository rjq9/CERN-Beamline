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
/// \file EventAction.cc
/// \brief Implementation of the B1::EventAction class

#include "EventAction.hh"

#include "RunAction.hh"
#include "G4AnalysisManager.hh"

namespace B1
{


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runAction) : fRunAction(runAction) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::clearSet() {
  Pi0IDs.clear();
}

bool EventAction::isMember(int ID) {
  return (Pi0IDs.find(ID)!=Pi0IDs.end());
}


void EventAction::BeginOfEventAction(const G4Event*)
{
  fTotalEMEnergy = 0.;
  fPi0EMEnergy = 0.;
  fChargedPions = 0.;
  fEdep = 0.;
  fNeutralPions = 0;
  fGamma = 0;
  fInteractions = 0;
  // reset set for next runthrough
  clearSet();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event*)
{
  auto analysisManager = G4AnalysisManager::Instance();
  // accumulate statistics in run action
  fRunAction->AddEdep(fEdep);
  fRunAction->AddEMEnergy(fTotalEMEnergy);
  fRunAction->AddPi0EMEnergy(fPi0EMEnergy);
  fRunAction->judas(fChargedPions);

  fNeutralPions = Pi0IDs.size();
  fRunAction->AddNeutralPion(fNeutralPions);

  fRunAction->AddGamma(fGamma);
  fRunAction->AddInteraction(fInteractions);
  // log information
  /*
  analysisManager->CreateNtupleDColumn("Total EM Energy (GeV)");
  analysisManager->CreateNtupleDColumn("Total Photon Count");
  analysisManager->CreateNtupleDColumn("Total PI 0 Energy");
  analysisManager->CreateNtupleDColumn("Total PI 0 Count");
  analysisManager->CreateNtupleDColumn("Total Charged Pi Count");*/
  // VERY IMPORTANT: the factor of 1000 is the GeV scaling factor. 
  analysisManager->FillNtupleDColumn(0, fTotalEMEnergy/1000);
  analysisManager->FillNtupleDColumn(1, fGamma);
  analysisManager->FillNtupleDColumn(2, fPi0EMEnergy);
  analysisManager->FillNtupleDColumn(3,fNeutralPions);
  analysisManager->FillNtupleDColumn(4,fChargedPions);
  analysisManager->AddNtupleRow();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B1
