#!/usr/bin/python

from local_classes import *
# from mixing_inputs import *

class Project(ProjectBase):
    def init_datasets(self):
#------------------------------------------------------------------------------
# datasets of this family
# 1. stage 3 : generator input, trace up to the exit from TS5 coll
#-------v----------------------------------------------------------------------
        self.add_dataset(Dataset('generator'                            ,'neut0b0s00r0000','local'))
        self.add_dataset(Dataset('sim.mu2e.MuminusStopsCat.MDC2020p.art','neut0b0s21r0000','local'))
        self.add_dataset(Dataset('dts.mu2e.neut0b0s31r0000.pipenu.art'  ,'neut0b0s31r0000','local'))
#------------------------------------------------------------------------------
# a job always has an input dataset, but...
#------------------------------------------------------------------------------
        self.fInputDsID = None;
        if (self.fIDsID) : self.fInputDataset = self.fDataset[self.fIDsID];


    def __init__(self,idsid=None):
        
        ProjectBase.__init__(self,project='pipenu',family_id='neut0b0',idsid=idsid);
        self.init_datasets();
#------------------------------------------------------------------------------
# s3:gen_sim : generator
# need a different FCL because of different collection names
#------------------------------------------------------------------------------  
        s                            = self.new_stage('s3');
        job                          = s.new_job('gen_sim',idsid);

        job.fNInputFiles             = 20                               # number of segments 

        job.fMaxInputFilesPerSegment = 1
        job.fNEventsPerSegment       = 500000
        job.fResample                = 'yes'                            # yes/no
        job.fResamplingModuleLabel   = 'TargetStopResampler'

        job.fRunNumber               = 1210
        job.fRequestedTime           = '10h'
        job.fIfdh                    = 'xrootd'                         # ifdh/xrootd

        odsid31                      = self.fFamilyID+s.name()+'1'+'r0000';
        job.fOutputStream            = [ 'PrimaryOutput'     ]
        job.fOutputDsID              = [ odsid31             ]
        job.fOutputFnPattern         = [ 'dts.mu2e.'+odsid31 ]
        job.fOutputFormat            = [ 'art'               ]
#------------------------------------------------------------------------------
# s3:stn_dts : ntuple output of detector steps
#------------------------------------------------------------------------------  
#        job                          = s.new_job('stn_dts','neut0b0s31r0000');
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
# s4:digi_trig : InputDsID is 'neut0b0s31r0000' 
#                digitization job has only one output stream
#------------------------------------------------------------------------------        
        s                            = self.new_stage('s4');
        job                          = s.new_job('digi_trig',idsid);

        job.fNInputFiles             = -1                     # number of segments defined by the input dataset
             
        job.fMaxInputFilesPerSegment =  50
        # job.fNEventsPerSegment       =  20000
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
# end
#------------------------------------------------------------------------------
