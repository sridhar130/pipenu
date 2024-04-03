#!/usr/bin/python

from local_classes import *
# from mixing_inputs import *

class Project(ProjectBase):

    def init_datasets(self):
#------------------------------------------------------------------------------
# datasets of this family
# 1. stage 1 : generator input
#------------------------------------------------------------------------------
        self.add_dataset(Dataset('generator'                          ,'bmup4b0s00r0000','local'))
#------------------------------------------------------------------------------
# 2. input for stage2 : datasets produced by stage1
#------------------------------------------------------------------------------
        self.add_dataset(Dataset('sim.mu2e.bmup4b0s11r0000.pipenu.art','bmup4b0s11r0000','local'))
#------------------------------------------------------------------------------
# 3. input for s3(digitization): dts. for decays in flight
#------------------------------------------------------------------------------
        self.add_dataset(Dataset('dts.mu2e.bmup4b0s24r0000.pipenu.art','bmup4b0s24r0000','local'))
#------------------------------------------------------------------------------
# s4: reconstruction and ntupling
#------------------------------------------------------------------------------
        self.add_dataset(Dataset('dig.mu2e.bmup4b0s34r0000.pipenu.art','bmup4b0s34r0000','local'))
        self.add_dataset(Dataset('mcs.mu2e.bmup4b0s44r0100.pipenu.art','bmup4b0s44r0100','local'))
        return

#------------------------------------------------------------------------------
#
#------------------------------------------------------------------------------
    def __init__(self,idsid=None):

        ProjectBase.__init__(self,project='pipenu',family_id='bmup4b0',idsid=idsid);
        self.init_datasets();
#------------------------------------------------------------------------------
# S1 10^8 proton interactions in the PT, half field in the DS
#------------------------------------------------------------------------------        
        s                            = self.new_stage('s1');
        job                          = s.new_job('sim','bmup4b0s00r0000');

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
#------------------------------------------------------------------------------
# init s1 stntuple
#------------------------------------------------------------------------------  
        job                          = s.new_job('beam_stn','bmup4b0s11r0000');

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
#------------------------------------------------------------------------------
# init stage 2. a Stage can have one or several jobs associated with it
#------------------------------------------------------------------------------        
        s                            = self.new_stage('s2');
        job                          = s.new_job('sim','bmup4b0s11r0000');

        job.fNInputFiles             = -1                     # number of segments defined by s1:sim
             
        job.fMaxInputFilesPerSegment =  50
        job.fNEventsPerSegment       =  20000
        job.fResample                = 'no'   # yes/no        # for resampling, need to define the run number again
        job.fRequestedTime           = '3h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        odsid21                      = self.fFamilyID+s.name()+'1'+'r0000';
        odsid22                      = self.fFamilyID+s.name()+'2'+'r0000';
        odsid23                      = self.fFamilyID+s.name()+'3'+'r0000';

        job.fOutputStream            = ['TargetStopOutput'            , 'ootStopOutput'               , 'IPAStopOutput'               ]
        job.fOutputDsID              = [odsid21                       , odsid22                       , odsid23                       ]
        job.fOutputFnPattern         = ['sim.mu2e.'+job.fOutputDsID[0], 'sim.mu2e.'+job.fOutputDsID[1], 'sim.mu2e.'+job.fOutputDsID[2]]
        job.fOutputFormat            = ['art'                         , 'art'                         , 'art'                         ]
#------------------------------------------------------------------------------
# s2:resample
#------------------------------------------------------------------------------        
        job                          = s.new_job('resample','bmup4b0s11r0000');

        job.fNInputFiles             = -1                     # number of segments defined by s1:sim
             
        job.fMaxInputFilesPerSegment =  1
        job.fNEventsPerSegment       =  1600000
        job.fResample                = 'yes'                  # yes/no, for resampling, need to define the run number again
        job.fResamplingModuleLabel   = 'beamResampler'
        job.fRunNumber               = 1210
        job.fRequestedTime           = '30h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        odsid                        = self.fFamilyID+s.name()+'4'+'r0000';

        job.fOutputStream            = [ 's24'             ]
        job.fOutputDsID              = [  odsid            ]
        job.fOutputFnPattern         = [ 'dts.mu2e.'+odsid ]
        job.fOutputFormat            = [ 'art'             ]
