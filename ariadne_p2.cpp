#include "RooRealVar.h"
#include "RooGaussian.h"
#include "RooDataHist.h"
#include "RooAbsPdf.h"
#include "RooLandau.h"
#include "RooFFTConvPdf.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "RooFitResult.h"
#include "TAxis.h"
#include <fstream>

using namespace RooFit;

void ariadne_p2()
{
    // defining observable
    RooRealVar dt{"dt", "dI/dX", 9e3, 31e3, "ADU/cm"};
    dt.setBins(50);

    // defining fit parameters
    RooRealVar ml{"ml", "m_{L}", 14e3, 13e3, 16e3};
    RooRealVar sl{"sl", "s_{L}", 1e3, 500, 2e3};
    RooRealVar mg{"mg", "m_{G}", 0.1, 0, 1};
    RooRealVar sg{"sg", "s_{G}", 1e3, 500, 2e3};

    // defining components of fit function
    RooLandau l{"l", "Landau", dt, ml, sl};
    RooGaussian g{"g", "Gauss", dt, mg, sg};

    // defining fit function
    RooFFTConvPdf model{"model", "Landau * Gauss", dt, l, g};

    // declaring data container + getting ready to read from file
    RooDataHist data{"data", "data", dt};
    Float_t val;
    Float_t weight;
    std::ifstream in;
    in.open("ariadne_g012.dat");

    // reading from file
    while (1)
    {
        in >> val >> weight;
        if (!in.good())
        {
            break;
        }
        if (val < 9e3 || val > 31e3)
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
    auto xframe = dt.frame(Title("Landau#otimesGauss"));
    data.plotOn(xframe);
    model.plotOn(xframe);
    model.paramOn(xframe, Format("teu", AutoPrecision(1)), Layout(0.6, 0.65, 0.9));
    xframe->Draw();

    // getting ready to write on file
    std::ofstream txt("./FitRes2.txt", std::ofstream::out);

    // writing on file
    if (txt.is_open())
    {
        txt << "Fit results:" << '\n';
        txt << '\n';
        txt << "m_{L}  =  " << ml.getVal() << "  +/-  " << ml.getError() << "  ADU/cm" << '\n';
        txt << "s_{L}  =  " << sl.getVal() << "  +/-  " << sl.getError() << "  ADU/cm" << '\n';
        txt << "m_{G}  =  " << mg.getVal() << "  +/-  " << mg.getError() << "  ADU/cm" << '\n';
        txt << "s_{G}  =  " << sg.getVal() << "  +/-  " << sg.getError() << "  ADU/cm" << '\n';
        txt << '\n';
        txt.close();
    }
    else
    {
        std::cout << "Cannot find or open file." << '\n';
    }
}