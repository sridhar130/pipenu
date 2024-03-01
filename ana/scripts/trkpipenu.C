///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "Stntuple/scripts/global_vars.h"
#include "pipenu/ana/scripts/modules.hh"

def_name stnpipenu_trk_001("stnpipenu_trk_ana");
def_name stnpipenu_trk_002("stnpipenu_trk_ana_05");


void stnpipenu_trk_ana(int DebugBit = -1) {
//-----------------------------------------------------------------------------
// configure validation module
//-----------------------------------------------------------------------------
  pipenu::m_trk = (pipenu::TTrackAnaModule*) g.x->AddModule("pipenu::TTrackAnaModule",0);  
  //  pipenu::m_trk->SetTrackBlockName("TrackBlock");
  // pipenu::m_trk->SetPdgCode       (PdgCode);
  // pipenu::m_trk->SetGeneratorCode (GeneratorCode);
  if (DebugBit >= 0) pipenu::m_trk->SetDebugBit(DebugBit,1);
}

void stnpipenu_trk_ana_05(int DebugBit = -1) {
//-----------------------------------------------------------------------------
// configure validation module
//-----------------------------------------------------------------------------
  pipenu::m_trk = (pipenu::TTrackAnaModule*) g.x->AddModule("pipenu::TTrackAnaModule",0);  

  //pipenu::m_trk->SetTrackBlockName(TrackBlockName);
  //pipenu::m_trk->SetPdgCode       (PdgCode);
  //pipenu::m_trk->SetGeneratorCode (GeneratorCode);

  pipenu::m_trk->SetBField (0.5);
  if (DebugBit >= 0) pipenu::m_trk->SetDebugBit(DebugBit,1);
}
