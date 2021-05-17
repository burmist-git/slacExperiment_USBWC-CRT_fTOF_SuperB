#ifndef fTOFConst_h
#define fTOFConst_h

#include "TMath.h"

namespace fTOFConst{

  // thickness of the absorbers and sensative detector
  const G4double detThikness = 15.0*mm;
  //const G4double detThikness = 12.0*mm;

  //envelope (BRUNO)
  //const G4double detTiltAngle = TMath::ATan((894.0-502.0)/(1974.0-1800.0))*180.0/(TMath::Pi()); //envelope for fTOF january 2010
  //const G4double detTiltAngle = TMath::ATan((894.0-502.0)/(1974.0-1790.0))*180.0/(TMath::Pi()); //additional tilting
  //5cm additional space ~5 deg tilting
  //const G4double detTiltAngle = TMath::ATan((894.0-502.0)/(1974.0-1750.0))*180.0/(TMath::Pi()); //additional tilting
  //const G4double detTiltAngle = TMath::ATan((894.0-502.0)/(1974.0-1700.0))*180.0/(TMath::Pi()); //additional tilting
  const G4double detTiltAngle = 90.0; //no tilting at all

  //with tilting
  //const G4double det_Rmin = (502.0*mm  - (1.0*mm + detThikness/2.0)*TMath::Cos(detTiltAngle*TMath::Pi()/180.0));
  //with NO tilting
  const G4double det_Rmin = (452.0*mm  - (1.0*mm + detThikness/2.0)*TMath::Cos(detTiltAngle*TMath::Pi()/180.0));
  const G4double det_Rmax = (894.0*mm  - (1.0*mm + detThikness/2.0)*TMath::Cos(detTiltAngle*TMath::Pi()/180.0));
  const G4double det_Zmin = (1800.0*mm - (1.0*mm + detThikness/2.0)*TMath::Sin(detTiltAngle*TMath::Pi()/180.0)); 
  //with tilting
  //const G4double det_Zmax = (1974.0*mm - (1.0*mm + detThikness/2.0)*TMath::Sin(detTiltAngle*TMath::Pi()/180.0));
  //with NO tilting
  const G4double det_Zmax = (1801.0*mm - (1.0*mm + detThikness/2.0)*TMath::Sin(detTiltAngle*TMath::Pi()/180.0));
  const G4int    N_det = 12;  //number of sectors
  const G4double detAngleSize= 360.0/N_det;
  
  //const G4double detSizeXmin  = (2.0*det_Rmin*TMath::Tan(detAngleSize*TMath::Pi()/180.0/2.0) - 10.0*mm);
  //const G4double detSizeXmax  = (2.0*det_Rmax*TMath::Sin(detAngleSize*TMath::Pi()/180.0/2.0) - 10.0*mm);
  const G4double detSizeXmin  = (2.0*det_Rmin*TMath::Tan(detAngleSize*TMath::Pi()/180.0/2.0) - 3.0*mm);
  const G4double detSizeXmax  = (2.0*det_Rmax*TMath::Sin(detAngleSize*TMath::Pi()/180.0/2.0) - 10.0*mm);
  const G4double detlength    = ((det_Rmax*TMath::Cos(detAngleSize*TMath::Pi()/180.0/2.0) - det_Rmin)/TMath::Cos((90.0 - detTiltAngle)*TMath::Pi()/180.0) - 5.0*mm);

  //Absorber 1
  const G4double abs1CovSur = 100.0; //in percent
  const G4double abs1Angle  = TMath::ATan((detSizeXmax - detSizeXmin)/detlength/2.0)*rad;
  const G4double abs1SizeX  = detlength/TMath::Cos(abs1Angle)*abs1CovSur/100.0;
  const G4double abs1SizeY  = detThikness;
  const G4double abs1Thikness = 1.2*mm;

  //Absorber 2
  const G4double abs2SizeX = detSizeXmin; 
  const G4double abs2SizeY = detThikness;
  const G4double abs2Thikness = 1.2*mm;

  //Absorber 3
  const G4double abs3SizeX = detSizeXmax;
  const G4double abs3SizeY = detThikness;
  const G4double abs3Thikness = 1.2*mm;

  //MCP-PMT
  //const G4int  N_PMT = 9;
  const G4int    N_PMT = 14;
  const G4int    N_ch  = 4; //per PMT
  const G4double pmtChThikness = 1.2*mm;
  const G4double pmtChSizeX = 5.5*mm;
  const G4double pmtChSizeY = 22.0*mm;
  const G4double pmtThikness = 16.6*mm;
  const G4double pmtSizeX = 27.5*mm;
  const G4double pmtSizeY = 27.5*mm;
  const G4double pmtGap = 0.01*mm;
  const G4double pmtBoxGap = 0.05*mm;

  //Absorber 4
  //with tilting
  //const G4double abs4SizeX = detSizeXmin/N_PMT/2.0 - 1.5*pmtGap - 2.0*pmtChSizeX - pmtGap;
  //with NO tilting
  const G4double abs4SizeX = detSizeXmax/N_PMT/2.0 - 1.5*pmtGap - 2.0*pmtChSizeX - pmtGap;
  const G4double abs4SizeY = detThikness;
  const G4double abs4Thikness = 0.4*mm;

  //time spread
  static const G4double sig_Elect = 10.0;//ps
  static const G4double sig_T0 = 15.0;//ps
  static const G4double sig_TTS = 35.0;//ps

}

#endif
