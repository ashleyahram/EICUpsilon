#include "RooPlot.h"
#include "RooArgSet.h"
#include "RooAbsRealLValue.h"
#include "RooHist.h"

// create the pull distribution from the frame where the fit is performed
TPad* GetPadPullDistribution(RooPlot* frame, const int nFitPars) {
	TPad* bottomPad = new TPad("bottomPad", "bottomPad", 0, 0.0, 1, .25);
	bottomPad->SetTopMargin(0.015);
	bottomPad->SetBottomMargin(0.4);
	bottomPad->SetTicks(1, 1);
	bottomPad->Draw();
	bottomPad->cd();

	RooPlot* pullFrame = (frame->getPlotVar())->frame(frame->GetXaxis()->GetXmin(), frame->GetXaxis()->GetXmax());
	pullFrame->addPlotable(frame->pullHist(), "PZ");
	pullFrame->SetTitle(" ");
	pullFrame->GetYaxis()->SetTitleOffset(0.35);
	pullFrame->GetYaxis()->SetTitle("Pull");
	pullFrame->GetYaxis()->SetTitleSize(0.17);
	pullFrame->GetYaxis()->SetLabelSize(0.15);
	pullFrame->GetYaxis()->CenterTitle();

	//pullFrame->GetXaxis()->SetTitle("m_{#mu^{+}#mu^{#minus}} (GeV)");
	//pullFrame->GetXaxis()->SetTitleOffset(1.20);
	//pullFrame->GetXaxis()->SetLabelOffset(0.1);
	pullFrame->GetXaxis()->SetLabelSize(0.15);
	pullFrame->GetXaxis()->SetTitleSize(0.17);
	pullFrame->GetXaxis()->CenterTitle();
	// pullFrame->GetXaxis()->SetTitleFont(43);
	// pullFrame->GetYaxis()->SetTitleFont(43);

	pullFrame->GetYaxis()->SetTickSize(0.03);
	//pullFrame->GetYaxis()->SetNdivisions(505);
	pullFrame->GetXaxis()->SetTickSize(0.1);
	pullFrame->Draw();

	pullFrame->SetMaximum(4.6);
	pullFrame->SetMinimum(-4.6);

	TLatex textChi2;
	textChi2.SetTextAlign(12);
	textChi2.SetTextSize(0.15);
	textChi2.DrawLatexNDC(0.7, 0.85, Form("#chi^{2} / n_{d.o.f.} = %.1f", frame->chiSquare(nFitPars)));

	return bottomPad;
}

void SaveMCSignalTailParameters(RooArgSet params, const char* outputName) {
	params.writeToFile(Form("../MonteCarlo/SignalParameters/%s.txt", outputName));
}
