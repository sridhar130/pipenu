#!/usr/bin/python

from local_classes import *
# from mixing_inputs import *

class Project(ProjectBase):
    def init_datasets(self):
#------------------------------------------------------------------------------
# datasets of this family
# 1. stage 3 : generator input, trace up to the exit from TS5 coll
#-------v----------------------------------------------------------------------
        self.add_dataset(Dataset('generator'                          ,'pgun0b0s00r0000','local'))
#------------------------------------------------------------------------------
# s4: digi_trig, use only pi+ --> e+ nu  decays in the ST
#-------v----------------------------------------------------------------------
        self.add_dataset(Dataset('dts.mu2e.pgun0b0s31r0000.pipenu.art','pgun0b0s31r0000','local'))
#------------------------------------------------------------------------------
# s5: reco_kk, reco_kff 
#-------v----------------------------------------------------------------------
        self.add_dataset(Dataset('dig.mu2e.pgun0b0s41r0000.pipenu.art','pgun0b0s41r0000','local'))
#------------------------------------------------------------------------------
# s5 : stn_kk, stn_kff
#-------v----------------------------------------------------------------------
        self.add_dataset(Dataset('mcs.mu2e.pgun0b0s51r0100.pipenu.art','pgun0b0s51r0100','local'))
#------------------------------------------------------------------------------
# a job always has an input dataset, but...
#------------------------------------------------------------------------------
        self.fInputDsID = None;
        if (self.fIDsID) : self.fInputDataset = self.fDataset[self.fIDsID];


    def __init__(self,idsid=None):
        
        ProjectBase.__init__(self,project='pipenu',family_id='pgun0b0',idsid=idsid);
        self.init_datasets();
#------------------------------------------------------------------------------
# s3:gen_sim : generator
# need a different FCL because of different collection names
#------------------------------------------------------------------------------  
        s                            = self.new_stage('s3');
        job                          = s.new_job('gen_sim','pgun0b0s00r0000');

        job.fNInputFiles             = 1                                # number of segments 

        job.fMaxInputFilesPerSegment = 1
        job.fNEventsPerSegment       = 1000
        job.fResample                = 'no'                             # yes/no
        job.fRunNumber               = 1210
        job.fRequestedTime           = '3h'
        job.fIfdh                    = 'ifdh'                            # ifdh/xrootd

        odsid31                      = self.fFamilyID+s.name()+'1'+'r0000';
        job.fOutputStream            = [ 'PrimaryOutput'     ]
        job.fOutputDsID              = [ odsid31             ]
        job.fOutputFnPattern         = [ 'dts.mu2e.'+odsid31 ]
        job.fOutputFormat            = [ 'art'               ]
#------------------------------------------------------------------------------
# s3:stn_dts : ntuple output of detector steps
#------------------------------------------------------------------------------  
#        job                          = s.new_job('stn_dts','pgun0b0s31r0000');
#
#        job.fNInputFiles             = -1                                # defined by the input DS    
#
#        job.fMaxInputFilesPerSegment = 100
#        job.fNEventsPerSegment       = 50000000                       
#        job.fResample                = 'no'                               # yes/no
#        job.fRequestedTime           = '3h'
#        job.fIfdh                    = 'ifdh'                           # ifdh/xrootd
#
#        odsid                        = self.fFamilyID+s.name()+'1'+'r0000';
#        job.fOutputStream            = [ 'InitStntuple'    ]
#        job.fOutputDsID              = [ odsid             ]
#        job.fOutputFnPattern         = [ 'nts.mu2e.'+odsid ]
#        job.fOutputFormat            = [ 'stn'             ]
#------------------------------------------------------------------------------
# stage 4
# s4:digi_trig : InputDsID is 'pgun0b0s31r0000' 
#                digitization job has only one output stream
#------------------------------------------------------------------------------        
        s                            = self.new_stage('s4');
        job                          = s.new_job('digi_trig',idsid);

        job.fNInputFiles             = -1                     # number of segments defined by the input dataset
             
        job.fMaxInputFilesPerSegment =  50
        job.fNEventsPerSegment       =  20000
        job.fResample                = 'no'   # yes/no        # for resampling, need to define the run number again
        job.fRequestedTime           = '3h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        output_stream                = self.fInputDataset.output_stream()

        odsid                        = self.fFamilyID+s.name()+output_stream+'r0000';

        job.fOutputStream            = ['defaultOutput'                ]
        job.fOutputDsID              = [odsid                          ]
        job.fOutputFnPattern         = ['dig.mu2e.'+job.fOutputDsID[0] ]
        job.fOutputFormat            = ['art'                          ]
