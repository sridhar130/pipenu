//-----PIPENU signal/background fit------------------
// *Take the fit parameters from siganl and background
// and generate a combined fit function.
// *Vary the combined fit gaussian uncertainty of bin
// content and fit 1000 times
// *Find the mean and sigma of the fitted means
// *Try with other background fits, and not just single
// exponential
// *Pull over peak.
// * Try this on normalized peaks and non normalized ones.
// * Currently the crystal ball fit is defined in two places.
// * Write a class to combine.
//-----------------------------------------------------
#ifdef __CLING__
#pragma cling optimize(0)
#endif

#include "TFile.h"
#include "Stntuple/val/stntuple_val_functions.hh"
#include "Stntuple/val/hist_file.hh"
#include "Stntuple/val/hist_data.hh"
#include "Stntuple/val/stn_book.hh"
#include "Stntuple/val/stn_catalog.hh"

#include "pipenu/ana/Analysis.hh"
R__LOAD_LIBRARY($/exp/mu2e/app/users/tripathy/Development_pipenu4/build/al9-prof-e28-p057/pipenu/lib/libpipenu_ana.so)

pipenu::Analysis* gPipenu; 


#include <TMath.h>
#include <TCanvas.h>
#include <TF1NormSum.h>
#include <TF1.h>
#include <TH1.h>
#include "TRandom3.h"

using namespace std;



// Define single exponential------------------
double sing_expo(double* X, double* P) {
  double f;
  f = P[0]*TMath::Exp(P[1]*X[0]+P[2]);
  return f;
}
// Define quad expo---------------------------
double quad_expo(double* X, double* P) {
  double f;
  f = TMath::Exp(P[0]*X[0]*X[0]+P[1]*X[0]+P[2]);
  return f;
}
//Define double expo-------------------------
double double_expo(double* X, double* P) {
  double f;
  f = P[0]*TMath::Exp(P[1]*X[0]+P[2])+P[3]*TMath::Exp(P[4]*X[0]+P[5]);
  return f;
}

// Define crystall ball-----------------------
double crystalball_root(double* X, double*P) {
       double mean    = P[0];
       double sigma   = P[1];
       double alpha   = P[2];
       double n       = P[3];
       double N       = P[4];
       double x       = X[0];

       // evaluate the crystal ball function
       if (sigma < 0.)     return 0.;
       double z = (x - mean)/sigma; 
       if (alpha < 0) z = -z; 
       double abs_alpha = std::abs(alpha);
       // double C = n/abs_alpha * 1./(n-1.) * std::exp(-alpha*alpha/2.);
       // double D = std::sqrt(M_PI/2.)*(1.+ROOT::Math::erf(abs_alpha/std::sqrt(2.)));
       // double N = 1./(sigma*(C+D));
       if (z  > - abs_alpha)
          return N*std::exp(- 0.5 * z * z);
       else {
          //double A = std::pow(n/abs_alpha,n) * std::exp(-0.5*abs_alpha*abs_alpha);
          double nDivAlpha = n/abs_alpha;
          double AA =  std::exp(-0.5*abs_alpha*abs_alpha);
          double B = nDivAlpha -abs_alpha;
          double arg = nDivAlpha/(B-z);
          return N*AA * std::pow(arg,n);
       }
    }

double combo_fit(double* X, double*P)
{ return sing_expo(X,P) + crystalball_root(X,&P[3]);
}

double combo_fit_quad(double* X, double*P)
{ return quad_expo(X,P) + crystalball_root(X,&P[3]);
}

double combo_fit_double(double* X, double*P)
{ return double_expo(X,P) + crystalball_root(X,&P[3]);
}


