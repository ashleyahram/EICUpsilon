# eSTARlight Upsilon Production

This simulates proton-electron collisions using eSTARlight for Upsilon (nS) production in electron channel and muon channel.

## Installation
Install eSTARlight as described here: https://github.com/ashleyahram/estarlight/tree/b8c6b02abe8600b6d5ed932539b1ade07577f6dc

## Particle Production
Run eSTARlight for Upsilon (nS) production: <br>
Run the simulation each of the Upsilon state using the provided input cards and combine the hepmc files <br>

input cards: <br>
(electron channel) <br>
Upsilon (1S): https://github.com/ashleyahram/EICUpsilon/blob/main/UpsilonResolution/Generator/slight_Upsilon1S_electron.in <br>
Upsilon (2S): https://github.com/ashleyahram/EICUpsilon/blob/main/UpsilonResolution/Generator/slight_Upsilon2S_electron.in <br>
Upsilon (3S): https://github.com/ashleyahram/EICUpsilon/blob/main/UpsilonResolution/Generator/slight_Upsilon3S_electron.in <br>

(muon channel) <br>
Upsilon (1S): https://github.com/ashleyahram/EICUpsilon/blob/main/UpsilonResolution/Generator/slight_Upsilon1S_muon.in <br>
Upsilon (2S): https://github.com/ashleyahram/EICUpsilon/blob/main/UpsilonResolution/Generator/slight_Upsilon2S_muon.in <br>
Upsilon (3S): https://github.com/ashleyahram/EICUpsilon/blob/main/UpsilonResolution/Generator/slight_Upsilon3S_muon.in <br>

locate the input card under the estarlight install directory and change the name: <br>
e.g. <br>
```mv slight_Upsilon1S_electron.in slight.in``` <br>

run the simulation: <br>
```./e_starlight > output.txt``` <br>

combine the HEPMC outputs of the three Upsilon states into one: <br>
```wget https://github.com/ashleyahram/EICUpsilon/blob/main/UpsilonResolution/Generator/combineHepMC.sh``` <br>
```./combineHepMC.sh``` <br>

## Add Beam Effects with AfterBurner
Get and run eic-shell

Take the generated outputs and run afterburner with the right setting for example
  abconv -p ip6_hidiv_275x18 hepmc_Lambda1.dat
