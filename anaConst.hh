#ifndef anaConst_h
#define anaConst_h

//root
#include <TROOT.h>
#include "TMath.h"

namespace anaConst{
  //general constans for DIRC-like TOF prototype test @ SLAC CRT
  static const int nChannels = 16;

  //USBWC
  static const Int_t usbwcNsamplingPoint = 256;
  static const Double_t usbwcSamplingPeriod = 0.3125;//ns

  //G4simulation
  static const int numberOf_Ev = 1000;
  static const int nMaxPe = 1000;
  static const double sigTTS = 0.035;
  static const double sigEle = 0.01;
  //static const double sigEle = 0.0;
  //LB 10.02.2011
  //good agreement with measurements
  //static const double collectionEff = 0.42;
  //static const double collectionEff = 0.7;
  //static const double collectionEff = 0.14;
  //static const double collectionEff = 0.07;
  //static const double collectionEff = 0.05;
  //much the data from RUN4
  static const double collectionEff = 0.14;

  //CRT
  static const Int_t nHodoX = 55;
  static const Int_t nHodoY = 27;
  static const Double_t hodoXBar_lenght = 51.054;   //cm
  static const Double_t hodoYBar_lenght = 106.9340; //cm
  static const Double_t hodoBar_width = 3.0;        //cm
  static const Double_t hodoBar_hight = 0.9525;     //cm
  static const Double_t hodo_hight = 263.36;        //cm

  //cuts on muon trak in deg
  static const Double_t phiMin = 0.0;
  static const Double_t phiMax = 0.0;
  static const Double_t thetaMax = 180.0;
  static const Double_t thetaMin = 0.0;


  //characteristic of the stepped face MCP-pmt 
  //(10 micron hole, 8x8 anodes ) (16 output channels)
  //Signal threashold for each channel
  //static const Double_t sigThreshold[nChannels] = {  0.1,   0.1,  0.06,  0.07,
  //					     0.09,  0.11, 0.07,  0.075,
  //					     0.06,  0.06, 0.06,  0.06,
  //					     0.06,  0.06, 0.06,  0.06};
  static const Double_t sigThreshold[nChannels] = {  0.08,  0.08, 0.08,  0.08,
						     0.08,  0.08, 0.08,  0.08,
						     0.08,  0.08, 0.08,  0.08,
						     0.08,  0.08, 0.08,  0.08};
  
  static const Double_t chargeMean[nChannels] = {0.597, 0.946, 0.614, 0.804,
						 0.64, 0.722, 0.563, 0.379,
						 0.612, 0.615, 0.706, 0.69, 
						 0.66, 0.605, 0.502, 0.6};
  
}
#endif
