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
  f = TMath::Exp(P[0]*X[0]+P[1]);
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


// non-root implementation
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

double combo_fit(double* X, double*P)
{ return sing_expo(X,P) + crystalball_root(X,&P[3]);
}

double combo_fit_quad(double* X, double*P)
{ return quad_expo(X,P) + crystalball_root(X,&P[3]);
}

double combo_fit_double(double* X, double*P)
{ return double_expo(X,P) + crystalball_root(X,&P[3]);
}


void total_fit3()
{

  //Define the combined fit--------------------
  
  TF1* cbFit2 = new TF1("cbFit2","crystalball_root",64.0,70.0,5);
  TF1* cbFit1 = new TF1("cbFit1","crystalball_root",64.0,70.0,5);
  TF1* total  = new TF1("total","combo_fit",60.,72.0,8);
  TF1* sDB    = new TF1("sDB","sing_expo",60.0,72.0,2);
   //TF1* total = new TF1("total","combo_fit_quad",60.,72.0,8;)
   //TF1* total = new TF1("total","combo_fit_double",60.,72.0,8;)

  //-----------------------------------------------------
  // background sing_expo fit

 sDB->SetParameters(-0.1817,-20.31);
  

  
  //------------------------------------------------------
   // crystall ball root parameters
 cbFit1->SetParameter(0, 68.94); // mean value 68.9
 cbFit1->SetParName(0,"mean");
 //cbFit1->SetParLimits(0,68.7,69.0);
 cbFit1->SetParameter(1, 0.3089); // sigma 0.32
 cbFit1->SetParName(1,"sigma");
 //cbFit1->SetParLimits(1,0.2,0.4);
 cbFit1->SetParameter(2, 0.4613); // power law tail parameter 0.47 0.3902
 cbFit1->SetParName(2,"alpha");
 //cbFit1->SetParLimits(2,1.0,2.0);
 cbFit1->SetParameter(3,51.15); // power of the power law component (exponent) 54.02 521.1
 cbFit1->SetParName(3,"n");
 cbFit1->SetParLimits(3,0,70);
 cbFit1->SetParameter(4, 268.1); // normalization (peak height) 265.1
 cbFit1->SetParName(4,"normt");
 // cbFit1->SetParLimits(4,268.0,275.0);
 // cbFit1->SetParLimits(4,268.0,275.0);
 
 //cbFit1->SetParLimits(4,2.6e-14,3.1e-14);


 //------------------------------------------------------
 // crystal ball non-root parameters
 cbFit2->SetParameter(0, 68.94); // mean value 68.9
 cbFit2->SetParName(0,"mean");
 //cbFit1->SetParLimits(0,68.7,69.0);
 cbFit2->SetParameter(1, 0.3157); // sigma 0.32
 cbFit2->SetParName(1,"sigma");
 //cbFit1->SetParLimits(1,0.2,0.4);
 cbFit2->SetParameter(2, 0.4683); // power law tail parameter 0.47 0.3902
 cbFit2->SetParName(2,"alpha");
 //cbFit1->SetParLimits(2,1.0,2.0);
 cbFit2->SetParameter(3, 51.1516); // power of the power law component (exponent) 54.02 521.1
 cbFit2->SetParName(3,"n");
 cbFit2->SetParLimits(3,0,70);
 cbFit2->SetParameter(4, 2.683e-14); // normalization (peak height) 265.1
 cbFit2->SetParName(4,"normt");

  //hist2->Fit("total","SQ","",60.0,72.0);
  //hist2->Fit("total","SQ","","R");

 
 
   // total2->Draw("same");

    
   double tt = (*cbFit2)(67.5);
   double x_value;
   double sigma;
   double gaus_sig;
   double bin_error;
   double true_peak;
   double fit_peak;
   double true_sigma;
   double bin_val;
   TRandom3 * r = new TRandom3();
   TRandom3 * r1 = new TRandom3();
   cout << "total value: "<<tt<<endl;
   TH1D * hSB0 = new TH1D("hSB0","histoSB",60,60.,72.);

   // Fill each bin content of this histogram with the fucntion value here.
   int nbins  = hSB0->GetNbinsX();
   cout<<" NBINS--" <<nbins<<endl;
   int iter   = 100;
   double inc = (72.0-60.0)/nbins;
   for (int j=0; j<=nbins; j++)
     {
       x_value = (*sDB)(60.0+inc*j);
       x_value = x_value*1e16;
       hSB0->SetBinContent(j,x_value);
     }
     
   TH1D * hSB1= new TH1D("hSB1","histoSB1",60,60.,72.);
   TH1D * hSB2 = new TH1D("hSB2","histoSB2",60,60.,72.);
   TF1 *f = new TF1("f", "TMath::Poisson(x, 0.3)", 0, 0.6);
   double g_ijk;
   // for (int lk=0; lk<=nbins; lk++)
   //   {
   //     //       x_value      = (*cbFit2)(60.0+inc*lk);
   //     x_value      = hSB0->GetBinContent(lk);
   //     double x_v   = x_value;
   //     //x_value      = x_value*1e16;
   //     gaus_sig     = r->Gaus(0,1.0);
   //     bin_error    = sqrt(x_value);
   //     double bin_v = x_value+(bin_error*gaus_sig);
   //     if (bin_v<0) bin_v=0;
   //     hSB1->SetBinContent(lk,bin_v);
   //     // for (int ijk=0;ijk<100;ijk++)
   //     //   { g_ijk = r->Gaus(0,0.1);
   //     //   }
   //     // double bin_v100 =  x_v+(bin_error*g_ijk);
   //     // hSB2->SetBinContent(lk,bin_v100);
   //   }
   //hSB->Draw();
   //
   
 for (int kk=0;kk<iter;kk++)
     {
      TH1D * hSB = new TH1D("hSB","histoSB11",60,60.,72.);
   for (int jk=0; jk<=nbins; jk++)
     {
       
       //       sigma = f->GetRandom();
       double bin_con = hSB0->GetBinContent(jk);
       bin_error = sqrt(hSB0->GetBinContent(jk));
       gaus_sig  = r1->Gaus(0,1.0);
       //gaus_sig= r->Gaus(0,bin_error);
       //bin_val   = hSB0->GetBinContent(jk)+gaus_sig;
       bin_val   = hSB0->GetBinContent(jk)+(gaus_sig*bin_error);
       //bin_val  = bin_con;
       if (bin_error < bin_val)
         {
           bin_val   = hSB0->GetBinContent(jk)+(gaus_sig*bin_error);
         }
       else
         {
           bin_val = bin_con;
         }
       if (bin_val<0) bin_val=0;
       hSB->SetBinContent(jk,bin_val);

       // cout <<"bin con: "<<bin_con<<" bin-error: "<<bin_error<<" bin_val: "<<bin_val<<" gaus_sig "<<gaus_sig<<endl;
     }
     
    hSB->Fit("sDB","I","",60.0,72.0);

     }
   
   

   TCanvas * c2 = new TCanvas("Simfit","S&B",
                              10,10,700,900);
   c2->Divide(1,2);
   c2->cd(1);
   gStyle->SetOptFit(1111);
   //   hSB0->Draw();
   //total->Draw("same");
   hSB0->Fit("sDB","I","",60.0,72.0);
   c2->cd(2);
   //   hres->Draw();
   //hSB->Fit("cbFit1","I","",60.0,72.0);
   //hSB1->Fit("sDB","I","",60.0,72.0);

   TCanvas * c3 = new TCanvas("1000","B",
                              10,10,700,500);
   gStyle->SetOptFit(1111);
   
   sDB->Draw();

   //hSB1k->Draw();
   //   total->Draw("same");
   /* 
   TH1D * huncer = new TH1D("huncer","histo_uncertainty",200,-1.5,1.5);
  
   TH1D * hSB;// = new TH1D("hSB","histoSB1",60,60.,72.);
   TH1D * hSB1k  = new TH1D("hSB1k","histoSB1k",60,60.,72.);


   for (int kk=0;kk<iter;kk++)
     {
       hSB = new TH1D("hSB","histoSB1",60,60.,72.);
   for (int jk=0; jk<=nbins; jk++)
     {
       
       //       sigma = f->GetRandom();
       double bin_con = hSB0->GetBinContent(jk);
       bin_error = sqrt(hSB0->GetBinContent(jk));
       gaus_sig= r->Gaus(0,1.0);
       //gaus_sig= r->Gaus(0,bin_error);
       //bin_val   = hSB0->GetBinContent(jk)+gaus_sig;
       bin_val   = hSB0->GetBinContent(jk)+(gaus_sig*bin_error);
       hSB->SetBinContent(jk,bin_val);

       // cout <<"bin con: "<<bin_con<<" bin-error: "<<bin_error<<" bin_val: "<<bin_val<<endl;

     }
    hSB->Fit("cbFit1","L","",60.0,72.0);
    //    hres->Fill(total->GetParameter(3),total->GetParError(3));
    hres->Fill(cbFit1->GetParameter(3));
    cout<<"par 3: "<< total->GetParameter(3)<<endl;
    // hpull->Fill((true_peak-(total->GetParameter(3)))/(total->GetParError(3)));
    //hcons->Fill(true_sigma-total->GetParError(3));
    if (kk==iter-5)
    {hSB1k = (TH1D*)hSB->Clone();
    }
    
     }
   hSB1k->Fit("total","L","",60.0,72.0);

   
  

 
   */
}
 
