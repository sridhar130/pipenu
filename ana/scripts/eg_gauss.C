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


void eg_gauss()
{

  //Define the combined fit--------------------
  
   TRandom3 * r = new TRandom3();
  
   double x_value;
   double sigma;
   double gaus_sig;
   double bin_error;
   double true_peak;
   double fit_peak;
   double true_sigma;
   double bin_val;

   TH1D * g0 = new TH1D("g0","gaus01",60,-3.0,3.0);
  
   for (int kk=0;kk<1000;kk++)
     {
       gaus_sig = r->Gaus(0.0,0.3);
       g0->Fill(gaus_sig);
     }
   g0->Draw();

  
}
