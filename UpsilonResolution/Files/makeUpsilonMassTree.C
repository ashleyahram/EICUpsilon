#include <iostream>
#include <cmath>

void makeUpsilonMassTree(const char* infileName = "./Q1_eicrecon_out.root", const char* outfileName = "./Q1_Upsilon1SMass.root"){

    /// Read EICrecon output file 
    TFile *f = TFile::Open(infileName, "READ");
    TTree *tree = (TTree *)f->Get("events");

    /// Create Array Reader
    TTreeReader tr(tree);

    TTreeReaderArray<Float_t> recotrk_energy(tr, "ReconstructedChargedParticles.energy");
    TTreeReaderArray<Float_t> recotrk_mass(tr, "ReconstructedChargedParticles.mass");
    TTreeReaderArray<Float_t> recotrk_px(tr, "ReconstructedChargedParticles.momentum.x");
    TTreeReaderArray<Float_t> recotrk_py(tr, "ReconstructedChargedParticles.momentum.y");
    TTreeReaderArray<Float_t> recotrk_pz(tr, "ReconstructedChargedParticles.momentum.z");
    TTreeReaderArray<Float_t> recotrk_charge(tr, "ReconstructedChargedParticles.charge");

	TFile *outfile = new TFile(outfileName, "RECREATE");
	TNtuple *UpsMNtuple = new TNtuple("UpsMNtuple", "UpsMNtuple", "m");

	TLorentzVector fourMomentum_ele;
	TLorentzVector fourMomentum_pos;
	TLorentzVector fourMomentum_ups;

	int eventnum = 0;

	while(tr.Next()){
		
		if(recotrk_px.GetSize()<2) continue;

        for(int irecotrk = 0; irecotrk < recotrk_px.GetSize(); irecotrk++){
          
            if(recotrk_charge[irecotrk] == -1){
                Double_t legele_energy_Reco = TMath::Sqrt(recotrk_px[irecotrk] * recotrk_px[irecotrk] + recotrk_py[irecotrk] * recotrk_py[irecotrk] + recotrk_pz[irecotrk] * recotrk_pz[irecotrk] + recotrk_mass[irecotrk] * recotrk_mass[irecotrk]);
                fourMomentum_ele.SetPxPyPzE(recotrk_px[irecotrk], recotrk_py[irecotrk], recotrk_pz[irecotrk], legele_energy_Reco);
            }
            else{
                Double_t legpos_energy_Reco = TMath::Sqrt(recotrk_px[irecotrk] * recotrk_px[irecotrk] + recotrk_py[irecotrk] * recotrk_py[irecotrk] + recotrk_pz[irecotrk] * recotrk_pz[irecotrk] + recotrk_mass[irecotrk] * recotrk_mass[irecotrk]);
                fourMomentum_pos.SetPxPyPzE(recotrk_px[irecotrk], recotrk_py[irecotrk], recotrk_pz[irecotrk], legpos_energy_Reco);
            }
        }

        fourMomentum_ups = fourMomentum_ele + fourMomentum_pos;
        UpsMNtuple -> Fill(fourMomentum_ups.M());
        eventnum++;
    }

    /// Write Histograms into a file	
	UpsMNtuple -> Write();
	outfile -> Close();
}