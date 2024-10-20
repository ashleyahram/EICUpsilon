#include <iostream>
#include <cmath>

#include "./Style/GetPullDistribution_ele.h"
#include "./Style/Legends_ele.h"

#include "RooRealVar.h"
#include "RooDataSet.h"

#include "./Style/tdrStyle.C"
#include "./Style/CMS_lumi.C"

#include "TLegend.h"

using namespace RooFit;

int eEnergy = 18; // GeV
int pEnergy = 275; // GeV

TCanvas* fitUpsilon_DSCB(int ymin = 0, int ymax = 1, int markerColor = kBlack, int lineColor = kAzure-1){
    
    const char* infileName = Form("./Files/ep_UpsilonCombined_18x275_podio_seeded_MCntuple_y%d_%d.root", ymin, ymax);

    // need a header file
    setTDRStyle();
    
    const float PDGmass_1S = 9.460; // in GeV
    const float PDGmass_2S = 10.023;
    const float PDGmass_3S = 10.355;
    
    Float_t massMin = 8., massMax = 11.;
    Int_t nBins = 60;

    /// Read skimmed Tree file 
    TFile* f = TFile::Open(infileName, "READ");
    TTree* eicreconTree = (TTree *)f->Get("T");

    RooRealVar m("ups_track_m", "ups_track_m",  massMin, massMax);
    RooDataSet *data = new RooDataSet("data", "dataset with m", eicreconTree, m);

    Long64_t nEntries = data->sumEntries();
    cout << "nEntries: " << nEntries << endl;

    // get the tail parameters of the signal shape first in case the MC fit is needed
    /*3*/RooRealVar* alphaInf = new RooRealVar("alphaInf", "", 0.1, 10);
    /*4*/RooRealVar* orderInf = new RooRealVar("orderInf", "", 0.1, 10);
    /*5*/RooRealVar* alphaSup = new RooRealVar("alphaSup", "", 0.1, 10);
    /*6*/RooRealVar* orderSup = new RooRealVar("orderSup", "", 0.1, 20);

    // Y(1S) signal shape
    /*1*/RooRealVar mean_1S("mean_1S", "mean 1S", PDGmass_1S, 9.3, 9.6);
    /*2*/RooRealVar sigma_1S("sigma_1S", "", .04, .01, .25);

    RooCrystalBall signal_1S("signal_1S", "", m, mean_1S, sigma_1S, *alphaInf, *orderInf, *alphaSup, *orderSup);
    RooRealVar nSignal_1S("nSignal_1S", "N 1S", nEntries, 0, nEntries);

    // Y(2S) signal shape
    RooConstVar massScaling_2S("massScaling_2S", "", PDGmass_2S / PDGmass_1S);
    
    /*1*/RooFormulaVar mean_2S("mean_2S", "massScaling_2S*mean_1S", RooArgSet(massScaling_2S, mean_1S));
    /*2*/RooFormulaVar sigma_2S("sigma_2S","massScaling_2S*sigma_1S", RooArgSet(massScaling_2S, sigma_1S));

    RooCrystalBall signal_2S("signal_2S", "", m, mean_2S, sigma_2S, *alphaInf, *orderInf, *alphaSup, *orderSup);
    RooRealVar nSignal_2S("nSignal_2S", "N 2S", nEntries / 5, 0, nEntries);

    // Y(3S) signal shape

    RooConstVar massScaling_3S("massScaling_3S", "", PDGmass_3S / PDGmass_1S);

    /*1*/RooFormulaVar mean_3S("mean_3S", "massScaling_3S*mean_1S", RooArgSet(massScaling_3S, mean_1S));
    /*2*/RooFormulaVar sigma_3S("sigma_3S", "massScaling_3S*sigma_1S", RooArgSet(massScaling_3S, sigma_1S));

    RooCrystalBall signal_3S("signal_3S", "", m, mean_3S, sigma_3S, *alphaInf, *orderInf, *alphaSup, *orderSup);
    RooRealVar nSignal_3S("nSignal_3S", "N 3S", nEntries / 5, 0, nEntries);

    // /*7*/RooRealVar nCB1S("nCB1S", "number of signal CB 1S", 1.5e5, 1e5, 2.5e5); //450
    ///**/RooRealVar nCB1S("nCB1S", "number of signal CB 1S", 4819); //450
    RooAddPdf model("model", "model", RooArgList(signal_1S, signal_2S, signal_3S), RooArgList(nSignal_1S, nSignal_2S, nSignal_3S));
    
    RooFitResult* fitResult = model.fitTo(*data, Save(), Extended(kTRUE), Minos(kTRUE), NumCPU(3), Range(massMin, massMax));
    fitResult->Print("v");

    RooPlot *xframe = m.frame(Title(" "), Range(massMin, massMax));
    // wdata.plotOn(xframe, Name("wdata"), XErrorSize(0), MarkerSize(1.),  MarkerColor(markerColor), Binning(nBins), DrawOption("P0Z"));
    data->plotOn(xframe, Name("data"), XErrorSize(0), MarkerSize(1.),  MarkerColor(markerColor), Binning(nBins), DrawOption("P0Z"), Range(massMin, massMax));
    model.plotOn(xframe, Name("Fit") /*, LineStyle(7)*/, LineWidth(5), LineColor(lineColor), Range(massMin, massMax));
    // model.plotOn(xframe, Components(nSignal_1S), Name("CB1S"), LineStyle(7), LineColor(kOrange+10), LineWidth(10));
    model.plotOn(xframe, Components(signal_1S), Name("CB1S"), LineStyle(7), LineColor(kOrange+10), LineWidth(3), Range(massMin, massMax));
    model.plotOn(xframe, Components(signal_2S), Name("CB2S"), LineStyle(7), LineColor(kOrange+10), LineWidth(3), Range(massMin, massMax));
    model.plotOn(xframe, Components(signal_3S), Name("CB3S"), LineStyle(7), LineColor(kOrange+10), LineWidth(3), Range(massMin, massMax));

    cout << fitResult->floatParsFinal() << endl;

    TCanvas *cFit = new TCanvas("cFit", "cFit", 600, 600);
    cFit->cd();
    
    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.16, 0.98, 1.0);
    pad1->SetTicks(1, 1);
    pad1->cd();

    gPad->SetLeftMargin(0.17);
    gPad->SetBottomMargin(0.17);

    xframe->SetTitle("Invariant Mass of #varUpsilon");
    xframe->SetTitleOffset(0.8);
    // xframe->SetTitleOffset(0.8, "y");
    xframe->GetYaxis()->CenterTitle();
    // xframe->GetYaxis()->SetTitle("Events / (0.1 GeV/c^{  2})");
    xframe->GetYaxis()->SetTitleSize(0.058);
    xframe->GetYaxis()->SetTitleOffset(1.5);
    xframe->GetXaxis()->CenterTitle();
    xframe->GetXaxis()->SetTitle("m_{e^{+}e^{#minus}} inv. mass (GeV/c^{2})");
    xframe->GetXaxis()->SetTitleSize(0.058);
    xframe->GetXaxis()->SetTitleOffset(1.);
    // xframe->GetXaxis()->SetLabelSize(0);
    // xframe->addObject(FitResultText(nSignal_1S, significance, nSignal_2S, significance2S));
    xframe->addObject(DSCBParamsText(mean_1S, sigma_1S, *alphaInf, *orderInf, *alphaSup, *orderSup));
    xframe->addObject(KinematicsText(ymin, ymax));

    // TLegend *legend = new TLegend(0.20, 0.68, 0.50, 0.88); // Adjust the position and size as needed
    // legend->SetFillColor(kWhite);
    // legend->SetLineColor(kWhite);
    // legend->SetTextSize(0.036); // Set the text size
    // // legend->SetEntrySeparation(0.0); // Set the spacing between entries
    // // legend->AddEntry("data", "#mu^{+}#mu^{#minus} reco.", "p");
    // legend->AddEntry(xframe->findObject("data"), "e^{+}e^{#minus} reco.", "p");
    // legend->AddEntry(xframe->findObject("Fit"), "Fit", "l");
    // legend->AddEntry(xframe->findObject("CB1S"), "#varUpsilon(1S), #varUpsilon(2S), #varUpsilon(3S)", "l");

    xframe->Draw();

    // legend->Draw();

    TLatex energyText;
    energyText.SetTextAlign(12);
    energyText.SetTextSize(0.044);
    energyText.SetTextFont(42);
    energyText.DrawLatexNDC(0.17, 0.96, Form("ep %dx%d GeV", eEnergy, pEnergy));

    TLatex particleText;
    particleText.SetTextAlign(12);
    particleText.SetTextSize(0.044);
    // particleText.SetTextFont(42);
    // particleText.DrawLatexNDC(0.60, 0.96, "#varUpsilon(1S)+#varUpsilon(2S)+#varUpsilon(3S) #rightarrow #mu^{+}#mu^{#minus}");
    particleText.DrawLatexNDC(0.90, 0.96, "EPIC");

    cFit->Update();

    cFit->cd();

    // RooPlot* pullFrame = (xframe->getPlotVar())->frame(xframe->GetXaxis()->GetXmin(), xframe->GetXaxis()->GetXmax());

    // cout << (xframe->getPlotVar())->frame(xframe->GetXaxis()->GetXmin(), xframe->GetXaxis()->GetXmax()) << endl;
    // cout << xframe->GetXaxis()->GetXmin() << endl;
    // cout << xframe->GetXaxis()->GetXmax() << endl;
    // xframe->pullHist("data","Fit");
    // TCanvas *cfull = new TCanvas("cfull", "cfull", 600, 600);
    // xframe->Draw();
    // xframe->Print();

    RooHist *hpull = xframe->pullHist("data","Fit");

    double chisq = 0;
    int nFullBinsPull = 0; 
    double *ypull = hpull->GetY();
   
    for(int i=0; i<nBins; i++){
     cout << ypull[i] << endl;
       if(ypull[i] == 0) continue;
       chisq += TMath::Power(ypull[i], 2);
       nFullBinsPull++;
    }

    double reducedchisq = chisq/(nFullBinsPull-fitResult->floatParsFinal().getSize());

    cout << "chisq: " << chisq << endl;
    cout << "nFullBinsPull: " << nFullBinsPull << endl;
    cout << "chi2: " << reducedchisq << endl;
    // cout << "a: " << model.Print("a") << endl;


    TPad* pad2 = GetPadPullDistribution(xframe, *data, model, reducedchisq, fitResult->floatParsFinal().getSize());
    cout << "fit parmeter nubmer: " << fitResult->floatParsFinal().getSize() << endl;

    cout << "chiSqure " << xframe->chiSquare(fitResult->floatParsFinal().getSize()) <<endl; 

    cFit->cd();
    pad1->Draw();
    pad2->Draw();

    cFit->SaveAs(Form("./Graphs/SeededUpsilonFit_ep_18x275_DSCB_allParas_y%d_%d.png", ymin, ymax));

    return cFit;
}

