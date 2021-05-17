//my
#include "anaData.hh"

//root
#include <TH2D.h>
#include <TH1D.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TString.h>

//C, C++
#include <string>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

anaData::anaData(TString name){
  _nPeTotPerEv_AmpCut = 0;
  // if parameter tree is not specified (or zero), connect the file
  // used to generate this class and read the Tree.
  //if (tree == 0) {
  //TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("merged.root");
  //if (!f) {
  //f = new TFile("merged.root");
  //}
  //tree = (TTree*)gDirectory->Get("T");
  //}
  //Init(tree);
  cout<<endl<<endl<<name<<endl;
  ifstream indata;
  string rootFileName;
  TChain *theChain = new TChain("T");
  indata.open(name.Data()); 
  assert(indata.is_open());  
  while (indata  >> rootFileName ){
    if(indata.eof()){
      std::cout<<"EOF"<<std::endl;
      break;
    }
    // NA 2010/11/03
    //TFile *f = new TFile(rootFileName.c_str());
    cout<<"        adding "<<rootFileName<<endl;
    theChain->Add(rootFileName.c_str(),-1);
  }
  indata.close();
  Init(theChain);
}

anaData::~anaData(){
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

void anaData::Loop(TString histN){

  Int_t i = 0;
  Int_t kCh1 = 0;
  Int_t kCh2 = 0;
  Int_t nRecCh3 = 0;
  Int_t nTotCh3 = 0;
  Int_t nCutsPerTrk_ok = 0;
  Int_t nTotTrkRec = 0;

  //////////// HISTOS ////////////
  //USBWC
  TH1D *h1Charge[anaConst::nChannels];       //all events
  TH1D *h1Time[anaConst::nChannels];         //all events
  TH1D *h1Amplitude[anaConst::nChannels];    //all events

  TH1D *h1dTimeL[anaConst::nChannels];       //
  TH1D *h1dTimeL2[anaConst::nChannels];      //
  TH1D *h1dTimeL3[anaConst::nChannels];      //
  TH1D *h1dTimeL4[anaConst::nChannels];      //
  TH1D *h1dTimeTOPtoBOT[anaConst::nChannels];//
  TH1D *h1dTime15[anaConst::nChannels];      //
  TH1D *h1dTimeUSBsoftMySoft[anaConst::nChannels];//

  //my
  TH1D *h1Charge_my[anaConst::nChannels];   //all events
  TH1D *h1Amplitude_my[anaConst::nChannels];//all events
  TH1D *h1Rise_my[anaConst::nChannels];     //all events
  TH1D *h1Width_my[anaConst::nChannels];    //all events

  TH1D *h1Charge_my_baseCut[anaConst::nChannels];
  TH1D *h1Amplitude_my_baseCut[anaConst::nChannels];
  TH1D *h1AmplitudeNeg_my_baseCut[anaConst::nChannels];
  TH1D *h1Rise_my_baseCut[anaConst::nChannels];
  TH1D *h1Fall_my_baseCut[anaConst::nChannels];
  TH1D *h1Width_my_baseCut[anaConst::nChannels];
  TH1D *h1nOfPosInterAtLevel_my_baseCut[anaConst::nChannels];
  TH1D *h1nOfNegInterAtLevel_my_baseCut[anaConst::nChannels];
  TH1D *h1MaxAmplBeforeFirstChangDerAmpl_my_baseCut[anaConst::nChannels];
  TH1D *h1MaxBaseLineAmpl_my_baseCut[anaConst::nChannels];
  TH1D *h1TotAmplitudePos_my_baseCut[anaConst::nChannels];
  TH1D *h1chanUsage_my_baseCut = new TH1D("h1chanUsage_my_baseCut","chanUsage my baseCut", anaConst::nChannels,0,anaConst::nChannels);

  TH1D *h1Sep[anaConst::nChannels];
  TH1D *h1Charge_my_cut[anaConst::nChannels];         //Events which contribute to the delta time measurements 
  TH1D *h1ChargeNorm_my_cut[anaConst::nChannels];     //Events which contribute to the delta time measurements 
  TH1D *h1SummNormCharge_my_cut[anaConst::nChannels]; //Events which contribute to the delta time measurements 
  TH1D *h1Amplitude_my_cut[anaConst::nChannels];      //Events which contribute to the delta time measurements 
  TH1D *h1Time_my_cut[anaConst::nChannels];           //Events which contribute to the delta time measurements 
  TH1D *h1Rise_cut[anaConst::nChannels];              //Events which contribute to the delta time measurements 
  TH1D *h1Width_cut[anaConst::nChannels];             //Events which contribute to the delta time measurements 

  initH1_F(h1Sep,"h1Sep","____________________",10,0,10);
  initH1_F(h1Charge,"h1Charge","Charge",200,-10,30);
  initH1_F(h1Time,"h1Time","Time",2000,-10.0,100);
  initH1_F(h1Amplitude,"h1Amplitude","Amplitude",1000,-1.5,1.5);
  
  initH1_F(h1dTimeL,"h1dTimeL","dTimeL"                     ,  1400, -10.0, 10);
  initH1_F(h1dTimeL2,"h1dTimeL2","dTimeL2"                  ,  1400, -10.0, 10);
  initH1_F(h1dTimeL3,"h1dTimeL3","dTimeL3"                  ,  1400, -10.0, 10);
  initH1_F(h1dTimeL4,"h1dTimeL4","dTimeL4"                  ,  1400, -10.0, 10);
  initH1_F(h1dTimeTOPtoBOT,"h1dTimeTOPtoBOT","dTimeTOPtoBOT",  1400, -10.0, 10);
  initH1_F(h1dTime15,"h1dTime15","dTime15"                  , 3000,   0.0, 30);
  initH1_F(h1dTimeUSBsoftMySoft,"h1dTimeUSBsoftMySoft","dTimeUSBsoftMySoft", 5000, -10.0, 10);

  initH1_F(h1Charge_my, "h1Charge_my", "Charge my",1000,-10.0,10.0);
  initH1_F(h1Amplitude_my,"h1Amplitude_my", "Amplitude my",1000,-1.5,1.5);
  initH1_F(h1Rise_my,"h1Rise_my","Rise my",300,0.0,3.0);
  initH1_F(h1Width_my,"h1Width_my","Width my",300,0.0,10.0);

  initH1_F(h1Charge_my_baseCut, "h1Charge_my_baseCut", "Charge my baseCut",1000,-10.0,10.0);
  initH1_F(h1Amplitude_my_baseCut,"h1Amplitude_my_baseCut", "Amplitude my baseCut",1000,-1.5,1.5);
  initH1_F(h1Rise_my_baseCut,"h1Rise_my_baseCut","Rise my baseCut",300,0.0,3.0);
  initH1_F(h1Width_my_baseCut,"h1Width_my_baseCut","Width my baseCut",300,0.0,10.0);
  initH1_F(h1AmplitudeNeg_my_baseCut,"h1AmplitudeNeg_my_baseCut", "AmplitudeNeg my baseCut",1000,-1.5,1.5);
  initH1_F(h1Fall_my_baseCut,"h1Fall_my_baseCut","Fall my baseCut",300,0.0,5.0);
  initH1_F(h1nOfPosInterAtLevel_my_baseCut,"h1nOfPosInterAtLevel_my_baseCut","nOfPosInterAtLevel my baseCut",20,0,20);
  initH1_F(h1nOfNegInterAtLevel_my_baseCut,"h1nOfNegInterAtLevel_my_baseCut","nOfNegInterAtLevel my baseCut",20,0,20);
  initH1_F(h1MaxAmplBeforeFirstChangDerAmpl_my_baseCut,"h1MaxAmplBeforeFirstChangDerAmpl_my_baseCut", "MaxAmplBeforeFirstChangDerAmpl my baseCut",1000,-1.5,1.5);
  initH1_F(h1MaxBaseLineAmpl_my_baseCut,"h1MaxBaseLineAmpl_my_baseCut", "MaxBaseLineAmpl my baseCut",1000,-1.5,1.5);
  initH1_F(h1TotAmplitudePos_my_baseCut,"h1TotAmplitudePos_my_baseCut", "TotAmplitudePos my baseCut",1000,-1.5,1.5);

  initH1_F(h1Charge_my_cut,"h1Charge_my_cut","Charge my cut",1000, -10.0, 10.0);
  initH1_F(h1ChargeNorm_my_cut,"h1ChargeNorm_my_cut","ChargeNorm my cut",1000, -10.0, 10.0);
  initH1_F(h1SummNormCharge_my_cut,"h1SummNormCharge_my_cut","h1SummNormCharge_my_cut",1000, -10.0, 10.0);
  initH1_F(h1Amplitude_my_cut,"h1Amplitude_my_cut","Amplitude my cut",1000,-1.5,1.5);
  initH1_F(h1Time_my_cut,"h1Time_my_cut","Time my cut"               ,1000,0.0,80.0);
  initH1_F(h1Rise_cut,"h1Rise_cut","Rise cut"                        ,300,0.0,3.0);
  initH1_F(h1Width_cut,"h1Width_cut","Width cut"                     ,300,0.0,10.0);
  
  //run 1
  //TH2D *h2dTL2vsUnixTime = new TH2D("h2dTL2vsUnixTime","dTL2vsUnixTime",10000,1286236959.94700,1291392634.42800, 500, -10,10); 
  //run 2
  //TH2D *h2dTL2vsUnixTime = new TH2D("h2dTL2vsUnixTime","dTL2vsUnixTime", 10000, 1291747289.60200, 1295881291.99800, 500, -10, 10); 
  //run 3
  //TH2D *h2dTL2vsUnixTime = new TH2D("h2dTL2vsUnixTime","dTL2vsUnixTime", 10000, 1296238879.16300, 1297119015.67900, 500, -10, 10); 

  Int_t nBins = (Int_t)((1297119015.67900 - 1286236959.94700)/(3600.0*4)); 
  TH2D *h2dTL2vsUnixTime = new TH2D("h2dTL2vsUnixTime","dTL2vsUnixTime", nBins, 1286236959.94700, 1297119015.67900, 200, -10, 10); 
  TH2D *h2dTLvsUnixTime = new TH2D("h2dTLvsUnixTime","dTLvsUnixTime", nBins, 1286236959.94700, 1297119015.67900, 200, -10, 10); 

  TH1D *h1x1 = new TH1D( "h1x1", "x1", 200, anaConst::hodoYBar_lenght/(-2.0), anaConst::hodoYBar_lenght/2.0);
  TH1D *h1x2 = new TH1D( "h1x2", "x2", 200, anaConst::hodoYBar_lenght/(-2.0), anaConst::hodoYBar_lenght/2.0);
  TH1D *h1y1 = new TH1D( "h1y1", "y1", 200, anaConst::hodoXBar_lenght/(-2.0), anaConst::hodoXBar_lenght/2.0);
  TH1D *h1y2 = new TH1D( "h1y2", "y2", 200, anaConst::hodoXBar_lenght/(-2.0), anaConst::hodoXBar_lenght/2.0);

  //TH2D *h1yvsx_FTOF_top = new TH2D("h1yvsx_FTOF_top", "yvsx FTOF top", (Int_t)(anaConst::hodoYBar_lenght), anaConst::hodoYBar_lenght/(-2.0), anaConst::hodoYBar_lenght/2.0,
  //			                                       (Int_t)(anaConst::hodoXBar_lenght), anaConst::hodoXBar_lenght/(-2.0), anaConst::hodoXBar_lenght/2.0);
  TH2D *h1yvsx_FTOF_top = new TH2D("h1yvsx_FTOF_top", "yvsx FTOF top", 100, -20.0, 20.0, 100, -10.0, 10.0);
  
  //TH1D *h1x_FTOF_top = new TH1D("h1x_FTOF_top", "x FTOF top", (Int_t)(anaConst::hodoYBar_lenght)*4, 
  //			anaConst::hodoYBar_lenght/(-2.0), anaConst::hodoYBar_lenght/2.0);
  //TH1D *h1y_FTOF_top = new TH1D("h1y_FTOF_top", "y FTOF top", (Int_t)(anaConst::hodoXBar_lenght)*4, 
  //			anaConst::hodoXBar_lenght/(-2.0), anaConst::hodoXBar_lenght/2.0);
  TH1D *h1x_FTOF_top = new TH1D("h1x_FTOF_top", "x FTOF top", 100, -20.0, 20.0);
  TH1D *h1y_FTOF_top = new TH1D("h1y_FTOF_top", "y FTOF top", 100, -10.0, 10.0);

  TH1D *h1trkPhi   = new TH1D("h1trkPhi",   "trk Phi",   200, -190.0, 190.0);
  TH1D *h1trkTheta = new TH1D("h1trkTheta", "trk Theta", 200,    0.0, 190.0);
  
  TH1D *h1trkMomID = new TH1D("h1trkMomID","trkMomID",9, -2, 7);
  
  TH1D *h1nPeTotPerEv_baseCut = new TH1D("h1nPeTotPerEv_baseCut", "nPeTot Per Ev baseCut", 20, 0.0, 20.0);
  TH1D *h1nPeTotPerEv_AmpCut  = new TH1D("h1nPeTotPerEv_AmpCut", "nPeTot Per Ev AmpCut", 20, 0.0, 20.0);
  Int_t nPeTotPerEv_baseCut = 0;
  Int_t nPeTotPerEv_AmpCut = 0;
  ////////////////////////////////

  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  cout<<"nentries "<<nentries<<endl;
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    //if(jentry == 0)
    //printf(" UnixTime_usb  = %20.5f \n", UnixTime_usb);

    if(jentry%50000==0)
      cout<<" processed events "<<jentry<<endl;    

    if(cutsPerTrk()){
      h1yvsx_FTOF_top->Fill(x_boxfTOF_crt, y_boxfTOF_crt);
      h1x_FTOF_top->Fill(x_boxfTOF_crt);
      h1y_FTOF_top->Fill(y_boxfTOF_crt);
      h1trkPhi->Fill(trkPhi_crt);
      h1trkTheta->Fill(trkTheta_crt);

      h1trkMomID->Fill(trkMomID_crt, 1.0/36530.0);

      nCutsPerTrk_ok++;
    }

    if(merged_eventID>-1.0)
      nTotTrkRec++;
    
    for(i=0;i<anaConst::nChannels;i++){
      h1Charge[i]->Fill(Charge_usb[i]);
      h1Time[i]->Fill(Time_usb[i]);
      h1Amplitude[i]->Fill(Amplitude_usb[i]);
      h1Charge_my[i]->Fill(chargeTOT_usb[i]);
      h1Amplitude_my[i]->Fill(FirstAmplitudePos_usb[i]);
      h1Rise_my[i]->Fill(FirstRiseTimePos_usb[i]);
      h1Width_my[i]->Fill(FirstWidthTimePos_usb[i]);
      h1dTimeUSBsoftMySoft[i]->Fill(firstTimePosCFD_usb[i] -Time_usb[i]);
    }

    nPeTotPerEv_baseCut = 0;
    nPeTotPerEv_AmpCut = 0;
    for(i = 0;i<anaConst::nChannels;i++){
      //if(FirstAmplitudePos_usb[i]>0.08 && i != 15){
      if(FirstAmplitudePos_usb[i]>0.08 && i != 15 ){
	nPeTotPerEv_AmpCut++;
      }
    }
    _nPeTotPerEv_AmpCut = nPeTotPerEv_AmpCut;

    for(i = 0;i<anaConst::nChannels;i++){
      if(cutsPerChannel(i,i)){
	h1Charge_my_baseCut[i]->Fill(chargeTOT_usb[i]);
	h1Amplitude_my_baseCut[i]->Fill(FirstAmplitudePos_usb[i]);
	h1Rise_my_baseCut[i]->Fill(FirstRiseTimePos_usb[i]);
	h1Width_my_baseCut[i]->Fill(FirstWidthTimePos_usb[i]);
	h1AmplitudeNeg_my_baseCut[i]->Fill(FirstAmplitudeNeg_usb[i]);
	h1Fall_my_baseCut[i]->Fill(FirstFallTimePos_usb[i]);
	h1nOfPosInterAtLevel_my_baseCut[i]->Fill(nOfPosInterAtLevel_usb[i]);
	h1nOfNegInterAtLevel_my_baseCut[i]->Fill(nOfNegInterAtLevel_usb[i]);
	h1MaxAmplBeforeFirstChangDerAmpl_my_baseCut[i]->Fill(MaxAmplBeforeFirstChangDerAmpl_usb[i]);
	h1MaxBaseLineAmpl_my_baseCut[i]->Fill(MaxBaseLineAmpl_usb[i]);
	h1TotAmplitudePos_my_baseCut[i]->Fill(TotAmplitudePos_usb[i]);
	h1chanUsage_my_baseCut->Fill(i);
	if(i != 15)
	  nPeTotPerEv_baseCut++;
      }
    }
    
    h1nPeTotPerEv_baseCut->Fill(nPeTotPerEv_baseCut);
    h1nPeTotPerEv_AmpCut->Fill(nPeTotPerEv_AmpCut);
    
    //--- time DIFFL ---
    //0    ch0  - ch1
    //2    ch2  - ch3
    //4    ch4  - ch5
    //6    ch6  - ch7
    //8    ch8  - ch9
    //10   ch10 - ch11
    //12   ch12 - ch13
    //14   ch14 - ch15
    for(i = 0;i<8;i++){
      kCh1 = 2*i;
      kCh2 = 2*i+1;
      if(cutsPerChannel(kCh1,kCh2)){
	h1dTimeL[kCh1]->Fill(firstTimePosCFD_usb[kCh1] - firstTimePosCFD_usb[kCh2]);
	//h1dTimeUSBsoftMySoft[kCh1]->Fill(firstTimePosCFD_usb[kCh1] -Time_usb[kCh1]);
	//h1dTimeUSBsoftMySoft[kCh2]->Fill(firstTimePosCFD_usb[kCh2] -Time_usb[kCh2]);
	if(kCh1 == 2)
	  h2dTLvsUnixTime->Fill(UnixTime_usb, firstTimePosCFD_usb[2] - firstTimePosCFD_usb[3]);	
	h1Charge_my_cut[kCh1]->Fill(chargeTOT_usb[kCh1]);
	h1ChargeNorm_my_cut[kCh1]->Fill(chargeTOT_usb[kCh1]/anaConst::chargeMean[kCh1]);
	h1Amplitude_my_cut[kCh1]->Fill(FirstAmplitudePos_usb[kCh1]);
	h1Time_my_cut[kCh1]->Fill(firstTimePosCFD_usb[kCh1]);
	h1Rise_cut[kCh1]->Fill(FirstRiseTimePos_usb[kCh1]);
	h1Width_cut[kCh1]->Fill(FirstWidthTimePos_usb[kCh1]);	
	h1Charge_my_cut[kCh2]->Fill(chargeTOT_usb[kCh2]);
	h1ChargeNorm_my_cut[kCh2]->Fill(chargeTOT_usb[kCh2]/anaConst::chargeMean[kCh2]);
	h1Amplitude_my_cut[kCh2]->Fill(FirstAmplitudePos_usb[kCh2]);
	h1Time_my_cut[kCh2]->Fill(firstTimePosCFD_usb[kCh2]);
	h1Rise_cut[kCh2]->Fill(FirstRiseTimePos_usb[kCh2]);
	h1Width_cut[kCh2]->Fill(FirstWidthTimePos_usb[kCh2]);
	h1SummNormCharge_my_cut[kCh1]->Fill(chargeTOT_usb[kCh2]/anaConst::chargeMean[kCh2] + chargeTOT_usb[kCh1]/anaConst::chargeMean[kCh1]);
      }
    }
    //---- time DIFFL ---
    
    //---- time DIFFL2 ---
    // 1  ch1 - ch2
    // 3  ch3 - ch4
    // 5  ch5 - ch6
    for(i = 0;i<3;i++){
      kCh1 = 2*i+1;
      kCh2 = 2*i+2;
      if(cutsPerChannel(kCh1,kCh2)){
	h1dTimeL2[kCh1]->Fill(firstTimePosCFD_usb[kCh1] - firstTimePosCFD_usb[kCh2]);
	if(kCh1==1)
	  h2dTL2vsUnixTime->Fill(UnixTime_usb, firstTimePosCFD_usb[3] - firstTimePosCFD_usb[4]);
      }
    }
    // 9  ch9  - ch10
    // 11  ch11 - ch12
    // 13  ch13 - ch14
    for(i = 4;i<7;i++){
      kCh1 = 2*i+1;
      kCh2 = 2*i+2;
      if(cutsPerChannel(kCh1,kCh2)){
	h1dTimeL2[kCh1]->Fill(firstTimePosCFD_usb[kCh1] - firstTimePosCFD_usb[kCh2]);
      }
    }
    //---- time DIFFL2 ----


    //---- time DIFFL3 ---
    // 1  ch1 - ch2
    // 3  ch3 - ch4
    // 5  ch5 - ch6
    kCh1 = 2;
    kCh2 = 4;
    if(cutsPerChannel(kCh1,kCh2)){
      //if(nPeTotPerEv_AmpCut<5)
	h1dTimeL3[kCh1]->Fill(firstTimePosCFD_usb[kCh1] - firstTimePosCFD_usb[kCh2]);
    }
    kCh1 = 3;
    kCh2 = 5;
    if(cutsPerChannel(kCh1,kCh2)){
      //if(nPeTotPerEv_AmpCut<5)
	h1dTimeL3[kCh1]->Fill(firstTimePosCFD_usb[kCh1] - firstTimePosCFD_usb[kCh2]);
    }
    kCh1 = 10;
    kCh2 = 12;
    if(cutsPerChannel(kCh1,kCh2)){
      //if(nPeTotPerEv_AmpCut<5)
	h1dTimeL3[kCh1]->Fill(firstTimePosCFD_usb[kCh1] - firstTimePosCFD_usb[kCh2]);
    }
    kCh1 = 11;
    kCh2 = 13;
    if(cutsPerChannel(kCh1,kCh2)){
      //if(nPeTotPerEv_AmpCut<5)
	h1dTimeL3[kCh1]->Fill(firstTimePosCFD_usb[kCh1] - firstTimePosCFD_usb[kCh2]);
    }
    //---- time DIFFL3 ----
    
    
    //---- time DIFFL4 ---
    //0   ch0 - ch8
    //1   ch1 - ch9
    //2   ch2 - ch10
    //3   ch3 - ch11
    //4   ch4 - ch12
    //5   ch5 - ch13
    //6   ch6 - ch14
    //7   ch7 - ch15
    for(i = 0;i<8;i++){
      kCh1 = i;
      kCh2 = 15-i;
      if(cutsPerChannel(kCh1,kCh2)){
	h1dTimeL4[kCh1]->Fill(firstTimePosCFD_usb[kCh1] - firstTimePosCFD_usb[kCh2]);
      }
    }
    //---- time DIFFL4 ---


    //---- time DIFFTOPTOBOT ---
    //0   ch0 - ch8
    //1   ch1 - ch9
    //2   ch2 - ch10
    //3   ch3 - ch11
    //4   ch4 - ch12
    //5   ch5 - ch13
    //6   ch6 - ch14
    //7   ch7 - ch15
    for(i = 0;i<8;i++){
      kCh1 = i;
      kCh2 = i+8;
      if(cutsPerChannel(kCh1,kCh2)){
	h1dTimeTOPtoBOT[kCh1]->Fill(firstTimePosCFD_usb[kCh1] - firstTimePosCFD_usb[kCh2]);
      }
    }
    //---- time DIFFTOPTOBOT ---

    
    //---- time DIFF15 ---
    for(i = 0;i<14;i++){
      if(timeAtLevelch15_usb>0.0 && Time_usb[i]>0.0 && wfID_usb[i]==1){
	h1dTime15[i]->Fill(timeAtLevelch15_usb - Time_usb[i]);
	
	if(i == 3){
	  nTotCh3++;
	  if(x1_crt > -200.0 && x2_crt > -200.0 && y1_crt > -200.0 && y2_crt > -200.0){
	    h1x1->Fill(x1_crt);
	    h1x2->Fill(x2_crt);
	    h1y1->Fill(y1_crt);
	    h1y2->Fill(y2_crt);
	    nRecCh3++;
	  }
	}
      }
    }
    //---- time DIFF15 ---

  }

  cout<<"nTotCh3 "<<nTotCh3<<endl
      <<"nRecCh3 "<<nRecCh3<<endl;
  
  TFile* rootFile = new TFile(histN.Data(), "RECREATE", " Histograms from USBWC merged with CRT", 1);
  rootFile->cd();
  if (rootFile->IsZombie()){
    cout<<"  ERROR ---> file "<<histN<<" is zombi"<<endl;
    assert(0);
  }
  else
    cout<<"  Output Histos file ---> "<<histN<<endl;

  for(i=0;i<anaConst::nChannels;i++){
    h1Sep[i]->Write();
    h1Charge[i]->Write();
    h1Time[i]->Write();
    h1Amplitude[i]->Write();
    h1Charge_my[i]->Write();
    h1Amplitude_my[i]->Write();
    h1Rise_my[i]->Write();
    h1Width_my[i]->Write();

    h1dTimeL[i]->Write();
    h1dTimeL2[i]->Write();
    h1dTimeL3[i]->Write();
    h1dTimeL4[i]->Write();
    h1dTimeTOPtoBOT[i]->Write();
    h1dTime15[i]->Write();
    h1dTimeUSBsoftMySoft[i]->Write();

    h1Charge_my_baseCut[i]->Write();
    h1Amplitude_my_baseCut[i]->Write();
    h1Rise_my_baseCut[i]->Write();
    h1Width_my_baseCut[i]->Write();
    h1AmplitudeNeg_my_baseCut[i]->Write();
    h1Fall_my_baseCut[i]->Write();
    h1nOfPosInterAtLevel_my_baseCut[i]->Write();
    h1nOfNegInterAtLevel_my_baseCut[i]->Write();
    h1MaxAmplBeforeFirstChangDerAmpl_my_baseCut[i]->Write();
    h1MaxBaseLineAmpl_my_baseCut[i]->Write();
    h1TotAmplitudePos_my_baseCut[i]->Write();

    h1Charge_my_cut[i]->Write();
    h1ChargeNorm_my_cut[i]->Write();
    h1SummNormCharge_my_cut[i]->Write();
    h1Amplitude_my_cut[i]->Write();
    h1Time_my_cut[i]->Write();
    h1Rise_cut[i]->Write();
    h1Width_cut[i]->Write();
  }    
  
  h2dTL2vsUnixTime->Write();
  h2dTLvsUnixTime->Write();

  h1x1->Write();
  h1x2->Write();
  h1y1->Write();
  h1y2->Write();

  h1yvsx_FTOF_top->Write();
  h1x_FTOF_top->Write();
  h1y_FTOF_top->Write();
  h1trkMomID->Write();
  h1trkPhi->Write();
  h1trkTheta->Write();

  h1chanUsage_my_baseCut->Write();

  h1nPeTotPerEv_baseCut->Write();
  h1nPeTotPerEv_AmpCut->Write();

  cout<<"nCutsPerTrk_ok  "<<nCutsPerTrk_ok<<endl
      <<"nTotTrkRec      "<<nTotTrkRec<<endl;

}


