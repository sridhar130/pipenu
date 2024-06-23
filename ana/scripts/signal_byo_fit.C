//-----------------------------------------------
// Fitting with user-defined crystal ball function.
// The Function:
// N*exp((x-x0)^2/2sig^2 for (x-x0)/sig > -alpha  (gaussian part)
// N*A*(B-(x-x0)/sig)^{-n} for (x-x0)/sig <= -alpha (power law part)
// A = (n/abs(alpha))^n*exp(-(abs(alpha)^2)/2)
// B = n/abs(alpha)-abs(alpha)
// Parameters:
// P[0] = N (normalization)
// P[1] = x0 (mean value)
// P[2] = sig (sigma)
// P[3] = alpha  (> 0)
// P[4] = n, power of the power law tail?
// P[5] = tail fraction (may not be required/ extra handle)
// P[6] = tail exponential slope (may not be required/ extra handle)
//-----------------------------------------------

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
//gSystem->Load("$/exp/mu2e/app/users/tripathy/Development_pipenu4/build/al9-prof-e28-p057/pipenu/lib/libpipenu_ana.so"); 

pipenu::Analysis* gPipenu; 
//-----------------------------------------------
// Define the fit function
//-----------------------------------------------
double f_crystal_ball(double* X, double* P) {
  double f, alpha, abs_alpha, n, a, b;

  double dx0 = (X[0]-P[1]);
  double dx = (X[0]-P[1])/P[2];

  alpha     = P[3];
  abs_alpha = fabs(alpha);
  n         = P[4];

  if (dx0 > -abs_alpha) {

    f = P[0]*TMath::Exp(-dx*dx/2.);
  }
  else {
    a = TMath::Power((n/abs_alpha),n)*TMath::Exp(-(alpha*alpha)/2);
    b = (n/abs_alpha)-abs_alpha;
    f = P[0]*a*TMath::Power((b-dx),-n);
  }
  
  return f;
}

double f_crystal_ball1(double* X, double* P) {
  double f1, alpha, abs_alpha, n, a, b;

  double dx0 = (X[0]-P[1]);
  double dx = (X[0]-P[1])/P[2];

  alpha     = P[3];
  abs_alpha = fabs(alpha);
  n         = P[4];

  if (dx0 > -abs_alpha) {
    if (dx < 0) {
      f1 = P[0]*TMath::Exp(-dx*dx/2.);
    }
    else if (dx0 < 0.5) {
      f1 = P[0]*(1-P[5])*TMath::Exp(-dx*dx/2.);
    }
    else {
      f1 = P[0]*((1-P[5])*TMath::Exp(-dx*dx/2.)+P[5]*TMath::Exp(-dx/P[6]));
    }
  }
  else {
    a = TMath::Power((n/abs_alpha),n)*TMath::Exp(-(alpha*alpha)/2);
    b = n/abs_alpha-abs_alpha;
    f1 = P[0]*a*TMath::Power((b-dx),-n);
  }
  
  return f1;
}

// dependent n
double f_crystal_ball2(double* X, double* P) {
  double f, alpha, abs_alpha, n, a, b, C, D, nby_alpha, exp_alpha, erf_alpha, N;

  double dx0 = (X[0]-P[0]);
  double dx  = (X[0]-P[0])/P[1];

  alpha     = P[2];
  abs_alpha = fabs(alpha);
  n         = P[3];
  nby_alpha = n/abs_alpha;
  exp_alpha = TMath::Exp(-abs_alpha*abs_alpha*0.5);
  erf_alpha = TMath::Erf(abs_alpha*0.7071); // |alpha|/sqrt(2)
  C         = (nby_alpha/(n-1))*exp_alpha;
  D         = 1.2533*(1+erf_alpha); // sqrt(pi/2)
  N         = 1/(P[1]*(C+D));
    
  if (dx0 > -abs_alpha) {

    f = N*TMath::Exp(-dx*dx/2.);
  }
  else {
    a = TMath::Power((n/abs_alpha),n)*TMath::Exp(-(alpha*alpha)/2);
    b = (n/abs_alpha)-abs_alpha;
    f = N*a*TMath::Power((b-dx),-n);
  }
  
  return f;
}

