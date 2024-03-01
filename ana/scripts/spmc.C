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
  std::cout<<"Module SPMC loaded-------------"<<std::endl;
 // weight parameter =0,1: 0->weight=1, 1->weight=surv_prob
  pipenu::m_spmc->SetWeightParameter(0);
}

void  pipenu_spmc_ana_1(int DebugBit = -1) {
  pipenu::m_spmc = (pipenu::TSpmcAnaModule*) g.x->AddModule("pipenu::TSpmcAnaModule",0);  
  pipenu::m_spmc->SetSpmcBlockName("SpmcBlockVDet");
  if (DebugBit >= 0) pipenu::m_spmc->SetDebugBit(DebugBit,1);
  // weight parameter =0,1: 0->weight=1, 1->weight=surv_prob
  pipenu::m_spmc->SetWeightParameter(1);
  // ....
}