void total_fit1()
{

  gPipenu = new pipenu::Analysis("a");
  TCanvas *c1 = new TCanvas("c1"," ",900,750);

  TH1F * hist1 = gh1("/exp/mu2e/data/projects/pipenu/hist/tripathy/pipenu.bpip4b0s51r0105.tripathy_pipenu_pipenu_ana.hist","PipenuAna","trk_178/p_2");
  hist1->Rebin(2);

  hist1->Scale(gPipenu->GetChannel("bpip4b0s51r0105")->NormSF());
  //hist1->Scale(1e16);
  TH1F * hist2 = gh1("/exp/mu2e/data/projects/pipenu/hist/tripathy/pipenu.bmup4b0s66r0105.tripathy_pipenu_pipenu_ana.hist","PipenuAna","trk_126/p_2");
  hist2->Rebin(2);
  hist2->Scale( gPipenu->GetChannel("bmup4b0s66r0100")->NormSF());
  //hist2->Scale(1e16);
  hist2->Add(hist1);

  hist2->GetXaxis()->SetRangeUser(60.,72.);
  hist2->SetLineColor(kBlue);
  hist2->SetLineStyle(1);
  hist2->SetLineWidth(2);
  hist2->SetMarkerStyle(20);
  hist2->SetMarkerSize(1.2);
  hist2->SetName("Signal_dif");
  hist2->SetMarkerColorAlpha(kBlue, 0.75);

  //  hist2->Draw();









  
  //Define the combined fit--------------------
  

  //TF1* total = new TF1("total","sing_expo(x,[0],[1],[2])+crystalball_root(x,[3],[4],[5],[6],[7])",60.,72.0);
  //TF1* total1 = new TF1("total1","sing_expo",60.,72.0,3);
  //TF1* total2 = new TF1("total2","crystalball_root",64.0,70.0,5);
  TF1* total = new TF1("total","combo_fit",60.,72.0,8);
  //TF1* total = new TF1("total","combo_fit_quad",60.,72.0,8);
  //TF1* total = new TF1("total","combo_fit_double",60.,72.0,11;)
   TF1* cb = new TF1("cb","crystalball_root",64.,70.0,5);

   

  //Parameters for the single combined fit--------------------
  
     //  total->SetParameter(0,0.9986);
  //  total->SetParameter(0,1.15);
  total->SetParameter(0,1.08e-5);
  //  total->SetParLimits(0,0.5,1.5);
  total->SetParameter(1,-0.1817);
  total->SetParameter(2,16.37);
  //  total->SetParameter(2,-20.31);

   
  // // set parameters from normalized DIF ogram
  // total->SetParameter(0,0.9986);
  // total->SetParameter(1,-0.1817);
  // total->SetParameter(2,-20.31);

  // // set parameters from normalized signal ogram
  //  total->SetParameter(7,2.65e-14);
  total->SetParameter(7,281.646);
  //total->SetParLimits(2,200.,400.);

  total->SetParameter(3,68.83);
  //  total->SetParLimits(3,66.,70.);
  total->SetParameter(4,0.319);
  total->SetParameter(5,0.46);
  //  total->SetParameter(5,0.47);
  //  total->SetParameter(6,53.);
  total->SetParameter(6,-4.72e8);
  //  total->SetParameter(6,51.82e5);
  //  total->SetParLimits(6,45.,60.0);
  


  

  //Parameters for the quad combined fit--------------------
  /*
  total->SetParameter(0,-0.0078);
   //  total->SetParLimits(0,0.5,1.5);
  total->SetParameter(1,0.8341);
  total->SetParameter(2,-53.61);
  //  total->SetParameter(2,-20.31);

  // // set parameters from normalized signal ogram
  total->SetParameter(7,2.65e-14);
  //total->SetParameter(7,268.3);
  //total->SetParLimits(2,200.,400.);

  total->SetParameter(3,68.93);
  total->SetParLimits(3,66.,70.);
  total->SetParameter(4,0.3232);
  total->SetParameter(5,0.46);
  //  total->SetParameter(5,0.47);
  //  total->SetParameter(6,53.);
  total->SetParameter(6,51.82);
  total->SetParLimits(6,45.,60.0);
  */

  //Parameters for the quad combined fit--------------------
  /* 
  total->SetParameter(0,7.36e-15);
   //  total->SetParLimits(0,0.5,1.5);
  total->SetParameter(1,-0.1801);
  total->SetParameter(2,-53.61);
  //  total->SetParameter(2,-20.31);

  // // set parameters from normalized signal ogram
  total->SetParameter(7,2.65e-14);
  //total->SetParameter(7,268.3);
  //total->SetParLimits(2,200.,400.);

  total->SetParameter(3,68.93);
  total->SetParLimits(3,66.,70.);
  total->SetParameter(4,0.3232);
  total->SetParameter(5,0.46);
  //  total->SetParameter(5,0.47);
  //  total->SetParameter(6,53.);
  total->SetParameter(6,51.82);
  total->SetParLimits(6,45.,60.0);
  */

    

  //hist2->Fit("total","SQ","",60.0,72.0);
  //hist2->Fit("total","SQ","","R");

 
  total->Draw();
   // total2->Draw("same");

    
   double tt = (*total)(67.5);
   double x_value;
   double sigma;
   double gaus_sig;
   double bin_error;
   double true_peak;
   double fit_peak;
   double true_sigma;
   double bin_val;
   TRandom3 * r = new TRandom3();
   cout << "total value: "<<tt<<endl;
   TH1D * hSB0 = new TH1D("hSB0","histoSB-quad expo",60,60.,72.);
   // Fill each bin content of this histogram with the fucntion value here.
   int nbins  = hSB0->GetNbinsX();
   cout<<" NBINS--" <<nbins<<endl;
   int iter   = 1000;
   double inc = (72.0-60.0)/nbins;
   for (int j=0; j<=nbins; j++)
     {
       x_value = (*total)(60.0+inc*j);
       hSB0->SetBinContent(j,x_value);
     }

   //hSB0->Fit("total","L","",60.0,72.0);
   true_peak=total->GetParameter(3);
   true_sigma=total->GetParError(3);
   //hSB0->Draw("same");

   TH1D * hSB = new TH1D("hSB","histoSB1",60,60.,72.);


    // hpull->Fill((true_peak-(total->GetParameter(3)))/(total->GetParError(3)));
    //hcons->Fill(true_sigma-total->GetParError(3));
 
}
 
