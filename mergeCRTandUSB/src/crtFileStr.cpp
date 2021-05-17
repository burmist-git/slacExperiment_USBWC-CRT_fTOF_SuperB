//my
#include "crtFileStr.hh"
#include "crtBox.hh"
#include "crtTrk.hh"
#include "../anaConst.hh"

//root
#include <TH2.h>
#include <TF1.h>
#include <TH1D.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TMath.h>
#include <TLine.h>
#include <TVector3.h>
#include <TPaveText.h>
#include <TRandom3.h>

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

crtFileStr::crtFileStr(){

  _rnd = new TRandom3(12312);

  Double_t dx = 29.3;//cm
  Double_t dy = 4.2; //cm
  Double_t dz = 1.5; //

  //fTOF
  Double_t x0 = 0.55;//cm
  Double_t y0 = 0.0; //cm
  Double_t z0 = 83.5 + 38.1 + 1.49 +  1.5 + 1.64 + 1.5/2.0;//cm
  TVector3 V0_fTOF(x0,y0,z0);

  _boxfTOF = new crtBox(dx,dy,dz);
  _boxfTOF->SetCenter(V0_fTOF);
  _boxfTOF->RotateY(0.0);
  //_boxfTOF->RotateZ(45.0);

  TVector3 V0_fTOF_BOT(0.55,0.0,123.1 + 1.5/2.0);
  _boxfTOF_Bot = new crtBox(dx,dy,dz);
  _boxfTOF_Bot->SetCenter(V0_fTOF_BOT);
  _boxfTOF_Bot->RotateY(0.0);

  TVector3 V0_QuartzS(4.007,0.0,(16.367+83.5));
  _boxQuartzS = new crtBox(dx,dy,dz);
  _boxQuartzS->SetCenter(V0_QuartzS);
  _boxQuartzS->RotateY(-43.0);

}

