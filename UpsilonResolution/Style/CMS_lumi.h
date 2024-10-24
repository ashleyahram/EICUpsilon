#include "TPad.h"
#include "TLatex.h"
#include "TLine.h"
#include "TBox.h"
#include "TASImage.h"

//
// Global variables
//

// TString cmsText = "CMS";
TString cmsText = "Private Work";
float cmsTextFont = 61; // default is helvetic-bold

bool writeExtraText = false;
TString extraText = "Preliminary";
float extraTextFont = 52; // default is helvetica-italics

// text sizes and text offsets with respect to the top frame
// in unit of the top margin size
// float lumiTextSize = 0.6;
float lumiTextSize = 0.55;
float lumiTextOffset = 0.2;
// float cmsTextSize = 0.75;
float cmsTextSize = 0.70;
float cmsTextOffset = 0.1; // only used in outOfFrame version

float relPosX = 0.045;
float relPosY = 0.035;
float relExtraDY = 1.2;

// ratio of "CMS" and extra text size
float extraOverCmsTextSize = 0.76;

TString lumi_13TeV = "20.1 fb^{-1}";
TString lumi_8TeV = "19.7 fb^{-1}";
TString lumi_7TeV = "5.1 fb^{-1}";
TString lumi_pPb = "pPb 173.4 nb^{-1} (8.16 TeV)";
TString lumi_PbPb = "PbPb 1.6 nb^{-1} (5.02 TeV)";

bool drawLogo = false;

void CMS_lumi(TPad* pad, int iPeriod = 3, int iPosX = 10);
