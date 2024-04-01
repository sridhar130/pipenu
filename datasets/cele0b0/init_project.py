#!/usr/bin/python

from local_classes import *
# from mixing_inputs import *

class Project(ProjectBase):

    def init_datasets(self):
#------------------------------------------------------------------------------
# s3. input for stage3 (simulation of mu- --> e- conversion) : muons stopped in the ST
#-------v----------------------------------------------------------------------
        self.add_dataset(Dataset('sim.mu2e.cele0b0s21r0000.pipenu.art','cele0b0s21r0000','local'))
#------------------------------------------------------------------------------
# s4: digi_trig, use only pi+ --> e+ nu  decays in the ST
#-------v----------------------------------------------------------------------
        self.add_dataset(Dataset('dts.mu2e.cele0b0s31r0000.pipenu.art','cele0b0s31r0000','local'))
#------------------------------------------------------------------------------
# s5: reco_kk, reco_kff 
#-------v----------------------------------------------------------------------
        self.add_dataset(Dataset('dig.mu2e.cele0b0s41r0000.pipenu.art','cele0b0s41r0000','local'))
#------------------------------------------------------------------------------
# s5 : stn_kk, stn_kff
#-------v----------------------------------------------------------------------
        self.add_dataset(Dataset('mcs.mu2e.cele0b0s51r0000.pipenu.art','cele0b0s51r0000','local'))
        self.add_dataset(Dataset('mcs.mu2e.cele0b0s51r0100.pipenu.art','cele0b0s51r0100','local'))
        return

    def __init__(self,idsid=None):

        ProjectBase.__init__(self,project='pipenu',family_id='cele0b0',idsid=idsid);
        self.init_datasets();
#------------------------------------------------------------------------------
# s3:gen_sim_tgt : pi+ --> e+ nu decays of pions stopped in the ST
# need a different FCL because of different collection names
#------------------------------------------------------------------------------  
        s                            = self.new_stage('s3');
        job                          = s.new_job('gen_sim_tgt',idsid);

        job.fNInputFiles             = 1                                # number of segments 

        job.fMaxInputFilesPerSegment = 1
        job.fNEventsPerSegment       = 1000
        job.fResample                = 'yes'                             # yes/no
        job.fResamplingModuleLabel   = 'TargetStopResampler'
        job.fRunNumber               = 1210
        job.fRequestedTime           = '3h'
        job.fIfdh                    = 'ifdh'                            # ifdh/xrootd

        odsid                        = self.fFamilyID+s.name()+'1'+'r0000';
        job.fOutputStream            = [ 'PrimaryOutput'   ]
        job.fOutputDsID              = [ odsid             ]
        job.fOutputFnPattern         = [ 'dts.mu2e.'+odsid ]
        job.fOutputFormat            = [ 'art'             ]
#------------------------------------------------------------------------------
# s3:stn_dts : ntuple output of detector steps
#------------------------------------------------------------------------------  
        job                          = s.new_job('stn_dts','cele0b0s31r0000');

        job.fNInputFiles             = -1                                # defined by the input DS    

        job.fMaxInputFilesPerSegment = 100
        job.fNEventsPerSegment       = 50000000                       
        job.fResample                = 'no'                               # yes/no
        job.fRequestedTime           = '3h'
        job.fIfdh                    = 'ifdh'                           # ifdh/xrootd

        odsid                        = self.fFamilyID+s.name()+'1'+'r0000';
        job.fOutputStream            = [ 'InitStntuple'    ]
        job.fOutputDsID              = [ odsid             ]
        job.fOutputFnPattern         = [ 'nts.mu2e.'+odsid ]
        job.fOutputFormat            = [ 'stn'             ]
#------------------------------------------------------------------------------
# stage 4
# s4:digi_trig : InputDsID is 'cele0b0s31r0000' 
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
        odsid                        = self.fFamilyID+'s4'+output_stream+'r0000';

        job.fOutputStream            = ['defaultOutput'                ]
        job.fOutputDsID              = [odsid                          ]
        job.fOutputFnPattern         = ['dig.mu2e.'+job.fOutputDsID[0] ]
        job.fOutputFormat            = ['art'                          ]
#------------------------------------------------------------------------------
# s4:digi_trig_early_000 : r0001
#------------------------------------------------------------------------------        
        job                          = s.new_job('digi_trig_early_000',idsid);

        job.fNInputFiles             = -1                     # number of segments defined by the input dataset
             
        job.fMaxInputFilesPerSegment =  50
        job.fNEventsPerSegment       =  20000
        job.fResample                = 'no'   # yes/no        # for resampling, need to define the run number again
        job.fRequestedTime           = '3h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        output_stream                = self.fInputDataset.output_stream()
        odsid                        = self.fFamilyID+'s4'+output_stream+'r0001';

        job.fOutputStream            = ['defaultOutput'                ]
        job.fOutputDsID              = [odsid                          ]
        job.fOutputFnPattern         = ['dig.mu2e.'+job.fOutputDsID[0] ]
        job.fOutputFormat            = ['art'                          ]
#------------------------------------------------------------------------------
# s4:digi_trig_early_200 : r0002
#------------------------------------------------------------------------------        
        job                          = s.new_job('digi_trig_early_200',idsid);

        job.fNInputFiles             = -1                     # number of segments defined by the input dataset
             
        job.fMaxInputFilesPerSegment =  50
        job.fNEventsPerSegment       =  20000
        job.fResample                = 'no'   # yes/no        # for resampling, need to define the run number again
        job.fRequestedTime           = '3h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        output_stream                = self.fInputDataset.output_stream()
        odsid                        = self.fFamilyID+'s4'+output_stream+'r0002';

        job.fOutputStream            = ['defaultOutput'                ]
        job.fOutputDsID              = [odsid                          ]
        job.fOutputFnPattern         = ['dig.mu2e.'+job.fOutputDsID[0] ]
        job.fOutputFormat            = ['art'                          ]
#------------------------------------------------------------------------------
# stage 5
# s5:reco_kff : InputDsID is 'cele0b0s41r0000' 
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
# s5:reco_kk : InputDsID is 'cele0b0s41r0000' 
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
# s5:stn_kff : InputDsID is 'cele0b0s51r0000' 
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

        job.fOutputStream            = ['InitStntuple'                 ]
        job.fOutputDsID              = [odsid                          ]
        job.fOutputFnPattern         = ['nts.mu2e.'+job.fOutputDsID[0] ]
        job.fOutputFormat            = ['art'                          ]
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