crtFileStr::~crtFileStr(){
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

void crtFileStr::addFilesToChain(string name){
  cout<<endl<<endl<<name<<endl;
  ifstream indata;
  string rootFileName;
  TChain *theChain = new TChain("T");
  indata.open(name.c_str()); 
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

void crtFileStr::GetUnixTime(Double_t *unixTimeCRT, Int_t Nmax, Int_t &nEv){
  Double_t timeBefor = 0.0;
  Long64_t nentries = fChain->GetEntriesFast();
  nEv = nentries;
  if(nentries>Nmax){
    cout<<endl
	<<" ERROR ---> nentries>Nmax "<<endl
	<<"            nentries = "<<nentries<<endl
	<<"            Nmax     = "<<Nmax<<endl;
    assert(0);
  }
  cout<<" ---- crtFileStr::GetUnixTime ---- "<<nentries<<endl;
  Long64_t jentry = 0;
  for (jentry=0; jentry<nentries;jentry++) {
    LoadTree(jentry);
    GetEntry(jentry);
    if(jentry%10000==0)
      cout<<jentry<<endl;
    if(jentry>Nmax){
      cout<<endl
	  <<" ERROR ---> jentry>Nmax "<<endl;
      assert(0);
    }
    if((time_CRT-timeBefor)<0){
      cout<<endl
	  <<" root files in root data list are not ordered in time "<<endl
	  <<fChain->GetCurrentFile()->GetName()<<endl;
      assert(0);
    }
    unixTimeCRT[jentry] = time_CRT;
    timeBefor = time_CRT;
  }
  cout<<" -------------------- "<<nentries<<endl;
  //cout<<jentry<<"  "<<time_CRT<<endl;
}

Bool_t crtFileStr::CheckDistInTime(){

  cout<<endl<<" Check ordering in time"<<endl
      <<"   crtFileStr::CheckDistInTime  "<<endl;
  Double_t timeOld = 0;

  Long64_t nentries = fChain->GetEntriesFast();
  cout<<" total number of event "<<nentries<<endl;
  Long64_t jentry = 0;
  LoadTree(jentry);
  GetEntry(jentry);
  //b_time->GetEntry(jentry);
  timeOld = time_CRT;
  for (jentry=0; jentry<nentries;jentry++) {
    LoadTree(jentry);
    GetEntry(jentry);
    if(jentry%100000==0)
      cout<<" processed events "<<jentry<<endl;
    //b_time->GetEntry(jentry);
    /*
    cout<<endl<<"jentry    "<<jentry<<endl
	<<"time_CRT  "<<time_CRT<<endl
	<<"timeOld   "<<timeOld<<endl
	<<"(timeOld - time_CRT)  "<<(timeOld - time_CRT)<<endl;
    printf(" timeOld  = %20.5f \n", timeOld);
    printf(" time_CRT = %20.5f \n", time_CRT);
    printf(" (timeOld - time_CRT) = %20.5f \n" , (timeOld - time_CRT));
    */
    if((timeOld - time_CRT)>0){
      cout<<endl
	  <<" ERROR ---> events are not ordered in time "<<endl
	  <<" file : "<<fChain->GetCurrentFile()->GetName()<<endl
	  <<"jentry    "<<jentry<<endl;
      printf(" timeOld  = %20.5f \n", timeOld);
      printf(" time_CRT = %20.5f \n", time_CRT);

      assert(0);
    }
    timeOld = time_CRT;
  }

  return true;

  /*
  Long64_t nbytes = 0, nb = 0;

  Long64_t jentry = 0;
  Long64_t ientry = 0;


  jentry = 998;
  ientry = LoadTree(jentry);
  nb = b_time->GetEntry(jentry);   nbytes += nb;
  printf(" %10d time_CRT  = %20.5f \n", 998,time_CRT);

  jentry = 999;
  ientry = LoadTree(jentry);
  nb = b_time->GetEntry(jentry);   nbytes += nb;
  printf(" %10d time_CRT  = %20.5f \n", 999,time_CRT);

  jentry = 1000;
  ientry = LoadTree(jentry);
  nb = b_time->GetEntry(jentry);   nbytes += nb;
  printf(" %10d time_CRT  = %20.5f \n", 1000,time_CRT);

  jentry = 1001;
  ientry = LoadTree(jentry);
  nb = b_time->GetEntry(jentry);   nbytes += nb;
  printf(" %10d time_CRT  = %20.5f \n", 1001,time_CRT);
  */

  /*
  LoadTree(998);
  b_time->GetEntry(998);


  LoadTree(999);
  b_time->GetEntry(999);
  printf(" %10d time_CRT  = %20.5f \n", 999,time_CRT);

  LoadTree(1000);
  b_time->GetEntry(1000);
  printf(" %10d time_CRT  = %20.5f \n", 1000,time_CRT);

  LoadTree(1001);
  b_time->GetEntry(1001);
  printf(" %10d time_CRT  = %20.5f \n", 1001,time_CRT);

  LoadTree(1002);
  b_time->GetEntry(1002);
  printf(" %10d time_CRT  = %20.5f \n", 1002,time_CRT);
  */  

}

Int_t crtFileStr::GetDataRate(){

  ///////////////TIME//////////////
  time_t rawtime;
  tm *ptm;
  Int_t crtTimeInt;
  //Int_t locTime;
  /////////////////////////////////

  Long64_t nentries = fChain->GetEntriesFast();
  Int_t nentriesInt = nentries;
  LoadTree(0);
  GetEntry(0);
  Double_t timeBeg = time_CRT;
  LoadTree((nentries-1));
  GetEntry((nentries-1));
  Double_t timeEnd = time_CRT;
  
  _timeStopUnix = timeEnd;
  _timeStartUnix = timeBeg;

  Double_t dT = (timeEnd - timeBeg);
  Double_t Rate = -999.0;
  
  if(dT>0)
    Rate = nentries/dT;

  _Rate = Rate;

  cout<<endl<<"--------- CRT timing information ----"<<endl
      <<" RATE = "<<setw(10)<<Rate<<"  ev/s"<<endl;
  printf("timeBeg = %20.4f \n", timeBeg);
  printf("timeEnd = %20.4f \n", timeEnd);
  cout<<" total run time:          "<<setw(10)<<dT/3600.0<<" hours"<<endl
      <<" total number of entries: "<<setw(10)<<nentries<<endl;

  crtTimeInt = (Int_t)timeBeg;
  rawtime = crtTimeInt;
  /*
  ptm = gmtime ( &rawtime );    
  locTime = (ptm->tm_hour);
  printf ("GMT time START run :  %2d.%2d.%4d   %2d:%02d:%02d\n", ptm->tm_mday,((ptm->tm_mon)+1),((ptm->tm_year)+1900),(ptm->tm_hour)%24, ptm->tm_min,ptm->tm_sec);

  _timeStart_hour = (ptm->tm_hour)%24;
  _timeStart_min  = ptm->tm_min;
  */

  ptm = localtime( &rawtime );
  printf( "\nLocal time START = %s", asctime( ptm ) );

  crtTimeInt = (Int_t)timeEnd;
  rawtime = crtTimeInt;

  /*
  ptm = gmtime ( &rawtime );    
  locTime = (ptm->tm_hour);
  printf ("GMT time END   run :  %2d.%2d.%4d   %2d:%02d:%02d\n", ptm->tm_mday,((ptm->tm_mon)+1),((ptm->tm_year)+1900),(ptm->tm_hour)%24, ptm->tm_min,ptm->tm_sec);
  */

  ptm = localtime( &rawtime );
  printf( " Local time STOP = %s\n", asctime( ptm ) );

  cout<<"-----------------------------"<<endl;

  //cout<<"  _timeStart_hour  "<<_timeStart_hour<<endl
  //  <<"  _timeStart_min   "<<_timeStart_min<<endl;
  //cout<<"timeBeg  = "<<timeBeg<<endl
  //  <<"timeEnd  = "<<timeEnd<<endl
  //  <<"dT       = "<<dT<<endl
  //  <<"nentries = "<<nentries<<endl
  //  <<"Rate     = "<<Rate<<endl;

  return nentriesInt;
}

void crtFileStr::Loop(TString outputRootHistFile){

  /////////////////////////////
  GetDataRate();
  /////////////////////////////

  Int_t i = 0;
  Int_t k = 0;
  Double_t prevUnixTime = 0.0;

  //trigger
  TH1D *h1Trig1A[4];
  TH1D *h1Trig2A[4];
  TH1D *h1Trig1T[4];
  TH1D *h1Trig2T[4];

  //Stack counters
  TH1D *h1StackCounter1A[4];
  TH1D *h1StackCounter2A[4];
  TH1D *h1StackCounter3A[4];
  TH1D *h1StackCounter4A[4];
  TH1D *h1StackCounter1T[4];
  TH1D *h1StackCounter2T[4];
  TH1D *h1StackCounter3T[4];
  TH1D *h1StackCounter4T[4];

  //start
  TH1D *h1Start1T[4];
  TH1D *h1Start1A[4];
  TH2D *h2Start1TvsA[4];

  //Hodoscope
  TH1D *h1Hodo1XT = new TH1D("h1Hodo1XT","Hodoscope1 X TDC",100,0.5,100.0); 
  TH1D *h1Hodo2XT = new TH1D("h1Hodo2XT","Hodoscope2 X TDC",100,0.5,100.0); 
  TH1D *h1Hodo1YT = new TH1D("h1Hodo1YT","Hodoscope1 Y TDC",100,0.5,100.0); 
  TH1D *h1Hodo2YT = new TH1D("h1Hodo2YT","Hodoscope2 Y TDC",100,0.5,100.0); 

  TH1D *h1Hodo1XarrT[55];
  TH1D *h1Hodo2XarrT[55];
  TH1D *h1Hodo1YarrT[27];
  TH1D *h1Hodo2YarrT[27];

  // The use of this variable is commented below
  // => Comment its declaration as well
  //Int_t hod2x_ch1 = 0;

  Int_t minBinST = 50;
  Int_t maxBinST = 1300;

  Int_t nStack1 = 0;
  Int_t nStack2 = 0;
  Int_t nStack3 = 0;
  Int_t nStack4 = 0;

  TH2D *h2Hodo1XTvsTime = new TH2D("h2Hodo1XTvsTime"," Hodo1XT vs Time ",100,_timeStartUnix,_timeStopUnix,100,0.0,500);
  TH2D *h2Hodo2XTvsTime = new TH2D("h2Hodo2XTvsTime"," Hodo2XT vs Time ",100,_timeStartUnix,_timeStopUnix,100,0.0,500);
  TH2D *h2Hodo1YTvsTime = new TH2D("h2Hodo1YTvsTime"," Hodo1YT vs Time ",100,_timeStartUnix,_timeStopUnix,100,0.0,500);
  TH2D *h2Hodo2YTvsTime = new TH2D("h2Hodo2YTvsTime"," Hodo2YT vs Time ",100,_timeStartUnix,_timeStopUnix,100,0.0,500);
  
  TH2D *h2HodoX1vsX2 = new TH2D("h2HodoX1vsX2"," X1 vs X2 ", 100, -55.5, 55.5, 100, -55.5, 55.5);
  TH2D *h2HodoY1vsY2 = new TH2D("h2HodoY1vsY2"," Y1 vs Y2 ", 100, -27.5, 27.5, 100, -27.5, 27.5);

  TH2D *h2Hodo1YvsX = new TH2D("h2Hodo1YvsX"," Hodo1 YvsX ",100,-55.5,55.5,100,-27.5,27.5);
  TH2D *h2Hodo2YvsX = new TH2D("h2Hodo2YvsX"," Hodo2 YvsX ",100,-55.5,55.5,100,-27.5,27.5);

  TH2D *h2_YintvsXint_fTOF = new TH2D("h2_YintvsXint_fTOF"," Yint vs Xint fTOF ", 400, -30.0, 30.0, 400, -30.0, 30.0);
  TH2D *h2_YintvsXint_Quartz = new TH2D("h2_YintvsXint_Quartz"," Yint vs Xint Quartz start counter ", 100, -20.0, 20.0, 100, -20.0, 20.0);

  TH1D *h1_Xint_Quartz = new TH1D("h1_Xint_Quartz"," Xint Quartz start counter ", 100, -20.0,  20.0);
  TH1D *h1_Yint_Quartz = new TH1D("h1_Yint_Quartz"," Yint Quartz start counter ", 100, -10.0,  10.0);
  TH1D *h1_Zint_Quartz = new TH1D("h1_Zint_Quartz"," Zint Quartz start counter ", 100,  80.0, 120.0);

  TH1D *h1_Xint_fTOF = new TH1D("h1_Xint_fTOF"," Xint fTOF ", 100, -20.0,  20.0);
  TH1D *h1_Yint_fTOF = new TH1D("h1_Yint_fTOF"," Yint fTOF ", 100, -10.0,  10.0);

  // NA 2010/11/03: fix the compilation warning coming from the fact that the number of bins 
  // should be an integer
  /*
  Double_t timeRun = (_timeStopUnix - _timeStartUnix)/3600;
  Int_t nTimeBin = (timeRun + 24.0)*3.0; // one time bin is 20 min

  TH1D *h1Hodo1XT_Time = new TH1D("h1Hodo1XT_Time"," Hodo1XT Time dist ", nTimeBin, 0.0, (timeRun + 24.0));
  TH1D *h1Hodo2XT_Time = new TH1D("h1Hodo2XT_Time"," Hodo2XT Time dist ", nTimeBin, 0.0, (timeRun + 24.0));
  TH1D *h1Hodo1YT_Time = new TH1D("h1Hodo1YT_Time"," Hodo1YT Time dist ", nTimeBin, 0.0, (timeRun + 24.0));
  TH1D *h1Hodo2YT_Time = new TH1D("h1Hodo2YT_Time"," Hodo2YT Time dist ", nTimeBin, 0.0, (timeRun + 24.0));
  */
  
  // NA 2010/11/06: no need to add 24 hours!
  //Double_t timeRun = ceil( (_timeStopUnix - _timeStartUnix)/3600.0 ) + 24.0;
  Double_t timeRun = ceil( (_timeStopUnix - _timeStartUnix)/3600.0 );
  Int_t nTimeBin = (Int_t)timeRun * 3;
  
  /*
  TH1D *h1Hodo1XT_Time = new TH1D("h1Hodo1XT_Time"," Hodo1XT Time dist ", nTimeBin, 0.0, timeRun );
  TH1D *h1Hodo2XT_Time = new TH1D("h1Hodo2XT_Time"," Hodo2XT Time dist ", nTimeBin, 0.0, timeRun );
  TH1D *h1Hodo1YT_Time = new TH1D("h1Hodo1YT_Time"," Hodo1YT Time dist ", nTimeBin, 0.0, timeRun );
  TH1D *h1Hodo2YT_Time = new TH1D("h1Hodo2YT_Time"," Hodo2YT Time dist ", nTimeBin, 0.0, timeRun );
  */
  TH1D *h1Hodo1XT_Time = new TH1D("h1Hodo1XT_Time"," Hodo1XT Time dist ", nTimeBin, _timeStartUnix + timeShiftForProperDisplay, _timeStartUnix + timeShiftForProperDisplay + 3600.0 * timeRun );
  TH1D *h1Hodo2XT_Time = new TH1D("h1Hodo2XT_Time"," Hodo2XT Time dist ", nTimeBin, _timeStartUnix + timeShiftForProperDisplay, _timeStartUnix + timeShiftForProperDisplay + 3600.0 * timeRun );
  TH1D *h1Hodo1YT_Time = new TH1D("h1Hodo1YT_Time"," Hodo1YT Time dist ", nTimeBin, _timeStartUnix + timeShiftForProperDisplay, _timeStartUnix + timeShiftForProperDisplay + 3600.0 * timeRun );
  TH1D *h1Hodo2YT_Time = new TH1D("h1Hodo2YT_Time"," Hodo2YT Time dist ", nTimeBin, _timeStartUnix + timeShiftForProperDisplay, _timeStartUnix + timeShiftForProperDisplay + 3600.0 * timeRun );

  // NA: the lines below make nice time displays on the histogram X axis

  h1Hodo1XT_Time->GetXaxis()->SetNdivisions( 405 );
  h1Hodo1XT_Time->GetXaxis()->SetTimeDisplay(1);
  h1Hodo1XT_Time->GetXaxis()->SetLabelSize( 0.03 );
  h1Hodo1XT_Time->GetXaxis()->SetTimeFormat( "%m/%d %H:%M" );
  h1Hodo1XT_Time->GetXaxis()->SetTitle( "Time" );

  h1Hodo2XT_Time->GetXaxis()->SetNdivisions( 405 ); 
  h1Hodo2XT_Time->GetXaxis()->SetTimeDisplay(1); 
  h1Hodo2XT_Time->GetXaxis()->SetLabelSize( 0.03 ); 
  h1Hodo2XT_Time->GetXaxis()->SetTimeFormat( "%m/%d %H:%M" ); 
  h1Hodo2XT_Time->GetXaxis()->SetTitle( "Time" ); 

  h1Hodo1YT_Time->GetXaxis()->SetNdivisions( 405 ); 
  h1Hodo1YT_Time->GetXaxis()->SetTimeDisplay(1); 
  h1Hodo1YT_Time->GetXaxis()->SetLabelSize( 0.03 ); 
  h1Hodo1YT_Time->GetXaxis()->SetTimeFormat( "%m/%d %H:%M" ); 
  h1Hodo1YT_Time->GetXaxis()->SetTitle( "Time" ); 

  h1Hodo2YT_Time->GetXaxis()->SetNdivisions( 405 ); 
  h1Hodo2YT_Time->GetXaxis()->SetTimeDisplay(1); 
  h1Hodo2YT_Time->GetXaxis()->SetLabelSize( 0.03 ); 
  h1Hodo2YT_Time->GetXaxis()->SetTimeFormat( "%m/%d %H:%M" ); 
  h1Hodo2YT_Time->GetXaxis()->SetTitle( "Time" ); 
  
  //LB 13.11.2010
  //Test of different reconstruction codes
  //TH1D *h1dX1 = new TH1D("h1dX1"," dX1 ",100,-3.0,3.0);
  //TH1D *h1dX2 = new TH1D("h1dX2"," dX2 ",100,-3.0,3.0);
  //TH1D *h1dY1 = new TH1D("h1dY1"," dY1 ",100,-3.0,3.0);
  //TH1D *h1dY2 = new TH1D("h1dY2"," dY2 ",100,-3.0,3.0);
  //TH1D *h1dXfTOFint = new TH1D("h1dXfTOFint"," dXfTOFint ",100,-3.0,3.0);
  //TH1D *h1dYfTOFint = new TH1D("h1dYfTOFint"," dYfTOFint ",100,-3.0,3.0);
  //TH1D *h1dXQuartzint = new TH1D("h1dXQuartzint"," dXQuartzint ",100,-3.0,3.0);
  //TH1D *h1dYQuartzint = new TH1D("h1dYQuartzint"," dYQuartzint ",100,-3.0,3.0);
  //TH1D *h1dZQuartzint = new TH1D("h1dZQuartzint"," dZQuartzint ",100,-3.0,3.0);

  TH1D *h1Hodo1X = new TH1D("h1Hodo1X"," Hodo1 X ",111*10,-55.5,55.5);
  TH1D *h1Hodo2X = new TH1D("h1Hodo2X"," Hodo2 X ",111*10,-55.5,55.5);
  TH1D *h1Hodo1Y = new TH1D("h1Hodo1Y"," Hodo1 Y ",55*10,-27.5,27.5);
  TH1D *h1Hodo2Y = new TH1D("h1Hodo2Y"," Hodo2 Y ",55*10,-27.5,27.5);

  TH1D *h1Hodo1XT_hit = new TH1D("h1Hodo1XT_hit"," Distribution of the channels hit in hodo1X ", 55, -0.5, 54.5); 
  TH1D *h1Hodo2XT_hit = new TH1D("h1Hodo2XT_hit"," Distribution of the channels hit in hodo2X ", 55, -0.5, 54.5); 
  TH1D *h1Hodo1YT_hit = new TH1D("h1Hodo1YT_hit"," Distribution of the channels hit in hodo1Y ", 27, -0.5, 26.5); 
  TH1D *h1Hodo2YT_hit = new TH1D("h1Hodo2YT_hit"," Distribution of the channels hit in hodo2Y ", 27, -0.5, 26.5);

  //Trk
  TH1D *h1TrkDirCosX = new TH1D("h1TrkDirCosX"," Trk Direction Cos X ", 100, -0.5, 0.5);
  TH1D *h1TrkDirCosY = new TH1D("h1TrkDirCosY"," Trk Direction Cos Y ", 100, -0.5, 0.5);
  TH1D *h1TrkDirCosZ = new TH1D("h1TrkDirCosZ"," Trk Direction Cos Z ", 100, -1.01,-0.9);  
  TH2D *h2TrkDirCosYvsCosX = new TH2D("h2TrkDirCosYvsCosX"," Trk Direction Cos Y vs X", 100, -0.5, 0.5,100, -0.5, 0.5);

  TH1D *h1TrkDirCosZ_deg = new TH1D("h1TrkDirCosZ_deg"," Trk Direction Cos Z (deg)", 100,150,200);  

  TH1D *h1TrkTheta = new TH1D("h1TrkTheta"," Trk theta ", 100,150.0,190.0);
  TH1D *h1TrkPhi = new TH1D("h1TrkTPhi"," Trk phi ", 100,-190.0,190.0);

  TH1D *h1TrkMomID = new TH1D("h1TrkMomID"," Trk mom ID ", 50, 0.0,2.0);

  TH1D *h1getTrkMomID_All = new TH1D("h1getTrkMomID_All", " Trk Mom ID All my ", 5, -0.5,4.5);
  TH1D *h1getTrkMomID     = new TH1D("h1getTrkMomID"    , " Trk Mom ID my     ", 5, -0.5,4.5);
  TH1D *h1getTrkMomID_QSC = new TH1D("h1getTrkMomID_QSC", " Trk Mom ID QSC my ", 5, -0.5,4.5);

  TH1D *h1nStack1 = new TH1D("h1nStack1"," nStack1 ", 50, 0.0,5.0);
  TH1D *h1nStack2 = new TH1D("h1nStack2"," nStack2 ", 50, 0.0,5.0);
  TH1D *h1nStack3 = new TH1D("h1nStack3"," nStack3 ", 50, 0.0,5.0);
  TH1D *h1nStack4 = new TH1D("h1nStack4"," nStack4 ", 50, 0.0,5.0);

  //trigger
  initHistos_H1D(h1Trig1A, "h1Trig1A", " Trigger counter A1, ADC", 101, 0.0, 2048.0);
  initHistos_H1D(h1Trig2A, "h1Trig2A", " Trigger counter A2, ADC", 101, 0.0, 2048.0);
  initHistos_H1D(h1Trig1T, "h1Trig1T", " Trigger counter T1, TDC", 101, 0.0, 2048.0);
  initHistos_H1D(h1Trig2T, "h1Trig2T", " Trigger counter T2, TDC", 101, 0.0, 2048.0);

  //Stack countesr
  initHistos_H1D(h1StackCounter1A, "h1StackCounter1A", " Stack counter 1, ADC", 101, 0.0, 2048.0);
  initHistos_H1D(h1StackCounter2A, "h1StackCounter2A", " Stack counter 2, ADC", 101, 0.0, 2048.0);
  initHistos_H1D(h1StackCounter3A, "h1StackCounter3A", " Stack counter 3, ADC", 101, 0.0, 2048.0);
  initHistos_H1D(h1StackCounter4A, "h1StackCounter4A", " Stack counter 4, ADC", 101, 0.0, 2048.0);
  initHistos_H1D(h1StackCounter1T, "h1StackCounter1T", " Stack counter 1, TDC", 101, 0.0, 2048.0);
  initHistos_H1D(h1StackCounter2T, "h1StackCounter2T", " Stack counter 2, TDC", 101, 0.0, 2048.0);
  initHistos_H1D(h1StackCounter3T, "h1StackCounter3T", " Stack counter 3, TDC", 101, 0.0, 2048.0);
  initHistos_H1D(h1StackCounter4T, "h1StackCounter4T", " Stack counter 4, TDC", 101, 0.0, 2048.0);

  //Hodoscope
  initHistosHodoX_H1D(h1Hodo1XarrT,"h1Hodo1XarrT","hodoscope 1X TDC ",100,0.5,500.0);
  initHistosHodoX_H1D(h1Hodo2XarrT,"h1Hodo2XarrT","hodoscope 2X TDC ",100,0.5,500.0);
  initHistosHodoY_H1D(h1Hodo1YarrT,"h1Hodo1YarrT","hodoscope 1Y TDC ",100,0.5,500.0);
  initHistosHodoY_H1D(h1Hodo2YarrT,"h1Hodo2YarrT","hodoscope 2Y TDC ",100,0.5,500.0);

  //START Quartz counter
  initHistos_H1D(h1Start1T,"h1Start1T"," Quartz start counter TDC ",200,600,1300);
  initHistos_H1D(h1Start1A,"h1Start1A"," Quartz start counter ADC ",101,0.0,500);

  initHistos_H2D(h2Start1TvsA,"h2Start1TvsA"," Quartz start counter TDC vs ADC",100,0.0,300.0,100,400,600);

  Int_t _hodoStatusIsOK = 0;
  Int_t quartzStartIsOk = 0;

  Int_t dTstartCounter[4];

  dTstartCounter[0] = 438;
  dTstartCounter[1] = 243;
  dTstartCounter[2] = 403;
  dTstartCounter[3] = 626;

  //////UNIX TIME/////////
  TH1D *h1dUnixTime = new TH1D("h1dUnixTime","delta Unix Time CRT",400,0.0,40.0);
  h1dUnixTime->GetXaxis()->SetTitle( "Seconds" );
  TF1 *f1expo = new TF1("f1expo","expo",1.5,30.0);
  
  // Fix the warning due to the ambiguous definition of ntimeBin
  /*
  Int_t ntimeBin = (Int_t)(_timeStopUnix - _timeStartUnix)/3600.0;
  if(ntimeBin == 0)
    ntimeBin = 1;
  TH1D *h1dUnixTimeInTime = new TH1D("h1dUnixTimeInTime","dUnix Time (between neighbor events) as function of unix time",ntimeBin,0.0,(_timeStopUnix - _timeStartUnix)/3600.0);
  */
  Int_t ntimeBin = (Int_t)( ceil((_timeStopUnix - _timeStartUnix)/3600.0) );
  if(ntimeBin == 0) { ntimeBin = 1; }
  TH1D *h1dUnixTimeInTime = 
    new TH1D("h1dUnixTimeInTime",
	     "dUnix Time (between neighbor events) as function of unix time",
	     ntimeBin,0.0,ceil( (_timeStopUnix - _timeStartUnix)/3600.0) );
  h1dUnixTimeInTime->GetXaxis()->SetTitle( "Hours" );
  
  h1dUnixTimeInTime->SetMaximum(15.0);
  h1dUnixTimeInTime->SetMinimum(0.0);
  TH1D *h1dUnixTime1hour = new TH1D("h1dUnixTime1hour"," dUnix Time (between neighbor events) in one hour",100,0.0,30.0);
  Double_t uTimeStart = -999.0;
  Double_t uTimeStop = -999.0;
  Double_t uTimeCenter;
  Double_t h1dUnixTimeInTime_LowEdge;
  Double_t h1dUnixTimeInTime_UpEdge;
  Double_t halfLifeTime;
  Double_t halfLifeTimeErr;
  ///////////////  

  //LB 13.11.2010
  //Test of different reconstruction codes
  //Double_t x1_K;
  //Double_t x1_M;
  //Double_t x2_K;
  //Double_t x2_M;
  //Bool_t hodoStatus_K;
  //Bool_t hodoStatus_M;
  //Double_t y1_K;
  //Double_t y1_M;
  //Double_t y2_K;
  //Double_t y2_M;
  //Double_t xIntFtof_K;
  //Double_t xIntFtof_M;
  //Double_t yIntFtof_K;
  //Double_t yIntFtof_M;
  //Double_t xIntQuartz_K;
  //Double_t xIntQuartz_M;
  //Double_t yIntQuartz_K;
  //Double_t yIntQuartz_M;
  //Double_t zIntQuartz_K;
  //Double_t zIntQuartz_M;


  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  Double_t nentriesD = nentries;
  cout<<"======================="<<endl;
  cout<<" total number of event "<<nentries<<endl;
  cout<<"======================="<<endl;
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    if(jentry%100000==0)
      cout<<" processed events "<<jentry<<endl;
    
    //if(jentry==100000)
    //break;

    GetDataFromDetectors();
    GetRelativeRunTime();
    GetHodoscopePositions();
    //GetHodoscopePositionsMyAlgo();
    GetTrkDirectionCos();

    //LB 13.11.2010
    //Test of different reconstruction codes
    //GetHodoscopePositions();
    //hodoStatus_K = _hodoStatus;
    //x1_K = _x1;
    //x2_K = _x2;
    //y1_K = _y1;
    //y2_K = _y2;
    //GetHodoscopePositionsMyAlgo();        
    //hodoStatus_M = _hodoStatus;
    //x1_M = _x1;
    //x2_M = _x2;
    //y1_M = _y1;
    //y2_M = _y2;
    //if(hodoStatus_K != hodoStatus_M){
    //cout<<"hodoStatus_K != hodoStatus_M"<<endl;
    //}
    //if(hodoStatus_K){
    //h1dX1->Fill(x1_M - x1_K);
    //h1dX2->Fill(x2_M - x2_K);
    //h1dY1->Fill(y1_M - y1_K);
    //h1dY2->Fill(y2_M - y2_K);      
    //_trk = new crtTrk( x1_K, y1_K, x2_K, y2_K);
    //_boxfTOF->GetIntersection(_trk);
    //_boxQuartzS->GetIntersection(_trk);
    //xIntFtof_K = _boxfTOF->_xInt;
    //yIntFtof_K = _boxfTOF->_yInt;
    //xIntQuartz_K = _boxQuartzS->_xInt;
    //yIntQuartz_K = _boxQuartzS->_yInt;
    //zIntQuartz_K = _boxQuartzS->_zInt;
    //delete _trk;
    //_trk = new crtTrk( x1_M, y1_M, x2_M, y2_M);
    //_boxfTOF->GetIntersection(_trk);
    //_boxQuartzS->GetIntersection(_trk);
    //xIntFtof_M = _boxfTOF->_xInt;
    //yIntFtof_M = _boxfTOF->_yInt;
    //xIntQuartz_M = _boxQuartzS->_xInt;
    //yIntQuartz_M = _boxQuartzS->_yInt;
    //zIntQuartz_M = _boxQuartzS->_zInt;
    //delete _trk;
    //h1dXfTOFint->Fill(xIntFtof_M-xIntFtof_K);
    //h1dYfTOFint->Fill(yIntFtof_M-yIntFtof_K);
    //h1dXQuartzint->Fill(xIntQuartz_M - xIntQuartz_K);
    //h1dYQuartzint->Fill(yIntQuartz_M - yIntQuartz_K);
    //h1dZQuartzint->Fill(zIntQuartz_M - zIntQuartz_K);
    //}
    
    //////Unix time///////
    h1dUnixTime->Fill(time_CRT - prevUnixTime);
    if(uTimeStart == -999.0){
      uTimeStart = time_CRT;
      uTimeStop = uTimeStart + 3600.0;
      uTimeCenter = ((uTimeStop - uTimeStart)/2.0 + uTimeStart - _timeStartUnix)/3600.0;
      delete h1dUnixTime1hour;
      h1dUnixTime1hour = new TH1D("h1dUnixTime1hour"," dUnix Time (between neighbor events) in one hour",100,0.0,30.0);
    }

    if(time_CRT>=uTimeStart && time_CRT<uTimeStop){
      h1dUnixTime1hour->Fill(time_CRT - prevUnixTime);
    }
    else{
      h1dUnixTime1hour->Fit(f1expo,"Q","",1.5,30.0);
      for(k = 1;k<=ntimeBin;k++){
	h1dUnixTimeInTime_LowEdge = h1dUnixTimeInTime->GetBinLowEdge(k);
	h1dUnixTimeInTime_UpEdge  = h1dUnixTimeInTime_LowEdge + h1dUnixTimeInTime->GetBinWidth(k);
	if(h1dUnixTimeInTime_LowEdge <= uTimeCenter && h1dUnixTimeInTime_UpEdge>uTimeCenter ){
	  if(f1expo->GetParameter(1)!=0.0)
	    halfLifeTime = TMath::Abs(TMath::Log(2.0)/(f1expo->GetParameter(1)));
	  else
	    halfLifeTime = 0.0;
	  
	  halfLifeTimeErr = halfLifeTime - TMath::Abs(TMath::Log(2.0)/(f1expo->GetParameter(1)-f1expo->GetParError(1)));
	  h1dUnixTimeInTime->SetBinContent(k,halfLifeTime);
	  h1dUnixTimeInTime->SetBinError(k,halfLifeTimeErr);
	  k = ntimeBin+1;
	}
      }
      uTimeStart = -999.0;
    }
    prevUnixTime = time_CRT;
    //////////////////////


    h1getTrkMomID_All->Fill(getTrkMomID());

    //--------------- _hodoStatus is true -----------------
    if(_hodoStatus){
      _hodoStatusIsOK++;

      h1getTrkMomID->Fill(getTrkMomID());

      h2Hodo1YvsX->Fill(_x1, _y1);
      h2Hodo2YvsX->Fill(_x2, _y2);
     
      _trk = new crtTrk( _x1, _y1, _x2, _y2); 
      _trk->SetMomID(getTrkMomID());
      //cout<<getTrkMomID()<<endl;

      nStack1 = 0;
      nStack2 = 0;
      nStack3 = 0;
      nStack4 = 0;

      for(i = 0;i<4;i++){
	if(stack1T[i]>minBinST && stack1T[i]<maxBinST)
	  nStack1++;
	if(stack2T[i]>minBinST && stack2T[i]<maxBinST)
	  nStack2++;
	if(stack3T[i]>minBinST && stack3T[i]<maxBinST)
	  nStack3++;
	if(stack4T[i]>minBinST && stack4T[i]<maxBinST)
	  nStack4++;
      }

      h1nStack1->Fill(nStack1);
      h1nStack2->Fill(nStack2);
      h1nStack3->Fill(nStack3);
      h1nStack4->Fill(nStack4);

      _boxfTOF->GetIntersection(_trk);
      _boxQuartzS->GetIntersection(_trk);

      //if(_boxQuartzS->_xInt > -9 && _boxQuartzS->_xInt < 13){
      //if(_boxQuartzS->_yInt > -2 && _boxQuartzS->_yInt < 3){
      //L.B.
      //31.07.2011
      if(_boxQuartzS->_xInt > -8.6 && _boxQuartzS->_xInt < 12.4){
	if(_boxQuartzS->_yInt > -1.6 && _boxQuartzS->_yInt < 2.59){
	  quartzStartIsOk++;

	  h1getTrkMomID_QSC->Fill(getTrkMomID());

	  if(_trk->getMomid()==0)
	    h1TrkMomID->Fill(0.0);
	  if(_trk->getMomid()==1)
	    h1TrkMomID->Fill(0.4);
	  if(_trk->getMomid()==2)
	    h1TrkMomID->Fill(0.8);
	  if(_trk->getMomid()==3)
	    h1TrkMomID->Fill(1.2);
	  if(_trk->getMomid()==4)
	    h1TrkMomID->Fill(1.6);

	  h1Hodo1X->Fill(_x1);
	  h1Hodo2X->Fill(_x2);
	  h1Hodo1Y->Fill(_y1);
	  h1Hodo2Y->Fill(_y2);
	  //cout<<"h2_YintvsXint_Quartz"<<endl;
	  h2_YintvsXint_Quartz->Fill(_boxQuartzS->_xInt,_boxQuartzS->_yInt);
	  h1_Xint_Quartz->Fill(_boxQuartzS->_xInt);
	  h1_Yint_Quartz->Fill(_boxQuartzS->_yInt);	  
	  h1_Zint_Quartz->Fill(_boxQuartzS->_zInt);
	  h2_YintvsXint_fTOF->Fill(_boxfTOF->_xInt,_boxfTOF->_yInt);
	  h1_Xint_fTOF->Fill(_boxfTOF->_xInt);
	  h1_Yint_fTOF->Fill(_boxfTOF->_yInt);
	  
	  for(i=0;i<55;i++){
	    if(hodo1X[i]>20 && hodo1X[i]<60)
	      h1Hodo1XT_hit->Fill(i);	    
	    if(hodo2X[i]>20 && hodo2X[i]<60)
	      h1Hodo2XT_hit->Fill(i);
	  }

	  for(i=0;i<27;i++){
	    if(hodo1Y[i]>20 && hodo1Y[i]<60)
	      h1Hodo1YT_hit->Fill(i);
	    if(hodo2Y[i]>20 && hodo2Y[i]<60)
	      h1Hodo2YT_hit->Fill(i);
	  }

	  h1TrkTheta->Fill(_trk->getTrkTheta());
	  h1TrkPhi->Fill(_trk->getTrkPhi());
	  
	}
      }
      
      if(_x1 > -6.0 && _x1 < 16.0 ){
	if(_x2 > -6.0 && _x2 < 16.0 ){	  	  
	  if(_y1 > -2.5 && _y1 < 2.5 ){
	    if(_y2 > -2.5 && _y2 < 2.5 ){
	      h2HodoX1vsX2->Fill(_x1,_x2);
	      h2HodoY1vsY2->Fill(_y1,_y2);
	      h1TrkDirCosZ_deg->Fill(TMath::ACos(_cz)/TMath::Pi()*180.0);
	    }
	  }
	}
      }

      h1TrkDirCosX->Fill(_cx);
      h1TrkDirCosY->Fill(_cy);
      h1TrkDirCosZ->Fill(_cz);
      h2TrkDirCosYvsCosX->Fill(_cx,_cy);

      if(_x1< -100 || _x2<-100 ||
	 _y1< -100 || _y2<-100 ){
	cout<<"_x1 = "<<_x1<<endl
	    <<"_x2 = "<<_x2<<endl
	    <<"_y1 = "<<_y1<<endl
	    <<"_y2 = "<<_y2<<endl;
      }


      delete _trk;
    }
    //--------------- _hodoStatus is true END -----------------


    for(i=0;i<4;i++){
      h1Trig1A[i]->Fill(trig1A[i]);
      h1Trig2A[i]->Fill(trig2A[i]);
      h1Trig1T[i]->Fill(trig1T[i]);
      h1Trig2T[i]->Fill(trig2T[i]);
      
      h1StackCounter1A[i]->Fill(stack1A[i]);
      h1StackCounter2A[i]->Fill(stack2A[i]);
      h1StackCounter3A[i]->Fill(stack3A[i]);
      h1StackCounter4A[i]->Fill(stack4A[i]);
      
      h1StackCounter1T[i]->Fill(stack1T[i]);
      h1StackCounter2T[i]->Fill(stack2T[i]);
      h1StackCounter3T[i]->Fill(stack3T[i]);
      h1StackCounter4T[i]->Fill(stack4T[i]);

      h1Start1T[i]->Fill(start1T[i]);
      h1Start1A[i]->Fill(start1A[i]);

      h2Start1TvsA[i]->Fill(start1A[i],start1T[i] - dTstartCounter[i]);
    }

    //if(19<slacLocTime && slacLocTime<24){
    //if(10<slacLocTime && slacLocTime<15){
      for(i=0;i<55;i++){
	//if(hodo2X[1]>50)
	//hod2x_ch1++;      
	h1Hodo1XT->Fill(hodo1X[i]);
	h1Hodo2XT->Fill(hodo2X[i]);
	h1Hodo1XarrT[i]->Fill(hodo1X[i]);
	h1Hodo2XarrT[i]->Fill(hodo2X[i]);
	h2Hodo1XTvsTime->Fill(time_CRT,hodo1X[i]);
	h2Hodo2XTvsTime->Fill(time_CRT,hodo2X[i]);

	if(hodo1X[i]>500)
	  h1Hodo1XT_Time->Fill(_relativeRunTime);
	  
	if(hodo2X[i]>500)
	  h1Hodo2XT_Time->Fill(_relativeRunTime);

	//LB 11.11.2010 fill these histos when track is reconstructed
	//if(hodo1X[i]>20 && hodo1X[i]<60)
	//h1Hodo1XT_hit->Fill(i);
	//if(hodo2X[i]>20 && hodo2X[i]<60)
	//h1Hodo2XT_hit->Fill(i);
      }
      
      for(i=0;i<27;i++){
	h1Hodo1YT->Fill(hodo1Y[i]);
	h1Hodo2YT->Fill(hodo2Y[i]);
	h1Hodo1YarrT[i]->Fill(hodo1Y[i]);
	h1Hodo2YarrT[i]->Fill(hodo2Y[i]);
	h2Hodo1YTvsTime->Fill(time_CRT,hodo1Y[i]);
	h2Hodo2YTvsTime->Fill(time_CRT,hodo2Y[i]);
	
	if(hodo1Y[i]>500)
	  h1Hodo1YT_Time->Fill(_relativeRunTime);
	  
	if(hodo2Y[i]>500)
	  h1Hodo2YT_Time->Fill(_relativeRunTime);

	//LB 11.11.2010 fill these histos when track is reconstructed	  
	//if(hodo1Y[i]>20 && hodo1Y[i]<60)
	//h1Hodo1YT_hit->Fill(i);
	//if(hodo2Y[i]>20 && hodo2Y[i]<60)
	//h1Hodo2YT_hit->Fill(i);
	
      }
      //}      

  }
  
  //cout<<"hod2x_ch1 "<<hod2x_ch1<<endl;

  //TFile* rootFile = new TFile(outputRootHistFile.Data(), "RECREATE", " Histograms from CRT ", 1);
  TString tmpRootName = outputRootHistFile + ".root";
  TFile* rootFile = new TFile(tmpRootName.Data(), "RECREATE", " Histograms from CRT ", 1);
  rootFile->cd();
  if (rootFile->IsZombie()){
    cout<<"  ERROR ---> file "<<tmpRootName<<" is zombie"<<endl;
    //assert(0);
  }
  else
    cout<<"  Output Histos file ---> "<<tmpRootName<<endl;
  
  for(i=0;i<55;i++){
    h1Hodo1XarrT[i]->Write();
    h1Hodo2XarrT[i]->Write();
  }
  for(i=0;i<27;i++){
    h1Hodo1YarrT[i]->Write();
    h1Hodo2YarrT[i]->Write();
  }

  for(i=0;i<4;i++){
    h1Trig1A[i]->Write();
    h1Trig2A[i]->Write();
    h1Trig1T[i]->Write();
    h1Trig2T[i]->Write();
    
    h1StackCounter1A[i]->Write();
    h1StackCounter2A[i]->Write();
    h1StackCounter3A[i]->Write();
    h1StackCounter4A[i]->Write();
    
    h1StackCounter1T[i]->Write();
    h1StackCounter2T[i]->Write();
    h1StackCounter3T[i]->Write();
    h1StackCounter4T[i]->Write();

    h1Start1T[i]->Write();
    h1Start1A[i]->Write();

    h2Start1TvsA[i]->Write();
  }

  h1Hodo1XT->Write();
  h1Hodo2XT->Write();
  h1Hodo1YT->Write();
  h1Hodo2YT->Write();
  h2Hodo1XTvsTime->Write();
  h2Hodo2XTvsTime->Write();
  h2Hodo1YTvsTime->Write();
  h2Hodo2YTvsTime->Write();
  h1Hodo1XT_Time->Write();
  h1Hodo2XT_Time->Write();
  h1Hodo1YT_Time->Write();
  h1Hodo2YT_Time->Write();
  h1Hodo1XT_hit->Write();
  h1Hodo2XT_hit->Write();
  h1Hodo1YT_hit->Write();
  h1Hodo2YT_hit->Write();

  h2Hodo1YvsX->Write();
  h2Hodo2YvsX->Write();

  h1TrkDirCosX->Write();
  h1TrkDirCosY->Write();
  h1TrkDirCosZ->Write();

  h1TrkDirCosZ_deg->Write();

  h2HodoX1vsX2->Write();
  h2HodoY1vsY2->Write();


  h2_YintvsXint_Quartz->Write();
  h1_Xint_Quartz->Write();
  h1_Yint_Quartz->Write();
  h1_Zint_Quartz->Write();

  h2_YintvsXint_fTOF->Write();
  h1_Xint_fTOF->Write();
  h1_Yint_fTOF->Write();

  h1dUnixTime->Write();
  h1dUnixTimeInTime->Write();

  h1TrkTheta->Write();
  h1TrkPhi->Write();

  h1TrkMomID->Write();

  h1nStack1->Write();
  h1nStack2->Write();
  h1nStack3->Write();
  h1nStack4->Write();

  h1Hodo1X->Write();
  h1Hodo2X->Write();
  h1Hodo1Y->Write();
  h1Hodo2Y->Write();

  h1getTrkMomID->Write();
  h1getTrkMomID_All->Write();
  h1getTrkMomID_QSC->Write();

  //LB 13.11.2010
  //Test of different reconstruction codes
  //h1dX1->Write();
  //h1dX2->Write();
  //h1dY1->Write();
  //h1dY2->Write();
  //h1dXfTOFint->Write();
  //h1dYfTOFint->Write();
  //h1dXQuartzint->Write();
  //h1dYQuartzint->Write();
  //h1dZQuartzint->Write();

  gStyle->SetPalette(1);

  TString namePS = outputRootHistFile + ".ps";
  TString nameO  = namePS+"[";
  TString nameC  = namePS+"]";

  TCanvas *c1 = new TCanvas("c1"," CRT plots",10,10,600,800);
  c1->Print(nameO.Data()); 

  c1->Clear();
  c1->SetFillColor(kWhite);
  c1->Divide(2,4);
  for(i=0;i<4;i++){
    c1->cd(i+1);
    c1->GetPad(i+1)->SetLogy();
    h1Trig1A[i]->Draw();
    c1->cd(i+5);
    c1->GetPad(i+5)->SetLogy();
    h1Trig2A[i]->Draw();
    if( 0 == i ) {
      TText *A2_ADC1_empty = 
	new TText( h1Trig2A[i]->GetXaxis()->GetXmin() +
		   0.2 * ( h1Trig2A[i]->GetXaxis()->GetXmax() -
			   h1Trig2A[i]->GetXaxis()->GetXmin() ),
		   h1Trig2A[i]->GetMinimum() +
		   0.05 * ( h1Trig2A[i]->GetMaximum() -
			   h1Trig2A[i]->GetMinimum() ),
		   "Channel not working" );
      A2_ADC1_empty->SetTextColor( kRed );
      A2_ADC1_empty->SetTextSize( 0.08 );
      A2_ADC1_empty->Draw();
    }
  }
  c1->Print(namePS.Data());

  c1->Clear();
  c1->SetFillColor(kWhite);
  c1->Divide(2,4);
  for(i=0;i<4;i++){
    c1->cd(i+1);
    //c1->GetPad(i+1)->SetLogy();
    h1Trig1T[i]->Draw();
    c1->cd(i+5);
    //c1->GetPad(i+5)->SetLogy();
    h1Trig2T[i]->Draw();
  }
  c1->Print(namePS.Data());

  c1->Clear();
  c1->SetFillColor(kWhite);
  c1->Divide(2,4);
  for(i=0;i<4;i++){
    c1->cd(i+1);
    c1->GetPad(i+1)->SetLogy();
    h1StackCounter1A[i]->Draw();
    if( 3 == i ) {
      TText *stack1_ADC4_empty = 
	new TText( h1StackCounter1A[i]->GetXaxis()->GetXmin() +
		   0.2 * ( h1StackCounter1A[i]->GetXaxis()->GetXmax() -
			   h1StackCounter1A[i]->GetXaxis()->GetXmin() ),
		   h1StackCounter1A[i]->GetMinimum() +
		   0.05 * ( h1StackCounter1A[i]->GetMaximum() -
			   h1StackCounter1A[i]->GetMinimum() ),
		   "Channel not working" );
      stack1_ADC4_empty->SetTextColor( kRed );
      stack1_ADC4_empty->SetTextSize( 0.08 );
      stack1_ADC4_empty->Draw();
    }
    c1->cd(i+5);
    c1->GetPad(i+5)->SetLogy();
    h1StackCounter2A[i]->Draw();
  }
  c1->Print(namePS.Data());

  c1->Clear();
  c1->SetFillColor(kWhite);
  c1->Divide(2,4);
  for(i=0;i<4;i++){
    c1->cd(i+1);
    c1->GetPad(i+1)->SetLogy();
    h1StackCounter3A[i]->Draw();
    c1->cd(i+5);
    c1->GetPad(i+5)->SetLogy();
    h1StackCounter4A[i]->Draw();
  }
  c1->Print(namePS.Data());

  c1->Clear();
  c1->SetFillColor(kWhite);
  c1->Divide(2,4);
  for(i=0;i<4;i++){
    c1->cd(i+1);
    //c1->GetPad(i+1)->SetLogy();
    h1StackCounter1T[i]->Draw();
    c1->cd(i+5);
    //c1->GetPad(i+5)->SetLogy();
    h1StackCounter2T[i]->Draw();
  }
  c1->Print(namePS.Data());

  c1->Clear();
  c1->SetFillColor(kWhite);
  c1->Divide(2,4);
  for(i=0;i<4;i++){
    c1->cd(i+1);
    //c1->GetPad(i+1)->SetLogy();
    h1StackCounter3T[i]->Draw();
    c1->cd(i+5);
    //c1->GetPad(i+5)->SetLogy();
    h1StackCounter4T[i]->Draw();
    if( 0 == i ) {
      TText *stack4_TDC1_empty = 
	new TText( h1StackCounter4T[i]->GetXaxis()->GetXmin() +
		   0.2 * ( h1StackCounter4T[i]->GetXaxis()->GetXmax() -
			   h1StackCounter4T[i]->GetXaxis()->GetXmin() ),
		   0.5,
		   "Channel not working" );
      stack4_TDC1_empty->SetTextColor( kRed );
      stack4_TDC1_empty->SetTextSize( 0.08 );
      stack4_TDC1_empty->Draw();
    }
  }
  c1->Print(namePS.Data());

  c1->Clear();
  c1->SetFillColor(kWhite);
  c1->Divide(2,2);
  c1->cd(1);
  h1Hodo1XT->Draw();
  c1->cd(2);
  h1Hodo2XT->Draw();
  c1->cd(3);
  h1Hodo1YT->Draw();
  c1->cd(4);
  h1Hodo2YT->Draw();
  c1->Print(namePS.Data());

  c1->Clear();
  c1->SetFillColor(kWhite);
  c1->Divide(2,2);
  c1->cd(1);
  h1Hodo1XT_hit->Draw();
  TLine *h1Hodo1XT_badChannel28 =
    new TLine( 28, h1Hodo1XT_hit->GetMinimum(),
	       28, h1Hodo1XT_hit->GetMaximum() * 1.05 );
  h1Hodo1XT_badChannel28->SetLineColor( kRed );
  h1Hodo1XT_badChannel28->Draw();
  c1->cd(2);
  h1Hodo2XT_hit->Draw();
  TLine *h1Hodo2XT_badChannel14 =
    new TLine( 14, h1Hodo2XT_hit->GetMinimum(),
	       14, h1Hodo2XT_hit->GetMaximum() * 1.05 );
  h1Hodo2XT_badChannel14->SetLineColor( kRed );
  h1Hodo2XT_badChannel14->Draw();
  TLine *h1Hodo2XT_badChannel46 =
    new TLine( 46, h1Hodo2XT_hit->GetBinContent( 47 ),
	       46, h1Hodo2XT_hit->GetMaximum() * 1.05 );
  h1Hodo2XT_badChannel46->SetLineColor( kOrange );
  h1Hodo2XT_badChannel46->Draw();
  c1->cd(3);
  h1Hodo1YT_hit->Draw();
  c1->cd(4);
  h1Hodo2YT_hit->Draw();
  TLine *h1Hodo2YT_badChannel11 =
    new TLine( 11, h1Hodo2YT_hit->GetMinimum(),
	       11, h1Hodo2YT_hit->GetMaximum() * 1.05 );
  h1Hodo2YT_badChannel11->SetLineColor( kRed );
  h1Hodo2YT_badChannel11->Draw();
  c1->Print(namePS.Data());

  c1->Clear();
  c1->SetFillColor(kWhite);
  c1->Divide(2,1);
  c1->cd(1);
  h1TrkTheta->Draw();
  c1->cd(2);
  h1TrkPhi->Draw();
  c1->Print(namePS.Data());

  TLine *lnqzStartTop = new TLine(-6.0,2.25,16.0,2.25);
  TLine *lnqzStartBot = new TLine(-6.0,-2.25,16.0,-2.25);
  TLine *lnqzStartLef = new TLine(-6.0,2.25,-6.0,-2.25);
  TLine *lnqzStartRig = new TLine(16.0,2.25,16.0,-2.25);
  lnqzStartTop->SetLineWidth(2);
  lnqzStartTop->SetLineColor(kRed);
  lnqzStartBot->SetLineWidth(2);
  lnqzStartBot->SetLineColor(kRed);
  lnqzStartLef->SetLineWidth(2);
  lnqzStartLef->SetLineColor(kRed);
  lnqzStartRig->SetLineWidth(2);
  lnqzStartRig->SetLineColor(kRed);

  c1->Clear();
  c1->SetFillColor(kWhite);
  c1->Divide(1,2);
  c1->cd(1);
  h2Hodo1YvsX->Draw("BOX");
  lnqzStartTop->Draw("same");
  lnqzStartBot->Draw("same");
  lnqzStartLef->Draw("same");
  lnqzStartRig->Draw("same");
  c1->cd(2);
  h2Hodo2YvsX->Draw("BOX");
  lnqzStartTop->Draw("same");
  lnqzStartBot->Draw("same");
  lnqzStartLef->Draw("same");
  lnqzStartRig->Draw("same");
  c1->Print(namePS.Data());


  c1->Clear();
  c1->SetFillColor(kWhite);
  c1->Divide(1,2);
  c1->cd(1);
  h1Hodo1X->Draw();
  c1->cd(2);
  h1Hodo2X->Draw();
  c1->Print(namePS.Data());


  c1->Clear();
  c1->SetFillColor(kWhite);
  c1->Divide(1,2);
  c1->cd(1);
  h1Hodo1Y->Draw();
  c1->cd(2);
  h1Hodo2Y->Draw();
  c1->Print(namePS.Data());


  c1->Clear();
  c1->SetFillColor(kWhite);
  c1->Divide(2,2);
  c1->cd(1);
  h2TrkDirCosYvsCosX->Draw("BOX");
  c1->cd(2);
  h1TrkDirCosY->Draw();
  c1->cd(3);
  h1TrkDirCosX->Draw();
  c1->cd(4);
  h1TrkDirCosZ->Draw();
  c1->Print(namePS.Data());

  c1->Clear();
  c1->SetFillColor(kWhite);
  c1->Divide(1,2);
  c1->cd(1);
  h1TrkDirCosZ->Draw();
  c1->cd(2);
  h1TrkDirCosZ_deg->Draw();
  c1->Print(namePS.Data());

  //my hodoscope
  c1->Clear();
  c1->SetFillColor(kWhite);
  c1->Divide(2,2);
  c1->cd(1);
  h2Hodo1XTvsTime->Draw("ZCOLOR");
  c1->cd(2);
  h2Hodo2XTvsTime->Draw("ZCOLOR");
  c1->cd(3);
  h2Hodo1YTvsTime->Draw("ZCOLOR");
  c1->cd(4);
  h2Hodo2YTvsTime->Draw("ZCOLOR");
  c1->Print(namePS.Data());

  c1->Clear();
  c1->SetFillColor(kWhite);
  c1->Divide(2,2);
  c1->cd(1);
  h1Hodo1XT_Time->Draw();
  c1->cd(2);
  h1Hodo2XT_Time->Draw();
  c1->cd(3);
  h1Hodo1YT_Time->Draw();
  c1->cd(4);
  h1Hodo2YT_Time->Draw();
  c1->Print(namePS.Data());

  TLine *lnqzStartTop_X = new TLine(16.0,16.0,-6.0,16.0);
  TLine *lnqzStartBot_X = new TLine(16.0,-6.0,-6.0,-6.0);
  TLine *lnqzStartLef_X = new TLine(-6.0,-6.0,-6.0,16.0);
  TLine *lnqzStartRig_X = new TLine(16.0,-6.0,16.0,16.0);
  lnqzStartTop_X->SetLineWidth(2);
  lnqzStartTop_X->SetLineColor(kRed);
  lnqzStartBot_X->SetLineWidth(2);
  lnqzStartBot_X->SetLineColor(kRed);
  lnqzStartLef_X->SetLineWidth(2);
  lnqzStartLef_X->SetLineColor(kRed);
  lnqzStartRig_X->SetLineWidth(2);
  lnqzStartRig_X->SetLineColor(kRed);

  TLine *lnqzStartTop_Y = new TLine(2.5,2.5,-2.5,2.5);
  TLine *lnqzStartBot_Y = new TLine(2.5,-2.5,-2.5,-2.5);
  TLine *lnqzStartLef_Y = new TLine(-2.5,-2.5,-2.5,2.5);
  TLine *lnqzStartRig_Y = new TLine(2.5,-2.5,2.5,2.5);
  lnqzStartTop_Y->SetLineWidth(2);
  lnqzStartTop_Y->SetLineColor(kRed);
  lnqzStartBot_Y->SetLineWidth(2);
  lnqzStartBot_Y->SetLineColor(kRed);
  lnqzStartLef_Y->SetLineWidth(2);
  lnqzStartLef_Y->SetLineColor(kRed);
  lnqzStartRig_Y->SetLineWidth(2);
  lnqzStartRig_Y->SetLineColor(kRed);

  c1->Clear();
  c1->SetFillColor(kWhite);
  c1->Divide(1,2);
  c1->cd(1);
  h2HodoX1vsX2->Draw("ZCOLOR");
  lnqzStartTop_X->Draw("same");
  lnqzStartBot_X->Draw("same");
  lnqzStartLef_X->Draw("same");
  lnqzStartRig_X->Draw("same");
  c1->cd(2);
  h2HodoY1vsY2->Draw("ZCOLOR");
  lnqzStartTop_Y->Draw("same");
  lnqzStartBot_Y->Draw("same");
  lnqzStartLef_Y->Draw("same");
  lnqzStartRig_Y->Draw("same");
  c1->Print(namePS.Data());
  ///////////////////////////////////

  c1->Clear();
  c1->SetFillColor(kWhite);
  c1->Divide(2,2);
  c1->cd(1);
  h2_YintvsXint_Quartz->Draw("ZCOLOR");
  c1->cd(3);
  h1_Xint_Quartz->Draw();
  c1->cd(4);
  h1_Yint_Quartz->Draw();
  c1->Print(namePS.Data());

  c1->Clear();
  c1->SetFillColor(kWhite);
  c1->Divide(2,2);
  c1->cd(1);
  h2_YintvsXint_fTOF->Draw("ZCOLOR");
  c1->cd(3);
  h1_Xint_fTOF->Draw();
  c1->cd(4);
  h1_Yint_fTOF->Draw();
  c1->Print(namePS.Data());

  c1->Clear();
  c1->SetFillColor(kWhite);
  c1->Divide(2,2);
  c1->cd(1);
  h1Start1T[0]->Draw();
  //c1->GetPad(1)->SetLogy();
  c1->cd(2);
  h1Start1T[1]->Draw();
  //c1->GetPad(2)->SetLogy();
  c1->cd(3);
  h1Start1T[2]->Draw();
  //c1->GetPad(3)->SetLogy();
  c1->cd(4);
  h1Start1T[3]->Draw();
  //c1->GetPad(4)->SetLogy();
  c1->Print(namePS.Data());

  c1->Clear();
  c1->SetFillColor(kWhite);
  c1->Divide(2,2);
  c1->cd(1);
  h1Start1A[0]->Draw();
  //c1->GetPad(1)->SetLogy();
  c1->cd(2);
  h1Start1A[1]->Draw();
  //c1->GetPad(2)->SetLogy();
  c1->cd(3);
  h1Start1A[2]->Draw();
  //c1->GetPad(3)->SetLogy();
  c1->cd(4);
  h1Start1A[3]->Draw();
  //c1->GetPad(4)->SetLogy();
  c1->Print(namePS.Data());

  c1->Clear();
  c1->SetFillColor(kWhite);
  h1dUnixTime->Draw();
  c1->Print(namePS.Data());

  c1->Clear();
  c1->SetFillColor(kWhite);
  h1dUnixTimeInTime->Draw();
  c1->Print(namePS.Data());

  c1->Clear();
  c1->SetFillColor(kWhite);
  
  //TPaveText *pt = new TPaveText(0.0,0.0,1.0,1.0);
  //pt->AddText("hodoStatusIsOK fraction");
  //pt->AddText("quartzStartIsOk fraction");
  //pt->Draw();
  TString myTextStr = "hodo reco eff = ";
  myTextStr+= (_hodoStatusIsOK/nentriesD);
  TText *myText = new TText(0.0,0.0, myTextStr.Data());
  myText->Draw();
  c1->Print(namePS.Data());

  c1->Print(nameC.Data());

  cout<<"_hodoStatusIsOK fraction   "<<_hodoStatusIsOK/nentriesD<<endl
      <<"quartzStartIsOk fraction   "<<quartzStartIsOk/nentriesD<<endl;
}


void  crtFileStr::crtHodoscopeDataTest(TString outputRootHistFile){

  GetDataRate();
  
  Int_t i = 0;
  
  //Hodoscope
  TH1D *h1nHodo1X = new TH1D("h1nHodo1X","number of hits Hodoscope1 X",301,-1.0,100.0); 
  TH1D *h1n2Hodo1X = new TH1D("h1n2Hodo1X","two hits in Hodoscope1 X",301,-1.0,100.0); 
  TH1D *h1nHodo2X = new TH1D("h1nHodo2X","number of hits Hodoscope2 X",301,-1.0,100.0); 
  TH1D *h1nHodo1Y = new TH1D("h1nHodo1Y","number of hits Hodoscope1 Y",301,-1.0,100.0); 
  TH1D *h1nHodo2Y = new TH1D("h1nHodo2Y","number of hits Hodoscope2 Y",301,-1.0,100.0); 
  
  //Int_t _hodoStatusIsOK = 0;
  Int_t nHitX1 = 0;
  Int_t nHitX2 = 0;

  Int_t nHitY1 = 0;
  Int_t nHitY2 = 0;

  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  Double_t nentriesD = fChain->GetEntriesFast();
  cout<<"======================="<<endl;
  cout<<" total number of event "<<nentries<<endl;
  cout<<" total number of event double "<<nentriesD<<endl;
  cout<<"======================="<<endl;
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    if(jentry%100000==0)
      cout<<" processed events "<<jentry<<endl;
      
    GetDataFromDetectors();

    nHitX1 = 0;
    nHitX2 = 0;
    nHitY1 = 0;
    nHitY2 = 0;

    for(i=0;i<55;i++){
      if(hodo1X[i]>20 && hodo1X[i]<60)
	nHitX1++;
    }

    for(i=0;i<55;i++){
      if(hodo2X[i]>20 && hodo2X[i]<60)
	nHitX2++;
    }
    
    for(i=0;i<27;i++){
      if(hodo1Y[i]>20 && hodo1Y[i]<60)
	nHitY1++;
    }
    
    for(i=0;i<27;i++){
      if(hodo2Y[i]>20 && hodo2Y[i]<60)
	nHitY2++;
    }
    
    if(nHitX2>0 && nHitX2<3.0 && nHitX1>0 && nHitX1<3.0){
      if(nHitY2>0 && nHitY2<3.0 && nHitY1>0 && nHitY1<3.0){
	h1nHodo1X->Fill(nHitX1,1.0/nentriesD);
	h1nHodo2X->Fill(nHitX2,1.0/nentriesD);
	h1nHodo1Y->Fill(nHitY1,1.0/nentriesD);
	h1nHodo2Y->Fill(nHitY2,1.0/nentriesD);
      }
    }
    
    //Int_t hitbarID = -1;
    if(nHitX2==2  && nHitX1>0 && nHitX1<3.0){
      if(nHitY2>0 && nHitY2<3.0 && nHitY1>0 && nHitY1<3.0){
	for(i = 0;i<55;i++){
	  if(20< hodo2X[i] && hodo2X[i]<60){
	    if((i+1)<55){
	      if(20< hodo2X[i+1] && hodo2X[i+1]<60){
		h1n2Hodo1X->Fill(2.0,1.0/nentriesD);
	      }
	    }
	    break;
	  }
	}
      }
    }
    
  }
  
  TFile* rootFile = new TFile(outputRootHistFile.Data(), "RECREATE", " Histograms from CRT ", 1);
  rootFile->cd();
  if (rootFile->IsZombie()){
    cout<<"  ERROR ---> file "<<outputRootHistFile<<" is zombi"<<endl;
    //assert(0);
  }
  else
    cout<<"  Output Histos file ---> "<<outputRootHistFile<<endl;

  h1nHodo1X->Write();
  h1nHodo2X->Write();
  h1nHodo1Y->Write();
  h1nHodo2Y->Write();
  h1n2Hodo1X->Write();

  /*
    gStyle->SetPalette(1);
    TString namePS = outputRootHistFile + ".ps";
    TString nameO  = namePS+"[";
    TString nameC  = namePS+"]";
    
    TCanvas *c1 = new TCanvas("c1"," CRT plots",10,10,600,800);
    c1->Print(nameO.Data()); 
    
    c1->Clear();
    c1->SetFillColor(kWhite);
    c1->Divide(2,4);
    for(i=0;i<4;i++){
    c1->cd(i+1);
    c1->GetPad(i+1)->SetLogy();
    h1Trig1A[i]->Draw();
    c1->cd(i+5);
    c1->GetPad(i+5)->SetLogy();
    h1Trig2A[i]->Draw();
    }
    c1->Print(namePS.Data());
    
    c1->Print(nameC.Data());
  */
  
}

