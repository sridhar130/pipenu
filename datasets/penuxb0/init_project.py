#!/usr/bin/python

from local_classes import *
# from mixing_inputs import *

class Project(ProjectBase):

    def init_datasets(self):
        # print("Project=%s:%s init_datasets"%(self.name(),self.family_id()));
#------------------------------------------------------------------------------
# s3. input for stage3 (simulation of pi+ --> e+ nu) : pions stopped in the ST .. note: no degrader
#     different distributions for stopped pions
#-------v----------------------------------------------------------------------
        self.add_dataset(Dataset('sim.mu2e.penuxb0s21r0000.pipenu.art','penuxb0s21r0000','local'))
        self.add_dataset(Dataset('sim.mu2e.penuxb0s21r0002.pipenu.art','penuxb0s21r0002','local'))
#------------------------------------------------------------------------------
# s4: digi_trig, use only pi+ --> e+ nu  decays in the ST
#-------v----------------------------------------------------------------------
        self.add_dataset(Dataset('dts.mu2e.penuxb0s31r0000.pipenu.art','penuxb0s31r0000','local'))
        self.add_dataset(Dataset('dts.mu2e.penuxb0s32r0000.pipenu.art','penuxb0s32r0000','local'))
#------------------------------------------------------------------------------
# s5: reco_kk, different geometries
#-------v----------------------------------------------------------------------
        self.add_dataset(Dataset('dig.mu2e.penuxb0s41r0000.pipenu.art','penuxb0s41r0000','local'))
        self.add_dataset(Dataset('dig.mu2e.penuxb0s42r0000.pipenu.art','penuxb0s42r0000','local'))
#------------------------------------------------------------------------------
# s5 : stn_kk, stn_kff
#-------v----------------------------------------------------------------------
        self.add_dataset(Dataset('mcs.mu2e.penuxb0s51r0100.pipenu.art','penuxb0s51r0100','local'))
        self.add_dataset(Dataset('mcs.mu2e.penuxb0s52r0100.pipenu.art','penuxb0s52r0100','local'))
        return;


    def __init__(self,idsid=None):

        ProjectBase.__init__(self,project='pipenu',family_id='penuxb0',idsid=idsid);
#------------------------------------------------------------------------------
# s3:gen_sim_000 : pi+ --> e+ nu decays of pions stopped in the ST
# need a different FCL because of different collection names
#------------------------------------------------------------------------------  
        s                            = self.new_stage('s3');
        job                          = s.new_job('gen_sim_000',idsid);

        job.fNInputFiles             = 1                                # number of segments 

        job.fMaxInputFilesPerSegment = 1
        job.fNEventsPerSegment       = 1000
        job.fResample                = 'yes'                             # yes/no
        job.fResamplingModuleLabel   = 'TargetStopResampler'
        job.fRunNumber               = 1210
        job.fRequestedTime           = '3h'
        job.fIfdh                    = 'ifdh'                            # ifdh/xrootd

        ostream                      = '1'
        odsid                        = self.fFamilyID+s.name()+ostream+'r0000';

        job.fOutputStream            = [ 'PrimaryOutput'   ]
        job.fOutputDsID              = [ odsid             ]
        job.fOutputFnPattern         = [ 'dts.mu2e.'+odsid ]
        job.fOutputFormat            = [ 'art'             ]
#------------------------------------------------------------------------------
# s3:gen_sim_002 : pi+ --> e+ nu decays of pions stopped in the ST 2m Ti degrader
# need a different FCL because of different collection names
#------------------------------------------------------------------------------  
        job                          = s.new_job('gen_sim_002',idsid);

        job.fNInputFiles             = 1                                # number of segments 

        job.fMaxInputFilesPerSegment = 1
        job.fNEventsPerSegment       = 1000
        job.fResample                = 'yes'                             # yes/no
        job.fResamplingModuleLabel   = 'TargetStopResampler'
        job.fRunNumber               = 1210
        job.fRequestedTime           = '3h'
        job.fIfdh                    = 'ifdh'                            # ifdh/xrootd

        ostream                      = '2';
        odsid                        = self.fFamilyID+s.name()+ostream+'r0000';

        job.fOutputStream            = [ 'PrimaryOutput'   ]
        job.fOutputDsID              = [ odsid             ]
        job.fOutputFnPattern         = [ 'dts.mu2e.'+odsid ]
        job.fOutputFormat            = [ 'art'             ]
