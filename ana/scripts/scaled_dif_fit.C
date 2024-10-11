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

#include "TH1F.h"
#include "TF1.h"
R__LOAD_LIBRARY($/exp/mu2e/app/users/tripathy/Development_pipenu4/build/al9-prof-e28-p057/pipenu/lib/libpipenu_ana.so)
pipenu::Analysis* gPipenu;

double double_expo(double* X, double* P) {
  double a,b,c, f;
  // a=P[0];
  // b=P[1];
  // c=P[2];
  double expoo = TMath::Power(P[1],X[0]+P[2]);
  f = TMath::Power(P[0],expoo);
  return f;
}

double quad_expo(double* X, double* P) {
  double f;
  f = TMath::Exp(P[0]*X[0]*X[0]+P[1]*X[0]+P[2]);
  return f;
}

double sing_expo(double* X, double* P) {
  double f;
  f = P[0]*TMath::Exp(P[1]*X[0]+P[2]);
  return f;
}

void scaled_dif_fit()
{
 gPipenu = new pipenu::Analysis("a");
 TCanvas *c1 = new TCanvas("c1"," ",900,750);
 gStyle->SetOptFit(1111);
 TH1F * hist2 = gh1("/exp/mu2e/data/projects/pipenu/hist/tripathy/pipenu.bmup4b0s66r0105.tripathy_pipenu_pipenu_ana.hist","PipenuAna","trk_126/p_2");
 hist2->Rebin(2);
 hist2->Scale( gPipenu->GetChannel("bmup4b0s66r0100")->NormSF());
 hist2->Scale(1e16);
 hist2->SetLineColor(kGreen+3);
 hist2->SetLineStyle(1);
 hist2->SetLineWidth(2);
 hist2->SetMarkerStyle(20);
 hist2->SetMarkerSize(1.2);
 hist2->SetName("DIF");
 hist2->SetMarkerColorAlpha(kGreen+3, 0.75);

 //common-------------
 hist2->GetXaxis()->SetTitle("p (MeV/c)");
 hist2->GetYaxis()->SetTitle("N/ 0.2 MeV/c");
 hist2->GetYaxis()->SetTitleOffset(1.25);
 hist2->GetXaxis()->SetRangeUser(60.,72.);
 //hist1->GetYaxis()->SetRangeUser(1e-14,1e-12);
 hist2->SetTitle("mu+ DIF, pol2 fit ");
 hist2->Draw();

// fitting----------

 TF1 * fB  = new TF1("fB","[p0]*exp((x* [p1])+[p2])",58,74);
 TF1 * dB  = new TF1("dB","[p0]*exp((x* [p1])+[p2])+[p3]*exp((x* [p4])+[p5])",58,74);
 TF1 * ddB = new TF1("ddB","double_expo",60.,72.,3);
 TF1 * qdB = new TF1("qdB","quad_expo",60.,72.,3);
 TF1 * sdB = new TF1("sdB","sing_expo",60.,72.,3);
 //fB->SetParameters(-0.2,-20);

 // parameters-----------
 fB->SetParameters(1.0,-0.1817,-20.31);
 sdB->SetParameters(0.85,-0.1617,16.31);
 dB->SetParameters(7.67e-15,-0.18,11.55,7.43e-15,-0.18,11.46);
 qdB->SetParameters(-0.0078,0.835,-53.18);

 // ddB parameters----------
 // ddB->SetParameter(0,7.67e-15);
 // ddB->SetParameter(1,2.0);
 // ddB->SetParameter(2,0.2);
 //ddB->Draw() ;
 hist2->Fit("sdB","I","",60.0,72.0);

 
 // hist2->Fit("dB","L","",60.0,72.0);

 // TH1F *h_fit = new TH1F("h_fit", "Fitted Histogram", 7500, 60.,72.0);

// // Fill the fitted histogram with the values of the fit function
//   for (int i = 0; i < 7500; i++) {
//     h_fit->Fill((fB->Eval(i))*1e-16);
//     cout << "values: "<<fB->Eval(i)<<endl;
//   }
// h_fit->Draw();
 TLegend *legend = new TLegend(0.55,0.65,0.76,0.82);
 // legend->AddEntry(hist1,"Signal","lep");
 //legend->AddEntry(hist2,"DIF","lep");
 //legend->AddEntry((TObject*)0, "f(x)=[p0]*exp((x* [p1])+[p2])", "");
 //legend->AddEntry((TObject*)0, "pol2", "");
 //legend->AddEntry((TObject*)0, "exp(P[0]*X[0]*X[0]+P[1]*X[0]+P[2])", "");
 legend->AddEntry((TObject*)0, "[p0]*exp((x* [p1])+[p2])+[p3]*exp((x* [p4])+[p5]", "");
 //legend->Draw();
 // //c1->SetLogy();

 
 c1->Modified();
 c1->Update();
 //gPad->BuildLegend();

}
