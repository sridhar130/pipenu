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

pipenu::Analysis* gPipenu; 

void signal_fit_5mm()
{
 gPipenu = new pipenu::Analysis("a");
 TCanvas *c1 = new TCanvas("c1"," ",900,750);
 double p0, p1, p2, p3, p4;
 p0=1.5;
 p1=68.9;
 p2=1.5;
 p3=1.5;
 p4=2.0;

 TH1F * hist1 = gh1("/exp/mu2e/data/projects/pipenu/hist/tripathy/pipenu.bpip5b0s51r0105.tripathy_pipenu_pipenu_ana.hist","PipenuAna","trk_178/p_2");
 hist1->Rebin(2);

 hist1->Scale(gPipenu->GetChannel("bpip5b0s51r0105")->NormSF());
 hist1->Scale(1e16);
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
 hist1->SetTitle("pipenu signal, crystal ball fit, 5mm ");
 hist1->Draw();

 //fitting------------
// Crystal Ball function with four parameters (alpha, n, mu, sigma)
 //TF1 *cbFit = new TF1("cbFit", "crystalball(x, [p0], [p1], [p2], [p3], p[4])", 62.0, 72.0);
 TF1 *cbFit = new TF1("cbFit", "[Constant] *ROOT::Math::crystalball_function(x, [Alpha], [N], [Sigma], [Mean])", 64., 70.);
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
  cbFit->SetParameters(1.5,100,68.1,1.5,1.0);
  cbFit->SetParLimits(2,68.4,68.9);
  cbFit->SetParLimits(4,0.29,0.31);
  hist1->Fit("cbFit","SQ","",64.0,70.0);
  cbFit->Draw("same");
 // TLegend *legend = new TLegend(0.55,0.65,0.76,0.82);
 // legend->AddEntry(hist1,"Signal","lep");
 // legend->AddEntry(hist2,"DIF","lep");
 // legend->Draw();
 // //c1->SetLogy();
 // c1->Modified();
 // c1->Update();
 // gPad->BuildLegend();

}
