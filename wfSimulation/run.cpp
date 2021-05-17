//my
#include "src/wfSim.hh"
#include "src/waveform.hh"

//root
#include "TROOT.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TRandom3.h"
#include "TGraph.h"
#include <TProfile.h>

//C, C++
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <iomanip>
#include <time.h>

using namespace std;

void getShapeFromFile(TString fileN, 
		      double *xxMcpPM, double *yyMcpPM, 
		      int &N, double &timeMax);

const Int_t nMcpPMmax = 100000;

int main(int argc, char *argv[]){

  clock_t start, finish;

  start = clock();

  if(argc == 5 && atoi(argv[1])==0){
    TString histF = argv[2];
    Int_t Nwf = atoi(argv[3]);
    Int_t MySeed = atoi(argv[4]);
    Int_t i = 0;
    Int_t j = 0;
    cout<<"--> wavefrom Sim test <--"<<endl
	<<" runID  = "<<atoi(argv[1])<<endl
	<<" histF  = "<<histF<<endl
	<<" Nwf    = "<<Nwf<<endl
	<<" MySeed = "<<MySeed<<endl;
    //Double_t xx[usbConst::usbwcNsamplingPoint];
    //Double_t yy[usbConst::usbwcNsamplingPoint];
    TRandom3 *rnd = new TRandom3(MySeed); 
    double t0;
    
    ////////HISTOS///////////
    TH1D *h1dTime = new TH1D("h1dTime","dTime",10000,-10.0,10.0);
    TH1D *h1Time_1 = new TH1D("h1Time_1","Time",100000, 0.0,80.0);
    TH1D *h1ampl_1 = new TH1D("h1ampl_1","ampl 1",300, 0.0,1.2);
    /////////////////////////

    Double_t time1;

    Double_t *wfNorAligA;
    Double_t *wfNorAligT;

    const Int_t nP = 256 + 255*5;

    Double_t wfNorAligA_avv[nP];
    Double_t wfNorAligT_avv[nP];

    for( i=0; i<nP; i++){
      wfNorAligA_avv[i] = 0.0;
      wfNorAligT_avv[i] = 0.0;
    }

    for( i=0; i<Nwf; i++){
      if(i%10000==0)
	cout<<i<<endl;

      t0 = rnd->Uniform(0.0,5.0);
      //t0 = 0.0;

      wfSim *wfS1 = new wfSim( rnd, 256, 0.3125, t0);
      //wfS1->genTriangleWF( 20.0, 2.3);
      wfS1->genGaussWF( 20.0, 2.3);
      wfS1->generateNoiseGauss(0.01);
      //waveform *wf1  = new waveform( wfS1->getWFT(), wfS1->getWFA(), 256);
      waveform *wf1  = new waveform( wfS1->getWFT(), wfS1->getWFA(), 256, 5); 
      wf1->SetSignalThreshold(60.0/1000.0);
      wf1->SetCrossTalkThreshold(-20.0/1000.0);
      wf1->findParametersOftheWaveform();
      wfNorAligA = new double[nP];
      wfNorAligT = new double[nP];  
      wf1->makeAlignmentAndNormalisation( wfNorAligT, wfNorAligA, wf1->GetTotAmplitudePosID(), 10.0, 1.0);
      for(j=0;j<nP;j++){
	wfNorAligA_avv[j] = wfNorAligA_avv[j] + wfNorAligA[j];
	wfNorAligT_avv[j] = wfNorAligT[j];
      }
      //waveform *wf1  = new waveform( wfS1->_wfReal, usbConst::usbwcdTimesampling); 
      //wfSim    *wfS2 = new wfSim(30.1,rnd, t0);
      //wfS2->addNextWF( 30.1 + rnd->Uniform(0.05,0.7), t0);
      //waveform *wf2  = new waveform( wfS2->_wfReal, usbConst::usbwcdTimesampling); 
      time1 = wf1->findFirstTimePosCFD(0.27);
      if( time1 != -999.0 ){
	//h1dTime->Fill(wf1->GetFirstTimePosCFD() - wf2->GetFirstTimePosCFD());
	h1Time_1->Fill(time1);
	//if(3.0<time1 && time1>4.0){
	//wfS1->Draw();
	//wf1->PrintWaveFormInfo();
	//return 0;
	//}
	//h1ampl_1->Fill(wf1->GetFirstAmplitudePos());
      }

      delete wfNorAligA;
      delete wfNorAligT;
      
      //if((wf1->GetFirstTimePosCFD() - wf2->GetFirstTimePosCFD())==0.0){
      //cout<<wf1->GetFirstTimePosCFD()<<endl
      //    <<wf2->GetFirstTimePosCFD()<<endl;
      //}

      //wf1->PrintWaveFormInfo();      

      delete wf1;
      delete wfS1;

      //delete wf2;
      //delete wfS2;
    }

    if(Nwf>0){
      for(j=0;j<nP;j++){
	wfNorAligA_avv[j] = wfNorAligA_avv[j]/Nwf;
      }
    }
    
    waveform *wf_avv  = new waveform( wfNorAligT_avv, wfNorAligA_avv, nP);
    wf_avv->findParametersOftheWaveform();
    wf_avv->findSamplingPeriod();
    wf_avv->PrintWaveFormInfo();

    TGraph *gr1 = new TGraph( nP, wfNorAligT_avv, wfNorAligA_avv); 
    gr1->SetName("gr1");
    //gr1->SetMaximum(3.0);
    //gr1->SetMinimum(-3.0);
    gr1->SaveAs("makeAlignmentAndNormalisation01.C");

    TFile* rootFile = new TFile(histF.Data(), "RECREATE", " Histograms from wfsim  ", 1);
    rootFile->cd();
    if (rootFile->IsZombie()){
      cout<<"  ERROR ---> file "<<histF<<" is zombi"<<endl;
      assert(0);
    }
    else
      cout<<"  Output Histos file ---> "<<histF<<endl;

    h1dTime->Write();
    h1Time_1->Write();
    h1ampl_1->Write();
  }
  else if(argc == 5 && atoi(argv[1])==1){

    TString histF = argv[2];
    Int_t Nwf = atoi(argv[3]);
    Int_t MySeed = atoi(argv[4]);
    Int_t i = 0;
    Int_t j = 0;
    cout<<"--> wavefrom SP, CT, CS test <--"<<endl
	<<" runID  = "<<atoi(argv[1])<<endl
	<<" histF  = "<<histF<<endl
	<<" Nwf    = "<<Nwf<<endl
	<<" MySeed = "<<MySeed<<endl;

    TRandom3 *rnd = new TRandom3(MySeed); 
    double t0;
    Double_t time1;
    //Double_t *wfNorAligA;
    //Double_t *wfNorAligT;
    const Int_t nSpline = 5;
    const Int_t nP = 256 + 255*nSpline;
    Int_t nINTatLevel = 0;
    double cfdL = 0.5;
    Int_t nPointsNoise = 15;
    Double_t signalThreshold = 70.0/1000.0;
    Double_t crossTalkThreshold =  -5.0/1000.0;
    //Double_t wfNorAligA_avv[nP];
    //Double_t wfNorAligT_avv[nP];
    //for( i=0; i<nP; i++){
    //wfNorAligA_avv[i] = 0.0;
    //wfNorAligT_avv[i] = 0.0;
    //}

    double xxMcpPM[nMcpPMmax];
    double yyMcpPM[nMcpPMmax];
    int nPiontsMcpPM;
    double timeMax;
    
    getShapeFromFile("./wfData/gr1SP_first.dat", 
    	     xxMcpPM, yyMcpPM,
    	     nPiontsMcpPM, timeMax);
    
    //getShapeFromFile("./wfData/gr1CT_first.dat", 
    //	     xxMcpPM, yyMcpPM,
    //	     nPiontsMcpPM, timeMax);

    //getShapeFromFile("./wfData/gr1CS_first.dat", 
    //	     xxMcpPM, yyMcpPM,
    //	     nPiontsMcpPM, timeMax);
    
    cout<<"nPiontsMcpPM "<<nPiontsMcpPM<<endl
	<<"timeMax      "<<timeMax<<endl;

    //for(i = 0;i<nPiontsMcpPM;i++){
    //cout<<setw(20)<<xxMcpPM[i]<<setw(20)<<yyMcpPM[i]<<endl;
    //}
  
    ////////HISTOS///////////
    TH1D *h1Charge = new TH1D("h1Charge","Charge",200,-2.0,10.0);
    TH1D *h1AmplitudeFirstPos = new TH1D("h1AmplitudeFirstPos","Amplitude First pos",1000,-0.2,1.5);
    TH1D *h1TimeFirstPos = new TH1D("h1TimeFirstPos","Time First pos",4000,-10.0,100);
    TH1D *h1RiseTimeFirstPos = new TH1D("h1RiseTimeFirstPos","Rise Time First Pos",300,0.0,2.0);
    TH1D *h1FallTimeFirstPos = new TH1D("h1FallTimeFirstPos","Fall Time First Pos",300,0.0,2.0);
    TH1D *h1FirstBasis = new TH1D("h1FirstBasis","First Basis",300,0.0,8.0);
    TH1D *h1FirstWidth = new TH1D("h1FirstWidth","First Width",300,0.0,4.0);
    TH1D *h1dTimeFirtsAmplPosChangLeftDer = new TH1D("h1dTimeFirtsAmplPosChangLeftDer","dTime Firts Ampl Pos and Chang Left Der",300,-5.0,10.0);
    TH1D *h1dTimeFirtsAmplPosFirstTimeAmplNeg = new TH1D("h1dTimeFirtsAmplPosFirstTimeAmplNeg","dTime Firts Ampl Pos First Time Ampl Neg",300,-10.0,10.0);
    TH1D *h1noise = new TH1D("h1noise","noise",4000,-1.0,1.0);
    TH1D *h1MaxBaseLineAmpl = new TH1D("h1MaxBaseLineAmpl","Max Base Line Ampl",4000,-1.0,1.0);
    TH1D *h1MinBaseLineAmpl = new TH1D("h1MinBaseLineAmpl","Min Base Line Ampl",4000,-1.0,1.0);
    TH1D *h1MaxAmplBeforeFirstChangDerAmpl = new TH1D("h1MaxAmplBeforeFirstChangDerAmpl","Max Ampl Before First Chang Der Ampl",4000,-1.0,1.0);
    TH1D *h1MinAmplBeforeFirstChangDerAmpl = new TH1D("h1MinAmplBeforeFirstChangDerAmpl","Min Ampl Before First Chang Der Ampl",4000,-1.0,1.0);
    /////////////////////////

    for( i=0; i<Nwf; i++){
      if(i%10000==0)
	cout<<i<<endl;

      t0 = rnd->Uniform(0.0,5.0);
      //t0 = 0.0;

      wfSim *wfS1 = new wfSim( rnd, 256, 0.3125, t0);
      
      wfS1->SetIdealShape_SP(nPiontsMcpPM, xxMcpPM, 
			     yyMcpPM, timeMax);
      wfS1->genMCPPMT_SP_WF(10.0);
      
      //wfS1->SetIdealShape_CT(nPiontsMcpPM, xxMcpPM, 
      //		     yyMcpPM, timeMax);
      //wfS1->genMCPPMT_CT_WF(10.0);

      //wfS1->SetIdealShape_CS(nPiontsMcpPM, xxMcpPM, 
      //		     yyMcpPM, timeMax);
      //wfS1->genMCPPMT_CS_WF(10.0);

      wfS1->generateNoiseGauss(0.0015);
      //waveform *wf1  = new waveform( wfS1->getWFT(), wfS1->getWFA(), 256);
      waveform *wf1  = new waveform( wfS1->getWFT(), wfS1->getWFA(), 256, nSpline); 
      wf1->SetSignalThreshold(signalThreshold);
      wf1->SetCrossTalkThreshold(crossTalkThreshold);
      wf1->findParametersOftheWaveform();
      //wfNorAligA = new double[nP];
      //wfNorAligT = new double[nP];  
      //wf1->makeAlignmentAndNormalisation( wfNorAligT, wfNorAligA, wf1->GetTotAmplitudePosID(), 10.0, 1.0);
      //for(j=0;j<nP;j++){
      //wfNorAligA_avv[j] = wfNorAligA_avv[j] + wfNorAligA[j];
      //wfNorAligT_avv[j] = wfNorAligT[j];
      //}
      //waveform *wf1  = new waveform( wfS1->_wfReal, usbConst::usbwcdTimesampling); 
      //wfSim    *wfS2 = new wfSim(30.1,rnd, t0);
      //wfS2->addNextWF( 30.1 + rnd->Uniform(0.05,0.7), t0);
      //waveform *wf2  = new waveform( wfS2->_wfReal, usbConst::usbwcdTimesampling); 
      time1 = wf1->findFirstTimePosCFD(cfdL);
      if( time1 != -999.0 ){
	if(wf1->GetWfID() == 1){
	  //nINTatLevel = wf1->findNumberOfPosIntersectionAtLevel(wf1->GetFirstAmplitudePos()*0.5);
	  //if(nINTatLevel == 1){
	    h1Charge->Fill(wf1->GetChargeTOT());
	    h1AmplitudeFirstPos->Fill(wf1->GetFirstAmplitudePos());
	    h1TimeFirstPos->Fill(time1);
	    h1RiseTimeFirstPos->Fill(wf1->GetFirstRiseTimePos());
	    h1FallTimeFirstPos->Fill(wf1->GetFirstFallTimePos());
	    h1FirstBasis->Fill(wf1->GetFirstTimeBasis());
	    h1FirstWidth->Fill(wf1->GetFirstWidthTimePos());
	    h1dTimeFirtsAmplPosChangLeftDer->Fill(wf1->GetdTimeFirtsAmplPosChangLeftDer());
	    h1dTimeFirtsAmplPosFirstTimeAmplNeg->Fill(wf1->GetdTimeFirtsAmplPosFirstTimeAmplNeg());
	    for(j = 0;j<nPointsNoise;j++){
	      h1noise->Fill(wfS1->getWFA()[j]);
	    }
	    h1MaxBaseLineAmpl->Fill(wf1->GetMaxBaseLineAmpl());
	    h1MinBaseLineAmpl->Fill(wf1->GetMinBaseLineAmpl());
	    h1MaxAmplBeforeFirstChangDerAmpl->Fill(wf1->GetMaxAmplBeforeFirstChangDerAmpl());
	    h1MinAmplBeforeFirstChangDerAmpl->Fill(wf1->GetMinAmplBeforeFirstChangDerAmpl());
	    //h1dTime->Fill(wf1->GetFirstTimePosCFD() - wf2->GetFirstTimePosCFD());
	    //if(3.0<time1 && time1>4.0){
	    //wfS1->Draw();
	    //wf1->PrintWaveFormInfo();
	    //return 0;
	    //}
	  //}
	}
      }

      //delete wfNorAligA;
      //delete wfNorAligT;
      
      //if((wf1->GetFirstTimePosCFD() - wf2->GetFirstTimePosCFD())==0.0){
      //cout<<wf1->GetFirstTimePosCFD()<<endl
      //    <<wf2->GetFirstTimePosCFD()<<endl;
      //}

      //wf1->PrintWaveFormInfo();      

      delete wf1;
      delete wfS1;

      //delete wf2;
      //delete wfS2;
    }

    //if(Nwf>0){
    //for(j=0;j<nP;j++){
    //wfNorAligA_avv[j] = wfNorAligA_avv[j]/Nwf;
    //}
    //}
    

    //waveform *wf_avv  = new waveform( wfNorAligT_avv, wfNorAligA_avv, nP);
    //wf_avv->findParametersOftheWaveform();
    //wf_avv->findSamplingPeriod();
    //wf_avv->PrintWaveFormInfo();

    //TGraph *gr1 = new TGraph( nP, wfNorAligT_avv, wfNorAligA_avv); 
    //gr1->SetName("gr1");
    //gr1->SetMaximum(3.0);
    //gr1->SetMinimum(-3.0);
    //gr1->SaveAs("makeAlignmentAndNormalisation01.C");

    TFile* rootFile = new TFile(histF.Data(), "RECREATE", " Histograms from wfsim  ", 1);
    rootFile->cd();
    if (rootFile->IsZombie()){
      cout<<"  ERROR ---> file "<<histF<<" is zombi"<<endl;
      assert(0);
    }
    else
      cout<<"  Output Histos file ---> "<<histF<<endl;

    h1Charge->Write();
    h1AmplitudeFirstPos->Write();
    h1TimeFirstPos->Write();
    h1RiseTimeFirstPos->Write();
    h1FallTimeFirstPos->Write();
    h1FirstBasis->Write();
    h1FirstWidth->Write();
    h1dTimeFirtsAmplPosChangLeftDer->Write();
    h1dTimeFirtsAmplPosFirstTimeAmplNeg->Write();
    h1noise->Write();
    h1MaxBaseLineAmpl->Write();
    h1MinBaseLineAmpl->Write();
    h1MaxAmplBeforeFirstChangDerAmpl->Write();
    h1MinAmplBeforeFirstChangDerAmpl->Write();

  }
  else if(argc == 5 && atoi(argv[1])==2){
    /////DTIMETEST/////
    TString histF = argv[2];
    Int_t Nwf = atoi(argv[3]);
    Int_t MySeed = atoi(argv[4]);
    Int_t i = 0;
    Int_t j = 0;
    cout<<"--> wavefrom dTime test <--"<<endl
	<<" runID  = "<<atoi(argv[1])<<endl
	<<" histF  = "<<histF<<endl
	<<" Nwf    = "<<Nwf<<endl
	<<" MySeed = "<<MySeed<<endl;

    TRandom3 *rnd = new TRandom3(MySeed); 
    Int_t   nEvOk = 0;
    double t0;
    double dTime;
    double dTimeConstL;
    double dTimeDL;
    double ctdt;
    Double_t time1;
    Double_t time2;
    Double_t time1_01V;
    Double_t time2_04V;
    Double_t time1_DL;
    Double_t time2_DL;

    const Int_t nSpline = 5;
    const Int_t nP      = 256 + 255*nSpline;
    Int_t nINTatLevel   = 0;
    double cfdL         = 0.55;
    Int_t nPointsNoise  = 15;
    Double_t noiseRMS   = 0.0013;
    Double_t noiseRMS_n;
    //Double_t noiseRMS   = 0.0;

    //Double_t signalThreshold = 30.0/1000.0;
    //Double_t crossTalkThreshold = -10.0/1000.0;
    //Double_t wfNorAligA_avv[nP];
    //Double_t wfNorAligT_avv[nP];
    //for( i=0; i<nP; i++){
    //wfNorAligA_avv[i] = 0.0;
    //wfNorAligT_avv[i] = 0.0;
    //}

    double xxMcpPM_SP[nMcpPMmax];
    double yyMcpPM_SP[nMcpPMmax];
    int nPiontsMcpPM_SP;
    double timeMax_SP;

    double xxMcpPM_CT[nMcpPMmax];
    double yyMcpPM_CT[nMcpPMmax];
    int nPiontsMcpPM_CT;
    double timeMax_CT;

    double xxMcpPM_CS[nMcpPMmax];
    double yyMcpPM_CS[nMcpPMmax];
    int nPiontsMcpPM_CS;
    double timeMax_CS;
    
    getShapeFromFile("./wfData/gr1SP_first.dat", 
		     xxMcpPM_SP, yyMcpPM_SP,
		     nPiontsMcpPM_SP, timeMax_SP);
    cout<<" --> nPiontsMcpPM_SP "<<nPiontsMcpPM_SP<<endl
	<<" --> timeMax_SP      "<<timeMax_SP<<endl;
    
    getShapeFromFile("./wfData/gr1CT_first.dat", 
		     xxMcpPM_CT, yyMcpPM_CT,
		     nPiontsMcpPM_CT, timeMax_CT);
    cout<<" --> nPiontsMcpPM_CT "<<nPiontsMcpPM_CT<<endl
	<<" --> timeMax_CT      "<<timeMax_CT<<endl;
    
    getShapeFromFile("./wfData/gr1CS_first.dat", 
		     xxMcpPM_CS, yyMcpPM_CS,
		     nPiontsMcpPM_CS, timeMax_CS);
    cout<<" --> nPiontsMcpPM_CS "<<nPiontsMcpPM_CS<<endl
	<<" --> timeMax_CS      "<<timeMax_CS<<endl;

    //for(i = 0;i<nPiontsMcpPM;i++){
    //cout<<setw(20)<<xxMcpPM[i]<<setw(20)<<yyMcpPM[i]<<endl;
    //}
  
    ////////HISTOS///////////
    TH1D *h1RiseTime_1          = new TH1D(     "h1RiseTime_1",            "Rise Time 1",100, 0.0,5.0);
    TH1D *h1RiseTime_2          = new TH1D(     "h1RiseTime_2",            "Rise Time 2",100, 0.0,5.0);
    TH1D *h1TimeFirstPos_1      = new TH1D(     "h1TimeFirstPos_1",            "Time First pos 1",10000,-10.0,100.0);
    TH1D *h1TimeFirstPos_2      = new TH1D(     "h1TimeFirstPos_2",            "Time First pos 2",10000,-10.0,100.0);
    TH1D *h1TimeFirstPos_01V_1  = new TH1D(     "h1TimeFirstPos_01V_1",       "h1TimeFirstPos_01V_1",10000,-10.0,100.0);
    TH1D *h1TimeFirstPos_04V_2  = new TH1D(     "h1TimeFirstPos_01V_2",       "h1TimeFirstPos_01V_2",10000,-10.0,100.0);
    TH1D *h1TimeFirstPos_DL_1  = new TH1D(     "h1TimeFirstPos_DL_1",       "h1TimeFirstPos_DL_1",10000,-10.0,100.0);
    TH1D *h1TimeFirstPos_DL_2  = new TH1D(     "h1TimeFirstPos_DL_2",       "h1TimeFirstPos_DL_2",10000,-10.0,100.0);
    TH1D *h1dTimeFirstPos       = new TH1D(      "h1dTimeFirstPos",        "delta Time First pos",10000,-10.0, 10.0);
    TH1D *h1dTimeFirstPos_zoom  = new TH1D( "h1dTimeFirstPos_zoom",   "delta Time First pos zoom",400, -0.1, 0.1);
    TH1D *h1dTimeFirstConstLPos = new TH1D( "h1dTimeFirstConstLPos",   "delta Time First ConstL Pos",10000,-10.0, 10.0);
    TH1D *h1dTimeFirstDLPos = new TH1D( "h1dTimeFirstDLPos",   "delta Time First DL Pos",10000,-10.0, 10.0);
    TH1D *h1FirstAmplitudePos_1 = new TH1D("h1FirstAmplitudePos_1",       "First Amplitude Pos 1", 3000, -5.0,  5.0);
    TH1D *h1FirstAmplitudePos_2 = new TH1D("h1FirstAmplitudePos_2",       "First Amplitude Pos 2", 3000, -5.0,  5.0);
    TH1D *h1FirstAmplitudePos_2_zoom = new TH1D("h1FirstAmplitudePos_2_zoom",       "First Amplitude Pos 2 zoom", 3000,  0.05,  0.15);
    TH1D *h1FirstAmplitudeNeg_1 = new TH1D("h1FirstAmplitudeNeg_1",       "First Amplitude Pos 1", 3000, -5.0,  5.0);
    TH1D *h1FirstAmplitudeNeg_2 = new TH1D("h1FirstAmplitudeNeg_2",       "First Amplitude Pos 2", 3000, -5.0,  5.0);

    TH1D *h1SNRv = new TH1D("h1SNRv", "SNR",1000,0.0,1000.0);

    TH2D *h2dTimeFirstPosVsT0   = new TH2D(  "h2dTimeFirstPosVsT0",  "delta Time First pos vs T0",  300, -1.0,6.0,  200,-0.1,0.1);
    TH2D *h2dTimeFirstPosCTct   = new TH2D(  "h2dTimeFirstPosCTct","delta Time First pos vs CTct",  600, -5.0,5.0,10000,-2.5,2.5);
    TH2D *h2dTime_VS_Ampl       = new TH2D(  "h2dTime_VS_Ampl","h2dTime_VS_Ampl",  200, -0.0,0.6, 400,-0.4,1.2);
    TH2D *h2dTime_VS_Ampl_constL= new TH2D(  "h2dTime_VS_Ampl_constL","dTime VS Ampl constL",  200, -0.0,0.6, 400,-0.4,1.2);
    TH2D *h2dTime_VS_Ampl_DL    = new TH2D(  "h2dTime_VS_Ampl_DL","dTime VS Ampl DL",  200, -0.0,0.6, 400,-0.4,1.2);

    Option_t *optProff("s");
    TProfile *prof_dT_vs_SNR = new TProfile("prof_dT_vs_SNR","prof dT vs SNR", 400, 0.0, 1000.0, -0.1, 0.1, optProff);
    Double_t SNRv;
    /////////////////////////

    t0 = rnd->Uniform(0.0,5.0);
    wfSim *wfS1 = new wfSim( rnd, 256, 0.3125, t0);
    wfSim *wfS2 = new wfSim( rnd, 256, 0.3125, t0);    
    wfS1->SetIdealShape_SP(nPiontsMcpPM_SP, xxMcpPM_SP, 
			   yyMcpPM_SP, timeMax_SP);
    wfS1->SetIdealShape_CT(nPiontsMcpPM_CT, xxMcpPM_CT, 
			   yyMcpPM_CT, timeMax_CT);
    wfS1->SetIdealShape_CS(nPiontsMcpPM_CS, xxMcpPM_CS, 
			   yyMcpPM_CS, timeMax_CS);
    wfS2->SetIdealShape_SP(nPiontsMcpPM_SP, xxMcpPM_SP, 
			   yyMcpPM_SP, timeMax_SP);
    wfS2->SetIdealShape_CT(nPiontsMcpPM_CT, xxMcpPM_CT, 
			   yyMcpPM_CT, timeMax_CT);
    wfS2->SetIdealShape_CS(nPiontsMcpPM_CS, xxMcpPM_CS, 
			   yyMcpPM_CS, timeMax_CS);
    
    for( i=0; i<Nwf; i++){
      if(i%10000==0)
	cout<<i<<endl;

      t0 = rnd->Uniform(0.0,5.0);
      wfS1->SetDigitTime0(t0);
      wfS2->SetDigitTime0(t0);
      
      //t0 = 0.0;
      
      //triangle
      //wfS1->genTriangleWF( 20.0, 2.3);
      //wfS1->genTriangleWF( 20.0, 2.3);
      //wfS2->genTriangleWF( 20.0, 2.3);
      //wfS2->genTriangleWF( 20.0, 2.3);
      
      //gaus
      //wfS1->genGaussWF( 20.0, 2.3);
      //wfS1->genGaussWF( 20.0, 2.3);
      //wfS2->genGaussWF( 20.0, 2.3);
      //wfS2->genGaussWF( 20.0, 2.3);
      
      //real WF

      ctdt = rnd->Uniform(-1.0,1.0);
      //ctdt = 0.0;
      wfS1->genMCPPMT_SP_WF(10.0 + rnd->Gaus(0.0,8.0/1000.0)); //1.003     //0.1
      //wfS1->genMCPPMT_SP_WF(10.0); //0.6697    //0.2
      //wfS1->genMCPPMT_SP_WF(10.0); //0.4422    //0.3
      //wfS1->genMCPPMT_SP_WF(10.0); //0.2576    //0.4
      //wfS1->genMCPPMT_SP_WF(10.0); //0.0939    //0.5
      //wfS1->genMCPPMT_SP_WF(10.0); //-0.06031  //0.6
      //wfS1->genMCPPMT_SP_WF(10.0); //-0.2139   //0.7
      //wfS1->genMCPPMT_SP_WF(10.0); //-0.   //0.8
      //wfS1->genMCPPMT_SP_WF(10.0 - 0.108 + 0.014);
      //wfS1->genGaussWFAmpl( 11.0 + rnd->Gaus(0.0,8.0/1000.0), 1.5, 0.8);
      //wfS1->genGaussWF(10.0+1.316, 1.77);
      //wfS1->genGaussWF(10.0+0.974, 1.48);
      //wfS1->genGaussWF(10.0+0.62, 1.18);
      //wfS1->genGaussWF(10.0+0.62-0.338, 0.89);
      //wfS1->genGaussWF(10.0-0.108, 0.56);
      //wfS1->genGaussWF(10.0-0.404, 0.3);
      //wfS1->genGaussWF(10.0, 0.3);
      //wfS2->genMCPPMT_SP_WF(10.0);
      //wfS1->genMCPPMT_SP_WF(10.0 + ctdt);
      //wfS1->genMCPPMT_CT_WF(10.0 + ctdt);
      //wfS1->genMCPPMT_CS_WF(10.0 + ctdt);
      //wfS2->genMCPPMT_CS_WF(10.0 - 0.0675);
      //wfS2->genMCPPMT_SP_WF(10.0);
      //wfS2->genGaussWF(10.0, 0.65);
      wfS2->genGaussWFAmpl( 11.0 + 0.0163, 1.5, 0.8);
      //wfS2->genGaussWFAmpl( 11.0, 0.56, 0.1);
      //wfS2->genGaussWF(10.0, 0.3);
      //wfS2->genMCPPMT_CT_WF(10.0);
      //wfS2->genMCPPMT_CS_WF(10.0);
      
      if(noiseRMS != 0.0){
	//noiseRMS_n  = (noiseRMS*rnd->Uniform(0.1,7.0));
	noiseRMS_n  = noiseRMS;
	//cout<<noiseRMS_n<<endl;
	wfS1->generateNoiseGauss(noiseRMS_n);
	wfS2->generateNoiseGauss(noiseRMS_n);
      }

      //waveform *wf1  = new waveform( wfS1->getWFT(), wfS1->getWFA(), 256);
      //waveform *wf2  = new waveform( wfS2->getWFT(), wfS2->getWFA(), 256);
      waveform *wf1  = new waveform( wfS1->getWFT(), wfS1->getWFA(), 256, nSpline);
      waveform *wf2  = new waveform( wfS2->getWFT(), wfS2->getWFA(), 256, nSpline);
      //wf1->SetSignalThreshold(signalThreshold);
      //wf1->SetCrossTalkThreshold(crossTalkThreshold);
      wf1->findParametersOftheWaveform();
      //wf2->SetSignalThreshold(signalThreshold);
      //wf2->SetCrossTalkThreshold(crossTalkThreshold);
      wf2->findParametersOftheWaveform();
      //wfNorAligA = new double[nP];
      //wfNorAligT = new double[nP];  
      //wf1->makeAlignmentAndNormalisation( wfNorAligT, wfNorAligA, wf1->GetTotAmplitudePosID(), 10.0, 1.0);
      //for(j=0;j<nP;j++){
      //wfNorAligA_avv[j] = wfNorAligA_avv[j] + wfNorAligA[j];
      //wfNorAligT_avv[j] = wfNorAligT[j];
      //}
      //waveform *wf1  = new waveform( wfS1->_wfReal, usbConst::usbwcdTimesampling); 
      //wfSim    *wfS2 = new wfSim(30.1,rnd, t0);
      //wfS2->addNextWF( 30.1 + rnd->Uniform(0.05,0.7), t0);
      //waveform *wf2  = new waveform( wfS2->_wfReal, usbConst::usbwcdTimesampling); 
      time1 = wf1->findFirstTimePosCFD(cfdL);
      time2 = wf2->findFirstTimePosCFD(cfdL);
      time1_01V = wf1->findFirstTimePosAtLevelRiseEdge(0.075);
      time2_04V = wf2->findFirstTimePosAtLevelRiseEdge(0.4);
      time1_DL  = wf1->findFirstTimePosAtDoubleLevelRiseEdge(0.04, 0.075);
      time2_DL  = wf2->findFirstTimePosAtDoubleLevelRiseEdge(0.25, 0.55);
      dTime = time1 - time2;
      dTimeConstL = time1_01V -  time2_04V;
      dTimeDL = time1_DL - time2_DL;
      if( time1 != -999.0 && time2 != -999.0 ){
	if(wf1->GetWfID() == 1 && wf2->GetWfID() == 1){
	  h1TimeFirstPos_1->Fill(time1);
	  h1TimeFirstPos_2->Fill(time2);
	  h1TimeFirstPos_01V_1->Fill(time1_01V);
	  h1TimeFirstPos_04V_2->Fill(time2_04V);
	  h1dTimeFirstPos->Fill(dTime);
	  h1dTimeFirstConstLPos->Fill(dTimeConstL);
	  h1dTimeFirstPos_zoom->Fill(dTime);
	  h2dTime_VS_Ampl->Fill(wf1->GetFirstAmplitudePos(),dTime);
	  h2dTime_VS_Ampl_constL->Fill(wf1->GetFirstAmplitudePos(),dTimeConstL);

	  h1TimeFirstPos_DL_1->Fill(time1_DL);
	  h1TimeFirstPos_DL_2->Fill(time2_DL);
	  h1dTimeFirstDLPos->Fill(dTimeDL);
	  h2dTime_VS_Ampl_DL->Fill(wf1->GetFirstAmplitudePos(),dTimeDL);

	  SNRv = wf1->GetFirstAmplitudePos()/noiseRMS_n;
	  h1SNRv->Fill(SNRv);
	  //cout<<SNRv<<endl;
	  prof_dT_vs_SNR->Fill(SNRv,dTime);

	  h2dTimeFirstPosVsT0->Fill(t0,dTime);
	  h2dTimeFirstPosCTct->Fill(ctdt,dTime);
	  h1FirstAmplitudePos_1->Fill(wf1->GetFirstAmplitudePos());
	  h1FirstAmplitudePos_2->Fill(wf2->GetFirstAmplitudePos());
	  h1FirstAmplitudePos_2_zoom->Fill(wf2->GetFirstAmplitudePos());
	  h1FirstAmplitudeNeg_1->Fill(wf1->GetFirstAmplitudeNeg());
	  h1FirstAmplitudeNeg_2->Fill(wf2->GetFirstAmplitudeNeg());

	  h1RiseTime_1->Fill(wf1->GetFirstRiseTimePos());
	  h1RiseTime_2->Fill(wf2->GetFirstRiseTimePos());
	  nEvOk++;
	}
      }
      //if((wf1->GetFirstTimePosCFD() - wf2->GetFirstTimePosCFD())==0.0){
      //cout<<wf1->GetFirstTimePosCFD()<<endl
      //    <<wf2->GetFirstTimePosCFD()<<endl;
      //}

      wfS1->CleanWf();
      wfS2->CleanWf();

      //wf1->PrintWaveFormInfo();      

      delete wf1;
      delete wf2;
    }

    delete wfS1;
    delete wfS2;


    TFile* rootFile = new TFile(histF.Data(), "RECREATE", " Histograms from wfsim  ", 1);
    rootFile->cd();
    if (rootFile->IsZombie()){
      cout<<"  ERROR ---> file "<<histF<<" is zombi"<<endl;
      assert(0);
    }
    else
      cout<<"  Output Histos file ---> "<<histF<<endl;

    h1TimeFirstPos_1->Write();
    h1TimeFirstPos_2->Write();
    h1TimeFirstPos_01V_1->Write();
    h1TimeFirstPos_04V_2->Write();
    h1TimeFirstPos_DL_1->Write();
    h1TimeFirstPos_DL_2->Write();
    h1dTimeFirstPos->Write();
    h1dTimeFirstPos_zoom->Write();
    h1dTimeFirstConstLPos->Write();
    h1dTimeFirstDLPos->Write();
    h2dTime_VS_Ampl->Write();
    h2dTime_VS_Ampl_constL->Write();
    h2dTime_VS_Ampl_DL->Write();
    h1FirstAmplitudePos_1->Write();
    h1FirstAmplitudePos_2->Write();
    h1FirstAmplitudePos_2_zoom->Write();
    h1FirstAmplitudeNeg_1->Write();
    h1FirstAmplitudeNeg_2->Write();
    h2dTimeFirstPosVsT0->Write();
    h2dTimeFirstPosCTct->Write();
    h1RiseTime_1->Write();
    h1RiseTime_2->Write();
    prof_dT_vs_SNR->Write();
    h1SNRv->Write();

    if(Nwf>0){
      cout<<"nEvOk     = "<<nEvOk<<endl
	  <<"Nwf       = "<<Nwf<<endl
	  <<"nEvOk/Nwf = "<<(Double_t)nEvOk/((Double_t)Nwf)<<endl;
    }
  }
  else{
    cout<<"  wavefrom Sim test               "<<endl
	<<"  runID [1] = 0                   "<<endl
	<<"        [2] - file with histos    "<<endl
	<<"        [3] - number of waveforms "<<endl
	<<"        [4] - seed                "<<endl;
    cout<<"  wavefrom SP, CT, CS test        "<<endl
	<<"  runID [1] = 1                   "<<endl
	<<"        [2] - file with histos    "<<endl
	<<"        [3] - number of waveforms "<<endl
	<<"        [4] - seed                "<<endl;
    cout<<"  wavefrom dTime test             "<<endl
	<<"  runID [1] = 2                   "<<endl
	<<"        [2] - file with histos    "<<endl
	<<"        [3] - number of waveforms "<<endl
	<<"        [4] - seed                "<<endl;
  }


  finish = clock();

  cout<<" time of work "<<((finish - start)/CLOCKS_PER_SEC )<<endl;


  return 0;
}

void getShapeFromFile(TString fileN, 
		      double *xxMcpPM, double *yyMcpPM, 
		      int &N, double &timeMax){
  ifstream indata;
  string mot;
  int kk;
  double x;
  double y;
  indata.open(fileN.Data());
  if(!indata) { // file couldn't be opened
    cout<<" ERROR --> file : "<< fileN<<" could not be opened"<<endl;
    assert(0);
  }
  indata>>mot;
  indata>>mot;
  indata>>N;
  indata>>mot;
  indata>>mot;
  indata>>timeMax;
  if(nMcpPMmax<N){
    cout<<" ERROR --> nMcpPMmax<N"<<endl
	<<"             nMcpPMmax "<<nMcpPMmax<<endl
	<<"                     N "<<N<<endl;
    assert(0);
  }
  //xxMcpPM = new double[N];
  //yyMcpPM = new double[N];
  for(Int_t i = 0;i<N;i++){
    indata>>kk>>x>>y;
    xxMcpPM[i] = x;
    yyMcpPM[i] = y;
    //cout<<setw(10)<<kk<<setw(20)<<xxMcpPM[i]<<setw(20)<<yyMcpPM[i]<<endl;
  }
  indata.close();
}
