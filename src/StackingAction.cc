#include "StackingAction.hh"
#include "G4Track.hh"

StackingAction::StackingAction() {}
StackingAction::~StackingAction() {}

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack (const G4Track* track) {
    return fUrgent;
}