void crtFileStr::GetDataToMerge(){
  GetDataFromDetectors();
  GetRelativeRunTime();
  //GetHodoscopePositions();
  GetHodoscopePositionsMyAlgo();
  GetTrkDirectionCos();
  if(_hodoStatus){
    _trk = new crtTrk( _x1, _y1, _x2, _y2); 
    _trk->SetMomID(getTrkMomID());
    _boxfTOF->GetIntersection(_trk);
    _boxfTOF_Bot->GetIntersection(_trk);
    _boxQuartzS->GetIntersection(_trk);
    _trkTheta = _trk->getTrkTheta();
    _trkPhi = _trk->getTrkPhi();
    _trkMomID = _trk->getMomid();
    delete _trk;
  }
  else{
    _trkTheta = -999.0;
    _trkPhi = -999.0;
    _trkMomID = -999;
  }

}

Int_t crtFileStr::getTrkMomID(){
  Int_t i = 0;
  Int_t nStack1 = 0;
  Int_t nStack2 = 0;
  Int_t nStack3 = 0;
  Int_t nStack4 = 0;

  Double_t minBin = 50;
  Double_t maxBin = 1300;

  for(i = 0;i<4;i++){
    if(stack1T[i]>minBin && stack1T[i]<maxBin )
      nStack1++;
    if(stack2T[i]>minBin && stack2T[i]<maxBin )
      nStack2++;
    if(stack3T[i]>minBin && stack3T[i]<maxBin )
      nStack3++;
    if(stack4T[i]>minBin && stack4T[i]<maxBin )
      nStack4++;
  }

  //if(nStack4 > 2 && nStack3 == 0 && nStack2 == 0 && nStack1 == 0 ){
  //return 1;
  //}
  //else if(nStack4 > 2 && nStack3 > 3 && nStack2 == 0 && nStack1 == 0 ){
  //return 2;
  //}
  //else if(nStack4 > 2 && nStack3 > 3 && nStack2 > 2 && nStack1 == 0 ){
  //return 3;
  //}
  //else if(nStack4 > 2 && nStack3 > 3 && nStack2 > 2 && nStack1 > 2 ){
  //return 4;
  //}
  //else{
  //return 0;
  //}

  if(nStack1 > 1  ){
    return 4;
  }
  else if(nStack2 > 1 ){
    return 3;
  }
  else if(nStack3 > 1 ){
    return 2;
  }
  else if(nStack4 > 1 ){
    return 1;
  }
  else{
    return 0;
  }

  return 0;
}

