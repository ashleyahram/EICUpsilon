#include "TAxis.h"
#include "TFile.h"
#include "TLatex.h"
#include "TPad.h"
#include "TTreeReaderArray.h"
#include "TTreeReader.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TPaveStats.h"
#include "TObject.h"


#include "RooAbsRealLValue.h"
#include "RooAddPdf.h"
#include "RooArgSet.h"
#include "RooDataSet.h"
#include "RooHist.h"
#include "RooPlot.h"
#include "RooCrystalBall.h"
#include "RooConstVar.h"
#include "RooFormulaVar.h"
#include "RooFitResult.h"


// create the pull distribution from the frame where the fit is performed
TPad* GetPadPullDistribution(RooPlot* frame, RooDataSet data, RooAddPdf model, double reducedchisq, const int nFitPars) {
	TPad* bottomPad = new TPad("bottomPad", "bottomPad", 0, 0.0, 0.98, .25);
	bottomPad->SetLeftMargin(0.17);
	bottomPad->SetTopMargin(0.015);
	bottomPad->SetBottomMargin(0.4);
	bottomPad->SetTicks(1, 1);
	bottomPad->Draw();
	bottomPad->cd();

	RooPlot* pullFrame = (frame->getPlotVar())->frame(frame->GetXaxis()->GetXmin(), frame->GetXaxis()->GetXmax());
	RooDataSet Data = data;
	RooAddPdf Fit = model;
	pullFrame->addPlotable(frame->pullHist("data","Fit"), "PZ");
	pullFrame->SetTitle(" ");
	pullFrame->GetYaxis()->SetTitleOffset(0.35);
	pullFrame->GetYaxis()->SetTitle("Pull");
	pullFrame->GetYaxis()->SetTitleSize(0.17);
	pullFrame->GetYaxis()->SetLabelSize(0.15);
	pullFrame->GetYaxis()->CenterTitle();

	// pullFrame->GetXaxis()->SetTitle("#mu^{+}#mu^{#minus} inv. mass (GeV)");
	pullFrame->GetXaxis()->SetTitle("e^{+}e^{#minus} inv. mass (GeV)");
	//pullFrame->GetXaxis()->SetTitleOffset(1.20);
	//pullFrame->GetXaxis()->SetLabelOffset(0.1);
	pullFrame->GetXaxis()->SetLabelSize(0.15);
	pullFrame->GetXaxis()->SetTitleSize(0.17);
	pullFrame->GetXaxis()->CenterTitle();
	// pullFrame->GetXaxis()->SetTitleFont(43);
	// pullFrame->GetYaxis()->SetTitleFont(43);

	pullFrame->GetYaxis()->SetTickSize(0.03);
	pullFrame->GetYaxis()->SetNdivisions(505);
	pullFrame->GetXaxis()->SetTickSize(0.1);
	pullFrame->Draw();

	pullFrame->SetMaximum(5.0);
	pullFrame->SetMinimum(-5.0);

	TLatex textChi2;
	textChi2.SetTextAlign(12);
	textChi2.SetTextSize(0.15);
	// textChi2.DrawLatexNDC(0.7, 0.85, Form("#chi^{2} / n_{d.o.f.} = %.1f", frame->chiSquare(nFitPars)));
	// textChi2.DrawLatexNDC(0.75, 0.13, Form("#chi^{2} / n_{d.o.f.} = %.1f", reducedchisq));

	return bottomPad;
}