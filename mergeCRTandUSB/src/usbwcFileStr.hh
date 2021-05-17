#ifndef usbwcFileStr_h
#define usbwcFileStr_h

//my
#include "../anaConst.hh"

//root
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include <TString.h>

using namespace std;

class TH2D;

class usbwcFileStr {
public :

  usbwcFileStr();
  ~usbwcFileStr();
  void addFilesToChain(string name);
  Int_t    GetEntry(Long64_t entry);
  Long64_t LoadTree(Long64_t entry);
  void     Init(TTree *tree);
  void     Loop(TString histFName);
  void     LaserAna(TString histFName); 
  void     CTAna(TString histFName); 
  void     Show(Long64_t entry = -1);

  // fill unix time into unixTimeUSB array
  void GetUnixTime(Double_t *unixTimeUSB, Int_t Nmax, Int_t &nEv);
  //fill _Rate, _unixTimeBegin, _unixTimeEnd variables of the class 
  Int_t GetDataRate();
  void DrawUSBWCevent(Long64_t entry);
  void DrawUSBWCeventCh(Long64_t entry, Int_t chDR);
  Double_t _Rate;
  Double_t _unixTimeBegin;
  Double_t _unixTimeEnd;
  
  Bool_t CheckDistInTime();
  void saveUnixTime2File(TString fileN);
  void initH1_F(TH1D *h[anaConst::nChannels],
		TString hName , TString hTitle,
		Int_t nBin, Double_t binMin, 
		Double_t binMax);    
  void initH2_F(TH2D *h[anaConst::nChannels],
		TString hName , TString hTitle,
		Int_t nBin1, Double_t binMin1, 
		Double_t binMax1, 
		Int_t nBin2, Double_t binMin2, 
		Double_t binMax2);    

  //Double_t  idealCosmicWF_t[100][anaConst::nChannels];
  //Double_t  idealCosmicWF_A[100][anaConst::nChannels];
  //void SetIdealCosmicWF(TString nam, Int_t chID);

  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain
  
  // Declaration of leaf types
  Int_t          eventID;
  Float_t        SamplingPeriod;
  Int_t          INLCorrection;
  Int_t          FCR;
  Double_t       UnixTime;
  Float_t        ch[16][256];
  Float_t        Charge[16];
  Float_t        Time[16];
  Float_t        Amplitude[16];
  
  // List of branches
  TBranch        *b_eventID;   //!
  TBranch        *b_SamplingPeriod;   //!
  TBranch        *b_INLCorrection;   //!
  TBranch        *b_FCR;   //!
  TBranch        *b_UnixTime;   //!
  TBranch        *b_ch;   //!
  TBranch        *b_Charge;   //!
  TBranch        *b_Time;   //!
  TBranch        *b_Amplitude;   //!
  
};

#endif
