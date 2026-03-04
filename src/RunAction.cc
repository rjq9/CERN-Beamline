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
/// \file RunAction.cc
/// \brief Implementation of the B1::RunAction class

#include "RunAction.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4AccumulableManager.hh"
#include "G4LogicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"


// for the stupid stuff
#include "G4AnalysisManager.hh"

namespace B1
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
{

  // data analysis ... 
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);

  analysisManager->CreateNtuple("data","Data");
  analysisManager->CreateNtupleDColumn("Total EM Energy (GeV)");
  analysisManager->CreateNtupleDColumn("Pion Count");
  analysisManager->FinishNtuple();
  // Register accumulable to the accumulable manager
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Register(fEdep);
  accumulableManager->Register(fEdep2);

  accumulableManager->Register(fSumEM);
  accumulableManager->Register(fSumEM2);

  accumulableManager->Register(fSumPi0EM);
  accumulableManager->Register(fSumPi0EM2);

  accumulableManager->Register(fSumChargedPi);
  accumulableManager->Register(fSumChargedPi2);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // reset accumulables to their initial values
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();
  // names the file
  auto analysisManager = G4AnalysisManager::Instance();
  auto det = static_cast<const DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  G4Material* currentmaterial = det->GetTargetMaterial();
  if (currentmaterial == NULL) {
    analysisManager->SetFileName("data_goes_here/beamOutput_default.root");
  }
  else {
    analysisManager->SetFileName("data_goes_here/beamOutput_" + currentmaterial->GetName() + ".root");
  }
  analysisManager->Reset();
  analysisManager->SetNtupleMerging(true);
  analysisManager->OpenFile();

  //analysisManager->CreateNtuple("data","Data");
  //analysisManager->CreateNtupleDColumn("Total EM Energy");
  //analysisManager->CreateNtupleDColumn("Pion Count");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // Merge accumulables
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  // not actually going to use RMS at this step - will export and do analysis later
  G4double edep = fEdep.GetValue();
  G4double edep2 = fEdep2.GetValue();

  G4double em1 = fSumEM.GetValue();
  G4double em2 = fSumEM2.GetValue();

  G4double pi1 = fSumPi0EM.GetValue();
  G4double pi2 = fSumPi0EM2.GetValue();

  G4int pic1 = fSumChargedPi.GetValue();
  G4int pic2 = fSumChargedPi2.GetValue();

  auto meanem   = em1 / nofEvents;
  
  auto rmsem    = std::sqrt(std::abs(em2/nofEvents - meanem*meanem));

  auto meanpi  = pi1 / nofEvents;
  auto rmspi   = std::sqrt(std::abs(pi2/nofEvents - meanpi*meanpi));

  auto meanpic = pic1 / nofEvents;
  auto rmspic  = std::sqrt(std::abs(pic2/nofEvents - meanpic * meanpic));


  G4double rms = edep2 - edep * edep / nofEvents;
  if (rms > 0.)
    rms = std::sqrt(rms);
  else
    rms = 0.;

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const auto generatorAction = static_cast<const PrimaryGeneratorAction*>(
    G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  if (generatorAction) {
    const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
    runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    G4double particleEnergy = particleGun->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy, "Energy");
  }

  // Print
  //
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();

  if (IsMaster()) {
    
    
    G4cout << G4endl << "--------------------End of Global Run-----------------------";
    G4cout << "Events: " << nofEvents << G4endl;
    G4cout << "Average EM energy: " << meanem/GeV << "GeV" << G4endl;
    G4cout << "Average Pi0 EM energy: " << meanpi/GeV << G4endl;
    G4cout << "Average # of charged pions:" << meanpic << G4endl;   
 
    // G4cout << "test" << rms << "&" << edep << G4endl;
  }
  else {
    ;
    //G4cout << G4endl << "--------------------End of Local Run------------------------";
  }

  
  /*G4cout << "Events: " << nofEvents << G4endl;
  G4cout << "Average EM energy: " << meanem/GeV << G4endl;
  G4cout << "Average Pi0 EM energy: " << meanpi/GeV << G4endl;
  G4cout << "Average # of charged pions:" << meanpic << G4endl;   
 
  G4cout << "test" << rms << "&" << edep << G4endl;*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::AddEdep(G4double edep)
{
  fEdep += edep;
  fEdep2 += edep * edep;
}
void RunAction::AddEMEnergy(G4double e) {
    fSumEM += e;
    fSumEM2 += e*e;
}

void RunAction::AddPi0EMEnergy(G4double e) {
    fSumPi0EM += e;
    fSumPi0EM2 += e*e;
}

void RunAction::judas(G4double n) {
    fSumChargedPi += n;
    fSumChargedPi2 += n*n;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B1
