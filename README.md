## NOTE FOR THE FOLKS AT CERN BEAMLINE

todo: write this

## TODO


make histograms per-gamma-energy level, as opposed to per-total-deposited-level

graphs


## USE

feel free to modify vars in detectorconstruction. note that materials you want to use in the macrofile have to be declared first: if i want to use G4_AIR as my target i should write

nist->FindOrBuildMaterial("G4_AIR");

in the detectorconstruction::construct () bit

currently using .csv creates a bunch of files, this can be disabled by disabling multithread but that might be slower. using .root right now