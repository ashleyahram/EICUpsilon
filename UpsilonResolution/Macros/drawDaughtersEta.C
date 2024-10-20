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

void drawEtaGraph(int ymin = 0, int ymax = 1) {

    const char* infileName = Form("../Files/ep_UpsilonCombined_18x275_podio_seeded_MCntuple_y%d_%d.root", ymin, ymax);

    // need a header file
    setTDRStyle();

    /// Read skimmed Tree file 
    TFile* f = TFile::Open(infileName, "READ");
    TTree* eicreconTree = (TTree *)f->Get("T");

    Float_t Eta_ele, Eta_pos;
    TBranch *Eta_ele_br = eicreconTree->GetBranch("e_track_eta");
    TBranch *Eta_pos_br = eicreconTree->GetBranch("p_track_eta");

    Eta_ele_br->SetAddress(&Eta_ele);
    Eta_pos_br->SetAddress(&Eta_pos);

    TH1D *etaEle1DHist = new TH1D("etaEle1DHist", "#eta_{e}; #eta_{e^{#minus}}; counts", 100, -4, 4);
  
    TH1D *etaPos1DHist = new TH1D("etaPos1DHist", "#eta_{e}; #eta_{e^{+}}; counts", 100, -4, 4);
   
    TH2D *eta2DHist = new TH2D("eta2DHist", "#eta_{e}; #eta_{e^{#minus}}; #eta_{e^{+}}", 50, -6, 6, 50, -6, 6);

    for (Int_t i = 0; i < eicreconTree->GetEntries(); ++i) {
        Eta_ele_br->GetEntry(i);
        Eta_pos_br->GetEntry(i);

        eta2DHist->Fill(Eta_ele, Eta_pos);
        // Do something with the data (e.g., print it)
        // printf("Entry %d: var1=%f, var2=%f\n", i, Eta_ele, Eta_pos);
        etaEle1DHist->Fill(Eta_ele);
        etaPos1DHist->Fill(Eta_pos);
    }

    TCanvas *c1 = new TCanvas("c1", "c1", 700, 600);

    c1->SetRightMargin(0.15);
    eta2DHist->GetXaxis()->CenterTitle();
    eta2DHist->GetYaxis()->CenterTitle();
    eta2DHist->GetYaxis()->SetTitleOffset(0.8);
    eta2DHist->Draw("COLZ");

    gStyle->SetPalette(kPastel);

    eta2DHist->SetStats(1);

    /// text
    TLatex text;
    text.SetTextAlign(12);
    text.SetTextSize(0.038);
    text.SetTextFont(42);
    // 0.1547278,0.9686957
    text.DrawLatexNDC(0.15, 0.96, Form("ep %dx%d GeV", eEnergy, pEnergy));
    // text.DrawLatexNDC(0.103, 0.915, Form("ep %dx%d GeV", eEnergy, pEnergy));

    // text.SetTextFont(42);
    // 0.5186246,0.9686957
    text.DrawLatexNDC(0.52, 0.96, "#varUpsilon(1S)+#varUpsilon(2S)+#varUpsilon(3S) #rightarrow ^{}e^{+}e^{#minus}");
    // 0.6891117,0.9669565
    // text.DrawLatexNDC(0.69, 0.96, "#varUpsilon(1S) #rightarrow ^{}e^{#minus}e^{+}");

    text.SetTextSize(0.045);
    text.SetTextFont(62);
    // 0.1905444,0.3634783
    // text.DrawLatexNDC(0.17, 0.23, "0^{ }<^{ }Q^{2}^{ }<^{ }10^{-2} GeV^{2}");
    text.DrawLatexNDC(0.2, 0.36, "10^{-3}^{ }<^{ }Q^{2}^{ }<^{ }10 GeV^{2}");
    // text.DrawLatexNDC(0.558, 0.155, "10^{ }<^{ }W^{ }<^{ }20 GeV");
    // 0.2005731,0.2904348
    text.DrawLatexNDC(0.2, 0.29, "no limits on W");
    // 0.2020057,0.2191304
    text.DrawLatexNDC(0.2, 0.22, Form("%d < y_{e^{+}e^{-}} < %d", ymin, ymax));

    c1->Update();

    // Access the stat box
    auto *statBox = dynamic_cast<TPaveStats*>(eta2DHist->FindObject("stats"));
    cout<< statBox << endl;
    if (statBox) {
        statBox->SetX1NDC(0.62); // New X1 coordinate in NDC
        statBox->SetX2NDC(0.82); // New X2 coordinate in NDC
        statBox->SetY1NDC(0.65); // New Y1 coordinate in NDC
        statBox->SetY2NDC(0.89); // New Y2 coordinate in NDC
        statBox->Draw();
    }
    else cout << "not found stat box!!" << endl;

    TCanvas *c2 = new TCanvas("c2", "c2", 700, 600);

    etaEle1DHist->GetXaxis()->CenterTitle();
    etaEle1DHist->GetYaxis()->CenterTitle();
    // etaHist->GetYaxis()->SetTitleOffset(0.8);
    etaEle1DHist->Draw("");

    etaEle1DHist->SetStats(1);

    text.SetTextAlign(12);
    text.SetTextSize(0.038);
    text.SetTextFont(42);

    text.DrawLatexNDC(0.15, 0.96, Form("ep %dx%d GeV", eEnergy, pEnergy));
    text.DrawLatexNDC(0.64, 0.96, "#varUpsilon(1S)+#varUpsilon(2S)+#varUpsilon(3S) #rightarrow ^{}e^{+}e^{#minus}");

    text.SetTextSize(0.045);
    text.SetTextFont(62);

    text.DrawLatexNDC(0.2, 0.87, "10^{-3}^{ }<^{ }Q^{2}^{ }<^{ }10 GeV^{2}");
    text.DrawLatexNDC(0.2, 0.80, "no limits on W");
    text.DrawLatexNDC(0.2, 0.73, Form("%d < y_{e^{+}e^{-}} < %d", ymin, ymax));

    c2->Update();

    // Access the stat box
    auto *statBox2 = dynamic_cast<TPaveStats*>(etaEle1DHist->FindObject("stats"));
    // cout<< statBox << endl;
    if (statBox2) {
        statBox2->SetX1NDC(0.62); // New X1 coordinate in NDC
        statBox2->SetX2NDC(0.82); // New X2 coordinate in NDC
        statBox2->SetY1NDC(0.65); // New Y1 coordinate in NDC
        statBox2->SetY2NDC(0.89); // New Y2 coordinate in NDC
        statBox2->Draw();
    }
    else cout << "not found stat box!!" << endl;

    TCanvas *c3 = new TCanvas("c3", "c3", 700, 600);

    etaPos1DHist->GetXaxis()->CenterTitle();
    etaPos1DHist->GetYaxis()->CenterTitle();
    // etaHist->GetYaxis()->SetTitleOffset(0.8);
    etaPos1DHist->Draw("");

    etaPos1DHist->SetStats(1);

    text.SetTextAlign(12);
    text.SetTextSize(0.038);
    text.SetTextFont(42);

    text.DrawLatexNDC(0.15, 0.96, Form("ep %dx%d GeV", eEnergy, pEnergy));
    text.DrawLatexNDC(0.64, 0.96, "#varUpsilon(1S)+#varUpsilon(2S)+#varUpsilon(3S) #rightarrow ^{}e^{+}e^{#minus}");

    text.SetTextSize(0.045);
    text.SetTextFont(62);

    text.DrawLatexNDC(0.2, 0.87, "10^{-3}^{ }<^{ }Q^{2}^{ }<^{ }10 GeV^{2}");
    text.DrawLatexNDC(0.2, 0.80, "no limits on W");
    text.DrawLatexNDC(0.2, 0.73, Form("%d < y_{e^{+}e^{-}} < %d", ymin, ymax));

    c3->Update();

    // Access the stat box
    auto *statBox3 = dynamic_cast<TPaveStats*>(etaPos1DHist->FindObject("stats"));
    // cout<< statBox << endl;
    if (statBox3) {
        statBox3->SetX1NDC(0.62); // New X1 coordinate in NDC
        statBox3->SetX2NDC(0.82); // New X2 coordinate in NDC
        statBox3->SetY1NDC(0.65); // New Y1 coordinate in NDC
        statBox3->SetY2NDC(0.89); // New Y2 coordinate in NDC
        statBox3->Draw();
    }
    else cout << "not found stat box!!" << endl;

    c1->SaveAs(Form("../Graphs/SeededElectronPseudorapidity_ep_%dx%d_y_%d_%d.png", eEnergy, pEnergy, ymin, ymax));
}

