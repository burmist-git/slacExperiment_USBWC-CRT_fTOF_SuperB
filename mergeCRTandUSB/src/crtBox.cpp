//my
#include "crtBox.hh"
#include "crtTrk.hh"

//root
#include <TH2.h>
#include <TH1D.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TMath.h>
#include <TLine.h>

//C, C++
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <math.h>

crtBox::crtBox(Double_t dx,Double_t dy,Double_t dz){
  _n.SetXYZ(0.0,0.0,1.0);
  _V0.SetXYZ(0.0,0.0,0.0);
  _angleX = 0;
  _angleY = 0;
  _angleZ = 0;
  _dx = dx;
  _dy = dy;
  _dz = dz;

 _xInt = -9999.0;
 _yInt = -9999.0;
 _zInt = -9999.0;
 _interStatus = -9999;
  //cout<<"crtBox::crtBox()"<<endl;
}

crtBox::~crtBox(){
}

void crtBox::RotateX(Double_t angle){
  if(angle != 0.0){
    _angleX = angle;
    _n.RotateX(angle/180.0*TMath::Pi());
  }
}

void crtBox::RotateY(Double_t angle){
  /*
  cout<<endl<<"crtBox::RotateY"<<endl
      <<"_n.X() "<<_n.X()<<endl
      <<"_n.Y() "<<_n.Y()<<endl
      <<"_n.Z() "<<_n.Z()<<endl
      <<"_n.Theta() "<<_n.Theta()*180.0/TMath::Pi()<<endl
      <<"_n.Phi()   "<<_n.Phi()*180.0/TMath::Pi()<<endl;
  */

  if(angle != 0.0){
    _angleY = angle;
    _n.RotateY(angle/180.0*TMath::Pi());
  }

  /*
  cout<<"*****"<<endl
      <<"angle "<<angle<<endl
      <<"_n.X() "<<_n.X()<<endl
      <<"_n.Y() "<<_n.Y()<<endl
      <<"_n.Z() "<<_n.Z()<<endl
      <<"_n.Theta() "<<_n.Theta()*180.0/TMath::Pi()<<endl
      <<"_n.Phi()   "<<_n.Phi()*180.0/TMath::Pi()<<endl
      <<"-----------------------------"<<endl;
  */

}

void crtBox::RotateZ(Double_t angle){
  /*
  cout<<endl<<"crtBox::RotateZ"<<endl
      <<"_n.X() "<<_n.X()<<endl
      <<"_n.Y() "<<_n.Y()<<endl
      <<"_n.Z() "<<_n.Z()<<endl
      <<"_n.Theta() "<<_n.Theta()*180.0/TMath::Pi()<<endl
      <<"_n.Phi()   "<<_n.Phi()*180.0/TMath::Pi()<<endl;
  */
  if(angle != 0.0){
    _angleZ = angle;
    _n.RotateZ(angle/180.0*TMath::Pi());
  }
  /*
  cout<<"*****"<<endl
      <<"angle "<<angle<<endl
      <<"_n.X() "<<_n.X()<<endl
      <<"_n.Y() "<<_n.Y()<<endl
      <<"_n.Z() "<<_n.Z()<<endl
      <<"_n.Theta() "<<_n.Theta()*180.0/TMath::Pi()<<endl
      <<"_n.Phi()   "<<_n.Phi()*180.0/TMath::Pi()<<endl
      <<"-----------------------------"<<endl;
  */
}

void crtBox::SetCenter(TVector3 V0){
  _V0 = V0;
};

void crtBox::GetIntersection(crtTrk *trk){
  Double_t t; //parameter of the trk
  _xInt = -9999.0;
  _yInt = -9999.0;
  _zInt = -9999.0;    
  
  //TVector3 u  = (r1 - r2)/(|r1 - r2|
  
  Double_t znam = (trk->_a.Dot(_n));
  Double_t chus = (_V0 - trk->_r1).Dot(_n);

  if( znam == 0.0 && chus == 0.0 ){
    _xInt = -9999.0;
    _yInt = -9999.0;
    _zInt = -9999.0;    
    _interStatus = 0;
  }
  else if( znam == 0.0 && chus != 0.0 ){
    _xInt = -9999.0;
    _yInt = -9999.0;
    _zInt = -9999.0;
    _interStatus = -1;
  }
  else if(znam != 0.0){
    t = chus/znam;
    _interStatus = 1;
    TVector3 r = (trk->_r1 + trk->_a*t);
    _xInt = r.X();
    _yInt = r.Y();
    _zInt = r.Z();
  }

  /*
  cout<<endl<<"crtBox::GetIntersection"<<endl
      <<" _xInt = "<<_xInt<<endl
      <<" _yInt = "<<_yInt<<endl
      <<" _zInt = "<<_zInt<<endl;
  */

}

