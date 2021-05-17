#ifndef crtBox_hh
#define crtBox_hh

//root
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TVector3.h>

using namespace std;

class crtTrk;

class crtBox {
public :

  crtBox(Double_t dx,Double_t dy,Double_t dz);
  ~crtBox();
  void SetCenter(TVector3 V0);
  void RotateX(Double_t angle);
  void RotateY(Double_t angle);
  void RotateZ(Double_t angle);

  void GetIntersection(crtTrk *trk);

  //parameters of trk intersection 
  Double_t _xInt;//cm
  Double_t _yInt;//cm
  Double_t _zInt;//cm

  Int_t _interStatus;
  //-9999 staus no defin
  //-1 no intesection
  // 0 infinit number of intersections
  // 1 one intersection 


private:

  Double_t _dx;//cm
  Double_t _dy;//cm
  Double_t _dz;//cm

  Double_t _angleX;//deg
  Double_t _angleY;//deg
  Double_t _angleZ;//deg

  TVector3 _n;
  TVector3 _V0;


};

#endif
