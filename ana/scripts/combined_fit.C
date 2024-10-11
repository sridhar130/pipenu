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
// Define single exponential------------------
double sing_expo(double* X, double* P) {
  double f;
  f = P[0]*TMath::Exp(P[1]*X[0]+P[2]);
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

void combined_fit()
{
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
 hist1->Scale(1e16);
 TH1F * hist2 = gh1("/exp/mu2e/data/projects/pipenu/hist/tripathy/pipenu.bmup4b0s66r0105.tripathy_pipenu_pipenu_ana.hist","PipenuAna","trk_126/p_2");
 hist2->Rebin(2);
 hist2->Scale(gPipenu->GetChannel("bmup4b0s66r0100")->NormSF());
 hist2->Scale(1e16);
 hist2->Add(hist1);


 hist2->SetLineColor(kRed-3);
 hist2->SetLineStyle(1);
 hist2->SetLineWidth(2);
 hist2->SetMarkerStyle(20);
 hist2->SetMarkerSize(1.2);
 hist2->SetName("Signal");
 hist2->SetMarkerColorAlpha(kRed, 0.75);

 //common-------------
 hist2->GetXaxis()->SetTitle("p (MeV/c)");
 hist2->GetYaxis()->SetTitle("N/ 0.2 MeV/c");
 hist2->GetYaxis()->SetTitleOffset(1.25);
 hist2->GetXaxis()->SetRangeUser(60.,72.);
 //hist2->GetYaxis()->SetRangeUser(0.0,1e-13);
 hist2->SetTitle("pipenu signal, crystal ball fit ");
 hist2->Draw();
 //int bins = hist2->GetXaxis()->GetNBins();
 int bins = hist2->GetNbinsX();
 cout<<" nbins: "<<bins<<endl;
float bin_val = hist2->GetBinContent(5);
     cout<<"----bin_val---"<<bin_val<<endl;
 //fitting------------


 // central histogram---------
 TH1D * hSB0 = new TH1D("hSB0","histoSB1",60,60.,72.);

 int nbins  = hSB0->GetNbinsX();
 cout<<" NBINS--" <<nbins<<endl;
 int iter   = 1000;
 double inc = (72.0-60.0)/nbins;
 for (int j=0; j<=nbins; j++)
   {
     //x_value = (*total)(60.0+inc*j);
     
     hSB0->SetBinContent(j,bin_val);
   }


 //TF1 * fB = new TF1("fB","exp((x* [p1])+[p0])",60.,72.0);
 // TF1 * fB = new TF1("fB","expo",60.,72.0);
 //F1 *cbFit = new TF1("cbFit", "[Constant] *ROOT::Math::crystalball_function(x, [Alpha], [N], [Sigma], [Mean])", 64., 70.);
 //TF1 *cbFit = new TF1("cbFit","crystalball",64.0,70.0);
 //fB->SetParameters(16.53,-0.18);
 // cbFit->SetParameters(0.468,268.3,68.48,51.17,0.316);

 //--default fit----------------------------------------------
 TF1* total = new TF1("total","expo(0)+crystalball(2)",61.,72.0);

 total->SetParameter(0,16.53);
 total->SetParLimits(0,12.,18.);
 total->SetParameter(1,-0.18);
 // total->SetParLimits(1,-0.1,-0.3);
 //total->SetParLimits(2,200.,400.);

 total->SetParameter(2,268.3);
 total->SetParLimits(2,268.,270.);

 total->SetParameter(3,68.9);
 total->SetParLimits(3,68.8,70.);
 total->SetParameter(4,0.3);
 total->SetParLimits(4,0.28,0.5);

 total->SetParameter(5,51.7);
 total->SetParLimits(5,40.,55.0);
 total->SetParameter(6,0.3);
 total->SetParLimits(6,0.4,0.5);
 //---------------------------------------------

 TF1* total1 = new TF1("total1","combo_fit",60.,72.0,8);
   
   //  total->SetParameter(0,0.9986);
 total1->SetParameter(0,1.0);
 // total1->SetParLimits(0,0.5,1.5);
 total1->SetParameter(1,-0.1817);
 total1->SetParameter(2,-20.31);

 total1->SetParameter(7,2.65e-14);
  //total->SetParLimits(2,200.,400.);
 total1->SetParameter(3,68.93);
 total1->SetParLimits(3,66.,70.);
 total1->SetParameter(4,0.3232);
 total1->SetParameter(5,0.46);
  //  total->SetParameter(5,0.47);
  //  total->SetParameter(6,53.);
 total1->SetParameter(6,51.82);
 total1->SetParLimits(6,45.,60.0);

 //---------------------------------------------
 
 //hSB0->Fit("total1","L","",60.0,72.0);
 //hist2->Fit("total","SQ","","R");
 //total1->Draw("same");
 //hSB0->Draw();


 
// Crystal Ball function with four parameters (alpha, n, mu, sigma)
 //TF1 *cbFit = new TF1("cbFit", "crystalball(x, [p0], [p1], [p2], [p3], p[4])", 62.0, 72.0);
// TF1 *cbFit = new TF1("cbFit", "[Constant] *ROOT::Math::crystalball_function(x, [Alpha], [N], [Sigma], [Mean])", 64., 70.);
 //TF1 *cbFit = new TF1("fit","crystalball",62.0,72.0);
 //"crystalball" = "[Constant] * ROOT::Math::crystalball_function(x, [Alpha], [N], [Sigma], [Mean])"
// Set initial guesses for the parameters
 // cbFit->SetParameter(0, 2.0); // alpha (power law tail parameter)
 // cbFit->SetParameter(1, 1.0); // n (power law tail exponent)
 // cbFit->SetParameter(2, 68.9); // mu (peak position)
 // cbFit->SetParameter(3, 1.0); // sigma (Gaussian core width)
 // cbFit->SetParameter(4, 1.0); // don't know
 // cbFit->SetParameters(2.0, 68.9, 0.3, 2, 1.5);
 //cbFit->SetParameters(1.5,6.0e-14,68.1,1.5,1.0);
 //hist1->Fit("cbFit","I","",64.0,70.0);
 // cbFit->Draw("same");
 // TLegend *legend = new TLegend(0.55,0.65,0.76,0.82);
 // legend->AddEntry(hist1,"Signal","lep");
 // legend->AddEntry(hist2,"DIF","lep");
 // legend->Draw();
 // //c1->SetLogy();
 // c1->Modified();
 // c1->Update();
 // gPad->BuildLegend();

}
