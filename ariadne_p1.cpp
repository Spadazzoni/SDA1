#include "RooRealVar.h"
#include "RooUniform.h"
#include "RooDataHist.h"
#include "RooAbsPdf.h"
#include "RooLandau.h"
#include "RooAddPdf.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "RooFitResult.h"
#include "TAxis.h"
#include <fstream>

using namespace RooFit;

void ariadne_p1()
{
    // defining observable
    RooRealVar dt{"dt", "Drift time", 460, 530, "#mus"};
    dt.setBins(35);

    // defining fit parameters
    RooRealVar l{"l", "m_{L}", 492, 490, 494};
    RooRealVar c{"c", "s_{L}", 2, 1, 3};
    RooRealVar f{"f", "f_{L}", 0.3, -0.5, 1.5};

    // defining fit functions + model
    RooUniform bkg{"bkg", "Background component", dt};
    RooLandau sig{"sig", "Signal component", dt, l, c};
    RooAddPdf model{"model", "Composite model", RooArgList(sig, bkg), f};

    // declaring data container + getting ready to read from file
    RooDataHist data{"data", "data", dt};
    Float_t val;
    Float_t weight;
    std::ifstream in;
    in.open("ariadne_g006_plus_400.dat");

    // reading from file
    while (1)
    {
        in >> val >> weight;
        if (!in.good())
        {
            break;
        }
        if (val < 460 || val > 530)
        {
            continue;
        }
        dt.setVal(val);
        data.set(dt, weight);
    }

    in.close();

    // fitting + saving fit results
    RooFitResult *res = model.fitTo(data, Save());

    // plotting + cosmetics
    auto xframe = dt.frame(Title("Drift time"));
    data.plotOn(xframe);
    model.paramOn(xframe, Format("teu", AutoPrecision(1)), Layout(0.6, 0.65, 0.9));
    model.plotOn(xframe, Components("sig"), LineColor(kRed), LineStyle(kDashed));
    model.plotOn(xframe, Components("bkg"), LineColor(kBlack), LineStyle(kDashed));
    model.plotOn(xframe);
    xframe->Draw();

    // getting ready to write on file
    std::ofstream txt("./FitRes1.txt", std::ofstream::out);

    // writing on file
    if (txt.is_open())
    {
        txt << "Fit results:" << '\n';
        txt << '\n';
        txt << "m_{L}  =  " << l.getVal() << "  +/-  " << l.getError() << "  us" << '\n';
        txt << "s_{L}  =  " << c.getVal() << "  +/-  " << c.getError() << "  us" << '\n';
        txt << "f_{L}  =  " << f.getVal() << "  +/-  " << f.getError() << '\n';
        txt << '\n';
        txt.close();
    }
    else
    {
        std::cout << "Cannot find or open file." << '\n';
    }
}