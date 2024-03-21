#!/usr/bin/python

from local_classes import *
# from mixing_inputs import *

class Project(ProjectBase):

    def __init__(self,idsid=None):

        ProjectBase.__init__(self,project='pipenu',family_id='bpip0b0',idsid=idsid);
#------------------------------------------------------------------------------
# datasets of this family
# 1. stage 1 : generator input, trace up to the exit from TS5 coll
#-------v----------------------------------------------------------------------
        self.add_dataset(Dataset('generator'                   ,'bpip0b0s00r0000','local'))
#------------------------------------------------------------------------------
# 2. input for stage2 : 
#-------v----------------------------------------------------------------------
        self.add_dataset(Dataset('sim.mu2e.bpip0b0s11r0000.pipenu.art','bpip0b0s11r0000','local'))
        self.add_dataset(Dataset('sim.mu2e.bpip0b0s12r0000.pipenu.art','bpip0b0s12r0000','local'))
#------------------------------------------------------------------------------
# s3. input for stage3: pions stopped in the ST .. note: no degrader, so no s24
#-------v----------------------------------------------------------------------
        self.add_dataset(Dataset('sim.mu2e.bpip0b0s21r0000.pipenu.art','bpip0b0s21r0000','local'))
#------------------------------------------------------------------------------
# s4: digi_trig, use only pi+ --> e+ nu  decays in the ST
#-------v----------------------------------------------------------------------
        self.add_dataset(Dataset('dts.mu2e.bpip0b0s31r0000.pipenu.art','bpip0b0s31r0000','local'))
#------------------------------------------------------------------------------
# s5: reco_kk, reco_kff 
#-------v----------------------------------------------------------------------
        self.add_dataset(Dataset('dig.mu2e.bpip0b0s41r0000.pipenu.art','bpip0b0s41r0000','local'))
        self.add_dataset(Dataset('dig.mu2e.bpip0b0s42r0000.pipenu.art','bpip0b0s42r0000','local'))
#------------------------------------------------------------------------------
# s5 : stn_kk, stn_kff
#-------v----------------------------------------------------------------------
        self.add_dataset(Dataset('mcs.mu2e.bpip0b0s51r0000.pipenu.art','bpip0b0s51r0000','local'))
        self.add_dataset(Dataset('mcs.mu2e.bpip0b0s51r0100.pipenu.art','bpip0b0s51r0100','local'))
        self.add_dataset(Dataset('mcs.mu2e.bpip0b0s52r0000.pipenu.art','bpip0b0s52r0000','local'))
#------------------------------------------------------------------------------
# a job always has an input dataset, but...
#------------------------------------------------------------------------------
        self.fInputDsID = None;
        if (idsid) : self.fInputDataset = self.fDataset[idsid];
#------------------------------------------------------------------------------
# S1 10^8 (250x400,000) proton interactions in the PT, half field in the DS
#-------v----------------------------------------------------------------------        
        s                            = self.new_stage('s1');
        job                          = s.new_job('sim',idsid);

        job.fRunNumber               = 1210;
        job.fBaseFcl                 = self.base_fcl(job,'sim');

        job.fNInputFiles             = 250                      # number of segments, for stopped pions don't need more
                                     
        job.fMaxInputFilesPerSegment = 1
        job.fNEventsPerSegment       = 400000
        job.fResample                = 'no'   # yes/no
        job.fRequestedTime           = '25h'
        job.fIfdh                    = 'xrootd'                 # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        odsid1                       = self.fFamilyID+'s11'+'r0000';
        odsid2                       = self.fFamilyID+'s12'+'r0000';

        job.fOutputStream            = ['BeamOutput'                  , 'NeutralsOutput'              ]
        job.fOutputDsID              = [odsid1                        ,  odsid2                       ] 
        job.fOutputFnPattern         = ['sim.mu2e.'+job.fOutputDsID[0], 'sim.mu2e.'+job.fOutputDsID[1]]
        job.fOutputFormat            = ['art'                         , 'art'                         ]
        
        # grid output dir
        desc                         = self.name()+'.'+job.input_dataset().id()+'.'+s.name()+'_'+job.name()
        job.fDescription             = desc;
