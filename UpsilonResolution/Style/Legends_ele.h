#include "TPaveText.h"

#include "RooRealVar.h"
// 0.5904034,0.5465839,0.9623916,0.879917
TPaveText* KinematicsText(Int_t yMin, Int_t yMax) {
	TPaveText* text = new TPaveText(0.59, 0.55, 0.96, 0.88, "NDCNB");
	text->SetFillColor(4000);
	text->SetBorderSize(0);
	// text->AddText(Form("Centrality %d-%d%%", centMin, centMax));
	// text->AddText("p_{T}^{e} > 3.5 GeV");
	text->AddText("#varUpsilon(1S)+#varUpsilon(2S)+#varUpsilon(3S) #rightarrow e^{+}e^{#minus}");
	text->AddText("eSTARlight");
	text->AddText("10^{-3} < Q^{2} < 10 GeV^{2}");
	text->AddText("no limit on W");
	text->AddText(Form("%d < y_{e^{+}e^{-}} < %d", yMin, yMax));
	// text->AddText(Form("%d < p_{T}^{ee} < %d GeV", ptMin, ptMax));

	text->SetAllWith("", "align", 32);
	text->SetTextFont(42);

	return text;
}
// 0.2013514,0.3809524,0.4829022,0.8095238
TPaveText* DSCBParamsText(RooRealVar mean, RooRealVar sigma, RooRealVar alphaL, RooRealVar orderL, RooRealVar alphaR, RooRealVar orderR) {
	TPaveText* text = nullptr;
	if (sigma.getVal() * 1000 < 100) text = new TPaveText(0.20, 0.38, 0.48, 0.81, "NDCNB");
	else if (sigma.getVal() * 1000 >= 100) text = new TPaveText(0.20, 0.38, 0.50, 0.81, "NDCNB");
	
	text->SetBorderSize(0);

	text->AddText("Double-sided CB");
	text->AddText(Form("m = %.3f #pm %.3f GeV", mean.getVal(), mean.getError()));
	text->AddText(Form("#sigma = %.1f #pm %.1f MeV", 1000 * sigma.getVal(), 1000 * sigma.getError()));
	text->AddText(Form("#alpha_{L} = %.3f #pm %.3f", alphaL.getVal(), alphaL.getError()));
	text->AddText(Form("n_{L} = %.3f #pm %.3f", orderL.getVal(), orderL.getError()));
	text->AddText(Form("#alpha_{H} = %.3f #pm %.3f", alphaR.getVal(), alphaR.getError()));
	text->AddText(Form("n_{H} = %.3f #pm %.3f", orderR.getVal(), orderR.getError()));
	
	text->SetAllWith(" ", "align", 12);
	text->SetTextFont(42);

	return text;
}

TPaveText* RefFrameText(Bool_t isCSframe, Float_t cosThetaMin, Float_t cosThetaMax, Int_t phiMin, Int_t phiMax) {
	TPaveText* text = new TPaveText(0.6, 0.9, 0.95, 0.65, "NDCNB");
	text->SetFillColor(4000);
	text->SetBorderSize(0);
	text->AddText(isCSframe ? "Collins-Soper frame" : "Helicity frame");
	text->AddText(Form("%.2f < cos #theta < %.2f", cosThetaMin, cosThetaMax));
	text->AddText(Form("%d#circ < #varphi < %d#circ", phiMin, phiMax));

	text->SetAllWith("", "align", 32);
	return text;
}

TPaveText* FitResultText(RooRealVar n1S, Float_t signif1S, RooRealVar n2S, Float_t signif2S) {
	TPaveText* text = new TPaveText(0.6, 0.6, 0.95, 0.3, "NDCNB");
	text->SetFillColor(4000);
	text->SetBorderSize(0);
	text->AddText(Form("N(#varUpsilon(1S)) = %.0f^{ #plus%.0f}_{ %.0f}", n1S.getVal(), n1S.getErrorHi(), n1S.getErrorLo()));
	text->AddText(Form("S / #sqrt{S+B} (3#sigma) = %.1f", signif1S));
	text->AddText(Form("N(#varUpsilon(2S)) = %.0f^{ #plus%.0f}_{ %.0f}", n2S.getVal(), n2S.getErrorHi(), n2S.getErrorLo()));
	text->AddText(Form("S / #sqrt{S+B} (3#sigma) = %.1f", signif2S));
	text->SetAllWith("", "align", 32);
	return text;
}

