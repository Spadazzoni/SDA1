#include "RooRealVar.h"
#include "RooFormulaVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooExponential.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
#include "RooAddPdf.h"

using namespace RooFit;

void composite_model()
{
   // defining observable and fit parameters
   RooRealVar x{"x", "Observable", -15, 25};
   RooRealVar m{"m", "mean", 0, -5, 5};
   RooRealVar s{"s", "sigma", 3, 0, 6};
   RooRealVar t{"t", "tau", 10, 5, 15};

   RooFormulaVar b{"beta", "Average decay rate", "-@1/@0", RooArgList(t, 1.)};

   // defining signal and background components
   RooGaussian sig{"sig", "Signal component", x, m, s};
   RooExponential bkg{"bkg", "Background component", x, b};

   // defining fraction of signal
   RooRealVar fsig{"fsig", "Fraction of events", 0.3, 0., 1.};

   // adding signal and background
   RooAddPdf model{"model", "Composite model", RooArgList(sig, bkg), fsig};

   // plotting s + b
   auto frame = x.frame();
   model.plotOn(frame);
   frame->Draw();

   // generating data according to s + b
   x.setBins(80);
   auto data = model.generateBinned(x, 1e3);

   // fitting
   model.fitTo(*data);

   // plotting + cosmetics
   new TCanvas();
   auto frame2 = x.frame(Title("Composite model: gaussian + exponential"));
   data->plotOn(frame2);
   model.plotOn(frame2);
   frame2->Draw();
}