#------------------------------------------------------------------------------
# stage 3 : s3:digi_trig : InputDsID is 'bmup2b0s24r0000' (dts from decays in flight)
#           digitization job has only one output stream
#------------------------------------------------------------------------------        
        s                            = self.new_stage('s3');
        job                          = s.new_job('digi_trig',idsid);

        job.fNInputFiles             = -1                     # number of segments defined by the input dataset
             
        job.fMaxInputFilesPerSegment =  1
        job.fNEventsPerSegment       =  2000000
        job.fResample                = 'no'   # yes/no        # for resampling, need to define the run number again
        job.fRequestedTime           = '10h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        output_stream                = job.input_dataset().output_stream()
        odsid                        = self.fFamilyID+s.name()+output_stream+'r0000';

        job.fOutputStream            = ['defaultOutput'                ]
        job.fOutputDsID              = [odsid                          ]
        job.fOutputFnPattern         = ['dig.mu2e.'+job.fOutputDsID[0] ]
        job.fOutputFormat            = ['art'                          ]
#------------------------------------------------------------------------------
# s4:reco_kk : reconstruction job has only one output stream
#------------------------------------------------------------------------------        
        s                            = self.new_stage('s4');
        job                          = s.new_job('reco_kk',idsid);

        job.fNInputFiles             = -1                     # number of segments defined by the input dataset
             
        job.fMaxInputFilesPerSegment =  5                     # can combine by 5
        job.fNEventsPerSegment       =  1000000
        job.fResample                = 'no'   # yes/no        # for resampling, need to define the run number again
        job.fRequestedTime           = '10h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        output_stream                = job.input_dataset().output_stream()
        odsid                        = self.fFamilyID+s.name()+output_stream+'r0100';

        job.fOutputStream            = ['defaultOutput'   ]
        job.fOutputDsID              = [odsid             ]
        job.fOutputFnPattern         = ['mcs.mu2e.'+odsid ]
        job.fOutputFormat            = ['art'             ]
#------------------------------------------------------------------------------
# s4:stn_kk : stntupling job has only one output stream
#             no need to redefine the stage ... N(events / input file) ~ 176000
#------------------------------------------------------------------------------        
        job                          = s.new_job('stn_kk',idsid);

        job.fNInputFiles             = -1                     # number of segments defined by the input dataset
             
        job.fMaxInputFilesPerSegment =  10                    # reco_kk does x5, expect < 2GBytes files
        job.fNEventsPerSegment       =  10000000
        job.fResample                = 'no'   # yes/no        # for resampling, need to define the run number again
        job.fRequestedTime           = '5h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        output_stream                = job.input_dataset().output_stream()
        odsid                        = self.fFamilyID+s.name()+output_stream+'r0100';

        job.fOutputStream            = ['InitStntuple'    ]
        job.fOutputDsID              = [odsid             ]
        job.fOutputFnPattern         = ['nts.mu2e.'+odsid ]
        job.fOutputFormat            = ['stn'             ]
#------------------------------------------------------------------------------
# s5:track_filter : strip events with at least one track above 60 MeV/c
#------------------------------------------------------------------------------        
        s                            = self.new_stage('s5');

        job                          = s.new_job('track_filter',idsid);

        job.fNInputFiles             = -1                     # number of segments defined by the input dataset
             
        job.fMaxInputFilesPerSegment =  20
        job.fNEventsPerSegment       =  100000000
        job.fResample                = 'no'   # yes/no        # for resampling, need to define the run number again
        job.fRequestedTime           = '10h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        output_stream                = job.input_dataset().output_stream()
        odsid                        = job.family_id()+s.name()+output_stream+'r0100';

        job.fOutputStream            = ['defaultOutput'   ]
        job.fOutputDsID              = [odsid             ]
        job.fOutputFnPattern         = ['mcs.mu2e.'+odsid ]
        job.fOutputFormat            = ['art'             ]
#------------------------------------------------------------------------------
# end
#------------------------------------------------------------------------------
