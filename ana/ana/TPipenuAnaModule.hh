///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#ifndef pipenu_ana_TPipenuAnaModule_hh
#define pipenu_ana_TPipenuAnaModule_hh

#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TDatabasePDG.h"
#include "TTree.h"
#include "TBranch.h"
#include "TParticlePDG.h"
#include <iostream>
#include "Stntuple/loop/TStnModule.hh"
#include "Stntuple/alg/TStntuple.hh"

#include "Stntuple/obj/TStnTimeClusterBlock.hh"
#include "Stntuple/obj/TStnTrackBlock.hh"
#include "Stntuple/obj/TStnTrackSeedBlock.hh"
#include "Stntuple/obj/TStnHelixBlock.hh"
#include "Stntuple/obj/TTrackStrawHitBlock.hh"
#include "Stntuple/obj/TStnClusterBlock.hh"
#include "Stntuple/obj/TCalDataBlock.hh"
#include "Stntuple/obj/TStepPointMCBlock.hh"
#include "Stntuple/obj/TStrawHitBlock.hh"
#include "Stntuple/obj/TGenpBlock.hh"
#include "Stntuple/obj/TSimpBlock.hh"
#include "Stntuple/obj/TCrvClusterBlock.hh"
#include "Stntuple/obj/TCrvPulseBlock.hh"

#include "Stntuple/base/TStnArrayI.hh"
#include "Stntuple/geom/TStnCrystal.hh"
#include "Stntuple/alg/TStnTrackID.hh"
#include "Stntuple/alg/TEmuLogLH.hh"
#include "Stntuple/alg/TStntuple.hh"

#include "pipenu/ana/EventPar_t.hh"
#include "pipenu/ana/SimPar_t.hh"
#include "pipenu/ana/SimpData_t.hh"
#include "pipenu/ana/HelixPar_t.hh"
#include "pipenu/ana/TrackPar_t.hh"

#include "pipenu/ana/EventHist_t.hh"
#include "pipenu/ana/SimpHist_t.hh"
#include "pipenu/ana/HelixHist_t.hh"
#include "pipenu/ana/TrackHist_t.hh"

#include "pipenu/ana/InitTrackPar.hh"


//-----------------------------------------------------------------------------
// among other things, includes definitions of various histogram structures - 
// TrackHist_t etc
//-----------------------------------------------------------------------------

namespace pipenu {
//using stntuple::HistBase_t;
//  using stntuple::SimPar_t;
//  using stntuple::TrackPar_t;
//using stntuple::EventPar_t;
//using stntuple::HelixPar_t;
//using stntuple::SimpData_t;
//using stntuple::TrackHist_t;
//using stntuple::EventHist_t;
//using stntuple::SimpHist_t;
//using stntuple::HelixHist_t;
class TPipenuAnaModule: public TStnModule {
public:
  enum { kNDisks = 2 };
  enum { kMaxNTrackID = 50 };
//-----------------------------------------------------------------------------
//  histograms
//-----------------------------------------------------------------------------
 struct TimeClusterHist_t {
    TH1F*    fNsh;
    TH1F*    fNch;
    TH1F*    fT0;
    TH1F*    fT0Err;
  }; 
  struct PipenuHist_t {
    TH2F*    fPVsT0;                    //
  };


//-----------------------------------------------------------------------------
  enum { kNPipenuHistSets        = 400 };
  enum { kNEventHistSets         = 100 };
  enum { kNSimpHistSets          = 10000 };
  enum { kNTrackHistSets         = 400 };
  enum { kNTrackIDHistSets       =  10 };
  enum { kNTimeClusterHistSets   =  10 };
  enum { kNHelixHistSets         = 100 };

  struct Hist_t {
    EventHist_t*           fEvent      [kNEventHistSets  ];
    SimpHist_t*            fSimp       [kNSimpHistSets   ];
    TrackHist_t*           fTrack      [kNTrackHistSets  ];
    PipenuHist_t*          fPipenu     [kNPipenuHistSets ];
    TStnTrackID::Hist_t*   fTrackID    [kNTrackIDHistSets];
    TimeClusterHist_t*     fTimeCluster[kNTimeClusterHistSets];
    HelixHist_t*           fHelix      [kNHelixHistSets  ];
  };
//-----------------------------------------------------------------------------
//  data members
//-----------------------------------------------------------------------------
public:
  TString               fTrackBlockName;      // 
  TString               fTrackStrawHitBlockName; // 
                                             // pointers to the data blocks used
  TStnTrackBlock*       fTrackBlock;
  TStnTrackSeedBlock*   fTrackSeedBlock;
  TStnHelixBlock*       fHelixBlock;
  TStnTimeClusterBlock* fTimeClusterBlock;
  TTrackStrawHitBlock* fTrackStrawHitBlock;
  TStnClusterBlock*     fClusterBlock;
  TCalDataBlock*        fCalDataBlock;
  TStrawHitBlock*      fStrawHitBlock;
  TStepPointMCBlock*    fVDetBlock;
  TGenpBlock*           fGenpBlock;
  TSimpBlock*           fSimpBlock;
                                        // additional parameters (assume nhelices/ntracks < 20)
  HelixPar_t           fHelixPar[20];
  TrackPar_t           fTrackPar[20];

  SimPar_t             fSimPar;

