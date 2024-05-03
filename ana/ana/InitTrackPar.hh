// 
#ifndef __pipenu_ana_InitVirtualDetectors_hh__
#define __pipenu_ana_InitVirtualDetectors_hh__

#include "pipenu/ana/VDetData_t.hh"
#include "pipenu/ana/TrackPar_t.hh"
#include "pipenu/ana/SimPar_t.hh"
#include "Stntuple/obj/TStnTrackBlock.hh"
#include "Stntuple/obj/TStnClusterBlock.hh"
#include "Stntuple/obj/TStnCluster.hh"
#include "Stntuple/obj/TStepPointMCBlock.hh"
#include "Stntuple/obj/TStepPointMC.hh"

namespace pipenu {

  int     InitTrackPar(TStnTrackBlock*    TrackBlock  , 
		       TStnClusterBlock*  ClusterBlock, 
		       TrackPar_t*        TrackPar    ,
		       SimPar_t*          SimPar      );

}
#endif
