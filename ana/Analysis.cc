

#include "pipenu/ana/Analysis.hh"


namespace pipenu {

  AnalysisChannel::AnalysisChannel(const char* Name, const char* Title) :
    TNamed(Name,Title)
    , fNPOT(0)
    , fNStops(0)
    , fNSimSignal(0)
    , fNormSF(0)
  {
  }

  Analysis::Analysis(const char* Name) : TNamed(Name,Name) {
    fListOfAnalysisChannels = new TObjArray();
    fListOfAnalysisChannels->SetOwner(1);
    fBR_pienu = 1.23e-4;

//-----------------------------------------------------------------------------
// pi+ ST no degrader
//-----------------------------------------------------------------------------
    AnalysisChannel* ch;

    ch = new AnalysisChannel("bpip0b0s51r0100","bpip0b0s51r0100");
    ch->fNPOT       = 2.5e8  ;
    ch->fNStops     = 312616.;
    ch->fNSimSignal = 100000.;
    ch->fNormSF     = (ch->fNStops/ch->fNPOT)*fBR_pienu*(1/ch->fNSimSignal);
    AddChannel(ch);
//-----------------------------------------------------------------------------
// pi+ , 2 mm degrader
//-----------------------------------------------------------------------------
    ch = new AnalysisChannel("bpip2b0s51r0100","bpip2b0s51r0100");
    ch->fNPOT       = 2.5e8  ;
    ch->fNStops     = 84785. ;
    ch->fNSimSignal = 100000.;
    ch->fNormSF     = (ch->fNStops/ch->fNPOT)*fBR_pienu*(1/ch->fNSimSignal);
    AddChannel(ch);

    ch  = new AnalysisChannel("bpip2b0s54r0100","bpip2b0s54r0100");
    ch->fNPOT       = 2.5e8  ;
    ch->fNStops     = 448131.;
    ch->fNSimSignal = 100000.;
    ch->fNormSF     = (ch->fNStops/ch->fNPOT)*fBR_pienu*(1/ch->fNSimSignal);
    AddChannel(ch);
//-----------------------------------------------------------------------------
// pi+ , 3 mm degrader
//-----------------------------------------------------------------------------
    ch = new AnalysisChannel("bpip3b0s51r0100","bpip3b0s51r0100");
    ch->fNPOT       = 2.5e8  ;
    ch->fNStops     = 50340. ;
    ch->fNSimSignal = 100000.;
    ch->fNormSF     = (ch->fNStops/ch->fNPOT)*fBR_pienu*(1/ch->fNSimSignal);
    AddChannel(ch);

    ch  = new AnalysisChannel("bpip3b0s54r0100","bpip3b0s54r0100");
    ch->fNPOT       = 2.5e8  ;
    ch->fNStops     = 532767.;
    ch->fNSimSignal = 100000.;
    ch->fNormSF     = (ch->fNStops/ch->fNPOT)*fBR_pienu*(1/ch->fNSimSignal);
    AddChannel(ch);
//-----------------------------------------------------------------------------
// pi+ , 4 mm degrader
//-----------------------------------------------------------------------------
    ch = new AnalysisChannel("bpip4b0s51r0100","bpip4b0s51r0100");
    ch->fNPOT       = 2.5e8  ;
    ch->fNStops     = 31681. ;
    ch->fNSimSignal = 100000.;
    ch->fNormSF     = (ch->fNStops/ch->fNPOT)*fBR_pienu*(1/ch->fNSimSignal);
    AddChannel(ch);

    ch  = new AnalysisChannel("bpip4b0s54r0100","bpip4b0s54r0100");
    ch->fNPOT       = 2.5e8  ;
    ch->fNStops     = 583855.;
    ch->fNSimSignal = 100000.;
    ch->fNormSF     = (ch->fNStops/ch->fNPOT)*fBR_pienu*(1/ch->fNSimSignal);
    AddChannel(ch);
//-----------------------------------------------------------------------------
// pi+ , 5 mm degrader
//-----------------------------------------------------------------------------
    ch = new AnalysisChannel("bpip5b0s51r0100","bpip5b0s51r0100");
    ch->fNPOT       = 2.5e8  ;
    ch->fNStops     = 17225. ;
    ch->fNSimSignal = 100000.;
    ch->fNormSF     = (ch->fNStops/ch->fNPOT)*fBR_pienu*(1/ch->fNSimSignal);
    AddChannel(ch);

    ch  = new AnalysisChannel("bpip5b0s54r0100","bpip5b0s54r0100");
    ch->fNPOT       = 2.5e8  ;
    ch->fNStops     = 617324.;
    ch->fNSimSignal = 100000.;
    ch->fNormSF     = (ch->fNStops/ch->fNPOT)*fBR_pienu*(1/ch->fNSimSignal);
    AddChannel(ch);
 
  }

  Analysis::~Analysis() {
    delete fListOfAnalysisChannels;
  }
}
