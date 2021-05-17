//my
#include "muonGen.hh"
#include "crtTrk.hh"
#include "crtConst.hh"

//root
#include <TRandom3.h>

#include <iostream>

using namespace std;

muonGen::muonGen(Int_t mySeed){ 
  _rnd = new TRandom3(mySeed);
  _x = -999.0;
  _y = -999.0;
  _z = -999.0;
  _Theta = -999.0;
  _Phi = -999.0;
}

muonGen::~muonGen(){
}

crtTrk* muonGen::GenerateMuon(Double_t zz){

  //_x = _rnd->Uniform(-1.0*crtConst::hodoYBar_lenght/2.0 - crtConst::topHodo_xShift,crtConst::hodoYBar_lenght/2.0 + crtConst::topHodo_xShift);
  //_y = _rnd->Uniform(-1.0*crtConst::hodoXBar_lenght/2.0 - crtConst::topHodo_yShift,crtConst::hodoXBar_lenght/2.0 + crtConst::topHodo_yShift);
  _x = _rnd->Uniform(-2.0*crtConst::hodoYBar_lenght, 2.0*crtConst::hodoYBar_lenght);
  _y = _rnd->Uniform(-2.0*crtConst::hodoXBar_lenght, 2.0*crtConst::hodoXBar_lenght);

  _z = zz;
  _Theta = 180.0 - genCos2dist();     //deg
  //_Theta = 177.0;     //deg
  _Phi = _rnd->Uniform(-180.0,180.0); //deg
  _trk = new crtTrk(_x,_y,_z,_Theta,_Phi);  
  return _trk;
}

Double_t muonGen::genCos2dist(){
  Double_t theta = -999.0;//deg 
  Double_t x = -999.0;
  Double_t y = -999.0;
  while(theta==-999.0){
    x = _rnd->Uniform(0.0,70.0*TMath::Pi()/180.0); //rad
    y = _rnd->Uniform(0.0,1.1); //rad
    if(TMath::Power(TMath::Cos(x),1.85)>y){
      theta = x*180.0/TMath::Pi();
    }
  }  
  return theta;
}

