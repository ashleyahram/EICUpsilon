#include "TPaveText.h"
#include "RooRealVar.h"

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


TPaveText* DSCBParamsText(RooRealVar mean, RooRealVar sigma, RooRealVar alphaL, RooRealVar orderL, RooRealVar alphaR, RooRealVar orderR) {
	auto* text = new TPaveText(0.15, 0.85, 0.45, 0.45, "NDCNB");
	text->SetBorderSize(0);

	text->AddText("Double-sided CB");
	text->AddText(Form("m = %.3f #pm %.3f GeV", mean.getVal(), mean.getError()));
	text->AddText(Form("#sigma = %.2f #pm %.2f MeV", 1000 * sigma.getVal(), 1000 * sigma.getError()));
	text->AddText(Form("#alpha_{L} = %.3f #pm %.3f", alphaL.getVal(), alphaL.getError()));
	text->AddText(Form("n_{L} = %.3f #pm %.3f", orderL.getVal(), orderL.getError()));
	text->AddText(Form("#alpha_{H} = %.3f #pm %.3f", alphaR.getVal(), alphaR.getError()));
	text->AddText(Form("n_{H} = %.3f #pm %.3f", orderR.getVal(), orderR.getError()));

	text->SetAllWith(" ", "align", 12);
	return text;
}


TPaveText* DSCBParamsText2(RooFormulaVar mean, RooRealVar sigma, RooRealVar alphaL, RooRealVar orderL, RooRealVar alphaR, RooRealVar orderR) {
	auto* text = new TPaveText(0.65, 0.85, 0.95, 0.45, "NDCNB");
	text->SetBorderSize(0);

	text->AddText("Double-sided CB");
	text->AddText(Form("m = %.3f", mean.getVal()/*, mean.getError()*/));
	text->AddText(Form("#sigma = %.2f #pm %.2f MeV", 1000 * sigma.getVal(), 1000 * sigma.getError()));
	text->AddText(Form("#alpha_{L} = %.3f #pm %.3f", alphaL.getVal(), alphaL.getError()));
	text->AddText(Form("n_{L} = %.3f #pm %.3f", orderL.getVal(), orderL.getError()));
	text->AddText(Form("#alpha_{H} = %.3f #pm %.3f", alphaR.getVal(), alphaR.getError()));
	text->AddText(Form("n_{H} = %.3f #pm %.3f", orderR.getVal(), orderR.getError()));

	text->SetAllWith(" ", "align", 12);
	return text;
}