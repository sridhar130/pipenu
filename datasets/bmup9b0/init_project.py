#!/usr/bin/python

from local_classes import *
# from mixing_inputs import *

class Project(ProjectBase):

    def init_datasets(self):
#------------------------------------------------------------------------------
# datasets of this family
# 1. stage 1 : generator input
#------------------------------------------------------------------------------
        self.add_dataset(Dataset('generator'                               ,'bmup9b0s00r0000','local'))
#------------------------------------------------------------------------------
# 2. input for stage2 : s2:resample or s2:sim 
#    in both cases trace everything 
#------------------------------------------------------------------------------
        self.add_dataset(Dataset('sim.mu2e.bmup9b0s11r0000.pipenu.art','bmup9b0s11r0000','local'))
        self.add_dataset(Dataset('sim.mu2e.bmup9b0s12r0000.pipenu.art','bmup9b0s12r0000','local'))
#------------------------------------------------------------------------------
# Input for stage3: s3:digi_trig
#------------------------------------------------------------------------------
        self.add_dataset(Dataset('sim.mu2e.bmup9b0s21r0000.pipenu.art','bmup9b0s21r0000','local'))
        self.add_dataset(Dataset('sim.mu2e.bmup9b0s22r0000.pipenu.art','bmup9b0s22r0000','local'))
#------------------------------------------------------------------------------
# Input s4 strip and s3 stn -- TargetStopOutput from s3
#------------------------------------------------------------------------------
        self.add_dataset(Dataset('sim.mu2e.bmup9b0s31r0000.pipenu.art','bmup9b0s31r0000','local'))
#------------------------------------------------------------------------------
# a job always has an input dataset, but...
#------------------------------------------------------------------------------
        if (self.fIDsID) : self.fInputDataset = self.dataset(self.fIDsID);
        return

#------------------------------------------------------------------------------
    def __init__(self,idsid=None):
        project                      = 'pipenu'
        self.fFamilyID               = 'bmup9b0'

        ProjectBase.__init__(self,project=project,family_id='bmup9b0',idsid=idsid);
        self.init_datasets();
#------------------------------------------------------------------------------
# S1 10^8 proton interactions in the PT, half field in the DS
#------------------------------------------------------------------------------        
        s                            = self.new_stage('s1');
        job                          = s.new_job('sim','bmup9b0s00r0000');

        job.fRunNumber               = 1210;
        job.fBaseFcl                 = self.base_fcl(job,'sim');

        job.fNInputFiles             = 250                      # number of segments
                                     
        job.fMaxInputFilesPerSegment =  1
        job.fNEventsPerSegment       = 400000
        job.fResample                = 'no'   # yes/no
        job.fRequestedTime           = '16h'
        job.fIfdh                    = 'xrootd'                 # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        odsid1                       = self.fFamilyID+'s11'+'r0000';
        odsid2                       = self.fFamilyID+'s12'+'r0000';

        job.fOutputStream            = ['BeamOutput'                  , 'NeutralsOutput'              ]
        job.fOutputDsID              = [odsid1                        ,  odsid2                       ] 
        job.fOutputFnPattern         = ['sim.mu2e.'+job.fOutputDsID[0], 'sim.mu2e.'+job.fOutputDsID[1]]
        job.fOutputFormat            = ['art'                         , 'art'                         ]
        
        # grid output dir
        desc                         = project+'.'+job.input_dataset().id()+'.'+s.name()+'_'+job.name()
        job.fDescription             = desc;
#------------------------------------------------------------------------------
# init s1 stntuple
#------------------------------------------------------------------------------  
        job                          = s.new_job('beam_stn','bmup9b0s11r0000');

        job.fRunNumber               = 1210;
        job.fBaseFcl                 = self.base_fcl(job,'beam_stn');

        job.fNInputFiles             = 1                                # number of segments    

        job.fMaxInputFilesPerSegment = 1000
        job.fNEventsPerSegment       = 10000000                       
        job.fResample                = 'no'                               # yes/no
        job.fMaxMemory               = '2000MB'
        job.fRequestedTime           = '2h'
        job.fIfdh                    = 'xrootd'                           # ifdh/xrootd

        job.fOutputStream            = [ 'InitStntuple'                  ]
        job.fOutputDsID              = [ odsid1                          ]
        job.fOutputFnPattern         = [ 'nts.mu2e.'+job.fOutputDsID[0]  ]
        job.fOutputFormat            = [ 'stn'                           ]

        desc                         = project+'.'+job.input_dataset().id()+'.'+s.name()+'_'+job.name()
        job.fDescription             = desc;       
