#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "G4VUserTrackInformation.hh"
#include "globals.hh"

class G4Track;


class TrackingAction : public G4UserTrackingAction
{
public:
    TrackingAction();
    virtual ~TrackingAction();

    // Called before a new track starts
    virtual void PreUserTrackingAction(const G4Track* track) override;

    // Called after a track ends
    virtual void PostUserTrackingAction(const G4Track* track) override;

    // upcoming pi 0 ID
    static int nextID;
};
class Pi0Tag  : public G4VUserTrackInformation {
public:
    bool IsPi0;
    bool IsPi0Descendant;
    // used for uniqueness
    int ID;

    // used to ensure we log total counts just once
    bool hasHitDetector=false;
};
#endif