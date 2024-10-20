#include <iostream>
#include <cmath>

#include "../Style/GetPullDistribution_ele.h"
#include "../Style/Legends_ele.h"

#include "RooRealVar.h"
#include "RooDataSet.h"

#include "../Style/tdrStyle.C"
#include "../Style/CMS_lumi.C"

#include "TLegend.h"

using namespace RooFit;

int eEnergy = 18; // GeV
int pEnergy = 275; // GeV

void skimEICReconOutput(int ymin = 0, int ymax = 1){

    const char* infileName = "../Files/ep_UpsilonCombined_18x275_podio_output.root";

    const char* outfileName = Form("../Files/ep_UpsilonCombined_18x275_podio_seeded_MCntuple_y%d_%d.root", ymin, ymax);


    /// Read EICrecon output file 
    TFile* f = TFile::Open(infileName, "READ");
    TTree* eicreconTree = (TTree *)f->Get("events");

    /// Create Array Reader
    TTreeReader tr(eicreconTree);

    TTreeReaderArray<Float_t> px_mc(tr,"GeneratedParticles.momentum.x");
    TTreeReaderArray<Float_t> py_mc(tr,"GeneratedParticles.momentum.y");
    TTreeReaderArray<Float_t> pz_mc(tr,"GeneratedParticles.momentum.z");
    TTreeReaderArray<Float_t> E_mc(tr,"GeneratedParticles.energy");
    TTreeReaderArray<Float_t> M_mc(tr,"GeneratedParticles.mass");
    TTreeReaderArray<Int_t>   PDG_mc(tr,"GeneratedParticles.PDG");
    TTreeReaderArray<Int_t>   type_mc(tr,"GeneratedParticles.type");

    // TTreeReaderArray<Float_t> px_track(tr,"ReconstructedChargedParticles.momentum.x");
    // TTreeReaderArray<Float_t> py_track(tr,"ReconstructedChargedParticles.momentum.y");
    // TTreeReaderArray<Float_t> pz_track(tr,"ReconstructedChargedParticles.momentum.z");
    // TTreeReaderArray<Float_t> E_track(tr,"ReconstructedChargedParticles.energy");
    // TTreeReaderArray<Float_t> M_track(tr,"ReconstructedChargedParticles.mass");
    // TTreeReaderArray<Int_t> PDG_track(tr,"ReconstructedChargedParticles.PDG");
    // TTreeReaderArray<Int_t> type_track(tr,"ReconstructedChargedParticles.type");
    // TTreeReaderArray<Int_t> simID_track(tr,"_ReconstructedChargedParticleAssociations_sim.index");
    // TTreeReaderArray<Float_t> charge_track(tr,"ReconstructedChargedParticles.charge");

    TTreeReaderArray<Float_t> px_track(tr,"ReconstructedSeededChargedParticles.momentum.x");
    TTreeReaderArray<Float_t> py_track(tr,"ReconstructedSeededChargedParticles.momentum.y");
    TTreeReaderArray<Float_t> pz_track(tr,"ReconstructedSeededChargedParticles.momentum.z");
    TTreeReaderArray<Float_t> E_track(tr,"ReconstructedSeededChargedParticles.energy");
    TTreeReaderArray<Float_t> M_track(tr,"ReconstructedSeededChargedParticles.mass");
    TTreeReaderArray<Int_t> PDG_track(tr,"ReconstructedSeededChargedParticles.PDG");
    TTreeReaderArray<Int_t> type_track(tr,"ReconstructedSeededChargedParticles.type");
    TTreeReaderArray<Int_t> simID_track(tr,"_ReconstructedSeededChargedParticleAssociations_sim.index");
    TTreeReaderArray<Float_t> charge_track(tr,"ReconstructedSeededChargedParticles.charge");



    TFile* outFile = new TFile(outfileName, "RECREATE");

    // TTree* outTree = CreateOutputTree();

    TTree* outTree = new TTree("T", "Reconstructed and generated information from EICRecon");

    float e_mc_p;
    float e_mc_eta;
    float e_mc_phi;

    int   e_rec_track;
    float e_track_p;
    float e_track_eta;
    float e_track_phi;

    float e_track_px;
    float e_track_py;
    float e_track_pz;

    float p_mc_p;
    float p_mc_eta;
    float p_mc_phi;

    int   p_rec_track;
    float p_track_p;
    float p_track_eta;
    float p_track_phi;

    float ups_mc_p;
    float ups_mc_eta;
    float ups_mc_phi;
    float ups_mc_y;
    float ups_mc_m;
    
    int   ups_rec_track;
    float ups_track_p;
    float ups_track_eta;
    float ups_track_y;
    float ups_track_phi;
    float ups_track_m;

    outTree->Branch("e_mc_p",         &e_mc_p);
    outTree->Branch("e_mc_eta",       &e_mc_eta);
    outTree->Branch("e_mc_phi",       &e_mc_phi);

    outTree->Branch("e_rec_track",    &e_rec_track);
    outTree->Branch("e_track_p",      &e_track_p);  
    outTree->Branch("e_track_eta",    &e_track_eta);    
    outTree->Branch("e_track_phi",    &e_track_phi);

    outTree->Branch("p_mc_p",         &p_mc_p);
    outTree->Branch("p_mc_eta",       &p_mc_eta);
    outTree->Branch("p_mc_phi",       &p_mc_phi);

    outTree->Branch("p_rec_track",    &p_rec_track);
    outTree->Branch("p_track_p",      &p_track_p);  
    outTree->Branch("p_track_eta",    &p_track_eta);    
    outTree->Branch("p_track_phi",    &p_track_phi);

    outTree->Branch("ups_mc_p",         &ups_mc_p);
    outTree->Branch("ups_mc_eta",       &ups_mc_eta);
    outTree->Branch("ups_mc_phi",       &ups_mc_phi);
    outTree->Branch("ups_mc_y",       &ups_mc_y);
    outTree->Branch("ups_mc_m",       &ups_mc_m);

    outTree->Branch("ups_rec_track",    &ups_rec_track);
    outTree->Branch("ups_track_p",      &ups_track_p);  
    outTree->Branch("ups_track_eta",    &ups_track_eta);
    outTree->Branch("ups_track_y",    &ups_track_y);    
    outTree->Branch("ups_track_phi",    &ups_track_phi);
    outTree->Branch("ups_track_m",    &ups_track_m);

    Int_t Nevents = eicreconTree->GetEntries();
    cout << "Total number of entries: " << Nevents << endl;
    
    Int_t ev(0);

    while(tr.Next()){
        
        if(ev%1000 == 0) cout << ev/1000 << "k / " << Nevents/1000 << "k events processed" << endl;

        // output variables
        e_mc_p = 0.;
        e_mc_eta = 0.;
        e_mc_phi = 0.;
                
        e_rec_track = 0.;      
        e_track_p = 0.;
        e_track_eta = 0.;
        e_track_phi = 0.;
        e_track_px = 0.;
        e_track_py = 0.;
        e_track_pz = 0.;

        p_mc_p = 0.;
        p_mc_eta = 0.;
        p_mc_phi = 0.;
                
        p_rec_track = 0.;      
        p_track_p = 0.;
        p_track_eta = 0.;
        p_track_phi = 0.;

        ups_mc_p = 0.;
        ups_mc_eta = 0.;
        ups_mc_phi = 0.;
        ups_mc_y = 0.;
        ups_mc_m = 0.;
        
        ups_rec_track = 0.;
        ups_track_p = 0.;
        ups_track_eta = 0.;
        ups_track_y = 0.;
        ups_track_phi = 0.;
        ups_track_m = 0.;

        TVector3 electron_mc(0., 0., 0.);
        TVector3 positron_mc(0., 0., 0.);
        TVector3 upsilon_mc(0., 0., 0.);

        TLorentzVector electron_mc_4mom(0., 0., 0., 0.);
        TLorentzVector positron_mc_4mom(0., 0., 0., 0.);
        TLorentzVector upsilon_mc_4mom(0., 0., 0., 0.);

        for (Int_t imc = 0; imc < (Int_t)PDG_mc.GetSize(); imc++){
            
            if (imc == 0 || imc == 3) continue;

            if (PDG_mc[imc] == 11) {
                electron_mc.SetXYZ(px_mc[imc], py_mc[imc], pz_mc[imc]);
                electron_mc_4mom.SetPxPyPzE(px_mc[imc], py_mc[imc], pz_mc[imc], E_mc[imc]);
            }
            else if (PDG_mc[imc] == -11) {
                positron_mc.SetXYZ(px_mc[imc], py_mc[imc], pz_mc[imc]);
                positron_mc_4mom.SetPxPyPzE(px_mc[imc], py_mc[imc], pz_mc[imc], E_mc[imc]);
            }
        }

        upsilon_mc_4mom = electron_mc_4mom + positron_mc_4mom;
        upsilon_mc = electron_mc + positron_mc;

        e_mc_p = electron_mc.Mag();
        e_mc_eta = electron_mc_4mom.Eta();
        e_mc_phi = electron_mc_4mom.Phi();

        p_mc_p = positron_mc.Mag();
        p_mc_eta = positron_mc_4mom.Eta();
        p_mc_phi = positron_mc_4mom.Phi();

        ups_mc_p = upsilon_mc.Mag();
        ups_mc_eta = upsilon_mc_4mom.Eta();
        ups_mc_phi = upsilon_mc_4mom.Phi();
        ups_mc_y = upsilon_mc_4mom.Rapidity();
        ups_mc_m = upsilon_mc_4mom.M();

        TVector3 electron_track(0., 0., 0.);
        TVector3 positron_track(0., 0., 0.);
        TVector3 upsilon_track(0., 0., 0.);
    
        TLorentzVector electron_track_4mom(0., 0., 0., 0.);
        TLorentzVector positron_track_4mom(0., 0., 0., 0.);
        TLorentzVector upsilon_track_4mom(0., 0., 0., 0.);

        // if (PDG_track.GetSize() != simID_track.GetSize()) {
            
        //     cout << "ev: " << ev << endl;

        //     cout << "PDG_track.GetSize(): " << PDG_track.GetSize() << endl;

        //     cout << "simID_track.GetSize(): " << simID_track.GetSize() << endl;
        // }

        Int_t failed = 0;

        for (Int_t itrk = 0; itrk < (Int_t)PDG_track.GetSize(); itrk++){

            // cout << "type_track[" << itrk << "]: " << type_track[itrk] << endl;

            // skip when matching failed 
            if (type_track[itrk] != 0) {
                // cout << "failed: " << failed << endl;
                failed++;
                ev++;
                continue;
            }

            // if (fabs(M_track[itrk] - 0.00051) > 1e-4) continue;

            // skip when simID is not 4 or 5 
            if (simID_track[itrk - failed] != 4 && simID_track[itrk - failed] != 5) {
                // cout << "simID index: " << itrk - failed << endl;
                // cout << "simID_track: " << simID_track[itrk - failed] << endl;
                ev++;
                continue;
            }

            if (charge_track[itrk] == -1) {
                electron_track.SetXYZ(px_track[itrk], py_track[itrk], pz_track[itrk]);
                electron_track_4mom.SetPxPyPzE(px_track[itrk], py_track[itrk], pz_track[itrk], E_track[itrk]);

                e_track_px = px_track[itrk];    
                e_track_py = py_track[itrk];
                e_track_pz = pz_track[itrk];
            }
            else if (charge_track[itrk] == 1) {
                positron_track.SetXYZ(px_track[itrk], py_track[itrk], pz_track[itrk]);
                positron_track_4mom.SetPxPyPzE(px_track[itrk], py_track[itrk], pz_track[itrk], E_track[itrk]);
            }
        }

        upsilon_track_4mom = electron_track_4mom + positron_track_4mom;
        upsilon_track = electron_track + positron_track;

        e_track_p = electron_track.Mag();
        e_track_eta = electron_track_4mom.Eta();
        e_track_phi = electron_track_4mom.Phi();

        p_track_p = positron_track.Mag();
        p_track_eta = positron_track_4mom.Eta();
        p_track_phi = positron_track_4mom.Phi();

        ups_track_p = upsilon_track.Mag();
        ups_track_eta = upsilon_track_4mom.Eta();
        ups_track_y = upsilon_track_4mom.Rapidity();
        ups_track_phi = upsilon_track_4mom.Phi();
        ups_track_m = upsilon_track_4mom.M();

        // skip the events when reconstruction failed
        if (e_track_p < 0.100 || p_track_p < 0.100) {
            // cout << "ev: " << ev << endl; 
            // cout << "electron track_p: " << e_track_p << endl;
            // cout << "electron track_px: " << e_track_px << endl;
            // cout << "positron track_p: " << p_track_p << endl;
            // cout << "diff : " << electron_track.Mag() - electron_track_4mom.Mag() << endl;
            
            ev++;

            continue;
        }

        // if (e_track_p < 0) {
        //     cout << "e_track_p: " << e_track_p << endl;
        //     cout << "e_track_px: " <<e_track_px << endl;
        //     cout << "e_track_py: " <<e_track_py << endl;
        //     cout << "e_track_pz: " <<e_track_pz << endl;
        // } 

        if (ups_track_y < ymin || ups_track_y > ymax) continue;

        outTree->Fill();
        ev++;

    }

    outFile->cd();
    outTree->Write(outTree->GetName(), 2);

    outFile->Close();

}