Double_t crtFileStr::GetRelativeRunTime(){
  /*
  Int_t dTime = 7;// GMT - SLAClocal time
  _relativeRunTime = (time_CRT - _timeStartUnix)/3600.0 + _timeStart_hour + _timeStart_min/60.0 - dTime;
  */
  //_relativeRunTime = (time_CRT - _timeStartUnix)/3600.0;
  _relativeRunTime = time_CRT + timeShiftForProperDisplay;
  return _relativeRunTime;
}


void crtFileStr::initHistos_H1D(TH1D *h1D1[4],string h1name, string h1Title,
				  Int_t Nbin, Float_t Vmin, Float_t Vmax){
  int i = 0;
  int ii = 0;
  TString myh1name;
  TString myh1Title;
  for(i = 0;i<4;i++){
    ii = i + 1;
    myh1name = h1name.c_str();
    myh1Title= h1Title.c_str();
    myh1name += "_"; 
    myh1name += ii; 
    myh1Title+= ii; 
    //cout<<"myh1name = "<<myh1name.Data()<<endl;
    //cout<<"h1Title  = "<<myh1Title.Data()<<endl;
    h1D1[i] = new TH1D(myh1name.Data(), myh1Title.Data(), 
		       Nbin, Vmin, Vmax);
  }  
}


