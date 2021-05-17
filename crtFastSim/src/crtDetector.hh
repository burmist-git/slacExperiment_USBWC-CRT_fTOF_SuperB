#ifndef crtDetector_hh
#define crtDetector_hh

//root
#include <TROOT.h>

class crtBox;
class crtTrk;

class crtDetector {

public :

  crtDetector();
  ~crtDetector();
  void recoTrk();
  inline void SetGenTrk(crtTrk *genTrk){_genTrk = genTrk;}
  inline crtTrk* GetRecoTrk(){return _recoTrk;}
  Bool_t Trigger();

  crtBox *_Trigg1;
  crtBox *_Trigg2;
  crtBox *_QuartzStart; 
  crtBox *_fTOFprot;

private :
  crtTrk *_genTrk;
  crtTrk *_recoTrk;



};

#endif
