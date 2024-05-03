//////////////////////////////////////////////////////////////////////////////
// use of TStnTrack::fTmp:
//
// Tmp(0) : corrected momentum at the tracker front - not yet
// 
// use of debug bits: 
//  0  : one line per track
//  1  : passed events
//  2  : rejected events
// 
//  3  : events with good tracks P > 70 and T0 > 300
//  5  : events with N(tracks) > 1
//
// 3 different ID : 
///////////////////////////////////////////////////////////////////////////////
#include "TF1.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TEnv.h"
#include "TSystem.h"
#include "TDirectory.h"

#include "Stntuple/loop/TStnAna.hh"
#include "Stntuple/obj/TStnHeaderBlock.hh"
#include "Stntuple/alg/TStntuple.hh"
#include "Stntuple/geom/TDisk.hh"
#include "Stntuple/obj/TStnNode.hh"
#include "Stntuple/val/stntuple_val_functions.hh"
#include "Offline/DataProducts/inc/VirtualDetectorId.hh"
#include "TDatabasePDG.h"
#include "TParticlePDG.h"
//------------------------------------------------------------------------------
// Mu2e offline includes
//-----------------------------------------------------------------------------

#include "pipenu/ana/ana/TPipenuAnaModule.hh"

ClassImp(pipenu::TPipenuAnaModule)