void crtFileStr::initHistos_H2D(TH2D *h2D1[4],string h2name, string h2Title,
				Int_t Nbin1, Float_t Vmin1, Float_t Vmax1,
				Int_t Nbin2, Float_t Vmin2, Float_t Vmax2){
  int i = 0;
  int ii = 0;
  TString myh2name;
  TString myh2Title;
  for(i = 0;i<4;i++){
    ii = i + 1;
    myh2name = h2name.c_str();
    myh2Title= h2Title.c_str();
    myh2name += "_"; 
    myh2name += ii; 
    myh2Title+= ii; 
    //cout<<"myh1name = "<<myh1name.Data()<<endl;
    //cout<<"h1Title  = "<<myh1Title.Data()<<endl;
    h2D1[i] = new TH2D(myh2name.Data(), myh2Title.Data(), 
		       Nbin1, Vmin1, Vmax1,
		       Nbin2, Vmin2, Vmax2);
  }
}


void crtFileStr::initHistosHodoX_H1D(TH1D *h1D1[55],string h1name, string h1Title,
				     Int_t Nbin, Float_t Vmin, Float_t Vmax){
  int i = 0;
  int ii = 0;
  TString myh1name;
  TString myh1Title;
  for(i = 0;i<55;i++){
    ii = i + 1;
    myh1name = h1name.c_str();
    myh1Title= h1Title.c_str();
    myh1name += "_"; 
    myh1name += ii; 
    myh1Title+= ii; 
    //cout<<"myh1name = "<<myh1name.Data()<<endl;
    //cout<<"h1Title  = "<<myh1Title.Data()<<endl;
    h1D1[i] = new TH1D(myh1name.Data(), myh1Title.Data(), 
		       Nbin, Vmin, Vmax);
  }  
}


