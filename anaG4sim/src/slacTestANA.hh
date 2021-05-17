#ifndef slacTestANA_hh
#define slacTestANA_hh

//my
#include "../../anaConst.hh"

//root
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1D.h>
#include "TRandom3.h"

//C, C++
#include <string>
#include <iostream>

using namespace std;

class slacTestANA {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           EventID;
   Int_t           BunchXID;
   Int_t           NTotPhot;
   Int_t           Nhits;
   Int_t           primType;
   Double_t        primMomX;
   Double_t        primMomY;
   Double_t        primMomZ;
   Double_t        primPosX;
   Double_t        primPosY;
   Double_t        primPosZ;
   Double_t        primTime;
   Int_t           nPhot;
   Int_t           TrackID[2317];     //[nPhot]
   Int_t           ParentID[2317];    //[nPhot]
   Double_t        Energy[2317];      //[nPhot]
   Double_t        Wavelength[2317];  //[nPhot]
   Double_t        Time[2317];        //[nPhot]
   Double_t        photPathLen[2317]; //[nPhot]
   Int_t           SecID[2317];       //[nPhot]
   Int_t           chID[2317];        //[nPhot]
   Double_t        PosX[2317];        //[nPhot]
   Double_t        PosY[2317];        //[nPhot]
   Double_t        PosZ[2317];        //[nPhot]
   Double_t        trkMomX[2317];     //[nPhot]
   Double_t        trkMomY[2317];     //[nPhot]
   Double_t        trkMomZ[2317];     //[nPhot]
   Double_t        trkPosX[2317];     //[nPhot]
   Double_t        trkPosY[2317];     //[nPhot]
   Double_t        trkPosZ[2317];     //[nPhot]
   Double_t        trkT[2317];        //[nPhot]
   Double_t        trkLength[2317];   //[nPhot]

   // List of branches
   TBranch        *b_Event;   //!
   TBranch        *b_BunchXID;   //!
   TBranch        *b_NTotPhot;   //!
   TBranch        *b_Nhits;   //!
   TBranch        *b_primType;   //!
   TBranch        *b_primMomX;   //!
   TBranch        *b_primMomY;   //!
   TBranch        *b_primMomZ;   //!
   TBranch        *b_primPosX;   //!
   TBranch        *b_primPosY;   //!
   TBranch        *b_primPosZ;   //!
   TBranch        *b_primTime;   //!
   TBranch        *b_nPhot;   //!
   TBranch        *b_TrackID;   //!
   TBranch        *b_ParentID;   //!
   TBranch        *b_Energy;   //!
   TBranch        *b_Wavelength;   //!
   TBranch        *b_Time;   //!
   TBranch        *b_photPathLen;   //!
   TBranch        *b_SecID;   //!
   TBranch        *b_chID;   //!
   TBranch        *b_PosX;   //!
   TBranch        *b_PosY;   //!
   TBranch        *b_PosZ;   //!
   TBranch        *b_trkMomX;   //!
   TBranch        *b_trkMomY;   //!
   TBranch        *b_trkMomZ;   //!
   TBranch        *b_trkPosX;   //!
   TBranch        *b_trkPosY;   //!
   TBranch        *b_trkPosZ;   //!
   TBranch        *b_trkT;   //!
   TBranch        *b_trkLength;   //!

   slacTestANA(string fileName, UInt_t runSeed);
   slacTestANA(TString name, Int_t key, UInt_t runSeed);
   virtual ~slacTestANA();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(TString histFileName);
   void LoopWfSim(TString histFileName);
   void LoopSimpl();
   Bool_t kinemCuts( Double_t primPhi, Double_t primTheta);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

 private:
   
  void getShapeFromFile(TString fileN, 
			double *xxMcpPM, double *yyMcpPM, 
			int &N, double &timeMax);
  ////////////////////////
  Double_t calculateTimeSpread(Double_t TimeMy);
  Bool_t ifBialkali_QE(Double_t WavelengthMy); 
  double interpolateAcceptProb( double xmin, double xmax,
				double ymin, double ymax, 
				double x);
  
  void normalizeh1QE(TH1D *hh1,TH1D *h1norm);
  
  TRandom3 *_Rand3;
  
  void initH1_F(TH1D *h[anaConst::nChannels],
		TString hName , TString hTitle,
		Int_t nBin, Double_t binMin, 
		Double_t binMax);    
  
  Int_t GetChID(Double_t photPosY, Double_t  photPosZ);
  
  void findFirstTime(double timePe[anaConst::nChannels][anaConst::nMaxPe], Int_t nPe[anaConst::nChannels]);

};

#endif
