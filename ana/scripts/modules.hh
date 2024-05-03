#ifndef pipenu_ana_scripts_modules_hh
#define pipenu_ana_scripts_modules_hh

#include "pipenu/ana/TSpmcAnaModule.hh"
#include "pipenu/ana/TTrackAnaModule.hh"
#include "pipenu/ana/THelixAnaModule.hh"
#include "pipenu/ana/TPipenuAnaModule.hh"
//-----------------------------------------------------------------------------
// modules known to clash with Stntuple and others
//-----------------------------------------------------------------------------
namespace pipenu {
  TSpmcAnaModule*          m_spmc  = NULL;
  TTrackAnaModule*         m_trk  = NULL;
  THelixAnaModule*         m_hel  = NULL;
  TPipenuAnaModule*        m_pipenu = NULL;
}

#endif

