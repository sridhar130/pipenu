#include "pipenu/ana/InitTrackPar.hh"
#include "Stntuple/obj/TStnHeaderBlock.hh"
#include "TEnv.h"
#include "TSystem.h"
#include "TDirectory.h"
#include "Stntuple/loop/TStnAna.hh"
#include "Stntuple/loop/TStnModule.hh"

namespace pipenu {
//-----------------------------------------------------------------------------
// on input: TrackPar->fFitType = 0: PAR, =1: DAR
//-----------------------------------------------------------------------------
int InitTrackPar(TStnTrackBlock*     TrackBlock  , 
			     TStnClusterBlock*   ClusterBlock, 
			     TrackPar_t*         TrackPar    ,
			     SimPar_t*           SimPar      ) {

  int fApplyCorr     =1; // ToDo: setting here could be a problem.
  double fMbTime     =1695.0; //micro bunch time
  int  fNID          = 2; //trackID set

//-----------------------------------------------------------------------------
// momentum corrections for KPAR and KDAR
//-----------------------------------------------------------------------------
  const double kMomentumCorr[2] = { 0.0 , 0.0 } ; // SU2020 // CD3: { 0.049, 0.020 };
  const double kDtTcmCorr   [2] = { 0.  , 0.  } ; // { 0.22 , -0.30 }; // ns, sign: fit peak positions
//-----------------------------------------------------------------------------
// loop over tracks
//-----------------------------------------------------------------------------
  int ntrk = TrackBlock->NTracks();

  for (int itrk=0; itrk<ntrk; itrk++) {
    TrackPar_t*   tp = TrackPar+itrk;
    TStnTrack* track = TrackBlock->Track(itrk);
    int fit_type = 0;
//-----------------------------------------------------------------------------
// process hit masks
//-----------------------------------------------------------------------------
    int i1, i2, n1(0) ,n2(0), ndiff(0);
    int nbits = track->fHitMask.GetNBits();
    for (int i=0; i<nbits; i++) {
      i1 = track->HitMask()->GetBit(i);
      i2 = track->ExpectedHitMask()->GetBit(i);
      n1 += i1;
      n2 += i2;
      if (i1 != i2) ndiff += 1;
    }
//-----------------------------------------------------------------------------
// define additional parameters
//-----------------------------------------------------------------------------
    tp->fNHPl = n1;
    tp->fNEPl = n2;
    tp->fNDPl        = ndiff;
    tp->fTimeCluster = nullptr;
//-----------------------------------------------------------------------------
// in this scheme correction is set right before the call
//-----------------------------------------------------------------------------
    if (fApplyCorr != 0) tp->fP = track->fP0 + kMomentumCorr[fit_type]; 
//-----------------------------------------------------------------------------
// hits on virtual detectors
//-----------------------------------------------------------------------------
    tp->fPStOut  = track->fPStOut;
    tp->fPFront  = track->fPFront;

    tp->fDpF     = tp->fP     -tp->fPFront;
    tp->fDp0     = track->fP0 -tp->fPFront;
    tp->fDp2     = track->fP2 -tp->fPFront;
    tp->fDpFSt   = tp->fPFront-tp->fPStOut;

    tp->fXDpF    = tp->fDpF/track->fFitMomErr;
    //    TStnModule* myModule= new TStnModule();
    //TStnHeaderBlock* headerBlock = myModule->GetHeaderBlock();
    // if (headerBlock) {
    //   // Use the headerBlock pointer (it points to a valid TStnHeaderBlock object)
    // } else {
    //   // Handle the case where GetHeaderBlock fails (e.g., print an error message)
    // }


    //tp->fLumWt   = headerBlock->LumWeight(); 
    tp->fLumWt   = 1; //TODO: header block not working.

    tp->fDtZ0 = -1.e6;
    if (SimPar->fTMid) {
      double ttrue = fmod(SimPar->fTMid->Time(),fMbTime);
      tp->fDtZ0 = track->T0()-ttrue;
    }

    tp->fXtZ0 = tp->fDtZ0/track->fT0Err;

    tp->fDtBack = -1.e6;
    if (SimPar->fTBack) {
      double ttrue = fmod(SimPar->fTBack->Time(),fMbTime);
      tp->fDtBack = track->T0()-ttrue;
    }
//-----------------------------------------------------------------------------
// track residuals
//-----------------------------------------------------------------------------
    TStnTrack::InterData_t*  vr = track->fVMaxEp; 
    double    nx, ny;

    tp->fCluster   = nullptr;

    tp->fEcl       = track->fClusterE;
    tp->fSeedFr    = -1.e6;
    tp->fNCrystals = -1.e6;
    tp->fDiskID    = -1;
    tp->fEp        = tp->fEcl/track->fP2;   // compare momentum on exit to the cluster energy
    tp->fDrDzCal   = -1.e6;
    tp->fDtClZ0    = -1.e6;

    tp->fDu        = -1.e6;
    tp->fDv        = -1.e6;
    tp->fDx        = -1.e6;
    tp->fDy        = -1.e6;
    tp->fDz        = -1.e6;
    tp->fDt        = -1.e6;

    tp->fChi2Tcm   = -1.e6;
    tp->fChi2XY    = -1.e6;
    tp->fChi2T     = -1.e6;
    tp->fPath      = -1.e6;
    tp->fSinTC     = -1.e6;
    tp->fDrTC      = -1.e6;
    tp->fSInt      = -1.e6;

    if (vr) {
      tp->fDiskID  = vr->fID;
      tp->fCluster = ClusterBlock->Cluster(vr->fClusterIndex);
      tp->fSeedFr  = tp->fCluster->SeedFr();
      tp->fNCrystals = tp->fCluster->NCrystals();
      tp->fEcl     = vr->fEnergy;
      tp->fEp      = tp->fEcl/track->fP2;
      tp->fDrDzCal = (vr->fXTrk*vr->fNxTrk+vr->fYTrk+vr->fNyTrk)/sqrt(vr->fXTrk*vr->fXTrk+vr->fYTrk*vr->fYTrk)/vr->fNzTrk;

      tp->fDx      = vr->fDx;
      tp->fDy      = vr->fDy;
      tp->fDz      = vr->fDz;
//-----------------------------------------------------------------------------
// v4_2_4: correct by additional 0.22 ns - track propagation by 6 cm
//-----------------------------------------------------------------------------
      tp->fDt  = vr->fDt ;                                       // v4_2_4: - 0.22; // - 1.;
      if (fApplyCorr != 0) tp->fDt  -= kDtTcmCorr[fit_type];

      nx  = vr->fNxTrk/sqrt(vr->fNxTrk*vr->fNxTrk+vr->fNyTrk*vr->fNyTrk);
      ny  = vr->fNyTrk/sqrt(vr->fNxTrk*vr->fNxTrk+vr->fNyTrk*vr->fNyTrk);

      tp->fDu        = vr->fDx*nx+vr->fDy*ny;
      tp->fDv        = vr->fDx*ny-vr->fDy*nx;
      tp->fChi2Tcm   = vr->fChi2Match;
					// from now on the matching chi2 has XY part only
      tp->fChi2XY    = vr->fChi2Match;
      tp->fChi2T     = vr->fChi2Time;
      tp->fPath      = vr->fPath;
//-----------------------------------------------------------------------------
// angle
//-----------------------------------------------------------------------------
      TStnCluster* cl = ClusterBlock->Cluster(vr->fClusterIndex);
      tp->fSinTC = nx*cl->fNy-ny*cl->fNx;
      tp->fDrTC  = vr->fDr;
      tp->fSInt  = vr->fSInt;

      if (SimPar->fTMid) {
	tp->fDtClZ0 = tp->fDt-tp->fDtZ0;
      }
    }

    // if ((tp->fEp > 0) && (track->fEp > 0) && (fabs(tp->fEp-track->fEp) > 5.e-6)) {
    //   GetHeaderBlock()->Print(Form(" TPipenuAnaModule ERROR: tp->fEp = %10.5f  track->fEp = %10.5f\n ",tp->fEp,track->fEp));
    //  }
// //-----------------------------------------------------------------------------
// // on-the-fly MVA calculation
// // tp->fMVAOut[0] : comes from offline
// // tp->fMVAOut[1] : calculated on the fly
// //-----------------------------------------------------------------------------
//     tp->fTrqMvaOut[0] = track->DaveTrkQual();        // comes from Offline
//     tp->fTrqMvaOut[1] = track->DaveTrkQual(); 

//     if (fUseTrqMVA != 0) {
// //-----------------------------------------------------------------------------
// // MVA output calculated on the fly - in principle, should be charge-symmetric
// // can have several TRQ MVA's defined, each track block uses only one of them,
// // indexed with tp->fTrqMvaIndex
// //-----------------------------------------------------------------------------
//       float na = track->NActive();
//       float nm = track->NMat();

//       int imva = tp->fTrqMvaIndex;	                          // 
	
//       if (fTrqMVA[imva] != nullptr) {
// 	fTrqMVA[imva]->fVar[0] = na;
// 	fTrqMVA[imva]->fVar[1] = na/track->NHits();
// 	fTrqMVA[imva]->fVar[2] = log10(track->FitCons());
// 	fTrqMVA[imva]->fVar[3] = track->FitMomErr();
// 	fTrqMVA[imva]->fVar[4] = track->T0Err();
// 	fTrqMVA[imva]->fVar[5] = track->NDoubletsAct()/na;
// 	fTrqMVA[imva]->fVar[6] = track->NHitsAmbZero()/na;
// 	fTrqMVA[imva]->fVar[7] = track->NMatActive()/nm;
//       // pmva[ 8] = track->D0();                          // low-rank, do not use
//       // pmva[ 9] = track->RMax();                        // low-rank, do not use

// 	tp->fTrqMvaOut[1] = fTrqMVA[imva]->Eval();

// 	if (tp->fTrqMvaOut[1] < -990) {
// 	  GetHeaderBlock()->Print(Form("BAD TRQ: ntrk, itrk= %2i %2i",ntrk,itrk));
// 	  PrintTrack(track,tp,"");
// 	}

// 	track->SetTmp(0,tp->fTrqMvaOut[1]);
//       }
//     }
//-----------------------------------------------------------------------------
// track ID 
//-----------------------------------------------------------------------------
    for (int k=0; k<fNID; k++) {
      tp->fIDWord[k]  = tp->fTrackID[k]->IDWord(track);
    }
//-----------------------------------------------------------------------------
// track-CRV residuals
//-----------------------------------------------------------------------------
    tp->fDtCRV       = -1.e6;
    tp->fZCRV        = -1.e6;
    tp->fDtCRV2      = -1.e6;
//-----------------------------------------------------------------------------
// TrackCaloHit
//-----------------------------------------------------------------------------
    TStnTrack::InterData_t*  tch = track->fVTCH;
  
    tp->fTchTime   = tch->fTime;
    tp->fTchPath   = tch->fPath;
    tp->fTchDr     = tch->fDr;
    tp->fTchDx     = tch->fDx;
    tp->fTchDy     = tch->fDy;
    tp->fTchDz     = tch->fDz;
    tp->fTchDt     = tch->fDt;
// //-----------------------------------------------------------------------------
// // MVA-based PID - can't calculate here, as need two reco outputs
// //-----------------------------------------------------------------------------
//     tp->fPidMvaOut[0] = -1.e6;
// //-----------------------------------------------------------------------------
// // both electron and muon tracks are there, need to apply preselections
// //-----------------------------------------------------------------------------
//     if (fUsePidMVA != 0) {
//       if ((tp->fTchDt > -100) && (tp->fTchDz > -50) && (tp->fTchDz < 250)) {

// 	fPidMVA[0]->fVar[0] = tp->fEp;
// 	fPidMVA[0]->fVar[1] = tp->fNCrystals;
// 	fPidMVA[0]->fVar[2] = tp->fSeedFr;
// 	fPidMVA[0]->fVar[3] = tp->fTchDt;
// 	fPidMVA[0]->fVar[4] = tp->fTchDz;
// 	fPidMVA[0]->fVar[5] = tp->fTchDr;
// 	fPidMVA[0]->fVar[6] = tp->fPath;

// 	tp->fPidMvaOut[0] = fPidMVA[0]->Eval();
//       }
//     }
 // std::cout<<"----------------------------------------------"<<std::endl;
 //      std::cout<<"Tp While setting:"<<std::endl;
 //      std::cout<<"----------------------------------------------"<<std::endl;
 //      std::cout<<"fP: "<<tp->fP<<" idWord: "<<tp->fIDWord[0]<<std::endl;
  }

  return 0;
     
}
}
