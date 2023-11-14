///////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////
#include "pipenu/ana/scripts/modules.hh"
def_name pipenu_spmc_001("pipenu_spmc_ana");
def_name pipenu_spmc_002("pipenu_spmc_ana_1");
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
void  pipenu_spmc_ana(int DebugBit = -1) {
  pipenu::m_spmc = (pipenu::TSpmcAnaModule*) g.x->AddModule("pipenu::TSpmcAnaModule",0);  
  pipenu::m_spmc->SetSpmcBlockName("SpmcBlockVDet");
  if (DebugBit >= 0) pipenu::m_spmc->SetDebugBit(DebugBit,1);
}

void  pipenu_spmc_ana_1(int DebugBit = -1) {
  pipenu::m_spmc = (pipenu::TSpmcAnaModule*) g.x->AddModule("pipenu::TSpmcAnaModule",0);  
  pipenu::m_spmc->SetSpmcBlockName("SpmcBlockVDet");
  if (DebugBit >= 0) pipenu::m_spmc->SetDebugBit(DebugBit,1);
  // plus pass a parameter defining the weight
  // ....
}
