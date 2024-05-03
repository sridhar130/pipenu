//
#include "TInterpreter.h"
#include "pipenu/ana/scripts/modules.hh"
//-----------------------------------------------------------------------------
int load_stnana_scripts_pipenu() {
  char        macro[200];
  const char* script[] = { 
    //    "global_vars.cc",
    //    "init_geometry.C",
    "spmc.C", "PWD",
    "trkpipenu.C","PWD",
    "helix.C","PWD",
    "pipenu.C","PWD",
    0 };

  TString work_dir = gEnv->GetValue("Stnana.TestReleaseDir",gSystem->Getenv("PWD"));

  TInterpreter* cint = gROOT->GetInterpreter();
  
  for (int i=0; script[i] != 0; i+=2) {
    const char* dir = gSystem->Getenv(script[i+1]);
    if (dir) {
      sprintf(macro,"%s/pipenu/ana/scripts/%s",dir,script[i]);
      if (! cint->IsLoaded(macro)) cint->LoadMacro(macro);
    }
  }
  
  return 0;
}
