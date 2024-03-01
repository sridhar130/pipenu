#ifndef pipenu_ana_scripts_modules_hh
#define pipenu_ana_scripts_modules_hh

#include "pipenu/ana/TSpmcAnaModule.hh"
#include "pipenu/ana/TTrackAnaModule.hh"
//-----------------------------------------------------------------------------
// modules known to clash with Stntuple and others
//-----------------------------------------------------------------------------
namespace pipenu {
  TSpmcAnaModule*          m_spmc  = NULL;
  TTrackAnaModule*         m_trk  = NULL;
}

#endif

