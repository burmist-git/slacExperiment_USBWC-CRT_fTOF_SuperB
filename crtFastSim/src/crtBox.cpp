//my
#include "crtBox.hh"
#include "crtTrk.hh"
#include "plane.hh"

//root
#include <TH2.h>
#include <TH1D.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TMath.h>
#include <TLine.h>
#include <TString.h>

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

crtBox::crtBox(TString name, Double_t dx, Double_t dy, Double_t dz, 
	       Double_t x0,Double_t y0,Double_t z0, 
	       Double_t angleY){
  _Name = name;  
  TString nameTop = _Name + "_Top";
  TString nameBot = _Name + "_Bot";
  plTop = new plane(nameTop,dx,dy,x0,y0,z0+dz/2.0,angleY);
  plBot = new plane(nameBot,dx,dy,x0,y0,z0-dz/2.0,angleY);

  _xIntTop = -999.0;
  _yIntTop = -999.0;
  _zIntTop = -999.0;
  _xIntBot = -999.0;
  _yIntBot = -999.0;
  _zIntBot = -999.0;
  _interStatus = -999;

}

crtBox::~crtBox(){
}

void crtBox::GetIntersection(crtTrk *trk){
  
  plTop->GetIntersection(trk);
  plBot->GetIntersection(trk);

  if(plTop->GetIntersecStatus()==2){
    _xIntTop = plTop->GetXint();
    _yIntTop = plTop->GetYint();
    _zIntTop = plTop->GetZint();
  }

  if(plBot->GetIntersecStatus()==2){
    _xIntBot = plBot->GetXint();
    _yIntBot = plBot->GetYint();
    _zIntBot = plBot->GetZint();
  }

  _interStatus = -1;

  if(plTop->GetIntersecStatus()==2 && plBot->GetIntersecStatus()!=2){
    _interStatus = 0;
  }
  else if(plTop->GetIntersecStatus()!=2 && plBot->GetIntersecStatus()==2){
    _interStatus = 1;
  }
  else if(plTop->GetIntersecStatus()==2 && plBot->GetIntersecStatus()==2){
    _interStatus = 2;
  }

  //cout<<endl<<"crtBox::GetIntersection   "<< _Name<<endl
  //  <<" _xInt        = "<<_xInt<<endl
  //  <<" _yInt        = "<<_yInt<<endl
  //  <<" _zInt        = "<<_zInt<<endl
  //  <<" _interStatus = "<<_interStatus<<endl;

}