void crtFileStr::initHistosHodoY_H1D(TH1D *h1D1[27],string h1name, string h1Title,
				     Int_t Nbin, Float_t Vmin, Float_t Vmax){
  int i = 0;
  int ii = 0;
  TString myh1name;
  TString myh1Title;
  for(i = 0;i<27;i++){
    ii = i + 1;
    myh1name = h1name.c_str();
    myh1Title= h1Title.c_str();
    myh1name += "_"; 
    myh1name += ii; 
    myh1Title+= ii; 
    //cout<<"myh1name = "<<myh1name.Data()<<endl;
    //cout<<"h1Title  = "<<myh1Title.Data()<<endl;
    h1D1[i] = new TH1D(myh1name.Data(), myh1Title.Data(), 
		       Nbin, Vmin, Vmax);
  }  
}


void crtFileStr::GetHodoscopePositions(){
  //Require only one hit in all hodoscope planes 
  //just in case when we have two neighbor hits 
  //we correct position.

  Int_t i = 0;
  Int_t hitbarID = -1;
  Double_t x1 = -9999.0;
  Double_t x2 = -9999.0;
  Double_t y1 = -9999.0;
  Double_t y2 = -9999.0;

  Int_t tdcBinMax = 60;
  Int_t tdcBinMin = 20;

  Bool_t smearingOn = false;

  _hodoStatus = false;

  hitbarID = -1;
  //x1 (lower plane)
  for(i = 0;i<55;i++){
    if(tdcBinMin< hodo1X[i] && hodo1X[i]<tdcBinMax){
      if(hitbarID == -1){
	hitbarID = i;
	//my
	//z1 = anaConst::hodoYBar_lenght - anaConst::hodoBar_width/2.0 - i*b;
	//kononov
	x1 = (hitbarID+1.0)*2.0-56;
      }
      else if( i == (hitbarID+1)){
	//my
      	//z1 = z1 - 1.0;
	x1 = x1 + 1.0;
      }
      else{
	x1 = -9999.0;
	break;
      }
    }
  }
  _x1 = x1;


  hitbarID = -1;
  //x2 (upperx plane)
  for(i = 0;i<55;i++){
    if(tdcBinMin< hodo2X[i] && hodo2X[i]<tdcBinMax){
      if(hitbarID == -1){
	hitbarID = i;
	//my
	//z1 = anaConst::hodoYBar_lenght - anaConst::hodoBar_width/2.0 - i*b;
	//kononov
	x2 = (hitbarID+1.0)*2.0-56;
      }
      else if( i == (hitbarID+1)){
	//my
      	//z1 = z1 - 1.0;
	x2 = x2 + 1.0;
      }
      else{
	x2 = -9999.0;
	break;
      }
    }
  }
  _x2 = x2;

  hitbarID = -1;
  //y1 (lower plane)
  for(i = 0;i<27;i++){
    if(tdcBinMin< hodo1Y[i] && hodo1Y[i]<tdcBinMax){
      if(hitbarID == -1){
	hitbarID = i;
	//my
	//z1 = anaConst::hodoYBar_lenght - anaConst::hodoBar_width/2.0 - i*b;
	//kononov
	y1 = (hitbarID+1.0)*2.0-28;
      }
      else if( i == (hitbarID+1)){
	//my
      	//z1 = z1 - 1.0;
	y1 = y1 + 1.0;
      }
      else{
	y1 = -9999.0;
	break;
      }
    }
  }
  _y1 = y1;


  hitbarID = -1;
  //y2 (upper plane)
  for(i = 0;i<27;i++){
    if(tdcBinMin< hodo2Y[i] && hodo2Y[i]<tdcBinMax){
      if(hitbarID == -1){
	hitbarID = i;
	//my
	//z1 = anaConst::hodoYBar_lenght - anaConst::hodoBar_width/2.0 - i*b;
	//kononov
	y2 = (hitbarID+1.0)*2.0-28;
      }
      else if( i == (hitbarID+1)){
	//my
      	//z1 = z1 - 1.0;
	y2 = y2 + 1.0;
      }
      else{
	y2 = -9999.0;
	break;
      }
    }
  }
  _y2 = y2;


  if(_x1!= -9999.0 && _x2!= -9999.0 &&
     _y1!= -9999.0 && _y2!= -9999.0){
    _hodoStatus = true;

    if(smearingOn){
      _x1 = _x1 + _rnd->Uniform(-0.5,0.5);
      _x2 = _x2 + _rnd->Uniform(-0.5,0.5);
      _y1 = _y1 + _rnd->Uniform(-0.5,0.5);
      _y2 = _y2 + _rnd->Uniform(-0.5,0.5);
    }

  }

}


