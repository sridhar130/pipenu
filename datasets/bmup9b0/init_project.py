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
#------------------------------------------------------------------------------
# Input for s2:stn_dif, s3:digi_trig
#------------------------------------------------------------------------------
        self.add_dataset(Dataset('dts.mu2e.bmup9b0s21r0000.pipenu.art','bmup9b0s21r0000','local'))
        self.add_dataset(Dataset('dts.mu2e.bmup9b0s24r0000.pipenu.art','bmup9b0s24r0000','local'))
        self.add_dataset(Dataset('dts.mu2e.bmup9b0s25r0000.pipenu.art','bmup9b0s25r0000','local'))
        self.add_dataset(Dataset('dts.mu2e.bmup9b0s26r0000.pipenu.art','bmup9b0s26r0000','local'))
        self.add_dataset(Dataset('dts.mu2e.bmup9b0s27r0000.pipenu.art','bmup9b0s27r0000','local'))
        self.add_dataset(Dataset('dts.mu2e.bmup9b0s28r0000.pipenu.art','bmup9b0s28r0000','local'))
        self.add_dataset(Dataset('dts.mu2e.bmup9b0s29r0000.pipenu.art','bmup9b0s29r0000','local'))
#------------------------------------------------------------------------------
# Input for s3:stn, s4:reco_kk
#------------------------------------------------------------------------------
        self.add_dataset(Dataset('dig.mu2e.bmup9b0s34r0000.pipenu.art','bmup9b0s34r0000','local'))
        self.add_dataset(Dataset('dig.mu2e.bmup9b0s36r0000.pipenu.art','bmup9b0s36r0000','local'))
#------------------------------------------------------------------------------
# output of the reconstruction
#------------------------------------------------------------------------------
        self.add_dataset(Dataset('mcs.mu2e.bmup9b0s44r0100.pipenu.art','bmup9b0s44r0100','local'))
        self.add_dataset(Dataset('mcs.mu2e.bmup9b0s46r0100.pipenu.art','bmup9b0s46r0100','local'))
#------------------------------------------------------------------------------
# a job always has an input dataset, but...
#------------------------------------------------------------------------------
        if (self.fIDsID) : self.fInputDataset = self.dataset(self.fIDsID);
        return

#------------------------------------------------------------------------------
# constructor
# force the job name and the fcl file name to be the same
#------------------------------------------------------------------------------
    def __init__(self,idsid=None):

        ProjectBase.__init__(self,project='pipenu',family_id='bmup9b0',idsid=idsid);

        # print("Project init: idsid:",idsid);
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

        job.fOutputStream            = ['BeamOutput'                   ] #, 'NeutralsOutput'              ]
        job.fOutputDsID              = [odsid1                         ] #,  odsid2                       ] 
        job.fOutputFnPattern         = ['sim.mu2e.'+job.fOutputDsID[0] ] #, 'sim.mu2e.'+job.fOutputDsID[1]]
        job.fOutputFormat            = ['art'                          ] #, 'art'                         ]
        
        # grid output dir
        desc                         = self.name()+'.'+job.input_dataset().id()+'.'+s.name()+'_'+job.name()
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

        desc                         = self.name()+'.'+job.input_dataset().id()+'.'+s.name()+'_'+job.name()
        job.fDescription             = desc;       
#------------------------------------------------------------------------------
# stage, s2:resample
#------------------------------------------------------------------------------        
        s                            = self.new_stage('s2');
        job                          = s.new_job('resample','bmup9b0s11r0000');

        job.fNInputFiles             = 10                     # number of segments defined by s1:sim
             
        job.fMaxInputFilesPerSegment =  1
        job.fNEventsPerSegment       =  100000
        job.fResample                = 'yes'                  # yes/no, for resampling, need to define the run number again
        job.fResamplingModuleLabel   = 'beamResampler'
        job.fRunNumber               = 1210
        job.fRequestedTime           = '5h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        odsid                        = self.fFamilyID+s.name()+'4'+'r0000';

        job.fOutputStream            = [ 's24'             ]
        job.fOutputDsID              = [  odsid            ]
        job.fOutputFnPattern         = [ 'sim.mu2e.'+odsid ]
        job.fOutputFormat            = [ 'art'             ]

        # job description defined the grid output directory
        desc                         = self.name()+'.'+job.input_dataset().id()+'.'+s.name()+'_'+job.name()
        job.fDescription             = desc;
#------------------------------------------------------------------------------
# s2:resample_050
#------------------------------------------------------------------------------        
        job                          = s.new_job('resample_050','bmup9b0s11r0000');

        job.fBaseFcl                 = self.base_fcl(job,'resample_050');

        job.fNInputFiles             = 10                     # number of segments defined by s1:sim
             
        job.fMaxInputFilesPerSegment =  1
        job.fNEventsPerSegment       =  100000
        job.fResample                = 'yes'                  # yes/no, for resampling, need to define the run number again
        job.fResamplingModuleLabel   = 'beamResampler'
        job.fRunNumber               = 1210
        job.fRequestedTime           = '5h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        odsid                        = self.fFamilyID+s.name()+'5'+'r0000';

        job.fOutputStream            = [ 's24'             ]
        job.fOutputDsID              = [  odsid            ]
        job.fOutputFnPattern         = [ 'sim.mu2e.'+odsid ]
        job.fOutputFormat            = [ 'art'             ]

        # job description defined the grid output directory
        desc                         = self.name()+'.'+job.input_dataset().id()+'.'+s.name()+'_'+job.name()
        job.fDescription             = desc;
