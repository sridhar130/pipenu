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

    ds = Book->NewDataset ("pipenu.bpip2b0s51r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );
    ds = Book->NewDataset ("pipenu.bpip2b0s54r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );

    ds = Book->NewDataset ("pipenu.bpip3b0s51r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );
    ds = Book->NewDataset ("pipenu.bpip3b0s54r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );

    ds = Book->NewDataset ("pipenu.bpip4b0s51r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );
    ds = Book->NewDataset ("pipenu.bpip4b0s54r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );

    ds = Book->NewDataset ("pipenu.bpip5b0s51r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );
    ds = Book->NewDataset ("pipenu.bpip5b0s54r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );
//-----------------------------------------------------------------------------
// mu+ decays in flight, different degrader thicknesses
// 's54' - decay in flight datasets simulated with the [0,150 ns] cutoff
//-----------------------------------------------------------------------------
    ds = Book->NewDataset ("pipenu.bmup0b0s54r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );

    ds = Book->NewDataset ("pipenu.bmup2b0s54r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );

    ds = Book->NewDataset ("pipenu.bmup3b0s54r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );

    ds = Book->NewDataset ("pipenu.bmup4b0s54r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );

    ds = Book->NewDataset ("pipenu.bmup5b0s54r0100" ,"",-1,   1000000000); 
    hf = Book->NewHistFile(ds->id()            ,"","murat_pipenu_ana.0000" );
  }
}

#endif
