///////////////////////////////////////////////////////////////////////////////
// pipenu datasets
///////////////////////////////////////////////////////////////////////////////
#ifndef __init_pipenu_datasets__
#define __init_pipenu_datasets__

#include "Stntuple/val/stn_dataset.hh"
#include "Stntuple/val/stn_book.hh"
//-----------------------------------------------------------------------------
void init_pipenu_datasets(stn_book* Book) {
  stn_dataset_t* ds;
  hist_file_t*   hf;
//-----------------------------------------------------------------------------
//  the best way to manage multiple datasets is to arrange them alphabetically
//-----------------------------------------------------------------------------
  if (strcmp(Book->GetName(),"pipenu") == 0) {
//-----------------------------------------------------------------------------
// pi+ --> e+ nu , 's51' - pi+ stops in the ST, 's54' - pion stops in the degrader
//-----------------------------------------------------------------------------
    ds = Book->NewDataset ("pipenu.bpip0b0s51r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );
    // 2 mm Ti
    ds = Book->NewDataset ("pipenu.bpip2b0s51r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );
    ds = Book->NewDataset ("pipenu.bpip2b0s54r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );
    // 3 mm Ti
    ds = Book->NewDataset ("pipenu.bpip3b0s51r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );
    ds = Book->NewDataset ("pipenu.bpip3b0s54r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );

    ds = Book->NewDataset ("pipenu.bpip4b0s51r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0001" );

    ds = Book->NewDataset ("pipenu.bpip4b0s54r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0001" );

    ds = Book->NewDataset ("pipenu.bpip5b0s51r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );
    ds = Book->NewDataset ("pipenu.bpip5b0s54r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );
//-----------------------------------------------------------------------------
// mu+ decays in flight, different degrader thicknesses
// 's54' - decay in flight datasets simulated with the [0,150 ns] cutoff
//-----------------------------------------------------------------------------
    ds = Book->NewDataset ("pipenu.bmup0b0s24r0000" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_spmc_ana.0000" );

    ds = Book->NewDataset ("pipenu.bmup0b0s25r0000" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_spmc_ana.0000" );

    ds = Book->NewDataset ("pipenu.bmup0b0s54r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );

    ds = Book->NewDataset ("pipenu.bmup2b0s54r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );

    ds = Book->NewDataset ("pipenu.bmup3b0s54r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );

    ds = Book->NewDataset ("pipenu.bmup4b0s54r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );

    ds = Book->NewDataset ("pipenu.bmup4b0s56r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );

    ds = Book->NewDataset ("pipenu.bmup4b0s66r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0001" );

    ds = Book->NewDataset ("pipenu.bmup5b0s24r0000" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_spmc_ana.0000" );

    ds = Book->NewDataset ("pipenu.bmup5b0s25r0000" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_spmc_ana.0000" );

    ds = Book->NewDataset ("pipenu.bmup5b0s54r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );

    ds = Book->NewDataset ("pipenu.bmup5b0s56r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );
//-----------------------------------------------------------------------------
// mu+ decays in flight, different timing constraints
//-----------------------------------------------------------------------------
    ds = Book->NewDataset ("pipenu.bmup9b0s24r0000" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_spmc_ana.0000" );

    ds = Book->NewDataset ("pipenu.bmup9b0s25r0000" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_spmc_ana.0000" );

    ds = Book->NewDataset ("pipenu.bmup9b0s26r0000" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_spmc_ana.0000" );

    ds = Book->NewDataset ("pipenu.bmup9b0s27r0000" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_spmc_ana.0000" );

    ds = Book->NewDataset ("pipenu.bmup9b0s28r0000" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_spmc_ana.0000" );

    ds = Book->NewDataset ("pipenu.bmup9b0s29r0000" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_spmc_ana.0000" );
//-----------------------------------------------------------------------------
// neutron pileup datasets
//-----------------------------------------------------------------------------
    ds = Book->NewDataset ("pipenu.neut0b0s31r0000" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"detstepana.0000",   "" );

    ds = Book->NewDataset ("pipenu.neut1b0s31r0000" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"detstepana.0000",   "" );
//-----------------------------------------------------------------------------
// pi- beam datasets
//-----------------------------------------------------------------------------
    ds = Book->NewDataset ("pipenu.bpim0b0s21r0000" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_spmc_ana.0000" );

    ds = Book->NewDataset ("pipenu.bpim0b0s24r0000" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_spmc_ana.0000" );

    ds = Book->NewDataset ("pipenu.bpim0b0s25r0000" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_spmc_ana.0000" );
//-----------------------------------------------------------------------------
// CE datasets
//-----------------------------------------------------------------------------
    ds = Book->NewDataset ("pipenu.cele0b0s51r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_trk_ana.0000" );

    ds = Book->NewDataset ("pipenu.cele1b0s51r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_trk_ana.0000" );

    ds = Book->NewDataset ("pipenu.cele2b0s51r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_trk_ana.0000" );

    ds = Book->NewDataset ("pipenu.cele3b0s51r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_trk_ana.0000" );
//-----------------------------------------------------------------------------
// RPC datasets
//-----------------------------------------------------------------------------
    ds = Book->NewDataset ("pipenu.rpc03b0s44r0000" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_drpc_ana.0000" );

    ds = Book->NewDataset ("pipenu.rpc04b0s44r0000" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_drpc_ana.0000" );

    ds = Book->NewDataset ("pipenu.rpc04b0s54r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_drpc_ana.0000" );

    ds = Book->NewDataset ("pipenu.rpc05b0s44r0000" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_drpc_ana.0000" );

    ds = Book->NewDataset ("pipenu.rpc07b0s54r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_drpc_ana.0000" );

  }
}

#endif
