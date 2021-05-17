#ifndef crtBox_hh
#define crtBox_hh

//root
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TVector3.h>

using namespace std;

class plane;
class crtTrk;
class TString;

class crtBox {
public :

  crtBox(TString name,Double_t dx,Double_t dy, Double_t dz,
	 Double_t x0,Double_t y0,Double_t z0, 
	 Double_t angleY);
  ~crtBox();
  
  void GetIntersection(crtTrk *trk);
  inline Int_t GetIntersecStatus(){return _interStatus;};  

  inline Double_t GetXintTop(){return _xIntTop;};//cm
  inline Double_t GetYintTop(){return _yIntTop;};//cm
  inline Double_t GetZintTop(){return _zIntTop;};//cm
  
  inline Double_t GetXintBot(){return _xIntBot;};//cm
  inline Double_t GetYintBot(){return _yIntBot;};//cm
  inline Double_t GetZintBot(){return _zIntBot;};//cm

  inline plane* GetplTop() const {return plTop;};//cm
  inline plane* GetplBot() const {return plBot;};//cm

private:
  
  plane *plTop;
  plane *plBot;
  
  TString _Name;
  
  //parameters of trk intersection with plain 
  Double_t _xIntTop;//cm
  Double_t _yIntTop;//cm
  Double_t _zIntTop;//cm
  
  Double_t _xIntBot;//cm
  Double_t _yIntBot;//cm
  Double_t _zIntBot;//cm
  
  Int_t _interStatus;
  //-999 staus not defined
  //-1 no intesection
  // 0 one intersection with top plane within sizes of the box
  // 1 one intersection with bot plane within sizes of the box
  // 2 two intersection with top and bot plane within sizes of the box

};

#endif
