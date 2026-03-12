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
/// \file EventAction.hh
/// \brief Definition of the B1::EventAction class

#ifndef B1EventAction_h
#define B1EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <set>

class G4Event;

namespace B1
{


class RunAction;
// realistically this should be in a class but idrc
// pi 0 logging feature: list of pi0 IDs


class EventAction : public G4UserEventAction
{
  public:
  /// Event action class
    std::set<int> Pi0IDs;
    //std::pair<iterator, bool> insertID(int ID);
    void clearSet();
    bool isMember(int ID);

    EventAction(RunAction* runAction);
    ~EventAction() override = default;
    void BeginOfEventAction(const G4Event* event) override;
    void EndOfEventAction(const G4Event* event) override;

    void addEnergy (G4double energy) {fTotalEMEnergy += energy;}
    void addPi0Energy (G4double energy) {fPi0EMEnergy += energy;}
    void judas (G4int count) {fChargedPions += count;}
    void AddEdep(G4double edep) { fEdep += edep; }
    void addPion(G4int count) { fNeutralPions += count; }
    void addGamma(G4int count) {fGamma += count; }
    void addInteraction(G4int count) {fInteractions+= count;}

  private:
    RunAction* fRunAction = nullptr;
    G4double fEdep = 0.;
    G4double fTotalEMEnergy;
    G4double fPi0EMEnergy;
    G4int    fChargedPions;
    G4int    fNeutralPions;
    G4int    fGamma;
    G4int    fInteractions;
};

}  // namespace B1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
