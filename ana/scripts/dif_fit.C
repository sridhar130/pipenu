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

pipenu::Analysis* gPipenu; 

void dif_fit()
{
 gPipenu = new pipenu::Analysis("a");
 TCanvas *c1 = new TCanvas("c1"," ",900,750);

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
 hist2->SetTitle("mu+ DIF, exponential fit ");
 hist2->Draw();

 // fitting----------

 TF1 * fB = new TF1("fB","exp((x* [p0])+[p1])",30,100);
 //fB->SetParameters(-0.2,-20);
 hist2->Fit("fB","I","",60.0,72.0);

 TH1F *h_fit = new TH1F("h_fit", "Fitted Histogram", 7500, 60.,72.0);

// // Fill the fitted histogram with the values of the fit function
//   for (int i = 0; i < 7500; i++) {
//     h_fit->Fill((fB->Eval(i))*1e-16);
//     cout << "values: "<<fB->Eval(i)<<endl;
//   }
// h_fit->Draw();
 // TLegend *legend = new TLegend(0.55,0.65,0.76,0.82);
 // legend->AddEntry(hist1,"Signal","lep");
 // legend->AddEntry(hist2,"DIF","lep");
 // legend->Draw();
 // //c1->SetLogy();
 // c1->Modified();
 // c1->Update();
 // gPad->BuildLegend();

}
