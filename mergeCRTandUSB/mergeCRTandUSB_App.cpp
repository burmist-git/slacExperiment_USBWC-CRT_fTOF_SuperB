//My
#include "src/usbwcFileStr.hh"
#include "src/crtFileStr.hh"
#include "src/crtBox.hh"
#include "../wfSimulation/src/waveform.hh"
#include "../anaConst.hh"

//root
#include "TH1D.h"
#include "TH2D.h"

//C, C++
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <assert.h>
#include <stdlib.h>

using namespace std;

Bool_t timingCRT(TH2D *h2, Double_t uTusb, Double_t uTcrt);

const int N = 1000000;       //maximum number of events for USB and CRT
const Int_t ndTimeBin = 80;  //number of dT (Y) bin in the h2MindTvsTime
const Double_t dTmin = -1.0;
const Double_t dTmax = 1.0;
const Double_t maxdTcut = 0.2;     //maximum time difference btween  
//const Double_t nSecBin = 480.0;  //number of seconds within one bin for timing histogram 
const Double_t nSecBin = 4*3600.0; //number of seconds within one bin for timing histogram 

int main(int argc, char* argv[]){
  
  cout<<"Maximum number of events for USB and CRT --> "<<N<<endl
      <<"Number of dT bin in the h2MindTvsTime    --> "<<ndTimeBin<<endl
      <<"dTmin                                    --> "<<dTmin<<endl
      <<"dTmax                                    --> "<<dTmax<<endl
      <<"Maximum time difference btween           --> "<<maxdTcut<<endl
      <<"Number of seconds within one bin for timing histogram -> "<<nSecBin<<endl;  
  
  //LB 04.02.2011
  //  This piece of the code was performing a merging of the 
  //data from USBWC and CRT. Within merged file information 
  //about all waveform was written (USBWC) and x1,x2, y1, y2 (CRT). 
  //Just in case if event was merged.
  //  Now new type of the date merging proposed, we store chosen 
  //information about wf and information about tracking done 
  //with CRT. In case events are not merged we still fill 
  //the root file, but with dummy  information from CRT side. 
  //If merged_eventID variable more than -1 means that two 
  //DAQ have been merged.
 
  /*
  if(argc == 6 && atoi(argv[1]) == 0){

    /////////////// MERGING ///////////////

    Int_t modeID = atoi(argv[1]);
    TString rootFileListUSBstr = argv[2];
    TString rootFileListCRTstr = argv[3];
    TString rootFileWithTiming = argv[4];
    TString outputMergRootFile = argv[5];
    
    cout<<endl
	<<"           --> merging <--       "<<endl
	<<" modeID                         --> "<<modeID<<endl
	<<" root file list with USB data   --> "<<rootFileListUSBstr<<endl
	<<" root file list with CRT data   --> "<<rootFileListCRTstr<<endl
	<<" root file with 2-D timing hist --> "<<rootFileWithTiming<<endl
	<<" output root file with data from CRT and USB --> "<<outputMergRootFile<<endl;
    
    TFile *ff = new TFile(rootFileWithTiming.Data());  
    TH2D *_h2MindTvsTime  = (TH2D*)ff->Get("h2MindTvsTime");
    
    //////////////////////////////
    int nEvCRT;//number of events in CRT
    int nEvUSB;//number of event in CRT
    Double_t *unixTimeCRT;
    unixTimeCRT = new Double_t[N];
    Double_t *unixTimeUSB;
    unixTimeUSB = new Double_t[N];
    //////////////////////////////

    // Arrays to monitor how many times a given event
    // is used by the merging procedure
    Int_t eventUsageCRT[ N ];
    Int_t eventUsageUSB[ N ];
        
    Int_t i = 0;
    Int_t j = 0;
    Int_t k = 0;
    Int_t l = 0;
    Double_t tMin = -999.0;
    Double_t tMax = -999.0;
    
    //init root files with USB data
    usbwcFileStr *usb = new usbwcFileStr(); 
    usb->addFilesToChain(rootFileListUSBstr.Data());
    usb->GetDataRate();
    
    //init root files with CRT data    
    crtFileStr   *crt = new crtFileStr(); 
    crt->addFilesToChain(rootFileListCRTstr.Data());
    crt->GetDataRate();

    usb->GetUnixTime( unixTimeUSB, N, nEvUSB);  
    crt->GetUnixTime( unixTimeCRT, N, nEvCRT);
    //printf("%20.5f \n",unixTimeCRT[nEvCRT-1]);
    
    //Build some useful histograms///////////
    tMin = unixTimeCRT[0];
    if(tMin>unixTimeUSB[0])
      tMin = unixTimeUSB[0];
    tMax = unixTimeCRT[nEvCRT-1];
    if(tMax<unixTimeUSB[nEvUSB-1])
      tMax = unixTimeUSB[nEvUSB-1];
    
    Double_t tMinMy = 0.0;
    Double_t tMaxMy = tMax - tMin;
    Int_t nUnixTimeBin = (Int_t)((tMax - tMin)/3600.0);

    TH1D *h1TimeUSB = new TH1D("h1TimeUSB"," Time USB ", nUnixTimeBin, tMinMy, tMaxMy); //USB event time distribution
    TH1D *h1TimeCRT = new TH1D("h1TimeCRT"," Time CRT ", nUnixTimeBin, tMinMy, tMaxMy); //CRT event time distribution
    for(i = 0;i<nEvUSB;i++){
      h1TimeUSB->Fill(unixTimeUSB[i] - tMin);
    }
    for(i = 0;i<nEvCRT;i++){
      h1TimeCRT->Fill(unixTimeCRT[i] - tMin);
    }
    h1TimeUSB->SetLineColor(kRed);  
    h1TimeUSB->SetLineWidth(2);
    h1TimeUSB->SetMinimum(0.0);
    h1TimeCRT->SetLineWidth(2);
    h1TimeCRT->SetMinimum(0.0);
    //////////////////////////////////////////

    for(i = 0; i < N ; i ++ ) {
      eventUsageCRT[ i ] = 0;
      eventUsageUSB[ i ] = 0;
    }    

    ////////////// CRT ////////////
    Int_t    eventID_crt;
    Double_t UnixTime_crt;
    Double_t x1_crt;
    Double_t y1_crt;
    Double_t x2_crt;
    Double_t y2_crt;
    
    ////////////// USB //////////
    Int_t    eventID_usb;
    Float_t  SamplingPeriod_usb;
    Double_t UnixTime_usb;
    Float_t  ch_usb[16][256];
    //Float_t  Charge_usb[16];
    //Float_t  Time_usb[16];
    //Float_t  Amplitude_usb[16];


    /////////////merged output file/////////
    TFile *hfile = new TFile(outputMergRootFile.Data(), "RECREATE", "data from CRT and USBWC", 1);
    if (hfile->IsZombie()) assert(0);
    TTree *tree = new TTree("T", "CRT and USBWC Data Tree");
    hfile->SetCompressionLevel(2);
    tree->SetAutoSave(1000000);
    TTree::SetBranchStyle(0);

    //CRT//
    tree->Branch("eventID_crt", &eventID_crt, "eventID_crt/I");
    tree->Branch("UnixTime_crt", &UnixTime_crt, "UnixTime_crt/D");
    tree->Branch("x1_crt", &x1_crt, "x1_crt/D");
    tree->Branch("y1_crt", &y1_crt, "y1_crt/D");
    tree->Branch("x2_crt", &x2_crt, "x2_crt/D");
    tree->Branch("y2_crt", &y2_crt, "y2_crt/D");
    
    //USB//
    tree->Branch("eventID_usb",&eventID_usb, "eventID_usb/I");
    tree->Branch("SamplingPeriod_usb", &SamplingPeriod_usb,  "SamplingPeriod_usb/F");
    tree->Branch("UnixTime_usb",&UnixTime_usb, "UnixTime_usb/D");
    tree->Branch("ch_usb", ch_usb, "ch_usb[16][256]/F");
    //tree->Branch("Charge_usb", Charge_usb, "Charge_usb[16]/F");
    //tree->Branch("Time_usb", Time_usb, "Time_usb[16]/F");
    //tree->Branch("Amplitude_usb", Amplitude_usb, "Amplitude_usb[16]/F");
    ////////////////////////////////////////////////
    
    Int_t nRunTimeBin = nUnixTimeBin/4; //number of run time (X) bin in the h2MindTvsTime

    // min time diff between CTR and USB
    TH1D *h1MindT = new TH1D("h1MindT","Min dT",1000,-20.0,20.0);  
    // min time diff between CTR and USB VS time
    TH2D *h2MindTvsTime = new TH2D("h2MindTvsTime","Min dT vs time",
				   nRunTimeBin, unixTimeUSB[0], unixTimeUSB[nEvUSB-1],ndTimeBin,dTmin,dTmax);

    Double_t mindT = 1000000.0;
    Int_t jentryCRTMin = 0;

    cout<<"---Merging of the data---"<<endl;

    Int_t nMergedEvents = 0;

    for(i = 0;i<nEvUSB;i++){

      for(j = jentryCRTMin;j<nEvCRT;j++){
	if(TMath::Abs(unixTimeUSB[i] - unixTimeCRT[j])<mindT){
	  mindT = TMath::Abs(unixTimeUSB[i] - unixTimeCRT[j]);
	  jentryCRTMin = j;
	}
	if((unixTimeCRT[j] - unixTimeUSB[i])>10.0)
	  j = nEvCRT - 1;
      }
      mindT = 10000000.0;      

      if( timingCRT(_h2MindTvsTime,unixTimeUSB[i], unixTimeCRT[jentryCRTMin]) ){

	////////////////merg DAQ here/////////////////////

	//CRT
	crt->LoadTree(jentryCRTMin);
	crt->GetEntry(jentryCRTMin);
	crt->GetDataToMerge();

	//USB
	usb->LoadTree(i);
	usb->GetEntry(i);

	//if(crt->_hodoStatus==true){

	  eventUsageUSB[ i ] ++;
	  eventUsageCRT[ jentryCRTMin ] ++;

	  nMergedEvents ++;

	  if((unixTimeUSB[i] - usb->UnixTime)!=0){
	    cout<<endl<<"  ERROR --> "<<endl
		<<"(unixTimeUSB[i] - usb->UnixTime)!=0 "<<endl
		<<"(unixTimeUSB[i] - usb->UnixTime) =  "<<(unixTimeUSB[i] - usb->UnixTime)<<endl;
	    assert(0);
	  }	  
	  if((unixTimeCRT[jentryCRTMin] - crt->time_CRT)!=0){
	    cout<<endl<<"  ERROR --> "<<endl
		<<"(unixTimeCRT[jentryCRTMin] - crt->time_CRT)!=0 "<<endl
		<<"(unixTimeCRT[jentryCRTMin] - crt->time_CRT) =  "<<(unixTimeCRT[jentryCRTMin] - crt->time_CRT)<<endl;
	    assert(0);
	  }

	  h1MindT->Fill((unixTimeUSB[i] - unixTimeCRT[jentryCRTMin]));
	  h2MindTvsTime->Fill(unixTimeUSB[i],(unixTimeUSB[i] - unixTimeCRT[jentryCRTMin]));	

	  //\\**********Init data*************
	  ////////////// CRT ////////////
	  eventID_crt = -999;
	  UnixTime_crt = -999.0;
	  x1_crt = -999.0;
	  y1_crt = -999.0;
	  x2_crt = -999.0;
	  y2_crt = -999.0;

	  //////////////USB/////////////
	  eventID_usb = -999;
	  SamplingPeriod_usb = -999.0;
	  UnixTime_usb = -999.0;
	  for(k = 0;k<16;k++){
	    //Charge_usb[k] = -999.0;
	    //Time_usb[k] = -999.0;
	    //Amplitude_usb[k] = -999.0;
	    for(l = 0;l<256;l++)
	      ch_usb[k][l] = -999.0;
	  }	  
	  //\\********************************	
	  
	  //----------CRT data-------------
	  eventID_crt = jentryCRTMin;
	  UnixTime_crt = crt->time_CRT;
	  x1_crt = crt->_x1;
	  y1_crt = crt->_y1;
	  x2_crt = crt->_x2;
	  y2_crt = crt->_y2;
	  
	  //----------USB data-------------
	  eventID_usb = i;
	  SamplingPeriod_usb = usb->SamplingPeriod;//ps
	  UnixTime_usb = usb->UnixTime;
	  for(k = 0;k<16;k++){
	    //Charge_usb[k] = usb->Charge[k];
	    //Time_usb[k] = usb->Time[k];
	    //Amplitude_usb[k] = usb->Amplitude[k];
	    for(l = 0;l<256;l++)
	      ch_usb[k][l] = usb->ch[k][l];
	  }	  	  
	  tree->Fill();

	  //}

      }
      
      

      if(i%10000 == 0)
	cout<<i<<endl;

    }
    /////////////////////////////////////////// 
    hfile = tree->GetCurrentFile();
    hfile->Write();

    h1TimeCRT->Write();
    h1TimeUSB->Write();
    h1MindT->Write();
    h2MindTvsTime->Write();

    TH1I *h1EventUsageCRT = 
      new TH1I( "h1EventUsageCRT", "Usage of CRT events", 11, -0.5, 10.5 );
    
    for(i = 0;i<nEvCRT;i++){
      h1EventUsageCRT->Fill( eventUsageCRT[ i ] );
      if( eventUsageCRT[ i ] > 1 ) {
	cout << "Warning: CRT event " << i 
	     << "is used in " << eventUsageCRT[ i ]
	     << " merged events" << endl;
      }
    }

    h1EventUsageCRT->Write();

    TH1I *h1EventUsageUSB = 
      new TH1I( "h1EventUsageUSB", "Usage of USB events", 11, -0.5, 10.5 );

    for(i = 0;i<nEvUSB;i++){
      h1EventUsageUSB->Fill( eventUsageUSB[ i ] );
      if( eventUsageUSB[ i ] > 1 ) {
	cout << "Warning: USB event " << i 
	     << "is used in " << eventUsageUSB[ i ]
	     << " merged events" << endl;
      }
    }
    
    h1EventUsageUSB->Write();

    hfile->Close();

    cout << "The output ntuple contains " 
	 << nMergedEvents << " merged CRT+USB events" << endl;

    delete usb;
    delete crt;
  } // if(argc == 6 && atoi(argv[1]) == 0){
  */
  
  if(argc == 12 && atoi(argv[1]) == 0){
    /////////////// MERGING ///////////////

    Int_t modeID = atoi(argv[1]);
    TString rootFileListUSBstr = argv[2];
    TString rootFileListCRTstr = argv[3];
    TString rootFileWithTiming = argv[4];
    TString outputMergRootFile = argv[5];

    Int_t nSplinePoints = atoi(argv[6]);
    Double_t signalThreshold = atof(argv[7])/1000.0; //V
    Double_t crossTalkThreshold = atof(argv[8])/1000.0; //V
    Double_t mpFraction = atof(argv[9]);
    Double_t cfdVAL = atof(argv[10]);
    Int_t nPointBaseLineR = atoi(argv[11]);
    Int_t nPointBaseLine  = nPointBaseLineR + (nPointBaseLineR-1)*nSplinePoints;
    Double_t levelch15 = -0.4;
    
    cout<<endl
	<<"         --> merging new data structure <--       "<<endl
	<<" modeID                         --> "<<modeID<<endl
	<<" root file list with USB data   --> "<<rootFileListUSBstr<<endl
	<<" root file list with CRT data   --> "<<rootFileListCRTstr<<endl
	<<" root file with 2-D timing hist --> "<<rootFileWithTiming<<endl
	<<" output root file with data from CRT and USB --> "<<outputMergRootFile<<endl;
    cout<<" n splines                      --> "<<nSplinePoints<<endl
	<<" signalThreshold                --> "<<signalThreshold<<" V"<<endl
	<<" crossTalkThreshold             --> "<<crossTalkThreshold<<" V"<<endl
	<<" mpFraction                     --> "<<mpFraction<<endl
	<<" cfdVAL                         --> "<<cfdVAL<<endl
	<<" nPointBaseLine                 --> "<<nPointBaseLine<<endl;

    //histogram with me difference between USBWC anf 
    //CRT events as a function of USBWC Unix time 
    TFile *ff = new TFile(rootFileWithTiming.Data());  
    if(!ff->IsOpen()){
      cout<<" ERROR ---> file : "<<rootFileWithTiming<<" is not OPEN"<<endl;
      assert(0);
    }
    TH2D *_h2MindTvsTime  = (TH2D*)ff->Get("h2MindTvsTime");

    //setups of the wf analysis
    //Int_t nSplinePoints = 5;
    //Double_t signalThreshold = 30.0/1000.0; //V
    //Double_t crossTalkThreshold = -10.0/1000.0; //V
    //Double_t mpFraction = 0.8;
    //Double_t cfdVAL = 0.5;
    
    //////////////////////////////
    int nEvCRT;//number of events in CRT
    int nEvUSB;//number of event in CRT
    Double_t *unixTimeCRT;
    unixTimeCRT = new Double_t[N];
    Double_t *unixTimeUSB;
    unixTimeUSB = new Double_t[N];
    //////////////////////////////

    // Arrays to monitor how many times a given event
    // is used by the merging procedure
    Int_t eventUsageCRT[N];
    Int_t eventUsageUSB[N];
        
    Int_t i = 0;
    Int_t j = 0;
    Int_t k = 0;
    Int_t l = 0;
    Double_t tMin = -999.0;//minimum Unix time from union of the USBWC and CRT unix times
    Double_t tMax = -999.0;//maximum Unix time from union of the USBWC and CRT unix times
    Double_t tMinOverlap = -999.0;//minimum Unix time from overlap between the USBWC and the CRT unix times
    Double_t tMaxOverlap = -999.0;//maximum Unix time from overlap between the USBWC and the CRT unix times    

    //init root files with USB data
    usbwcFileStr *usb = new usbwcFileStr(); 
    usb->addFilesToChain(rootFileListUSBstr.Data());
    usb->GetDataRate();
    
    //init root files with CRT data    
    crtFileStr   *crt = new crtFileStr(); 
    crt->addFilesToChain(rootFileListCRTstr.Data());
    crt->GetDataRate();

    usb->GetUnixTime( unixTimeUSB, N, nEvUSB);  
    crt->GetUnixTime( unixTimeCRT, N, nEvCRT);
    //printf("%20.5f \n",unixTimeCRT[nEvCRT-1]);
    
    //Build some useful histograms///////////
    tMin = unixTimeCRT[0];
    if(tMin>unixTimeUSB[0])
      tMin = unixTimeUSB[0];
    tMax = unixTimeCRT[nEvCRT-1];
    if(tMax<unixTimeUSB[nEvUSB-1])
      tMax = unixTimeUSB[nEvUSB-1];
    
    tMinOverlap = unixTimeCRT[0];
    if(tMinOverlap<unixTimeUSB[0])
      tMinOverlap = unixTimeUSB[0];
    
    tMaxOverlap = unixTimeCRT[nEvCRT-1];
    if(tMaxOverlap>unixTimeUSB[nEvUSB-1])
      tMaxOverlap = unixTimeUSB[nEvUSB-1];

    if(tMinOverlap>=tMaxOverlap){
      cout<<" ERROR ---> tMinOverlap>=tMaxOverlap"<<endl;
      printf(" //--> %20.5f \n", tMinOverlap);
      printf(" //--> %20.5f \n", tMaxOverlap);
      //<<"tMinOverlap = "<<tMinOverlap<<endl
      //<<"tMaxOverlap = "<<tMaxOverlap<<endl
      cout<<" CRT and USB date are not overlap in time "<<endl;
      assert(0);
    }

    Double_t tMinMy = 0.0;
    Double_t tMaxMy = tMax - tMin;
    Int_t nUnixTimeBin = (Int_t)((tMax - tMin)/3600.0);

    if(nUnixTimeBin < 1.0){
      cout<<" ERROR --> nUnixTimeBin<1.0"<<endl
	  <<"           tMax = "<<tMax<<" s"<<endl
	  <<"           tMin = "<<tMin<<" s"<<endl
	  <<"           dt   = "<<(tMax - tMin)<<" s"<<endl;
      assert(0);
    }

    //06.02.2011 LB change scale from s to h 
    //TH1D *h1TimeUSB = new TH1D("h1TimeUSB"," Time USB ", nUnixTimeBin, tMinMy, tMaxMy); //USB event time distribution
    //TH1D *h1TimeCRT = new TH1D("h1TimeCRT"," Time CRT ", nUnixTimeBin, tMinMy, tMaxMy); //CRT event time distribution
    TH1D *h1TimeUSB = new TH1D("h1TimeUSB"," Time USB ", nUnixTimeBin, tMinMy/3600.0, tMaxMy/3600.0); //USB event time distribution
    TH1D *h1TimeCRT = new TH1D("h1TimeCRT"," Time CRT ", nUnixTimeBin, tMinMy/3600.0, tMaxMy/3600.0); //CRT event time distribution
    //13.02.2011 LB add histogram of CRT USBWC data merging
    TH1D *h1TimeUSBCRT = new TH1D("h1TimeUSBCRT"," Time USB CRT merging ", nUnixTimeBin, tMinMy/3600.0, tMaxMy/3600.0); //CRT event time distribution
    //LB. 13.02.2011 add histograms with time diffeerence between consecutive events
    TH1D *h1dTimeConsUSB    = new TH1D("h1dTimeConsUSB", "dTime Cons USB", 400, 0.0, 200.0);
    TH1D *h1dTimeConsCRT    = new TH1D("h1dTimeConsCRT", "dTime Cons CRT", 400, 0.0, 200.0);
    TH1D *h1dTimeConsUSBCRT = new TH1D("h1dTimeConsUSBCRT", "dTime USB CRT merging", 400, 0.0, 200.0);
    TH1D *h1dTimeConsUSB_short    = new TH1D("h1dTimeConsUSB_short", "dTime Cons USB short", 100, 0.0, 40.0);
    TH1D *h1dTimeConsCRT_short    = new TH1D("h1dTimeConsCRT_short", "dTime Cons CRT short", 100, 0.0, 40.0);
    TH1D *h1dTimeConsUSBCRT_short = new TH1D("h1dTimeConsUSBCRT_short", "dTime USB CRT merging short", 100, 0.0, 40.0);

    Double_t timeLast = 0.0;
    for(i = 0;i<nEvUSB;i++){
      h1TimeUSB->Fill((unixTimeUSB[i] - tMin)/3600.0);
      h1dTimeConsUSB->Fill(unixTimeUSB[i] - timeLast);
      h1dTimeConsUSB_short->Fill(unixTimeUSB[i] - timeLast);
      timeLast = unixTimeUSB[i];
    }
    timeLast = 0.0;
    for(i = 0;i<nEvCRT;i++){
      h1TimeCRT->Fill((unixTimeCRT[i] - tMin)/3600.0);
      h1dTimeConsCRT->Fill(unixTimeCRT[i] - timeLast);
      h1dTimeConsCRT_short->Fill(unixTimeCRT[i] - timeLast);
      timeLast = unixTimeCRT[i];
    }
    h1TimeUSB->SetLineColor(kRed);  
    h1TimeUSB->SetLineWidth(2);
    h1TimeUSB->SetMinimum(0.0);
    h1TimeCRT->SetLineWidth(2);
    h1TimeCRT->SetMinimum(0.0);
    h1TimeUSB->GetXaxis()->SetTitle("time, h");
    h1TimeCRT->GetXaxis()->SetTitle("time, h");
    //////////////////////////////////////////

    for(i = 0; i < N ; i ++ ) {
      eventUsageCRT[i] = 0;
      eventUsageUSB[i] = 0;
    }    

    ////////////// CRT ////////////
    Int_t    eventID_crt;
    Double_t UnixTime_crt;
    Double_t x1_crt;
    Double_t y1_crt;
    Double_t x2_crt;
    Double_t y2_crt;
    Bool_t hodoStatus_crt;
    Double_t trkTheta_crt;
    Double_t trkPhi_crt;
    Int_t trkMomID_crt;
    Double_t x_boxfTOF_crt;
    Double_t y_boxfTOF_crt;
    Double_t z_boxfTOF_crt;
    Double_t x_boxfTOF_Bot_crt;
    Double_t y_boxfTOF_Bot_crt;
    Double_t z_boxfTOF_Bot_crt;
    Double_t x_boxQuartzS_crt;
    Double_t y_boxQuartzS_crt;
    Double_t z_boxQuartzS_crt;


    ////////////// USB //////////
    Double_t  SamplingPeriod_usb;
    Double_t  ch_A_usb[anaConst::usbwcNsamplingPoint];
    Double_t  ch_T_usb[anaConst::usbwcNsamplingPoint];
    Double_t  Charge_usb[anaConst::nChannels];
    Double_t  Time_usb[anaConst::nChannels];
    Double_t  Amplitude_usb[anaConst::nChannels];

    Int_t    merged_eventID = -999; // if events are merged this variable more than -1
    Int_t    mYmerged_eventID = 0;
    Int_t    eventID_usb;
    Double_t UnixTime_usb;

    Int_t    wfID_usb[anaConst::nChannels];
    Double_t MaxBaseLineAmpl_usb[anaConst::nChannels];
    Double_t MinBaseLineAmpl_usb[anaConst::nChannels];
    Double_t FirstAmplitudePos_usb[anaConst::nChannels];
    Double_t FirstAmplitudeNeg_usb[anaConst::nChannels];
    Double_t chargeTOT_usb[anaConst::nChannels];
    Double_t chargeTOT_p_usb[anaConst::nChannels];
    Double_t chargeTOT_m_usb[anaConst::nChannels];
    Double_t FirstRiseTimePos_usb[anaConst::nChannels];
    Double_t FirstFallTimePos_usb[anaConst::nChannels];
    Double_t TotAmplitudePos_usb[anaConst::nChannels];
    Double_t TotTimeAmplitudePos_usb[anaConst::nChannels];
    Double_t FirstWidthTimePos_usb[anaConst::nChannels];
    Double_t FirstTimeBasis_usb[anaConst::nChannels];
    Double_t dTimeFirtsAmplPosChangLeftDer_usb[anaConst::nChannels];
    Double_t dTimeFirtsAmplPosFirstTimeAmplNeg_usb[anaConst::nChannels];
    Double_t MaxAmplBeforeFirstChangDerAmpl_usb[anaConst::nChannels];
    Double_t MinAmplBeforeFirstChangDerAmpl_usb[anaConst::nChannels];
    Double_t firstTimePosCFD_usb[anaConst::nChannels];
    Double_t firstTimeNegCFD_usb[anaConst::nChannels];
    Int_t nOfPosInterAtLevel_usb[anaConst::nChannels];
    Int_t nOfNegInterAtLevel_usb[anaConst::nChannels];
    Double_t SignalThreshold_usb;
    Double_t crossTalkThreshold_usb;
    Double_t mpFraction_usb;
    Double_t valCFD_usb;
    Int_t nSplinePoints_usb;
    Int_t nPointBaseLine_usb;
    Double_t levelch15_usb;
    Double_t timeAtLevelch15_usb;
    /////////////merged output file/////////
    TFile *hfile = new TFile(outputMergRootFile.Data(), "RECREATE", "data from CRT and USBWC", 1);
    if (hfile->IsZombie()) assert(0);
    TTree *tree = new TTree("T", "CRT and USBWC Data Tree");
    hfile->SetCompressionLevel(2);
    tree->SetAutoSave(1000000);
    TTree::SetBranchStyle(0);

    //CRT//
    tree->Branch("eventID_crt", &eventID_crt, "eventID_crt/I");
    tree->Branch("UnixTime_crt", &UnixTime_crt, "UnixTime_crt/D");
    tree->Branch("x1_crt", &x1_crt, "x1_crt/D");
    tree->Branch("y1_crt", &y1_crt, "y1_crt/D");
    tree->Branch("x2_crt", &x2_crt, "x2_crt/D");
    tree->Branch("y2_crt", &y2_crt, "y2_crt/D");
    tree->Branch("hodoStatus_crt", &hodoStatus_crt, "hodoStatus_crt/O");
    tree->Branch("trkTheta_crt", &trkTheta_crt, "trkTheta_crt/D");
    tree->Branch("trkPhi_crt", &trkPhi_crt, "trkPhi_crt/D");
    tree->Branch("trkMomID_crt", &trkMomID_crt, "trkMomID_crt/I");
    tree->Branch("x_boxfTOF_crt", &x_boxfTOF_crt, "x_boxfTOF_crt/D");
    tree->Branch("y_boxfTOF_crt", &y_boxfTOF_crt, "y_boxfTOF_crt/D");
    tree->Branch("z_boxfTOF_crt", &z_boxfTOF_crt, "z_boxfTOF_crt/D");
    tree->Branch("x_boxfTOF_Bot_crt", &x_boxfTOF_Bot_crt, "x_boxfTOF_Bot_crt/D");
    tree->Branch("y_boxfTOF_Bot_crt", &y_boxfTOF_Bot_crt, "y_boxfTOF_Bot_crt/D");
    tree->Branch("z_boxfTOF_Bot_crt", &z_boxfTOF_Bot_crt, "z_boxfTOF_Bot_crt/D");
    tree->Branch("x_boxQuartzS_crt", &x_boxQuartzS_crt, "x_boxQuartzS_crt/D");
    tree->Branch("y_boxQuartzS_crt", &y_boxQuartzS_crt, "y_boxQuartzS_crt/D");
    tree->Branch("z_boxQuartzS_crt", &z_boxQuartzS_crt, "z_boxQuartzS_crt/D");
    
    Int_t chnum = 16;
    if(chnum != anaConst::nChannels){
      cout<<" ERROR ---> chnum != anaConst::nChannels"<<endl
	  <<" chnum              = "<<chnum<<endl
	  <<"anaConst::nChannels = "<<anaConst::nChannels<<endl
	  <<" check size of the arrays in the branches !!! "<<endl;
      assert(0);
    }
    TString ch16Save = "Charge_usb[";
    ch16Save += chnum; 
    ch16Save += "]/D";
    
    //USB//
    tree->Branch("eventID_usb", &eventID_usb, "eventID_usb/I");
    tree->Branch("UnixTime_usb", &UnixTime_usb, "UnixTime_usb/D");
    tree->Branch("merged_eventID", &merged_eventID,"merged_eventID/I");
    tree->Branch("Charge_usb", Charge_usb, ch16Save.Data());
    tree->Branch("Time_usb", Time_usb, "Time_usb[16]/D");
    tree->Branch("Amplitude_usb", Amplitude_usb, "Amplitude_usb[16]/D");
    tree->Branch("wfID_usb", wfID_usb, "wfID_usb[16]/I");
    tree->Branch("MaxBaseLineAmpl_usb", MaxBaseLineAmpl_usb, "MaxBaseLineAmpl_usb[16]/D");
    tree->Branch("MinBaseLineAmpl_usb", MinBaseLineAmpl_usb, "MinBaseLineAmpl_usb[16]/D");
    tree->Branch("FirstAmplitudePos_usb", FirstAmplitudePos_usb, "FirstAmplitudePos_usb[16]/D");
    tree->Branch("FirstAmplitudeNeg_usb", FirstAmplitudeNeg_usb, "FirstAmplitudeNeg_usb[16]/D");
    tree->Branch("chargeTOT_usb", chargeTOT_usb, "chargeTOT_usb[16]/D");
    tree->Branch("chargeTOT_p_usb", chargeTOT_p_usb, "chargeTOT_p_usb[16]/D");
    tree->Branch("chargeTOT_m_usb", chargeTOT_m_usb, "chargeTOT_m_usb[16]/D");
    tree->Branch("FirstRiseTimePos_usb", FirstRiseTimePos_usb, "FirstRiseTimePos_usb[16]/D");
    tree->Branch("FirstFallTimePos_usb", FirstFallTimePos_usb, "FirstFallTimePos_usb[16]/D");
    tree->Branch("TotAmplitudePos_usb", TotAmplitudePos_usb, "TotAmplitudePos_usb[16]/D");
    tree->Branch("TotTimeAmplitudePos_usb", TotTimeAmplitudePos_usb, "TotTimeAmplitudePos_usb[16]/D");
    tree->Branch("FirstWidthTimePos_usb", FirstWidthTimePos_usb, "FirstWidthTimePos_usb[16]/D");
    tree->Branch("FirstTimeBasis_usb", FirstTimeBasis_usb, "FirstTimeBasis_usb[16]/D");
    tree->Branch("dTimeFirtsAmplPosChangLeftDer_usb", dTimeFirtsAmplPosChangLeftDer_usb, "dTimeFirtsAmplPosChangLeftDer_usb[16]/D");
    tree->Branch("dTimeFirtsAmplPosFirstTimeAmplNeg_usb", dTimeFirtsAmplPosFirstTimeAmplNeg_usb, "dTimeFirtsAmplPosFirstTimeAmplNeg_usb[16]/D");
    tree->Branch("MaxAmplBeforeFirstChangDerAmpl_usb", MaxAmplBeforeFirstChangDerAmpl_usb, "MaxAmplBeforeFirstChangDerAmpl_usb[16]/D");
    tree->Branch("MinAmplBeforeFirstChangDerAmpl_usb", MinAmplBeforeFirstChangDerAmpl_usb, "MinAmplBeforeFirstChangDerAmpl_usb[16]/D");
    tree->Branch("firstTimePosCFD_usb", firstTimePosCFD_usb, "firstTimePosCFD_usb[16]/D");
    tree->Branch("firstTimeNegCFD_usb", firstTimeNegCFD_usb, "firstTimeNegCFD_usb[16]/D");
    tree->Branch("nOfPosInterAtLevel_usb", nOfPosInterAtLevel_usb, "nOfPosInterAtLevel_usb[16]/I");
    tree->Branch("nOfNegInterAtLevel_usb", nOfNegInterAtLevel_usb, "nOfNegInterAtLevel_usb[16]/I");
    tree->Branch("SignalThreshold_usb", &SignalThreshold_usb, "SignalThreshold_usb/D");
    tree->Branch("crossTalkThreshold_usb", &crossTalkThreshold_usb, "crossTalkThreshold_usb/D");
    tree->Branch("mpFraction_usb", &mpFraction_usb, "mpFraction_usb/D");
    tree->Branch("valCFD_usb", &valCFD_usb, "valCFD_usb/D");
    tree->Branch("nSplinePoints_usb", &nSplinePoints_usb, "nSplinePoints_usb/I");    
    tree->Branch("nPointBaseLine_usb", &nPointBaseLine_usb, "nPointBaseLine_usb/I");
    tree->Branch("levelch15_usb", &levelch15_usb,"levelch15_usb/D");
    tree->Branch("timeAtLevelch15_usb", &timeAtLevelch15_usb, "timeAtLevelch15_usb/D");
    ////////////////////////////////////////////////
    
    //LB 06.02.2011
    //Int_t nRunTimeBin = nUnixTimeBin/4; //number of run time (X) bin in the h2MindTvsTime
    if(nSecBin<1){
      cout<<" ERROR --> nSecBin<1"<<endl
	  <<" nSecBin = "<<nSecBin<<endl;
      assert(0);
    }
    Int_t nRunTimeBin = (Int_t)((tMaxOverlap - tMinOverlap)/nSecBin);
    if(nRunTimeBin < 1.0){
      cout<<" ERROR --> nRunTimeBin < 1.0"<<endl;
      printf(" tMaxOverlap = %20.5f \n",tMaxOverlap);
      printf(" tMinOverlap = %20.5f \n",tMinOverlap);
      printf(" dt          = %20.5f \n",(tMaxOverlap - tMinOverlap));
      cout<<"           sec/bin     = "<<nSecBin<<" c"<<endl;
      assert(0);
    }    
    printf(" tMinOverlap = %20.5f s \n",tMinOverlap);
    printf(" tMaxOverlap = %20.5f s \n",tMaxOverlap);
    printf(" dt          = %20.5f s \n",(tMaxOverlap - tMinOverlap));
    printf(" dt          = %20.5f h \n",((tMaxOverlap - tMinOverlap)/3600));
    cout<<" nRunTimeBin = "<<nRunTimeBin<<endl;
    cout<<" nSecBin     = "<<nSecBin<<" s"<<endl;

    // min time diff between CTR and USB
    TH1D *h1MindT = new TH1D("h1MindT","Min dT",1000,-20.0,20.0);  
    // min time diff between CTR and USB VS time
    //LB 06.02.2011
    //TH2D *h2MindTvsTime = new TH2D("h2MindTvsTime","Min dT vs time",
    //			   nRunTimeBin, unixTimeUSB[0], unixTimeUSB[nEvUSB-1],ndTimeBin,dTmin,dTmax);
    TH2D *h2MindTvsTime = new TH2D("h2MindTvsTime","Min dT vs time",
				   nRunTimeBin, tMinOverlap, tMaxOverlap,ndTimeBin,dTmin,dTmax);

    Double_t mindT = 1000000.0;
    Int_t jentryCRTMin = 0;

    cout<<"---Merging of the data---"<<endl;

    Int_t nMergedEvents = 0;

    //cout<<"nEvUSB = "<<nEvUSB<<endl;

    Int_t nFillTree = 0;//number of the filling tree

    timeLast = 0.0;
    for(i = 0;i<nEvUSB;i++){

      for(j = jentryCRTMin;j<nEvCRT;j++){
	if(TMath::Abs(unixTimeUSB[i] - unixTimeCRT[j])<mindT){
	  mindT = TMath::Abs(unixTimeUSB[i] - unixTimeCRT[j]);
	  jentryCRTMin = j;
	}
	if((unixTimeCRT[j] - unixTimeUSB[i])>10.0)
	  j = nEvCRT - 1;
      }	
      mindT = 10000000.0;

      if(i%10000 == 0)
	cout<<"merging "<<i<<endl;
      
      //LB 09.02.2011
      //if CRT and USBWC events are mearged 
      if( timingCRT(_h2MindTvsTime,unixTimeUSB[i], unixTimeCRT[jentryCRTMin]) ){
	//Just in case of the laser RUN
	//if( 0>1 ){

	////////////////merg DAQ here/////////////////////

	//CRT
	crt->LoadTree(jentryCRTMin);
	crt->GetEntry(jentryCRTMin);
	crt->GetDataToMerge();

	//USB
	usb->LoadTree(i);
	usb->GetEntry(i);


	//if(crt->_hodoStatus==true){

	eventUsageUSB[i]++;
	eventUsageCRT[jentryCRTMin]++;	
	nMergedEvents++;
	
	if((unixTimeUSB[i] - usb->UnixTime)!=0){
	  cout<<endl<<"  ERROR --> "<<endl
	      <<"(unixTimeUSB[i] - usb->UnixTime)!=0 "<<endl
	      <<"(unixTimeUSB[i] - usb->UnixTime) =  "<<(unixTimeUSB[i] - usb->UnixTime)<<endl;
	  assert(0);
	}	  
	if((unixTimeCRT[jentryCRTMin] - crt->time_CRT)!=0){
	  cout<<endl<<"  ERROR --> "<<endl
	      <<"(unixTimeCRT[jentryCRTMin] - crt->time_CRT)!=0 "<<endl
	      <<"(unixTimeCRT[jentryCRTMin] - crt->time_CRT) =  "<<(unixTimeCRT[jentryCRTMin] - crt->time_CRT)<<endl;
	  assert(0);
	}
	
	h1MindT->Fill((unixTimeUSB[i] - unixTimeCRT[jentryCRTMin]));
	h2MindTvsTime->Fill(unixTimeUSB[i],(unixTimeUSB[i] - unixTimeCRT[jentryCRTMin]));	
	
	h1TimeUSBCRT->Fill((usb->UnixTime - tMin)/3600.0);
	h1dTimeConsUSBCRT->Fill(usb->UnixTime - timeLast);
	h1dTimeConsUSBCRT_short->Fill(usb->UnixTime - timeLast);
	timeLast = usb->UnixTime;

	//\\**********Init data*************
	////////////// CRT ////////////
	eventID_crt = -999;
	UnixTime_crt = -999.0;
	x1_crt = -999.0;
	y1_crt = -999.0;
	x2_crt = -999.0;
	y2_crt = -999.0;
	hodoStatus_crt = 0;
	trkTheta_crt = -999.0;
	trkPhi_crt = -999.0;
	trkMomID_crt = -999;
	x_boxfTOF_crt = -999.0;
	y_boxfTOF_crt = -999.0;
	z_boxfTOF_crt = -999.0;
	x_boxfTOF_Bot_crt = -999.0;
	y_boxfTOF_Bot_crt = -999.0;
	z_boxfTOF_Bot_crt = -999.0;
	x_boxQuartzS_crt = -999.0;
	y_boxQuartzS_crt = -999.0;
	z_boxQuartzS_crt = -999.0;	
	
	//////////////USB/////////////
	eventID_usb = -999;
	SamplingPeriod_usb = -999.0;
	UnixTime_usb = -999.0;
	for(k = 0;k<anaConst::nChannels;k++){
	  Charge_usb[k] = -999.0;
	  Time_usb[k] = -999.0;
	  Amplitude_usb[k] = -999.0;
	  //-------------------
	  wfID_usb[k] = -999;
	  MaxBaseLineAmpl_usb[k] = -999.0;
	  MinBaseLineAmpl_usb[k] = -999.0;
	  FirstAmplitudePos_usb[k] = -999.0;
	  FirstAmplitudeNeg_usb[k] = -999.0;
	  chargeTOT_usb[k] = -999.0;
	  chargeTOT_p_usb[k] = -999.0;
	  chargeTOT_m_usb[k] = -999.0;
	  FirstRiseTimePos_usb[k] = -999.0;
	  FirstFallTimePos_usb[k] = -999.0;
	  TotAmplitudePos_usb[k] = -999.0;
	  TotTimeAmplitudePos_usb[k] = -999.0;
	  FirstWidthTimePos_usb[k] = -999.0;
	  FirstTimeBasis_usb[k] = -999.0;
	  dTimeFirtsAmplPosChangLeftDer_usb[k] = -999.0;
	  dTimeFirtsAmplPosFirstTimeAmplNeg_usb[k] = -999.0;
	  MaxAmplBeforeFirstChangDerAmpl_usb[k] = -999.0;
	  MinAmplBeforeFirstChangDerAmpl_usb[k] = -999.0;
	  firstTimePosCFD_usb[k] = -999.0;
	  firstTimeNegCFD_usb[k] = -999.0;
	  nOfPosInterAtLevel_usb[k] = -999;
	  nOfNegInterAtLevel_usb[k] = -999;
	  //-----------------------
	  for(l = 0;l<anaConst::usbwcNsamplingPoint;l++){
	    ch_A_usb[l] = -999.0;
	    ch_T_usb[l] = -999.0;
	  }
	}
	SignalThreshold_usb = -999.0;
	crossTalkThreshold_usb = -999.0;
	mpFraction_usb = -999.0;
	valCFD_usb = -999.0;
	nSplinePoints_usb = -999;	  
	nPointBaseLine_usb = -999;
	levelch15_usb = -999.0;
	timeAtLevelch15_usb = -999.0;
	//\\********************************	
	
	//----------CRT data-------------
	eventID_crt = jentryCRTMin;
	UnixTime_crt = crt->time_CRT;
	if(crt->_hodoStatus){
	  x1_crt = crt->_x1;
	  y1_crt = crt->_y1;
	  x2_crt = crt->_x2;
	  y2_crt = crt->_y2;
	  hodoStatus_crt = 1;
	  trkTheta_crt = crt->_trkTheta;
	  trkPhi_crt = crt->_trkPhi;
	  trkMomID_crt = crt->_trkMomID;
	  x_boxfTOF_crt = crt->_boxfTOF->_xInt;
	  y_boxfTOF_crt = crt->_boxfTOF->_yInt;
	  z_boxfTOF_crt = crt->_boxfTOF->_zInt;
	  x_boxfTOF_Bot_crt = crt->_boxfTOF_Bot->_xInt;
	  y_boxfTOF_Bot_crt = crt->_boxfTOF_Bot->_yInt;
	  z_boxfTOF_Bot_crt = crt->_boxfTOF_Bot->_zInt;
	  x_boxQuartzS_crt = crt->_boxQuartzS->_xInt;
	  y_boxQuartzS_crt = crt->_boxQuartzS->_yInt;
	  z_boxQuartzS_crt = crt->_boxQuartzS->_zInt;
	}
	else{
	  x1_crt = -999.0;
	  y1_crt = -999.0;
	  x2_crt = -999.0;
	  y2_crt = -999.0;
	  hodoStatus_crt = 0;
	  trkTheta_crt = -999.0;
	  trkPhi_crt = -999.0;
	  trkMomID_crt = -999;
	  x_boxfTOF_crt = -999.0;
	  y_boxfTOF_crt = -999.0;
	  z_boxfTOF_crt = -999.0;
	  x_boxfTOF_Bot_crt = -999.0;
	  y_boxfTOF_Bot_crt = -999.0;
	  z_boxfTOF_Bot_crt = -999.0;
	  x_boxQuartzS_crt = -999.0;
	  y_boxQuartzS_crt = -999.0;
	  z_boxQuartzS_crt = -999.0;
	}
	
	//----------USB data-------------
	eventID_usb = i;
	UnixTime_usb = usb->UnixTime;
	SamplingPeriod_usb = usb->SamplingPeriod;//ps
	for(k = 0;k<anaConst::nChannels;k++){
	  Charge_usb[k] = usb->Charge[k];
	  Time_usb[k] = usb->Time[k];
	  Amplitude_usb[k] = usb->Amplitude[k];
	  for(l = 0;l<anaConst::usbwcNsamplingPoint;l++){
	    ch_A_usb[l] = usb->ch[k][l];
	    ch_T_usb[l] = l*SamplingPeriod_usb/1000.0;
	  }

	  waveform  *wf = new waveform( ch_T_usb, ch_A_usb, anaConst::usbwcNsamplingPoint, nSplinePoints);
	  wf->SetSignalThreshold(signalThreshold);
	  wf->SetCrossTalkThreshold(crossTalkThreshold);
	  wf->SetMpFraction(mpFraction);
	  wf->SetNpointBaseLine(nPointBaseLine);
	  wf->findParametersOftheWaveform();

	  wfID_usb[k] = wf->GetWfID();
	  MaxBaseLineAmpl_usb[k] = wf->GetMaxBaseLineAmpl();
	  MinBaseLineAmpl_usb[k] = wf->GetMinBaseLineAmpl();
	  FirstAmplitudePos_usb[k] = wf->GetFirstAmplitudePos();
	  FirstAmplitudeNeg_usb[k] = wf->GetFirstAmplitudeNeg();
	  chargeTOT_usb[k] = wf->GetChargeTOT();
	  chargeTOT_p_usb[k] = wf->GetChargeTOT_p();
	  chargeTOT_m_usb[k] = wf->GetChargeTOT_m();
	  FirstRiseTimePos_usb[k] = wf->GetFirstRiseTimePos();
	  FirstFallTimePos_usb[k] = wf->GetFirstFallTimePos();
	  TotAmplitudePos_usb[k] = wf->GetTotAmplitudePos();
	  TotTimeAmplitudePos_usb[k] = wf->GetTotTimeAmplitudePos();
	  FirstWidthTimePos_usb[k] = wf->GetFirstWidthTimePos();
	  FirstTimeBasis_usb[k] = wf->GetFirstTimeBasis();
	  dTimeFirtsAmplPosChangLeftDer_usb[k] = wf->GetdTimeFirtsAmplPosChangLeftDer();
	  dTimeFirtsAmplPosFirstTimeAmplNeg_usb[k] = wf->GetdTimeFirtsAmplPosFirstTimeAmplNeg();
	  MaxAmplBeforeFirstChangDerAmpl_usb[k] = wf->GetMaxAmplBeforeFirstChangDerAmpl();
	  MinAmplBeforeFirstChangDerAmpl_usb[k] = wf->GetMinAmplBeforeFirstChangDerAmpl();
	  firstTimePosCFD_usb[k] = wf->findFirstTimePosCFD(cfdVAL);
	  firstTimeNegCFD_usb[k] = wf->findFirstTimeNegCFD(cfdVAL);
	  if(FirstAmplitudePos_usb[k] != -999.0)
	    nOfPosInterAtLevel_usb[k] = wf->findNumberOfPosIntersectionAtLevel(FirstAmplitudePos_usb[k]*cfdVAL);
	  if(FirstAmplitudeNeg_usb[k] != -999.0)
	    nOfNegInterAtLevel_usb[k] = wf->findNumberOfPosIntersectionAtLevel(FirstAmplitudeNeg_usb[k]*cfdVAL);
	  if(k==15){
	    levelch15_usb = levelch15;
	    timeAtLevelch15_usb = wf->findFirstTimePosAtLevelFallEdge(levelch15);
	  }
	  delete wf;
	}
	SignalThreshold_usb = signalThreshold;
	crossTalkThreshold_usb = crossTalkThreshold;
	mpFraction_usb = mpFraction;
	valCFD_usb = cfdVAL;
	nSplinePoints_usb = nSplinePoints;	  
	nPointBaseLine_usb = nPointBaseLine;
	//cout<<"merging tree->Fill() "<<nFillTree<<endl;
	nFillTree++;
	merged_eventID = mYmerged_eventID;
	tree->Fill();
	mYmerged_eventID++;
	//}
      }
      else{
	//if no merging !!!!
	//USB
	usb->LoadTree(i);
	usb->GetEntry(i);
	
	if((unixTimeUSB[i] - usb->UnixTime)!=0){
	  cout<<endl<<"  ERROR --> "<<endl
	      <<"(unixTimeUSB[i] - usb->UnixTime)!=0 "<<endl
	      <<"(unixTimeUSB[i] - usb->UnixTime) =  "<<(unixTimeUSB[i] - usb->UnixTime)<<endl;
	  assert(0);
	}	  
	
	//\\**********Init data*************
	////////////// CRT ////////////
	eventID_crt = -999;
	UnixTime_crt = -999.0;
	x1_crt = -999.0;
	y1_crt = -999.0;
	x2_crt = -999.0;
	y2_crt = -999.0;
	hodoStatus_crt = 0;
	trkTheta_crt = -999.0;
	trkPhi_crt = -999.0;
	trkMomID_crt = -999;
	x_boxfTOF_crt = -999.0;
	y_boxfTOF_crt = -999.0;
	z_boxfTOF_crt = -999.0;
	x_boxfTOF_Bot_crt = -999.0;
	y_boxfTOF_Bot_crt = -999.0;
	z_boxfTOF_Bot_crt = -999.0;
	x_boxQuartzS_crt = -999.0;
	y_boxQuartzS_crt = -999.0;
	z_boxQuartzS_crt = -999.0;	
	
	//////////////USB/////////////
	eventID_usb = -999;
	SamplingPeriod_usb = -999.0;
	UnixTime_usb = -999.0;
	for(k = 0;k<16;k++){
	  Charge_usb[k] = -999.0;
	  Time_usb[k] = -999.0;
	  Amplitude_usb[k] = -999.0;
	  //-------------------
	  wfID_usb[k] = -999;
	  MaxBaseLineAmpl_usb[k] = -999.0;
	  MinBaseLineAmpl_usb[k] = -999.0;
	  FirstAmplitudePos_usb[k] = -999.0;
	  FirstAmplitudeNeg_usb[k] = -999.0;
	  chargeTOT_usb[k] = -999.0;
	  chargeTOT_p_usb[k] = -999.0;
	  chargeTOT_m_usb[k] = -999.0;
	  FirstRiseTimePos_usb[k] = -999.0;
	  FirstFallTimePos_usb[k] = -999.0;
	  TotAmplitudePos_usb[k] = -999.0;
	  TotTimeAmplitudePos_usb[k] = -999.0;
	  FirstWidthTimePos_usb[k] = -999.0;
	  FirstTimeBasis_usb[k] = -999.0;
	  dTimeFirtsAmplPosChangLeftDer_usb[k] = -999.0;
	  dTimeFirtsAmplPosFirstTimeAmplNeg_usb[k] = -999.0;
	  MaxAmplBeforeFirstChangDerAmpl_usb[k] = -999.0;
	  MinAmplBeforeFirstChangDerAmpl_usb[k] = -999.0;
	  firstTimePosCFD_usb[k] = -999.0;
	  firstTimeNegCFD_usb[k] = -999.0;
	  nOfPosInterAtLevel_usb[k] = -999;
	  nOfNegInterAtLevel_usb[k] = -999;
	  //-----------------------
	  for(l = 0;l<anaConst::usbwcNsamplingPoint;l++){
	    ch_A_usb[l] = -999.0;
	    ch_T_usb[l] = -999.0;
	  }
	  SignalThreshold_usb = -999.0;
	  crossTalkThreshold_usb = -999.0;
	  mpFraction_usb = -999.0;
	  valCFD_usb = -999.0;
	  nSplinePoints_usb = -999;
	  nPointBaseLine_usb = -999;  
	  levelch15_usb = -999.0;
	  timeAtLevelch15_usb = -999.0;
	}
	//\\********************************	
		
	//----------USB data-------------
	eventID_usb = i;
	UnixTime_usb = usb->UnixTime;
	SamplingPeriod_usb = usb->SamplingPeriod;//ps
	for(k = 0;k<16;k++){
	  Charge_usb[k] = usb->Charge[k];
	  Time_usb[k] = usb->Time[k];
	  Amplitude_usb[k] = usb->Amplitude[k];
	  for(l = 0;l<anaConst::usbwcNsamplingPoint;l++){
	    ch_A_usb[l] = usb->ch[k][l];
	    ch_T_usb[l] = l*SamplingPeriod_usb/1000.0;
	  }
	  
	  waveform  *wf = new waveform( ch_T_usb, ch_A_usb, anaConst::usbwcNsamplingPoint, nSplinePoints);
	  wf->SetSignalThreshold(signalThreshold);
	  wf->SetCrossTalkThreshold(crossTalkThreshold);
	  wf->SetMpFraction(mpFraction);
	  wf->SetNpointBaseLine(nPointBaseLine);
	  wf->findParametersOftheWaveform();
	  
	  wfID_usb[k] = wf->GetWfID();
	  MaxBaseLineAmpl_usb[k] = wf->GetMaxBaseLineAmpl();
	  MinBaseLineAmpl_usb[k] = wf->GetMinBaseLineAmpl();
	  FirstAmplitudePos_usb[k] = wf->GetFirstAmplitudePos();
	  FirstAmplitudeNeg_usb[k] = wf->GetFirstAmplitudeNeg();
	  chargeTOT_usb[k] = wf->GetChargeTOT();
	  chargeTOT_p_usb[k] = wf->GetChargeTOT_p();
	  chargeTOT_m_usb[k] = wf->GetChargeTOT_m();
	  FirstRiseTimePos_usb[k] = wf->GetFirstRiseTimePos();
	  FirstFallTimePos_usb[k] = wf->GetFirstFallTimePos();
	  TotAmplitudePos_usb[k] = wf->GetTotAmplitudePos();
	  TotTimeAmplitudePos_usb[k] = wf->GetTotTimeAmplitudePos();
	  FirstWidthTimePos_usb[k] = wf->GetFirstWidthTimePos();
	  FirstTimeBasis_usb[k] = wf->GetFirstTimeBasis();
	  dTimeFirtsAmplPosChangLeftDer_usb[k] = wf->GetdTimeFirtsAmplPosChangLeftDer();
	  dTimeFirtsAmplPosFirstTimeAmplNeg_usb[k] = wf->GetdTimeFirtsAmplPosFirstTimeAmplNeg();
	  MaxAmplBeforeFirstChangDerAmpl_usb[k] = wf->GetMaxAmplBeforeFirstChangDerAmpl();
	  MinAmplBeforeFirstChangDerAmpl_usb[k] = wf->GetMinAmplBeforeFirstChangDerAmpl();
	  firstTimePosCFD_usb[k] = wf->findFirstTimePosCFD(cfdVAL);
	  firstTimeNegCFD_usb[k] = wf->findFirstTimeNegCFD(cfdVAL);
	  if(FirstAmplitudePos_usb[k] != -999.0)
	    nOfPosInterAtLevel_usb[k] = wf->findNumberOfPosIntersectionAtLevel(FirstAmplitudePos_usb[k]*cfdVAL);
	  if(FirstAmplitudeNeg_usb[k] != -999.0)
	    nOfNegInterAtLevel_usb[k] = wf->findNumberOfPosIntersectionAtLevel(FirstAmplitudeNeg_usb[k]*cfdVAL);
	  if(k==15){
	    levelch15_usb = levelch15;
	    timeAtLevelch15_usb = wf->findFirstTimePosAtLevelFallEdge(levelch15);
	  }
	  delete wf;
	}
	SignalThreshold_usb = signalThreshold;
	crossTalkThreshold_usb = crossTalkThreshold;
	mpFraction_usb = mpFraction;
	valCFD_usb = cfdVAL;
	nSplinePoints_usb = nSplinePoints;
	nPointBaseLine_usb = nPointBaseLine;	  
	//cout<<"No merging tree->Fill() "<<nFillTree<<endl;
	nFillTree++;
	merged_eventID = -999;
	tree->Fill();

      }
    }
    /////////////////////////////////////////// 
    hfile = tree->GetCurrentFile();
    hfile->Write();
    
    h1TimeCRT->Write();
    h1TimeUSB->Write();

    h1MindT->Write();
    h2MindTvsTime->Write();


    h1TimeUSBCRT->SetLineColor(kBlue);
    h1TimeUSBCRT->SetLineWidth(2);
    h1TimeUSBCRT->SetMinimum(0.0);
    h1TimeUSBCRT->GetXaxis()->SetTitle("time, h");
    h1TimeUSBCRT->Write();

    h1dTimeConsUSB->Write();
    h1dTimeConsCRT->Write();
    h1dTimeConsUSBCRT->Write();
    h1dTimeConsUSB_short->Write();
    h1dTimeConsCRT_short->Write();
    h1dTimeConsUSBCRT_short->Write();

    
    TH1I *h1EventUsageCRT = 
      new TH1I( "h1EventUsageCRT", "Usage of CRT events", 11, -0.5, 10.5 );
    
    for(i = 0;i<nEvCRT;i++){
      h1EventUsageCRT->Fill( eventUsageCRT[ i ] );
      if( eventUsageCRT[ i ] > 1 ){
	cout<<endl<<" CRT event Usage "<<endl
	    << "Warning: CRT event " << i 
	    << " is used in " << eventUsageCRT[ i ]
	    << " merged events" << endl;
      }
    }
    
    h1EventUsageCRT->Write();
    
    TH1I *h1EventUsageUSB = 
      new TH1I( "h1EventUsageUSB", "Usage of USB events", 11, -0.5, 10.5 );
    
    for(i = 0;i<nEvUSB;i++){
      h1EventUsageUSB->Fill( eventUsageUSB[ i ] );
      if( eventUsageUSB[ i ] > 1 ) {
	cout<<endl<<" USB event Usage "<<endl
	    << "ERROR: USB event " << i 
	    << "is used in " << eventUsageUSB[ i ]
	    << " merged events" << endl;
	assert(0);
      }
    }
    
    h1EventUsageUSB->Write();
    
    hfile->Close();
    cout<<"nFillTree = "<<nFillTree<<endl;
    
    cout << "The output ntuple contains " 
	 << nMergedEvents << " merged CRT+USB events" << endl;

    cout<<"     ////\\\\ merging new data structure ////\\\\"<<endl
	<<"                         END"<<endl;

    delete usb;
    delete crt;
  } // if(argc == 6 && atoi(argv[1]) == 0){
  else if(argc == 5 && atoi(argv[1]) == 1){

    ///////////// TIMING ///////////// 

    Int_t modeID = atoi(argv[1]);
    TString rootFileListUSBstr = argv[2];
    TString rootFileListCRTstr = argv[3];
    TString outputRootFileWithTiming = argv[4];
    
    cout<<endl
	<<"           --> timing histograms <--       "<<endl
	<<" modeID                         --> "<<modeID<<endl
	<<" root file list with USB data   --> "<<rootFileListUSBstr<<endl
	<<" root file list with CRT data   --> "<<rootFileListCRTstr<<endl
	<<" output root file with 2-D timing hist --> "<<outputRootFileWithTiming<<endl;

    //////////////////////////////

    int nEvCRT;//number of events in CRT
    int nEvUSB;//number of event in CRT
    Double_t *unixTimeCRT;
    unixTimeCRT = new Double_t[N];
    Double_t *unixTimeUSB;
    unixTimeUSB = new Double_t[N];
    //////////////////////////////
    
    Int_t i = 0;
    Int_t j = 0;
    Double_t tMin = -999.0;//minimum Unix time from union of the USBWC and CRT unix times
    Double_t tMax = -999.0;//maximum Unix time from union of the USBWC and CRT unix times
    Double_t tMinOverlap = -999.0;//minimum Unix time from overlap between the USBWC and the CRT unix times
    Double_t tMaxOverlap = -999.0;//maximum Unix time from overlap between the USBWC and the CRT unix times
    
    //init root files with USB data
    usbwcFileStr *usb = new usbwcFileStr(); 
    usb->addFilesToChain(rootFileListUSBstr.Data());
    Int_t nUSBev = usb->GetDataRate();
    //LB 08.02.2011
    //check if nUSBev<N (N - is maximum possible number of events for USB and CRT) 
    if(nUSBev>N){
      cout<<endl
	  <<" ERROR ---> nUSBev>N "<<endl
	  <<"            nUSBev = "<<nUSBev<<endl
	  <<"            N      = "<<N<<endl;
      assert(0);
    }    

    //init root files with CRT data    
    crtFileStr   *crt = new crtFileStr(); 
    crt->addFilesToChain(rootFileListCRTstr.Data());
    Int_t nCRTev = crt->GetDataRate(); //Total number of CRT events.
    //LB 08.02.2011
    //check if nCRTev<N (N - is maximum possible number of events for USB and CRT) 
    if(nCRTev>N){
      cout<<endl
	  <<" ERROR ---> nCRTev>N "<<endl
	  <<"            nCRTev = "<<nCRTev<<endl
	  <<"            N      = "<<N<<endl;
      assert(0);
    }

    usb->GetUnixTime( unixTimeUSB, N, nEvUSB);  
    crt->GetUnixTime( unixTimeCRT, N, nEvCRT);
    //printf("%20.5f \n",unixTimeCRT[nEvCRT-1]);
    
    //Build some useful histograms///////////
    tMin = unixTimeCRT[0];
    if(tMin>unixTimeUSB[0])
      tMin = unixTimeUSB[0];
    tMax = unixTimeCRT[nEvCRT-1];
    if(tMax<unixTimeUSB[nEvUSB-1])
      tMax = unixTimeUSB[nEvUSB-1];

    tMinOverlap = unixTimeCRT[0];
    if(tMinOverlap<unixTimeUSB[0])
      tMinOverlap = unixTimeUSB[0];
    
    tMaxOverlap = unixTimeCRT[nEvCRT-1];
    if(tMaxOverlap>unixTimeUSB[nEvUSB-1])
      tMaxOverlap = unixTimeUSB[nEvUSB-1];

    if(tMinOverlap>=tMaxOverlap){
      cout<<" ERROR ---> tMinOverlap>=tMaxOverlap"<<endl;
      printf(" //--> %20.5f \n", tMinOverlap);
      printf(" //--> %20.5f \n", tMaxOverlap);
      //<<"tMinOverlap = "<<tMinOverlap<<endl
      //<<"tMaxOverlap = "<<tMaxOverlap<<endl
      cout<<" CRT and USB date are not overlap in time "<<endl;
      assert(0);
    }
    
    Double_t tMinMy = 0.0;
    Double_t tMaxMy = tMax - tMin;
    //Int_t nUnixTimeBin = (Int_t)((tMax - tMin)/(3600.0/24.0));
    Int_t nUnixTimeBin = (Int_t)((tMax - tMin)/3600.0);
    
    if(nUnixTimeBin < 1.0){
      cout<<" ERROR --> nUnixTimeBin<1.0"<<endl
	  <<"           tMax = "<<tMax<<" s"<<endl
	  <<"           tMin = "<<tMin<<" s"<<endl
	  <<"           dt   = "<<(tMax - tMin)<<" s"<<endl;
      assert(0);
    }

    //06.02.2011 LB change scale from s to h 
    //TH1D *h1TimeUSB = new TH1D("h1TimeUSB"," Time USB ", nUnixTimeBin, tMinMy, tMaxMy); //USB event time distribution
    //TH1D *h1TimeCRT = new TH1D("h1TimeCRT"," Time CRT ", nUnixTimeBin, tMinMy, tMaxMy); //CRT event time distribution
    TH1D *h1TimeUSB = new TH1D("h1TimeUSB"," Time USB ", nUnixTimeBin, tMinMy/3600.0, tMaxMy/3600.0); //USB event time distribution
    TH1D *h1TimeCRT = new TH1D("h1TimeCRT"," Time CRT ", nUnixTimeBin, tMinMy/3600.0, tMaxMy/3600.0); //CRT event time distribution
    //LB 14.02.2011 add information about unix dt between two consecutive events
    TH1D *h1dTimeConsUSB    = new TH1D("h1dTimeConsUSB", "dTime Cons USB", 400, 0.0, 200.0);
    TH1D *h1dTimeConsCRT    = new TH1D("h1dTimeConsCRT", "dTime Cons CRT", 400, 0.0, 200.0);
    //TH1D *h1dTimeConsUSBCRT = new TH1D("h1dTimeConsUSBCRT", "dTime USB CRT merging", 400, 0.0, 200.0);
    TH1D *h1dTimeConsUSB_short    = new TH1D("h1dTimeConsUSB_short", "dTime Cons USB short", 100, 0.0, 40.0);
    TH1D *h1dTimeConsCRT_short    = new TH1D("h1dTimeConsCRT_short", "dTime Cons CRT short", 100, 0.0, 40.0);
    //TH1D *h1dTimeConsUSBCRT_short = new TH1D("h1dTimeConsUSBCRT_short", "dTime USB CRT merging short", 100, 0.0, 40.0);

    Double_t timeLast = 0.0;
    for(i = 0;i<nEvUSB;i++){
      h1TimeUSB->Fill((unixTimeUSB[i] - tMin)/3600.0);
      h1dTimeConsUSB->Fill(unixTimeUSB[i] - timeLast);
      h1dTimeConsUSB_short->Fill(unixTimeUSB[i] - timeLast);
      timeLast = unixTimeUSB[i];
    }
    timeLast = 0.0;
    for(i = 0;i<nEvCRT;i++){
      h1TimeCRT->Fill((unixTimeCRT[i] - tMin)/3600.0);
      h1dTimeConsCRT->Fill(unixTimeCRT[i] - timeLast);
      h1dTimeConsCRT_short->Fill(unixTimeCRT[i] - timeLast);
      timeLast = unixTimeCRT[i];
    }
    h1TimeUSB->SetLineColor(kRed);  
    h1TimeUSB->SetLineWidth(2);
    h1TimeUSB->SetMinimum(0.0);
    h1TimeCRT->SetLineWidth(2);
    h1TimeCRT->SetMinimum(0.0);
    h1TimeUSB->GetXaxis()->SetTitle("time, h");
    h1TimeCRT->GetXaxis()->SetTitle("time, h");
    //////////////////////////////////////////

    //LB 04.02.2011
    //Int_t nRunTimeBin = nUnixTimeBin/4; //number of run time (X) bin in the h2MindTvsTime

    if(nSecBin<1){
      cout<<" ERROR --> nSecBin<1"<<endl
	  <<" nSecBin = "<<nSecBin<<endl;
      assert(0);
    }
    Int_t nRunTimeBin = (Int_t)((tMaxOverlap - tMinOverlap)/nSecBin);
    if(nRunTimeBin < 1.0){
      cout<<" ERROR --> nRunTimeBin < 1.0"<<endl;
      printf(" tMaxOverlap = %20.5f \n",tMaxOverlap);
      printf(" tMinOverlap = %20.5f \n",tMinOverlap);
      printf(" dt          = %20.5f \n",(tMaxOverlap - tMinOverlap));
      cout<<"           sec/bin     = "<<nSecBin<<" c"<<endl;
      assert(0);
    }    
    printf(" tMinOverlap = %20.5f s \n",tMinOverlap);
    printf(" tMaxOverlap = %20.5f s \n",tMaxOverlap);
    printf(" dt          = %20.5f s \n",(tMaxOverlap - tMinOverlap));
    printf(" dt          = %20.5f h \n",((tMaxOverlap - tMinOverlap)/3600));
    cout<<" nRunTimeBin = "<<nRunTimeBin<<endl;
    cout<<" nSecBin     = "<<nSecBin<<" s"<<endl;
    
    // min time diff between CTR and USB
    TH1D *h1MindT = new TH1D("h1MindT","Min dT",1000,-20.0,20.0);  
    // min time diff between CTR and USB VS time
    //LB 04.02.2011
    //TH2D *h2MindTvsTime = new TH2D("h2MindTvsTime","Min dT vs time",
    //			   nRunTimeBin, unixTimeUSB[0], unixTimeUSB[nEvUSB-1],ndTimeBin,dTmin,dTmax);
    TH2D *h2MindTvsTime = new TH2D("h2MindTvsTime","Min dT vs time",
				   nRunTimeBin, tMinOverlap, tMaxOverlap,ndTimeBin,dTmin,dTmax);
    
    Double_t mindT = 1000000.0;
    Int_t jentryCRTMin = 0;

    cout<<"---Filling timing Histograms---"<<endl;    
    for(i = 0;i<nEvUSB;i++){      
      for(j = jentryCRTMin;j<nEvCRT;j++){
	if(TMath::Abs(unixTimeUSB[i] - unixTimeCRT[j])<mindT){
	  mindT = TMath::Abs(unixTimeUSB[i] - unixTimeCRT[j]);
	  jentryCRTMin = j;
	}
	if((unixTimeCRT[j] - unixTimeUSB[i])>10.0)
	  j = nEvCRT;
      }
      mindT = 10000000.0;

      h1MindT->Fill((unixTimeUSB[i] - unixTimeCRT[jentryCRTMin]));
      h2MindTvsTime->Fill(unixTimeUSB[i],(unixTimeUSB[i] - unixTimeCRT[jentryCRTMin]));

      if(i%10000 == 0)
	cout<<i<<endl;      
    }    

    TFile *hfile = new TFile(outputRootFileWithTiming.Data(), "RECREATE", " delta unix time for CRT and USBWC", 1);
    if (hfile->IsZombie()) assert(0);
    h1TimeUSB->Write();
    h1TimeCRT->Write();
    h1MindT->Write();
    h2MindTvsTime->Write();
    h1dTimeConsUSB->Write();
    h1dTimeConsCRT->Write();
    //h1dTimeConsUSBCRT->Write();
    h1dTimeConsUSB_short->Write();
    h1dTimeConsCRT_short->Write();
    //h1dTimeConsUSBCRT_short->Write();

    cout<<" ****** --> //// timing histograms \\\\ <-- ****** "<<endl
	<<" ****** --> ////        END        \\\\ <-- ****** "<<endl<<endl<<endl;

  } // if(argc == 5 && atoi(argv[1]) == 1){
  else if(argc == 12 && atoi(argv[1]) == 2){
    /////////////// FASTMERGING ///////////////

    Int_t modeID = atoi(argv[1]);
    TString rootFileListUSBstr = argv[2];
    TString rootFileListCRTstr = argv[3];
    TString rootFileWithTiming = argv[4];
    TString outputMergRootFile = argv[5];

    Int_t nSplinePoints = atoi(argv[6]);
    Double_t signalThreshold = atof(argv[7])/1000.0; //V
    Double_t crossTalkThreshold = atof(argv[8])/1000.0; //V
    Double_t mpFraction = atof(argv[9]);
    Double_t cfdVAL = atof(argv[10]);
    Int_t nPointBaseLineR = atoi(argv[11]);
    Int_t nPointBaseLine  = nPointBaseLineR + (nPointBaseLineR-1)*nSplinePoints;

    cout<<endl
	<<"         --> merging test 'fast merging' <--       "<<endl
	<<" modeID                         --> "<<modeID<<endl
	<<" root file list with USB data   --> "<<rootFileListUSBstr<<endl
	<<" root file list with CRT data   --> "<<rootFileListCRTstr<<endl
	<<" root file with 2-D timing hist --> "<<rootFileWithTiming<<endl
	<<" root file with output histograms --> "<<outputMergRootFile<<endl;
    cout<<" n splines                      --> "<<nSplinePoints<<endl
	<<" signalThreshold                --> "<<signalThreshold<<" V"<<endl
	<<" crossTalkThreshold             --> "<<crossTalkThreshold<<" V"<<endl
	<<" mpFraction                     --> "<<mpFraction<<endl
	<<" cfdVAL                         --> "<<cfdVAL<<endl
	<<" nPointBaseLine                 --> "<<nPointBaseLine<<endl;

    //histogram with min difference between USBWC and 
    //CRT events as a function of USBWC Unix time 
    TFile *ff = new TFile(rootFileWithTiming.Data());  
    TH2D *_h2MindTvsTime  = (TH2D*)ff->Get("h2MindTvsTime");
    
    //////////////////////////////
    int nEvCRT;//number of events in CRT
    int nEvUSB;//number of event in CRT
    Double_t *unixTimeCRT;
    unixTimeCRT = new Double_t[N];
    Double_t *unixTimeUSB;
    unixTimeUSB = new Double_t[N];
    //////////////////////////////

    TH1I *h1crtEvIDMerg = new TH1I("h1crtEvIDMerg","crt EvID Merg",N,0,N);

    // Arrays to monitor how many times a given event
    // is used by the merging procedure
    Int_t eventUsageCRT[N];
    Int_t eventUsageUSB[N];
        
    Int_t i = 0;
    Int_t j = 0;
    Double_t tMin = -999.0;//minimum Unix time from union of the USBWC and CRT unix times
    Double_t tMax = -999.0;//maximum Unix time from union of the USBWC and CRT unix times
    Double_t tMinOverlap = -999.0;//minimum Unix time from overlap between the USBWC and the CRT unix times
    Double_t tMaxOverlap = -999.0;//maximum Unix time from overlap between the USBWC and the CRT unix times    

    //init root files with USB data
    usbwcFileStr *usb = new usbwcFileStr(); 
    usb->addFilesToChain(rootFileListUSBstr.Data());
    usb->GetDataRate();
    
    //init root files with CRT data    
    crtFileStr   *crt = new crtFileStr(); 
    crt->addFilesToChain(rootFileListCRTstr.Data());
    crt->GetDataRate();

    usb->GetUnixTime( unixTimeUSB, N, nEvUSB);  
    crt->GetUnixTime( unixTimeCRT, N, nEvCRT);
    //printf("%20.5f \n",unixTimeCRT[nEvCRT-1]);
    
    //Build some useful histograms///////////
    tMin = unixTimeCRT[0];
    if(tMin>unixTimeUSB[0])
      tMin = unixTimeUSB[0];
    tMax = unixTimeCRT[nEvCRT-1];
    if(tMax<unixTimeUSB[nEvUSB-1])
      tMax = unixTimeUSB[nEvUSB-1];
    
    tMinOverlap = unixTimeCRT[0];
    if(tMinOverlap<unixTimeUSB[0])
      tMinOverlap = unixTimeUSB[0];
    
    tMaxOverlap = unixTimeCRT[nEvCRT-1];
    if(tMaxOverlap>unixTimeUSB[nEvUSB-1])
      tMaxOverlap = unixTimeUSB[nEvUSB-1];

    if(tMinOverlap>=tMaxOverlap){
      cout<<" ERROR ---> tMinOverlap>=tMaxOverlap"<<endl;
      printf(" //--> %20.5f \n", tMinOverlap);
      printf(" //--> %20.5f \n", tMaxOverlap);
      //<<"tMinOverlap = "<<tMinOverlap<<endl
      //<<"tMaxOverlap = "<<tMaxOverlap<<endl
      cout<<" CRT and USB date are not overlap in time "<<endl;
      assert(0);
    }

    Double_t tMinMy = 0.0;
    Double_t tMaxMy = tMax - tMin;
    Int_t nUnixTimeBin = (Int_t)((tMax - tMin)/3600.0);

    if(nUnixTimeBin < 1.0){
      cout<<" ERROR --> nUnixTimeBin<1.0"<<endl
	  <<"           tMax = "<<tMax<<" s"<<endl
	  <<"           tMin = "<<tMin<<" s"<<endl
	  <<"           dt   = "<<(tMax - tMin)<<" s"<<endl;
      assert(0);
    }

    TH1D *h1TimeUSB = new TH1D("h1TimeUSB"," Time USB ", nUnixTimeBin, tMinMy/3600.0, tMaxMy/3600.0); //USB event time distribution
    TH1D *h1TimeCRT = new TH1D("h1TimeCRT"," Time CRT ", nUnixTimeBin, tMinMy/3600.0, tMaxMy/3600.0); //CRT event time distribution
    TH1D *h1TimeUSBCRT = new TH1D("h1TimeUSBCRT"," Time USB CRT merging ", nUnixTimeBin, tMinMy/3600.0, tMaxMy/3600.0); //CRT event time distribution
    TH1D *h1dTimeConsUSB    = new TH1D("h1dTimeConsUSB", "dTime Cons USB", 400, 0.0, 200.0);
    TH1D *h1dTimeConsCRT    = new TH1D("h1dTimeConsCRT", "dTime Cons CRT", 400, 0.0, 200.0);
    TH1D *h1dTimeConsUSBCRT = new TH1D("h1dTimeConsUSBCRT", "dTime USB CRT merging", 400, 0.0, 200.0);
    TH1D *h1dTimeConsUSB_short    = new TH1D("h1dTimeConsUSB_short", "dTime Cons USB short", 100, 0.0, 40.0);
    TH1D *h1dTimeConsCRT_short    = new TH1D("h1dTimeConsCRT_short", "dTime Cons CRT short", 100, 0.0, 40.0);
    TH1D *h1dTimeConsUSBCRT_short = new TH1D("h1dTimeConsUSBCRT_short", "dTime USB CRT merging short", 100, 0.0, 40.0);

    Double_t timeLast = 0.0;
    for(i = 0;i<nEvUSB;i++){
      h1TimeUSB->Fill((unixTimeUSB[i] - tMin)/3600.0);
      h1dTimeConsUSB->Fill(unixTimeUSB[i] - timeLast);
      h1dTimeConsUSB_short->Fill(unixTimeUSB[i] - timeLast);
      timeLast = unixTimeUSB[i];
    }
    timeLast = 0.0;
    for(i = 0;i<nEvCRT;i++){
      h1TimeCRT->Fill((unixTimeCRT[i] - tMin)/3600.0);
      h1dTimeConsCRT->Fill(unixTimeCRT[i] - timeLast);
      h1dTimeConsCRT_short->Fill(unixTimeCRT[i] - timeLast);
      timeLast = unixTimeCRT[i];
    }
    h1TimeUSB->SetLineColor(kRed);  
    h1TimeUSB->SetLineWidth(2);
    h1TimeUSB->SetMinimum(0.0);
    h1TimeCRT->SetLineWidth(2);
    h1TimeCRT->SetMinimum(0.0);
    h1TimeUSB->GetXaxis()->SetTitle("time, h");
    h1TimeCRT->GetXaxis()->SetTitle("time, h");
    //////////////////////////////////////////

    for(i = 0; i < N ; i ++ ) {
      eventUsageCRT[i] = 0;
      eventUsageUSB[i] = 0;
    }    
    
    //LB 06.02.2011
    //Int_t nRunTimeBin = nUnixTimeBin/4; //number of run time (X) bin in the h2MindTvsTime
    if(nSecBin<1){
      cout<<" ERROR --> nSecBin<1"<<endl
	  <<" nSecBin = "<<nSecBin<<endl;
      assert(0);
    }
    Int_t nRunTimeBin = (Int_t)((tMaxOverlap - tMinOverlap)/nSecBin);
    if(nRunTimeBin < 1.0){
      cout<<" ERROR --> nRunTimeBin < 1.0"<<endl;
      printf(" tMaxOverlap = %20.5f \n",tMaxOverlap);
      printf(" tMinOverlap = %20.5f \n",tMinOverlap);
      printf(" dt          = %20.5f \n",(tMaxOverlap - tMinOverlap));
      cout<<"           sec/bin     = "<<nSecBin<<" c"<<endl;
      assert(0);
    }    
    printf(" tMinOverlap = %20.5f s \n",tMinOverlap);
    printf(" tMaxOverlap = %20.5f s \n",tMaxOverlap);
    printf(" dt          = %20.5f s \n",(tMaxOverlap - tMinOverlap));
    printf(" dt          = %20.5f h \n",((tMaxOverlap - tMinOverlap)/3600));
    cout<<" nRunTimeBin = "<<nRunTimeBin<<endl;
    cout<<" nSecBin     = "<<nSecBin<<" s"<<endl;

    // min time diff between CTR and USB
    TH1D *h1MindT = new TH1D("h1MindT","Min dT",1000,-20.0,20.0);  
    TH2D *h2MindTvsTime_ = new TH2D("h2MindTvsTime_","Min dT vs time",
				    nRunTimeBin, tMinOverlap, tMaxOverlap,ndTimeBin,dTmin,dTmax);
    
    Double_t mindT = 1000000.0;
    Int_t jentryCRTMin = 0;
    
    cout<<"--- TEST Merging ---"<<endl;

    Int_t nMergedEvents = 0;

    //cout<<"nEvUSB = "<<nEvUSB<<endl;

    Int_t nFillTree = 0;//number of the filling tree

    timeLast = 0.0;
    for(i = 0;i<nEvUSB;i++){

      for(j = jentryCRTMin;j<nEvCRT;j++){
	if(TMath::Abs(unixTimeUSB[i] - unixTimeCRT[j])<mindT){
	  mindT = TMath::Abs(unixTimeUSB[i] - unixTimeCRT[j]);
	  jentryCRTMin = j;
	}
	if((unixTimeCRT[j] - unixTimeUSB[i])>10.0)
	  j = nEvCRT - 1;
      }
      mindT = 10000000.0;

      if(i%10000 == 0)
	cout<<"merging "<<i<<endl;

      //if CRT and USBWC events are mearged 
      if( timingCRT(_h2MindTvsTime,unixTimeUSB[i], unixTimeCRT[jentryCRTMin]) ){

	////////////////merg DAQ here/////////////////////

	//CRT
	crt->LoadTree(jentryCRTMin);
	crt->GetEntry(jentryCRTMin);
	crt->GetDataToMerge();

	//USB
	usb->LoadTree(i);
	usb->GetEntry(i);

	h1crtEvIDMerg->SetBinContent(i+1,jentryCRTMin);
	
	//if(crt->_hodoStatus==true){

	eventUsageUSB[i]++;
	eventUsageCRT[jentryCRTMin]++;	
	nMergedEvents++;
	
	if((unixTimeUSB[i] - usb->UnixTime)!=0){
	  cout<<endl<<"  ERROR --> "<<endl
	      <<"(unixTimeUSB[i] - usb->UnixTime)!=0 "<<endl
	      <<"(unixTimeUSB[i] - usb->UnixTime) =  "<<(unixTimeUSB[i] - usb->UnixTime)<<endl;
	  assert(0);
	}	  
	if((unixTimeCRT[jentryCRTMin] - crt->time_CRT)!=0){
	  cout<<endl<<"  ERROR --> "<<endl
	      <<"(unixTimeCRT[jentryCRTMin] - crt->time_CRT)!=0 "<<endl
	      <<"(unixTimeCRT[jentryCRTMin] - crt->time_CRT) =  "<<(unixTimeCRT[jentryCRTMin] - crt->time_CRT)<<endl;
	  assert(0);
	}
	
	h1MindT->Fill((unixTimeUSB[i] - unixTimeCRT[jentryCRTMin]));
	h2MindTvsTime_->Fill(unixTimeUSB[i],(unixTimeUSB[i] - unixTimeCRT[jentryCRTMin]));	
	
	h1TimeUSBCRT->Fill((usb->UnixTime - tMin)/3600.0);
	h1dTimeConsUSBCRT->Fill(usb->UnixTime - timeLast);
	h1dTimeConsUSBCRT_short->Fill(usb->UnixTime - timeLast);
	timeLast = usb->UnixTime;
	
	nFillTree++;

	
	//} //if(crt->_hodoStatus==true){
      }
    }
    /////////////////////////////////////////// 

    TFile *hfile = new TFile(outputMergRootFile.Data(), "RECREATE", "data from CRT and USBWC", 1);
    if (hfile->IsZombie()) assert(0);

    h1TimeCRT->Write();
    h1TimeUSB->Write();

    h1MindT->Write();
    h2MindTvsTime_->Write();

    h1TimeUSBCRT->SetLineColor(kBlue);
    h1TimeUSBCRT->SetLineWidth(2);
    h1TimeUSBCRT->SetMinimum(0.0);
    h1TimeUSBCRT->GetXaxis()->SetTitle("time, h");
    h1TimeUSBCRT->Write();

    h1dTimeConsUSB->Write();
    h1dTimeConsCRT->Write();
    h1dTimeConsUSBCRT->Write();
    h1dTimeConsUSB_short->Write();
    h1dTimeConsCRT_short->Write();
    h1dTimeConsUSBCRT_short->Write();

    h1crtEvIDMerg->Write();
    
    TH1I *h1EventUsageCRT = 
      new TH1I( "h1EventUsageCRT", "Usage of CRT events", 11, -0.5, 10.5 );
    
    for(i = 0;i<nEvCRT;i++){
      h1EventUsageCRT->Fill( eventUsageCRT[ i ] );
      if( eventUsageCRT[ i ] > 1 ) {
	cout << "Warning: CRT event " << i 
	     << "is used in " << eventUsageCRT[ i ]
	     << " merged events" << endl;
      }
    }
    
    h1EventUsageCRT->Write();
    
    TH1I *h1EventUsageUSB = 
      new TH1I( "h1EventUsageUSB", "Usage of USB events", 11, -0.5, 10.5 );
    
    for(i = 0;i<nEvUSB;i++){
      h1EventUsageUSB->Fill( eventUsageUSB[ i ] );
      if( eventUsageUSB[ i ] > 1 ) {
	cout << "ERROR: USB event " << i 
	     << "is used in " << eventUsageUSB[ i ]
	     << " merged events" << endl;
	assert(0);
      }
    }
    
    h1EventUsageUSB->Write();
    
    hfile->Close();
    cout<<" MERGING TEST"<<endl
	<<" 'nFillTree' = "<<nFillTree<<endl
	<<"The output ntuple would contains: "<<endl 
	<<"     "<<nMergedEvents<< " merged CRT+USB events"<<endl;

    cout<<"  //// --> merging test 'fast merging' <-- \\\\"<<endl
	<<"  //// =======          END        ======= \\\\"<<endl<<endl;
    
    delete usb;
    delete crt;
  } // if(argc == 6 && atoi(argv[1]) == 2){
  else{
    cout<<endl
	<<"  ERROR ---> input arguments are wrong "<<endl
      //LB 04.02.2011
      //<<"  --> (merging of the data) <-- "<<endl
      //<<"             [1] : mode ID 0 "<<endl
      //<<"             [2] : root file list with USB data"<<endl
      //<<"             [3] : root file list with CRT data"<<endl
      //<<"             [4] : root file with timing histo"<<endl
      //<<"             [5] : output root file with data from CRT and USB"<<endl
	<<"  --> (merging of the data with new data structure) <-- "<<endl
	<<"             [1]  : mode ID 0 "<<endl
	<<"             [2]  : root file list with USB data"<<endl
	<<"             [3]  : root file list with CRT data"<<endl
	<<"             [4]  : root file with timing histo"<<endl
	<<"             [5]  : output root file with data from CRT and USB"<<endl;
    cout<<"             [6]  : n splines "<<endl
	<<"             [7]  : signalThreshold, mV"<<endl
	<<"             [8]  : crossTalkThreshold, mV"<<endl
	<<"             [9]  : mpFraction"<<endl
	<<"             [10] : cfdVAL"<<endl;
    cout<<"  --> (timing histogram) <-- "<<endl
	<<"             [1] : mode ID 1 "<<endl
	<<"             [2] : root file list with USB data"<<endl
	<<"             [3] : root file list with CRT data"<<endl
	<<"             [4] : output root file with time histograms"<<endl;
    cout<<"  --> (merging test 'fast merging' ) <-- "<<endl
	<<"             [1]  : mode ID 2 "<<endl
	<<"             [2]  : root file list with USB data"<<endl
	<<"             [3]  : root file list with CRT data"<<endl
	<<"             [4]  : root file with timing histo"<<endl
	<<"             [5]  : root file with output histograms"<<endl;
    cout<<"             [6]  : n splines "<<endl
	<<"             [7]  : signalThreshold, mV"<<endl
	<<"             [8]  : crossTalkThreshold, mV"<<endl
	<<"             [9]  : mpFraction"<<endl
	<<"             [10] : cfdVAL"<<endl;
  }

}