#------------------------------------------------------------------------------
# stage 5
# s5:reco_kff : InputDsID is 'pgun0b0s41r0000' 
#               reconstruction job has only one output stream
#------------------------------------------------------------------------------        
        s                            = self.new_stage('s5');
        job                          = s.new_job('reco_kff',idsid);

        job.fNInputFiles             = -1                     # number of segments defined by the input dataset
             
        job.fMaxInputFilesPerSegment =  50
        job.fNEventsPerSegment       =  20000
        job.fResample                = 'no'   # yes/no        # for resampling, need to define the run number again
        job.fRequestedTime           = '3h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        output_stream                = self.fInputDataset.output_stream()

        odsid                        = self.fFamilyID+'s5'+output_stream+'r0000';

        job.fOutputStream            = ['defaultOutput'                ]
        job.fOutputDsID              = [odsid                          ]
        job.fOutputFnPattern         = ['mcs.mu2e.'+job.fOutputDsID[0] ]
        job.fOutputFormat            = ['art'                          ]
#------------------------------------------------------------------------------
# stage 5
# s5:reco_kk : InputDsID is 'pgun0b0s41r0000' 
#              reconstruction job has only one output stream
#------------------------------------------------------------------------------        
        job                          = s.new_job('reco_kk',idsid);

        job.fNInputFiles             = -1                     # number of segments defined by the input dataset
             
        job.fMaxInputFilesPerSegment =  50
        job.fNEventsPerSegment       =  20000
        job.fResample                = 'no'   # yes/no        # for resampling, need to define the run number again
        job.fRequestedTime           = '3h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        output_stream                = self.fInputDataset.output_stream()

        odsid                        = self.fFamilyID+s.name()+output_stream+'r0100';

        job.fOutputStream            = ['defaultOutput'                ]
        job.fOutputDsID              = [odsid                          ]
        job.fOutputFnPattern         = ['mcs.mu2e.'+job.fOutputDsID[0] ]
        job.fOutputFormat            = ['art'                          ]
#------------------------------------------------------------------------------
# s5:stn_kff : InputDsID is 'pgun0b0s51r0000' 
#              stntupling job has only one output stream
#------------------------------------------------------------------------------        
        job                          = s.new_job('stn_kff',idsid);

        job.fNInputFiles             = -1                     # number of segments defined by the input dataset
             
        job.fMaxInputFilesPerSegment =  50
        job.fNEventsPerSegment       =  20000
        job.fResample                = 'no'   # yes/no        # for resampling, need to define the run number again
        job.fRequestedTime           = '3h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        output_stream                = self.fInputDataset.output_stream()

        odsid                        = self.fFamilyID+s.name()+output_stream+'r0000';

        job.fOutputStream            = ['defaultOutput'                ]
        job.fOutputDsID              = [odsid                          ]
        job.fOutputFnPattern         = ['nts.mu2e.'+job.fOutputDsID[0] ]
        job.fOutputFormat            = ['stn'                          ]
#------------------------------------------------------------------------------
# s5:stn_kk : stntupling job has only one output stream
#             no ned to redefine the stage
#------------------------------------------------------------------------------        
        job                          = s.new_job('stn_kk',idsid);

        job.fNInputFiles             = -1                     # number of segments defined by the input dataset
             
        job.fMaxInputFilesPerSegment =  50
        job.fNEventsPerSegment       =  100000
        job.fResample                = 'no'   # yes/no        # for resampling, need to define the run number again
        job.fRequestedTime           = '3h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        output_stream                = self.fInputDataset.output_stream()

        odsid                        = self.fFamilyID+s.name()+output_stream+'r0100';

        job.fOutputStream            = ['InitStntuple'    ]
        job.fOutputDsID              = [odsid             ]
        job.fOutputFnPattern         = ['nts.mu2e.'+odsid ]
        job.fOutputFormat            = ['stn'             ]
#------------------------------------------------------------------------------
# end
#------------------------------------------------------------------------------