#------------------------------------------------------------------------------
# s1:stn_beam: stntuple the beam stream
#------------------------------------------------------------------------------  
        job                          = s.new_job('stn_beam','bpip0b0s11r0000');

        job.fRunNumber               = 1210;
        job.fBaseFcl                 = self.base_fcl(job,'stn_beam');

        job.fNInputFiles             = 1                                # number of segments    

        job.fMaxInputFilesPerSegment = 500
        job.fNEventsPerSegment       = 100000                       
        job.fResample                = 'no'                               # yes/no
        job.fMaxMemory               = '2000MB'
        job.fRequestedTime           = '2h'
        job.fIfdh                    = 'xrootd'                           # ifdh/xrootd

        job.fOutputStream            = [ 'InitStntuple'                  ]
        job.fOutputDsID              = [ odsid1                          ]
        job.fOutputFnPattern         = [ 'nts.mu2e.'+job.fOutputDsID[0]  ]
        job.fOutputFormat            = [ 'stn'                           ]

        desc                         = self.name()+'.'+job.input_dataset().id()+'.'+s.name()+'_'+job.name()
        job.fDescription             = desc;       
#------------------------------------------------------------------------------
# s2:sim : 
#------------------------------------------------------------------------------        
        s                            = self.new_stage('s2');

        job                          = s.new_job('sim','bpip0b0s11r0000');

        job.fBaseFcl                 = self.base_fcl(job,'sim');

        job.fNInputFiles             = -1                     # number of segments defined by s1:sim
             
        job.fMaxInputFilesPerSegment =  50
        job.fNEventsPerSegment       =  20000
        job.fResample                = 'no'   # yes/no        # for resampling, need to define the run number again
        job.fRequestedTime           = '3h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        odsid21                      = self.fFamilyID+'s21'+'r0000';
        odsid22                      = self.fFamilyID+'s22'+'r0000';
        odsid23                      = self.fFamilyID+'s23'+'r0000';

        job.fOutputStream            = ['TargetStopOutput'            , 'ootStopOutput'               , 'IPAStopOutput'               ]
        job.fOutputDsID              = [odsid21                       , odsid22                       , odsid23                       ]
        job.fOutputFnPattern         = ['sim.mu2e.'+job.fOutputDsID[0], 'sim.mu2e.'+job.fOutputDsID[1], 'sim.mu2e.'+job.fOutputDsID[2]]
        job.fOutputFormat            = ['art'                         , 'art'                         , 'art'                         ]

        # job description defined the grid output directory
        desc                         = self.name()+'.'+job.input_dataset().id()+'.'+s.name()+'_'+job.name()
        job.fDescription             = desc;
#------------------------------------------------------------------------------
# s2:concat: concatenation of the input dataset, has to be defined ! 
#------------------------------------------------------------------------------        
        job                          = s.new_job('concat',idsid);

        job.fBaseFcl                 = self.base_fcl(job,'concat');

        job.fNInputFiles             = -1                     # number of segments defined by s1:sim
             
        job.fMaxInputFilesPerSegment =  50
        job.fNEventsPerSegment       =  20000000
        job.fResample                = 'no'   # yes/no        # for resampling, need to define the run number again
        job.fRequestedTime           = '3h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        odsid                        = idsid;                 # this is concatenation...

        job.fOutputStream            = ['defaultOutput'                ]
        job.fOutputDsID              = [odsid                          ]
        job.fOutputFnPattern         = ['sim.mu2e.'+job.fOutputDsID[0] ]
        job.fOutputFormat            = ['art'                          ]

        # job description defined the grid output directory
        desc                         = self.name()+'.'+job.input_dataset().id()+'.'+s.name()+'_'+job.name()
        job.fDescription             = desc;
#------------------------------------------------------------------------------
# s2:tgt_stn
#------------------------------------------------------------------------------  
        job                          = s.new_job('stn_tgt','bpip0b0s21r0000');

        job.fRunNumber               = 1210;
        job.fBaseFcl                 = self.base_fcl(job,'stn_tgt');

        job.fNInputFiles             = 10                                # number of segments    

        job.fMaxInputFilesPerSegment = 10
        job.fNEventsPerSegment       = 50000                       
        job.fResample                = 'no'                               # yes/no
        job.fRequestedTime           = '3h'
        job.fIfdh                    = 'ifdh'                           # ifdh/xrootd

        job.fOutputStream            = [ 'InitStntuple'                  ]
        job.fOutputDsID              = [ odsid21                         ]
        job.fOutputFnPattern         = [ 'nts.mu2e.'+job.fOutputDsID[0]  ]
        job.fOutputFormat            = [ 'stn'                           ]

        desc                         = self.name()+'.'+job.input_dataset().id()+'.'+s.name()+'_'+job.name()
        job.fDescription             = desc;
