///////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////
#include "pipenu/ana/scripts/modules.hh"
def_name pipenu_spmc_001("pipenu_spmc_ana");
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
void  pipenu_spmc_ana(int DebugBit = -1) {
  pipenu::m_spmc = (pipenu::TSpmcAnaModule*) g.x->AddModule("pipenu::TSpmcAnaModule",0);  
  pipenu::m_spmc->SetSpmcBlockName("SpmcBlockVDet");
  if (DebugBit >= 0) pipenu::m_spmc->SetDebugBit(DebugBit,1);
}
