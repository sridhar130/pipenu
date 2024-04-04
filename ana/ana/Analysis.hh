#ifndef __pipenu_ana_Analysis_hh__
#define __pipenu_ana_Analysis_hh__

#include "TNamed.h"
#include "TObjArray.h"

namespace pipenu {

  class AnalysisChannel : public TNamed {
  public:
    float    fNPOT;
    float    fNStops;
    float    fNSimSignal;
    float    fNormSF;

    AnalysisChannel(const char* Name, const char* Title);
    
//-----------------------------------------------------------------------------
// for different channels, the calculation of fNormSH may be different
//-----------------------------------------------------------------------------
    float   NormSF() { return fNormSF; }
  };


  
  class Analysis: public TNamed {
  public:
    
    TObjArray* fListOfAnalysisChannels;

    double     fBR_pienu;               // BR(pi --> e nu)

    Analysis(const char* Name);
    ~Analysis();

    AnalysisChannel* GetChannel(const char* Name) {
      return (AnalysisChannel*) fListOfAnalysisChannels->FindObject(Name);
    }

    void AddChannel(TObject* Channel) { fListOfAnalysisChannels->Add(Channel); }
    
  };
}

#endif
