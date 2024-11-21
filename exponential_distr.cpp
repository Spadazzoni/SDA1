#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooExponential.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
using namespace RooFit;

void exponential_distr()
{
    // defining observable and parameter
    RooRealVar x{"x", "Observable", -20, -16, "GeV"};
    RooRealVar a{"a", "1/tau", -3., -4.4, -1.6};

    // defining the fit function
    RooExponential model{"model", "Exponential", x, a};

    // plotting the fit function
    auto frame = x.frame();
    model.plotOn(frame);
    frame->Draw();

    // generating events distributed according the fit function
    x.setBins(8);
    auto data = model.generateBinned(x, 1000);

    // fitting
    model.fitTo(*data);

    // drawing data + fit
    new TCanvas;
    auto frame2 = x.frame();
    data->plotOn(frame2);
    model.plotOn(frame2);
    frame2->Draw();
}