namespace pipenu {
//-----------------------------------------------------------------------------
TPipenuAnaModule::TPipenuAnaModule(const char* name, const char* title): 
  TStnModule(name,title) { 
  fTrackBlockName         = "TrackBlock";
  fTrackStrawHitBlockName = "TrackStrawHitBlock";
  fTrackNumber.Set(100);
  fCalorimeterType = 2;
  fMinT0           = 700; 
  fEventWeight = 1.; // need to look into it later.
  fApplyCorr   = 1;                    // by default, corfect momentum and delta(T)     
  fMbTime      = 1695.;
// //-----------------------------------------------------------------------------
// // multivariate ways of figuring out the track quality
// //-----------------------------------------------------------------------------
//   fUseTrqMVA = 0;
//   fTrqMVA[0] = nullptr;
//   fTrqMVA[1] = nullptr;

//-----------------------------------------------------------------------------
// initialize Track ID
// 0: SetC  1: TrkQual>0.1 2:TrkQual>0.4
// what about number of hits ? - 3: no cuts on the number of hits
//-----------------------------------------------------------------------------
  fNID             = 2;

  fTrackID[0]      = new TStnTrackID();

  int mask0 = TStnTrackID::kNActiveBit | TStnTrackID::kChi2DofBit | TStnTrackID::kMomErrBit ;
  fTrackID[0]->SetMaxChi2Dof(3.0 );
  fTrackID[0]->SetMinNActive(25  );
  fTrackID[0]->SetMaxMomErr (0.25);

  mask0    |= TStnTrackID::kTanDipBit;
  fTrackID[0]->SetMinTanDip (0.6);
  fTrackID[0]->SetMaxTanDip (1.0);

  mask0    |= TStnTrackID::kD0Bit;
  fTrackID[0]->SetMinD0 (-100);         // mm
  fTrackID[0]->SetMaxD0 ( 100);

  fTrackID[0]->SetUseMask(mask0);
//-----------------------------------------------------------------------------
// tight ID
//-----------------------------------------------------------------------------
  fTrackID[1]      = new TStnTrackID();

  int mask1 = TStnTrackID::kNActiveBit | TStnTrackID::kChi2DofBit | TStnTrackID::kMomErrBit ;
  fTrackID[1]->SetMaxChi2Dof(3.0 );
  fTrackID[1]->SetMinNActive(30  );     // cut tighter on the number of hits
  fTrackID[1]->SetMaxMomErr (0.25);

  mask1    |= TStnTrackID::kTanDipBit;
  fTrackID[1]->SetMinTanDip (0.65);
  fTrackID[1]->SetMaxTanDip (1.20);

  mask1    |= TStnTrackID::kD0Bit;
  fTrackID[1]->SetMinD0 ( -50);         // mm
  fTrackID[1]->SetMaxD0 ( 100);

  fTrackID[1]->SetUseMask(mask1);

  fBestID           = 0;

  for (int i=0; i<20; i++) {
    TrackPar_t* tp  = fTrackPar+i;
    tp->fTrackID[0] = fTrackID[0];
    tp->fTrackID[1] = fTrackID[1];
  }

//-----------------------------------------------------------------------------
// MC truth: define which MC particle to consider as signal
// 2:conversionGun, 28:StoppedParticleReactionGun - see 
//-----------------------------------------------------------------------------
  fDirection        = 1;

  fDnMax            = 15;
//-----------------------------------------------------------------------------
// this is redefiend by the dataset catalog anyway
//-----------------------------------------------------------------------------
  fPDGCode          = -11;
  fMCProcessCode    = 181;   // mu2e::ProcessCode::mu2ePienu;
}

//-----------------------------------------------------------------------------
TPipenuAnaModule::~TPipenuAnaModule() {
  for (int i=0; i<fNID; i++) delete fTrackID[i];
}


//-----------------------------------------------------------------------------
// register data blocks and book histograms
//-----------------------------------------------------------------------------
int TPipenuAnaModule::BeginJob() {
//-----------------------------------------------------------------------------
// register data blocks
/// /-----------------------------------------------------------------------------
  RegisterDataBlock(fTrackBlockName.Data()        ,"TStnTrackBlock"     ,&fTrackBlock  );
  // RegisterDataBlock(fTrackStrawHitBlockName.Data(),"TTrackStrawHitBlock",&fTrackStrawHitBlock);

  RegisterDataBlock("KSFBlock"        ,"TStnTrackSeedBlock"  ,&fTrackSeedBlock  );
  RegisterDataBlock("HelixBlock"      ,"TStnHelixBlock"      ,&fHelixBlock      );
  RegisterDataBlock("TimeClusterBlock","TStnTimeClusterBlock",&fTimeClusterBlock);

  RegisterDataBlock("ClusterBlock" ,"TStnClusterBlock" ,&fClusterBlock );
  RegisterDataBlock("CalDataBlock" ,"TCalDataBlock"    ,&fCalDataBlock );
  RegisterDataBlock("StrawHitBlock","TStrawHitBlock"   ,&fStrawHitBlock);
  RegisterDataBlock("GenpBlock"    ,"TGenpBlock"       ,&fGenpBlock    );
  RegisterDataBlock("SimpBlock"    ,"TSimpBlock"       ,&fSimpBlock    );
  RegisterDataBlock("SpmcBlockVDet","TStepPointMCBlock",&fVDetBlock    );
//-----------------------------------------------------------------------------
// book histograms
//-----------------------------------------------------------------------------
  BookHistograms();

  return 0;
}

//-----------------------------------------------------------------------------
void TPipenuAnaModule::BookPipenuHistograms(PipenuHist_t* Hist, const char* Folder) {
  HBook2F(Hist->fPVsT0 ,"p_vs_t0",Form("%s: track P s T0"       ,Folder), 200, 0,  2000, 100, 0, 100,Folder);
}
//-----------------------------------------------------------------------------
void TPipenuAnaModule::BookTimeClusterHistograms(TimeClusterHist_t* Hist, const char* Folder) {
  HBook1F(Hist->fNsh  ,"nsh"  ,Form("%s: N(single straw hits)",Folder), 200, 0,  200,Folder);
  HBook1F(Hist->fNch  ,"nch"  ,Form("%s: N(combo hits)"       ,Folder), 200, 0,  200,Folder);
  HBook1F(Hist->fT0   ,"t0"   ,Form("%s: T0"                  ,Folder), 200, 0, 2000,Folder);
  HBook1F(Hist->fT0Err,"t0err",Form("%s: T0Err"               ,Folder), 200, 0, 2000,Folder);
}
//-----------------------------------------------------------------------------
void TPipenuAnaModule::BookEventHistograms(EventHist_t* Hist, const char* Folder) {
  EventHist_t* hist = (EventHist_t*) Hist;
  //  char name [200];
  //  char title[200];

  HBook1F(hist->fEventWeight[0],"eventweight_0" ,Form("%s: Event Weight"            ,Folder), 100, -1.,   2.,Folder);
  HBook1F(hist->fEventWeight[1],"eventweight_1" ,Form("%s: Log10(Event Weight)"     ,Folder), 100,-15.,   5.,Folder);
  HBook1F(hist->fEventE        ,"event_energy"  ,Form("%s: Relevant Event Energy"   ,Folder), 400,  0., 200.,Folder);
  HBook1F(hist->fInstLumi[0]   ,"inst_lumi_0"   ,Form("%s: POT"                     ,Folder), 300,  0.,1.5e8,Folder);
  HBook1F(hist->fInstLumi[1]   ,"inst_lumi_1"   ,Form("%s: POT"                     ,Folder), 300,  0.,1.5e8,Folder);
  HBook1F(hist->fInstLumi[2]   ,"inst_lumi_2"   ,Form("%s: POT"                     ,Folder), 300,  0.,1.5e8,Folder);
  HBook1F(hist->fBatchWeight[0],"batchweight_0" ,Form("%s: Log10(One Batch Weight)" ,Folder), 100, -20., 2.,Folder);
  HBook1F(hist->fBatchWeight[1],"batchweight_1" ,Form("%s: Log10(Two Batch Weight)" ,Folder), 100, -20., 2.,Folder);

  HBook1F(hist->fMcCosTh   ,"mc_costh" ,Form("%s: Conversion Electron Cos(Theta)"  ,Folder),100,-1,1,Folder);
  HBook1F(hist->fMcMom     ,"mc_mom"   ,Form("%s: Conversion Electron Momentum"    ,Folder),1000,  0,200,Folder);
  HBook1D(hist->fDioMom    ,"dio_mom"  ,Form("%s: DIO momentum"                    ,Folder),1000, 50,150,Folder);
  HBook1F(hist->fRv        ,"rv"      ,Form("%s: R(Vertex)"                       ,Folder), 100, 0, 1000,Folder);
  HBook1F(hist->fZv        ,"zv"      ,Form("%s: Z(Vertex)"                       ,Folder), 300, 0,15000,Folder);
  //  HBook1F(hist->fNClusters ,"ncl"      ,Form("%s: Number of Reconstructed Clusters",Folder),200,0,200,Folder);
  HBook1F(hist->fNTracks[0] ,"ntrk_0"     ,Form("%s: Number of Reconstructed Tracks[0]"  ,Folder),100,0,100,Folder);
  HBook1F(hist->fNTracks[1] ,"ntrk_1"     ,Form("%s: Number of Reconstructed Tracks[1]"  ,Folder),100,0,100,Folder);
  HBook1F(hist->fNShTot[0],"nsh_0" ,Form("%s: Number of Straw Hits [0]"        ,Folder),250,0,250,Folder);
  HBook1F(hist->fNShTot[1],"nsh_1" ,Form("%s: Number of Straw Hits [1]"        ,Folder),500,0,10000,Folder);
  HBook1F(hist->fNGoodSH   ,"nsh50"    ,Form("%s: N(SH) +/-50"                     ,Folder),300,0,1500,Folder);
  HBook1F(hist->fNChTot[0],"nch_0" ,Form("%s: Number of Combo Hits [0]"        ,Folder),250,0,250,Folder);
  HBook1F(hist->fNChTot[1],"nch_1" ,Form("%s: Number of Combo Hits [1]"        ,Folder),500,0,10000,Folder);
  HBook1F(hist->fDtClT     ,"dt_clt"   ,Form("%s: DT(cluster-track)"               ,Folder),100,-100,100,Folder);
  HBook1F(hist->fDtClS     ,"dt_cls"   ,Form("%s: DT(cluster-straw hit)"           ,Folder),200,-200,200,Folder);
  HBook1F(hist->fSHTime    ,"shtime"   ,Form("%s: Straw Hit Time"                  ,Folder),400,0,2000,Folder);
  HBook1F(hist->fEClMax    ,"eclmax"   ,Form("%s: Max cluster energy"              ,Folder),150,0,150,Folder);
  HBook1F(hist->fNHyp      ,"nhyp"     ,Form("%s: N(fit hypotheses)"               ,Folder),5,0,5,Folder);
  HBook1F(hist->fBestHyp[0],"bfh0"     ,Form("%s: Best Fit Hyp[0](e-,e+,mu-,mu+)"  ,Folder),5,0,5,Folder);
  HBook1F(hist->fBestHyp[1],"bfh1"     ,Form("%s: Best Fit Hyp[1](e-,e+,mu-,mu+)"  ,Folder),5,0,5,Folder);
  HBook1F(hist->fNGenp     ,"ngenp"    ,Form("%s: N(Gen Particles)"                ,Folder),500,0,500,Folder);

  HBook1F(hist->fNCrvClusters    ,"ncrvcl"  ,Form("%s: N(CRV Clusters)"                 ,Folder),100,0,  100,Folder);
  HBook1F(hist->fNCrvCoincidences[0],"ncrvco_0"  ,Form("%s: N(CRV coincidences)[0]"     ,Folder),200,0, 1000,Folder);
  HBook1F(hist->fNCrvCoincidences[1],"ncrvco_1"  ,Form("%s: N(CRV coincidences)[1]"     ,Folder),200,0,  200,Folder);
  HBook1F(hist->fNCrvPulses[0]   ,"ncrvp_0" ,Form("%s: N(CRV pulses)[0]"                ,Folder),500,0,10000,Folder);
  HBook1F(hist->fNCrvPulses[1]   ,"ncrvp_1" ,Form("%s: N(CRV pulses)[1]"                ,Folder),500,0,  500,Folder);

}


//-----------------------------------------------------------------------------
void TPipenuAnaModule::BookSimpHistograms(SimpHist_t* Hist, const char* Folder) {
  SimpHist_t* hist = (SimpHist_t*) Hist;
  HBook1F(hist->fStage          ,"stage"       ,Form("%s: Stage"                   ,Folder),  10,     0,   10,Folder);

  HBook1F(hist->fPdgCode[0]     ,"pdg_0"       ,Form("%s: PDG code[0]"             ,Folder), 200,-100, 100,Folder);
  HBook1F(hist->fPdgCode[1]     ,"pdg_1"       ,Form("%s: PDG code[1]"             ,Folder),1000,-500, 500,Folder);
  //  HBook1F(hist->fParentMom      ,"pmom"        ,Form("%s: Parent Mom"  ,Folder), 250,     0, 500,Folder);
  //HBook1F(hist->fParentPDG      ,"ppdg"        ,Form("%s: Parent PDG"  ,Folder), 200, -1000, 1000,Folder);

  HBook1F(hist->fGeneratorID    ,"gen_id"      ,Form("%s: Generator ID"            ,Folder), 200,   -10,  190,Folder);

  HBook1F(hist->fStartVolumeID  ,"vid0"        ,Form("%s: start volume ID"         ,Folder), 200,2900,3100,Folder);
  HBook1F(hist->fEndVolumeID    ,"vid1"        ,Form("%s: end volume ID"           ,Folder), 200,2900,3100,Folder);

  HBook1F(hist->fStartTime      ,"stime"       ,Form("%s: start time"              ,Folder), 200,   0,2000,Folder);
  HBook1F(hist->fEndTime        ,"etime"       ,Form("%s: end time"                ,Folder), 200,   0,2000,Folder);
  HBook1F(hist->fStageDt        ,"sdt"         ,Form("%s: Stage T1-T0"             ,Folder), 400,   0,2000,Folder);

  HBook1F(hist->fStartMom[0]    ,"mom"         ,Form("%s: start Mom[0]"            ,Folder), 500,     0,  500,Folder);
  HBook1F(hist->fStartMom[1]    ,"mom_1"       ,Form("%s: start Mom[1]"            ,Folder), 500,     0, 5000,Folder);

  HBook1F(hist->fEndMom[0]      ,"emom"        ,Form("%s: end Mom[0]"              ,Folder), 500,     0,  500,Folder);
  HBook1F(hist->fEndMom[1]      ,"emom_1"      ,Form("%s: end Mom[1]"              ,Folder), 500,     0, 5000,Folder);

  HBook1F(hist->fNStrawHits     ,"nsh"      ,Form("%s: N(straw hits)" ,Folder), 100,     0,  100,Folder);

  HBook1F(hist->fMomTargetEnd   ,"ptarg"       ,Form("%s: mom after ST"            ,Folder), 400,   0, 200,Folder);
  HBook1F(hist->fMomTrackerFront,"pfront"      ,Form("%s: mom at the Tracker Front",Folder), 400,   0, 200,Folder);
  HBook1F(hist->fCosTh          ,"costh"       ,Form("%s: cos(theta)"              ,Folder), 200,  -1,   1,Folder);
  HBook1F(hist->fZStart         ,"zstart"      ,Form("%s: ZStart"                  ,Folder), 250,  -5000,20000,Folder);
  HBook1F(hist->fZEnd           ,"zend"        ,Form("%s: ZEnd"                    ,Folder), 250,  -5000,20000,Folder);

  HBook2F(hist->fNshVsCosTh     ,"nsh_vs_costh",Form("%s: nsh vs costh"            ,Folder), 20 ,-1,1,40,0,200,Folder);

  HBook2F(hist->fYVsX           ,"y_vs_x"     ,Form("%s: yend vs Xend " ,Folder), 250,  -250, 250, 250, -250, 250,Folder);
  HBook2F(hist->fXEndVsZEnd     ,"xe_vs_ze"   ,Form("%s: xend vs zend " ,Folder), 250,  -5000, 20000, 100, -5000, 5000,Folder);
  HBook2F(hist->fYcVsZEnd       ,"yc_vs_ze"   ,Form("%s: yc vs zend "   ,Folder), 250,  -5000, 20000, 200,   -200, 200,Folder);
  HBook2F(hist->fPVD9VsZEnd     ,"pvd9_vs_ze" ,Form("%s: pvd9 vs zend " ,Folder), 250,  -5000, 20000, 200,      0, 200,Folder);

  HBook2F(hist->fYVsX_2480      ,"y_vs_x_2480",Form("%s: Y vs X [2480]" ,Folder), 250,  -250, 250, 250, -250, 250,Folder);
  HBook2F(hist->fYVsX_2513      ,"y_vs_x_2513",Form("%s: Y vs X [2513]" ,Folder), 250,  -250, 250, 250, -250, 250,Folder);

  HBook2F(hist->fCosThVsMom[0]  ,"cth_vs_mom"  ,Form("%s: Cos(Th) vs Mom[0]",Folder), 250,   0, 5000,100,-1,1,Folder);
  HBook2F(hist->fCosThVsMom[1]  ,"cth_vs_mom_1",Form("%s: Cos(Th) vs Mom[1]",Folder), 250,   0,  250,100,-1,1,Folder);

  HBook2F(hist->fCosThVsMomPV   ,"cth_vs_mom_pv",Form("%s: Cos(Th):Mom PV" ,Folder), 250,   0,  5000,100,-1,1,Folder);
}
//-----------------------------------------------------------------------------
void TPipenuAnaModule::BookTrackHistograms(TrackHist_t* Hist, const char* Folder) {
//   char name [200];
//   char title[200];
 HBook1F(Hist->fP[0]       ,"p"        ,Form("%s: Track P(Z1)"       ,Folder), 800,  80  ,120. ,Folder);
  HBook1F(Hist->fP[1]       ,"p_1"      ,Form("%s: Track P(total)[1]" ,Folder), 100, 100  ,105. ,Folder);
  HBook1F(Hist->fP[2]       ,"p_2"      ,Form("%s: Track P(total)[1]" ,Folder),2000,   0  ,200. ,Folder);
  HBook1F(Hist->fP0         ,"p0"       ,Form("%s: Track P(Z0)"       ,Folder),1000,   0  ,200. ,Folder);
  HBook1F(Hist->fP2         ,"p2"       ,Form("%s: Track P(z=-1540)"  ,Folder),1000,   0  ,200. ,Folder);
  HBook1F(Hist->fPDio       ,"pdio"     ,Form("%s: Track P Wt=LL(DIO)",Folder), 100, 100  ,105. ,Folder);
//-----------------------------------------------------------------------------
  HBook1F(Hist->fFitMomErr  ,"momerr"   ,Form("%s: Track FitMomError" ,Folder), 200,   0  ,  1. ,Folder);
  HBook1F(Hist->fPFront     ,"pf"       ,Form("%s: Track P(front)   " ,Folder), 400,  90  ,110. ,Folder);
  HBook1F(Hist->fDpFront    ,"dpf"      ,Form("%s: Track P-P(front) " ,Folder),1000,  -5. ,  5. ,Folder);
  HBook1F(Hist->fXDpF       ,"xdpf"     ,Form("%s: DpF/momErr"        ,Folder),1000, -50. , 50. ,Folder);
  HBook1F(Hist->fDpFront0   ,"dp0f"     ,Form("%s: Track P0-P(front)" ,Folder),1000,  -5. ,  5. ,Folder);
  HBook1F(Hist->fDpFront2   ,"dp2f"     ,Form("%s: Track P2-P(front)" ,Folder),1000,  -5. ,  5. ,Folder);
  HBook1F(Hist->fPStOut     ,"pstout"   ,Form("%s: Track P(ST_Out)  " ,Folder), 400,  90. ,110. ,Folder);
  HBook1F(Hist->fDpFSt      ,"dpfst"    ,Form("%s: Track Pf-Psto"     ,Folder),1000,  -5  ,  5. ,Folder);
  HBook2F(Hist->fDpFVsZ1    ,"dpf_vs_z1",Form("%s: Track DPF Vs Z1"   ,Folder), 200, -2000.,0,200,-5.,5,Folder);
  HBook2F(Hist->fPVsGenE    ,"p_vs_gene",Form("%s: Track P vs Gen E"  ,Folder), 300, 0., 150., 300, 0., 150., Folder);

  HBook1F(Hist->fPt         ,"pt"       ,Form("%s: Track Pt"          ,Folder), 600, 75,95,Folder);
  HBook1F(Hist->fCosTh      ,"costh"    ,Form("%s: Track cos(theta)"  ,Folder), 100,-1,1  ,Folder);
  HBook1F(Hist->fChi2       ,"chi2"     ,Form("%s: Track chi2 total"  ,Folder), 200, 0,200,Folder);
  HBook1F(Hist->fChi2Dof    ,"chi2d"    ,Form("%s: track chi2/N(dof)" ,Folder), 500, 0, 10,Folder);

  HBook1F(Hist->fNActive    ,"nactv"    ,Form("%s: N(active)"         ,Folder), 200,  0  , 200 ,Folder);
  HBook1F(Hist->fNaFract    ,"nafr"     ,Form("%s: N(active fraction)",Folder), 110,  0.5,1.05 ,Folder);
  HBook1F(Hist->fDNa        ,"dna"      ,Form("%s: Nhits-Nactive"     ,Folder), 100, -0.5 ,99.5,Folder);
  HBook1F(Hist->fNtch       ,"ntch"     ,Form("%s: N timecluster hits",Folder), 200, -0.5 ,199.5,Folder);
  HBook1F(Hist->fNWrong     ,"nwrng"    ,Form("%s: N(wrong drift sgn)",Folder), 100,  0   ,100,Folder);
  HBook1F(Hist->fNDoublets  ,"nd"       ,Form("%s: N(doublets)"       ,Folder),  50, 0, 50,Folder);
  HBook1F(Hist->fNadOverNd  ,"nad_nd"   ,Form("%s: Nad/N(doublets)"   ,Folder), 110, 0,1.1,Folder);
  HBook1F(Hist->fNSSD       ,"nssd"     ,Form("%s: N(SS doublets)"    ,Folder),  50, 0, 50,Folder);
  HBook1F(Hist->fNOSD       ,"nosd"     ,Form("%s: N(OS doublets)"    ,Folder),  50, 0, 50,Folder);
  HBook1F(Hist->fNdOverNa   ,"nd_na"    ,Form("%s: NDoublets/Nactive" ,Folder), 100, 0,0.5,Folder);
  HBook1F(Hist->fNssdOverNa ,"nssd_na"  ,Form("%s: NSSD/Nactive"      ,Folder), 100, 0,0.5,Folder);
  HBook1F(Hist->fNosdOverNa ,"nosd_na"  ,Form("%s: NOSD/Nactive"      ,Folder), 100, 0,0.5,Folder);
  HBook1F(Hist->fNZeroAmb   ,"nza"      ,Form("%s: N (Iamb = 0) hits" ,Folder), 100, 0,100,Folder);
  HBook1F(Hist->fNzaOverNa  ,"nza_na"   ,Form("%s: NZeroAmb/Nactive"  ,Folder), 100, 0,  1,Folder);
  HBook1F(Hist->fNMatActive ,"nma"      ,Form("%s: N (Mat Active"     ,Folder), 100, 0,100,Folder);
  HBook1F(Hist->fNmaOverNa  ,"nma_na"   ,Form("%s: NMatActive/Nactive",Folder), 200, 0,   2,Folder);
  HBook1F(Hist->fNBend      ,"nbend"    ,Form("%s: Nbend"             ,Folder), 100, 0,1000,Folder);

  HBook1F(Hist->fT0         ,"t0"       ,Form("%s: track T0"          ,Folder), 200,    0, 2000,Folder);
  HBook1F(Hist->fT0Err      ,"t0err"    ,Form("%s: track T0Err"       ,Folder), 100,    0,   10,Folder);
  HBook1F(Hist->fDtTc       ,"dttc"     ,Form("%s: T(trk)-T(TC)"      ,Folder), 100,  -50,   50,Folder);

  HBook1F(Hist->fQ          ,"q"        ,Form("%s: track Q"           ,Folder),   4,-2,   2,Folder);
  HBook1F(Hist->fFitCons[0] ,"fcon"     ,Form("%s: track fit cons [0]",Folder), 200, 0,   1,Folder);
  HBook1F(Hist->fFitCons[1] ,"fcon1"    ,Form("%s: track fit cons [1]",Folder), 1000, 0,   0.1,Folder);
  HBook1F(Hist->fD0         ,"d0"       ,Form("%s: track D0      "    ,Folder), 200,-200, 200,Folder);
  HBook1F(Hist->fZ0         ,"z0"       ,Form("%s: track Z0      "    ,Folder), 200,-2000,2000,Folder);
  HBook1F(Hist->fTanDip     ,"tdip"     ,Form("%s: track tan(dip)"    ,Folder), 200, 0.0 ,2.0,Folder);
  HBook1F(Hist->fRMax       ,"rmax"     ,Form("%s: track R(max)  "    ,Folder), 200, 0., 1000,Folder);
  HBook1F(Hist->fDtZ0       ,"dtz0"     ,Form("%s: T0_trk-T0_MC(Z=0)" ,Folder), 200, -10.0 ,10.0,Folder);
  HBook1F(Hist->fXtZ0       ,"xtz0"     ,Form("%s: DT(Z0)/sigT"       ,Folder), 200, -10.0 ,10.0,Folder);

  HBook1F(Hist->fResid      ,"resid"    ,Form("%s: hit residuals"     ,Folder), 500,-0.5 ,0.5,Folder);
  HBook1F(Hist->fAlgMask    ,"alg"      ,Form("%s: algorithm mask"    ,Folder),  10,  0, 10,Folder);

  HBook1F(Hist->fChi2Tcm  ,"chi2tcm"  ,Form("%s: chi2(t-c match)"   ,Folder), 250,  0  ,250 ,Folder);
  HBook1F(Hist->fChi2XY     ,"chi2xy"   ,Form("%s: chi2(t-c match) XY",Folder), 300,-50  ,250 ,Folder);
  HBook1F(Hist->fChi2T      ,"chi2t"    ,Form("%s: chi2(t-c match) T" ,Folder), 250,  0  ,250 ,Folder);

  HBook1F(Hist->fDt         ,"dt"       ,Form("%s: T(trk)-T(cl)"      ,Folder), 400,-20  ,20 ,Folder);
  HBook1F(Hist->fDx         ,"dx"       ,Form("%s: X(trk)-X(cl)"      ,Folder), 200,-500 ,500,Folder);
  HBook1F(Hist->fDy         ,"dy"       ,Form("%s: Y(trk)-Y(cl)"      ,Folder), 200,-500 ,500,Folder);
  HBook1F(Hist->fDz         ,"dz"       ,Form("%s: Z(trk)-Z(cl)"      ,Folder), 200,-250 ,250,Folder);
  HBook1F(Hist->fDu         ,"du"       ,Form("%s: track-cluster DU"  ,Folder), 250,-250 ,250,Folder);
  HBook1F(Hist->fDv         ,"dv"       ,Form("%s: track-cluster DV"  ,Folder), 200,-100 ,100,Folder);
  HBook1F(Hist->fPath       ,"path"     ,Form("%s: track sdisk"       ,Folder),  50,   0 ,500,Folder);

  HBook1F(Hist->fECl        ,"ecl"      ,Form("%s: cluster E"         ,Folder), 300, 0   ,150,Folder);
  HBook1F(Hist->fSeedFr     ,"seed_fr"  ,Form("%s: Eseed/Etot"        ,Folder), 100, 0   ,1  ,Folder);
  HBook1F(Hist->fNCrystals  ,"ncr"      ,Form("%s: N(crystals)"       ,Folder), 100, 0   ,100,Folder);
  HBook1F(Hist->fEClEKin    ,"ecl_ekin" ,Form("%s: cluster E/Ekin(mu)",Folder), 500, 0   ,5,Folder);
  HBook1F(Hist->fEp         ,"ep"       ,Form("%s: track E/P"         ,Folder), 300, 0   ,1.5,Folder);
  HBook1F(Hist->fDrDzCal    ,"drdzcal"  ,Form("%s: track dr/dz cal"   ,Folder), 200, -5  ,5  ,Folder);
  HBook1F(Hist->fDtClZ0     ,"dtclz0"   ,Form("%s: T(cl_z0)-T(Z0)"    ,Folder), 250, -5 , 5,Folder);
  HBook2F(Hist->fDtClZ0VsECl,"dtclz0_vs_ecl",Form("%s: DtClZ0 vs ECl" ,Folder), 100, 0 , 200, 250, -5 , 5,Folder);
  HBook2F(Hist->fDtClZ0VsP  ,"dtclz0_vs_p"  ,Form("%s: DtClZ0 vs p"   ,Folder), 100, 0 , 200, 250, -5 , 5,Folder);

  HBook2F(Hist->fFConsVsNActive,"fc_vs_na" ,Form("%s: FitCons vs NActive",Folder),  150, 0, 150, 200,0,1,Folder);
  HBook1F(Hist->fDaveTrkQual,"dtqual"   ,Form("%s:DaveTrkQual"        ,Folder), 200, -0.5, 1.5,Folder);
  HBook1F(Hist->fTrqMvaOut  ,"trqmvaout",Form("%s:TRQ MVA output"     ,Folder), 200, -0.5, 1.5,Folder);

  HBook2F(Hist->fPVsTime    ,"p_vs_time",Form("%s: P(Z1) vs T(0)"     ,Folder), 800,  80  ,120. , 200, 0, 2000,Folder);

  HBook1F(Hist->fDtCRV      ,"dtcrv"     ,Form("%s:dT(CRV)"            ,Folder), 400,-1000, 1000,Folder);
  HBook1F(Hist->fDtCRV2     ,"dtcrv2"    ,Form("%s:dT(CRV2)"           ,Folder), 400,-1000, 1000,Folder);
  HBook2F(Hist->fZVsDtCRV   ,"dtcrv_vs_z",Form("%s: dT(CRV) vs Z(CRV)" ,Folder), 250,-5000,20000,500,-500, 500,Folder);
//-----------------------------------------------------------------------------
// TrkCaloHit histograms
//-----------------------------------------------------------------------------
  HBook1F(Hist->fTchTime    ,"tch_time"  ,Form("%s:TCH time"           ,Folder), 200,    0, 2000,Folder);
  HBook1F(Hist->fTchPath    ,"tch_path"  ,Form("%s:TCH path"           ,Folder), 200, -500,  500,Folder);
  HBook1F(Hist->fTchDx      ,"tch_dx"    ,Form("%s:TCH Dx"             ,Folder), 200, -500,  500,Folder);
  HBook1F(Hist->fTchDy      ,"tch_dy"    ,Form("%s:TCH Dy"             ,Folder), 200, -500,  500,Folder);
  HBook1F(Hist->fTchDz      ,"tch_dz"    ,Form("%s:TCH DZ"             ,Folder), 200, -500,  500,Folder);
  HBook1F(Hist->fTchDr      ,"tch_dr"    ,Form("%s:TCH DR"             ,Folder), 200, -100,  100,Folder);
  HBook1F(Hist->fTchDt      ,"tch_dt"    ,Form("%s:TCH Dt"             ,Folder), 400,  -10,   10,Folder);

  HBook2F(Hist->fEpVsPath   ,"ep_vs_path",Form("%s: E/P vs Path"       ,Folder), 100, -100,  400, 120,  0,  1.2,Folder);
  HBook2F(Hist->fEpVsTchDz  ,"ep_vs_tchdz",Form("%s: E/P vs Tch_DZ"    ,Folder), 100, -100,  400, 120,  0,  1.2,Folder);
  HBook2F(Hist->fTchDtVsDz  ,"tchdt_vs_dz",Form("%s: TCH_DT vs Tch_DZ" ,Folder), 100, -100,  400, 200, -10, 10 ,Folder);
//-----------------------------------------------------------------------------
// PID histograms
//-----------------------------------------------------------------------------
  HBook1F(Hist->fPidMvaOut  ,"pidmvaout",Form("%s:PID MVA output"     ,Folder), 200, -0.5, 1.5,Folder);
}
//-----------------------------------------------------------------------------
void TPipenuAnaModule::BookTrackIDHistograms(TStnTrackID::Hist_t* Hist, const char* Folder) {
//   char name [200];
//   char title[200]
  for (int i=0; i<5; i++) {
    HBook1F(Hist->fNActive[i],Form("nactive_%i",i) ,Form("%s: Nactive [%i]"        ,Folder,i), 150,   0  , 150. ,Folder);
    HBook1F(Hist->fFitCons[i],Form("fcons_%i"  ,i) ,Form("%s: FitCons [%i]"        ,Folder,i), 200,   0  ,   1. ,Folder);
    HBook1F(Hist->fChi2Dof[i],Form("chi2d_%i"  ,i) ,Form("%s: Chi2/Dof[%i]"        ,Folder,i), 200,   0  ,  20. ,Folder);
    HBook1F(Hist->fT0     [i],Form("t0_%i"     ,i) ,Form("%s: T0      [%i]"        ,Folder,i), 200,   0  ,2000. ,Folder);
    HBook1F(Hist->fT0Err  [i],Form("t0err_%i"  ,i) ,Form("%s: T0Err   [%i]"        ,Folder,i), 200,   0  ,   2. ,Folder);
    HBook1F(Hist->fMomErr [i],Form("momerr_%i" ,i) ,Form("%s: MomErr  [%i]"        ,Folder,i), 200,   0  ,   1. ,Folder);
    HBook1F(Hist->fDNa    [i],Form("dna_%i"    ,i) ,Form("%s: DNa     [%i]"        ,Folder,i), 100,   0  , 100. ,Folder);
    HBook1F(Hist->fTanDip [i],Form("tandip_%i" ,i) ,Form("%s: TanDip  [%i]"        ,Folder,i), 400,   0  ,   4. ,Folder);
    HBook1F(Hist->fD0     [i],Form("d0_%i"     ,i) ,Form("%s: D0      [%i]"        ,Folder,i), 400, -200., 200. ,Folder);
    HBook1F(Hist->fRMax   [i],Form("rmax_%i"   ,i) ,Form("%s: RMax    [%i]"        ,Folder,i), 400,    0., 800. ,Folder);
    HBook1F(Hist->fTrkQual[i],Form("trkqual_%i",i) ,Form("%s: TrkQual [%i]"        ,Folder,i), 200,  -0.5,   1.5,Folder);
  } // i loop

  HBook1F(Hist->fPassed    ,"passed"     ,Form("%s: Passed     "         ,Folder),  5,  0,   5,Folder);
  HBook1F(Hist->fFailedBits,"failed_bits",Form("%s: Failed Bits"         ,Folder), 40,  0,  40,Folder);
} // BookTrackIDHistograms loop

//-----------------------------------------------------------------------------
void TPipenuAnaModule::BookHelixHistograms(HistBase_t* Hist, const char* Folder) {
//   char name [200];
//   char title[200];

  HelixHist_t* hist = (HelixHist_t*) Hist;

  HBook1F(hist->fBestAlg ,"best_alg",Form("%s: best algorithm"       ,Folder),  10,     0  ,  10  ,Folder);
  HBook1F(hist->fChi2XY  ,"chi2xy"  ,Form("%s: Chi2(XY)/DOF"         ,Folder), 200,     0  ,  20  ,Folder);
  HBook1F(hist->fChi2ZPhi,"chi2zp"  ,Form("%s: Chi2(ZP)/DOF"         ,Folder), 200,     0  ,  20  ,Folder);
  HBook1F(hist->fSinTh   ,"sin_th"  ,Form("%s: sin_th = Pt/P"        ,Folder), 200,    -1  ,   1  ,Folder);
  HBook1F(hist->fD0      ,"d0"      ,Form("%s: D0"                   ,Folder), 250,   250  , 250  ,Folder);
  HBook1F(hist->fHelicity,"hel"     ,Form("%s: helicity"             ,Folder),   3,    -1.5,  1.5 ,Folder);
  HBook1F(hist->fLambda  ,"lam"     ,Form("%s: lambda=dphi/dz"       ,Folder), 200,   -500 , 500  ,Folder);
  HBook1F(hist->fNCh     ,"nch"     ,Form("%s: N(combo hits)"        ,Folder), 200,    -0.5, 199.5,Folder);
  HBook1F(hist->fNSh     ,"nsh"     ,Form("%s: N(straw hits)"        ,Folder), 200,    -0.5, 199.5,Folder);
  HBook1F(hist->fP       ,"p"       ,Form("%s: P"                    ,Folder), 250,     0  , 250  ,Folder);
  HBook1F(hist->fRadius  ,"r"       ,Form("%s: Radius"               ,Folder), 200,     0  , 1000 ,Folder);
}
//_____________________________________________________________________________
void TPipenuAnaModule::BookHistograms() {

  //  char name [200];
  //  char title[200];

  TFolder* fol;
  TFolder* hist_folder;
  char     folder_name[200];

  DeleteHistograms();
  hist_folder = (TFolder*) GetFolder()->FindObject("Hist");
//-----------------------------------------------------------------------------
// book crystal histograms
//-----------------------------------------------------------------------------
  // HBook1F(fHist.fCrystalR[0],"rc_0"     ,Form("disk [0] crystal radius"),100,0,1000,"Hist");
  // HBook1F(fHist.fCrystalR[1],"rc_1"     ,Form("disk [1] crystal radius"),100,0,1000,"Hist");
//-----------------------------------------------------------------------------
// book pipenu histograms
//-----------------------------------------------------------------------------
  int book_pipenu_histset[kNPipenuHistSets];
  for (int i=0; i<kNPipenuHistSets; i++) book_pipenu_histset[i] = 0;

  book_pipenu_histset[  0] = 1;		// all  tracks
  book_pipenu_histset[  1] = 1;		// IDWord[0]=0 tracks
  book_pipenu_histset[ 21] = 1;		// IDWord[1]=0 tracks

  book_pipenu_histset[ 50] = 1;		// all  tracks, wt = pion surv prob

  book_pipenu_histset[ 51] = 1;		// IDWord[0]=0 tracks, wt=pionSurvProb
  book_pipenu_histset[ 71] = 1;		// IDWord[1]=0 tracks, wt=pionSurvProb

  book_pipenu_histset[100] = 1;		// all  tracks Q>0

  book_pipenu_histset[101] = 1;		// IDWord[0]=0 tracks Q>0
  book_pipenu_histset[102] = 1;		// IDWord[0]=0 tracks Q>0 p>60
  book_pipenu_histset[103] = 1;		// IDWord[0]=0 tracks Q>0 p>60 T>300
  book_pipenu_histset[104] = 1;		// IDWord[0]=0 tracks Q>0 p>60 T>300 DnTrackTc
  book_pipenu_histset[105] = 1;		// IDWord[0]=0 tracks Q>0 p>60 T>300 DnTrackTc DtTcTc
  book_pipenu_histset[106] = 1;		// IDWord[0]=0 tracks Q>0 T>300
  book_pipenu_histset[107] = 1;		// IDWord[0]=0 tracks Q>0 T>300 DnTrackTc
  book_pipenu_histset[108] = 1;		// IDWord[0]=0 tracks Q>0 T>300 DnTrackTc DtTcTc

  book_pipenu_histset[121] = 1;		// IDWord[1]=0 tracks Q>0
  book_pipenu_histset[122] = 1;		// IDWord[1]=0 tracks Q>0 p>60
  book_pipenu_histset[123] = 1;		// IDWord[1]=0 tracks Q>0 p>60 T>300
  book_pipenu_histset[124] = 1;		// IDWord[1]=0 tracks Q>0 p>60 T>300 DnTrackTc
  book_pipenu_histset[125] = 1;		// IDWord[1]=0 tracks Q>0 p>60 T>300 DnTrackTc DtTcTc
  book_pipenu_histset[126] = 1;		// IDWord[1]=0 tracks Q>0 T>300
  book_pipenu_histset[127] = 1;		// IDWord[1]=0 tracks Q>0 T>300 DnTrackTc
  book_pipenu_histset[128] = 1;		// IDWord[1]=0 tracks Q>0 T>300 DnTrackTc DtTcTc

  book_pipenu_histset[150] = 1;		// all  tracks, wt = pionSurvProb Q>0

  book_pipenu_histset[151] = 1;		// IDWord[0]=0 tracks, wt = pionSurvProb Q>0
  book_pipenu_histset[152] = 1;		// IDWord[0]=0 tracks, wt = pionSurvProb Q>0 p>60
  book_pipenu_histset[153] = 1;		// IDWord[0]=0 tracks, wt = pionSurvProb Q>0 p>60 T>300
  book_pipenu_histset[154] = 1;		// IDWord[0]=0 tracks, wt = pionSurvProb Q>0 p>60 T>300 DnTrackTc
  book_pipenu_histset[155] = 1;		// IDWord[0]=0 tracks, wt = pionSurvProb Q>0 p>60 T>300 DnTrackTc DtTcTc
  book_pipenu_histset[156] = 1;		// IDWord[0]=0 tracks, wt = pionSurvProb Q>0 T>300
  book_pipenu_histset[157] = 1;		// IDWord[0]=0 tracks, wt = pionSurvProb Q>0 T>300 DnTrackTc
  book_pipenu_histset[158] = 1;		// IDWord[0]=0 tracks, wt = pionSurvProb Q>0 T>300 DnTrackTc DtTcTc
  book_pipenu_histset[159] = 1;          // IDWord[0]=0 tracks, wt=pionSurvProb Q=1 T>300 p>67.5 & p<70.0

  book_pipenu_histset[171] = 1;		// IDWord[1]=0 tracks, wt = pionSurvProb Q>0
  book_pipenu_histset[172] = 1;		// IDWord[1]=0 tracks, wt = pionSurvProb Q>0 p>60
  book_pipenu_histset[173] = 1;		// IDWord[1]=0 tracks, wt = pionSurvProb Q>0 p>60 T>300
  book_pipenu_histset[174] = 1;		// IDWord[1]=0 tracks, wt = pionSurvProb Q>0 p>60 T>300 DnTrackTc
  book_pipenu_histset[175] = 1;		// IDWord[1]=0 tracks, wt = pionSurvProb Q>0 p>60 T>300 DnTrackTc DtTcTc
  book_pipenu_histset[176] = 1;		// IDWord[1]=0 tracks, wt = pionSurvProb Q>0 T>300
  book_pipenu_histset[177] = 1;		// IDWord[1]=0 tracks, wt = pionSurvProb Q>0 T>300 DnTrackTc
  book_pipenu_histset[178] = 1;		// IDWord[1]=0 tracks, wt = pionSurvProb Q>0 T>300 DnTrackTc DtTcTc
  book_pipenu_histset[179] = 1;          // IDWord[1]=0 tracks, wt=pionSurvProb Q=1 T>300 p>67.5 & p<70.0
  book_pipenu_histset[200] = 1;		// all         tracks Q<0

  book_pipenu_histset[201] = 1;		// IDWord[0]=0 tracks Q<0
  book_pipenu_histset[221] = 1;		// IDWord[1]=0 tracks Q<0

  book_pipenu_histset[250] = 1;		// all  tracks, wt = pion surv prob Q<0

  book_pipenu_histset[251] = 1;		// IDWord[0]=0 tracks, wt = pion surv prob Q<0
  book_pipenu_histset[271] = 1;		// IDWord[1]=0 tracks, wt = pion surv prob Q<0
 

  for (int i=0; i<kNPipenuHistSets; i++) {
    if (book_pipenu_histset[i] == 0)                        continue;
    sprintf(folder_name,"pip_%i",i);
    fol = (TFolder*) hist_folder->FindObject(folder_name);
    if (! fol) fol = hist_folder->AddFolder(folder_name,folder_name);
    fHist.fPipenu[i] = new PipenuHist_t;
    BookPipenuHistograms(fHist.fPipenu[i],Form("Hist/%s",folder_name));
  }
//-----------------------------------------------------------------------------
// book time cluster histograms
//-----------------------------------------------------------------------------
  int book_tc_histset[kNTimeClusterHistSets];
  for (int i=0; i<kNTimeClusterHistSets; i++) book_tc_histset[i] = 0;

  book_tc_histset[0] = 1;		// all clusters

  for (int i=0; i<kNTimeClusterHistSets; i++) {
    if (book_tc_histset[i] == 0)                            continue;
    sprintf(folder_name,"tc_%i",i);
    fol = (TFolder*) hist_folder->FindObject(folder_name);
    if (! fol) fol = hist_folder->AddFolder(folder_name,folder_name);
    fHist.fTimeCluster[i] = new TimeClusterHist_t;
    BookTimeClusterHistograms(fHist.fTimeCluster[i],Form("Hist/%s",folder_name));
  }
//-----------------------------------------------------------------------------
// book event histograms
//-----------------------------------------------------------------------------
  int book_event_histset[kNEventHistSets];
  for (int i=0; i<kNEventHistSets; i++) book_event_histset[i] = 0;

  book_event_histset[ 0] = 1;		// all events
  book_event_histset[ 1] = 1;	        // events with a reconstructed track
  book_event_histset[ 2] = 1;	        // events without reconstructed tracks
  book_event_histset[ 3] = 1;	        // events with a reconstructed cluster
  book_event_histset[ 4] = 1;	        // events without reconstructed clusters
  book_event_histset[ 5] = 1;	        // events w/o reconstructed tracks, |costh|<0.4
  book_event_histset[ 6] = 1;	        // events with tracks passing "Set C" cuts
  book_event_histset[ 7] = 1;	        // events with E(cluster) > 60 MeV
  book_event_histset[ 8] = 1;	        // events with the highest energy cluster on the 1st disk
  book_event_histset[ 9] = 1;	        // events with the highest energy cluster on the 2nd disk
  book_event_histset[10] = 1;	        // events with SetC tracks 103.5 < p < 105.
  for (int i=0; i<kNEventHistSets; i++) {
    if (book_event_histset[i] != 0) {
      sprintf(folder_name,"evt_%i",i);
      fol = (TFolder*) hist_folder->FindObject(folder_name);
      if (! fol) fol = hist_folder->AddFolder(folder_name,folder_name);
      fHist.fEvent[i] = new EventHist_t;
      BookEventHistograms(fHist.fEvent[i],Form("Hist/%s",folder_name));
    }
  }
//-----------------------------------------------------------------------------
// book simp histograms
//-----------------------------------------------------------------------------
  int book_simp_histset[kNSimpHistSets];
  for (int i=0; i<kNSimpHistSets; i++) book_simp_histset[i] = 0;

  book_simp_histset[ 0] = 1;		// all events
  book_simp_histset[ 1] = 1;		// events with a      track
  book_simp_histset[ 2] = 1;		// events with a good track

  book_simp_histset[10] = 1;		// all events                     volID=3015 (last foil)
  book_simp_histset[11] = 1;		// events with a reco track and a volID=3015 (last foil)

  book_simp_histset[31] = 1;		// muons events with a      track T>300
  book_simp_histset[32] = 1;		// muons events with a      track T>300
  book_simp_histset[33] = 1;		// muons events with a      track T>300

  for (int i=0; i<kNSimpHistSets; i++) {
    if (book_simp_histset[i] != 0) {
      sprintf(folder_name,"sim_%i",i);
      fol = (TFolder*) hist_folder->FindObject(folder_name);
      if (! fol) fol = hist_folder->AddFolder(folder_name,folder_name);
      fHist.fSimp[i] = new SimpHist_t;
      BookSimpHistograms(fHist.fSimp[i],Form("Hist/%s",folder_name));
    }
  }
//-----------------------------------------------------------------------------
// book track histograms
//-----------------------------------------------------------------------------
  int book_track_histset[kNTrackHistSets];
  for (int i=0; i<kNTrackHistSets; i++) book_track_histset[i] = 0;

  book_track_histset[  0] = 1;		// all tracks
  book_track_histset[  1] = 1;		// fIDWord[0]=0 tracks
  book_track_histset[ 21] = 1;		// fIDWord[1]=0 tracks

  book_track_histset[ 50] = 1;          // all         tracks wt=pionSurvProb

  book_track_histset[ 51] = 1;          // IDWord[0]=0 tracks wt=pionSurvProb
  book_track_histset[ 71] = 1;          // IDWord[1]=0 tracks wt=pionSurvProb
  
  book_track_histset[100] = 1;		// all  tracks Q=1

  book_track_histset[101] = 1;		// IDWord[0]=0 tracks Q=1
  book_track_histset[102] = 1;		// IDWord[0]=0 tracks Q=1 p>60 
  book_track_histset[103] = 1;		// IDWord[0]=0 tracks Q=1 p>60 T>300 
  book_track_histset[104] = 1;		// IDWord[0]=0 tracks Q=1 p>60 T>300 DnTrackTc 
  book_track_histset[105] = 1;		// IDWord[0]=0 tracks Q=1 p>60 T>300 DnTrackTc DtTcTc
  book_track_histset[106] = 1;		// IDWord[0]=0 tracks Q=1 T>300 
  book_track_histset[107] = 1;		// IDWord[0]=0 tracks Q=1 T>300 DnTrackTc 
  book_track_histset[108] = 1;		// IDWord[0]=0 tracks Q=1 T>300 DnTrackTc DtTcTc

  book_track_histset[121] = 1;		// IDWord[1]=0 tracks Q=1
  book_track_histset[122] = 1;		// IDWord[1]=0 tracks Q=1 p>60 
  book_track_histset[123] = 1;		// IDWord[1]=0 tracks Q=1 p>60 T>300 
  book_track_histset[124] = 1;		// IDWord[1]=0 tracks Q=1 p>60 T>300 DnTrackTc 
  book_track_histset[125] = 1;		// IDWord[1]=0 tracks Q=1 p>60 T>300 DnTrackTc DtTcTc
  book_track_histset[126] = 1;		// IDWord[1]=0 tracks Q=1 T>300 
  book_track_histset[127] = 1;		// IDWord[1]=0 tracks Q=1 T>300 DnTrackTc 
  book_track_histset[128] = 1;		// IDWord[1]=0 tracks Q=1 T>300 DnTrackTc DtTcTc

  book_track_histset[150] = 1;          // all         tracks          wt=pionSurvProb Q=1

  book_track_histset[151] = 1;          // IDWord[0]=0 tracks, wt=pionSurvProb Q=1
  book_track_histset[152] = 1;          // IDWord[0]=0 tracks, wt=pionSurvProb Q=1 p>60 
  book_track_histset[153] = 1;          // IDWord[0]=0 tracks, wt=pionSurvProb Q=1 p>60 T>300
  book_track_histset[154] = 1;          // IDWord[0]=0 tracks, wt=pionSurvProb Q=1 p>60 T>300 DnTrackTC 
  book_track_histset[155] = 1;          // IDWord[0]=0 tracks, wt=pionSurvProb Q=1 p>60 T>300 DnTrackTC DtTcTc
  book_track_histset[156] = 1;          // IDWord[0]=0 tracks, wt=pionSurvProb Q=1 T>300
  book_track_histset[157] = 1;          // IDWord[0]=0 tracks, wt=pionSurvProb Q=1 T>300 DnTrackTC 
  book_track_histset[158] = 1;          // IDWord[0]=0 tracks, wt=pionSurvProb Q=1 T>300 DnTrackTC DtTcTc
  book_track_histset[159] = 1;          // IDWord[0]=0 tracks, wt=pionSurvProb Q=1 T>300 p>67.5 & p<70.0
  
  book_track_histset[171] = 1;          // IDWord[1]=0 tracks, wt=pionSurvProb Q=1
  book_track_histset[172] = 1;          // IDWord[1]=0 tracks, wt=pionSurvProb Q=1 p>60 
  book_track_histset[173] = 1;          // IDWord[1]=0 tracks, wt=pionSurvProb Q=1 p>60 T>300
  book_track_histset[174] = 1;          // IDWord[1]=0 tracks, wt=pionSurvProb Q=1 p>60 T>300 DnTrackTC 
  book_track_histset[175] = 1;          // IDWord[1]=0 tracks, wt=pionSurvProb Q=1 p>60 T>300 DnTrackTC DtTcTc
  book_track_histset[176] = 1;          // IDWord[1]=0 tracks, wt=pionSurvProb Q=1 T>300
  book_track_histset[177] = 1;          // IDWord[1]=0 tracks, wt=pionSurvProb Q=1 T>300 DnTrackTC 
  book_track_histset[178] = 1;          // IDWord[1]=0 tracks, wt=pionSurvProb Q=1 T>300 DnTrackTC DtTcTc
  book_track_histset[179] = 1;          // IDWord[1]=0 tracks, wt=pionSurvProb Q=1 T>300 p>67.5 & p<70.0
  book_track_histset[200] = 1;		// all  tracks Q=-1
  book_track_histset[201] = 1;		// IDWord[0]=0 tracks Q=-1

  book_track_histset[221] = 1;		// IDWord[1]=0 tracks Q=-1

  book_track_histset[250] = 1;          // all         tracks wt=pionSurvPro Q=-1
  book_track_histset[251] = 1;          // IDWord[0]=0 tracks wt=pionSurvPro Q=-1

  book_track_histset[271] = 1;		// IDWord[1]=0 tracks wt=pionSurvPro Q=-1
  for (int i=0; i<kNTrackHistSets; i++) {
    if (book_track_histset[i] != 0) {
      sprintf(folder_name,"trk_%i",i);
      fol = (TFolder*) hist_folder->FindObject(folder_name);
      if (! fol) fol = hist_folder->AddFolder(folder_name,folder_name);
      fHist.fTrack[i] = new TrackHist_t;
      BookTrackHistograms(fHist.fTrack[i],Form("Hist/%s",folder_name));
    }
  }
//-----------------------------------------------------------------------------
// book track ID histograms
//-----------------------------------------------------------------------------
  int book_track_id_histset[kNTrackIDHistSets];
  for (int i=0; i<kNTrackIDHistSets; i++) book_track_id_histset[i] = 0;

  book_track_id_histset[  0] = 1;		// all tracks

  for (int i=0; i<kNTrackIDHistSets; i++) {
    if (book_track_id_histset[i] == 0)                      continue;
    sprintf(folder_name,"tid_%i",i);
    fol = (TFolder*) hist_folder->FindObject(folder_name);
    if (! fol) fol = hist_folder->AddFolder(folder_name,folder_name);
    fHist.fTrackID[i] = new TStnTrackID::Hist_t;
    BookTrackIDHistograms(fHist.fTrackID[i],Form("Hist/%s",folder_name));
  }

//-----------------------------------------------------------------------------
// book helix histograms
//-----------------------------------------------------------------------------
  int book_helix_histset[kNHelixHistSets];
  for (int i=0; i<kNHelixHistSets; i++) book_helix_histset[i] = 0;

  book_helix_histset[ 0] = 1;		// all events
  book_helix_histset[ 1] = 1;		// events with N(reconstructed tracks) > 0
  book_helix_histset[ 2] = 1;		// helices with a TrackSeed

  for (int i=0; i<kNHelixHistSets; i++) {
    if (book_helix_histset[i] != 0) {
      sprintf(folder_name,"hel_%i",i);
      fol = (TFolder*) hist_folder->FindObject(folder_name);
      if (! fol) fol = hist_folder->AddFolder(folder_name,folder_name);
      fHist.fHelix[i] = new HelixHist_t;
      BookHelixHistograms(fHist.fHelix[i],Form("Hist/%s",folder_name));
    }
  }
} //BookHistogram ends?? confirm it.

//-----------------------------------------------------------------------------
void TPipenuAnaModule::FillPipenuHistograms    (PipenuHist_t*  Hist, 
                                                   TStnTrack*     Trk, 
                                                   TrackPar_t*    Tp, 
                                                   SimPar_t*      SimPar,
                                                   double         Weight) {
  //calculate the 

  Hist->fPVsT0->Fill(Trk->fT0,Tp->fP,Weight);
}
//-----------------------------------------------------------------------------
void    TPipenuAnaModule::FillTimeClusterHistograms(TimeClusterHist_t* Hist  , 
                                                    TStnTimeCluster*   Tc    ,
                                                    double             Weight) {
  Hist->fNsh->Fill  (Tc->NHits()     ,Weight);
  Hist->fNch->Fill  (Tc->NComboHits(),Weight);
  Hist->fT0->Fill   (Tc->T0()        ,Weight);
  Hist->fT0Err->Fill(Tc->T0Err()     ,Weight);
}
//-----------------------------------------------------------------------------
void TPipenuAnaModule::FillEventHistograms (HistBase_t* Hist, EventPar_t* Evp) {
  EventHist_t* hist = (EventHist_t*) Hist;
    double            cos_th(-2), xv(-1.e6), yv(-1.e6), rv(-1.e6), zv(-1.e6), p(-1.), dio_wt(-1.);
  //  double            e, m, r;
  TLorentzVector    mom;

  if (Evp->fSimp) {
    mom = *Evp->fSimp->StartMom();
    p      = mom.P();
    cos_th = mom.Pz()/p;
    xv     = Evp->fSimp->StartPos()->X()+3904.;
    yv     = Evp->fSimp->StartPos()->Y();
    rv     = sqrt(xv*xv+yv*yv);
    zv     = Evp->fSimp->StartPos()->Z();
    dio_wt = TStntuple::DioWeightAl(p);
  }

  hist->fEventWeight[0]->Fill(fEventWeight);
  hist->fEventWeight[1]->Fill(log10(fEventWeight));
  hist->fEventE        ->Fill(Evp->fPartE  , fEventWeight);
  hist->fInstLumi[0]   ->Fill(Evp->fInstLum, fEventWeight);

  if (fBatchMode == 1) {
    hist->fInstLumi[1]->Fill(Evp->fInstLum, fEventWeight/Evp->fOneBatchWeight);
    hist->fInstLumi[2]->Fill(Evp->fInstLum, fEventWeight/Evp->fOneBatchWeight*Evp->fTwoBatchWeight);
  } else if(fBatchMode == 2) {
    hist->fInstLumi[1]->Fill(Evp->fInstLum, fEventWeight/Evp->fTwoBatchWeight);
    hist->fInstLumi[2]->Fill(Evp->fInstLum, fEventWeight/Evp->fTwoBatchWeight*Evp->fOneBatchWeight);
  }

  hist->fBatchWeight[0]->Fill(log10(Evp->fOneBatchWeight));
  hist->fBatchWeight[1]->Fill(log10(Evp->fTwoBatchWeight));

  hist->fMcMom->Fill(p);
  hist->fDioMom->Fill(p,dio_wt);
  hist->fMcCosTh->Fill(cos_th);
  hist->fRv->Fill(rv);
  hist->fZv->Fill(zv);

  // hist->fNClusters->Fill(fNClusters);
  hist->fNTracks[0]->Fill  (Evp->fNTracks[0]);
  hist->fNTracks[1]->Fill  (Evp->fNTracks[1]);
  hist->fNShTot[0]->Fill(Evp->fNStrawHits);
  hist->fNShTot[1]->Fill(Evp->fNStrawHits);

  hist->fNChTot[0]->Fill(Evp->fNComboHits);
  hist->fNChTot[1]->Fill(Evp->fNComboHits);

  double emax   = -1;
  double dt     = 9999.;

  hist->fDtClT->Fill(dt);
  hist->fEClMax->Fill(emax);

  // hist->fNHyp->Fill(fNHyp);
  // hist->fBestHyp[0]->Fill(fBestHyp[0]);
  // hist->fBestHyp[1]->Fill(fBestHyp[1]);
  hist->fNGenp->Fill(Evp->fNGenp);

  hist->fNCrvClusters->Fill(Evp->fNCrvClusters);
  hist->fNCrvCoincidences[0]->Fill(Evp->fNCrvCoincidences);
  hist->fNCrvCoincidences[1]->Fill(Evp->fNCrvCoincidences);
  hist->fNCrvPulses[0]->Fill(Evp->fNCrvPulses);
  hist->fNCrvPulses[1]->Fill(Evp->fNCrvPulses);
  
} // FillEventHistograms ends
void TPipenuAnaModule::FillSimpHistograms (HistBase_t* Hist, TSimParticle* Simp, SimpData_t* Sd, double Weight) {
  SimpHist_t* hist = (SimpHist_t*) Hist;

  int stage  = Simp->SimStage();

  hist->fStage->Fill(stage,Weight);

  hist->fPdgCode[0]->Fill(Simp->fPdgCode,Weight);
  hist->fPdgCode[1]->Fill(Simp->fPdgCode,Weight);

  //if (Sd->fParent) {
    //    hist->fParentMom->Fill(Sd->fParent->EndMom()->P(),Weight);
    //hist->fParentPDG->Fill(Sd->fParent->PDGCode(),Weight);
    // }
  hist->fGeneratorID->Fill(Simp->fGeneratorID,Weight);

  hist->fStartVolumeID->Fill(Simp->fStartVolumeIndex,Weight);
  hist->fEndVolumeID->Fill(Simp->fEndVolumeIndex,Weight);

  float xe = Simp->EndPos()->X()+3904.;
  float ye = Simp->EndPos()->Y();
  float ze = Simp->EndPos()->Z();
  float te = Simp->EndPos()->T();

  hist->fStartTime->Fill(Simp->StartPos()->T(),Weight);
  hist->fEndTime->Fill(te,Weight);
  float dt = (Simp->EndProperTime()-Simp->StartProperTime())*Sd->fTau;
  hist->fStageDt->Fill(dt,Weight);

  float p = Simp->StartMom()->P();
  hist->fStartMom[0]->Fill(p,Weight);
  hist->fStartMom[1]->Fill(p,Weight);

  float pe = Simp->EndMom()->P();
  hist->fEndMom[0]->Fill(pe,Weight);
  hist->fEndMom[1]->Fill(pe,Weight);

  int nsh = Simp->NStrawHits();
  hist->fNStrawHits->Fill(nsh,Weight);

  hist->fMomTargetEnd->Fill(Simp->fMomTargetEnd,Weight);
  hist->fMomTrackerFront->Fill(Simp->fMomTrackerFront,Weight);

  double costh = Simp->StartMom()->Z()/Simp->StartMom()->P();
  hist->fCosTh->Fill(costh,Weight);
  hist->fZStart->Fill(Simp->StartPos()->Z(),Weight);
  hist->fZEnd->Fill(ze,Weight);

  hist->fNshVsCosTh->Fill(costh,nsh,Weight);

  hist->fYVsX->Fill(xe,ye,Weight);
  hist->fXEndVsZEnd->Fill(ze,xe,Weight);
  hist->fYcVsZEnd->Fill(ze,Sd->fY0,Weight);
  hist->fPVD9VsZEnd->Fill(ze,Sd->fPVD9,Weight);
//-----------------------------------------------------------------------------
// looks like something to do with the stopping target - but this is still 34 foils..
//-----------------------------------------------------------------------------
  if (Simp->fEndVolumeIndex == 2480) hist->fYVsX_2480->Fill(xe,ye,Weight);
  if (Simp->fEndVolumeIndex == 2513) hist->fYVsX_2513->Fill(xe,ye,Weight);

  float cos_th = Simp->StartMom()->Pz()/p;
  hist->fCosThVsMom[0]->Fill(p,cos_th,Weight);
  hist->fCosThVsMom[1]->Fill(p,cos_th,Weight);

  //  hist->fCosThVsMomPV->Fill(fPbarMomPV,fPbarCosThPV,Weight);
  
} //fillsimpHistogram ends

void TPipenuAnaModule::FillTrackHistograms(TrackHist_t* Hist, TStnTrack* Track, 
				     TrackPar_t* Tp, SimPar_t* SimPar, double Weight) {

  TLorentzVector  mom;
     // std::cout<<"----------------------------------------------"<<std::endl;
     //  std::cout<<"Tp While getting:"<<std::endl;
     //  std::cout<<"----------------------------------------------"<<std::endl;
     //  std::cout<<"fP: "<<Tp->fP<<" idWord: "<<Tp->fIDWord[0]<<std::endl;
  // std::cout<<"fP From Tp: "<<Tp->fP<<" fStage from Simp: "<<SimPar->fStage<<std::endl;
					// Tp->fP - corrected momentum, fP0 and fP2 - not corrected
  Hist->fP[0]->Fill (Tp->fP,Weight);
  Hist->fP[1]->Fill (Tp->fP,Weight);
  Hist->fP[2]->Fill (Tp->fP,Weight);
					// fP0: uncorrected momentum in the first point,  fP2 - in the last
  Hist->fP0->  Fill (Track->fP0,Weight);
  Hist->fP2->  Fill (Track->fP2,Weight);

  Hist->fPDio->  Fill (Tp->fP,Tp->fDioLLWt);     // fixed for debugging

  Hist->fFitMomErr->Fill(Track->fFitMomErr,Weight);

  Hist->fPt    ->Fill(Track->fPt    , Weight);
  Hist->fPFront->Fill(Track->fPFront, Weight);
  Hist->fPStOut->Fill(Track->fPStOut, Weight);
  Hist->fPVsGenE->Fill(fEleE, Track->fP, Weight);
//-----------------------------------------------------------------------------
// dp: Tracker-only resolution
//-----------------------------------------------------------------------------
  Hist->fDpFront ->Fill(Tp->fDpF   , Weight);
  Hist->fXDpF    ->Fill(Tp->fXDpF  , Weight);
  Hist->fDpFront0->Fill(Tp->fDp0   , Weight);
  Hist->fDpFront2->Fill(Tp->fDp2   , Weight);
  Hist->fDpFSt   ->Fill(Tp->fDpFSt , Weight);
  Hist->fDpFVsZ1 ->Fill(Track->fZ1 ,Tp->fDpF, Weight);

  Hist->fCosTh->Fill(Track->Momentum()->CosTheta(), Weight);
  Hist->fChi2->Fill (Track->fChi2, Weight);
  Hist->fChi2Dof->Fill(Track->Chi2Dof(), Weight);

  float na  = Track->NActive();
  float dna = Track->NHits()-na;

  Hist->fNActive->Fill(na, Weight);
  Hist->fNaFract->Fill(na/(Track->NHits()+0.), Weight);
  Hist->fDNa->Fill(dna, Weight);

  if (Tp->fTimeCluster != nullptr) Hist->fNtch->Fill(Tp->fTimeCluster->NHits(),Weight);
  else                             Hist->fNtch->Fill(-1,Weight);
  Hist->fDtTc->Fill(Tp->fDtTrackTc,Weight);

  Hist->fNWrong->Fill(Track->NWrong(), Weight);

  float nd = Track->NDoublets();

  float nad = Track->NDoubletsAct();
  Hist->fNDoublets->Fill(nd, Weight);
  Hist->fNadOverNd->Fill(nad/nd, Weight);
  Hist->fNOSD->Fill(Track->NOSDoublets(), Weight);
  Hist->fNSSD->Fill(Track->NSSDoublets(), Weight);
  Hist->fNdOverNa->Fill(nd/na, Weight);
  Hist->fNosdOverNa->Fill(Track->NOSDoublets()/na , Weight);
  Hist->fNssdOverNa->Fill(Track->NSSDoublets()/na , Weight);
  Hist->fNZeroAmb  ->Fill(Track->NHitsAmbZero()   , Weight);
  Hist->fNzaOverNa ->Fill(Track->NHitsAmbZero()/na, Weight);

  int nma = Track->NMatActive();

  Hist->fNMatActive->Fill(nma   , Weight);
  Hist->fNmaOverNa ->Fill(nma/na, Weight);

  Hist->fNBend->Fill(Track->NBend(), Weight);

  Hist->fT0->Fill(Track->fT0, Weight);
  Hist->fT0Err->Fill(Track->fT0Err, Weight);
  Hist->fQ->Fill(Track->fCharge, Weight);
//-----------------------------------------------------------------------------
// the two histograms just have different limits
//-----------------------------------------------------------------------------
  Hist->fFitCons[0]->Fill(Track->fFitCons, Weight);
  Hist->fFitCons[1]->Fill(Track->fFitCons, Weight);

  Hist->fD0->Fill(Track->fD0, Weight);
  Hist->fZ0->Fill(Track->fZ0, Weight);
  Hist->fTanDip->Fill(Track->fTanDip, Weight);
  Hist->fDtZ0->Fill(Tp->fDtZ0, Weight);
  Hist->fXtZ0->Fill(Tp->fXtZ0, Weight);
  Hist->fRMax->Fill(Track->RMax(), Weight);
  
  Hist->fAlgMask->Fill(Track->AlgMask(), Weight);

  Hist->fChi2Tcm->Fill(Tp->fChi2Tcm, Weight);
  Hist->fChi2XY ->Fill(Tp->fChi2XY , Weight);
  Hist->fChi2T  ->Fill(Tp->fChi2T  , Weight);

  Hist->fDt->Fill(Tp->fDt, Weight);
  Hist->fDx->Fill(Tp->fDx, Weight);
  Hist->fDy->Fill(Tp->fDy, Weight);
  Hist->fDz->Fill(Tp->fDz, Weight);
  Hist->fDu->Fill(Tp->fDu, Weight);
  Hist->fDv->Fill(Tp->fDv, Weight);

  Hist->fPath->Fill(Tp->fPath, Weight);
  Hist->fECl ->Fill(Tp->fEcl , Weight);
  Hist->fSeedFr->Fill(Tp->fSeedFr, Weight);
  Hist->fNCrystals->Fill(Tp->fNCrystals, Weight);
//-----------------------------------------------------------------------------
// assume muon hypothesis
//-----------------------------------------------------------------------------
  double    ekin(-1.);
  if (SimPar->fParticle) {
    double p, m;
    p    = Tp->fP;
    m    = 105.658; // muon mass, in MeV
    ekin = sqrt(p*p+m*m)-m;
  }

  Hist->fEClEKin->Fill(Tp->fEcl/ekin, Weight);
  Hist->fEp     ->Fill(Tp->fEp      , Weight);
  Hist->fDrDzCal->Fill(Tp->fDrDzCal , Weight);
  Hist->fDtClZ0 ->Fill(Tp->fDtClZ0  , Weight);

  Hist->fDtClZ0VsECl->Fill(Tp->fEcl,Tp->fDtClZ0, Weight);
  Hist->fDtClZ0VsP  ->Fill(Tp->fP  ,Tp->fDtClZ0, Weight);

  Hist->fFConsVsNActive->Fill(Track->NActive(),Track->fFitCons, Weight);
//-----------------------------------------------------------------------------
// MVA variables
//-----------------------------------------------------------------------------
  Hist->fDaveTrkQual->Fill(Track->DaveTrkQual(), Weight);
  Hist->fTrqMvaOut->Fill(Tp->fTrqMvaOut[1], Weight);
//-----------------------------------------------------------------------------
// 2D momentum() vs time (tracker center) histogram for sensitivity calculation
//-----------------------------------------------------------------------------
  Hist->fPVsTime->Fill(Tp->fP, Track->fT0, Weight);

  Hist->fDtCRV   ->Fill(Tp->fDtCRV , Weight);
  Hist->fDtCRV2  ->Fill(Tp->fDtCRV2, Weight);
  Hist->fZVsDtCRV->Fill(Tp->fZCRV  , Tp->fDtCRV, Weight);
//-----------------------------------------------------------------------------
// TrkCaloHit histograms
//-----------------------------------------------------------------------------
  Hist->fTchTime->Fill(Tp->fTchTime, Weight);
  Hist->fTchPath->Fill(Tp->fTchPath, Weight);
  Hist->fTchDx  ->Fill(Tp->fTchDx  , Weight);
  Hist->fTchDy  ->Fill(Tp->fTchDy  , Weight);
  Hist->fTchDz  ->Fill(Tp->fTchDz  , Weight);
  Hist->fTchDr  ->Fill(Tp->fTchDr  , Weight);
  Hist->fTchDt  ->Fill(Tp->fTchDt  , Weight);

  Hist->fEpVsPath ->Fill(Tp->fPath ,Tp->fEp   ,Weight);
  Hist->fEpVsTchDz->Fill(Tp->fTchDz,Tp->fEp   ,Weight);
  Hist->fTchDtVsDz->Fill(Tp->fTchDz,Tp->fTchDt,Weight);
//-----------------------------------------------------------------------------
// MVA-based PID
//-----------------------------------------------------------------------------
  Hist->fPidMvaOut->Fill(Tp->fPidMvaOut[0], Weight);

} //FillTrackHistogram ends

//-----------------------------------------------------------------------------
  void TPipenuAnaModule::FillHelixHistograms(HistBase_t* Hist, TStnHelix* Helix, HelixPar_t* Help, double Weight) {
//   char name [200];
//   char title[200];

    HelixHist_t* hist = (HelixHist_t*) Hist;

    hist->fBestAlg->Fill(Helix->BestAlg(),Weight);
    hist->fChi2XY->Fill(Helix->Chi2XY(),Weight);
    hist->fChi2ZPhi->Fill(Helix->Chi2ZPhi(),Weight);

    float sinth = Helix->Pt()/Helix->P();
    hist->fSinTh->Fill(sinth,Weight);

    hist->fD0->Fill(Helix->D0(),Weight);

    hist->fHelicity->Fill(Helix->Helicity(),Weight);
    hist->fLambda->Fill(Helix->Lambda(),Weight);
    hist->fNCh->Fill(Helix->fNComboHits,Weight);
    hist->fNSh->Fill(Helix->fNHits,Weight);
    hist->fP->Fill(Helix->P(),Weight);
    hist->fRadius->Fill(Helix->Radius(),Weight);
  } //FillHelixHistograms ends

//-----------------------------------------------------------------------------
// fill efficiency histograms : need 10 histogram sets
// pitch = 1./tan(dip)
//-----------------------------------------------------------------------------
void TPipenuAnaModule::FillEfficiencyHistograms(TStnTrackBlock*  TrackBlock, 
                                                TStnTrackID*     TrackID   , 
                                                int              HistSet   ) {
  // having MC truth is a must for calculating efficiency!
  if (fEvtPar.fSimp == nullptr) return;

  if (fEvtPar.fSimp->NStrawHits() >= 20) {
    FillEventHistograms(fHist.fEvent[HistSet],&fEvtPar);

    if (fEvtPar.fSimp->fMomTrackerFront > 100.) {
      FillEventHistograms(fHist.fEvent[HistSet+1],&fEvtPar);

      TVector3 vdmom;

      if (fSimPar.fTFront != NULL) {
//-----------------------------------------------------------------------------
// regular case
//-----------------------------------------------------------------------------
	vdmom.SetXYZ(fSimPar.fTFront->Mom()->X(),
		     fSimPar.fTFront->Mom()->Y(),		      
		     fSimPar.fTFront->Mom()->Z());
      }
      else {
//-----------------------------------------------------------------------------
// pathological case when an upstream simulated MC particle is reconstructed 
// as the downstream one and there is no hit... efficiency doens't make sense
// just make sure the code doesnt' crash
//-----------------------------------------------------------------------------
	vdmom.SetXYZ(fSimPar.fParticle->fStartMom.X(),
		     fSimPar.fParticle->fStartMom.Y(),
		     fSimPar.fParticle->fStartMom.Z());
      }

      float ce_pitch  = vdmom.Pt()/vdmom.Pz();
      float min_pitch = 1./TrackID->MaxTanDip();
      float max_pitch = 1./TrackID->MinTanDip();

      if ((min_pitch < ce_pitch) && (ce_pitch < max_pitch)) {
	FillEventHistograms(fHist.fEvent[HistSet+2],&fEvtPar);
	  
	if (TrackBlock->NTracks() > 0) {
	  TStnTrack* track = TrackBlock->Track(0);
	  int id_word      = TrackID->IDWord(track);

	  FillEventHistograms(fHist.fEvent[HistSet+3],&fEvtPar);
	  
	  if ((id_word & TStnTrackID::kTrkQualBit) == 0) {
	    FillEventHistograms(fHist.fEvent[HistSet+4],&fEvtPar);
	    
	    if ((id_word & TStnTrackID::kT0Bit) == 0) {
	      FillEventHistograms(fHist.fEvent[HistSet+5],&fEvtPar);
	      
	      if ((id_word & TStnTrackID::kTanDipBit) == 0) {
		FillEventHistograms(fHist.fEvent[HistSet+6],&fEvtPar);
		
		if (((id_word & TStnTrackID::kD0Bit  ) == 0) && 
		    ((id_word & TStnTrackID::kRMaxBit) == 0)    ) {
		  
		  FillEventHistograms(fHist.fEvent[HistSet+7],&fEvtPar);
		  
		  if ((id_word & TStnTrackID::kTanDipBit) == 0) {
		    FillEventHistograms(fHist.fEvent[HistSet+8],&fEvtPar);

		    if ((103.5 < track->fP) && (track->fP < 105)) {
		      FillEventHistograms(fHist.fEvent[HistSet+9],&fEvtPar);
		    }
		  }
		}
	      }
	    }
	  }
	}
      }
    }
  }
}
// fill effieciency hist ends-------------
void TPipenuAnaModule::FillHistograms() {

  double       cos_th (-2.); //,  cl_e(-1.);
  int          disk_id(-1);
//-----------------------------------------------------------------------------
// event histograms
//-----------------------------------------------------------------------------
  FillEventHistograms(fHist.fEvent[0],&fEvtPar); //event hist-0
  if (fEvtPar.fNTracks[0]> 0) FillEventHistograms(fHist.fEvent[1],&fEvtPar); //event hist-1
  else                        FillEventHistograms(fHist.fEvent[2],&fEvtPar); //event hist-2

  if (fNClusters > 0) FillEventHistograms(fHist.fEvent[3],&fEvtPar); //event hist-3
  else                FillEventHistograms(fHist.fEvent[4],&fEvtPar); //event hist-4

  if ((fEvtPar.fNTracks[0] == 0) && (fabs(cos_th) < 0.4)) {
    FillEventHistograms(fHist.fEvent[5],&fEvtPar); //event hist-5
  }

  if (fEvtPar.fNGoodTracks[0] > 0) {
    FillEventHistograms(fHist.fEvent[6],&fEvtPar); //event hist-6

    TLorentzVector    mom(1.,0.,0.,0);
    
    if (fSimPar.fParticle) mom = *fSimPar.fParticle->StartMom();

    double p, cos_th;

    p      = mom.P();
    cos_th = mom.Pz()/p;

    if (GetDebugBit(31) && (cos_th > 0.8)) {
      GetHeaderBlock()->Print(Form(" bit:031 cos_th = %10.3f p = %10.3f ntrk = %5i",
				   cos_th, p, fEvtPar.fNTracks[0]));
    }
  }

  if      (disk_id == 0) FillEventHistograms(fHist.fEvent[8],&fEvtPar); //event hist-8
  else if (disk_id == 1) FillEventHistograms(fHist.fEvent[9],&fEvtPar); //event hist-9
//-----------------------------------------------------------------------------
// EVT_10: events with SetC tracks 103.5 < p < 105.0
//-----------------------------------------------------------------------------
  if (fEvtPar.fNGoodTracks[0] > 0) {
    TStnTrack* trk = fTrackBlock->Track(0);
    if ((103.5 < trk->fP) && (trk->fP < 105.)) {
      FillEventHistograms(fHist.fEvent[10],&fEvtPar);  //event hist-10
    }
  }
//-----------------------------------------------------------------------------
// helix histograms
// HEL_0: all
// HEL_1: helices in events with reconstructed tracks
//-----------------------------------------------------------------------------
  int nhel = fHelixBlock->NHelices();
  for (int i=0; i<nhel; i++) {
    TStnHelix*  hel = fHelixBlock->Helix(i);
    HelixPar_t* hp  = fHelixPar+i;
    FillHelixHistograms(fHist.fHelix[0],hel,hp);
    if (fEvtPar.fNTracks[0]  >  0) FillHelixHistograms(fHist.fHelix[1],hel,hp);
    if (hel->fTrackSeedIndex >= 0) FillHelixHistograms(fHist.fHelix[2],hel,hp);
  }
//-----------------------------------------------------------------------------
// SIMP histograms
//-----------------------------------------------------------------------------
  if (fEvtPar.fSimp) {
    SimpData_t sd;
    sd.fParent  = nullptr;
    sd.fStepVD9 = nullptr;

    TSimParticle* mom = fSimpBlock->Particle(fEvtPar.fSimp->ParentID());

    FillSimpHistograms(fHist.fSimp[0],fEvtPar.fSimp,&sd);
    if (fEvtPar.fNTracks[0] > 0) {
      FillSimpHistograms(fHist.fSimp[ 1],fEvtPar.fSimp,&sd);
      FillSimpHistograms(fHist.fSimp[31],mom,&sd);
      if (fEvtPar.fNGoodTracks[0] > 0) {
        FillSimpHistograms(fHist.fSimp[ 2],fEvtPar.fSimp,&sd);
        FillSimpHistograms(fHist.fSimp[32],mom,&sd);
      }

      if (fN300 > 0) {
        FillSimpHistograms(fHist.fSimp[33],mom,&sd);
      }
    }

    if (fEvtPar.fSimp->StartVolumeIndex() == 3015) { 
//-----------------------------------------------------------------------------
// last foil
//-----------------------------------------------------------------------------
      FillSimpHistograms(fHist.fSimp[10],fEvtPar.fSimp,&sd);
      if (fEvtPar.fNTracks[0] > 0) FillSimpHistograms(fHist.fSimp[11],fEvtPar.fSimp,&sd);
    }
  }
//-----------------------------------------------------------------------------
// track/pipenu histograms, fill them only for the downstream e- hypothesis
//-----------------------------------------------------------------------------
  TStnTrack*   trk;
  TrackPar_t*  tp;

  for (int i=0; i<fEvtPar.fNTracks[0]; ++i ) {
    trk       = fTrackBlock->Track(i);
    tp        = fTrackPar+i;
    //    std::cout<<tp->fP<<" : Momentum"<<std::endl;
    fTrackID[0]->FillHistograms(fHist.fTrackID[0],trk);

    //tp->fDnTrackTc = tp->fTimeCluster->NHits()-trk->NActive();
    //tp->fDtTrackTc = tp->fTimeCluster->NHits()-trk->NActive();
//-----------------------------------------------------------------------------
// find closest time cluster 
//-----------------------------------------------------------------------------
    float dt_tc_min = 1.e6;   // signed !!!

    for (int itc=0; itc<fEvtPar.fNTimeClusters; itc++) {
      TStnTimeCluster* tc = fTimeClusterBlock->TimeCluster(itc);
      if (tc == tp->fTimeCluster)                 continue;
      float dt = tp->fTimeCluster->T0()-tc->T0();
      if (fabs(dt) < fabs(dt_tc_min)) {
        dt_tc_min = dt;
      }
    }

    tp->fDtTcTc = dt_tc_min;

    FillTrackHistograms (fHist.fTrack [0],trk,tp,&fSimPar); //track hist-0
    FillPipenuHistograms(fHist.fPipenu[0],trk,tp,&fSimPar); //pipenu hist-0
    
if (trk->Charge() > 0) {
      FillTrackHistograms (fHist.fTrack [100],trk,tp,&fSimPar); //track hist-100
      FillPipenuHistograms(fHist.fPipenu[100],trk,tp,&fSimPar); //pipenu hist-100
    }
    else {
      FillTrackHistograms (fHist.fTrack [200],trk,tp,&fSimPar);  //track hist-200
      FillPipenuHistograms(fHist.fPipenu[200],trk,tp,&fSimPar);  //pipenu hist-200
    }
// based on trackID selection begins----------------------------------------

    if (tp->fIDWord[fBestID] == 0) {
					// track passes the quality selection

      FillTrackHistograms (fHist.fTrack [  1],trk,tp,&fSimPar);
      FillPipenuHistograms(fHist.fPipenu[  1],trk,tp,&fSimPar);
      if (trk->Charge() > 0) {
        FillTrackHistograms (fHist.fTrack [101],trk,tp,&fSimPar);
        FillPipenuHistograms(fHist.fPipenu[101],trk,tp,&fSimPar);
//-----------------------------------------------------------------------------
// at this point, apply the analysis-specific cuts
// 1. require the number of track hits to be consistent with the number of hits 
// 2. require no close time clusters
// TRK_102: P>60
// TRK_103: P>60 T>300
// TRK_104: P>60 T>300 dNTrackTC < fDnMax
// TRK_105: P>60 T>300 dNTrackTC < fDnMax abs(fDtTcTc) > 100
//-----------------------------------------------------------------------------
        if (tp->fP > 60) {
          FillTrackHistograms (fHist.fTrack [102],trk,tp,&fSimPar);
          FillPipenuHistograms(fHist.fPipenu[102],trk,tp,&fSimPar);
          if (trk->T0() > 300) {
            FillTrackHistograms (fHist.fTrack [103],trk,tp,&fSimPar);
            FillPipenuHistograms(fHist.fPipenu[103],trk,tp,&fSimPar);
            if (tp->fDnTrackTc <= fDnMax) { 
              FillTrackHistograms (fHist.fTrack [104],trk,tp,&fSimPar);
              FillPipenuHistograms(fHist.fPipenu[104],trk,tp,&fSimPar);
              if (fabs(tp->fDtTcTc) > 100) {
                fEventPassedSelections = 1;
                FillTrackHistograms (fHist.fTrack [105],trk,tp,&fSimPar);
                FillPipenuHistograms(fHist.fPipenu[105],trk,tp,&fSimPar);
              }
            }
          }
        }
//-----------------------------------------------------------------------------
// the same, but w/o p>60 MeV/c cut
//-----------------------------------------------------------------------------
        if (trk->T0() > 300) {
          FillTrackHistograms (fHist.fTrack [106],trk,tp,&fSimPar);
          FillPipenuHistograms(fHist.fPipenu[106],trk,tp,&fSimPar);
          if (tp->fDnTrackTc <= fDnMax) { 
            FillTrackHistograms (fHist.fTrack [107],trk,tp,&fSimPar);
            FillPipenuHistograms(fHist.fPipenu[107],trk,tp,&fSimPar);
            if (fabs(tp->fDtTcTc) > 100) {
              fEventPassedSelections = 1;
              FillTrackHistograms (fHist.fTrack [108],trk,tp,&fSimPar);
              FillPipenuHistograms(fHist.fPipenu[108],trk,tp,&fSimPar);
            }
          }
        }
      }
      else {
        FillTrackHistograms (fHist.fTrack [201],trk,tp,&fSimPar);
        FillPipenuHistograms(fHist.fPipenu[201],trk,tp,&fSimPar);
      }
    }
// based on trackID selection ends----------------------------------------
// tight cut begins ------------------------------------------------------
//-----------------------------------------------------------------------------
// tight ID
//-----------------------------------------------------------------------------
    if (tp->fIDWord[1] == 0) {
					// track passes the quality selection

      FillTrackHistograms (fHist.fTrack [ 21],trk,tp,&fSimPar);
      FillPipenuHistograms(fHist.fPipenu[ 21],trk,tp,&fSimPar);
      if (trk->Charge() > 0) {
        FillTrackHistograms (fHist.fTrack [121],trk,tp,&fSimPar);
        FillPipenuHistograms(fHist.fPipenu[121],trk,tp,&fSimPar);
//-----------------------------------------------------------------------------
// at this point, apply the analysis-specific cuts
// 1. require the number of track hits to be consistent with the number of hits 
// 2. require no close time clusters
// TRK_102: P>60
// TRK_103: P>60 T>300
// TRK_104: P>60 T>300 dNTrackTC < fDnMax
// TRK_105: P>60 T>300 dNTrackTC < fDnMax abs(fDtTcTc) > 100
//-----------------------------------------------------------------------------
        if (tp->fP > 60) {
          FillTrackHistograms (fHist.fTrack [122],trk,tp,&fSimPar);
          FillPipenuHistograms(fHist.fPipenu[122],trk,tp,&fSimPar);
          if (trk->T0() > 300) {
            FillTrackHistograms (fHist.fTrack [123],trk,tp,&fSimPar);
            FillPipenuHistograms(fHist.fPipenu[123],trk,tp,&fSimPar);
            if (tp->fDnTrackTc <= fDnMax) { 
              FillTrackHistograms (fHist.fTrack [124],trk,tp,&fSimPar);
              FillPipenuHistograms(fHist.fPipenu[124],trk,tp,&fSimPar);
              if (fabs(tp->fDtTcTc) > 100) {
                fEventPassedSelections = 1;
                FillTrackHistograms (fHist.fTrack [125],trk,tp,&fSimPar);
                FillPipenuHistograms(fHist.fPipenu[125],trk,tp,&fSimPar);
              }
            }
          }
        }
//-----------------------------------------------------------------------------
// the same, but w/o p>60 MeV/c cut
//-----------------------------------------------------------------------------
        if (trk->T0() > 300) {
          FillTrackHistograms (fHist.fTrack [126],trk,tp,&fSimPar);
          FillPipenuHistograms(fHist.fPipenu[126],trk,tp,&fSimPar);
          if (tp->fDnTrackTc <= fDnMax) { 
            FillTrackHistograms (fHist.fTrack [127],trk,tp,&fSimPar);
            FillPipenuHistograms(fHist.fPipenu[127],trk,tp,&fSimPar);
            if (fabs(tp->fDtTcTc) > 100) {
              fEventPassedSelections = 1;
              FillTrackHistograms (fHist.fTrack [128],trk,tp,&fSimPar);
              FillPipenuHistograms(fHist.fPipenu[128],trk,tp,&fSimPar);
            }
          }
        }
      }
      else {
        FillTrackHistograms (fHist.fTrack [221],trk,tp,&fSimPar);
        FillPipenuHistograms(fHist.fPipenu[221],trk,tp,&fSimPar);
      }
    }
// tight cut ends ------------------------------------------------------------
//-----------------------------------------------------------------------------
// TRK_101: all tracks pi+ --> e+ nu weighted with the pion survival prob
//-----------------------------------------------------------------------------
    if (fEvtPar.fPionSurvProb > 0) {
      FillTrackHistograms (fHist.fTrack [ 50],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
      FillPipenuHistograms(fHist.fPipenu[ 50],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
      if (trk->Charge() > 0) {
        FillTrackHistograms (fHist.fTrack [150],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
        FillPipenuHistograms(fHist.fPipenu[150],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
      }
      else {
        FillTrackHistograms (fHist.fTrack [250],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
        FillPipenuHistograms(fHist.fPipenu[250],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
      }
      //      std::cout<<"Whatever BestID: "<<tp->fIDWord[fBestID]<<std::endl;
      if (tp->fIDWord[fBestID] == 0) {
        FillTrackHistograms (fHist.fTrack [ 51],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
        FillPipenuHistograms(fHist.fPipenu[ 51],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
        if (trk->Charge() > 0) {
          FillTrackHistograms (fHist.fTrack [151],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
          FillPipenuHistograms(fHist.fPipenu[151],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
          if (tp->fP > 60) {
            FillTrackHistograms (fHist.fTrack [152],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
            FillPipenuHistograms(fHist.fPipenu[152],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
            if (trk->T0() > 300) {
              FillTrackHistograms (fHist.fTrack [153],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
              FillPipenuHistograms(fHist.fPipenu[153],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
//-----------------------------------------------------------------------------
// In the range 67.5 to 70.0, compare with BU results
//-----------------------------------------------------------------------------
              if ((tp->fP >= 67.5) & (tp->fP <= 70.0)) {
                FillTrackHistograms (fHist.fTrack [159],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
                FillPipenuHistograms(fHist.fPipenu[159],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
              }
//-----------------------------------------------------------------------------
// at this point, apply the analysis-specific cuts
// 1. require the number of track hits to be consistent with the number of hits 
// 2. require no close time clusters
//-----------------------------------------------------------------------------
              if (tp->fDnTrackTc <= fDnMax) {
                FillTrackHistograms (fHist.fTrack [154],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
                FillPipenuHistograms(fHist.fPipenu[154],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
                if (fabs(tp->fDtTcTc) > 100) {
                  FillTrackHistograms (fHist.fTrack [155],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
                  FillPipenuHistograms(fHist.fPipenu[155],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
                }
              }
            }
          }
//-----------------------------------------------------------------------------
// the same but w/o p>60 MeV/c cut
//-----------------------------------------------------------------------------
          if (trk->T0() > 300) {
            FillTrackHistograms (fHist.fTrack [156],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
            FillPipenuHistograms(fHist.fPipenu[156],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
            if (tp->fDnTrackTc <= fDnMax) {
              FillTrackHistograms (fHist.fTrack [157],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
              FillPipenuHistograms(fHist.fPipenu[157],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
              if (fabs(tp->fDtTcTc) > 100) {
                FillTrackHistograms (fHist.fTrack [158],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
                FillPipenuHistograms(fHist.fPipenu[158],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
              }
            }
          }
        }
        else {
          FillTrackHistograms (fHist.fTrack [251],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
          FillPipenuHistograms(fHist.fPipenu[251],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
        }
      }
//-----------------------------------------------------------------------------
// tight ID
//-----------------------------------------------------------------------------
      if (tp->fIDWord[1] == 0) {
        FillTrackHistograms (fHist.fTrack [ 71],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
        FillPipenuHistograms(fHist.fPipenu[ 71],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
        if (trk->Charge() > 0) {
          FillTrackHistograms (fHist.fTrack [171],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
          FillPipenuHistograms(fHist.fPipenu[171],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
          if (tp->fP > 60) {
            FillTrackHistograms (fHist.fTrack [172],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
            FillPipenuHistograms(fHist.fPipenu[172],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
            if (trk->T0() > 300) {
              FillTrackHistograms (fHist.fTrack [173],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
              FillPipenuHistograms(fHist.fPipenu[173],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
//-----------------------------------------------------------------------------
// at this point, apply the analysis-specific cuts
// 1. require the number of track hits to be consistent with the number of hits 
// 2. require no close time clusters
//-----------------------------------------------------------------------------
              if (tp->fDnTrackTc <= fDnMax) {
                FillTrackHistograms (fHist.fTrack [174],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
                FillPipenuHistograms(fHist.fPipenu[174],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
                if (fabs(tp->fDtTcTc) > 100) {
                  FillTrackHistograms (fHist.fTrack [175],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
                  FillPipenuHistograms(fHist.fPipenu[175],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
                }
              }
//-----------------------------------------------------------------------------
// In the range 67.5 to 70.0, compare with BU results
//-----------------------------------------------------------------------------
              if ((tp->fP >= 67.5) & (tp->fP <= 70.0)) {
                FillTrackHistograms (fHist.fTrack [179],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
                FillPipenuHistograms(fHist.fPipenu[179],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
              }
            }
          }
//-----------------------------------------------------------------------------
// the same but w/o p>60 MeV/c cut
//-----------------------------------------------------------------------------
          if (trk->T0() > 300) {
            FillTrackHistograms (fHist.fTrack [176],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
            FillPipenuHistograms(fHist.fPipenu[176],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
            if (tp->fDnTrackTc <= fDnMax) {
              FillTrackHistograms (fHist.fTrack [177],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
              FillPipenuHistograms(fHist.fPipenu[177],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
              if (fabs(tp->fDtTcTc) > 100) {
                FillTrackHistograms (fHist.fTrack [178],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
                FillPipenuHistograms(fHist.fPipenu[178],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
              }
            }
          }
        }
        else {
          FillTrackHistograms (fHist.fTrack [271],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
          FillPipenuHistograms(fHist.fPipenu[271],trk,tp,&fSimPar,fEvtPar.fPionSurvProb);
        }
      }
    } // pi+ e+ nu, 101 set finishes
  } // track and pipenu histograms filling

// //-----------------------------------------------------------------------------
// // fill track ID histograms
// //-----------------------------------------------------------------------------

}// fillhistogram ends


//-----------------------------------------------------------------------------
// 2014-04-30: it looks that reading the straw hits takes a lot of time - 
//              turn off by default by commenting it out
//-----------------------------------------------------------------------------
int TPipenuAnaModule::Event(int ientry) {

  fTrackBlock->GetEntry(ientry);
  fTrackSeedBlock->GetEntry(ientry);
  fHelixBlock->GetEntry(ientry);
  fTimeClusterBlock->GetEntry(ientry);
  fGenpBlock->GetEntry(ientry);
  fSimpBlock->GetEntry(ientry);
  fVDetBlock->GetEntry(ientry);
//-----------------------------------------------------------------------------
// extras from spmcana, remove later ---
//-----------------------------------------------------------------------------
  fNVDetHits = fVDetBlock->NStepPoints();
  fNSimp = fSimpBlock->NParticles();
  fPbarCosThPV = -2.;
  fPbarMomPV   = -1.;
  // fWeight     = fGenpBlock->Weight();
  //  std::cout<<"fweight from genblock: "<<fWeight<<std::endl;
  fTMaxSimp   = -1;
  fTMaxSpmc = -1;
  fHitVD08  = nullptr;
  fHitVD15  = nullptr;
//-----------------------------------------------------------------------------
// at some point, the TVdetBlock class got obsolete, and now the virtual detector 
// hits are stored in TStepPointMCBlock 
// dont' try to read it as that would fail
//-----------------------------------------------------------------------------
  fEvtPar.fDioLOWt          = 1.;
  fEvtPar.fDioLLWt          = 1.;
  fEvtPar.fNCrvClusters     = -1;
  fEvtPar.fNCrvPulses       = -1;
  fEvtPar.fNCrvCoincidences = -1;
  fEvtPar.fNStrawHits       = GetHeaderBlock()->NStrawHits();
  fEvtPar.fNComboHits       = GetHeaderBlock()->NComboHits();
  fEvtPar.fNGenp            = fGenpBlock->NParticles();
  fEvtPar.fNSimp            = fSimpBlock->NParticles();
  fEvtPar.fSimp             = nullptr;
  fEvtPar.fPartE            = -1.;
  fEvtPar.fPionSurvProb     = 1.;
  fEvtPar.fNHelices         = fHelixBlock->NHelices();
  fEvtPar.fNTimeClusters    = fTimeClusterBlock->NTimeClusters();
//-----------------------------------------------------------------------------
// luminosity weight
//-----------------------------------------------------------------------------
  fLumWt = GetHeaderBlock()->LumWeight();
//-----------------------------------------------------------------------------
// figure the MC truth
// pi+ --> e+ nu case : determine the event weight
//-----------------------------------------------------------------------------
  for (int i=fEvtPar.fNSimp-1; i>=0; i--) {
    TSimParticle* simp = fSimpBlock->Particle(i);
    int pdg_code       = simp->PDGCode();
    int generator_id   = simp->GeneratorID();         // MC process code

    if ((pdg_code == fPDGCode) and (generator_id == fMCProcessCode)) {
      fEvtPar.fSimp  = simp;
      fEvtPar.fPartE = simp->StartMom()->Energy();
    }

    if ((abs(pdg_code) == 211) && (simp->GeneratorID() == 56)) {
//-----------------------------------------------------------------------------
// found the pion, survival probability
//-----------------------------------------------------------------------------
      fEvtPar.fPionSurvProb  = exp(-simp->fEndProperTime);
      break;
    }
  }
//-----------------------------------------------------------------------------
// calculate DIO weights once per event
//-----------------------------------------------------------------------------
  if (fEvtPar.fPartE > 0) {
    fEvtPar.fDioLOWt = TStntuple::DioWeightAl   (fEvtPar.fPartE);
    fEvtPar.fDioLLWt = TStntuple::DioWeightAl_LL(fEvtPar.fPartE);
  }
//-----------------------------------------------------------------------------
// may want to revisit the definition of fSimp in the future
//-----------------------------------------------------------------------------
  fSimPar.fParticle = fEvtPar.fSimp;
  fSimPar.fTFront   = NULL;
  fSimPar.fTMid     = NULL;
  fSimPar.fTBack    = NULL;
//-----------------------------------------------------------------------------
// process virtual detectors - for fSimp need parameters at tracker entrance
// use the first fit
//-----------------------------------------------------------------------------
  fSimPar.fTFront   = NULL;
  fSimPar.fTMid     = NULL;
  fSimPar.fTBack    = NULL;
  int nvdhits = fVDetBlock->NStepPoints();
  for (int i=0; i<nvdhits; i++) {
    TStepPointMC* vdhit = fVDetBlock->StepPointMC(i);
    if (vdhit->PDGCode() == fEvtPar.fSimp->fPdgCode) {
      if ((vdhit->VolumeID() == 13) || (vdhit->VolumeID() == 14)) {
	if (fDirection*vdhit->Mom()->Z() > 0) {
	  if (fSimPar.fTFront == 0) fSimPar.fTFront = vdhit;
	}
      }
      else if ((vdhit->VolumeID() == 11) || (vdhit->VolumeID() == 12)) {
	if (fDirection*vdhit->Mom()->Z() > 0) {
	  if (fSimPar.fTMid == 0) fSimPar.fTMid = vdhit;
	}
      }
      else if (vdhit->VolumeID() == mu2e::VirtualDetectorId::TT_Back) {
	if (fDirection*vdhit->Mom()->Z() > 0) {
	  if (fSimPar.fTBack == NULL) fSimPar.fTBack = vdhit;
	}
      }
    }
  }
//-----------------------------------------------------------------------------
// init calorimeter
//-----------------------------------------------------------------------------
 fNClusters  = fClusterBlock->NClusters();
 fNCalHits   = fCalDataBlock->NHits();
//-----------------------------------------------------------------------------
// init calorimeter clusters - remember, the first one not necessarily is the 
// most energetic
//-----------------------------------------------------------------------------
 fNClusters = fClusterBlock->NClusters();
 if (fNClusters == 0) fCluster = 0;
 else                 fCluster = fClusterBlock->Cluster(0);
//-----------------------------------------------------------------------------
// loop over tracks and calculate needed parameters
//-----------------------------------------------------------------------------
  fNStrawHits          = GetHeaderBlock()->fNStrawHits;

  fEvtPar.fNTracks[0]  = fTrackBlock->NTracks();
  if (fEvtPar.fNTracks[0] == 0) fTrack = 0;
  else                          fTrack = fTrackBlock->Track(0);

  fEvtPar.fNGoodTracks[0] = 0;
  fEvtPar.fNGoodTracks[1] = 0;
  fNMatchedTracks         = 0;
//-----------------------------------------------------------------------------
// determine the number of CalPatRec tracks - this assumes that the list of 
// tracks has been created by MergePatRec
//-----------------------------------------------------------------------------
  int ntrk = fTrackBlock->NTracks();

  InitTrackPar(fTrackBlock,fClusterBlock,fTrackPar,&fSimPar);
//-----------------------------------------------------------------------------
// additional initializations - helices and time clusters
//-----------------------------------------------------------------------------
  fN300 = 0;
  for (int itrk=0; itrk<ntrk; itrk++) {
    TStnTrack*   trk = fTrackBlock->Track(itrk);
    TrackPar_t*  tp  = fTrackPar+itrk;

    tp->fHelix       = nullptr;
    tp->fTimeCluster = nullptr;

    //int ih = trk->fHelixIndex;
    //    if (ih < fEvtPar.fNHelices) {
    // tp->fHelix = fHelixBlock->Helix(ih);
    //}   // look at the helix comparison later
//-----------------------------------------------------------------------------
// fN300 : the number of late (T0>300) good tracks above 60 MeV/c
//-----------------------------------------------------------------------------
   if ((tp->fIDWord[fBestID] == 0) and (tp->fP > 60) and (trk->T0() > 300)) { 
     fN300++;
   }
//-----------------------------------------------------------------------------
// currently, at least in the KK case, an TStnHelix doesn't have a helix index stored
// find a time cluster closest to teh track in time
//-----------------------------------------------------------------------------
    TStnTimeCluster* closest_tc(nullptr);
    float min_dt = 1.e6;
    for (int itc=0; itc<fEvtPar.fNTimeClusters; itc++) {
      TStnTimeCluster* tc = fTimeClusterBlock->TimeCluster(itc);
      float dt = trk->T0()-tc->T0();
      if (fabs(dt) < fabs(min_dt)) {
        closest_tc = tc;
        min_dt     = dt;
      }
    }

    tp->fTimeCluster = closest_tc;
    tp->fDtTrackTc   = min_dt;
  }

  fEventPassedSelections = 0;

  FillHistograms();

  Debug();

  return 0;		       
}

//-----------------------------------------------------------------------------
void TPipenuAnaModule::Debug() {

  TStnTrack* trk;
  TrackPar_t* tp;
  int ntrk = fTrackBlock->NTracks();

  for (int itrk=0; itrk<ntrk; itrk++) {
    trk = fTrackBlock->Track(itrk);
    tp  = &fTrackPar[itrk];
//-----------------------------------------------------------------------------
// bit 0: all tracks
//-----------------------------------------------------------------------------
    if (GetDebugBit(0) == 1) {
      GetHeaderBlock()->Print(Form("bit_000: All p = %10.3f",trk->Momentum()->P()));
    }
//-----------------------------------------------------------------------------
// bit 1: IDWord =0 0 tracks
//-----------------------------------------------------------------------------
    if (GetDebugBit(1) == 1) {
      if (tp->fIDWord[fBestID] == 0) {
	GetHeaderBlock()->Print(Form("bit_001: IDWord=0 p = %10.3f",trk->Momentum()->P()));
      }
    }
//-----------------------------------------------------------------------------
// bit 3: good tracks with P > 70 T0 > 300
//-----------------------------------------------------------------------------
    if (GetDebugBit(3) == 1) {
      if (fEventPassedSelections == 1) {
        if ((tp->fP > 70) and (trk->fT0 > 300)) {
	  GetHeaderBlock()->Print(Form("large P: %f",tp->fP));
	}
      }
    }
//-----------------------------------------------------------------------------
// bit 4: good events with 65 < P < 70 , T0 > 300
//-----------------------------------------------------------------------------
    if (GetDebugBit(4) == 1) {
      if (tp->fIDWord[fBestID] == 0) {
        if ((tp->fDnTrackTc <= fDnMax) and (fabs(tp->fDtTcTc) > 100)) {
          if ((tp->fP > 65) and (tp->fP < 700) and (trk->fT0 > 300)) {
            GetHeaderBlock()->Print(Form(":bit_04: candidate: i=%i trk->P0=%10.3f tp->fP=%10.3f",itrk,trk->P0(),tp->fP));
          }
        }
      }
    }
  }
//-----------------------------------------------------------------------------
// bit 5: events with N(tracks) > 1
//-----------------------------------------------------------------------------
  if (GetDebugBit(5) == 1) {
    int ntrk = fTrackBlock->NTracks();
    if (ntrk > 1) {
      GetHeaderBlock()->Print(Form("NTracks = %5i",ntrk));
    }
  }
}

//_____________________________________________________________________________
int TPipenuAnaModule::EndJob() {
  printf("----- end job: ---- %s\n",GetName());
  return 0;
}

//_____________________________________________________________________________
void TPipenuAnaModule::Test001() {
}


}