#------------------------------------------------------------------------------
# s3:gen_sim_tgt : pi+ --> e+ nu decays of pions stopped in the ST
# need a different FCL because of different collection names
#------------------------------------------------------------------------------  
        s                            = self.new_stage('s3');

        job                          = s.new_job('gen_sim_tgt',idsid);

        job.fRunNumber               = 1210;
        job.fBaseFcl                 = self.base_fcl(job,'gen_sim_tgt');

        job.fNInputFiles             = 1                                # number of segments 

        job.fMaxInputFilesPerSegment = 1
        job.fNEventsPerSegment       = 100000
        job.fResample                = 'yes'                             # yes/no
        job.fRequestedTime           = '3h'
        job.fIfdh                    = 'ifdh'                            # ifdh/xrootd

        odsid31                      = self.fFamilyID+s.name()+'1'+'r0000';
        job.fOutputStream            = [ 'PrimaryOutput'                 ]
        job.fOutputDsID              = [ odsid31                         ]
        job.fOutputFnPattern         = [ 'dts.mu2e.'+job.fOutputDsID[0]  ]
        job.fOutputFormat            = [ 'art'                           ]

        desc                         = self.name()+'.'+job.input_dataset().id()+'.'+s.name()+'_'+job.name()
        job.fDescription             = desc;
#------------------------------------------------------------------------------
# stage 4
# s4:digi_trig : InputDsID is 'bpip0b0s31r0000' 
#                digitization job has only one output stream
#------------------------------------------------------------------------------        
        s                            = self.new_stage('s4');

        job                          = s.new_job('digi_trig',idsid);

        job.fBaseFcl                 = self.base_fcl(job,'digi_trig');

        job.fNInputFiles             = -1                     # number of segments defined by the input dataset
             
        job.fMaxInputFilesPerSegment =  50
        job.fNEventsPerSegment       =  20000
        job.fResample                = 'no'   # yes/no        # for resampling, need to define the run number again
        job.fRequestedTime           = '3h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        output_stream                = self.fInputDataset.output_stream()

        odsid                        = self.fFamilyID+'s4'+output_stream+'r0000';

        job.fOutputStream            = ['defaultOutput'                ]
        job.fOutputDsID              = [odsid                          ]
        job.fOutputFnPattern         = ['sim.mu2e.'+job.fOutputDsID[0] ]
        job.fOutputFormat            = ['art'                          ]

        # job description defined the grid output directory
        desc                         = self.name()+'.'+job.input_dataset().id()+'.'+s.name()+'_'+job.name()
        job.fDescription             = desc;
#------------------------------------------------------------------------------
# stage 5
# s5:reco_kff : InputDsID is 'bpip0b0s41r0000' 
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
        job.fOutputFnPattern         = ['sim.mu2e.'+job.fOutputDsID[0] ]
        job.fOutputFormat            = ['art'                          ]

        # job description defined the grid output directory
        desc                         = self.name()+'.'+job.input_dataset().id()+'.'+s.name()+'_'+job.name()
        job.fDescription             = desc;
#------------------------------------------------------------------------------
# stage 5
# s5:reco_kk : InputDsID is 'bpip0b0s41r0000' 
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

        odsid                        = self.fFamilyID+s.name()+output_stream+'r0000';

        job.fOutputStream            = ['defaultOutput'                ]
        job.fOutputDsID              = [odsid                          ]
        job.fOutputFnPattern         = ['sim.mu2e.'+job.fOutputDsID[0] ]
        job.fOutputFormat            = ['art'                          ]

        # job description defined the grid output directory
        job.fDescription             = self.job_description(job);
#------------------------------------------------------------------------------
# s5:stn_kff : InputDsID is 'bpip0b0s51r0000' 
#              stntupling job has only one output stream
#------------------------------------------------------------------------------        
        job                          = s.new_job('stn_kff',idsid);

        job.fBaseFcl                 = self.base_fcl(job,'stn_kff');

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
        job.fOutputFnPattern         = ['sim.mu2e.'+job.fOutputDsID[0] ]
        job.fOutputFormat            = ['art'                          ]

        # job description defined the grid output directory
        job.fDescription             = self.job_description(job);
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
        job.fOutputFnPattern         = ['sim.mu2e.'+odsid ]
        job.fOutputFormat            = ['stn'             ]

        # job description defined the grid output directory
        job.fDescription             = self.job_description(job);
#------------------------------------------------------------------------------
# end
#------------------------------------------------------------------------------
