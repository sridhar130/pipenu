/// \file
/// \ingroup tutorial_fit
/// \notebook
/// Tutorial for normalized sum of two functions
/// Here: a background exponential and a crystalball function
/// Parameters can be set:
///  1.   with the TF1 object before adding the function (for 3) and 4))
///  2.  with the TF1NormSum object (first two are the coefficients, then the non constant parameters)
///  3. with the TF1 object after adding the function
///
/// Sum can be constructed by:
///  1. by a string containing the names of the functions and/or the coefficient in front
///  2. by a string containg formulas like expo, gaus...
///  3. by the list of functions and coefficients (which are 1 by default)
///  4. by a std::vector for functions and coefficients
///
/// \macro_image
/// \macro_output
/// \macro_code
///
/// \author Lorenzo Moneta

#include <TMath.h>
#include <TCanvas.h>
#include <TF1NormSum.h>
#include <TF1.h>
#include <TH1.h>
#include "TRandom3.h"

using namespace std;

void fitpipenu()
{
  
  TF1* total = new TF1("total","expo(0)+crystalball(2)",60.,72.0);
  // set parameters from normalized DIF ogram
  total->SetParameter(0,16.53);
  total->SetParameter(1,-0.18);

  // set parameters from normalized signal ogram
  total->SetParameter(2,268.3);
  //total->SetParLimits(2,200.,400.);

  total->SetParameter(3,68.94);
  total->SetParLimits(3,66.,70.);
  total->SetParameter(4,0.3);
  total->SetParameter(5,51.17);
  total->SetParameter(6,0.32);


  //hist2->Fit("total","SQ","",60.0,72.0);
  //hist2->Fit("total","SQ","","R");
  //total->Draw();
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
   TH1D * hres = new TH1D("hres","resolution",1000,68.4,69.4);
   TH1D * hpull = new TH1D("hpull","pull on the peak",200,-5.,5.);
   TH1D * hcons = new TH1D("hcons","constraint ",200,-2.5,2.5);
   // Fill each bin content of this histogram with the fucntion value here.
   int nbins = hSB0->GetNbinsX();
   int iter  = 1000;
   for (int j=0; j<=nbins; j++)
     {
       x_value = (*total)(60.0+0.2*j);
       hSB0->SetBinContent(j,x_value);
     }

   hSB0->Fit("total","L","",60.0,72.0);
   true_peak=total->GetParameter(3);
   true_sigma=total->GetParError(3);
   
   TH1D * huncer = new TH1D("huncer","histo_uncertainty",200,-1.5,1.5);
   TF1 *f = new TF1("f", "TMath::Poisson(x, 0.3)", 0, 0.6);
   //TF1 *f = new TF1("f", "TMath::Poisson(x, 5.0)", 0, 10.0);
   f->SetParameter(0, 0.3);
   TH1D * hSB = new TH1D("hSB","histoSB",60,60.,72.);
   for (int kk=0;kk<1000;kk++)
     {
   for (int jk=0; jk<=nbins; jk++)
     {
       gaus_sig= r->Gaus(0,1.0); 
       //       sigma = f->GetRandom();
       bin_error = sqrt(hSB0->GetBinContent(jk));
       bin_val   = hSB0->GetBinContent(jk)+(gaus_sig*bin_error);
       hSB->SetBinContent(jk,bin_val);

     }
    hSB->Fit("total","L","",60.0,72.0);
    //    hres->Fill(total->GetParameter(3),total->GetParError(3));
    hres->Fill(total->GetParameter(3));
    cout<<"par 3: "<< total->GetParameter(3)<<endl;
    hpull->Fill((true_peak-(total->GetParameter(3)))/(total->GetParError(3)));
    hcons->Fill(true_sigma-total->GetParError(3));
     }
   
   
   //cout<<"par 3: "<< total->GetParError(3)<<endl;
    


   TCanvas * c1 = new TCanvas("Simfit","S&B",
                              10,10,700,700);
   c1->Divide(1,2);
   c1->cd(1);
   gStyle->SetOptFit(1111);
   hSB->Draw();
   total->Draw("same");
   c1->cd(2);
   hres->Draw();

    TCanvas * c2 = new TCanvas("pull","pull",
                              10,10,700,700);

   c2->Divide(1,2);
   c2->cd(1);
   gStyle->SetOptFit(1111);
   hpull->Draw();
   c2->cd(2);
   hcons->Draw();
   

   //   cout<<"Total beans: "<<Totalbins<<endl;
   
   //hSB->FillRandom("total",9000);

  

   
   
  // for (unsigned short i=1; i<=100; ++i)
  //   {
  //     //cout<<"number: "<<f->GetRandom()<<endl;
  //     //                printf("Random number %i was generated\n", r.Poisson(4));
  // 		huncer->Fill(f->GetRandom());
  //   }
  //huncer->Draw();
  // f_sum->Draw();
  //f_exp->Draw();
  //f_cb->Draw();
   
  /*
  // III.:
  f_sum->SetParameters( fnorm_exp_cb->GetParameters().data() );
  f_sum->SetParName(1,"NBackground");
  f_sum->SetParName(0,"NSignal");
  for (int i = 2; i < f_sum->GetNpar(); ++i)
  f_sum->SetParName(i,fnorm_exp_cb->GetParName(i) );

  // GENERATE HISTOGRAM TO FIT ..............................................................
  TStopwatch w;
  w.Start();
  TH1D *h_sum = new TH1D("h_ExpCB", "Exponential Bkg + CrystalBall function", NBins, -5., 5.);
  for (int i=0; i<NEvents; i++)
  {
  h_sum -> Fill(f_sum -> GetRandom());
  }
  printf("Time to generate %d events:  ",NEvents);
  w.Print();
  //TH1F *h_orig = new TH1F(*h_sum);

  // need to scale histogram with width since we are fitting a density
  h_sum -> Sumw2();
  h_sum -> Scale(1., "width");

  //fit - use Minuit2 if available
  ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");
  new TCanvas("Fit","Fit",800,1000);
  // do a least-square fit of the spectrum
  auto result = h_sum -> Fit("fsum","SQ");
  result->Print();
  h_sum -> Draw();
  printf("Time to fit using ROOT TF1Normsum: ");
  w.Print();

  // test if parameters are fine
  std::vector<double>  pref = {nsig, nbkg, signal_mean};
  for (unsigned int i = 0; i< pref.size(); ++i)  {
  if (!TMath::AreEqualAbs(pref[i], f_sum->GetParameter(i), f_sum->GetParError(i)*10.) )
  Error("testFitNormSum","Difference found in fitted %s - difference is %g sigma",f_sum->GetParName(i), (f_sum->GetParameter(i)-pref[i])/f_sum->GetParError(i));
  }

  gStyle->SetOptStat(0);
  // add parameters
  auto t1 = new TLatex(-2.5, 300000, TString::Format("%s = %8.0f #pm %4.0f", "NSignal",f_sum->GetParameter(0), f_sum->GetParError(0) ) );
  auto t2 = new TLatex(-2.5, 270000, TString::Format("%s = %8.0f #pm %4.0f", "Nbackgr",f_sum->GetParameter(1), f_sum->GetParError(1) ) );
  t1->Draw();
  t2->Draw();
  */
}
