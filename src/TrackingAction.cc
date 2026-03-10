#include "TrackingAction.hh"
#include "G4Track.hh"
#include "G4PionZero.hh"
#include "G4VUserTrackInformation.hh"
#include "G4TrackingManager.hh"
// #include "EventAction.hh"

int TrackingAction::nextID = 0;

TrackingAction::TrackingAction(): G4UserTrackingAction() {
    
}

TrackingAction::~TrackingAction() {
}
// this code is crazy inefficient / redundant but whatever 
void TrackingAction::PreUserTrackingAction(const G4Track* track) {
    auto og_tag = dynamic_cast<Pi0Tag*>(track->GetUserInformation());

    auto tag = new Pi0Tag();

    if (og_tag) {
        tag->IsPi0Descendant = og_tag->IsPi0Descendant;
        tag->ID = og_tag->ID;
        tag->IsPi0 = og_tag->IsPi0;
    }
    else {
        // defaults
        tag->IsPi0 = false;
        // for when it's irrelevant
        tag->ID = -1;
        tag->IsPi0Descendant = false;
    }
    if(track->GetDefinition() == G4PionZero::PionZero()) {
        tag->IsPi0 = true;
        tag->IsPi0Descendant = true;
        tag->ID = nextID;
        nextID++;
        //G4cout << "->>" << std::to_string(nextID) << G4endl;
    }

    track->SetUserInformation(tag);
}

void TrackingAction::PostUserTrackingAction(const G4Track* track) {
    auto secondaries = fpTrackingManager->GimmeSecondaries();
    if(!secondaries) return;

    auto parent_tag = (Pi0Tag*) track->GetUserInformation();
    bool q = parent_tag->IsPi0 || parent_tag->IsPi0Descendant;
    if (q) {
        for(auto sec : *secondaries) {
            auto tag = new Pi0Tag();
            tag->IsPi0Descendant = q;
            tag->ID = parent_tag->ID;
            sec->SetUserInformation(tag);
        }
    }
}
    

    

