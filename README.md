## WHAT THIS IS

This repository runs a simplified simulation of pi 0 production from proton-nucleus scattering via the Geant4 software.
The code is a bit of a mess, but it provides an accurate measurement of pion production, both charged and neutral.

## SOURCE

This code is derived - loosely - from the Geant4 example file B1, which was invaluable in understanding Geant4 structure. Some residual elements from this code still linger. 

## HOW TO USE

Assuming Geant4 is installed, this repository can be downloaded and run just like any other Geant4 program. Typically, run1.mac was used for simulation, with run2.mac as a test. Other macro files must be added to CMakeLists.txt. Once ran, the program usually takes about a minute per ~10000 events.

The target material can be swapped in the macro file with /material/setTarget {MATERIAL}. Materials must first be initialized in DetectorConstruction, though. Look for the section with nist->FindOrBuildMaterial. Lots are already installed. 

Although not included in the final proposal, this repository also logs energy deposition (total calorimeter energy, total EM cal. energy, total EM cal. energy from neutral pions.) It also logs total inelastic scattering events, which were used to determine the 'normalized' value of pi 0 production. 

The program also outputs event logs (of the statistics listed above) in the form of .ROOT files saved at build/data_goes_here. The filenames are of the form {TIMESTAMP}beamOutput_{MATERIAL}.root. It also prints averaged data into the terminal.

The codebase is quite powerful, although repurposing it might not be worth the effort (the code is very messy.)

## THANKS

The authors would like to thank the Geant4 team, for making something so powerful; the Beamline team, for their generosity; Sam W., for his motivational speech; Kaitlyn C., for her endless reassurance; and Folgers Classic Roast K-Cup pods, for their sacrifice. Dedicated to LC.