#------------------------------------------------------------------------------
# s2:resample_100
#------------------------------------------------------------------------------        
        job                          = s.new_job('resample_100','bmup9b0s11r0000');

        # job.fBaseFcl                 = self.base_fcl(job,'resample_100');

        job.fNInputFiles             = 10                     # number of segments defined by s1:sim
             
        job.fMaxInputFilesPerSegment =  1
        job.fNEventsPerSegment       =  100000
        job.fResample                = 'yes'                  # yes/no, for resampling, need to define the run number again
        job.fResamplingModuleLabel   = 'beamResampler'
        job.fRunNumber               = 1210
        job.fRequestedTime           = '5h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        odsid                        = self.fFamilyID+s.name()+'6'+'r0000';

        job.fOutputStream            = [ 's24'             ]
        job.fOutputDsID              = [  odsid            ]
        job.fOutputFnPattern         = [ 'sim.mu2e.'+odsid ]
        job.fOutputFormat            = [ 'art'             ]

        # job description defined the grid output directory
        desc                         = self.name()+'.'+job.input_dataset().id()+'.'+s.name()+'_'+job.name()
        job.fDescription             = desc;
#------------------------------------------------------------------------------
# s2:resample_150
#------------------------------------------------------------------------------        
        job                          = s.new_job('resample_150','bmup9b0s11r0000');

        job.fBaseFcl                 = self.base_fcl(job,'resample_150');

        job.fNInputFiles             = 10                     # number of segments defined by s1:sim
             
        job.fMaxInputFilesPerSegment =  1
        job.fNEventsPerSegment       =  100000
        job.fResample                = 'yes'                  # yes/no, for resampling, need to define the run number again
        job.fResamplingModuleLabel   = 'beamResampler'
        job.fRunNumber               = 1210
        job.fRequestedTime           = '5h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        odsid                        = self.fFamilyID+s.name()+'7'+'r0000';

        job.fOutputStream            = [ 's24'             ]
        job.fOutputDsID              = [  odsid            ]
        job.fOutputFnPattern         = [ 'sim.mu2e.'+odsid ]
        job.fOutputFormat            = [ 'art'             ]

        # job description defined the grid output directory
        desc                         = self.name()+'.'+job.input_dataset().id()+'.'+s.name()+'_'+job.name()
        job.fDescription             = desc;
#------------------------------------------------------------------------------
# s2:resample_200
#------------------------------------------------------------------------------        
        job                          = s.new_job('resample_200','bmup9b0s11r0000');

        job.fBaseFcl                 = self.base_fcl(job,'resample_200');

        job.fNInputFiles             = 10                     # number of segments defined by s1:sim
             
        job.fMaxInputFilesPerSegment =  1
        job.fNEventsPerSegment       =  100000
        job.fResample                = 'yes'                  # yes/no, for resampling, need to define the run number again
        job.fResamplingModuleLabel   = 'beamResampler'
        job.fRunNumber               = 1210
        job.fRequestedTime           = '5h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        odsid                        = self.fFamilyID+s.name()+'8'+'r0000';

        job.fOutputStream            = [ 's24'             ]
        job.fOutputDsID              = [  odsid            ]
        job.fOutputFnPattern         = [ 'sim.mu2e.'+odsid ]
        job.fOutputFormat            = [ 'art'             ]

        # job description defined the grid output directory
        desc                         = self.name()+'.'+job.input_dataset().id()+'.'+s.name()+'_'+job.name()
        job.fDescription             = desc;
#------------------------------------------------------------------------------
# s2:resample_300
#------------------------------------------------------------------------------        
        job                          = s.new_job('resample_300','bmup9b0s11r0000');

        job.fBaseFcl                 = self.base_fcl(job,'resample_300');

        job.fNInputFiles             = 10                     # number of segments defined by s1:sim
             
        job.fMaxInputFilesPerSegment =  1
        job.fNEventsPerSegment       =  100000
        job.fResample                = 'yes'                  # yes/no, for resampling, need to define the run number again
        job.fResamplingModuleLabel   = 'beamResampler'
        job.fRunNumber               = 1210
        job.fRequestedTime           = '5h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        odsid                        = self.fFamilyID+s.name()+'9'+'r0000';

        job.fOutputStream            = [ 's24'             ]
        job.fOutputDsID              = [  odsid            ]
        job.fOutputFnPattern         = [ 'sim.mu2e.'+odsid ]
        job.fOutputFormat            = [ 'art'             ]

        # job description defined the grid output directory
        desc                         = self.name()+'.'+job.input_dataset().id()+'.'+s.name()+'_'+job.name()
        job.fDescription             = desc;