                                        // histograms filled
  Hist_t               fHist;

  int                  fDirection;         // 1:downstream, -1:upstream  [direction of the particle]

  double               fEleE;           // electron energy

  int                  fCalorimeterType;

  int                  fNClusters;
  int                  fNCalPatRec;
  int                  fNMatchedTracks;
  int                  fNStrawHits;
  int                  fNCalHits;
  int                  fNGenp;          // N(generated particles)

                                        // fTrackNumber[i]: track number, 
                                        // corresponding to OBSP particle #i
                                        // or -1
  TStnArrayI           fTrackNumber;

  int                  fBestID;

  TStnTrack*           fTrack;
  TStnCluster*         fCluster;

  double               fLumWt;
  int                  fApplyCorrections;  // 0: do not apply momentum ant DT corrections

  int                  fEventPassedSelections; // 1: event passed analysis selections; 0: event didn't pass
  int                  fDnMax;
  int                  fN300;

  double               fMinT0;
  double               fEventWeight;
  int                  fBatchMode;
  int                  fPDGCode;		// determines which one
  int                  fMCProcessCode;
  double               fWeight;         // event weight, determined by the production cross section
  double               fTMaxSimp;	 // in seconds
  double               fTMaxSpmc;	 // in ns
				 // antiproton-specific : in the production vertex
  double               fPbarCosThPV;
  double               fPbarMomPV;
  TStepPointMC*        fHitVD08;
  TStepPointMC*        fHitVD15;
  int                  fNSimp;
  EventPar_t           fEvtPar;
  int                  fNVDetHits  ;
  int                  fNID;  // number of different track ID's used, default=0
  TStnTrackID*         fTrackID[kMaxNTrackID];
  TStntuple*           fStnt;                   // STNTUPLE singleton
  TStnTrackID*         GetTrackID(int I) { return fTrackID[I]; } //accessor
  // mva_data*            fTrqMVA[20];
  int                  fApplyCorr;              // default=1
  double               fMbTime;                 // microbunch time
  //int                  fUseTrqMVA;
  //mva_data*            fPidMVA[20];
  //int                  fUsePidMVA;
//-----------------------------------------------------------------------------
//  functions
//-----------------------------------------------------------------------------
public:
  TPipenuAnaModule(const char* name="PipenuAna", const char* title="PipenuAna");
  ~TPipenuAnaModule();
//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// modifiers
//-----------------------------------------------------------------------------
  void               SetDirection    (int Dir  ) { fDirection     = Dir  ; }
   void               SetDnMax        (int N    ) { fDnMax         = N    ; }

  void               SetTrackBlockName         (const char* Name) { fTrackBlockName         = Name; }
  void               SetTrackStrawHitBlockName (const char* Name) { fTrackStrawHitBlockName = Name; }

  void               SetApplyCorrections(int YesNo) { fApplyCorrections = YesNo; }
//-----------------------------------------------------------------------------
// overloaded methods of TStnModule
//-----------------------------------------------------------------------------
  virtual int     BeginJob();
  // virtual int     BeginRun();   // use TAnaModule::BeginRun()
  virtual int     Event   (int ientry);
  virtual int     EndJob  ();
//-----------------------------------------------------------------------------
// other methods
//-----------------------------------------------------------------------------
  void    BookPipenuHistograms       (PipenuHist_t*      Hist, const char* Folder);
  void    BookEventHistograms        (EventHist_t* Hist, const char* Folder);
  void    BookSimpHistograms         (SimpHist_t* Hist, const char* Folder);
  void    BookTrackHistograms        (TrackHist_t*         Hist, const char* Folder);
  void    BookTrackIDHistograms      (TStnTrackID::Hist_t* Hist, const char* Folder);
  void    BookTimeClusterHistograms  (TimeClusterHist_t* Hist, const char* Folder);
  void    BookHelixHistograms        (HistBase_t*          Hist, const char* Folder);
  void    FillPipenuHistograms    (PipenuHist_t*  Hist, 
                                   TStnTrack*     Trk, 
  				   TrackPar_t*    Tp, 
  				   SimPar_t*      SimPar,
  				   double         Weight = 1.);

  void    FillEventHistograms     (HistBase_t*  Hist, EventPar_t*  Evtpar  );
  void    FillSimpHistograms         (HistBase_t* Hist, TSimParticle* Simp, SimpData_t* SimpData, double Weight = 1.);
  void    FillTrackHistograms     (TrackHist_t* Hist, 
                                           TStnTrack*   Trk, 
                                           TrackPar_t*  Tp, 
                                           SimPar_t*    SimPar,
                                           double       Weight = 1.);
  void    FillTimeClusterHistograms(TimeClusterHist_t* Hist, TStnTimeCluster* Tc     , double Weight = 1.);
virtual void    FillHelixHistograms     (HistBase_t*  Hist, TStnHelix* Hel, HelixPar_t* Help, double Weight = 1);
  void    FillEfficiencyHistograms(TStnTrackBlock* TrackBlock, TStnTrackID* TrackID, int HistSet);

  
 
  void    BookHistograms();
  void    FillHistograms();

  void    Debug();
//-----------------------------------------------------------------------------
// test
//-----------------------------------------------------------------------------
  void    Test001();

  ClassDef(pipenu::TPipenuAnaModule,0)
};
}
#endif