// dependent cyrstal ball root

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

Double_t myFunc(double x) { 
  double ff = x+sin(x);
  return ff; }

//-----------------------------------------------
// main function
//-----------------------------------------------
void signal_byo_fit()
{
  
  // gStyle->SetOptFit(1011);
 gPipenu = new pipenu::Analysis("a");
 TCanvas *c1 = new TCanvas("c1"," ",900,750);
 double p0, p1, p2, p3, p4;
 p0=1.5;
 p1=68.9;
 p2=1.5;
 p3=1.5;
 p4=2.0;

 TH1F * hist1 = gh1("/exp/mu2e/data/projects/pipenu/hist/tripathy/pipenu.bpip4b0s51r0105.tripathy_pipenu_pipenu_ana.hist","PipenuAna","trk_178/p_2");
 hist1->Rebin(2);

 hist1->Scale(gPipenu->GetChannel("bpip4b0s51r0105")->NormSF());
 // hist1->Scale(1e16); //check with and with noramlizing
 hist1->SetLineColor(kRed);
 hist1->SetLineStyle(1);
 hist1->SetLineWidth(2);
 hist1->SetMarkerStyle(20);
 hist1->SetMarkerSize(1.2);
 hist1->SetName("Signal");
 hist1->SetMarkerColorAlpha(kRed, 0.75);

 //common-------------
 hist1->GetXaxis()->SetTitle("p (MeV/c)");
 hist1->GetYaxis()->SetTitle("N/ 0.2 MeV/c");
 hist1->GetYaxis()->SetTitleOffset(1.25);
 hist1->GetXaxis()->SetRangeUser(64.,72.);
 //hist1->GetYaxis()->SetRangeUser(0.0,1e-13);
 hist1->SetTitle("pipenu signal, crystal ball fit ");
 hist1->Draw();

 //fitting------------
 //crystalball_root
 TF1 * cbFit1 = new TF1("cbFit1",crystalball_root,64.0,70.0,5);
 TF1 * u_cbFit = new TF1("u_cbFit",f_crystal_ball,64.0,70.0,5);
 TF1 * u_cbt = new TF1("u_cb",f_crystal_ball,64.0,70.0,5); 
 //TF1 *f1 = new TF1("myfunc",myfunction,0,10,2);
 // u_cbFit->SetParameter(0, 2.65e-14); // normalization (peak height)
 // u_cbFit->SetParName(0,"normt");
 // u_cbFit->SetParLimits(0,2.6e-14,3.1e-14);

 u_cbFit->SetParameter(0, 269.0); // normalization (peak height)
 u_cbFit->SetParName(0,"normt");
 //u_cbFit->SetParLimits(0,268.0,275.0);

 //u_cbFit->SetParameters(68.9,0.32,0.5,6); 
 u_cbFit->SetParameter(1, 68.9); // mean value
 u_cbFit->SetParName(1,"mean");
 u_cbFit->SetParLimits(1,68.7,69.0);
 u_cbFit->SetParameter(2, 0.32); // sigma
 u_cbFit->SetParName(2,"sigma");
 // u_cbFit->SetParLimits(2,0.2,0.4);
 u_cbFit->SetParameter(3, 1.3); // power law tail parameter
 u_cbFit->SetParName(3,"alpha");
 // u_cbFit->SetParLimits(3,1.0,2.0);
 u_cbFit->SetParameter(4, 52); // power of the power law component (exponent)
 u_cbFit->SetParName(4,"n");
 // u_cbFit->SetParLimits(4,0,70);

 u_cbt->SetParameters(265.42,68.89,0.34,0.6,51.03);
 //cbFit1->SetParameters(68.89,0.34,0.6,51.03);

 // crystall ball root parameters
 cbFit1->SetParameter(0, 68.9); // mean value
 cbFit1->SetParName(0,"mean");
 cbFit1->SetParLimits(0,68.7,69.0);
 cbFit1->SetParameter(1, 0.32); // sigma
 cbFit1->SetParName(1,"sigma");
 //cbFit1->SetParLimits(1,0.2,0.4);
 cbFit1->SetParameter(2, 1.3); // power law tail parameter
 cbFit1->SetParName(2,"alpha");
 //cbFit1->SetParLimits(2,1.0,2.0);
 cbFit1->SetParameter(3, 52); // power of the power law component (exponent)
 cbFit1->SetParName(3,"n");
 //cbFit1->SetParLimits(3,0,70);
 // cbFit1->SetParameter(4, 269.0); // normalization (peak height)
 // cbFit1->SetParName(4,"normt");
 // cbFit1->SetParLimits(4,268.0,275.0);

 cbFit1->SetParameter(4, 2.65e-14); // normalization (peak height)
 cbFit1->SetParName(4,"normt");
 cbFit1->SetParLimits(4,2.6e-14,3.1e-14);

 
 // u_cbt->SetParameter(0, 265.0); // mean value
 // u_cbt->SetParameter(0, 68.94); // mean value
 // u_cbt->SetParameter(1, 0.34); // sigma
 // u_cbt->SetParameter(2, 0.46); // power law tail parameter
 // u_cbt->SetParameter(3,50);
 
 
 //u_cbt->SetLineColor(kBlue);
 //u_cbt->Draw("same");
 

// u_cbFit->SetParameter(5, 1);
 // u_cbFit->SetParameter(6, 1);

 // test draw
 // TF1 * u1 = new TF1("u1","myFunc(x)",64.0,70.0);
 //  TF1 *func = new TF1("fitf",fitf,-2,2,3);




// Crystal Ball function with four parameters (alpha, n, mu, sigma)
 //TF1 *cbFit = new TF1("cbFit", "crystalball(x, [p0], [p1], [p2], [p3], p[4])", 62.0, 72.0);
 // TF1 *cbFit = new TF1("cbFit", "f_crystal_ball", 64., 70.);
 //TF1 *cbFit = new TF1("fit","crystalball",62.0,72.0);
 //"crystalball" = "[Constant] * ROOT::Math::crystalball_function(x, [Alpha], [N], [Sigma], [Mean])"
// Set initial guesses for the parameters
 // cbFit->SetParameter(0, 2.0); // alpha (power law tail parameter)
 // cbFit->SetParameter(1, 1.0); // n (power law tail exponent)
 // cbFit->SetParameter(2, 68.9); // mu (peak position)
 // cbFit->SetParameter(3, 1.0); // sigma (Gaussian core width)
 // cbFit->SetParameter(4, 1.0); // don't know
 // cbFit->SetParameters(2.0, 68.9, 0.3, 2, 1.5);
 // cbFit->SetParameters(1.5,6.0e-14,68.1,1.5,1.0);
 //  cbFit->SetParameters(1.5,100,68.1,1.5,1.0);

 hist1->Fit("cbFit1","L","",64.0,70.0);
 cbFit1->Draw("same");
 
 
 //hist1->Fit("u_cbFit","L","",64.0,70.0);
 //u_cbFit->Draw("same");
  //  u_cbFit->Draw();
 // TLegend *legend = new TLegend(0.55,0.65,0.76,0.82);
 // legend->AddEntry(hist1,"Signal","lep");
 // legend->AddEntry(hist2,"DIF","lep");
 // legend->Draw();
 // //c1->SetLogy();


  // just fit don't draw
  
  
 c1->Modified();
 c1->Update();
 // gPad->BuildLegend();

}
