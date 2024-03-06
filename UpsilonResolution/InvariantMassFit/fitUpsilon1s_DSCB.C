#include <iostream>
#include <cmath>

#include "../Tools/GetPullDistribution.h"
#include "../Tools/Legends.h"

#include "RooRealVar.h"
#include "RooDataSet.h"

using namespace RooFit;

void fitUpsilon1s_DSCB(const char* infileName = "../Files/Q1_Upsilon1SMass.root"){
	
	const float PDGmass_1S = 9.460; // in GeV
	const float PDGmass_2S = 10.023;
	const float PDGmass_3S = 10.355;
	
	Float_t binMin = 8, binMax = 11;
	Int_t nBins = 60;

	/// Read EICrecon output file 
   TFile *file = new TFile(infileName, "Read");
   TNtuple *UpsMNtuple = (TNtuple*)gDirectory -> Get("UpsMNtuple");

   RooRealVar m("m", "m",  binMin, binMax);
   RooDataSet data("data", "dataset with m", UpsMNtuple, m);

   Long64_t nEntries = data.sumEntries();
   cout << "nEntries: " << nEntries << endl;

   // Y(1S) signal shape
	RooRealVar mean_1S("mean_1S", "mean 1S", PDGmass_1S, 9.3, 9.6);
	RooRealVar sigma_1S("sigma_1S", "", .01, .15);

   // get the tail parameters of the signal shape first in case the MC fit is needed
	RooRealVar* alphaInf = new RooRealVar("alphaInf", "", 0, 10);
	RooRealVar* orderInf = new RooRealVar("orderInf", "", 0, 10);
	RooRealVar* alphaSup = new RooRealVar("alphaSup", "", 0, 10);
	RooRealVar* orderSup = new RooRealVar("orderSup", "", 0, 10);

	RooCrystalBall signal_1S("signal_1S", "", m, mean_1S, sigma_1S, *alphaInf, *orderInf, *alphaSup, *orderSup);
	RooRealVar nSignal_1S("nSignal_1S", "N 1S", nEntries / 5, 0, nEntries+10000);

	RooRealVar nCB1S("nCB1S", "number of signal CB 1S", 1.5e5, 1e5, 2.5e5); //450
	
	RooAddPdf model("model", "model", RooArgList(signal_1S), RooArgList(nSignal_1S));
	
	RooFitResult* fitResult = model.fitTo(data, Save(), Extended(kTRUE), Minos(kTRUE), NumCPU(3), Range(binMin, binMax));
	fitResult -> Print("v");

	RooPlot *xframe = m.frame(Title(" "), Range(binMin, binMax));
	data.plotOn(xframe, Name("data"), XErrorSize(0), MarkerSize(1.),  Binning(nBins), DrawOption("P0Z"));
	model.plotOn(xframe, Name("Fit"), LineWidth(5), LineColor(kOrange+10), Range(binMin, binMax));

	TCanvas *cFit = new TCanvas("cFit", "cFit", 600, 600);
	cFit -> cd();
	TPad *pad1 = new TPad("pad1", "pad1", 0, 0.16, 0.98, 1.0);
	pad1 -> SetTicks(1, 1);
	pad1 -> cd();

	gPad -> SetLeftMargin(0.17);
	gPad -> SetBottomMargin(0.17);

	xframe -> SetTitle("Invariant Mass of #varUpsilon (1S)");
	xframe -> SetTitleOffset(0.8);
	xframe -> GetYaxis() -> CenterTitle();
	xframe -> GetYaxis() -> SetTitleSize(0.058);
	xframe -> GetYaxis() -> SetTitleOffset(1.5);
	xframe -> GetXaxis() -> CenterTitle();
	xframe -> GetXaxis() -> SetTitle("m_{e^{+}e^{-}} (GeV/c^{2})");
	xframe -> GetXaxis() -> SetTitleSize(0.058);
	xframe -> GetXaxis() -> SetTitleOffset(1.);
	xframe -> GetXaxis() -> SetLabelSize(0);
	xframe -> addObject(DSCBParamsText(mean_1S, sigma_1S, *alphaInf, *orderInf, *alphaSup, *orderSup));

	xframe -> Draw();

	cFit->cd();
	cout << fitResult -> floatParsFinal() << endl;

	RooHist *hpull = xframe -> pullHist("data","Fit");

   double chisq = 0;
   int nFullBinsPull = 0; 
   double *ypull = hpull -> GetY();
   
   for(int i=0; i<nBins; i++){
   	cout << ypull[i] << endl;
      if(ypull[i] == 0) continue;
      chisq += TMath::Power(ypull[i], 2);
      nFullBinsPull++;
   }

   double reducedchisq = chisq/(nFullBinsPull-fitResult->floatParsFinal().getSize());

	TPad* pad2 = GetPadPullDistribution(xframe, data, model, reducedchisq, fitResult->floatParsFinal().getSize());
	cout << "fit parmeter nubmer: " << fitResult->floatParsFinal().getSize() << endl;

	cout << "chiSqure " << xframe -> chiSquare(fitResult->floatParsFinal().getSize()) <<endl; 

	cFit->cd();
	pad1->Draw();
	pad2->Draw();

	gSystem->mkdir("./Graphs", kTRUE);
	cFit->SaveAs("./Graphs/Upsilon1SFit_DSCB.png");
}