void drawFinalPlot(){
    
    TCanvas* canvas1 = fitUpsilon_DSCB(-3, 4);
    
    TCanvas *c1 = new TCanvas("c1", "c1", 1500, 1000);
    c1->Divide(3, 2);

    // // Adjust margins for each pad
    // for (int i = 1; i <= 6; ++i) {
    //     if (i == 1 || i == 4) continue;
    //     c1->cd(i);
    //     gPad->SetLeftMargin(0.1);  // Adjust left margin
    //     gPad->SetRightMargin(0); // Adjust right margin
    //     gPad->SetTopMargin(0.1);   // Adjust top margin
    //     gPad->SetBottomMargin(0.1);// Adjust bottom margin
    // }

    c1->cd(1);

    canvas1->DrawClonePad();
    
    gPad->Modified();
    gPad->Update();
    
    TCanvas* canvas2 = fitUpsilon_DSCB(-3, -1);

    c1->cd(2);

    canvas2->DrawClonePad();

    gPad->SetLeftMargin(0);  // Adjust left margin
    gPad->Modified();
    gPad->Update();

    TCanvas* canvas3 = fitUpsilon_DSCB(-1, 0);
    
    c1->cd(3);

    canvas3->DrawClonePad();

    gPad->Modified();
    gPad->Update();

    TCanvas* canvas4 = fitUpsilon_DSCB(0, 1);

    c1->cd(4);

    canvas4->DrawClonePad();

    gPad->Modified();
    gPad->Update();

    TCanvas* canvas5 = fitUpsilon_DSCB(1, 2);

    c1->cd(5);

    canvas5->DrawClonePad();

    gPad->Modified();
    gPad->Update();

    TCanvas* canvas6 = fitUpsilon_DSCB(2, 4);

    c1->cd(6);

    canvas6->DrawClonePad();

    gPad->Modified();
    gPad->Update();

    c1->SaveAs("./Graphs/SeededUpsilonFit_ep_18x275_DSCB.png");
}
