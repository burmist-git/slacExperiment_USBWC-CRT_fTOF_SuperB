#ifndef crtFileStr_hh
#define crtFileStr_hh

//root
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>

using namespace std;

// NA: constant shift needed to get the time axis properly
// labelled with the format MM/DD HH:MM
//const Double_t timeShiftForProperDisplay = 36000.0;
const Double_t timeShiftForProperDisplay = 32400.0;

class crtBox;
class crtTrk;
class TRandom3;

class crtFileStr {
public :

  crtFileStr();
  ~crtFileStr();
  Int_t  GetEntry(Long64_t entry);
  Long64_t LoadTree(Long64_t entry);
  void  Init(TTree *tree);
  void  Loop(TString outputRootHistFile);
  void  crtHodoscopeDataTest(TString outputRootHistFile);
  void  Show(Long64_t entry = -1);

  TRandom3 *_rnd;
  
  ////////////////New Functions//////////////////////
  void addFilesToChain(string name);
  void PrintEventInfo(Int_t evI);
  void GetUnixTime(Double_t *unixTimeCRT, Int_t Nmax, Int_t &nEv);
  void GetDataToMerge();
  void GetDataFromDetectors();
  void GetHodoscopePositions();
  void GetHodoscopePositionsMyAlgo();
  void GetTrkDirectionCos();
  void saveUnixTime2File(TString fileN);
  Bool_t CheckDistInTime();
  Int_t GetDataRate();//ev/s
  Int_t getTrkMomID();
  Double_t GetRelativeRunTime();

  void initHistos_H1D(TH1D *h1D1[4],string h1name, string h1Title,
		      Int_t Nbin, Float_t Vmin, Float_t Vmax);
  void initHistos_H2D(TH2D *h2D1[4],string h2name, string h2Title,
		      Int_t Nbin1, Float_t Vmin1, Float_t Vmax1,
		      Int_t Nbin2, Float_t Vmin2, Float_t Vmax2);
  void initHistosHodoX_H1D(TH1D *h1D1[55],string h1name, string h1Title,
			   Int_t Nbin, Float_t Vmin, Float_t Vmax);
  void initHistosHodoY_H1D(TH1D *h1D1[27],string h1name, string h1Title,
			   Int_t Nbin, Float_t Vmin, Float_t Vmax);
  /////////////////////////////////////////////////////
  
  /////////////////////////
  crtBox *_boxfTOF;
  crtBox *_boxfTOF_Bot;
  crtBox *_boxQuartzS;
  crtTrk *_trk;
  /////////////////////////

  Double_t _timeStopUnix;
  Double_t _timeStartUnix;
  Double_t _Rate;
  Int_t _timeStart_hour;
  Int_t _timeStart_min;
  Double_t _relativeRunTime;

  Bool_t _hodoStatus;

  Double_t _x1;
  Double_t _x2;
  Double_t _y1;
  Double_t _y2;

  Double_t _trkTheta;
  Double_t _trkPhi;
  Int_t _trkMomID;

  Double_t _cx;
  Double_t _cy;
  Double_t _cz;
  ////////////////Data from detectors/////////

  //trigger T1, T2
  Int_t trig1A[4];//ADC
  Int_t trig2A[4];//ADC
  Int_t trig1T[4];//TDC
  Int_t trig2T[4];//TDC
  //stack counter information
  Int_t stack1A[4];//ADC
  Int_t stack2A[4];//ADC
  Int_t stack3A[4];//ADC
  Int_t stack4A[4];//ADC
  Int_t stack1T[4];//TDC
  Int_t stack2T[4];//TDC
  Int_t stack3T[4];//TDC
  Int_t stack4T[4];//TDC
  //hodoscope
  Int_t hodo1X[55];
  Int_t hodo2X[55];
  Int_t hodo1Y[27];
  Int_t hodo2Y[27];
  //quartz start counter 
  Int_t start1T[4];
  Int_t start1A[4];//ADC information pulled out form tdc11 which is wery strange 
  ////////////////////////////////////////////

  TTree          *fChain;
  Int_t           fCurrent;
  
  // Declaration of leaf types
  Double_t        time_CRT;
  Int_t           tdc1_CRT[8];
  Int_t           tdc2_CRT[8];
  Int_t           tdc3_CRT[8];
  Int_t           adc1_CRT[12];
  Int_t           adc2_CRT[12];
  Int_t           scl1_CRT[4];
  Int_t           tdc4_CRT[32];
  Int_t           tdc5_CRT[32];
  Int_t           tdc6_CRT[32];
  Int_t           tdc7_CRT[32];
  Int_t           tdc8_CRT[32];
  Int_t           tdc9_CRT[32];
  Int_t           scl2_CRT[12];
  Int_t           scl3_CRT[12];
  Int_t           tdc10_CRT[16];
  Int_t           tdc11_CRT[16];
  Int_t           adc3_CRT[12];
  Int_t           tdc12_CRT[8];
  Float_t         temp_fDIRC[7][4];
  Float_t         rovdd_fDIRC[7][4];
  Int_t           row_fDIRC[7][4][16];
  Int_t           col_fDIRC[7][4][16];
  Int_t           rowref_fDIRC[7][4];
  Int_t           colref_fDIRC[7][4];
  Int_t           scaler_fDIRC[7][4][16];
  Int_t           scalerref_fDIRC[7][4];
  Int_t           placeholder1_fDIRC[7][4][8];
  Int_t           placeholder2_fDIRC[7][4][8];
  Float_t         adc_fdirc_fDIRC[7][4][16];
  Float_t         tdc_fdirc_fDIRC[7][4][16];
  
  // List of branches
  TBranch        *b_time;   //!
  TBranch        *b_tdc1;   //!
  TBranch        *b_tdc2;   //!
  TBranch        *b_tdc3;   //!
  TBranch        *b_adc1;   //!
  TBranch        *b_adc2;   //!
  TBranch        *b_scl1;   //!
  TBranch        *b_tdc4;   //!
  TBranch        *b_tdc5;   //!
  TBranch        *b_tdc6;   //!
  TBranch        *b_tdc7;   //!
  TBranch        *b_tdc8;   //!
  TBranch        *b_tdc9;   //!
  TBranch        *b_scl2;   //!
  TBranch        *b_scl3;   //!
  TBranch        *b_tdc10;   //!
  TBranch        *b_tdc11;   //!
  TBranch        *b_adc3;   //!
  TBranch        *b_tdc12;   //!
  TBranch        *b_temp;   //!
  TBranch        *b_rovdd;   //!
  TBranch        *b_row;   //!
  TBranch        *b_col;   //!
  TBranch        *b_rowref;   //!
  TBranch        *b_colref;   //!
  TBranch        *b_scaler;   //!
  TBranch        *b_scalerref;   //!
  TBranch        *b_placeholder1;   //!
  TBranch        *b_placeholder2;   //!
  TBranch        *b_adc_fdirc;   //!
  TBranch        *b_tdc_fdirc;   //!
  

};

#endif
