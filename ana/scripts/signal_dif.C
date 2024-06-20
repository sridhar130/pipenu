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

void signal_dif()
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
 hist2->Scale( gPipenu->GetChannel("bmup4b0s66r0100")->NormSF());
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
 hist2->GetXaxis()->SetRangeUser(61.,72.);
 //hist2->GetYaxis()->SetRangeUser(0.0,1e-13);
 hist2->SetTitle("pipenu signal, crystal ball fit ");
 hist2->Draw();
 //int bins = hist2->GetXaxis()->GetNBins();
 int bins = hist2->GetNbinsX();
 cout<<" nbins: "<<bins<<endl;

 //fitting------------


 //TF1 * fB = new TF1("fB","exp((x* [p1])+[p0])",60.,72.0);
 // TF1 * fB = new TF1("fB","expo",60.,72.0);
 //F1 *cbFit = new TF1("cbFit", "[Constant] *ROOT::Math::crystalball_function(x, [Alpha], [N], [Sigma], [Mean])", 64., 70.);
 //TF1 *cbFit = new TF1("cbFit","crystalball",64.0,70.0);
 //fB->SetParameters(16.53,-0.18);
 // cbFit->SetParameters(0.468,268.3,68.48,51.17,0.316);

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


 hist2->Fit("total","L","",61.0,72.0);
 //hist2->Fit("total","SQ","","R");
total->Draw("same");

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
