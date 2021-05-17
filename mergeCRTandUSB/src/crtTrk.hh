#ifndef crtTrk_hh
#define crtTrk_hh

//root
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TVector3.h>

using namespace std;

class crtTrk {
public :

  crtTrk(Double_t x1,Double_t y1, Double_t x2,Double_t y2);
  ~crtTrk();
  Double_t getTrkTheta();
  Double_t getTrkPhi();
  Int_t getMomid(){
    return _Momid;
  }
  void SetMomID(Int_t momID){
    _Momid = momID;
  }
  TVector3 _a;
  TVector3 _r1;

private:

  Double_t _x1;//cm
  Double_t _y1;//cm
  Double_t _z1;//cm
  Double_t _x2;//cm
  Double_t _y2;//cm
  Double_t _z2;//cm

  Double_t _Theta;//deg
  Double_t _Phi;//deg
  //momentum ID
  //0 - no coinsidence with stack count
  //1 - only stack 4
  //2 - stack 4 and stack 3 only 
  //3 - stack 4 and stack 3 stack 2 only
  //4 - stack 4 and stack 3 stack 2 stack 1
  Int_t _Momid;

};

#endif
