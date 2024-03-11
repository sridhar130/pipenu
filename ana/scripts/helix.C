///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
#include "Stntuple/scripts/global_vars.h"
#include "pipenu/ana/scripts/modules.hh"

def_name pipenu_helix_001("pipenu_helix_ana");
def_name pipenu_helix_002("pipenu_helix_ana_1");
def_name pipenu_helix_003("pipenu_helix_ana_old");


//-----------------------------------------------------------------------------
void  pipenu_helix_ana(int DebugBit = -1) {
  pipenu::m_hel = (pipenu::THelixAnaModule*) g.x->AddModule("pipenu::THelixAnaModule",0);  

  if (DebugBit >= 0)pipenu::m_hel->SetDebugBit(DebugBit,1);
  // weight parameter =0,1: 0->weight=1, 1->weight=surv_prob
  pipenu::m_hel->SetWeightParameter(0);
}
void  pipenu_helix_ana_1(int DebugBit = -1) {
  pipenu::m_hel = (pipenu::THelixAnaModule*) g.x->AddModule("pipenu::THelixAnaModule",0);  

  if (DebugBit >= 0)pipenu::m_hel->SetDebugBit(DebugBit,1);
  // weight parameter =0,1: 0->weight=1, 1->weight=surv_prob
  pipenu::m_hel->SetWeightParameter(1);
}
//-----------------------------------------------------------------------------
void  pipenu_helix_ana_old(int DebugBit = -1) {
  pipenu::m_hel = (pipenu::THelixAnaModule*) g.x->AddModule("pipenu::THelixAnaModule",0);

  pipenu::m_hel->SetHelixBlockName("HelixBlock");
  if (DebugBit >= 0) pipenu::m_hel->SetDebugBit(DebugBit,1);
}