#------------------------------------------------------------------------------
# s2:stn_dif : ntuple decays in flight
#------------------------------------------------------------------------------  
        # print(" s2:stn_dif: idsid=",idsid)
        job                          = s.new_job('stn_dif',idsid);

        job.fRunNumber               = 1210;
        job.fBaseFcl                 = self.base_fcl(job,'stn_dif');

        job.fNInputFiles             = 1                                # number of segments    

        job.fMaxInputFilesPerSegment = 100
        job.fNEventsPerSegment       = 50000                       
        job.fResample                = 'no'                               # yes/no
        job.fRequestedTime           = '3h'
        job.fIfdh                    = 'ifdh'                           # ifdh/xrootd

        odsid                        = self.fFamilyID+s.name()+job.input_dataset().output_stream()+'r0000';

        job.fOutputStream            = [ 'InitStntuple'     ]
        job.fOutputDsID              = [ odsid              ]
        job.fOutputFnPattern         = [ 'nts.mu2e.'+odsid  ]
        job.fOutputFormat            = [ 'stn'              ]

        desc                         = self.name()+'.'+job.input_dataset().id()+'.'+s.name()+'_'+job.name()
        job.fDescription             = desc;
#------------------------------------------------------------------------------
# stage 3
# s3:digi_trig : InputDsID is 'bmup9b0s2xr0000', digitization job has only one output stream
#------------------------------------------------------------------------------        
        s                            = self.new_stage('s3');

        job                          = s.new_job('digi_trig',idsid);

        # job.fBaseFcl                 = self.base_fcl(job,'digi_trig');

        job.fNInputFiles             = -1                     # number of segments defined by the input dataset
             
        job.fMaxInputFilesPerSegment =  50
        job.fNEventsPerSegment       =  100000
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
# job description defined the grid output directory
#------------------------------------------------------------------------------
        desc                         = self.name()+'.'+job.input_dataset().id()+'.'+s.name()+'_'+job.name()
        job.fDescription             = desc;
#------------------------------------------------------------------------------
# s3:stn : ntuple decays in flight after digitization
#------------------------------------------------------------------------------  
        job                          = s.new_job('stn',idsid);

        # job.fRunNumber               = 1210;
        # job.fBaseFcl                 = self.base_fcl(job,'stn_dif');

        job.fNInputFiles             = 1                                # number of segments    

        job.fMaxInputFilesPerSegment = 100
        job.fNEventsPerSegment       = 50000                       
        job.fResample                = 'no'                               # yes/no
        job.fRequestedTime           = '3h'
        job.fIfdh                    = 'ifdh'                           # ifdh/xrootd

        odsid                        = self.fFamilyID+s.name()+job.input_dataset().output_stream()+'r0000';

        job.fOutputStream            = [ 'InitStntuple'     ]
        job.fOutputDsID              = [ odsid              ]
        job.fOutputFnPattern         = [ 'nts.mu2e.'+odsid  ]
        job.fOutputFormat            = [ 'stn'              ]

        desc                         = self.name()+'.'+job.input_dataset().id()+'.'+s.name()+'_'+job.name()
        job.fDescription             = desc;
#------------------------------------------------------------------------------
# s4:reco_kk : reconstruction job has only one output stream
#------------------------------------------------------------------------------        
        s                            = self.new_stage('s4');

        job                          = s.new_job('reco_kk',idsid);

        job.fNInputFiles             = -1                     # number of segments defined by the input dataset
             
        job.fMaxInputFilesPerSegment =  50
        job.fNEventsPerSegment       =  100000
        job.fResample                = 'no'   # yes/no        # for resampling, need to define the run number again
        job.fRequestedTime           = '3h'   
        job.fIfdh                    = 'xrootd'               # ifdh/xrootd
        job.fMaxMemory               = '3000MB'

        output_stream                = self.fInputDataset.output_stream()
        odsid                        = self.fFamilyID+s.name()+output_stream+'r0100';

        job.fOutputStream            = ['defaultOutput'   ]
        job.fOutputDsID              = [odsid             ]
        job.fOutputFnPattern         = ['sim.mu2e.'+odsid ]
        job.fOutputFormat            = ['art'             ]

        # job description defined the grid output directory
        desc                         = self.name()+'.'+job.input_dataset().id()+'.'+s.name()+'_'+job.name()
        job.fDescription             = desc;
#------------------------------------------------------------------------------
# s4:stn_kk : stntupling job has only one output stream
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

        odsid                        = self.fFamilyID+'s5'+output_stream+'r0100';

        job.fOutputStream            = ['InitStntuple'    ]
        job.fOutputDsID              = [odsid             ]
        job.fOutputFnPattern         = ['sim.mu2e.'+odsid ]
        job.fOutputFormat            = ['stn'             ]

        # job description defined the grid output directory
        desc                         = self.name()+'.'+job.input_dataset().id()+'.'+s.name()+'_'+job.name()
        job.fDescription             = desc;
#------------------------------------------------------------------------------
# end
#------------------------------------------------------------------------------