void anaData::LoopNew(TString histN){

  Int_t i = 0;
  Int_t nSigChannels = 0;

  Int_t nSP = 0;
  Int_t nCT = 0;
  Int_t sigChID[anaConst::nChannels];

  Int_t nSig1Cut = 0;
  Int_t nSig2Cut = 0;
  Int_t nSig3Cut = 0;
  Int_t nSig4Cut = 0;
  Int_t nSig5Cut = 0;

  //////////// HISTOS ////////////
  //no cuts
  TH1D *h1Sep[anaConst::nChannels];
  TH1D *h1Time_my[anaConst::nChannels];               //all events
  TH1D *h1firstTimeNegCFD_my[anaConst::nChannels];    //all events
  TH1D *h1Rise_my[anaConst::nChannels];               //all events
  TH1D *h1Width_my[anaConst::nChannels];              //all events
  TH1D *h1Fall_my[anaConst::nChannels];               //all events
  TH1D *h1FirstTimeBasis_my[anaConst::nChannels];     //all events
  TH1D *h1Charge_my[anaConst::nChannels];             //all events
  TH1D *h1Amplitude_my[anaConst::nChannels];          //all events
  TH1D *h1AmplMax_my[anaConst::nChannels];            //all events
  TH1D *h1AmplitudeNeg_my[anaConst::nChannels];       //all events
  TH1D *h1nOfPosInterAtLevel_my[anaConst::nChannels]; //all events
  TH1D *h1nOfNegInterAtLevel_my[anaConst::nChannels]; //all events
  TH1D *h1wfID_my[anaConst::nChannels];               //all events

  initH1_F(h1Sep,"h1Sep","___________________________",10,0,10);
  initH1_F(h1Time_my,"h1Time_my","Time my", 3200,0.0,80.0);
  initH1_F(h1firstTimeNegCFD_my,"h1firstTimeNegCFD_my","firstTimeNegCFD my", 3200,0.0,80.0);
  initH1_F(h1Rise_my,"h1Rise_my","Rise my",300,0.0,3.0);
  initH1_F(h1Width_my,"h1Width_my","Width my",400,0.0,5.0);
  initH1_F(h1Fall_my,"h1Fall_my","h1Fall my",300,0.0,10.0);
  initH1_F(h1FirstTimeBasis_my,"h1FirstTimeBasis_my","FirstTimeBasis my", 300, 0.0,10.0);
  initH1_F(h1Charge_my, "h1Charge_my", "Charge my",1000,-10.0,10.0);
  initH1_F(h1Amplitude_my,"h1Amplitude_my", "Amplitude my",1000,-1.5,1.5);
  initH1_F(h1AmplMax_my, "h1AmplMax_my","AmplMax my",1000,-1.5,1.5);
  initH1_F(h1AmplitudeNeg_my,"h1AmplitudeNeg_my","AmplitudeNeg my",3000,-1.5,1.5);
  initH1_F(h1nOfPosInterAtLevel_my,"h1nOfPosInterAtLevel_my","nOfPosInterAtLevel my",50,0,50);
  initH1_F(h1nOfNegInterAtLevel_my,"h1nOfNegInterAtLevel_my","nOfNegInterAtLevel my",100,0,100);
  initH1_F(h1wfID_my,"h1wfID_my","wfID my",10,-2,8);

  //CT
  TH1D *h1Amplitude_my_CT[anaConst::nChannels];
  TH1D *h1AmplitudeNeg_my_CT[anaConst::nChannels];
  TH1D *h1Charge_my_CT[anaConst::nChannels];
  TH1D *h1Rise_my_CT[anaConst::nChannels];
  TH1D *h1Width_my_CT[anaConst::nChannels];
  TH1D *h1Fall_my_CT[anaConst::nChannels];
  TH1D *h1FirstTimeBasis_my_CT[anaConst::nChannels];

  initH1_F(h1Amplitude_my_CT,"h1Amplitude_my_CT", "Amplitude my CT",1000,-1.5,1.5);
  initH1_F(h1AmplitudeNeg_my_CT,"h1AmplitudeNeg_my_CT","AmplitudeNeg my CT",3000,-1.5,1.5);
  initH1_F(h1Charge_my_CT, "h1Charge_my_CT", "Charge my CT",1000,-10.0,10.0);
  initH1_F(h1Rise_my_CT,"h1Rise_my_CT","Rise my CT",300,0.0,3.0);
  initH1_F(h1Width_my_CT,"h1Width_my_CT","Width my CT",400,0.0,5.0);
  initH1_F(h1Fall_my_CT,"h1Fall_my_CT","h1Fall my CT",300,0.0,10.0);
  initH1_F(h1FirstTimeBasis_my_CT,"h1FirstTimeBasis_my_CT","FirstTimeBasis my CT", 300, 0.0,10.0);

  //SP
  TH1D *h1Amplitude_my_SP[anaConst::nChannels];
  TH1D *h1AmplitudeNeg_my_SP[anaConst::nChannels];
  TH1D *h1Charge_my_SP[anaConst::nChannels];
  TH1D *h1Rise_my_SP[anaConst::nChannels];
  TH1D *h1Width_my_SP[anaConst::nChannels];
  TH1D *h1Fall_my_SP[anaConst::nChannels];
  TH1D *h1FirstTimeBasis_my_SP[anaConst::nChannels];

  initH1_F(h1Amplitude_my_SP,"h1Amplitude_my_SP", "Amplitude my SP",1000,-1.5,1.5);
  initH1_F(h1AmplitudeNeg_my_SP,"h1AmplitudeNeg_my_SP","AmplitudeNeg my SP",3000,-1.5,1.5);
  initH1_F(h1Charge_my_SP, "h1Charge_my_SP", "Charge my SP",1000,-10.0,10.0);
  initH1_F(h1Rise_my_SP,"h1Rise_my_SP","Rise my SP",300,0.0,3.0);
  initH1_F(h1Width_my_SP,"h1Width_my_SP","Width my SP",400,0.0,5.0);
  initH1_F(h1Fall_my_SP,"h1Fall_my_SP","h1Fall my SP",300,0.0,10.0);
  initH1_F(h1FirstTimeBasis_my_SP,"h1FirstTimeBasis_my_SP","FirstTimeBasis my SP", 300, 0.0,10.0);

  //nSigChannels =  1 ... 2 ... 3 ... 4 ... 5
  TH1D *h1Amplitude_my_nSig1[anaConst::nChannels];
  TH1D *h1Amplitude_my_nSig2[anaConst::nChannels];
  TH1D *h1Amplitude_my_nSig3[anaConst::nChannels];
  TH1D *h1Amplitude_my_nSig4[anaConst::nChannels];
  TH1D *h1Amplitude_my_nSig5[anaConst::nChannels];
  TH1D *h1Amplitude_my_nSig6[anaConst::nChannels];
  TH1D *h1Charge_my_nSig1[anaConst::nChannels];
  TH1D *h1Charge_my_nSig2[anaConst::nChannels];
  TH1D *h1Charge_my_nSig3[anaConst::nChannels];
  TH1D *h1Charge_my_nSig4[anaConst::nChannels];
  TH1D *h1Charge_my_nSig5[anaConst::nChannels];
  TH1D *h1Charge_my_nSig6[anaConst::nChannels];

  TH1D *h1Amplitude_my_SP_nSig1[anaConst::nChannels];
  TH1D *h1Amplitude_my_SP_nSig2[anaConst::nChannels];
  TH1D *h1Amplitude_my_SP_nSig3[anaConst::nChannels];
  TH1D *h1Amplitude_my_SP_nSig4[anaConst::nChannels];
  TH1D *h1Amplitude_my_SP_nSig5[anaConst::nChannels];
  TH1D *h1Amplitude_my_SP_nSig6[anaConst::nChannels];
  TH1D *h1Charge_my_SP_nSig1[anaConst::nChannels];
  TH1D *h1Charge_my_SP_nSig2[anaConst::nChannels];
  TH1D *h1Charge_my_SP_nSig3[anaConst::nChannels];
  TH1D *h1Charge_my_SP_nSig4[anaConst::nChannels];
  TH1D *h1Charge_my_SP_nSig5[anaConst::nChannels];
  TH1D *h1Charge_my_SP_nSig6[anaConst::nChannels];

  TH1D *h1Amplitude_my_CT_nSig1[anaConst::nChannels];
  TH1D *h1Amplitude_my_CT_nSig2[anaConst::nChannels];
  TH1D *h1Amplitude_my_CT_nSig3[anaConst::nChannels];
  TH1D *h1Amplitude_my_CT_nSig4[anaConst::nChannels];
  TH1D *h1Amplitude_my_CT_nSig5[anaConst::nChannels];
  TH1D *h1Amplitude_my_CT_nSig6[anaConst::nChannels];
  TH1D *h1Charge_my_CT_nSig1[anaConst::nChannels];
  TH1D *h1Charge_my_CT_nSig2[anaConst::nChannels];
  TH1D *h1Charge_my_CT_nSig3[anaConst::nChannels];
  TH1D *h1Charge_my_CT_nSig4[anaConst::nChannels];
  TH1D *h1Charge_my_CT_nSig5[anaConst::nChannels];
  TH1D *h1Charge_my_CT_nSig6[anaConst::nChannels];

  initH1_F(h1Amplitude_my_nSig1,"h1Amplitude_my_nSig1", "Amplitude my nSig1",1000,-1.5,1.5);
  initH1_F(h1Amplitude_my_nSig2,"h1Amplitude_my_nSig2", "Amplitude my nSig2",1000,-1.5,1.5);
  initH1_F(h1Amplitude_my_nSig3,"h1Amplitude_my_nSig3", "Amplitude my nSig3",1000,-1.5,1.5);
  initH1_F(h1Amplitude_my_nSig4,"h1Amplitude_my_nSig4", "Amplitude my nSig4",1000,-1.5,1.5);
  initH1_F(h1Amplitude_my_nSig5,"h1Amplitude_my_nSig5", "Amplitude my nSig5",1000,-1.5,1.5);
  initH1_F(h1Amplitude_my_nSig6,"h1Amplitude_my_nSig6", "Amplitude my nSig6",1000,-1.5,1.5);
  initH1_F(h1Charge_my_nSig1, "h1Charge_my_nSig1", "Charge my nSig1",1000,-10.0,10.0);
  initH1_F(h1Charge_my_nSig2, "h1Charge_my_nSig2", "Charge my nSig2",1000,-10.0,10.0);
  initH1_F(h1Charge_my_nSig3, "h1Charge_my_nSig3", "Charge my nSig3",1000,-10.0,10.0);
  initH1_F(h1Charge_my_nSig4, "h1Charge_my_nSig4", "Charge my nSig4",1000,-10.0,10.0);
  initH1_F(h1Charge_my_nSig5, "h1Charge_my_nSig5", "Charge my nSig5",1000,-10.0,10.0);
  initH1_F(h1Charge_my_nSig6, "h1Charge_my_nSig6", "Charge my nSig6",1000,-10.0,10.0);

  initH1_F(h1Amplitude_my_SP_nSig1,"h1Amplitude_my_SP_nSig1", "Amplitude my SP nSig1",1000,-1.5,1.5);
  initH1_F(h1Amplitude_my_SP_nSig2,"h1Amplitude_my_SP_nSig2", "Amplitude my SP nSig2",1000,-1.5,1.5);
  initH1_F(h1Amplitude_my_SP_nSig3,"h1Amplitude_my_SP_nSig3", "Amplitude my SP nSig3",1000,-1.5,1.5);
  initH1_F(h1Amplitude_my_SP_nSig4,"h1Amplitude_my_SP_nSig4", "Amplitude my SP nSig4",1000,-1.5,1.5);
  initH1_F(h1Amplitude_my_SP_nSig5,"h1Amplitude_my_SP_nSig5", "Amplitude my SP nSig5",1000,-1.5,1.5);
  initH1_F(h1Amplitude_my_SP_nSig6,"h1Amplitude_my_SP_nSig6", "Amplitude my SP nSig6",1000,-1.5,1.5);
  initH1_F(h1Charge_my_SP_nSig1, "h1Charge_my_SP_nSig1", "Charge my SP nSig1",1000,-10.0,10.0);
  initH1_F(h1Charge_my_SP_nSig2, "h1Charge_my_SP_nSig2", "Charge my SP nSig2",1000,-10.0,10.0);
  initH1_F(h1Charge_my_SP_nSig3, "h1Charge_my_SP_nSig3", "Charge my SP nSig3",1000,-10.0,10.0);
  initH1_F(h1Charge_my_SP_nSig4, "h1Charge_my_SP_nSig4", "Charge my SP nSig4",1000,-10.0,10.0);
  initH1_F(h1Charge_my_SP_nSig5, "h1Charge_my_SP_nSig5", "Charge my SP nSig5",1000,-10.0,10.0);
  initH1_F(h1Charge_my_SP_nSig6, "h1Charge_my_SP_nSig6", "Charge my SP nSig6",1000,-10.0,10.0);

  initH1_F(h1Amplitude_my_CT_nSig1,"h1Amplitude_my_CT_nSig1", "Amplitude my CT nSig1",1000,-1.5,1.5);
  initH1_F(h1Amplitude_my_CT_nSig2,"h1Amplitude_my_CT_nSig2", "Amplitude my CT nSig2",1000,-1.5,1.5);
  initH1_F(h1Amplitude_my_CT_nSig3,"h1Amplitude_my_CT_nSig3", "Amplitude my CT nSig3",1000,-1.5,1.5);
  initH1_F(h1Amplitude_my_CT_nSig4,"h1Amplitude_my_CT_nSig4", "Amplitude my CT nSig4",1000,-1.5,1.5);
  initH1_F(h1Amplitude_my_CT_nSig5,"h1Amplitude_my_CT_nSig5", "Amplitude my CT nSig5",1000,-1.5,1.5);
  initH1_F(h1Amplitude_my_CT_nSig6,"h1Amplitude_my_CT_nSig6", "Amplitude my CT nSig6",1000,-1.5,1.5);
  initH1_F(h1Charge_my_CT_nSig1, "h1Charge_my_CT_nSig1", "Charge my CT nSig1",1000,-10.0,10.0);
  initH1_F(h1Charge_my_CT_nSig2, "h1Charge_my_CT_nSig2", "Charge my CT nSig2",1000,-10.0,10.0);
  initH1_F(h1Charge_my_CT_nSig3, "h1Charge_my_CT_nSig3", "Charge my CT nSig3",1000,-10.0,10.0);
  initH1_F(h1Charge_my_CT_nSig4, "h1Charge_my_CT_nSig4", "Charge my CT nSig4",1000,-10.0,10.0);
  initH1_F(h1Charge_my_CT_nSig5, "h1Charge_my_CT_nSig5", "Charge my CT nSig5",1000,-10.0,10.0);
  initH1_F(h1Charge_my_CT_nSig6, "h1Charge_my_CT_nSig6", "Charge my CT nSig6",1000,-10.0,10.0);

  TH1D *h1singleSig = new TH1D( "h1singleSig", "singleSig", 17, 0,17.0);
  TH1D *h1_2Sig = new TH1D( "h1_2Sig", "2Sig", 17, 0,17.0);
  TH1D *h1_3Sig = new TH1D( "h1_3Sig", "3Sig", 17, 0,17.0);
  TH1D *h1_4Sig = new TH1D( "h1_4Sig", "4Sig", 17, 0,17.0);
  TH1D *h1_5Sig = new TH1D( "h1_5Sig", "5Sig", 17, 0,17.0);
  TH1D *h1_6Sig = new TH1D( "h1_6Sig", "6Sig", 17, 0,17.0);


  TH1D *h1nSigChannels = new TH1D( "h1nSigChannels", "nSigChannels", 17, 0,17.0);


  TH1D *h1nSP = new TH1D("h1nSP", "nSP", 16, 0,16.0);
  TH1D *h1nCT = new TH1D("h1nCT", "nCT", 16, 0,16.0);
  
  TH1D *h1Amplitude_my_CT_nSig1_o    = new TH1D("h1Amplitude_my_CT_nSig1_o", "Amplitude my CT nSig1", 1000, -1.5, 1.5);
  TH1D *h1AmplitudeNeg_my_CT_nSig1 = new TH1D("h1AmplitudeNeg_my_CT_nSig1", "AmplitudeNeg my CT nSig1", 3000, -1.5, 1.5);
  TH1D *h1Amplitude_my_CT_nSig2_o    = new TH1D("h1Amplitude_my_CT_nSig2_o", "Amplitude my CT nSig2", 1000, -1.5, 1.5);
  TH1D *h1AmplitudeNeg_my_CT_nSig2 = new TH1D("h1AmplitudeNeg_my_CT_nSig2", "AmplitudeNeg my CT nSig2", 3000, -1.5, 1.5);
  TH1D *h1Amplitude_my_CT_nSig3_o    = new TH1D("h1Amplitude_my_CT_nSig3_o", "Amplitude my CT nSig3", 1000, -1.5, 1.5);
  TH1D *h1AmplitudeNeg_my_CT_nSig3 = new TH1D("h1AmplitudeNeg_my_CT_nSig3", "AmplitudeNeg my CT nSig3", 3000, -1.5, 1.5);
  TH1D *h1Amplitude_my_CT_nSig4_o    = new TH1D("h1Amplitude_my_CT_nSig4_o", "Amplitude my CT nSig4", 1000, -1.5, 1.5);
  TH1D *h1AmplitudeNeg_my_CT_nSig4 = new TH1D("h1AmplitudeNeg_my_CT_nSig4", "AmplitudeNeg my CT nSig4", 3000, -1.5, 1.5);
  TH1D *h1Amplitude_my_CT_nSig5_o    = new TH1D("h1Amplitude_my_CT_nSig5_o", "Amplitude my CT nSig5", 1000, -1.5, 1.5);
  TH1D *h1AmplitudeNeg_my_CT_nSig5 = new TH1D("h1AmplitudeNeg_my_CT_nSig5", "AmplitudeNeg my CT nSig5", 3000, -1.5, 1.5);

  ////////////////////////////////

  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  cout<<"nentries "<<nentries<<endl;
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    //if(jentry == 0)
    //printf(" UnixTime_usb  = %20.5f \n", UnixTime_usb);

    if(jentry%50000==0)
      cout<<" processed events "<<jentry<<endl;    

    nSigChannels = 0;    
    nSP = 0;
    nCT = 0;
    for(i=0;i<anaConst::nChannels;i++)
      sigChID[i] = 0;
    
    for(i=0;i<anaConst::nChannels;i++){
      h1Time_my[i]->Fill(firstTimePosCFD_usb[i]);
      h1firstTimeNegCFD_my[i]->Fill(firstTimeNegCFD_usb[i]);
      h1Rise_my[i]->Fill(FirstRiseTimePos_usb[i]);
      h1Width_my[i]->Fill(FirstWidthTimePos_usb[i]);
      h1Fall_my[i]->Fill(FirstFallTimePos_usb[i]);
      h1FirstTimeBasis_my[i]->Fill(FirstTimeBasis_usb[i]);
      h1Charge_my[i]->Fill(chargeTOT_usb[i]);
      h1Amplitude_my[i]->Fill(FirstAmplitudePos_usb[i]);
      h1AmplMax_my[i]->Fill(TotAmplitudePos_usb[i]);
      h1AmplitudeNeg_my[i]->Fill(FirstAmplitudeNeg_usb[i]);
      h1nOfPosInterAtLevel_my[i]->Fill(nOfPosInterAtLevel_usb[i]);
      h1nOfNegInterAtLevel_my[i]->Fill(nOfNegInterAtLevel_usb[i]);
      h1wfID_my[i]->Fill(wfID_usb[i]);

      if(wfID_usb[i] == 0){
	h1Amplitude_my_CT[i]->Fill(FirstAmplitudePos_usb[i]);
	h1AmplitudeNeg_my_CT[i]->Fill(FirstAmplitudeNeg_usb[i]);
	h1Charge_my_CT[i]->Fill(chargeTOT_usb[i]);
	h1Rise_my_CT[i]->Fill(FirstRiseTimePos_usb[i]);
	h1Width_my_CT[i]->Fill(FirstWidthTimePos_usb[i]);
	h1Fall_my_CT[i]->Fill(FirstFallTimePos_usb[i]);
	h1FirstTimeBasis_my_CT[i]->Fill(FirstTimeBasis_usb[i]);
	if(i!=15)
	  nCT++;
      }

      if(wfID_usb[i] == 1 && FirstAmplitudePos_usb[i] > 0.06){
	h1Amplitude_my_SP[i]->Fill(FirstAmplitudePos_usb[i]);
	h1AmplitudeNeg_my_SP[i]->Fill(FirstAmplitudeNeg_usb[i]);
	h1Charge_my_SP[i]->Fill(chargeTOT_usb[i]);
	h1Rise_my_SP[i]->Fill(FirstRiseTimePos_usb[i]);
	h1Width_my_SP[i]->Fill(FirstWidthTimePos_usb[i]);
	h1Fall_my_SP[i]->Fill(FirstFallTimePos_usb[i]);
	h1FirstTimeBasis_my_SP[i]->Fill(FirstTimeBasis_usb[i]);
	if(i!=15)
	  nSP++;
      }

      //if(chargeTOT_usb[i]>0.4 && i!=15 ){
      if(FirstAmplitudePos_usb[i]>0.09 && i!=15 ){
	//if(TotAmplitudePos_usb[i]>0.1 && i!=15 && wfID_usb[i] == 1){
	//if(TotAmplitudePos_usb[i]>0.1 && i!=15 ){
	//if(TotAmplitudePos_usb[i]>0.06 && i!=15){
	nSigChannels++;
	sigChID[i] = 1;
      }
      
    }

    if(nSigChannels == 1){
      for(i = 0;i<15;i++){
	if(sigChID[i] == 1){
	  h1singleSig->Fill(i);
	  h1Amplitude_my_nSig1[i]->Fill(FirstAmplitudePos_usb[i]);
	  //h1Amplitude_my_nSig1[i]->Fill(TotAmplitudePos_usb[i]);
	  h1Charge_my_nSig1[i]->Fill(chargeTOT_usb[i]);
	}
	if(wfID_usb[i]==1){
	  h1Amplitude_my_SP_nSig1[i]->Fill(FirstAmplitudePos_usb[i]);
	  h1Charge_my_SP_nSig1[i]->Fill(chargeTOT_usb[i]);
	}
	if(wfID_usb[i]==0){
	  h1Amplitude_my_CT_nSig1[i]->Fill(FirstAmplitudePos_usb[i]);
	  h1Charge_my_CT_nSig1[i]->Fill(chargeTOT_usb[i]);
	}
      }
    }

    if(nSigChannels == 2){
      for(i = 0;i<15;i++){
	if(sigChID[i] == 1){
	  h1_2Sig->Fill(i);
	  h1Amplitude_my_nSig2[i]->Fill(FirstAmplitudePos_usb[i]);
	  //h1Amplitude_my_nSig2[i]->Fill(TotAmplitudePos_usb[i]);
	  h1Charge_my_nSig2[i]->Fill(chargeTOT_usb[i]);
	}
	if(wfID_usb[i]==1){
	  h1Amplitude_my_SP_nSig2[i]->Fill(FirstAmplitudePos_usb[i]);
	  h1Charge_my_SP_nSig2[i]->Fill(chargeTOT_usb[i]);
	}
	if(wfID_usb[i]==0){
	  h1Amplitude_my_CT_nSig2[i]->Fill(FirstAmplitudePos_usb[i]);
	  h1Charge_my_CT_nSig2[i]->Fill(chargeTOT_usb[i]);
	}
      }
    }

    if(nSigChannels == 3){
      for(i = 0;i<15;i++){
	if(sigChID[i] == 1){
	  h1_3Sig->Fill(i);
	  h1Amplitude_my_nSig3[i]->Fill(FirstAmplitudePos_usb[i]);
	  h1Charge_my_nSig3[i]->Fill(chargeTOT_usb[i]);
	}
	if(wfID_usb[i]==1){
	  h1Amplitude_my_SP_nSig3[i]->Fill(FirstAmplitudePos_usb[i]);
	  h1Charge_my_SP_nSig3[i]->Fill(chargeTOT_usb[i]);
	}
	if(wfID_usb[i]==0){
	  h1Amplitude_my_CT_nSig3[i]->Fill(FirstAmplitudePos_usb[i]);
	  h1Charge_my_CT_nSig3[i]->Fill(chargeTOT_usb[i]);
	}
      }
    }

    if(nSigChannels == 4){
      for(i = 0;i<15;i++){
	if(sigChID[i] == 1){
	  h1_4Sig->Fill(i);
	  h1Amplitude_my_nSig4[i]->Fill(FirstAmplitudePos_usb[i]);
	  h1Charge_my_nSig4[i]->Fill(chargeTOT_usb[i]);
	}
	if(wfID_usb[i]==1){
	  h1Amplitude_my_SP_nSig4[i]->Fill(FirstAmplitudePos_usb[i]);
	  h1Charge_my_SP_nSig4[i]->Fill(chargeTOT_usb[i]);
	}
	if(wfID_usb[i]==0){
	  h1Amplitude_my_CT_nSig4[i]->Fill(FirstAmplitudePos_usb[i]);
	  h1Charge_my_CT_nSig4[i]->Fill(chargeTOT_usb[i]);
	}
      }
    }

    if(nSigChannels == 5){
      for(i = 0;i<15;i++){
	if(sigChID[i] == 1){
	  h1_5Sig->Fill(i);
	  h1Amplitude_my_nSig5[i]->Fill(FirstAmplitudePos_usb[i]);
	  h1Charge_my_nSig5[i]->Fill(chargeTOT_usb[i]);
	}
	if(wfID_usb[i]==1){
	  h1Amplitude_my_SP_nSig5[i]->Fill(FirstAmplitudePos_usb[i]);
	  h1Charge_my_SP_nSig5[i]->Fill(chargeTOT_usb[i]);
	}
	if(wfID_usb[i]==0){
	  h1Amplitude_my_CT_nSig5[i]->Fill(FirstAmplitudePos_usb[i]);
	  h1Charge_my_CT_nSig5[i]->Fill(chargeTOT_usb[i]);
	}
      }
    }

    if(nSigChannels == 6){
      for(i = 0;i<15;i++){
	if(sigChID[i] == 1){
	  h1_6Sig->Fill(i);
	  h1Amplitude_my_nSig6[i]->Fill(FirstAmplitudePos_usb[i]);
	  h1Charge_my_nSig6[i]->Fill(chargeTOT_usb[i]);
	}
	if(wfID_usb[i]==1){
	  h1Amplitude_my_SP_nSig6[i]->Fill(FirstAmplitudePos_usb[i]);
	  h1Charge_my_SP_nSig6[i]->Fill(chargeTOT_usb[i]);
	}
	if(wfID_usb[i]==0){
	  h1Amplitude_my_CT_nSig6[i]->Fill(FirstAmplitudePos_usb[i]);
	  h1Charge_my_CT_nSig6[i]->Fill(chargeTOT_usb[i]);
	}
      }
    }


    //////// CT as function of the number of p.e. ////////
    if(nSigChannels == 1){
      for(i=0;i<(anaConst::nChannels-1);i++){
	if(sigChID[i] != 1 && wfID_usb[i] == 0){
	  h1Amplitude_my_CT_nSig1_o->Fill(TotAmplitudePos_usb[i], 1.0/14549.0);
	  h1AmplitudeNeg_my_CT_nSig1->Fill(FirstAmplitudeNeg_usb[i], 1.0/14549.0);
	  nSig1Cut++;
	}
      }
    }
    
    if(nSigChannels == 2){
      for(i=0;i<(anaConst::nChannels-1);i++){
	if(sigChID[i] != 1 && wfID_usb[i] == 0){
	  h1Amplitude_my_CT_nSig2_o->Fill(TotAmplitudePos_usb[i], 1.0/46497.0);
	  h1AmplitudeNeg_my_CT_nSig2->Fill(FirstAmplitudeNeg_usb[i], 1.0/46497.0);
	  nSig2Cut++;
	}
      }
    }
    
    if(nSigChannels == 3){
      for(i=0;i<(anaConst::nChannels-1);i++){
	if(sigChID[i] != 1 && wfID_usb[i] == 0){
	  h1Amplitude_my_CT_nSig3_o->Fill(TotAmplitudePos_usb[i], 1.0/95374.0);
	  h1AmplitudeNeg_my_CT_nSig3->Fill(FirstAmplitudeNeg_usb[i], 1.0/95374.0);
	  nSig3Cut++;
	}
      }
    }

    if(nSigChannels == 4){
      for(i=0;i<(anaConst::nChannels-1);i++){
	if(sigChID[i] != 1 && wfID_usb[i] == 0){
	  h1Amplitude_my_CT_nSig4_o->Fill(TotAmplitudePos_usb[i], 1.0/138231.0);
	  h1AmplitudeNeg_my_CT_nSig4->Fill(FirstAmplitudeNeg_usb[i], 1.0/138231.0);
	  nSig4Cut++;
	}
      }
    }

    if(nSigChannels == 5){
      for(i=0;i<(anaConst::nChannels-1);i++){
	if(sigChID[i] != 1 && wfID_usb[i] == 0){
	  h1Amplitude_my_CT_nSig5_o->Fill(TotAmplitudePos_usb[i], 1.0/146822.0);
	  h1AmplitudeNeg_my_CT_nSig5->Fill(FirstAmplitudeNeg_usb[i], 1.0/146822.0);
	  nSig5Cut++;
	}
      }
    }
    //////////////////////////////////////////////////////

    //if(nSigChannels == 1){
    //cout<<jentry<<endl;
    //assert(0);
    //}

    h1nSigChannels->Fill(nSigChannels);
    h1nSP->Fill(nSP);
    h1nCT->Fill(nCT);

  }
  
  TFile* rootFile = new TFile(histN.Data(), "RECREATE", " Histograms from USBWC merged with CRT", 1);
  rootFile->cd();
  if (rootFile->IsZombie()){
    cout<<"  ERROR ---> file "<<histN<<" is zombi"<<endl;
    assert(0);
  }
  else
    cout<<"  Output Histos file ---> "<<histN<<endl;

  for(i=0;i<anaConst::nChannels;i++){
    h1Sep[i]->Write();
    h1Time_my[i]->Write();
    h1firstTimeNegCFD_my[i]->Write();
    h1Rise_my[i]->Write();
    h1Width_my[i]->Write();
    h1Fall_my[i]->Write();
    h1FirstTimeBasis_my[i]->Write();
    h1Charge_my[i]->Write();
    h1Amplitude_my[i]->Write();
    h1AmplMax_my[i]->Write();
    h1AmplitudeNeg_my[i]->Write();
    h1nOfPosInterAtLevel_my[i]->Write();
    h1nOfNegInterAtLevel_my[i]->Write();
    h1wfID_my[i]->Write();

    h1Amplitude_my_CT[i]->Write();
    h1AmplitudeNeg_my_CT[i]->Write();
    h1Charge_my_CT[i]->Write();
    h1Rise_my_CT[i]->Write();
    h1Width_my_CT[i]->Write();
    h1Fall_my_CT[i]->Write();
    h1FirstTimeBasis_my_CT[i]->Write();

    h1Amplitude_my_SP[i]->Write();
    h1AmplitudeNeg_my_SP[i]->Write();
    h1Charge_my_SP[i]->Write();
    h1Rise_my_SP[i]->Write();
    h1Width_my_SP[i]->Write();
    h1Fall_my_SP[i]->Write();
    h1FirstTimeBasis_my_SP[i]->Write();

    h1Amplitude_my_nSig1[i]->Write();
    h1Charge_my_nSig1[i]->Write();
    h1Amplitude_my_nSig2[i]->Write();
    h1Charge_my_nSig2[i]->Write();
    h1Amplitude_my_nSig3[i]->Write();
    h1Charge_my_nSig3[i]->Write();
    h1Amplitude_my_nSig4[i]->Write();
    h1Charge_my_nSig4[i]->Write();
    h1Amplitude_my_nSig5[i]->Write();
    h1Charge_my_nSig5[i]->Write();
    h1Amplitude_my_nSig6[i]->Write();
    h1Charge_my_nSig6[i]->Write();

    h1Amplitude_my_SP_nSig1[i]->Write();
    h1Charge_my_SP_nSig1[i]->Write();
    h1Amplitude_my_CT_nSig1[i]->Write();
    h1Charge_my_CT_nSig1[i]->Write();
    h1Amplitude_my_SP_nSig2[i]->Write();
    h1Charge_my_SP_nSig2[i]->Write();
    h1Amplitude_my_CT_nSig2[i]->Write();
    h1Charge_my_CT_nSig2[i]->Write();
    h1Amplitude_my_SP_nSig3[i]->Write();
    h1Charge_my_SP_nSig3[i]->Write();
    h1Amplitude_my_CT_nSig3[i]->Write();
    h1Charge_my_CT_nSig3[i]->Write();
    h1Amplitude_my_SP_nSig4[i]->Write();
    h1Charge_my_SP_nSig4[i]->Write();
    h1Amplitude_my_CT_nSig4[i]->Write();
    h1Charge_my_CT_nSig4[i]->Write();
    h1Amplitude_my_SP_nSig5[i]->Write();
    h1Charge_my_SP_nSig5[i]->Write();
    h1Amplitude_my_CT_nSig5[i]->Write();
    h1Charge_my_CT_nSig5[i]->Write();
    h1Amplitude_my_SP_nSig6[i]->Write();
    h1Charge_my_SP_nSig6[i]->Write();
    h1Amplitude_my_CT_nSig6[i]->Write();
    h1Charge_my_CT_nSig6[i]->Write();
  }    
 
  h1nSigChannels->Write();
  h1nSP->Write();
  h1nCT->Write();

  h1Amplitude_my_CT_nSig1_o->Write();
  h1AmplitudeNeg_my_CT_nSig1->Write();
  h1Amplitude_my_CT_nSig2_o->Write();
  h1AmplitudeNeg_my_CT_nSig2->Write();
  h1Amplitude_my_CT_nSig3_o->Write();
  h1AmplitudeNeg_my_CT_nSig3->Write();
  h1Amplitude_my_CT_nSig4_o->Write();
  h1AmplitudeNeg_my_CT_nSig4->Write();
  h1Amplitude_my_CT_nSig5_o->Write();
  h1AmplitudeNeg_my_CT_nSig5->Write();

  h1singleSig->Write();
  h1_2Sig->Write();
  h1_3Sig->Write();
  h1_4Sig->Write();
  h1_5Sig->Write();
  h1_6Sig->Write();

  cout<<"nSig1Cut "<<nSig1Cut<<endl
      <<"nSig2Cut "<<nSig2Cut<<endl
      <<"nSig3Cut "<<nSig3Cut<<endl
      <<"nSig4Cut "<<nSig4Cut<<endl
      <<"nSig5Cut "<<nSig5Cut<<endl;
  

}

