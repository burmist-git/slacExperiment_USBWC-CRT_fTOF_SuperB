//my
#include "crtTrk.hh"
#include "../anaConst.hh"

//root
#include <TH2.h>
#include <TH1D.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TMath.h>
#include <TLine.h>
#include <TVector3.h>

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

crtTrk::crtTrk(Double_t x1,Double_t y1, Double_t x2,Double_t y2){
  _x1 = x1;
  _y1 = y1;
  _z1 = 0.0;
  _x2 = x2;
  _y2 = y2;
  _z2 = anaConst::hodo_hight;

  TVector3 r1(_x1,_y1,_z1);
  TVector3 r2(_x2,_y2,_z2);
  //TVector3 r2(_x1,_y1,_z1);
  //TVector3 r1(_x2,_y2,_z2);

  TVector3 dr = (r1 - r2);

  if(dr.Z()>0.0){
    dr.SetX(-1.0*dr.X());
    dr.SetY(-1.0*dr.Y());
    dr.SetZ(-1.0*dr.Z());
  }

  Double_t magdr = dr.Mag();
  dr.SetXYZ(dr.x()/magdr,dr.y()/magdr,dr.z()/magdr);
  _a = dr;
  _r1 = r1;

  _Theta = _a.Theta()*180.0/TMath::Pi();
  _Phi = _a.Phi()*180.0/TMath::Pi();

  _Momid = -999;
  //cout<<"_u.Theta()   "<<_u.Theta()*180.0/TMath::Pi()<<endl;
  
}

crtTrk::~crtTrk(){
}

Double_t crtTrk::getTrkTheta(){
  return _Theta;
}

Double_t crtTrk::getTrkPhi(){
  return _Phi;
}
