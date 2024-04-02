#!/usr/bin/python

from local_classes import *
# from mixing_inputs import *

class Project(ProjectBase):
#------------------------------------------------------------------------------
# no need to have config files, can do initialization in python directly
#------------------------------------------------------------------------------
    def init_datasets(self):
#------------------------------------------------------------------------------
# datasets of this family
# 1. stage 1 : generator input
#------------------------------------------------------------------------------
        self.add_dataset(Dataset('generator'                          ,'bmup0b0s00r0000','local'))
#------------------------------------------------------------------------------
# 2. input for stage2 : datasets produced by stage1
#------------------------------------------------------------------------------
        self.add_dataset(Dataset('sim.mu2e.bmup0b0s11r0000.pipenu.art','bmup0b0s11r0000','local'))
#------------------------------------------------------------------------------
# stage3 (digitization): dts. for decays in flight
# s24: G4 forces muon decays within 150 ns in the DS
# s24: G4 defaults
#------------------------------------------------------------------------------
        self.add_dataset(Dataset('dts.mu2e.bmup0b0s24r0000.pipenu.art','bmup0b0s24r0000','local'))
        self.add_dataset(Dataset('dts.mu2e.bmup0b0s25r0000.pipenu.art','bmup0b0s25r0000','local'))
#------------------------------------------------------------------------------
# s4: reconstruction and ntupling
#------------------------------------------------------------------------------
        self.add_dataset(Dataset('dig.mu2e.bmup0b0s34r0000.pipenu.art','bmup0b0s34r0000','local'))
        self.add_dataset(Dataset('mcs.mu2e.bmup0b0s44r0100.pipenu.art','bmup0b0s44r0100','local'))
        return

#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# constructor
#------------------------------------------------------------------------------
    def __init__(self,idsid=None):

        ProjectBase.__init__(self,project='pipenu',family_id='bmup0b0',idsid=idsid);
        self.init_datasets();
#------------------------------------------------------------------------------
# S1 10^8 proton interactions in the PT, half field in the DS
# the input dataset ID is fixed, not changed via the command prompt
#------------------------------------------------------------------------------        
        s                            = self.new_stage('s1');
        job                          = s.new_job('sim','bmup0b0s00r0000');

        job.fRunNumber               = 1210;
        job.fNInputFiles             = 1000                      # number of segments
                                     
        job.fMaxInputFilesPerSegment =  1
        job.fNEventsPerSegment       = 500000
        job.fResample                = 'no'   # yes/no
        job.fRequestedTime           = '30h'
        job.fIfdh                    = 'xrootd'                 # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        odsid1                       = self.fFamilyID+'s11'+'r0000';
        odsid2                       = self.fFamilyID+'s12'+'r0000';

        job.fOutputStream            = ['BeamOutput'       ] ## , 'NeutralsOutput'   ]
        job.fOutputDsID              = [ odsid1            ] ## ,  odsid2            ] 
        job.fOutputFnPattern         = ['sim.mu2e.'+odsid1 ] ## , 'sim.mu2e.'+odsid2 ]
        job.fOutputFormat            = ['art'              ] ## , 'art'              ]
#------------------------------------------------------------------------------
# s1:stn job. The beam and non-beam output strams have different collection names,
#             thus different FCLs
#------------------------------------------------------------------------------  
        job                          = s.new_job('stn_beam','bmup0b0s11r0000');

        job.fNInputFiles             = 1                                # number of segments    

        job.fMaxInputFilesPerSegment = 1000
        job.fNEventsPerSegment       = 10000000                       
        job.fResample                = 'no'                               # yes/no
        job.fMaxMemory               = '2000MB'
        job.fRequestedTime           = '2h'
        job.fIfdh                    = 'xrootd'                           # ifdh/xrootd

        odsid                        = job.family_id()+s.name()+'1'+'r0000';

        job.fOutputStream            = [ 'InitStntuple'    ]
        job.fOutputDsID              = [ odsid1            ]
        job.fOutputFnPattern         = [ 'nts.mu2e.'+odsid ]
        job.fOutputFormat            = [ 'stn'             ]
#------------------------------------------------------------------------------
# init stage 2. a Stage can have one or several jobs associated with it
#------------------------------------------------------------------------------        
        s                            = self.new_stage('s2');
        job                          = s.new_job('sim',idsid);

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

        job.fOutputStream            = ['TargetStopOutput' , 'ootStopOutput'    , 'IPAStopOutput'     ]
        job.fOutputDsID              = [odsid21            , odsid22            , odsid23             ]
        job.fOutputFnPattern         = ['sim.mu2e.'+odsid21, 'sim.mu2e.'+odsid22, 'sim.mu2e.'+odsid23 ]
        job.fOutputFormat            = ['art'              , 'art'              , 'art'               ]