Bool_t anaData::cutsPerTrk(){
  if(hodoStatus_crt){
    if(x_boxQuartzS_crt>-9.0 && x_boxQuartzS_crt<13){
    if(y_boxQuartzS_crt>-2.0 && y_boxQuartzS_crt<3){
    if(x_boxfTOF_crt>-14.0 && x_boxfTOF_crt<15.0){
    if(y_boxfTOF_crt>-2.0 && y_boxfTOF_crt<3.5){
      //if(x_boxQuartzS_crt>-8.6 && x_boxQuartzS_crt<12.4){
      //if(y_boxQuartzS_crt>-1.6 && y_boxQuartzS_crt<2.59){
      //if(x_boxfTOF_crt>-14.0 && x_boxfTOF_crt<15.0){
      //if(y_boxfTOF_crt>-1.5 && y_boxfTOF_crt<3.0){
	    // if(x_boxfTOF_crt>5.0){
	    // //if(trkPhi_crt>-100.0 && trkPhi_crt<100.0){
	    // //if(trkTheta_crt<165){
	     return true;
	    // //}
	    // //}
	    // }
	  }
	}
      }
    }
  }
  //if(UnixTime_crt > 1000){
  //return true;
  //}
  return false;
  //return true;
}

Bool_t anaData::cutsPerChannel(Int_t kCh1, Int_t kCh2){
  if(kCh1<0 || kCh2<0){
    cout<<" ERROR ---> kCh1<0 || kCh2<0"<<endl;
    assert(0);
  }
  if(kCh1>=anaConst::nChannels || kCh2>=anaConst::nChannels){
    cout<<" ERROR ---> kCh1>=anaConst::nChannels || kCh2>=anaConst::nChannels"<<endl;
    assert(0);
  }
  if(wfID_usb[kCh1]==1 && wfID_usb[kCh2]==1){
    if(firstTimePosCFD_usb[kCh1]>0.0 && firstTimePosCFD_usb[kCh2]>0.0){
      if(FirstAmplitudePos_usb[kCh1]>anaConst::sigThreshold[kCh1] && FirstAmplitudePos_usb[kCh2]>anaConst::sigThreshold[kCh2]){
	//if(FirstRiseTimePos_usb[kCh1]>0.95 && FirstRiseTimePos_usb[kCh1]<1.15 ){
	//if(FirstRiseTimePos_usb[kCh2]>0.95 && FirstRiseTimePos_usb[kCh2]<1.15 ){
	if(_nPeTotPerEv_AmpCut<6){
	  if(cutsPerTrk()){
	    return true;
	  }
	}
	//}
	//}
      }
    }
  }
  return false;
}