void crtFileStr::GetHodoscopePositionsMyAlgo(){
  //Require only one hit in all hodoscope planes 
  //just in case when we have two neighbor hits 
  //we correct position.

  Int_t i = 0;
  Int_t iF = -999;      //ID of the first bar which give signal
  Int_t iS = -999;      //ID of the second bar which give signal
  Int_t hitbarID = -1;  //ID of the current bar which give signal

  Int_t tdcBinMax = 60; //max hodoscope tdc value
  Int_t tdcBinMin = 20; //min hodoscope tdc value

  Bool_t smearingOn = false;

  Double_t dx = (anaConst::hodoYBar_lenght+1.0)/28.0/4.0; //0.964 cm = resolution*sqrt(12) of the x hodoscope plane
  Double_t dy = (anaConst::hodoXBar_lenght+1.0)/14.0/4.0; //0.93  cm = resolution*sqrt(12) of the y hodoscope plane

  Bool_t x1OK = true;
  Bool_t x2OK = true;
  Bool_t y1OK = true;
  Bool_t y2OK = true;

  _hodoStatus = false;

  ///////////////x1 (BOT plane)/////////////////
  hitbarID = -1;
  iF = -999;
  iS = -999;
  for(i = 0;i<55;i++){
    if(tdcBinMin<hodo1X[i] && hodo1X[i]<tdcBinMax){
      if(hitbarID == -1){
	hitbarID = i;
	iF = i;
      }
      else if( i == (hitbarID+1)){
	iS = i;
      }
      else{
	iF = -999;
	iS = -999;
	_x1 = -9999.0;
	x1OK = false;
	break;
      }
    }
  }

  if(iF==-999 && iS==-999)
    x1OK = false;

  if(iF==-999 && iS!=-999){
    cout<<" ERROR --> iF==-999 && iS!=-999 "<<endl
	<<" iF == -999"<<endl
	<<" iS == -999"<<endl;
    assert(0);
  }

  if(x1OK){
    if( iF!=-999 && iS ==-999 ){
      if(iF==0){
	_x1 = -1.0*(anaConst::hodoYBar_lenght)/2.0 + dx;
	if(smearingOn)
	  _x1 = _x1 + _rnd->Uniform(-dx,dx);
      }
      else if (iF == 54){
	_x1 = (anaConst::hodoYBar_lenght)/2.0 - dx;
	if(smearingOn)
	  _x1 = _x1 + _rnd->Uniform(-dx,dx);	
      }
      else if (iF == 27){
	//take into account not worcking channel N28
	_x1 = dx/2.0;
	if(smearingOn)
	  _x1 = _x1 + _rnd->Uniform(-dx,dx);	
      }
      else if (iF == 29){
	//take into account not worcking channel N28
	_x1 = (iF - 27)*2.0*dx - dx/2.0;
	if(smearingOn)
	  _x1 = _x1 + _rnd->Uniform(-dx,dx);	
      }
      else{
	_x1 = (iF - 27)*2.0*dx;
	if(smearingOn)
	  _x1 = _x1 + _rnd->Uniform(-dx/2.0,dx/2.0);
      }
    }
    else if( iF!=-999 && iS !=-999 ){
      _x1 = (iS - 27)*2.0*dx - dx;
      if(smearingOn)
	_x1 = _x1 + _rnd->Uniform(-dx/2.0,dx/2.0);
    }
  }
  else{
    _x1 = -9999.0;  
  }
  ///////////////x1 END/////////////////////////


  ///////////////x2 (TOP plane)/////////////////
  hitbarID = -1;
  iF = -999;
  iS = -999;
  for(i = 0;i<55;i++){
    if(tdcBinMin<hodo2X[i] && hodo2X[i]<tdcBinMax){
      if(hitbarID == -1){
	hitbarID = i;
	iF = i;
      }
      else if( i == (hitbarID+1)){
	iS = i;
      }
      else{
	iF = -999;
	iS = -999;
	_x2 = -9999.0;
	x2OK = false;
	break;
      }
    }
  }

  if(iF==-999 && iS==-999)
    x2OK = false;
  
  if(iF==-999 && iS!=-999){
    cout<<" ERROR --> iF==-999 && iS!=-999 "<<endl
	<<" iF == -999"<<endl
	<<" iS == -999"<<endl;
    assert(0);
  }

  if(x2OK){
    if( iF!=-999 && iS ==-999 ){
      if(iF==0){
	_x2 = -1.0*(anaConst::hodoYBar_lenght)/2.0 + dx;
	if(smearingOn)
	  _x2 = _x2 + _rnd->Uniform(-dx,dx);
      }
      else if (iF == 54){
	_x2 = (anaConst::hodoYBar_lenght)/2.0 - dx;
	if(smearingOn)
	  _x2 = _x2 + _rnd->Uniform(-dx,dx);	
      }
      else if (iF == 13){
	_x2 = (iF - 27)*2.0*dx + dx/2.0;
	if(smearingOn)
	  _x2 = _x2 + _rnd->Uniform(-dx,dx);	
      }
      else if (iF == 15){
	_x2 = (iF - 27)*2.0*dx - dx/2.0;
	if(smearingOn)
	  _x2 = _x2 + _rnd->Uniform(-dx,dx);	
      }
      else if (iF == 45){
	_x2 = (iF - 27)*2.0*dx + dx/2.0;
	if(smearingOn)
	  _x2 = _x2 + _rnd->Uniform(-dx,dx);	
      }
      else if (iF == 47){
	_x2 = (iF - 27)*2.0*dx - dx/2.0;
	if(smearingOn)
	  _x2 = _x2 + _rnd->Uniform(-dx,dx);	
      }
      else{
	_x2 = (iF - 27)*2.0*dx;
	if(smearingOn)
	  _x2 = _x2 + _rnd->Uniform(-dx/2.0,dx/2.0);
      }
    }
    else if( iF!=-999 && iS !=-999 ){
      _x2 = (iS - 27)*2.0*dx - dx;
      if(smearingOn)
	_x2 = _x2 + _rnd->Uniform(-dx/2.0,dx/2.0);
    }
  }
  else{
    _x2 = -9999.0;  
  }
  ///////////////x2 END/////////////////////////


  ///////////////y1 (BOT plane)/////////////////
  hitbarID = -1;
  iF = -999;
  iS = -999;
  for(i = 0;i<27;i++){
    if(tdcBinMin<hodo1Y[i] && hodo1Y[i]<tdcBinMax){
      if(hitbarID == -1){
	hitbarID = i;
	iF = i;
      }
      else if( i == (hitbarID+1)){
	iS = i;
      }
      else{
	iF = -999;
	iS = -999;
	_y1 = -9999.0;
	y1OK = false;
	break;
      }
    }
  }

  if(iF==-999 && iS==-999)
    y1OK = false;
  
  if(iF==-999 && iS!=-999){
    cout<<" ERROR --> iF==-999 && iS!=-999 "<<endl
	<<" iF == -999"<<endl
	<<" iS == -999"<<endl;
    assert(0);
  }

  if(y1OK){
    if( iF!=-999 && iS ==-999 ){
      if(iF==0){
	_y1 = -1.0*(anaConst::hodoXBar_lenght)/2.0 + dy;
	if(smearingOn)
	  _y1 = _y1 + _rnd->Uniform(-dy,dy);
      }
      else if (iF == 26){
	_y1 = (anaConst::hodoYBar_lenght)/2.0 - dy;
	if(smearingOn)
	  _y1 = _y1 + _rnd->Uniform(-dy,dy);	
      }
      else{
	_y1 = (iF - 13)*2.0*dy;
	if(smearingOn)
	  _y1 = _y1 + _rnd->Uniform(-dy/2.0,dy/2.0);
      }
    }
    else if( iF!=-999 && iS !=-999 ){
      _y1 = (iS - 13)*2.0*dy - dy;
      if(smearingOn)
	_y1 = _y1 + _rnd->Uniform(-dy/2.0,dy/2.0);
    }
  }
  else{
    _y1 = -9999.0;
  }
  ///////////////y1 END/////////////////////////

  ///////////////y2 (BOT plane)/////////////////
  hitbarID = -1;
  iF = -999;
  iS = -999;
  for(i = 0;i<27;i++){
    if(tdcBinMin<hodo2Y[i] && hodo2Y[i]<tdcBinMax){
      if(hitbarID == -1){
	hitbarID = i;
	iF = i;
      }
      else if( i == (hitbarID+1)){
	iS = i;
      }
      else{
	iF = -999;
	iS = -999;
	_y2 = -9999.0;
	y2OK = false;
	break;
      }
    }
  }

  if(iF==-999 && iS==-999)
    y2OK = false;
  
  if(iF==-999 && iS!=-999){
    cout<<" ERROR --> iF==-999 && iS!=-999 "<<endl
	<<" iF == -999"<<endl
	<<" iS == -999"<<endl;
    assert(0);
  }

  if(y2OK){
    if( iF!=-999 && iS ==-999 ){
      if(iF==0){
	_y2 = -1.0*(anaConst::hodoXBar_lenght)/2.0 + dy;
	if(smearingOn)
	  _y2 = _y2 + _rnd->Uniform(-dy,dy);
      }
      else if (iF == 26){
	_y2 = (anaConst::hodoYBar_lenght)/2.0 - dy;
	if(smearingOn)
	  _y2 = _y2 + _rnd->Uniform(-dy,dy);	
      }
      else if (iF == 10){
	_y2 = (iF - 13)*2.0*dy + dy/2.0;
	if(smearingOn)
	  _y2 = _y2 + _rnd->Uniform(-dy,dy);	
      }
      else if (iF == 12){
	_y2 = (iF - 13)*2.0*dy - dy/2.0;
	if(smearingOn)
	  _y2 = _y2 + _rnd->Uniform(-dy,dy);	
      }
      else{
	_y2 = (iF - 13)*2.0*dy;
	if(smearingOn)
	  _y2 = _y2 + _rnd->Uniform(-dy/2.0,dy/2.0);
      }
    }
    else if( iF!=-999 && iS !=-999 ){
      _y2 = (iS - 13)*2.0*dy - dy;
      if(smearingOn)
	_y2 = _y2 + _rnd->Uniform(-dy/2.0,dy/2.0);
    }
  }
  else{
    _y2 = -9999.0;
  }
  ///////////////y2 END/////////////////////////


  //if(_x1!= -9999.0 && _x2!= -9999.0 &&
  // _y1!= -9999.0 && _y2!= -9999.0){
  //_hodoStatus = true;
  //}

  if( x1OK==true && x2OK==true && y1OK==true && y2OK==true ){
    _hodoStatus = true;
  }
  else{
    _x1 = -9999.0;
    _x2 = -9999.0;
    _y1 = -9999.0;
    _y2 = -9999.0;
  }
  
}


void crtFileStr::GetTrkDirectionCos(){

  Double_t H = anaConst::hodo_hight;//Distance between hodoscopes middle planes, cm
  Double_t vx = _x2 - _x1;
  Double_t vy = _y2 - _y1;        
  Double_t length = (TMath::Sqrt(H*H+vx*vx+vy*vy));
  

  if(_x2>-100.0 && _x1>-100.0 && _y2>-100.0 && _y1>-100.0){
    _cx = -vx/length;
    _cy = -vy/length;
    _cz = -H/length;  
  }
  else{
    _cx = -9999.0;
    _cy = -9999.0;
    _cz = -9999.0;
  }

}


