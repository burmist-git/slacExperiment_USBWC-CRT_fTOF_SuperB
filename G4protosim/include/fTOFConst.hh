#ifndef fTOFConst_h
#define fTOFConst_h

#include "TMath.h"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

namespace fTOFConst{

  const G4double cmMy = 10;
  const G4double mmMy = 1;

  //Sec
  const G4double barSizeX = 29.3*CLHEP::cm;
  const G4double barSizeY = 1.50*cm;
  const G4double barSizeZ = 4.20*cm;

  //update from Jerry 20.11.2010
  //const G4double barDeltaPosY = 5.0*mm;
  const G4double barDeltaPosY = 1.8*mm;

  //PmtWindow
  const G4double pmtWin1SizeX = 1.93*mm;
  const G4double pmtWin1SizeY = 64.0*mm;
  const G4double pmtWin1SizeZ = 64.0*mm;

  const G4double pmtWin2SizeX = 6.88*mm - pmtWin1SizeX;
  const G4double pmtWin2SizeY = 50.0*mm;
  const G4double pmtWin2SizeZ = 50.0*mm;

  //channel
  const G4double pmtChSizeX = 1.5*mm;
  const G4double pmtChSizeY = 18.0*mm;
  const G4double pmtChSizeZ = 6.0*mm;
  const G4double pmtChGap = 0.1*mm;

  //pmt Abs1
  const G4double pmtAbs1SizeX = 1.5*mm;
  const G4double pmtAbs1SizeY = 12.0*mm;
  const G4double pmtAbs1SizeZ = pmtWin2SizeZ;

  //pmt Box size
  const G4double pmtBoxSizeX = 20.4*mm;
  const G4double pmtBoxSizeY = 64.0*mm;
  const G4double pmtBoxSizeZ = 64.0*mm;
  const G4double pmtBoxGapp = 0.1*mm;

  //abs1
  const G4double abs1SizeX = 1.5*mm;
  const G4double abs1SizeY = barSizeY;
  const G4double abs1SizeZ = barSizeZ;

  //abs2
  const G4double abs2SizeX = barSizeX;
  const G4double abs2SizeY = barSizeY;
  const G4double abs2SizeZ = 1.5*mm;


  //const G4int  N_PMT = 9;
  //const G4int    N_PMT = 14;
  //const G4int    N_ch  = 4; //per PMT


  //time spread
  //static const G4double sig_Elect = 10.0; //ps
  //static const G4double sig_T0    = 15.0; //ps
  //static const G4double sig_TTS   = 35.0; //ps

}

#endif
