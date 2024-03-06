#include <iostream>
#include <cmath>

#include "../Tools/GetPullDistribution.h"
#include "../Tools/Legends.h"

#include "RooRealVar.h"
#include "RooDataSet.h"

using namespace RooFit;

void fitUpsilonMass_DSCB(const char* infileName = "../Files/Q1_UpsilonMass.root"){
		
	const float PDGmass_1S = 9.460; // in GeV
	const float PDGmass_2S = 10.023;
	const float PDGmass_3S = 10.355;
	
	Float_t binMin = 8., binMax = 11.;
	Int_t nBins = 60;

    RooRealVar w("w", "w", 0, 10.);

	/// Read EICrecon output file 
	const char* infileName1S = "../Files/Q1_Upsilon1SMass.root";
    TFile *file1S = new TFile(infileName1S, "Read");
    TNtuple *Ups1SMNtuple = (TNtuple*)gDirectory -> Get("UpsMNtuple");
    RooRealVar m1S("m", "m1S",  binMin, binMax);
   	RooDataSet *data1S = new RooDataSet("data1S", "dataset with 1S m", Ups1SMNtuple, m1S);

   	w = 1.;
   	RooRealVar *w1Svar = (RooRealVar *)data1S -> addColumn(w);
   	RooDataSet wdata1S(data1S->GetName(), data1S->GetTitle(), data1S, *data1S->get(), 0, "w");

   	Long64_t nEntries1S = data1S -> sumEntries();
   	cout << "nEntries Y(1S): " << nEntries1S << endl;

   	// Y(1S) signal shape
	RooRealVar mean_1S("mean_1S", "mean 1S", PDGmass_1S, 9.3, 9.6);
	RooRealVar sigma_1S("sigma_1S", "", 0.08, .01, .15);

   	// get the tail parameters of the signal shape first in case the MC fit is needed
	RooRealVar* alphaInf1S = new RooRealVar("alphaInf1S", "alphaInf1S", 1.5, 0, 10);
	RooRealVar* orderInf1S = new RooRealVar("orderInf1S", "orderInf1S", 1.5, 0, 10);
	RooRealVar* alphaSup1S = new RooRealVar("alphaSup1S", "alphaSup1S", 1.5, 0, 10);
	RooRealVar* orderSup1S = new RooRealVar("orderSup1S", "orderSup1S", 3, 0, 10);

	RooCrystalBall signal_1S("signal_1S", "", m1S, mean_1S, sigma_1S, *alphaInf1S, *orderInf1S, *alphaSup1S, *orderSup1S);
	RooRealVar nSignal_1S("nSignal_1S", "N 1S", nEntries1S / 5, 0, nEntries1S+10000);
	
	RooAddPdf model1S("model1S", "model1S", RooArgList(signal_1S), RooArgList(nSignal_1S));
	RooFitResult* fitResult1S = model1S.fitTo(wdata1S, Save(), Extended(kTRUE), Minos(kTRUE), NumCPU(3), Range(binMin, binMax));

	/// Read Y(2S) EICrecon output file 
	const char* infileName2S = "../Files/Q1_Upsilon2SMass.root";
    TFile *file2S = new TFile(infileName2S, "Read");
    TNtuple *Ups2SMNtuple = (TNtuple*)gDirectory -> Get("UpsMNtuple");
    RooRealVar m2S("m", "m2S",  binMin, binMax);  
   	RooDataSet *data2S = new RooDataSet("data2S", "dataset with 2S m", Ups2SMNtuple, m2S);
   
   	Long64_t nEntries2S = data2S -> sumEntries();
    w = 0.15*nEntries1S/nEntries2S;
   	RooRealVar *w2Svar = (RooRealVar *)data2S -> addColumn(w);
   	RooDataSet wdata2S(data2S->GetName(), data2S->GetTitle(), data2S, *data2S->get(), 0, "w");

	// Y(2S) signal shape, mass scaling for mean and widths
	RooConstVar massScaling_2S("massScaling_2S", "", PDGmass_2S / PDGmass_1S);

	RooFormulaVar mean_2S("mean_2S", "massScaling_2S*mean_1S", RooArgSet(massScaling_2S, mean_1S));
	RooRealVar sigma_2S("sigma_2S", "", 0.08, .01, .15);

   	// get the tail parameters of the signal shape first in case the MC fit is needed
	RooRealVar* alphaInf2S = new RooRealVar("alphaInf2S", "alphaInf2S", 1.5, 0, 10);
	RooRealVar* orderInf2S = new RooRealVar("orderInf2S", "orderInf2S", 1.5, 0, 10);
	RooRealVar* alphaSup2S = new RooRealVar("alphaSup2S", "alphaSup2S", 1.5, 0, 10);
	RooRealVar* orderSup2S = new RooRealVar("orderSup2S", "orderSup2S", 3, 0, 10);

	RooCrystalBall signal_2S("signal_2S", "", m2S, mean_2S, sigma_2S, *alphaInf2S, *orderInf2S, *alphaSup2S, *orderSup2S);
	RooRealVar nSignal_2S("nSignal_2S", "N 2S", nEntries1S / 10, 0, nEntries1S / 2);

	RooAddPdf model2S("model2S", "model2S", RooArgList(signal_2S), RooArgList(nSignal_2S));
	RooFitResult* fitResult2S = model2S.fitTo(wdata2S, Save(), Extended(kTRUE), Minos(kTRUE), NumCPU(3), Range(binMin, binMax));

	/// Read Y(3S) EICrecon output file 
	const char* infileName3S = "../Files/Q1_Upsilon3SMass.root";
    TFile *file3S = new TFile(infileName3S, "Read");
    TNtuple *Ups3SMNtuple = (TNtuple*)gDirectory -> Get("UpsMNtuple");
    RooRealVar m3S("m", "m3S",  binMin, binMax);
   	RooDataSet *data3S = new RooDataSet("data3S", "dataset with 3S m", Ups3SMNtuple, m3S);

   	Long64_t nEntries3S = data3S -> sumEntries();
   	w = 0.08*nEntries1S/nEntries3S;   
   	RooRealVar *w3Svar = (RooRealVar *)data3S->addColumn(w);

   	RooDataSet wdata3S(data3S->GetName(), data3S->GetTitle(), data3S, *data3S->get(), 0, "w");

	// Y(3S) signal shape, mass scaling for mean and widths
	RooConstVar massScaling_3S("massScaling_3S", "", PDGmass_3S / PDGmass_1S);

	RooFormulaVar mean_3S("mean_3S", "massScaling_3S*mean_1S", RooArgSet(massScaling_3S, mean_1S));
	RooRealVar sigma_3S("sigma_3S", "", 0.08, .01, .15);

   	// get the tail parameters of the signal shape first in case the MC fit is needed
	RooRealVar* alphaInf3S = new RooRealVar("alphaInf3S", "alphaInf3S", 1.5, 0, 10);
	RooRealVar* orderInf3S = new RooRealVar("orderInf3S", "orderInf3S", 1.5, 0, 10);
	RooRealVar* alphaSup3S = new RooRealVar("alphaSup3S", "alphaSup3S", 1.5, 0, 10);
	RooRealVar* orderSup3S = new RooRealVar("orderSup3S", "orderSup3S", 3, 0, 10);

	RooCrystalBall signal_3S("signal_3S", "", m3S, mean_3S, sigma_3S, *alphaInf3S, *orderInf3S, *alphaSup3S, *orderSup3S);
	RooRealVar nSignal_3S("nSignal_3S", "N 3S", nEntries1S / 20, 0, nEntries1S / 2);

	RooAddPdf model3S("model3S", "model3S", RooArgList(signal_3S), RooArgList(nSignal_3S));

	RooFitResult* fitResult3S = model3S.fitTo(wdata3S, Save(), Extended(kTRUE), Minos(kTRUE), NumCPU(3), Range(binMin, binMax));
	
	RooPlot *xframe1S = m1S.frame(Title(" "), Range(binMin, binMax));
	RooPlot *xframe2S = m2S.frame(Title(" "), Range(binMin, binMax));
	RooPlot *xframe3S = m3S.frame(Title(" "), Range(binMin, binMax));
	wdata1S.plotOn(xframe1S, Name("wdata1S"), XErrorSize(0), MarkerSize(1.),  Binning(nBins), DrawOption("P0Z"));
	wdata2S.plotOn(xframe2S, Name("wdata2S"), XErrorSize(0), MarkerSize(1.),  Binning(nBins), DrawOption("P0Z"));
	wdata3S.plotOn(xframe3S, Name("wdata3S"), XErrorSize(0), MarkerSize(1.),  Binning(nBins), DrawOption("P0Z"));
	model1S.plotOn(xframe1S, Name("Fit1S"), LineWidth(5), LineColor(kOrange+10), Range(binMin, binMax));
	model2S.plotOn(xframe2S, Name("Fit2S"), LineWidth(5), LineColor(kOrange+10), Range(binMin, binMax));
	model3S.plotOn(xframe3S, Name("Fit3S"), LineWidth(5), LineColor(kOrange+10), Range(binMin, binMax));

	TCanvas *cFit = new TCanvas("cFit", "cFit", 600, 600);
	cFit -> cd();
	TPad *pad1 = new TPad("pad1", "pad1", 0, 0.16, 0.98, 1.0);
	pad1 -> SetTicks(1, 1);
	pad1 -> cd();

	gPad -> SetLeftMargin(0.17);
	gPad -> SetRightMargin(0.05);
	gPad -> SetBottomMargin(0.17);

	xframe1S -> SetTitle("Invariant Mass of #varUpsilon (1S)");
	xframe1S -> SetTitleOffset(0.8);
	xframe1S -> GetYaxis() -> CenterTitle();
	xframe1S -> GetYaxis() -> SetTitleSize(0.058);
	xframe1S -> GetYaxis() -> SetTitleOffset(1.5);
	xframe1S -> GetXaxis() -> CenterTitle();
	xframe1S -> GetXaxis() -> SetTitle("m_{e^{+}e^{-}} (GeV/c^{2})");
	xframe1S -> GetXaxis() -> SetTitleSize(0.058);
	xframe1S -> GetXaxis() -> SetTitleOffset(1.);

	xframe1S -> addObject(DSCBParamsText(mean_1S, sigma_1S, *alphaInf1S, *orderInf1S, *alphaSup1S, *orderSup1S));
	xframe1S -> Draw();

	xframe2S -> addObject(DSCBParamsText2(mean_2S, sigma_2S, *alphaInf2S, *orderInf2S, *alphaSup2S, *orderSup2S));
	xframe2S -> Draw("Same");

	xframe3S -> addObject(DSCBParamsText2(mean_3S, sigma_3S, *alphaInf3S, *orderInf3S, *alphaSup3S, *orderSup3S));
	xframe3S -> Draw("Same");

	cFit->cd();
	cout << fitResult1S -> floatParsFinal() << endl;
	cout << fitResult2S -> floatParsFinal() << endl;
	cout << fitResult3S -> floatParsFinal() << endl;

	pad1->Draw();

	gSystem->mkdir("./Graphs", kTRUE);
	cFit -> SaveAs("./Graphs/UpsilonFit_DSCB.png");
}
