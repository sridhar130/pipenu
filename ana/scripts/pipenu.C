///////////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////////
#include "pipenu/ana/scripts/modules.hh"

def_name pipenu_pipenu_0010 ("pipenu_pipenu_ana");
//def_name murat_pipenu_0020 ("pipenu_strip_tracks");
///////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
void  pipenu_pipenu_ana(int DebugBit = -1) {
//-----------------------------------------------------------------------------
// configure analysis module
//-----------------------------------------------------------------------------
  pipenu::m_pipenu = (pipenu::TPipenuAnaModule*) g.x->AddModule("pipenu::TPipenuAnaModule",0);  

  //  pipenu::m_pipenu->SetSignalParticle(-11,14);

  printf("DebugBit:%i\n",DebugBit);
  if (DebugBit >= 0) pipenu::m_pipenu->SetDebugBit(DebugBit,1);
}

// //-----------------------------------------------------------------------------
// void  murat_strip_tracks(const char* Project, const char* ODsid) {
// //-----------------------------------------------------------------------------
// // configure analysis module *g.x" is the TStnAna thing
// //-----------------------------------------------------------------------------
//   murat::m_filter = (murat::TFilterModule*) g.x->AddModule("murat::TFilterModule",0);  
//   murat::m_filter->SetFilteringMode(1);   // 0: disabled (default), 1: filter, 2: veto

//   TStnOutputModule* m = new TStnOutputModule();
//   m->SetProject  (Project);
//   m->SetDatasetID(ODsid  );

//   g.x->SetOutputModule(m);
// }