#------------------------------------------------------------------------------
# s3:stn_dts : ntuple output of detector steps
#------------------------------------------------------------------------------  
        job                          = s.new_job('stn_dts',idsid);

        job.fNInputFiles             = -1                                # defined by the input DS    

        job.fMaxInputFilesPerSegment = 100
        job.fNEventsPerSegment       = 50000000                       
        job.fResample                = 'no'                               # yes/no
        job.fRequestedTime           = '3h'
        job.fIfdh                    = 'ifdh'                           # ifdh/xrootd

        ostream                      = job.input_dataset().output_stream();
        odsid                        = self.fFamilyID+s.name()+ostream+'r0000';

        job.fOutputStream            = [ 'InitStntuple'    ]
        job.fOutputDsID              = [ odsid             ]
        job.fOutputFnPattern         = [ 'nts.mu2e.'+odsid ]
        job.fOutputFormat            = [ 'stn'             ]
#------------------------------------------------------------------------------
# stage 4
# s4:digi_trig_000 : InputDsID is 'penuxb0s31r0000' 
#                    digitization job has only one output stream
#------------------------------------------------------------------------------        
        s                            = self.new_stage('s4');

        job                          = s.new_job('digi_trig_000',idsid);

        job.fNInputFiles             = -1                     # number of segments defined by the input dataset
             
        job.fMaxInputFilesPerSegment =  50
        job.fNEventsPerSegment       =  20000
        job.fResample                = 'no'   # yes/no        # for resampling, need to define the run number again
        job.fRequestedTime           = '3h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        output_stream                = job.fInputDataset.output_stream()

        odsid                        = self.fFamilyID+s.name()+output_stream+'r0000';

        job.fOutputStream            = ['defaultOutput'                ]
        job.fOutputDsID              = [odsid                          ]
        job.fOutputFnPattern         = ['sim.mu2e.'+job.fOutputDsID[0] ]
        job.fOutputFormat            = ['art'                          ]
#------------------------------------------------------------------------------
# s4:digi_trig_002 : InputDsID is 'penuxb0s31r0000' 
#                    digitization job has only one output stream
# use different geometry file
#------------------------------------------------------------------------------        
        job                          = s.new_job('digi_trig_002',idsid);

        job.fNInputFiles             = -1                     # number of segments defined by the input dataset
             
        job.fMaxInputFilesPerSegment =  50
        job.fNEventsPerSegment       =  20000
        job.fResample                = 'no'   # yes/no        # for resampling, need to define the run number again
        job.fRequestedTime           = '3h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        output_stream                = job.fInputDataset.output_stream()

        odsid                        = self.fFamilyID+s.name()+output_stream+'r0000';

        job.fOutputStream            = ['defaultOutput'                ]
        job.fOutputDsID              = [odsid                          ]
        job.fOutputFnPattern         = ['sim.mu2e.'+job.fOutputDsID[0] ]
        job.fOutputFormat            = ['art'                          ]
#------------------------------------------------------------------------------
# stage 5
# s5:reco_kk : InputDsID is 'penuxb0s41r0000' 
#              reconstruction job has only one output stream
#              different geometry files in place
#------------------------------------------------------------------------------        
        s                            = self.new_stage('s5');
        job                          = s.new_job('reco_kk_000',idsid);

        job.fNInputFiles             = -1                     # number of segments defined by the input dataset
             
        job.fMaxInputFilesPerSegment =  50
        job.fNEventsPerSegment       =  20000
        job.fResample                = 'no'   # yes/no        # for resampling, need to define the run number again
        job.fRequestedTime           = '3h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        output_stream                = job.input_dataset().output_stream()
        odsid                        = self.fFamilyID+s.name()+output_stream+'r0100';

        job.fOutputStream            = ['defaultOutput'                ]
        job.fOutputDsID              = [odsid                          ]
        job.fOutputFnPattern         = ['sim.mu2e.'+job.fOutputDsID[0] ]
        job.fOutputFormat            = ['art'                          ]
#------------------------------------------------------------------------------
# s5:reco_kk_002 : InputDsID is 'penuxb0s41r0000' 
#              reconstruction job has only one output stream
#              different geometry files in place
#------------------------------------------------------------------------------        
        job                          = s.new_job('reco_kk_002',idsid);

        job.fNInputFiles             = -1                     # number of segments defined by the input dataset
             
        job.fMaxInputFilesPerSegment =  50
        job.fNEventsPerSegment       =  20000
        job.fResample                = 'no'   # yes/no        # for resampling, need to define the run number again
        job.fRequestedTime           = '3h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        output_stream                = job.input_dataset().output_stream()
        odsid                        = self.fFamilyID+s.name()+output_stream+'r0100';

        job.fOutputStream            = ['defaultOutput'                ]
        job.fOutputDsID              = [odsid                          ]
        job.fOutputFnPattern         = ['sim.mu2e.'+job.fOutputDsID[0] ]
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
        job.fOutputFnPattern         = ['sim.mu2e.'+odsid ]
        job.fOutputFormat            = ['stn'             ]
#------------------------------------------------------------------------------
# end
#------------------------------------------------------------------------------
