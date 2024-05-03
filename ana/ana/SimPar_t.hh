#ifndef __pipenu_ana_SimPar_t__
#define __pipenu_ana_SimPar_t__

class TGenParticle;
class TSimParticle;
class TStepPointMC;

namespace pipenu {

struct SimPar_t {
  TSimParticle*  fParticle;		// pointer to the signal particle
  TStepPointMC*  fTFront;		// VD hit at the tracker front
  TStepPointMC*  fTMid;		// VD hit at the tracker center
  TStepPointMC*  fTBack;		// VD hit at the tracker back (+1610 mm)
  int            fStage;              // last stage the particle reached
  double         fWeight;             // weight , either 1 or d2sigma/dp/dcosth
};

}
#endif
