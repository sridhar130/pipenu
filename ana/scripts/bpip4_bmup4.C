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

void bpip4_bmup4()
{
 gPipenu = new pipenu::Analysis("a");
 TCanvas *c1 = new TCanvas("c1"," ",900,750);

 TH1F * hist1 = gh1("/exp/mu2e/data/projects/pipenu/hist/tripathy/pipenu.bpip4b0s51r0105.tripathy_pipenu_pipenu_ana.hist","PipenuAna","trk_178/p_2");
 hist1->Rebin(2);

 hist1->Scale(gPipenu->GetChannel("bpip4b0s51r0105")->NormSF());
 hist1->SetLineColor(kRed);
 hist1->SetLineStyle(1);
 hist1->SetLineWidth(2);
 hist1->SetMarkerStyle(20);
 hist1->SetMarkerSize(1.2);
 hist1->SetName("Signal");
 hist1->SetMarkerColorAlpha(kRed, 0.75);

 TH1F * hist2 = gh1("/exp/mu2e/data/projects/pipenu/hist/tripathy/pipenu.bmup4b0s66r0105.tripathy_pipenu_pipenu_ana.hist","PipenuAna","trk_128/p_2");
 hist2->Rebin(2);
 // hist1->GetXaxis()->SetTitle("p (MeV/c)");
 hist2->Scale( gPipenu->GetChannel("bmup4b0s66r0100")->NormSF());
 hist2->SetLineColor(kGreen+3);
 hist2->SetLineStyle(1);
 hist2->SetLineWidth(2);
 hist2->SetMarkerStyle(20);
 hist2->SetMarkerSize(1.2);
 hist2->SetName("DIF");
 hist2->SetMarkerColorAlpha(kGreen+3, 0.75);

 //common-------------
 hist1->GetXaxis()->SetTitle("p (MeV/c)");
 hist1->GetYaxis()->SetTitle("N/  0.2 MeV/c");
 hist1->GetYaxis()->SetTitleOffset(1.25);
 hist1->GetXaxis()->SetRangeUser(40.,90.);
 hist1->GetYaxis()->SetRangeUser(0.0,5e-14);
 hist1->SetTitle("4 mm Ti deg, ");

 hist1->Draw();
 hist2->Draw("sames");

 TLegend *legend = new TLegend(0.55,0.65,0.76,0.82);
 legend->AddEntry(hist1,"Signal","lep");
 legend->AddEntry(hist2,"DIF","lep");
 legend->Draw();
 //c1->SetLogy();
 c1->Modified();
 c1->Update();
 // gPad->BuildLegend();

}