void drawRapidityGraph() {

    const char* infileName = Form("../Files/ep_UpsilonCombined_18x275_podio_seeded_MCntuple_y-3_4.root");

    // need a header file
    setTDRStyle();

    /// Read skimmed Tree file 
    TFile* f = TFile::Open(infileName, "READ");
    TTree* eicreconTree = (TTree *)f->Get("T");

    Float_t y_ups;
    TBranch *y_ups_br = eicreconTree->GetBranch("ups_track_y");

    y_ups_br->SetAddress(&y_ups);

    float ymin = -3, ymax = 4;

    int nyBins = 100;

    TH1D *yUpsHist = new TH1D("yUpsHist", Form("; y_{e^{+}e^{#minus}}; Events / %.2f", (float)(ymax - ymin) / (float)nyBins), nyBins, ymin, ymax);
  
    for (Int_t i = 0; i < eicreconTree->GetEntries(); ++i) {
        y_ups_br->GetEntry(i);

        yUpsHist->Fill(y_ups);
        // Do something with the data (e.g., print it)
        // printf("Entry %d: var1=%f, var2=%f\n", i, Eta_ele, Eta_pos);

        cout << "y_ups: " << y_ups << endl;
    }

    TCanvas *c1 = new TCanvas("c1", "c1", 700, 500);

    c1->SetRightMargin(0.02);
    yUpsHist->GetXaxis()->CenterTitle();
    yUpsHist->GetYaxis()->CenterTitle();
    yUpsHist->GetYaxis()->SetTitleOffset(1.1);

    yUpsHist->SetStats(1);

    yUpsHist->Draw();

    // lines
    TLine* verline1 = new TLine(-1, 0, -1, 4380);

    verline1->SetLineStyle(kDashed);
    verline1->SetLineColor(kGray);
    verline1->SetLineWidth(2);
    verline1->Draw("SAME");

    TLine* verline2 = new TLine(0, 0, 0, 4380);
    
    verline2->SetLineStyle(kDashed);
    verline2->SetLineColor(kGray);
    verline2->SetLineWidth(2);
    verline2->Draw("SAME");

    TLine* verline3 = new TLine(1, 0, 1, 4380);

    verline3->SetLineStyle(kDashed);
    verline3->SetLineColor(kGray);
    verline3->SetLineWidth(2);
    verline3->Draw("SAME");

    TLine* verline4 = new TLine(2, 0, 2, 4380);

    verline4->SetLineStyle(kDashed);
    verline4->SetLineColor(kGray);
    verline4->SetLineWidth(2);
    verline4->Draw("SAME");

    /// text
    TLatex text;
    text.SetTextAlign(12);
    text.SetTextSize(0.045);
    text.SetTextFont(42);
    // 0.1547278,0.9686957
    text.DrawLatexNDC(0.15, 0.96, Form("ep %dx%d GeV", eEnergy, pEnergy));
    // text.DrawLatexNDC(0.103, 0.915, Form("ep %dx%d GeV", eEnergy, pEnergy));

    // text.SetTextFont(42);
    // 0.7120344,0.9621053
    text.DrawLatexNDC(0.66, 0.96, "#varUpsilon(1S)+#varUpsilon(2S)+#varUpsilon(3S) #rightarrow ^{}e^{+}e^{#minus}");
    // 0.6891117,0.9669565
    // text.DrawLatexNDC(0.69, 0.96, "#varUpsilon(1S) #rightarrow ^{}e^{#minus}e^{+}");

    text.SetTextSize(0.05);
    text.SetTextFont(62);
    // 0.7249284,0.8610526
    text.DrawLatexNDC(0.69, 0.84, "10^{-3}^{ }<^{ }Q^{2}^{ }<^{ }10 GeV^{2}");
    // text.DrawLatexNDC(0.17, 0.23, "10^{-3}^{ }<^{ }Q^{2}^{ }<^{ }10 GeV^{2}");
    // text.DrawLatexNDC(0.558, 0.155, "10^{ }<^{ }W^{ }<^{ }20 GeV");
    // 0.7277937,0.7936842
    text.DrawLatexNDC(0.69, 0.77, "no limits on W");

    c1->Update();

    // Access the stat box
    auto *statBox = dynamic_cast<TPaveStats*>(yUpsHist->FindObject("stats"));
    cout<< statBox << endl;
    if (statBox) {
        statBox->SetX1NDC(0.62); // New X1 coordinate in NDC
        statBox->SetX2NDC(0.82); // New X2 coordinate in NDC
        statBox->SetY1NDC(0.65); // New Y1 coordinate in NDC
        statBox->SetY2NDC(0.89); // New Y2 coordinate in NDC
        statBox->Draw();
    }
    else cout << "not found stat box!!" << endl;

    c1->SaveAs(Form("../Graphs/SeededUpsilonRapidity_ep_%dx%d.png", eEnergy, pEnergy));
}
