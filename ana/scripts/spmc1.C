///////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////
#include "pipenu/ana/scripts/modules.hh"
def_name pipenu_spmc1_001("pipenu_spmc1_ana");
def_name pipenu_spmc1_002("pipenu_spmc1_ana_1");
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
void  pipenu_spmc1_ana(int DebugBit = -1) {
  pipenu::m_spmc1 = (pipenu::TSpmcAnaModule*) g.x->AddModule("pipenu::TSpmcAnaModule",0);  
  pipenu::m_spmc1->SetSpmcBlockName("SpmcBlockVDet");
  if (DebugBit >= 0) pipenu::m_spmc1->SetDebugBit(DebugBit,1);
  std::cout<<"Module SPMC1 loaded-------------"<<std::endl;
 // weight parameter =0,1: 0->weight=1, 1->weight=surv_prob
  pipenu::m_spmc1->SetWeightParameter(0);
}

void  pipenu_spmc1_ana_1(int DebugBit = -1) {
  pipenu::m_spmc1 = (pipenu::TSpmcAnaModule*) g.x->AddModule("pipenu::TSpmcAnaModule",0);  
  pipenu::m_spmc1->SetSpmcBlockName("SpmcBlockVDet");
  if (DebugBit >= 0) pipenu::m_spmc1->SetDebugBit(DebugBit,1);
  // weight parameter =0,1: 0->weight=1, 1->weight=surv_prob
  pipenu::m_spmc1->SetWeightParameter(1);
  // ....
}