void crtFileStr::PrintEventInfo(Int_t evI){
  //fChain->GetEntry(evI);
  GetEntry(evI);
  Int_t i = 0;
  Int_t j = 0;
  Int_t k = 0;
  cout<<"============================"<<endl
      <<"--> Event ID    : "<<evI<<endl
      <<"--> Event time  : "<<time_CRT<<endl;

  cout<<"-------------------"
      <<"--------------------  CRT Data  ---------------------"
      <<"-------------------"<<endl;
  cout<<"          tdc1  : ";
  for(i=0;i<8;i++)
    cout<<setw(6)<<tdc1_CRT[i];
  cout<<endl;
  cout<<"          tdc2  : ";
  for(i=0;i<8;i++)
    cout<<setw(6)<<tdc2_CRT[i];
  cout<<endl;
  cout<<"          tdc3  : ";
  for(i=0;i<8;i++)
    cout<<setw(6)<<tdc3_CRT[i];
  cout<<endl;
  cout<<"          adc1  : ";
  for(i=0;i<12;i++)
    cout<<setw(6)<<adc1_CRT[i];
  cout<<endl;
  cout<<"          adc2  : ";
  for(i=0;i<12;i++)
    cout<<setw(6)<<adc2_CRT[i];
  cout<<endl;
  cout<<"          scl1  : ";
  for(i=0;i<4;i++)
    cout<<setw(6)<<scl1_CRT[i];
  cout<<endl;

  //---tdc4---
  cout<<endl;
  cout<<"          tdc4  : ";
  for(i=0;i<12;i++)
    cout<<setw(6)<<tdc4_CRT[i];
  cout<<endl<<"                  ";
  for(i=12;i<24;i++)
    cout<<setw(6)<<tdc4_CRT[i];
  cout<<endl<<"                  ";
  for(i=24;i<32;i++)
    cout<<setw(6)<<tdc4_CRT[i];
  cout<<endl;

  //---tdc5---
  cout<<endl;
  cout<<"          tdc5  : ";
  for(i=0;i<12;i++)
    cout<<setw(6)<<tdc5_CRT[i];
  cout<<endl<<"                  ";
  for(i=12;i<24;i++)
    cout<<setw(6)<<tdc5_CRT[i];
  cout<<endl<<"                  ";
  for(i=24;i<32;i++)
    cout<<setw(6)<<tdc5_CRT[i];
  cout<<endl;

  //---tdc6---
  cout<<endl;
  cout<<"          tdc6  : ";
  for(i=0;i<12;i++)
    cout<<setw(6)<<tdc6_CRT[i];
  cout<<endl<<"                  ";
  for(i=12;i<24;i++)
    cout<<setw(6)<<tdc6_CRT[i];
  cout<<endl<<"                  ";
  for(i=24;i<32;i++)
    cout<<setw(6)<<tdc6_CRT[i];
  cout<<endl;


  //---tdc7---
  cout<<endl;
  cout<<"          tdc7  : ";
  for(i=0;i<12;i++)
    cout<<setw(6)<<tdc7_CRT[i];
  cout<<endl<<"                  ";
  for(i=12;i<24;i++)
    cout<<setw(6)<<tdc7_CRT[i];
  cout<<endl<<"                  ";
  for(i=24;i<32;i++)
    cout<<setw(6)<<tdc7_CRT[i];
  cout<<endl;


  //---tdc8---
  cout<<endl;
  cout<<"          tdc8  : ";
  for(i=0;i<12;i++)
    cout<<setw(6)<<tdc8_CRT[i];
  cout<<endl<<"                  ";
  for(i=12;i<24;i++)
    cout<<setw(6)<<tdc8_CRT[i];
  cout<<endl<<"                  ";
  for(i=24;i<32;i++)
    cout<<setw(6)<<tdc8_CRT[i];
  cout<<endl;

  //---tdc9---
  cout<<endl;
  cout<<"          tdc9  : ";
  for(i=0;i<12;i++)
    cout<<setw(6)<<tdc9_CRT[i];
  cout<<endl<<"                  ";
  for(i=12;i<24;i++)
    cout<<setw(6)<<tdc9_CRT[i];
  cout<<endl<<"                  ";
  for(i=24;i<32;i++)
    cout<<setw(6)<<tdc9_CRT[i];
  cout<<endl;


  cout<<endl;
  cout<<"          scl2  : ";
  for(i=0;i<12;i++)
    cout<<setw(8)<<scl2_CRT[i];
  cout<<endl;
  cout<<"          scl3  : ";
  for(i=0;i<12;i++)
    cout<<setw(8)<<scl3_CRT[i];
  cout<<endl;


  cout<<"          tdc10 : ";
  for(i=0;i<16;i++)
    cout<<setw(6)<<tdc10_CRT[i];
  cout<<endl;
  cout<<"          tdc11 : ";
  for(i=0;i<16;i++)
    cout<<setw(6)<<tdc11_CRT[i];
  cout<<endl;

  cout<<"          adc3  : ";
  for(i=0;i<12;i++)
    cout<<setw(6)<<adc3_CRT[i];
  cout<<endl;

  cout<<"          tdc12 : ";
  for(i=0;i<8;i++)
    cout<<setw(6)<<tdc12_CRT[i];
  cout<<endl;
  cout<<endl;


  cout<<"-------------------"
      <<"-------------------- FDIRC Data ---------------------"
      <<"-------------------"<<endl;
  
  for(i=0;i<7;i++){
    for(j=0;j<4;j++){
      cout<<"*******"<<endl;
      cout<<setw(9)<<"  slot  "<<setw(8)<<i<<endl;
      cout<<setw(9)<<"  blab  "<<setw(8)<<j<<endl;
      cout<<setw(9)<<"  temp  "<<setw(8)<<temp_fDIRC[i][j]<<endl
	  <<setw(9)<<" rovdd  "<<setw(8)<<rovdd_fDIRC[i][j]<<endl;
      
      //row
      cout<<setw(10)<<"row : ";
      for(k=0;k<16;k++)
	cout<<setw(6)<<row_fDIRC[i][j][k];
      cout<<endl;
      cout<<setw(8)<<"rowref "<<setw(8)<<rowref_fDIRC[i][j]<<endl;

      //col
      cout<<setw(10)<<"col : ";
      for(k=0;k<16;k++)
	cout<<setw(6)<<col_fDIRC[i][j][k];
      cout<<endl;
      cout<<setw(8)<<"colref "<<setw(8)<<colref_fDIRC[i][j]<<endl;

      //adc_fdirc
      cout<<"adc_fdirc : ";
      for(k=0;k<8;k++)
	cout<<setw(12)<<adc_fdirc_fDIRC[i][j][k];
      cout<<endl<<"            ";
      for(k=8;k<16;k++)
	cout<<setw(12)<<adc_fdirc_fDIRC[i][j][k];
      cout<<endl;

      //tdc_fdirc
      cout<<"tdc_fdirc : ";
      for(k=0;k<8;k++)
	cout<<setw(12)<<tdc_fdirc_fDIRC[i][j][k];
      cout<<endl<<"            ";
      for(k=8;k<16;k++)
	cout<<setw(12)<<tdc_fdirc_fDIRC[i][j][k];
      cout<<endl;


      //scaler
      cout<<setw(10)<<"scaler : ";
      for(k=0;k<8;k++)
	cout<<setw(8)<<scaler_fDIRC[i][j][k];
      cout<<endl<<"          ";
      for(k=8;k<16;k++)
	cout<<setw(8)<<scaler_fDIRC[i][j][k];
      cout<<endl;
      cout<<setw(8)<<"scalerref "<<setw(8)<<scalerref_fDIRC[i][j]<<endl;


      //placeholder
      cout<<setw(10)<<"placeholder1 : ";
      for(k=0;k<8;k++)
	cout<<setw(6)<<placeholder1_fDIRC[i][j][k];
      cout<<endl;
      cout<<setw(10)<<"placeholder2 : ";
      for(k=0;k<8;k++)
	cout<<setw(6)<<placeholder2_fDIRC[i][j][k];
      cout<<endl;

      cout<<endl;
      cout<<endl;
      cout<<endl;
    }
  }

  cout<<"****************************"<<endl;
}



Int_t crtFileStr::GetEntry(Long64_t entry)
{
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}

Long64_t crtFileStr::LoadTree(Long64_t entry)
{
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (!fChain->InheritsFrom(TChain::Class()))  return centry;
  TChain *chain = (TChain*)fChain;
  if (chain->GetTreeNumber() != fCurrent) {
    fCurrent = chain->GetTreeNumber();
  }
  return centry;
}

void crtFileStr::saveUnixTime2File(TString fileN){

  FILE *stream;
  stream = fopen(fileN.Data(), "w");

  Long64_t nentries = fChain->GetEntriesFast();
  cout<<"======================="<<endl;
  cout<<" crtFileStr::saveUnixTime2File : "<<fileN<<endl;
  cout<<" total number of event "<<nentries<<endl;
  cout<<"======================="<<endl;
  Long64_t jentry = 0;
  for (jentry=0; jentry<nentries;jentry++) {
    LoadTree(jentry);
    GetEntry(jentry);
    fprintf (stream, "%20.4f \n",time_CRT);
  }
  fclose(stream);
  
  //ofstream myfile;
  //myfile.open(fileN.Data());
  //Long64_t nentries = fChain->GetEntriesFast();
  //cout<<"======================="<<endl;
  //cout<<" crtFileStr::saveUnixTime2File : "<<fileN<<endl;
  //cout<<" total number of event "<<nentries<<endl;
  //cout<<"======================="<<endl;
  //Long64_t jentry = 0;
  //for (jentry=0; jentry<nentries;jentry++) {
  //LoadTree(jentry);
  //GetEntry(jentry);
  //myfile<<time_CRT<<endl;
  //}
  //myfile.close();

}


void crtFileStr::GetDataFromDetectors(){
  Int_t i = 0;
  for(i = 0;i<4;i++){
    trig1A[i] = adc1_CRT[i];
    trig2A[i] = adc1_CRT[i+4];

    trig1T[i] = tdc3_CRT[i];
    trig2T[i] = tdc3_CRT[i+4];

    stack1A[i] = adc1_CRT[i+8];
    stack2A[i] = adc2_CRT[i];
    stack3A[i] = adc2_CRT[i+4];
    stack4A[i] = adc2_CRT[i+8];

    stack1T[i] = tdc1_CRT[i];
    stack2T[i] = tdc1_CRT[i+4];
    stack3T[i] = tdc2_CRT[i];
    stack4T[i] = tdc2_CRT[i+4];

    start1T[i] = tdc10_CRT[i];
    start1A[i] = tdc11_CRT[i];
  }


  for(i = 0;i<55;i++){
    if(i<32){
      hodo1X[i] = tdc4_CRT[i];//TDC4 channels 0-31
      hodo2X[i] = tdc7_CRT[i];//TDC7 channels 0-31
    }
    else{
      hodo1X[i] = tdc5_CRT[i-32]; //TDC5 channels 0-22
      hodo2X[i] = tdc8_CRT[i-32]; //TDC8 channels 0-22
    }
  }

  for(i = 0;i<27;i++){
    if(i<8){
      hodo1Y[i] = tdc5_CRT[i+24]; //TDC5 channels 24-31
      hodo2Y[i] = tdc8_CRT[i+24]; //TDC8 channels 24-31
    }
    else{
      hodo1Y[i] = tdc6_CRT[i-8];  //TDC6 channels 0-18
      hodo2Y[i] = tdc9_CRT[i-8];  //TDC9 channels 0-18
    }  
  }

}




void crtFileStr::Init(TTree *tree)
{
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

   fChain->SetBranchAddress("time_CRT", &time_CRT, &b_time);
   fChain->SetBranchAddress("tdc1_CRT", tdc1_CRT, &b_tdc1);
   fChain->SetBranchAddress("tdc2_CRT", tdc2_CRT, &b_tdc2);
   fChain->SetBranchAddress("tdc3_CRT", tdc3_CRT, &b_tdc3);
   fChain->SetBranchAddress("adc1_CRT", adc1_CRT, &b_adc1);
   fChain->SetBranchAddress("adc2_CRT", adc2_CRT, &b_adc2);
   fChain->SetBranchAddress("scl1_CRT", scl1_CRT, &b_scl1);
   fChain->SetBranchAddress("tdc4_CRT", tdc4_CRT, &b_tdc4);
   fChain->SetBranchAddress("tdc5_CRT", tdc5_CRT, &b_tdc5);
   fChain->SetBranchAddress("tdc6_CRT", tdc6_CRT, &b_tdc6);
   fChain->SetBranchAddress("tdc7_CRT", tdc7_CRT, &b_tdc7);
   fChain->SetBranchAddress("tdc8_CRT", tdc8_CRT, &b_tdc8);
   fChain->SetBranchAddress("tdc9_CRT", tdc9_CRT, &b_tdc9);
   fChain->SetBranchAddress("scl2_CRT", scl2_CRT, &b_scl2);
   fChain->SetBranchAddress("scl3_CRT", scl3_CRT, &b_scl3);
   fChain->SetBranchAddress("tdc10_CRT", tdc10_CRT, &b_tdc10);
   fChain->SetBranchAddress("tdc11_CRT", tdc11_CRT, &b_tdc11);
   fChain->SetBranchAddress("adc3_CRT", adc3_CRT, &b_adc3);
   fChain->SetBranchAddress("tdc12_CRT", tdc12_CRT, &b_tdc12);
   fChain->SetBranchAddress("temp_fDIRC", temp_fDIRC, &b_temp);
   fChain->SetBranchAddress("rovdd_fDIRC", rovdd_fDIRC, &b_rovdd);
   fChain->SetBranchAddress("row_fDIRC", row_fDIRC, &b_row);
   fChain->SetBranchAddress("col_fDIRC", col_fDIRC, &b_col);
   fChain->SetBranchAddress("rowref_fDIRC", rowref_fDIRC, &b_rowref);
   fChain->SetBranchAddress("colref_fDIRC", colref_fDIRC, &b_colref);
   fChain->SetBranchAddress("scaler_fDIRC", scaler_fDIRC, &b_scaler);
   fChain->SetBranchAddress("scalerref_fDIRC", scalerref_fDIRC, &b_scalerref);
   fChain->SetBranchAddress("placeholder1_fDIRC", placeholder1_fDIRC, &b_placeholder1);
   fChain->SetBranchAddress("placeholder2_fDIRC", placeholder2_fDIRC, &b_placeholder2);
   fChain->SetBranchAddress("adc_fdirc_fDIRC", adc_fdirc_fDIRC, &b_adc_fdirc);
   fChain->SetBranchAddress("tdc_fdirc_fDIRC", tdc_fdirc_fDIRC, &b_tdc_fdirc);
}


void crtFileStr::Show(Long64_t entry)
{
   if (!fChain) return;
   fChain->Show(entry);
}