Bool_t timingCRT(TH2D *h2, Double_t uTusb, Double_t uTcrt){

  Int_t i = 0;

  Double_t unixTimeL = -999.0;
  Double_t unixTimeR = -999.0;

  Double_t maxNumdT = 0.0;

  Double_t dTmaxProb = -999.0;

  Double_t dt = uTusb - uTcrt;

  Int_t nh2binX = h2->GetNbinsX();
  Int_t nh2binY = h2->GetNbinsY();

  Int_t unixTimebinID = -999;

  //Double_t unixTimeS = h2->GetXaxis()->GetBinLowEdge(1);
  //Double_t unixTimeE = h2->GetXaxis()->GetBinLowEdge(nh2binX) 
  //                 + h2->GetXaxis()->GetBinWidth(nh2binX);

  //Double_t dTmin = h2->GetYaxis()->GetBinLowEdge(1);
  //Double_t dTmax = h2->GetYaxis()->GetBinLowEdge(nh2binY) 
  //             + h2->GetYaxis()->GetBinWidth(nh2binY);

  //TH1D *hh = new TH1D("hh","delta time",nh2binY,dTmin,dTmax);

  //find within which time bin the uTusb is
  for(i = 1;i<=nh2binX;i++){
    unixTimeL = h2->GetXaxis()->GetBinLowEdge(i);
    unixTimeR = unixTimeL + h2->GetXaxis()->GetBinWidth(i);
    if(uTusb>=unixTimeL && uTusb<unixTimeR){
      unixTimebinID = i;
      break;
    }
  }

  //find maximum probable dT from 2D histogram
  for(i = 1;i<=nh2binY;i++){
    if(maxNumdT < h2->GetBinContent(unixTimebinID,i)){
      maxNumdT = h2->GetBinContent(unixTimebinID,i);
      dTmaxProb = h2->GetYaxis()->GetBinCenter(i);
    }
  }
  
  //cout<<"maxNumdT  "<<maxNumdT<<endl
  //<<"dTmaxProb "<<dTmaxProb<<endl;
  
  if( TMath::Abs(dTmaxProb - dt)<maxdTcut){
    return true;
  }

  return false;
}
