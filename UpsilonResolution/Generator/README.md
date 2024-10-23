# eSTARlight Upsilon Production

This simulates proton-electron collisions using eSTARlight for Upsilon (nS) production in electron channel and muon channel.

## Installation
Install eSTARlight as described here: https://github.com/ashleyahram/estarlight/tree/b8c6b02abe8600b6d5ed932539b1ade07577f6dc

## Particle Production
Run eSTARlight for Upsilon (nS) production: <br>
Simulate each of the Upsilon state using the provided input cards and combine the hepmc files into one. <br>
  
  input cards:
  
    # (electron channel)
    # Upsilon (1S):
    wget https://raw.githubusercontent.com/ashleyahram/EICUpsilon/refs/heads/main/UpsilonResolution/Generator/slight_Upsilon1S_electron.in
    # Upsilon (2S): 
    wget https://raw.githubusercontent.com/ashleyahram/EICUpsilon/refs/heads/main/UpsilonResolution/Generator/slight_Upsilon2S_electron.in
    # Upsilon (3S)
    wget https://raw.githubusercontent.com/ashleyahram/EICUpsilon/refs/heads/main/UpsilonResolution/Generator/slight_Upsilon3S_electron.in
    
    # (muon channel)
    # Upsilon (1S):
    wget https://raw.githubusercontent.com/ashleyahram/EICUpsilon/refs/heads/main/UpsilonResolution/Generator/slight_Upsilon1S_muon.in
    # Upsilon (2S): 
    wget https://raw.githubusercontent.com/ashleyahram/EICUpsilon/refs/heads/main/UpsilonResolution/Generator/slight_Upsilon2S_muon.in
    # Upsilon (3S)
    wget https://raw.githubusercontent.com/ashleyahram/EICUpsilon/refs/heads/main/UpsilonResolution/Generator/slight_Upsilon3S_muon.in

    
  * locate the input card under the estarlight install directory and change the name: <br>
  e.g. <br>
  ```mv slight_Upsilon1S_electron.in slight.in``` <br>
  
  * run the simulation: <br>
  ```./e_starlight > output.txt``` <br>

  * combine the HEPMC outputs of the three Upsilon states into one: <br>
  ```wget https://github.com/ashleyahram/EICUpsilon/blob/main/UpsilonResolution/Generator/combineHepMC.sh``` <br>
  ```./combineHepMC.sh <file1Name> <file2Name> <file3Name> <combinedFileName>``` <br>

## Add Beam Effects with AfterBurner
Get and run eic-shell

Take the generated outputs and run afterburner with the right setting for example
  ```abconv <inputfileName>.hepmc -o <outputfileName> -l 178000```
