#ifndef anaData_hh
#define anaData_hh

//my
#include "../anaConst.hh"

//root
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>

//C, C++

using namespace std;

class TH1D;

class anaData {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           eventID_crt;
   Double_t        UnixTime_crt;
   Double_t        x1_crt;
   Double_t        y1_crt;
   Double_t        x2_crt;
   Double_t        y2_crt;
   Bool_t          hodoStatus_crt;
   Double_t        trkTheta_crt;
   Double_t        trkPhi_crt;
   Int_t           trkMomID_crt;
   Double_t        x_boxfTOF_crt;
   Double_t        y_boxfTOF_crt;
   Double_t        z_boxfTOF_crt;
   Double_t        x_boxfTOF_Bot_crt;
   Double_t        y_boxfTOF_Bot_crt;
   Double_t        z_boxfTOF_Bot_crt;
   Double_t        x_boxQuartzS_crt;
   Double_t        y_boxQuartzS_crt;
   Double_t        z_boxQuartzS_crt;
   Int_t           eventID_usb;
   Double_t        UnixTime_usb;
   Int_t           merged_eventID;
   Double_t        Charge_usb[16];
   Double_t        Time_usb[16];
   Double_t        Amplitude_usb[16];
   Int_t           wfID_usb[16];
   Double_t        MaxBaseLineAmpl_usb[16];
   Double_t        MinBaseLineAmpl_usb[16];
   Double_t        FirstAmplitudePos_usb[16];
   Double_t        FirstAmplitudeNeg_usb[16];
   Double_t        chargeTOT_usb[16];
   Double_t        chargeTOT_p_usb[16];
   Double_t        chargeTOT_m_usb[16];
   Double_t        FirstRiseTimePos_usb[16];
   Double_t        FirstFallTimePos_usb[16];
   Double_t        TotAmplitudePos_usb[16];
   Double_t        TotTimeAmplitudePos_usb[16];
   Double_t        FirstWidthTimePos_usb[16];
   Double_t        FirstTimeBasis_usb[16];
   Double_t        dTimeFirtsAmplPosChangLeftDer_usb[16];
   Double_t        dTimeFirtsAmplPosFirstTimeAmplNeg_usb[16];
   Double_t        MaxAmplBeforeFirstChangDerAmpl_usb[16];
   Double_t        MinAmplBeforeFirstChangDerAmpl_usb[16];
   Double_t        firstTimePosCFD_usb[16];
   Double_t        firstTimeNegCFD_usb[16];
   Int_t           nOfPosInterAtLevel_usb[16];
   Int_t           nOfNegInterAtLevel_usb[16];
   Double_t        SignalThreshold_usb;
   Double_t        crossTalkThreshold_usb;
   Double_t        mpFraction_usb;
   Double_t        valCFD_usb;
   Int_t           nSplinePoints_usb;
   Int_t           nPointBaseLine_usb;
   Double_t        levelch15_usb;
   Double_t        timeAtLevelch15_usb;


   // List of branches
   TBranch        *b_eventID_crt;   //!
   TBranch        *b_UnixTime_crt;   //!
   TBranch        *b_x1_crt;   //!
   TBranch        *b_y1_crt;   //!
   TBranch        *b_x2_crt;   //!
   TBranch        *b_y2_crt;   //!
   TBranch        *b_hodoStatus_crt;   //!
   TBranch        *b_trkTheta_crt;   //!
   TBranch        *b_trkPhi_crt;   //!
   TBranch        *b_trkMomID_crt;   //!
   TBranch        *b_x_boxfTOF_crt;   //!
   TBranch        *b_y_boxfTOF_crt;   //!
   TBranch        *b_z_boxfTOF_crt;   //!
   TBranch        *b_x_boxfTOF_Bot_crt;   //!
   TBranch        *b_y_boxfTOF_Bot_crt;   //!
   TBranch        *b_z_boxfTOF_Bot_crt;   //!
   TBranch        *b_x_boxQuartzS_crt;   //!
   TBranch        *b_y_boxQuartzS_crt;   //!
   TBranch        *b_z_boxQuartzS_crt;   //!
   TBranch        *b_eventID_usb;   //!
   TBranch        *b_UnixTime_usb;   //!
   TBranch        *b_merged_eventID;   //!
   TBranch        *b_Charge_usb;   //!
   TBranch        *b_Time_usb;   //!
   TBranch        *b_Amplitude_usb;   //!
   TBranch        *b_wfID_usb;   //!
   TBranch        *b_MaxBaseLineAmpl_usb;   //!
   TBranch        *b_MinBaseLineAmpl_usb;   //!
   TBranch        *b_FirstAmplitudePos_usb;   //!
   TBranch        *b_FirstAmplitudeNeg_usb;   //!
   TBranch        *b_chargeTOT_usb;   //!
   TBranch        *b_chargeTOT_p_usb;   //!
   TBranch        *b_chargeTOT_m_usb;   //!
   TBranch        *b_FirstRiseTimePos_usb;   //!
   TBranch        *b_FirstFallTimePos_usb;   //!
   TBranch        *b_TotAmplitudePos_usb;   //!
   TBranch        *b_TotTimeAmplitudePos_usb;   //!
   TBranch        *b_FirstWidthTimePos_usb;   //!
   TBranch        *b_FirstTimeBasis_usb;   //!
   TBranch        *b_dTimeFirtsAmplPosChangLeftDer_usb;   //!
   TBranch        *b_dTimeFirtsAmplPosFirstTimeAmplNeg_usb;   //!
   TBranch        *b_MaxAmplBeforeFirstChangDerAmpl_usb;   //!
   TBranch        *b_MinAmplBeforeFirstChangDerAmpl_usb;   //!
   TBranch        *b_firstTimePosCFD_usb;   //!
   TBranch        *b_firstTimeNegCFD_usb;   //!
   TBranch        *b_nOfPosInterAtLevel_usb;   //!
   TBranch        *b_nOfNegInterAtLevel_usb;   //!
   TBranch        *b_SignalThreshold_usb;   //!
   TBranch        *b_crossTalkThreshold_usb;   //!
   TBranch        *b_mpFraction_usb;   //!
   TBranch        *b_valCFD_usb;   //!
   TBranch        *b_nSplinePoints_usb;   //!
   TBranch        *b_nPointBaseLine_usb;   //!
   TBranch        *b_levelch15_usb;   //!
   TBranch        *b_timeAtLevelch15_usb;   //!

   anaData(TString name);
   virtual ~anaData();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(TString histN);
   virtual void     LoopNew(TString histN);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   Bool_t cutsPerChannel(Int_t kCh1,Int_t kCh2);
   Bool_t cutsPerTrk();

private:
  void initH1_F(TH1D *h[anaConst::nChannels],
		TString hName , TString hTitle,
		Int_t nBin, Double_t binMin, 
		Double_t binMax);

  Int_t _nPeTotPerEv_AmpCut;
};

#endif