#------------------------------------------------------------------------------
# stage, s2:resample
#------------------------------------------------------------------------------        
        job                          = s.new_job('resample',idsid);

        job.fNInputFiles             = -1                     # number of segments defined by s1:sim
             
        job.fMaxInputFilesPerSegment =  1
        job.fNEventsPerSegment       =  1600000
        job.fResample                = 'yes'                  # yes/no, for resampling, need to define the run number again
        job.fResamplingModuleLabel   = 'beamResampler'
        job.fRunNumber               = 1210
        job.fRequestedTime           = '20h'   
        job.fIfdh                    = 'ifdh'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        odsid                        = self.fFamilyID+s.name()+'4'+'r0000';

        job.fOutputStream            = [ 's24'             ]
        job.fOutputDsID              = [  odsid            ]
        job.fOutputFnPattern         = [ 'dts.mu2e.'+odsid ]
        job.fOutputFormat            = [ 'art'             ]
#------------------------------------------------------------------------------
# stage, s2:resample_default, use gen_fcl with --nseg=100 - for determining the scale factor
#------------------------------------------------------------------------------        
        job                          = s.new_job('resample_default',idsid);

        job.fNInputFiles             = -1                     # number of segments defined by s1:sim
             
        job.fMaxInputFilesPerSegment =  1
        job.fNEventsPerSegment       =  1000000
        job.fResample                = 'yes'                  # yes/no, for resampling, need to define the run number again
        job.fResamplingModuleLabel   = 'beamResampler'
        job.fRunNumber               = 1210
        job.fRequestedTime           = '20h'   
        job.fIfdh                    = 'ifdh'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        odsid                        = self.fFamilyID+s.name()+'5'+'r0000';

        job.fOutputStream            = [ 's24'             ]
        job.fOutputDsID              = [  odsid            ]
        job.fOutputFnPattern         = [ 'dts.mu2e.'+odsid ]
        job.fOutputFormat            = [ 'art'             ]
#------------------------------------------------------------------------------
# s2:stn_dif : ntuple decays in flight
#              use the same fcl for s24 and s25 
#              s24 needs fMaxInputFilesPerSegment=10 
#              s25 is OK with fMaxInputFilesPerSegment=100
#------------------------------------------------------------------------------  
        job                          = s.new_job('stn_dif',idsid);

        job.fNInputFiles             = 1                    # number of segments    

        job.fMaxInputFilesPerSegment = 10                   # resulting in a 1GB file for 's24'
        job.fNEventsPerSegment       = 50000000                       
        job.fResample                = 'no'                 # yes/no
        job.fRequestedTime           = '3h'
        job.fIfdh                    = 'ifdh'               # ifdh/xrootd

        ostream                      = job.input_dataset().output_stream();
        odsid                        = self.fFamilyID+s.name()+ostream+'r0000';

        job.fOutputStream            = [ 'InitStntuple'    ]
        job.fOutputDsID              = [ odsid             ]
        job.fOutputFnPattern         = [ 'nts.mu2e.'+odsid ]
        job.fOutputFormat            = [ 'stn'             ]
#------------------------------------------------------------------------------
# stage 3 : s3:digi_trig : InputDsID is 'bpip0b0s31r0000' 
#           digitization job has only one output stream
#------------------------------------------------------------------------------        
        s                            = self.new_stage('s3');
        job                          = s.new_job('digi_trig',idsid);

        job.fNInputFiles             = -1                     # number of segments defined by the input dataset
             
        job.fMaxInputFilesPerSegment =  1
        job.fNEventsPerSegment       =  2000000
        job.fResample                = 'no'   # yes/no        # for resampling, need to define the run number again
        job.fRequestedTime           = '20h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        output_stream                = job.input_dataset().output_stream()

        odsid                        = job.family_id()+s.name()+output_stream+'r0000';

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
             
        job.fMaxInputFilesPerSegment =  1
        job.fNEventsPerSegment       =  1000000
        job.fResample                = 'no'   # yes/no        # for resampling, need to define the run number again
        job.fRequestedTime           = '10h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        output_stream                = self.fInputDataset.output_stream()
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
             
        job.fMaxInputFilesPerSegment =  10                    # based on bmup0b0, expect < 2GB
        job.fNEventsPerSegment       =  10000000
        job.fResample                = 'no'   # yes/no        # for resampling, need to define the run number again
        job.fRequestedTime           = '5h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        output_stream                = self.fInputDataset.output_stream()

        odsid                        = self.fFamilyID+'s5'+output_stream+'r0100';

        job.fOutputStream            = ['InitStntuple'    ]
        job.fOutputDsID              = [odsid             ]
        job.fOutputFnPattern         = ['nts.mu2e.'+odsid ]
        job.fOutputFormat            = ['stn'             ]
#------------------------------------------------------------------------------
# end
#------------------------------------------------------------------------------