void anaData::initH1_F(TH1D *h[anaConst::nChannels],
		       TString hName , TString hTitle,
		       Int_t nBin, Double_t binMin, 
		       Double_t binMax){
  Int_t i = 0;
  TString hNameM;
  TString hTitleM;
  for(i = 0;i<anaConst::nChannels;i++){
    hNameM = hName;
    hTitleM = hTitle;
    hNameM += "_ch";
    hNameM += i;
    hTitleM += " ch";
    hTitleM += i;
    h[i] = new TH1D(hNameM.Data(),hTitleM.Data(), nBin, binMin, binMax);
  }
}

void anaData::Init(TTree *tree){
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("eventID_crt", &eventID_crt, &b_eventID_crt);
   fChain->SetBranchAddress("UnixTime_crt", &UnixTime_crt, &b_UnixTime_crt);
   fChain->SetBranchAddress("x1_crt", &x1_crt, &b_x1_crt);
   fChain->SetBranchAddress("y1_crt", &y1_crt, &b_y1_crt);
   fChain->SetBranchAddress("x2_crt", &x2_crt, &b_x2_crt);
   fChain->SetBranchAddress("y2_crt", &y2_crt, &b_y2_crt);
   fChain->SetBranchAddress("hodoStatus_crt", &hodoStatus_crt, &b_hodoStatus_crt);
   fChain->SetBranchAddress("trkTheta_crt", &trkTheta_crt, &b_trkTheta_crt);
   fChain->SetBranchAddress("trkPhi_crt", &trkPhi_crt, &b_trkPhi_crt);
   fChain->SetBranchAddress("trkMomID_crt", &trkMomID_crt, &b_trkMomID_crt);
   fChain->SetBranchAddress("x_boxfTOF_crt", &x_boxfTOF_crt, &b_x_boxfTOF_crt);
   fChain->SetBranchAddress("y_boxfTOF_crt", &y_boxfTOF_crt, &b_y_boxfTOF_crt);
   fChain->SetBranchAddress("z_boxfTOF_crt", &z_boxfTOF_crt, &b_z_boxfTOF_crt);
   fChain->SetBranchAddress("x_boxfTOF_Bot_crt", &x_boxfTOF_Bot_crt, &b_x_boxfTOF_Bot_crt);
   fChain->SetBranchAddress("y_boxfTOF_Bot_crt", &y_boxfTOF_Bot_crt, &b_y_boxfTOF_Bot_crt);
   fChain->SetBranchAddress("z_boxfTOF_Bot_crt", &z_boxfTOF_Bot_crt, &b_z_boxfTOF_Bot_crt);
   fChain->SetBranchAddress("x_boxQuartzS_crt", &x_boxQuartzS_crt, &b_x_boxQuartzS_crt);
   fChain->SetBranchAddress("y_boxQuartzS_crt", &y_boxQuartzS_crt, &b_y_boxQuartzS_crt);
   fChain->SetBranchAddress("z_boxQuartzS_crt", &z_boxQuartzS_crt, &b_z_boxQuartzS_crt);
   fChain->SetBranchAddress("eventID_usb", &eventID_usb, &b_eventID_usb);
   fChain->SetBranchAddress("UnixTime_usb", &UnixTime_usb, &b_UnixTime_usb);
   fChain->SetBranchAddress("merged_eventID", &merged_eventID, &b_merged_eventID);
   fChain->SetBranchAddress("Charge_usb", Charge_usb, &b_Charge_usb);
   fChain->SetBranchAddress("Time_usb", Time_usb, &b_Time_usb);
   fChain->SetBranchAddress("Amplitude_usb", Amplitude_usb, &b_Amplitude_usb);
   fChain->SetBranchAddress("wfID_usb", wfID_usb, &b_wfID_usb);
   fChain->SetBranchAddress("MaxBaseLineAmpl_usb", MaxBaseLineAmpl_usb, &b_MaxBaseLineAmpl_usb);
   fChain->SetBranchAddress("MinBaseLineAmpl_usb", MinBaseLineAmpl_usb, &b_MinBaseLineAmpl_usb);
   fChain->SetBranchAddress("FirstAmplitudePos_usb", FirstAmplitudePos_usb, &b_FirstAmplitudePos_usb);
   fChain->SetBranchAddress("FirstAmplitudeNeg_usb", FirstAmplitudeNeg_usb, &b_FirstAmplitudeNeg_usb);
   fChain->SetBranchAddress("chargeTOT_usb", chargeTOT_usb, &b_chargeTOT_usb);
   fChain->SetBranchAddress("chargeTOT_p_usb", chargeTOT_p_usb, &b_chargeTOT_p_usb);
   fChain->SetBranchAddress("chargeTOT_m_usb", chargeTOT_m_usb, &b_chargeTOT_m_usb);
   fChain->SetBranchAddress("FirstRiseTimePos_usb", FirstRiseTimePos_usb, &b_FirstRiseTimePos_usb);
   fChain->SetBranchAddress("FirstFallTimePos_usb", FirstFallTimePos_usb, &b_FirstFallTimePos_usb);
   fChain->SetBranchAddress("TotAmplitudePos_usb", TotAmplitudePos_usb, &b_TotAmplitudePos_usb);
   fChain->SetBranchAddress("TotTimeAmplitudePos_usb", TotTimeAmplitudePos_usb, &b_TotTimeAmplitudePos_usb);
   fChain->SetBranchAddress("FirstWidthTimePos_usb", FirstWidthTimePos_usb, &b_FirstWidthTimePos_usb);
   fChain->SetBranchAddress("FirstTimeBasis_usb", FirstTimeBasis_usb, &b_FirstTimeBasis_usb);
   fChain->SetBranchAddress("dTimeFirtsAmplPosChangLeftDer_usb", dTimeFirtsAmplPosChangLeftDer_usb, &b_dTimeFirtsAmplPosChangLeftDer_usb);
   fChain->SetBranchAddress("dTimeFirtsAmplPosFirstTimeAmplNeg_usb", dTimeFirtsAmplPosFirstTimeAmplNeg_usb, &b_dTimeFirtsAmplPosFirstTimeAmplNeg_usb);
   fChain->SetBranchAddress("MaxAmplBeforeFirstChangDerAmpl_usb", MaxAmplBeforeFirstChangDerAmpl_usb, &b_MaxAmplBeforeFirstChangDerAmpl_usb);
   fChain->SetBranchAddress("MinAmplBeforeFirstChangDerAmpl_usb", MinAmplBeforeFirstChangDerAmpl_usb, &b_MinAmplBeforeFirstChangDerAmpl_usb);
   fChain->SetBranchAddress("firstTimePosCFD_usb", firstTimePosCFD_usb, &b_firstTimePosCFD_usb);
   fChain->SetBranchAddress("firstTimeNegCFD_usb", firstTimeNegCFD_usb, &b_firstTimeNegCFD_usb);
   fChain->SetBranchAddress("nOfPosInterAtLevel_usb", nOfPosInterAtLevel_usb, &b_nOfPosInterAtLevel_usb);
   fChain->SetBranchAddress("nOfNegInterAtLevel_usb", nOfNegInterAtLevel_usb, &b_nOfNegInterAtLevel_usb);
   fChain->SetBranchAddress("SignalThreshold_usb", &SignalThreshold_usb, &b_SignalThreshold_usb);
   fChain->SetBranchAddress("crossTalkThreshold_usb", &crossTalkThreshold_usb, &b_crossTalkThreshold_usb);
   fChain->SetBranchAddress("mpFraction_usb", &mpFraction_usb, &b_mpFraction_usb);
   fChain->SetBranchAddress("valCFD_usb", &valCFD_usb, &b_valCFD_usb);
   fChain->SetBranchAddress("nSplinePoints_usb", &nSplinePoints_usb, &b_nSplinePoints_usb);
   fChain->SetBranchAddress("nPointBaseLine_usb", &nPointBaseLine_usb, &b_nPointBaseLine_usb);
   fChain->SetBranchAddress("levelch15_usb", &levelch15_usb, &b_levelch15_usb);
   fChain->SetBranchAddress("timeAtLevelch15_usb", &timeAtLevelch15_usb, &b_timeAtLevelch15_usb);
   Notify();
}


Int_t anaData::GetEntry(Long64_t entry){
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

Long64_t anaData::LoadTree(Long64_t entry){
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}


Bool_t anaData::Notify(){
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.
   return kTRUE;
}

void anaData::Show(Long64_t entry){
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

Int_t anaData::Cut(Long64_t entry){
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

