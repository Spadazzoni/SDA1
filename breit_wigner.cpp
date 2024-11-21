#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooBreitWigner.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
#include "TFile.h"
#include "TH1.h"

using namespace RooFit;

void breit_wigner()
{
    // retrieving data from file
    TFile *MyFile = new TFile("B0sInvariantMass.root", "read");
    TH1F *h = (TH1F *)MyFile->Get("massaB0");

    // retrieving parameters from data
    Float_t max = h->GetMaximum();
    Float_t min = h->GetMinimum();
    Float_t mean = h->GetMean();
    Float_t err_mean = h->GetMeanError();
    Float_t sigma = h->GetRMS();
    Float_t err_sigma = h->GetRMSError();

    // defining observable and data container
    RooRealVar x{"x", "Mass", min, max, "Mev/c^{2}"};
    RooDataHist data("data", "data", x, Import(*h));

    // defining fit parameters
    RooRealVar m{"m", "mean", mean, 0, 5 * mean};
    RooRealVar g{"g", "gamma", sigma, 0, 5 * sigma};

    // defining 1st fit function
    RooBreitWigner model1{"model1", "BW distribution", x, m, g};

    // fitting
    model1.fitTo(data);

    // defining 2nd fit function
    RooGaussian model2{"model2", "Gaussian distribution", x, m, g};

    // fitting
    model2.fitTo(data);

    // plotting + cosmetics
    auto frame = x.frame(Title("Invariant mass spectrum for a B0 meson"));
    data.plotOn(frame);
    model1.plotOn(frame);
    model2.plotOn(frame, LineColor(kRed));
    frame->Draw();
}