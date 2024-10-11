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

double combo_fit(double* X, double*P)
{ return sing_expo(X,P) + crystalball_root(X,&P[2]);
}

double combo_fit_quad(double* X, double*P)
{ return quad_expo(X,P) + crystalball_root(X,&P[2]);
}

double combo_fit_double(double* X, double*P)
{ return double_expo(X,P) + crystalball_root(X,&P[2]);
}


void total_fitf()
{

  //Define the combined fit--------------------
  

  //TF1* total = new TF1("total","sing_expo(x,[0],[1],[2])+crystalball_root(x,[3],[4],[5],[6],[7])",60.,72.0);
  //TF1* total1 = new TF1("total1","sing_expo",60.,72.0,3);
  //TF1* total2 = new TF1("total2","crystalball_root",64.0,70.0,5);
   TF1* total = new TF1("total","combo_fit",60.,72.0,7);
   //TF1* total = new TF1("total","combo_fit_quad",60.,72.0,8;)
   //TF1* total = new TF1("total","combo_fit_double",60.,72.0,8;)

   

  //Parameters for the single expo in combined fit--------------------
   //  total->SetParameter(0,0.9986);
  //  total->SetParLimits(0,0.5,1.5);
  total->SetParameter(0,-0.1817);
  total->SetParName(0,"slope");
  total->SetParameter(1,16.51);
  total->SetParName(1,"coef");
 
  //Parameters for the cb in combined fit--------------------
  total->SetParameter(2,69.785);
  total->SetParName(2,"mean");
  total->SetParameter(3,0.3157);
  total->SetParName(3,"sigma");
  total->SetParameter(4,0.4613);
  total->SetParName(4,"alpha");
  total->SetParameter(5,51.15);
  total->SetParName(5,"n");
  total->SetParLimits(5,0,70);
  total->SetParameter(6,268.1);
  total->SetParName(6,"normt");
  //total->SetParLimits(6,260.0,310.0);
  
 TCanvas * c3 = new TCanvas("1000","B",
                              10,10,700,700);
    gStyle->SetOptFit(1111);
    //hSB1k->Draw();
    total->Draw();

    
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
   TH1D * hSB0 = new TH1D("hSB0","histoSB",60,60.,72.);
   TH1D * hres = new TH1D("hres","resolution",100,68.4,69.4);
   TH1D * hpull = new TH1D("hpull","pull on the peak",200,-5.,5.);
   TH1D * hcons = new TH1D("hcons","constraint ",200,-2.5,2.5);
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

   hSB0->Fit("total","L","",60.0,72.0);
   true_peak=total->GetParameter(3);
   true_sigma=total->GetParError(3);
   //hSB0->Draw();
   TH1D * huncer = new TH1D("huncer","histo_uncertainty",200,-1.5,1.5);
   TF1 *f = new TF1("f", "TMath::Poisson(x, 0.3)", 0, 0.6);
   //TF1 *f = new TF1("f", "TMath::Poisson(x, 5.0)", 0, 10.0);
   f->SetParameter(0, 0.3);
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
       //hSB->SetBinContent(jk,bin_val);
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
       // cout <<"bin con: "<<bin_con<<" bin-error: "<<bin_error<<" bin_val: "<<bin_val<<endl;

     }
       hSB->Fit("total","L","",60.0,72.0);
    //    hres->Fill(total->GetParameter(3),total->GetParError(3));
    hres->Fill(total->GetParameter(2));
    cout<<"mean: "<< total->GetParameter(2)<<endl;
    hpull->Fill((true_peak-(total->GetParameter(3)))/(total->GetParError(3)));
    hcons->Fill(true_sigma-total->GetParError(3));
   //  if (kk==iter-5)
   //  {hSB1k = (TH1D*)hSB->Clone();
   //  }
    
     } //iter loop ends
   // hSB1k->Fit("total","L","",60.0,72.0);

   
   TCanvas * c2 = new TCanvas("Simfit","S&B",
                              10,10,700,900);
   c2->Divide(1,2);
   c2->cd(1);
   gStyle->SetOptFit(1111);
   hSB0->Draw();
   total->Draw("same");
   c2->cd(2);
   hres->Draw();

   TCanvas * c4 = new TCanvas("pull","pull",
                              10,10,700,900);
   c4->Divide(1,2);
   c4->cd(1);
   gStyle->SetOptFit(1111);
   hpull->Draw();
   c4->cd(2);
   hcons->Draw();
  
  
   
}
 
