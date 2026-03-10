#include "TrackingAction.hh"
#include "G4Track.hh"
#include "G4PionZero.hh"
#include "G4VUserTrackInformation.hh"
#include "G4TrackingManager.hh"

TrackingAction::TrackingAction(): G4UserTrackingAction() {
    
}

TrackingAction::~TrackingAction() {
}

void TrackingAction::PreUserTrackingAction(const G4Track* track) {
    auto og_tag = dynamic_cast<Pi0Tag*>(track->GetUserInformation());
    auto tag = new Pi0Tag();
    tag->IsPi0 = false;
    if (og_tag) {
        tag->IsPi0Descendant = og_tag->IsPi0Descendant;
    }
    //tag->IsPi0Descendant = false;
    if(track->GetDefinition() == G4PionZero::PionZero()) {
        tag->IsPi0 = true;
        tag->IsPi0Descendant = true;
    }
    
    /*// 22 is the gamma ray encoding
    if (track->GetDefinition()->GetPDGEncoding() == 22) {
        if (track->HasParentResonance()) {
            G4int parentType = track->GetParentResonancePDGEncoding();
            
            // this is the pi0 encoding
            if (parentType == 111) {
                tag->IsPi0 = true;
            }
            if (parentType) {
                G4cout << "Parent Type: " << parentType << G4endl;
            }
        }
    }*/
    track->SetUserInformation(tag);
}

void TrackingAction::PostUserTrackingAction(const G4Track* track) {
    auto secondaries = fpTrackingManager->GimmeSecondaries();
    if(!secondaries) return;
    auto parent_tag = (Pi0Tag*) track->GetUserInformation();
    bool q = parent_tag->IsPi0 || parent_tag->IsPi0Descendant;
    
    for(auto sec : *secondaries) {
        auto tag = new Pi0Tag();
        tag->IsPi0Descendant = q;
        sec->SetUserInformation(tag);
    }
}
    

    