#------------------------------------------------------------------------------
# init stage 2: resampling and tracing
#------------------------------------------------------------------------------        
        s                            = self.new_stage('s2');

        job                          = s.new_job('resample','bmup9b0s11r0000');

        job.fBaseFcl                 = self.base_fcl(job,'resample');

        job.fNInputFiles             = 10                     # number of segments defined by s1:sim
             
        job.fMaxInputFilesPerSegment =  1
        job.fNEventsPerSegment       =  1000000
        job.fResample                = 'yes'                  # yes/no, for resampling, need to define the run number again
        job.fResamplingModuleLabel   = 'beamResampler'
        job.fRunNumber               = 1210
        job.fRequestedTime           = '3h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        odsid                        = self.fFamilyID+s.name()+'4'+'r0000';

        job.fOutputStream            = [ 's24'             ]
        job.fOutputDsID              = [  odsid            ]
        job.fOutputFnPattern         = [ 'sim.mu2e.'+odsid ]
        job.fOutputFormat            = [ 'art'             ]

        # job description defined the grid output directory
        desc                         = project+'.'+job.input_dataset().id()+'.'+s.name()+'_'+job.name()
        job.fDescription             = desc;
#------------------------------------------------------------------------------
# s2:stn_tgt : ntuple target stops
#------------------------------------------------------------------------------  
        job                          = s.new_job('stn_tgt','bmup9b0s21r0000');

        job.fRunNumber               = 1210;
        job.fBaseFcl                 = self.base_fcl(job,'stn_tgt');

        job.fNInputFiles             = 1                                # number of segments    

        job.fMaxInputFilesPerSegment = 100
        job.fNEventsPerSegment       = 50000                       
        job.fResample                = 'no'                               # yes/no
        job.fRequestedTime           = '3h'
        job.fIfdh                    = 'ifdh'                           # ifdh/xrootd

        odsid                        = self.fFamilyID+s.name()+'4'+'r0000';

        job.fOutputStream            = [ 'InitStntuple'     ]
        job.fOutputDsID              = [ odsid              ]
        job.fOutputFnPattern         = [ 'nts.mu2e.'+odsid  ]
        job.fOutputFormat            = [ 'stn'              ]

        desc                         = project+'.'+job.input_dataset().id()+'.'+s.name()+'_'+job.name()
        job.fDescription             = desc;
#------------------------------------------------------------------------------
# s3:strip_mup
#------------------------------------------------------------------------------        
        s                            = self.new_stage('s3');

        job                          = s.new_job('strip_mup','bmup9b0s21r0000');

        job.fRunNumber               = 1210;
        job.fBaseFcl                 = self.base_fcl(job,'strip_mup');

        job.fNInputFiles             = -1                     # number of segments defined by the input datasets
             
        job.fMaxInputFilesPerSegment = 100 
        job.fNEventsPerSegment       =  5000
        job.fResample                = 'no'   # yes/no
        job.fRequestedTime           = '2h'   
        job.fIfdh                    = 'ifdh'                 # ifdh/xrootd

        odsid31                       = self.fFamilyID+'s31'+'r0000';

        job.fOutputStream            = ['muonout'                     ]
        job.fOutputDsID              = [odsid31                       ]
        job.fOutputFnPattern         = ['sim.mu2e.'+job.fOutputDsID[0]]
        job.fOutputFormat            = ['art'                         ]

        # grid output dir
        desc                         = project+'.'+job.input_dataset().id()+'.'+s.name()+'_'+job.name()
        job.fDescription             = desc;
#------------------------------------------------------------------------------
# end
#------------------------------------------------------------------------------
