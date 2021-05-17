//my
#include "usbwcFileStr.hh"
#include "../anaConst.hh"
#include "../wfSimulation/src/waveform.hh"

//root
#include <TH2.h>
#include <TH2D.h>
#include <TH1D.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TGraphErrors.h>

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

usbwcFileStr::usbwcFileStr(){
  _Rate = -999.0;
  _unixTimeBegin = -999.0;
  _unixTimeEnd = -999.0;
}

void usbwcFileStr::addFilesToChain(string name){
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

usbwcFileStr::~usbwcFileStr(){
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

void usbwcFileStr::Loop(TString histFName){
  
  /////////////////////////////
  GetDataRate();
  /////////////////////////////

  Int_t i = 0;
  Int_t k = 0;
  Double_t prevUnixTime = 0.0;
  
  TH1D *h1Charge[anaConst::nChannels];
  TH1D *h1Time[anaConst::nChannels];
  TH1D *h1Amplitude[anaConst::nChannels];
  TH1D *h1dTime[anaConst::nChannels];
  TH1D *h1BaseLineAmpl[anaConst::nChannels];

  initH1_F(h1Charge,"h1Charge","charge",200,-10000,20000);
  initH1_F(h1Time,"h1Time","Time",200,-10.0,100);
  initH1_F(h1dTime,"h1dTime","delta Time",1000,-5.0,5.0);
  initH1_F(h1Amplitude,"h1Amplitude","Amplitude",100,-1.5,1.5);
  initH1_F(h1BaseLineAmpl,"h1BaseLineAmpl","BaseLineAmpl",1000,-0.35,-0.25);


  ///////////////
  TH1D *h1dUnixTime = new TH1D("h1dUnixTime","delta Unix Time USBWC",5000,0.0,40.0);
  TF1 *f1expo = new TF1("f1expo","expo",1.5,30.0);

  // Fix the warning due to the ambiguous definition of ntimeBin
  /*
  Int_t ntimeBin = (Int_t)(_unixTimeEnd - _unixTimeBegin)/3600.0;
  if(ntimeBin == 0)
    ntimeBin = 1;
  TH1D *h1dUnixTimeInTime = new TH1D("h1dUnixTimeInTime","dUnix Time (between neighbor events) as function of unix time",ntimeBin,0.0,(_unixTimeEnd - _unixTimeBegin)/3600.0);
  */
  Int_t ntimeBin = (Int_t)( ceil( (_unixTimeEnd - _unixTimeBegin)/3600.0 ) );
  if(ntimeBin == 0) { ntimeBin = 1; }
  TH1D *h1dUnixTimeInTime = 
    new TH1D("h1dUnixTimeInTime",
	     "dUnix Time (between neighbor events) as function of unix time",
	     ntimeBin,0.0,ceil( (_unixTimeEnd - _unixTimeBegin)/3600.0 ) );

  h1dUnixTimeInTime->SetMaximum(10.0);
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

  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  cout<<"======================="<<endl;
  cout<<" total number of event "<<nentries<<endl;
  cout<<"======================="<<endl;
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry); nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    if(jentry%10000==0)
      cout<<" processed events "<<jentry<<endl;    

    //////Unix time///////
    h1dUnixTime->Fill(UnixTime - prevUnixTime);
    if(uTimeStart == -999.0){
      uTimeStart = UnixTime;
      uTimeStop = uTimeStart + 3600.0;
      uTimeCenter = ((uTimeStop - uTimeStart)/2.0 + uTimeStart - _unixTimeBegin)/3600.0;
      delete h1dUnixTime1hour;
      h1dUnixTime1hour = new TH1D("h1dUnixTime1hour"," dUnix Time (between neighbor events) in one hour",100,0.0,30.0);
      //cout<<"uTimeStart  "<<uTimeStart<<endl
      //<<"uTimeStop   "<<uTimeStop<<endl
      //<<"uTimeCenter "<<uTimeCenter<<endl;
    }

    if(UnixTime>=uTimeStart && UnixTime<uTimeStop){
      h1dUnixTime1hour->Fill(UnixTime - prevUnixTime);
    }
    else{
      //h1dUnixTime1hour->Fit("expo");
      //fill dUnix Time histogram as function of time 
      //k = 1;
      h1dUnixTime1hour->Fit(f1expo,"Q","",1.5,30.0);
      for(k = 1;k<=ntimeBin;k++){
	h1dUnixTimeInTime_LowEdge = h1dUnixTimeInTime->GetBinLowEdge(k);
	h1dUnixTimeInTime_UpEdge  = h1dUnixTimeInTime_LowEdge + h1dUnixTimeInTime->GetBinWidth(k);
	//cout<<"h1dUnixTimeInTime_LowEdge          "<<h1dUnixTimeInTime_LowEdge<<endl
	//<<"uTimeCenter                        "<<uTimeCenter<<endl
	//<<"h1dUnixTimeInTime_UpEdge           "<<h1dUnixTimeInTime_UpEdge<<endl
	//<<"h1dUnixTimeInTime->GetBinWidth(k)  "<<h1dUnixTimeInTime->GetBinWidth(k)<<endl;
	if(h1dUnixTimeInTime_LowEdge <= uTimeCenter && h1dUnixTimeInTime_UpEdge>uTimeCenter ){
	  //h1dUnixTimeInTime->SetBinContent(k,h1dUnixTime1hour->GetRMS());
	  //h1dUnixTimeInTime->SetBinError(k,h1dUnixTime1hour->GetRMSError());
	  if(f1expo->GetParameter(1)!=0.0)
	    halfLifeTime = TMath::Abs(TMath::Log(2.0)/(f1expo->GetParameter(1)));
	  else
	    halfLifeTime = 0.0;
	  
	  halfLifeTimeErr = halfLifeTime - TMath::Abs(TMath::Log(2.0)/(f1expo->GetParameter(1)+f1expo->GetParError(1)));
	  h1dUnixTimeInTime->SetBinContent(k,halfLifeTime);
	  h1dUnixTimeInTime->SetBinError(k,halfLifeTimeErr);
	  k = ntimeBin+1;
	  //cout<<"h1dUnixTime1hour->GetRMS() = "<<h1dUnixTime1hour->GetRMS()<<endl;
	}
      }
      uTimeStart = -999.0;
    }
    prevUnixTime = UnixTime;
    //////////////////////

    for(i = 0;i<anaConst::nChannels;i++){
      h1Charge[i]->Fill(Charge[i]);
      h1Time[i]->Fill(Time[i]);
      h1Amplitude[i]->Fill(Amplitude[i]);
    }

  }

  TFile* rootFile = new TFile(histFName.Data(), "RECREATE", " Histograms from USBWC", 1);
  rootFile->cd();
  if (rootFile->IsZombie()){
    cout<<"  ERROR ---> file "<<histFName<<" is zombi"<<endl;
    assert(0);
  }
  else
    cout<<"  Output Histos file ---> "<<histFName<<endl;

  for(i = 0;i<anaConst::nChannels;i++){
    h1Charge[i]->Write();
    h1Time[i]->Write();
    h1Amplitude[i]->Write();
    h1BaseLineAmpl[i]->Write();
    h1dTime[i]->Write();
  }

  h1dUnixTime->Write();
  h1dUnixTimeInTime->Write();

  /*
  TString namePS = histFName + ".ps";
  TString nameO  = namePS+"[";
  TString nameC  = namePS+"]";

  TCanvas *c1 = new TCanvas("c1"," CRT plots",10,10,600,800);
  c1->Print(nameO.Data()); 

  for(Int_t j=0;j<4;j++){
    c1->Clear();
    c1->SetFillColor(kWhite);
    c1->Divide(2,2);
    for(i=0;i<4;i++){
      c1->cd(i+1);
      c1->GetPad(i+1)->SetLogy();
      h1Time[4*j + i]->SetLineWidth(2);
      h1Time[4*j + i]->Draw();
    }
    c1->Print(namePS.Data());
  }

  for(Int_t j=0;j<4;j++){
    c1->Clear();
    c1->SetFillColor(kWhite);
    c1->Divide(2,2);
    for(i=0;i<4;i++){
      c1->cd(i+1);
      c1->GetPad(i+1)->SetLogy();
      h1Charge[4*j + i]->SetLineWidth(2);
      h1Charge[4*j + i]->Draw();
    }
    c1->Print(namePS.Data());
  }

  for(Int_t j=0;j<4;j++){
    c1->Clear();
    c1->SetFillColor(kWhite);
    c1->Divide(2,2);
    for(i=0;i<4;i++){
      c1->cd(i+1);
      c1->GetPad(i+1)->SetLogy();
      h1Amplitude[4*j + i]->SetLineWidth(2);
      h1Amplitude[4*j + i]->Draw();
    }
    c1->Print(namePS.Data());
  }

  c1->Clear();
  c1->SetFillColor(kWhite);
  h1dUnixTime->SetLineWidth(2);
  h1dUnixTime->Draw();
  c1->Print(namePS.Data());
  c1->Clear();

  c1->SetFillColor(kWhite);
  h1dUnixTimeInTime->Draw();
  c1->Print(namePS.Data());

  c1->Print(nameC.Data());
*/

}

void usbwcFileStr::LaserAna(TString histFName){
  
  cout<<"--> LaserAna <--"<<endl;
  
  /////////////////////////////
  GetDataRate();
  /////////////////////////////

  Int_t nWFsave = 0;

  Int_t i = 0;
  Int_t j = 0;
  Double_t wfMyA[anaConst::usbwcNsamplingPoint];
  Double_t wfMyT[anaConst::usbwcNsamplingPoint];

  waveform *wf;

  //for(i = 0;i<100;i++){
  //for(j = 1;j<2; j++){
  //cout<<"idealCosmicWF_t[i][j] = "<<idealCosmicWF_t[i][j]<<endl
  //  <<"idealCosmicWF_A[i][j] = "<<idealCosmicWF_A[i][j]<<endl;
  //}
  //}
  //return;

  //Double_t tIdeal_C[100];
  //Double_t aIdeal_C[100];

  TH1D *h1Charge[anaConst::nChannels];
  TH1D *h1ChargeSP[anaConst::nChannels];
  TH1D *h1ChargeFirstSP[anaConst::nChannels];
  TH1D *h1RiseTimeSP[anaConst::nChannels];
  TH1D *h1TimePos[anaConst::nChannels];
  TH1D *h1AmplitudePos[anaConst::nChannels];
  TH1D *h1AmplitudeTotMax[anaConst::nChannels];
  TH1D *h1TimeNeg[anaConst::nChannels];
  TH1D *h1AmplitudeNeg[anaConst::nChannels];
  TH1D *h1waveformID[anaConst::nChannels];
  TH1D *h1dTime[anaConst::nChannels];
  TH1D *h1dTime_L[anaConst::nChannels];
  TH1D *h1dTime_L_CUT[anaConst::nChannels];
  TH1D *h1dTime_L2[anaConst::nChannels];
  TH1D *h1dTime_Spline[anaConst::nChannels];
  TH1D *h1dTime_Spline_chi2[anaConst::nChannels];
  TH1D *h1dTimeSP_Charge[anaConst::nChannels];
  TH1D *h1dTimeCT[anaConst::nChannels];
  TH1D *h1Chi2[anaConst::nChannels];

  TH2D *h2RiseTime_vs_Charge[anaConst::nChannels];
  TH2D *h2waveFormNorm[anaConst::nChannels];

  TH2D *h2dTimeL_vs_dRisetime[anaConst::nChannels];
  TH2D *h2dTimeL_vs_Risetime[anaConst::nChannels];

  TH2D *h2dTimeCH2_vs_dRiseTime = new TH2D("h2dTimeCH2_vs_dRiseTime","dTimeCH2 vs dRiseTime",200,-1.5,1.5,1000,-5.0,5.0);
  TH2D *h2dTimeCH3_vs_dRiseTime = new TH2D("h2dTimeCH3_vs_dRiseTime","dTimeCH3 vs dRiseTime",200,-1.5,1.5,1000,-5.0,5.0);
  TH2D *h2dTimeCH4_vs_dRiseTime = new TH2D("h2dTimeCH4_vs_dRiseTime","dTimeCH4 vs dRiseTime",200,-1.5,1.5,1000,-5.0,5.0);
  TH2D *h2dTimeCH5_vs_dRiseTime = new TH2D("h2dTimeCH5_vs_dRiseTime","dTimeCH5 vs dRiseTime",200,-1.5,1.5,1000,-5.0,5.0);

  TH2D *h2dTimeCH3_vs_RiseTime = new TH2D("h2dTimeCH3_vs_RiseTime","dTimeCH3 vs RiseTime",300,0.0,3.0,1000,-5.0,5.0);

  TH2D *h2dTimeCH3_vs_Charge1Charge2 = new TH2D("h2dTimeCH3_vs_Charge1Charge2","dTimeCH3 vs Charge1+Charge2",1000,0.0,10.0,1000,-5.0,5.0);

  Double_t myTimeSP[anaConst::nChannels];
  Double_t myTimeSP_Spline[anaConst::nChannels];
  Double_t myTimeCT[anaConst::nChannels];
  Double_t myRiseTimeSP[anaConst::nChannels];
  Double_t myChargeSP[anaConst::nChannels];
  Double_t myChi2SP[anaConst::nChannels];

  initH1_F(h1Charge,"h1Charge","charge",200,-2.0,10.0);
  initH1_F(h1Chi2,"h1Chi2","Chi2",20000,-0.1,20.0);
  initH1_F(h1ChargeSP,"h1ChargeSP","charge",200,-2.0,10.0);
  initH1_F(h1RiseTimeSP,"h1RiseTimeSP","Rise Time SP",300,0.0,2.0);
  initH1_F(h1ChargeFirstSP,"h1ChargeFirstSP","charge",200,-2.0,10.0);
  initH1_F(h1TimePos,"h1TimePos","Time pos",20000,-10.0,100);
  initH1_F(h1AmplitudePos,"h1AmplitudePos","Amplitude pos",1000,-0.2,1.5);
  initH1_F(h1AmplitudeTotMax,"h1AmplitudeTotMax","Amplitude Tot Max",1000,-0.2,1.5);
  initH1_F(h1TimeNeg,"h1TimeNeg","Time neg",2000,-10.0,100);
  initH1_F(h1AmplitudeNeg,"h1AmplitudeNeg","Amplitude neg",1000,-1.5,0.2);
  initH1_F(h1waveformID,"h1waveformID","waveform ID",10,-2.0,4.0);
  initH1_F(h1dTime,"h1dTime","delta Time",20000,-10.0,10.0);
  initH1_F(h1dTime_L,"h1dTime_L","delta Time L",20000,-10.0,10.0);
  initH1_F(h1dTime_L_CUT,"h1dTime_L_CUT","delta Time L CUT",2000,-10.0,10.0);
  initH1_F(h1dTime_L2,"h1dTime_L2","delta Time L2",20000,-10.0,10.0);
  initH1_F(h1dTime_Spline,"h1dTime_Spline","delta Time Spline",1000,-5.0,5.0);
  initH1_F(h1dTime_Spline_chi2,"h1dTime_Spline_chi2","delta Time Spline chi2",1000,-5.0,5.0);
  initH1_F(h1dTimeSP_Charge,"h1dTimeSP_Charge","delta Time SP cut on Charge",1000,-5.0,5.0);
  initH1_F(h1dTimeCT,"h1dTimeCT"," including dTimeCT ",1000,-5.0,5.0);

  initH2_F(h2RiseTime_vs_Charge, "h2RiseTime_vs_Charge"," RiseTime vs Charge",
	   1000, 0.0, 10.0, 
	   1000, 0.0, 5.0);
  initH2_F(h2waveFormNorm, "h2waveFormNorm"," waveform norm ",
	   2500, 0.0, 80.0, 
	   1500, -1.0, 4.0);
  initH2_F(h2dTimeL_vs_dRisetime, "h2dTimeL_vs_dRisetime"," dTimeL vs dRisetime ",
	   400,   -3.0,  3.0, 
	   2000, -10.0, 10.0);
  initH2_F(h2dTimeL_vs_Risetime, "h2dTimeL_vs_Risetime"," dTimeL vs Risetime ",
	   400,   0.0,  5.0, 
	   2000, -10.0, 10.0);
  
  const Int_t nSplinePoint = 5;
  double cfdVal = 0.5;
  Double_t signalThreshold = 30.0/1000.0;
  Double_t crossTalkThreshold = -10.0/1000.0;
  const Int_t nP = anaConst::usbwcNsamplingPoint + (anaConst::usbwcNsamplingPoint-1)*nSplinePoint;
  
  Double_t *wfNorAligA;
  Double_t *wfNorAligT;

  Double_t wfNorAligA_avv[nP];
  Double_t wfNorAligT_avv[nP];

  for( i=0; i<nP; i++){
    wfNorAligA_avv[i] = 0.0;
    wfNorAligT_avv[i] = 0.0;
  }

  Int_t nch3 = 0;

  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  cout<<"======================="<<endl;
  cout<<" total number of event "<<nentries<<endl;
  cout<<"======================="<<endl;
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    //if (jentry > 1000) break;
    nb = fChain->GetEntry(jentry); nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    if(jentry%1000==0)
      cout<<" processed events "<<jentry<<endl;    

    for(i = 0;i<anaConst::nChannels;i++){
      myTimeSP[i] = -999.0;
      myRiseTimeSP[i] = -999.0;
      myTimeCT[i] = -999.0;
      myTimeSP_Spline[i] = -999.0;
      myChargeSP[i] = -999.0;
      myChi2SP[i] = -999.0;
    }

    for(i = 0;i<anaConst::nChannels;i++){

      for(j = 0;j<anaConst::usbwcNsamplingPoint;j++){
	wfMyA[j] = ch[i][j];
	wfMyT[j] = j*SamplingPeriod/1000.0;
      }
      wf = new waveform(wfMyT,wfMyA,anaConst::usbwcNsamplingPoint,nSplinePoint);
      wf->SetSignalThreshold(signalThreshold);
      wf->SetCrossTalkThreshold(crossTalkThreshold);
      wf->findParametersOftheWaveform();
      //cout<<"wf->GetChi2()  "<<wf->GetChi2()<<endl;
      //return;
      h1Charge[i]->Fill(wf->GetChargeTOT());
      h1AmplitudeTotMax[i]->Fill(wf->GetTotAmplitudePos());
      //if(wf->GetWfID()==1 && wf->GetFirstAmplitudePos()> 0.06){
      if(wf->GetWfID()==1){
	//if(wf->GetWfID()==1){
	//if(wf->GetWfID()==1 && wf->GetFirstAmplitudePos()> 0.05 && wf->GetFirstAmplitudePos()< 0.3){
	//if(wf->GetFirstRiseTimePos()>0.9 && wf->GetFirstRiseTimePos()< 1.1){
	//if(i==3 && nWFsave<10){
	//TString wffName = "../../idealWF_full/spline3_ch3_";
	//wffName += nWFsave;
	//wffName += ".C";
	//wf->SaveSplineFunction(wffName);
	//nWFsave++;
	//}
	//else if (nWFsave>=10){
	//return;
	//}
	//h1Chi2[i]->Fill(wf->GetChi2());
	//myChi2SP[i] = wf->GetChi2();
	//h1ChargeSP[i]->Fill(wf->GetChargeTOT());
	//h1ChargeFirstSP[i]->Fill(wf->GetChargeFirst());
	h1RiseTimeSP[i]->Fill(wf->GetFirstRiseTimePos());
	myTimeSP[i] = wf->findFirstTimePosCFD(cfdVal);
	//myTimeSP_Spline[i] = wf->
	//cout<<"(myTimeSP[i] - myTimeSP_Spline[i])  "
	//<<(myTimeSP[i] - myTimeSP_Spline[i])<<endl;
	myRiseTimeSP[i] = wf->GetFirstRiseTimePos();
	myChargeSP[i] = wf->GetChargeTOT();
	h2RiseTime_vs_Charge[i]->Fill(wf->GetChargeTOT(),wf->GetFirstRiseTimePos());
	h1TimePos[i]->Fill(wf->findFirstTimePosCFD(cfdVal));
	h1AmplitudePos[i]->Fill(wf->GetFirstAmplitudePos());
	//h1TimeNeg[i]->Fill(wf->);
	h1AmplitudeNeg[i]->Fill(wf->GetFirstAmplitudeNeg());
	if(i == 3){
	  wfNorAligA = new double[nP];
	  wfNorAligT = new double[nP];  
	  wf->makeAlignmentAndNormalisation( wfNorAligT, wfNorAligA, wf->GetTotAmplitudePosID(), 10.0, 0.5);
	  for(j=0;j<nP;j++){
	    wfNorAligA_avv[j] = wfNorAligA_avv[j] + wfNorAligA[j];
	    wfNorAligT_avv[j] = wfNorAligT[j];
	  }
	  delete wfNorAligA;
	  delete wfNorAligT;
	  nch3++;
	}
      }
      
      h1waveformID[i]->Fill(wf->GetWfID());

      delete wf;
    }


    for(i = 0;i<8;i++){
      
      if(myTimeSP[2*i]!=-999.0 && myTimeSP[2*i+1]!=-999.0 &&
	 myRiseTimeSP[2*i] !=-999.0 && myRiseTimeSP[2*i+1]!=-999.0){
	h2dTimeL_vs_dRisetime[i]->Fill((myRiseTimeSP[2*i] - myRiseTimeSP[2*i+1]),
				       (myTimeSP[2*i] - myTimeSP[2*i+1]));
      }      

      if(TMath::Abs(myRiseTimeSP[2*i] - myRiseTimeSP[2*i+1])<0.15){
	if(myTimeSP[2*i]!=-999.0 && myTimeSP[2*i+1]!=-999.0 &&
	   myRiseTimeSP[2*i] !=-999.0 && myRiseTimeSP[2*i+1]!=-999.0){
	  if(myRiseTimeSP[2*i]>0.9 && myRiseTimeSP[2*i]<1.2){
	    if(myRiseTimeSP[2*i+1]>0.9 && myRiseTimeSP[2*i+1]<1.2){
	      h2dTimeL_vs_Risetime[i]->Fill(myRiseTimeSP[2*i], (myTimeSP[2*i] - myTimeSP[2*i+1]));
	      h1dTime_L_CUT[i]->Fill(myTimeSP[2*i] - myTimeSP[2*i+1]);	  
	    }
	  }
	}
      }
      
      if(myTimeSP[2*i]!=-999.0 && myTimeSP[2*i+1]!=-999.0){
	//if(myRiseTimeSP[2*i]>0.95 && myRiseTimeSP[2*i]<1.05 && myRiseTimeSP[2*i+1]>0.95 && myRiseTimeSP[2*i+1]<1.05){
	h1dTime_L[i]->Fill(myTimeSP[2*i] - myTimeSP[2*i+1]);	  
	//cout<<"i = "<<i<<" jentry "<<jentry<<"    myTimeSP[2*i] - myTimeSP[2*i+1]  "<< myTimeSP[2*i] - myTimeSP[2*i+1]<<endl;
	//}
      }

      if(i<3){
	if(myTimeSP[2*i+1]!=-999.0 && myTimeSP[2*i+2]!=-999.0){
	  if(myRiseTimeSP[2*i+1]>0.9 && myRiseTimeSP[2*i+1]<1.2 && myRiseTimeSP[2*i+2]>0.9 && myRiseTimeSP[2*i+2]<1.2){
	    h1dTime_L2[i]->Fill(myTimeSP[2*i+1] - myTimeSP[2*i+2]);
	  }
	}
      }

      if(i>3 && i< 7){
	if(myTimeSP[2*i+1]!=-999.0 && myTimeSP[2*i+2]!=-999.0){
	  if(myRiseTimeSP[2*i+1]>0.9 && myRiseTimeSP[2*i+1]<1.2 && myRiseTimeSP[2*i+2]>0.9 && myRiseTimeSP[2*i+2]<1.2){
	    h1dTime_L2[i]->Fill(myTimeSP[2*i+1] - myTimeSP[2*i+2]);
	  }
	}
      }


      //if(myTimeSP[2*i]!=-999.0 && myTimeSP[2*i+1]!=-999.0){
      //if(myRiseTimeSP[2*i]>0.9 && myRiseTimeSP[2*i]<1.2 && myRiseTimeSP[2*i+1]>0.9 && myRiseTimeSP[2*i+1]<1.2){
      //  h1dTime_L[i]->Fill(myTimeSP[2*i] - myTimeSP[2*i+1]);	  
      //}
      //}



      if(myTimeSP[i]!=-999.0 && myTimeSP[i+8]!=-999.0){
	//if(myRiseTimeSP[i]>0.9 && myRiseTimeSP[i]<1.2){
	//if(myRiseTimeSP[i+8]>0.9 && myRiseTimeSP[i+8]<1.2){
	//if( TMath::Abs(myRiseTimeSP[i]- myRiseTimeSP[i+8])<0.02){
	h1dTime[i]->Fill(myTimeSP[i] - myTimeSP[i+8]);
	h1dTimeCT[i]->Fill(myTimeSP[i] - myTimeSP[i+8]);
	h1dTime_Spline[i]->Fill(myTimeSP_Spline[i] - myTimeSP_Spline[i+8]);
	if(myChi2SP[i] != -999.0 && myChi2SP[i+8] != -999.0){
	  if(myChi2SP[i] < 0.005 && myChi2SP[i+8] < 0.005){
	    h1dTime_Spline_chi2[i]->Fill(myTimeSP_Spline[i] - myTimeSP_Spline[i+8]);
	  }
	}
	//}
	//}
	//}
	
	if(myChargeSP[i] < 1.7 && myChargeSP[i+8] < 1.7){
	  //cout<<" myChargeSP[i]                "<<myChargeSP[i]<<endl
	  //<<"(myTimeSP[i] - myTimeSP[i+8]) "<<(myTimeSP[i] - myTimeSP[i+8])<<endl;
	  h1dTimeSP_Charge[i]->Fill(myTimeSP[i] - myTimeSP[i+8]);
	}
	
	if(i==2)
	  h2dTimeCH2_vs_dRiseTime->Fill( (myRiseTimeSP[i] - myRiseTimeSP[i+8]) , myTimeSP[i] - myTimeSP[i+8]);
	
	if(i==3){
	  h2dTimeCH3_vs_dRiseTime->Fill( (myRiseTimeSP[i] - myRiseTimeSP[i+8]) , myTimeSP[i] - myTimeSP[i+8]);
	  h2dTimeCH3_vs_RiseTime->Fill( myRiseTimeSP[i] , myTimeSP[i] - myTimeSP[i+8]);
	  
	  h2dTimeCH3_vs_Charge1Charge2->Fill(myChargeSP[i] + myChargeSP[i+8], myTimeSP[i] - myTimeSP[i+8]);
	  
	}
	
	if(i==4)
	  h2dTimeCH4_vs_dRiseTime->Fill( (myRiseTimeSP[i] - myRiseTimeSP[i+8]) , myTimeSP[i] - myTimeSP[i+8]);
	if(i==5)
	  h2dTimeCH5_vs_dRiseTime->Fill( (myRiseTimeSP[i] - myRiseTimeSP[i+8]) , myTimeSP[i] - myTimeSP[i+8]);
	
      }
      
      if(myTimeCT[i]!=-999.0 && myTimeCT[i+8]!=-999.0){
	if(myTimeCT[i]!=myTimeCT[i+8]){
	  h1dTimeCT[i]->Fill(myTimeCT[i] - myTimeCT[i+8]);
	  //if((myTimeCT[i] - myTimeCT[i+8])==0.0){
	  //cout<<"myTimeCT[i]   = "<<myTimeCT[i]<<endl
	  //    <<"myTimeCT[i+8] = "<<myTimeCT[i+8]<<endl;
	  //}
	}
      }
      
    }

  }

  if(nch3>0){
    for(j=0;j<nP;j++){
      wfNorAligA_avv[j] = wfNorAligA_avv[j]/nch3;
    }
  }

  //TGraph *gr1 = new TGraph( nP, wfNorAligT_avv, wfNorAligA_avv); 
  //gr1->SetName("gr1");
  //gr1->SetMaximum(3.0);
  //gr1->SetMinimum(-3.0);
  //gr1->SaveAs("makeAlignmentAndNormalisation01_ch03.C");


  TFile* rootFile = new TFile(histFName.Data(), "RECREATE", " Histograms from USBWC  ", 1);
  rootFile->cd();
  if (rootFile->IsZombie()){
    cout<<"  ERROR ---> file "<<histFName<<" is zombi"<<endl;
    assert(0);
  }
  else
    cout<<"  Output Histos file ---> "<<histFName<<endl;

  for(i = 0;i<anaConst::nChannels;i++){
    h1Charge[i]->Write();
    h1ChargeSP[i]->Write();
    h1ChargeFirstSP[i]->Write();
    h1RiseTimeSP[i]->Write();
    h1TimePos[i]->Write();
    h1AmplitudePos[i]->Write();
    h1AmplitudeTotMax[i]->Write();
    h1TimeNeg[i]->Write();
    h1AmplitudeNeg[i]->Write();
    h1waveformID[i]->Write();
    h1dTime[i]->Write();
    h1dTime_L[i]->Write();
    h1dTime_L2[i]->Write();
    h1dTime_Spline[i]->Write();
    h1dTimeCT[i]->Write();
    h1dTime_Spline_chi2[i]->Write();
    h1dTimeSP_Charge[i]->Write();
    h2RiseTime_vs_Charge[i]->Write();
    h2waveFormNorm[i]->Write();
    h1Chi2[i]->Write();
    h2dTimeL_vs_dRisetime[i]->Write();

    h2dTimeL_vs_Risetime[i]->Write();
    h1dTime_L_CUT[i]->Write();

    //h1BaseLineAmpl[i]->Write();
  }

  h2dTimeCH2_vs_dRiseTime->Write();
  h2dTimeCH3_vs_dRiseTime->Write();
  h2dTimeCH4_vs_dRiseTime->Write();
  h2dTimeCH5_vs_dRiseTime->Write();
  h2dTimeCH3_vs_RiseTime->Write();
  h2dTimeCH3_vs_Charge1Charge2->Write();

  /*
  TString namePS = histFName + ".ps";
  TString nameO  = namePS+"[";
  TString nameC  = namePS+"]";

  TCanvas *c1 = new TCanvas("c1"," CRT plots",10,10,600,800);
  c1->Print(nameO.Data()); 

  for(Int_t j=0;j<4;j++){
    c1->Clear();
    c1->SetFillColor(kWhite);
    c1->Divide(2,2);
    for(i=0;i<4;i++){
      c1->cd(i+1);
      c1->GetPad(i+1)->SetLogy();
      h1Time[4*j + i]->SetLineWidth(2);
      h1Time[4*j + i]->Draw();
    }
    c1->Print(namePS.Data());
  }

  for(Int_t j=0;j<4;j++){
    c1->Clear();
    c1->SetFillColor(kWhite);
    c1->Divide(2,2);
    for(i=0;i<4;i++){
      c1->cd(i+1);
      c1->GetPad(i+1)->SetLogy();
      h1Charge[4*j + i]->SetLineWidth(2);
      h1Charge[4*j + i]->Draw();
    }
    c1->Print(namePS.Data());
  }

  for(Int_t j=0;j<4;j++){
    c1->Clear();
    c1->SetFillColor(kWhite);
    c1->Divide(2,2);
    for(i=0;i<4;i++){
      c1->cd(i+1);
      c1->GetPad(i+1)->SetLogy();
      h1Amplitude[4*j + i]->SetLineWidth(2);
      h1Amplitude[4*j + i]->Draw();
    }
    c1->Print(namePS.Data());
  }

  c1->Print(nameC.Data());
  */

}

void usbwcFileStr::CTAna(TString histFName){
  cout<<"--> CTAna <--"<<endl;  
  GetDataRate();

  ////////////////////
  Int_t i = 0;
  Int_t j = 0;
  waveform *wf[anaConst::nChannels];
  Double_t wfMyA[anaConst::usbwcNsamplingPoint];
  Double_t wfMyT[anaConst::usbwcNsamplingPoint];
  const Int_t nSplinePoint = 0;
  const Int_t nP = anaConst::usbwcNsamplingPoint + (anaConst::usbwcNsamplingPoint-1)*nSplinePoint;
  Double_t cfdVal = 0.5;
  Double_t cfdAllVal = 1.0;
  Int_t nPointsNoise = 15;
  Int_t chADAll = 3;
  Double_t signalThreshold = 30.0/1000.0;
  Double_t crossTalkThreshold = -10.0/1000.0;
  Double_t mydTime;
  Double_t wfNorAligA[nP];
  Double_t wfNorAligT[nP];
  Double_t wfNorAligA_SP_Avv[nP];
  Double_t wfNorAligT_SP_Avv[nP];
  Double_t wfNorAligA_CT_Avv[nP];
  Double_t wfNorAligT_CT_Avv[nP];
  Double_t wfNorAligA_CS_Avv[nP];
  Double_t wfNorAligT_CS_Avv[nP];
  Double_t timeSP = -999.0;
  Double_t timeCT = -999.0;
  Double_t time01CS = -999.0;
  Double_t time02CS = -999.0;
  Int_t nINTatLevel = 0;
  Int_t nINTatLevelCS01 = 0;
  Int_t nINTatLevelCS02 = 0;
  Int_t nCT = 0;
  Int_t nSP = 0;
  Int_t nCS = 0;
  for(j = 0;j<nP;j++){
    wfNorAligA_CT_Avv[j] = 0.0;
    wfNorAligT_CT_Avv[j] = 0.0;
    wfNorAligA_SP_Avv[j] = 0.0;
    wfNorAligT_SP_Avv[j] = 0.0;
    wfNorAligA_CS_Avv[j] = 0.0;
    wfNorAligT_CS_Avv[j] = 0.0;
  }
  j=0;

  Double_t sigAmplTh[anaConst::nChannels];
  sigAmplTh[0]  = 0.09;   //V
  sigAmplTh[1]  = 0.095;
  sigAmplTh[2]  = 0.06;
  sigAmplTh[3]  = 0.07;
  sigAmplTh[4]  = 0.09;
  sigAmplTh[5]  = 0.115;
  sigAmplTh[6]  = 0.075;
  sigAmplTh[7]  = 0.08;
  sigAmplTh[8]  = 0.09;
  sigAmplTh[9]  = 0.09;
  sigAmplTh[10] = 0.09;
  sigAmplTh[11] = 0.09;
  sigAmplTh[12] = 0.09;
  sigAmplTh[13] = 0.09;
  sigAmplTh[14] = 0.09;
  sigAmplTh[15] = 0.09;

  //ns
  Double_t timingWindowLeft[anaConst::nChannels];
  timingWindowLeft[0]  = 20.0;
  timingWindowLeft[1]  = 20.0;
  timingWindowLeft[2]  = 20.0;
  timingWindowLeft[3]  = 20.0;
  timingWindowLeft[4]  = 20.0;
  timingWindowLeft[5]  = 20.0;
  timingWindowLeft[6]  = 20.6;
  timingWindowLeft[7]  = 20.6;
  timingWindowLeft[8]  = 19.0;
  timingWindowLeft[9]  = 19.0;
  timingWindowLeft[10] = 19.0;
  timingWindowLeft[11] = 19.0;
  timingWindowLeft[12] = 19.0;
  timingWindowLeft[13] = 19.0;
  timingWindowLeft[14] = 19.0;
  timingWindowLeft[15] = 19.0;

  //ns
  Double_t timingWindowRight[anaConst::nChannels];
  timingWindowRight[0]  = 26.0;
  timingWindowRight[1]  = 26.0;
  timingWindowRight[2]  = 26.0;
  timingWindowRight[3]  = 26.0;
  timingWindowRight[4]  = 26.0;
  timingWindowRight[5]  = 26.0;
  timingWindowRight[6]  = 26.6;
  timingWindowRight[7]  = 26.6;
  timingWindowRight[8]  = 27.0;
  timingWindowRight[9]  = 27.0;
  timingWindowRight[10] = 27.0;
  timingWindowRight[11] = 27.0;
  timingWindowRight[12] = 27.0;
  timingWindowRight[13] = 27.0;
  timingWindowRight[14] = 27.0;
  timingWindowRight[15] = 27.0;

  //Int_t wfIDMaxAmpl = -999;
  //Int_t wfIDSecondMaxAmpl = -999;
  //Double_t maxAmpl = 0.0;

  TH1D *h1ChargeSP[anaConst::nChannels];
  TH1D *h1AmplitudeFirstPosSP[anaConst::nChannels];
  TH1D *h1TimeFirstPosSP[anaConst::nChannels];
  TH1D *h1RiseTimeFirstPosSP[anaConst::nChannels];
  TH1D *h1FallTimeFirstPosSP[anaConst::nChannels];
  TH1D *h1FirstBasisSP[anaConst::nChannels];
  TH1D *h1FirstWidthSP[anaConst::nChannels];
  TH1D *h1dTimeFirtsAmplPosChangLeftDerSP[anaConst::nChannels];
  TH1D *h1dTimeFirtsAmplPosFirstTimeAmplNegSP[anaConst::nChannels];
  TH1D *h1noiseSP[anaConst::nChannels];
  TH1D *h1MaxBaseLineAmplSP[anaConst::nChannels];
  TH1D *h1MinBaseLineAmplSP[anaConst::nChannels];
  TH1D *h1MaxAmplBeforeFirstChangDerAmplSP[anaConst::nChannels];
  TH1D *h1MinAmplBeforeFirstChangDerAmplSP[anaConst::nChannels];

  TH1D *h1ChargeCT[anaConst::nChannels];
  TH1D *h1AmplitudeFirstPosCT[anaConst::nChannels];
  TH1D *h1TimeFirstPosCT[anaConst::nChannels];
  TH1D *h1RiseTimeFirstPosCT[anaConst::nChannels];
  TH1D *h1FallTimeFirstPosCT[anaConst::nChannels];
  TH1D *h1FirstBasisCT[anaConst::nChannels];
  TH1D *h1FirstWidthCT[anaConst::nChannels];
  TH1D *h1dTimeFirtsAmplPosChangLeftDerCT[anaConst::nChannels];
  TH1D *h1dTimeFirtsAmplPosFirstTimeAmplNegCT[anaConst::nChannels];

  TH1D *h1ChargeCS[anaConst::nChannels];
  TH1D *h1AmplitudeFirstPosCS[anaConst::nChannels];
  TH1D *h1AmplitudeSigPOSDIVbyAmplitudeSigCS[anaConst::nChannels];
  TH1D *h1TimeFirstPosCS[anaConst::nChannels];
  TH1D *h1RiseTimeFirstPosCS[anaConst::nChannels];
  TH1D *h1FallTimeFirstPosCS[anaConst::nChannels];
  TH1D *h1FirstBasisCS[anaConst::nChannels];
  TH1D *h1FirstWidthCS[anaConst::nChannels];
  TH1D *h1dTimeFirtsAmplPosChangLeftDerCS[anaConst::nChannels];
  TH1D *h1dTimeFirtsAmplPosFirstTimeAmplNegCS[anaConst::nChannels];
  TH2D *h2AmplitudeFirstPosSIGvsAmplitudeFirstPosCS;

  TH1D *h1dTimeSigCTLeft[anaConst::nChannels];

  TH1D *h1TimeFirstPosAll = new TH1D("h1TimeFirstPosAll","Time First Pos All",8000,0.0,80.0);
  ////////////////////

  /////////////////////////////////////////////////////////////
  initH1_F(h1ChargeSP,"h1ChargeSP","Charge SP",200,-2.0,10.0);
  initH1_F(h1AmplitudeFirstPosSP,"h1AmplitudeFirstPosSP","Amplitude First pos SP",1000,-0.2,1.5);
  initH1_F(h1TimeFirstPosSP,"h1TimeFirstPosSP","Time First pos SP",4000,-10.0,100);
  initH1_F(h1RiseTimeFirstPosSP,"h1RiseTimeFirstPosSP","Rise Time First Pos SP",300,0.0,2.0);
  initH1_F(h1FallTimeFirstPosSP,"h1FallTimeFirstPosSP","Fall Time First Pos SP",300,0.0,2.0);
  initH1_F(h1FirstBasisSP,"h1FirstBasisSP","First Basis SP",300,0.0,8.0);
  initH1_F(h1FirstWidthSP,"h1FirstWidthSP","First Width SP",300,0.0,4.0);
  initH1_F(h1dTimeFirtsAmplPosChangLeftDerSP,"h1dTimeFirtsAmplPosChangLeftDerSP"," dTime Firts Ampl Pos and Chang Left Der SP",300,-5.0,10.0);
  initH1_F(h1dTimeFirtsAmplPosFirstTimeAmplNegSP,"h1dTimeFirtsAmplPosFirstTimeAmplNegSP","dTime Firts Ampl Pos First Time Ampl Neg SP",300,-10.0,10.0);
  initH1_F(h1noiseSP,"h1noiseSP","noise SP",4000,-1.0,1.0);
  initH1_F(h1MaxBaseLineAmplSP,"h1MaxBaseLineAmplSP","Max Base Line Ampl SP",4000,-1.0,1.0);
  initH1_F(h1MinBaseLineAmplSP,"h1MinBaseLineAmplSP","Min Base Line Ampl SP",4000,-1.0,1.0);
  initH1_F(h1MaxAmplBeforeFirstChangDerAmplSP,"h1MaxAmplBeforeFirstChangDerAmplSP","Max Ampl Before First Chang Der Ampl SP",4000,-1.0,1.0);
  initH1_F(h1MinAmplBeforeFirstChangDerAmplSP,"h1MinAmplBeforeFirstChangDerAmplSP","Min Ampl Before First Chang Der Ampl SP",4000,-1.0,1.0);

  initH1_F(h1ChargeCT,"h1ChargeCT","Charge CT",200,-2.0,10.0);
  initH1_F(h1AmplitudeFirstPosCT,"h1AmplitudeFirstPosCT","Amplitude First pos CT",1000,-0.2,1.5);
  initH1_F(h1TimeFirstPosCT,"h1TimeFirstPosCT","Time First pos CT",4000,-10.0,100);
  initH1_F(h1RiseTimeFirstPosCT,"h1RiseTimeFirstPosCT","Rise Time First Pos CT",300,0.0,2.0);
  initH1_F(h1FallTimeFirstPosCT,"h1FallTimeFirstPosCT","Fall Time First Pos CT",300,0.0,2.0);
  initH1_F(h1FirstBasisCT,"h1FirstBasisCT","First Basis CT",300,0.0,8.0);
  initH1_F(h1FirstWidthCT,"h1FirstWidthCT","First Width CT",300,0.0,4.0);
  initH1_F(h1dTimeFirtsAmplPosChangLeftDerCT,"h1dTimeFirtsAmplPosChangLeftDerCT"," dTime Firts Ampl Pos and Chang Left Der CT",300,-5.0,10.0);
  initH1_F(h1dTimeFirtsAmplPosFirstTimeAmplNegCT,"h1dTimeFirtsAmplPosFirstTimeAmplNegCT","dTime Firts Ampl Pos First Time Ampl Neg CT",300,-10.0,10.0);

  initH1_F(h1ChargeCS,"h1ChargeCS","Charge CS",200,-2.0,10.0);
  initH1_F(h1AmplitudeFirstPosCS,"h1AmplitudeFirstPosCS","Amplitude First pos CS",1000,-0.2,1.5);
  initH1_F(h1TimeFirstPosCS,"h1TimeFirstPosCS","Time First pos CS",4000,-10.0,100);
  initH1_F(h1RiseTimeFirstPosCS,"h1RiseTimeFirstPosCS","Rise Time First Pos CS",300,0.0,2.0);
  initH1_F(h1FallTimeFirstPosCS,"h1FallTimeFirstPosCS","Fall Time First Pos CS",300,0.0,2.0);
  initH1_F(h1FirstBasisCS,"h1FirstBasisCS","First Basis CS",300,0.0,8.0);
  initH1_F(h1FirstWidthCS,"h1FirstWidthCS","First Width CS",300,0.0,4.0);
  initH1_F(h1dTimeFirtsAmplPosChangLeftDerCS,"h1dTimeFirtsAmplPosChangLeftDerCS"," dTime Firts Ampl Pos and Chang Left Der CS",300,-5.0,10.0);
  initH1_F(h1dTimeFirtsAmplPosFirstTimeAmplNegCS,"h1dTimeFirtsAmplPosFirstTimeAmplNegCS","dTime Firts Ampl Pos First Time Ampl Neg CS",300,-10.0,10.0);
  initH1_F(h1AmplitudeSigPOSDIVbyAmplitudeSigCS,"h1AmplitudeSigPOSDIVbyAmplitudeSigCS","AmplitudeSigPOS DIV by AmplitudeSigCS",1000,0.0,1.0);

  h2AmplitudeFirstPosSIGvsAmplitudeFirstPosCS = new TH2D("h2AmplitudeFirstPosSIGvsAmplitudeFirstPosCS","AmplitudeFirstPosSIGvsAmplitudeFirstPosCS", 1000, -0.2, 1.5, 1000, -0.2, 1.5);

  initH1_F(h1dTimeSigCTLeft,"h1dTimeSigCTLeft","dTime Sig CT Left",5000,-10.0,10.0);
  ///////////////////////////////////////////////////////////////

  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  cout<<"======================="<<endl;
  cout<<" total number of event "<<nentries<<endl;
  cout<<"======================="<<endl;
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    //jentry = 71;
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    //if (ientry > 10000) break;
    nb = fChain->GetEntry(jentry); nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    if(jentry%1000==0)
      cout<<" processed events "<<jentry<<endl;

    for(i = 0;i<anaConst::nChannels;i++){
    //for(i = 2;i<3;i++){    
      for(j = 0;j<anaConst::usbwcNsamplingPoint;j++){
	if(i==15)
	  wfMyA[j] = -ch[i][j];
	else
	  wfMyA[j] = ch[i][j];
	wfMyT[j] = j*SamplingPeriod/1000.0;
      }
      wf[i] = new waveform(wfMyT,wfMyA,anaConst::usbwcNsamplingPoint,nSplinePoint);
      //wf[i] = new waveform(wfMyT,wfMyA,anaConst::usbwcNsamplingPoint);
      wf[i]->SetSignalThreshold(signalThreshold);
      wf[i]->SetCrossTalkThreshold(crossTalkThreshold);
      wf[i]->findParametersOftheWaveform();
      //wf[i]->PrintWaveFormInfo();
      if(i!=15)
	h1TimeFirstPosAll->Fill(wf[i]->findFirstTimePosCFD(cfdVal));
      if(nP!=wf[i]->GetnPoints()){
	cout<<"  ERROR --> nP!=wf[i]->GetnPoints()"<<endl
	    <<"                  nP = "<<nP<<endl
	    <<" wf[i]->GetnPoints() = "<<wf[i]->GetnPoints()<<endl;
	assert(0);
      }
    }  
    
    //SP
    for(i = 0;i<anaConst::nChannels;i++){
      if(wf[i]->GetWfID() == 1 && i!=15){
	if(wf[i]->GetFirstAmplitudePos()>sigAmplTh[i]){
	  nINTatLevel = wf[i]->findNumberOfPosIntersectionAtLevel(wf[i]->GetFirstAmplitudePos()*0.5);
	  //if(nINTatLevel > 1){
	  //cout<<"jentry      "<<jentry<<endl
	  //<<"chID        "<<i<<endl
	  //<<"nINTatLevel "<<nINTatLevel<<endl;
	  //wf[i]->PrintWaveFormInfo();
	  //return;
	  //}
	  timeSP = wf[i]->findFirstTimePosCFD(cfdVal);
	  //if(timeSP>=timingWindowLeft[i] && timeSP<=timingWindowRight[i]){
	    if(nINTatLevel == 1){
	      if(wf[i]->makeAlignmentAndNormalisation( wfNorAligT, wfNorAligA,
						       wf[i]->GetFirstAmplitudePosID(), 15.0, cfdAllVal)){
		if(i==chADAll ){
		  for(j = 0;j<nP;j++){
		    wfNorAligA_SP_Avv[j] = wfNorAligA_SP_Avv[j] + wfNorAligA[j];
		    wfNorAligT_SP_Avv[j] = wfNorAligT[j];
		  }
		  nSP++;
		}
		h1ChargeSP[i]->Fill(wf[i]->GetChargeTOT());
		h1AmplitudeFirstPosSP[i]->Fill(wf[i]->GetFirstAmplitudePos());
		h1TimeFirstPosSP[i]->Fill(timeSP);
		h1RiseTimeFirstPosSP[i]->Fill(wf[i]->GetFirstRiseTimePos());
		h1FallTimeFirstPosSP[i]->Fill(wf[i]->GetFirstFallTimePos());
		h1FirstBasisSP[i]->Fill(wf[i]->GetFirstTimeBasis());
		h1FirstWidthSP[i]->Fill(wf[i]->GetFirstWidthTimePos());
		h1dTimeFirtsAmplPosChangLeftDerSP[i]->Fill(wf[i]->GetdTimeFirtsAmplPosChangLeftDer());
		h1dTimeFirtsAmplPosFirstTimeAmplNegSP[i]->Fill(wf[i]->GetdTimeFirtsAmplPosFirstTimeAmplNeg());
		for(j = 0;j<nPointsNoise;j++){
		  h1noiseSP[i]->Fill(ch[i][j]);
		}
		h1MaxBaseLineAmplSP[i]->Fill(wf[i]->GetMaxBaseLineAmpl());
		h1MinBaseLineAmplSP[i]->Fill(wf[i]->GetMinBaseLineAmpl());
		h1MaxAmplBeforeFirstChangDerAmplSP[i]->Fill(wf[i]->GetMaxAmplBeforeFirstChangDerAmpl());
		h1MinAmplBeforeFirstChangDerAmplSP[i]->Fill(wf[i]->GetMinAmplBeforeFirstChangDerAmpl());
		//if(wf[i]->GetMinAmplBeforeFirstChangDerAmpl()<-5.0/1000){
		//cout<<"jentry "<<jentry<<endl
		//<<"chID   "<<i<<endl;
		//wf[i]->PrintWaveFormInfo();
		//}
	      }
	    }
	    //}
	}
      }
    }
    
    //CT
    for(i = 0;i<anaConst::nChannels;i++){
      if(wf[i]->GetWfID() == 0 && i != 15){
	if(wf[i]->GetFirstAmplitudePos()<0.04){
	  timeCT = wf[i]->findFirstTimePosCFD(cfdVal);
	  if(timeCT>=timingWindowLeft[i] && timeCT<=timingWindowRight[i]){
	    if(wf[i]->GetdTimeFirtsAmplPosFirstTimeAmplNeg()>=1 &&
	       wf[i]->GetdTimeFirtsAmplPosFirstTimeAmplNeg()<=2){
	      if(wf[i]->findNumberOfPosIntersectionAtLevel(wf[i]->GetFirstAmplitudePos()*0.98)==1){
		if(wf[i]->makeAlignmentAndNormalisation( wfNorAligT, wfNorAligA,
							 wf[i]->GetFirstAmplitudePosID(), 15.0, cfdAllVal)){
		  if(i==chADAll){
		    for(j = 0;j<nP;j++){
		      wfNorAligA_CT_Avv[j] = wfNorAligA_CT_Avv[j] + wfNorAligA[j];
		      wfNorAligT_CT_Avv[j] = wfNorAligT[j];
		    }
		    nCT++;
		  }
		  //cout<<"jentry "<<jentry<<endl
		  //  <<"chID   "<<i<<endl;
		  h1ChargeCT[i]->Fill(wf[i]->GetChargeTOT());
		  h1AmplitudeFirstPosCT[i]->Fill(wf[i]->GetFirstAmplitudePos());
		  h1TimeFirstPosCT[i]->Fill(timeCT);
		  h1RiseTimeFirstPosCT[i]->Fill(wf[i]->GetFirstRiseTimePos());
		  h1FallTimeFirstPosCT[i]->Fill(wf[i]->GetFirstFallTimePos());
		  h1FirstBasisCT[i]->Fill(wf[i]->GetFirstTimeBasis());
		  h1FirstWidthCT[i]->Fill(wf[i]->GetFirstWidthTimePos());
		  h1dTimeFirtsAmplPosChangLeftDerCT[i]->Fill(wf[i]->GetdTimeFirtsAmplPosChangLeftDer());
		  h1dTimeFirtsAmplPosFirstTimeAmplNegCT[i]->Fill(wf[i]->GetdTimeFirtsAmplPosFirstTimeAmplNeg());
		}
	      }
	    }
	  }
	}
      }
    }
    
    //CS
    for(i = 0;i<8;i++){
      if(wf[i*2+1]->GetWfID() == 1 && wf[i*2]->GetWfID() == 1 && i!=7){
	if(wf[i*2+1]->GetTotAmplitudePos()>sigAmplTh[i*2+1]){ 
	  nINTatLevelCS02 = wf[i*2+1]->findNumberOfPosIntersectionAtLevel(wf[i*2+1]->GetFirstAmplitudePos()*0.5);
	  if(nINTatLevelCS02 == 1){
	    time02CS = wf[i*2+1]->findFirstTimePosCFD(cfdVal);
	    if(time02CS>=timingWindowLeft[i*2+1] && time02CS<=timingWindowRight[i*2+1]){	 

	      if(wf[i*2]->GetTotAmplitudePos()<sigAmplTh[i*2]){
		time01CS = wf[i*2]->findFirstTimePosCFD(cfdVal);
		if(time01CS>=timingWindowLeft[i*2] && time01CS<=timingWindowRight[i*2]){
		  nINTatLevelCS01 = wf[i*2]->findNumberOfPosIntersectionAtLevel(wf[i*2]->GetFirstAmplitudePos()*0.7);
		  if(nINTatLevelCS01 == 1){
		    if(wf[i*2]->makeAlignmentAndNormalisation( wfNorAligT, wfNorAligA,
							       wf[i*2]->GetFirstAmplitudePosID(), 15.0, cfdAllVal)){
		      if(i*2==chADAll){
			for(j = 0;j<nP;j++){
			  wfNorAligA_CS_Avv[j] = wfNorAligA_CS_Avv[j] + wfNorAligA[j];
			  wfNorAligT_CS_Avv[j] = wfNorAligT[j];
			}
			nCS++;
		      }  
		      h1ChargeCS[i*2]->Fill(wf[i*2]->GetChargeTOT());
		      h1AmplitudeFirstPosCS[i*2]->Fill(wf[i*2]->GetFirstAmplitudePos());
		      h1TimeFirstPosCS[i*2]->Fill(time01CS);
		      h1RiseTimeFirstPosCS[i*2]->Fill(wf[i*2]->GetFirstRiseTimePos());
		      h1FallTimeFirstPosSP[i*2]->Fill(wf[i*2]->GetFirstFallTimePos());
		      h1FirstBasisCS[i*2]->Fill(wf[i*2]->GetFirstTimeBasis());
		      h1FirstWidthCS[i*2]->Fill(wf[i*2]->GetFirstWidthTimePos());
		      h1dTimeFirtsAmplPosChangLeftDerCS[i*2]->Fill(wf[i*2]->GetdTimeFirtsAmplPosChangLeftDer());
		      h1dTimeFirtsAmplPosFirstTimeAmplNegCS[i*2]->Fill(wf[i*2]->GetdTimeFirtsAmplPosFirstTimeAmplNeg());
		      if(wf[i*2+1]->GetFirstAmplitudePos()>0.0){
			h1AmplitudeSigPOSDIVbyAmplitudeSigCS[i*2]->Fill(wf[i*2]->GetFirstAmplitudePos()/wf[i*2+1]->GetFirstAmplitudePos());
		      }

		      if(i*2==chADAll){
			h2AmplitudeFirstPosSIGvsAmplitudeFirstPosCS->Fill(wf[i*2]->GetFirstAmplitudePos(),wf[i*2+1]->GetFirstAmplitudePos());
		      }
		    }
		  }
		}
	      }
	      
	    }
	  }
	}
	if(wf[i*2]->GetTotAmplitudePos()>sigAmplTh[i*2]){
	  nINTatLevelCS02 = wf[i*2]->findNumberOfPosIntersectionAtLevel(wf[i*2]->GetFirstAmplitudePos()*0.5);
	  if(nINTatLevelCS02 == 1){
	    time02CS = wf[i*2]->findFirstTimePosCFD(cfdVal);
	    if(time02CS>=timingWindowLeft[i*2] && time02CS<=timingWindowRight[i*2]){

	      if(wf[i*2+1]->GetTotAmplitudePos()<sigAmplTh[i*2+1]){
		time01CS = wf[i*2+1]->findFirstTimePosCFD(cfdVal);
		if(time01CS>=timingWindowLeft[i*2+1] && time01CS<=timingWindowRight[i*2+1]){
		  nINTatLevelCS01 = wf[i*2+1]->findNumberOfPosIntersectionAtLevel(wf[i*2+1]->GetFirstAmplitudePos()*0.7);
		  if(nINTatLevelCS01 == 1){
		    if(wf[i*2+1]->makeAlignmentAndNormalisation( wfNorAligT, wfNorAligA,
								 wf[i*2+1]->GetFirstAmplitudePosID(), 15.0, cfdAllVal)){
		      if((i*2+1)==chADAll){
			for(j = 0;j<nP;j++){
			  wfNorAligA_CS_Avv[j] = wfNorAligA_CS_Avv[j] + wfNorAligA[j];
			  wfNorAligT_CS_Avv[j] = wfNorAligT[j];
			}
			nCS++;
		      }
		      h1ChargeCS[i*2+1]->Fill(wf[i*2+1]->GetChargeTOT());
		      h1AmplitudeFirstPosCS[i*2+1]->Fill(wf[i*2+1]->GetFirstAmplitudePos());
		      h1TimeFirstPosCS[i*2+1]->Fill(wf[i*2+1]->findFirstTimePosCFD(cfdVal));
		      h1RiseTimeFirstPosCS[i*2+1]->Fill(wf[i*2+1]->GetFirstRiseTimePos());
		      h1FallTimeFirstPosSP[i*2+1]->Fill(wf[i*2+1]->GetFirstFallTimePos());
		      h1FirstBasisCS[i*2+1]->Fill(wf[i*2+1]->GetFirstTimeBasis());
		      h1FirstWidthCS[i*2+1]->Fill(wf[i*2+1]->GetFirstWidthTimePos());
		      h1dTimeFirtsAmplPosChangLeftDerCS[i*2+1]->Fill(wf[i*2+1]->GetdTimeFirtsAmplPosChangLeftDer());
		      h1dTimeFirtsAmplPosFirstTimeAmplNegCS[i*2+1]->Fill(wf[i*2+1]->GetdTimeFirtsAmplPosFirstTimeAmplNeg());
		      if(wf[i*2]->GetFirstAmplitudePos()>0.0){
			h1AmplitudeSigPOSDIVbyAmplitudeSigCS[i*2+1]->Fill(wf[i*2+1]->GetFirstAmplitudePos()/wf[i*2]->GetFirstAmplitudePos());
		      }
		      if((i*2+1)==chADAll){
			h2AmplitudeFirstPosSIGvsAmplitudeFirstPosCS->Fill(wf[i*2+1]->GetFirstAmplitudePos(),wf[i*2]->GetFirstAmplitudePos());
		      }
		    }		    
		  }
		}
	      }

	    }
	  }
	}
      }
    }
    
    for(i = 0;i<8;i++){
      if(i!=7){
	//cout<<i*2+1<<"   -   "<<i*2<<endl;
	mydTime = (wf[i*2+1]->findTimeLeftCFD(wf[i*2+1]->GetTotAmplitudePosID(),0.5) - 
		   wf[i*2]->findTimeLeftCFD(wf[1*2]->GetTotAmplitudePosID(),0.5));      
	if(wf[i*2+1]->GetWfID() == 1 && wf[i*2]->GetWfID() == 1){
	  if(wf[i*2+1]->GetTotAmplitudePos()>sigAmplTh[i*2+1] && 
	     wf[i*2]->GetTotAmplitudePos()<sigAmplTh[i*2] && wf[i*2]->GetTotAmplitudePos()>0.01){
	    h1dTimeSigCTLeft[i*2+1]->Fill(mydTime);
	  }
	  if(wf[i*2]->GetTotAmplitudePos()>sigAmplTh[i*2] && 
	     wf[i*2+1]->GetTotAmplitudePos()<sigAmplTh[i*2+1] && wf[i*2+1]->GetTotAmplitudePos()>0.01){
	    h1dTimeSigCTLeft[i*2]->Fill(mydTime);
	  }
	}
      }
    }
        
    for(i = 0;i<anaConst::nChannels;i++){
      delete wf[i];
    }

  }

  if(nSP>0){
    for(j = 0;j<nP;j++){
      wfNorAligA_SP_Avv[j] =  wfNorAligA_SP_Avv[j]/nSP;
    }
  }
  if(nCT>0){
    for(j = 0;j<nP;j++){
      wfNorAligA_CT_Avv[j] =  wfNorAligA_CT_Avv[j]/nCT;
    }
  }
  if(nCS>0){
    for(j = 0;j<nP;j++){
      wfNorAligA_CS_Avv[j] =  wfNorAligA_CS_Avv[j]/nCS;
    }
  }

  cout<<"nSP = "<<nSP<<endl
      <<"nCT = "<<nCT<<endl
      <<"nCS = "<<nCS<<endl;
  
  TGraph *gr1SP = new TGraph( nP, wfNorAligT_SP_Avv, wfNorAligA_SP_Avv); 
  gr1SP->SetName("gr1SP");
  //gr1SP->SaveAs("makeAlignmentAndNormalisation_SP_All_CH.C");
  TGraph *gr1CT = new TGraph( nP, wfNorAligT_CT_Avv, wfNorAligA_CT_Avv); 
  gr1CT->SetName("gr1CT");
  //gr1CT->SaveAs("makeAlignmentAndNormalisation_CT_All_CH.C");
  TGraph *gr1CS = new TGraph( nP, wfNorAligT_CS_Avv, wfNorAligA_CS_Avv); 
  gr1CS->SetName("gr1CS");
  //gr1CS->SaveAs("makeAlignmentAndNormalisation_CS_All_CH.C");

  TFile* rootFile = new TFile(histFName.Data(), "RECREATE", " Histograms from USBWC  ", 1);
  rootFile->cd();
  if (rootFile->IsZombie()){
    cout<<"  ERROR ---> file "<<histFName<<" is zombi"<<endl;
    assert(0);
  }
  else
    cout<<"  Output Histos file ---> "<<histFName<<endl;

  for(i = 0;i<anaConst::nChannels;i++){
    h1ChargeSP[i]->Write();
    h1AmplitudeFirstPosSP[i]->Write();
    h1TimeFirstPosSP[i]->Write();
    h1RiseTimeFirstPosSP[i]->Write();
    h1FallTimeFirstPosSP[i]->Write();
    h1FirstBasisSP[i]->Write();
    h1FirstWidthSP[i]->Write();
    h1dTimeFirtsAmplPosChangLeftDerSP[i]->Write();
    h1dTimeFirtsAmplPosFirstTimeAmplNegSP[i]->Write();
    h1noiseSP[i]->Write();
    h1MaxBaseLineAmplSP[i]->Write();
    h1MinBaseLineAmplSP[i]->Write();
    h1MaxAmplBeforeFirstChangDerAmplSP[i]->Write();
    h1MinAmplBeforeFirstChangDerAmplSP[i]->Write();

    h1ChargeCT[i]->Write();
    h1AmplitudeFirstPosCT[i]->Write();
    h1TimeFirstPosCT[i]->Write();
    h1RiseTimeFirstPosCT[i]->Write();
    h1FallTimeFirstPosCT[i]->Write();
    h1FirstBasisCT[i]->Write();
    h1FirstWidthCT[i]->Write();
    h1dTimeFirtsAmplPosChangLeftDerCT[i]->Write();
    h1dTimeFirtsAmplPosFirstTimeAmplNegCT[i]->Write();

    h1ChargeCS[i]->Write();
    h1AmplitudeFirstPosCS[i]->Write();
    h1TimeFirstPosCS[i]->Write();
    h1RiseTimeFirstPosCS[i]->Write();
    h1FallTimeFirstPosCS[i]->Write();
    h1FirstBasisCS[i]->Write();
    h1FirstWidthCS[i]->Write();
    h1dTimeFirtsAmplPosChangLeftDerCS[i]->Write();
    h1dTimeFirtsAmplPosFirstTimeAmplNegCS[i]->Write();
    h1AmplitudeSigPOSDIVbyAmplitudeSigCS[i]->Write();

    h1dTimeSigCTLeft[i]->Write();
  }

  h2AmplitudeFirstPosSIGvsAmplitudeFirstPosCS->Write();
  gr1SP->Write();
  gr1CT->Write();
  gr1CS->Write();

  h1TimeFirstPosAll->Write();

}


//LB 09.02.2011
//for the moment just with NO splines !!!!
void usbwcFileStr::DrawUSBWCevent(Long64_t entry){
  GetEntry((entry));

  Int_t i = 0;
  Int_t j = 0;
  Double_t wfMyA[anaConst::usbwcNsamplingPoint];
  Double_t wfMyT[anaConst::usbwcNsamplingPoint];

  //Double_t A_min = -0.3;
  //Double_t A_max =  1.5;

  Double_t A_min = -0.05;
  Double_t A_max =  0.17;

  //////graph init//////
  TGraph *wfGr[16];
  Int_t wfID[16];
  TString nameG;
  TString titleG;
  for(i = 0;i<anaConst::nChannels;i++){
    wfGr[i] = new TGraph();
    nameG = "wfgr_";
    nameG += i;
    nameG += "_";
    nameG += entry;
    titleG = "wave form graph ";
    titleG += i;
    titleG += "_";
    titleG += entry;
    wfGr[i]->SetNameTitle(nameG.Data(),titleG.Data());
  }
  //////////////////////

  for(i = 0;i<anaConst::nChannels;i++){
    for(j = 0;j<anaConst::usbwcNsamplingPoint;j++){
      wfMyA[j] = ch[i][j];
      wfMyT[j] = j*SamplingPeriod/1000.0;
    }
    waveform *wf = new waveform( wfMyT, wfMyA, anaConst::usbwcNsamplingPoint);
    wf->findParametersOftheWaveform();
    wfID[i] = wf->GetWfID();
    for(j = 0;j<anaConst::usbwcNsamplingPoint;j++){
      wfGr[i]->SetPoint(j,wf->_wfTpublic[j],wf->_wfApublic[j]);
      //wfGr[i]->SetPoint(j,(j*SamplingPeriod/1000.0),ch[i][j]);
    }
    delete wf;
  }

  TString titleCh;

  TCanvas *c11 = new TCanvas("c11"," wf ch0 - ch7",10,10,400,1400);
  c11->SetFillColor(0);
  c11->Divide(1,8);
  for(i = 0;i<8;i++){
    c11->cd(i+1);
     //c11->GetPad(i+1)->SetRightMargin(0.01);
    //c11->GetPad(i+1)->SetLeftMargin(0.05);
    //c11->GetPad(i+1)->SetTopMargin(0.0);
    //c11->GetPad(i+1)->SetBottomMargin(0.1);
    c11->GetPad(i+1)->SetRightMargin(0.001);
    c11->GetPad(i+1)->SetLeftMargin(0.05);
    c11->GetPad(i+1)->SetTopMargin(0.0);
    if(i == 7)
      c11->GetPad(i+1)->SetBottomMargin(0.1);
    else
      c11->GetPad(i+1)->SetBottomMargin(0.007);
    wfGr[i]->SetMarkerSize(0.5);
    wfGr[i]->SetMarkerStyle(20);
    wfGr[i]->SetMinimum(A_min);
    wfGr[i]->SetMaximum(A_max);
    wfGr[i]->SetTitle("");
    if(wfID[i]==1){
      wfGr[i]->SetMarkerColor(kBlue);
    }
    //titleCh = "time, ns    channel ";
    //titleCh += i;
    //wfGr[i]->GetXaxis()->SetTitle(titleCh.Data());
    if(i == 7){
    wfGr[i]->GetXaxis()->SetTitle("time, ns");
    wfGr[i]->GetXaxis()->SetLabelSize(0.09);
    }
    titleCh = "channel ";
    titleCh += i;

    TLatex *tt = new TLatex(50.0,0.1, titleCh.Data());
    tt->SetTextSize(0.3);

    wfGr[i]->GetXaxis()->SetTitleSize(0.1);
    wfGr[i]->GetXaxis()->SetTitleOffset(0.3);
    wfGr[i]->GetXaxis()->SetRangeUser(0.0,80.0);

    wfGr[i]->GetYaxis()->SetTitleSize(0.13);
    wfGr[i]->GetYaxis()->SetTitleOffset(0.1);

    if(i != 7)
    wfGr[i]->GetYaxis()->SetLabelSize(0.09);

    if(i == 7)
    wfGr[i]->GetYaxis()->SetTitle("Amplitude, V");

    wfGr[i]->Draw("APL");

    tt->Draw("same");
  }

  //TPaveText *tt = new TPaveText(0.0,10.0,0.01,10,"ss");
  //TText *tt = new TText(0.0,0.0,"ss");
  TCanvas *c12 = new TCanvas("c12"," wf ch8 - ch15",20,20,400,1400);
  c12->SetFillColor(0);
  c12->Divide(1,8);
  for(i = 8;i<anaConst::nChannels;i++){
    c12->cd(i + 1 - 8);
    c12->GetPad(i + 1 - 8)->SetLeftMargin(0.007);
    c12->GetPad(i + 1 - 8)->SetRightMargin(0.001);
    c12->GetPad(i + 1 - 8)->SetTopMargin(0.0);
    //if(i != 15)
    //c12->GetPad(i + 1 - 8)->SetBottomMargin(0.1);
    //c12->SetBorderSize(10);
    if(i == 15)
      c12->GetPad(i+1 - 8)->SetBottomMargin(0.1);
    else
      c12->GetPad(i+1 - 8)->SetBottomMargin(0.007);
    wfGr[i]->SetMarkerSize(0.5);
    wfGr[i]->SetMarkerStyle(20);
    wfGr[i]->SetMinimum(A_min);
    wfGr[i]->SetMaximum(A_max);
    if(i == 15){
      wfGr[i]->SetMinimum(-1.0);
      wfGr[i]->SetMaximum( 0.2);
      wfGr[i]->SetMarkerColor(kRed);
    }
    wfGr[i]->SetTitle("");
    if(wfID[i]==1){
      wfGr[i]->SetMarkerColor(kBlue);
    }
    //titleCh = "time, ns    channel ";
    titleCh = "channel ";
    titleCh += i;

    TLatex *tt = new TLatex(50.0,0.1, titleCh.Data());
    tt->SetTextSize(0.3);

    if(i == 15){
      TLatex *tt = new TLatex(50.0,-0.2, titleCh.Data());
      tt->SetTextSize(0.3);
    }

    //if(i == 15){
    //wfGr[i]->SetMarkerColor(kRed);
    //}
    //cout<<"titleCh.Data() "<<titleCh.Data()<<endl;
    //wfGr[i]->GetXaxis()->SetTitle(titleCh.Data());
    //if(i== 7)
    //wfGr[i]->GetXaxis()->SetTitle("time, ns");
    if(i == 15){
      wfGr[i]->GetXaxis()->SetTitle("time, ns");
      wfGr[i]->GetXaxis()->SetLabelSize(0.09);
    }

    wfGr[i]->GetXaxis()->SetTitleSize(0.1);
    wfGr[i]->GetXaxis()->SetTitleOffset(0.3);

    wfGr[i]->GetYaxis()->SetTitleSize(0.13);
    wfGr[i]->GetYaxis()->SetTitleOffset(0.1);
    //wfGr[i]->GetYaxis()->SetTitle("Amplitude, V");

    wfGr[i]->GetXaxis()->SetRangeUser(0.0,80.0);
    wfGr[i]->Draw("APL");
    tt->Draw("same");
  }
}


void usbwcFileStr::DrawUSBWCeventCh(Long64_t entry, Int_t chDR){

  GetEntry((entry));

  Int_t i = 0;
  Int_t j = 0;
  Double_t wfMyA[anaConst::usbwcNsamplingPoint];
  Double_t wfMyT[anaConst::usbwcNsamplingPoint];

  //Double_t A_min = -0.8;
  //Double_t A_max =  0.2;

  //////graph init//////
  TGraph *wfGr_r;
  TGraph *wfGr_s;
  //TString nameG;
  //TString titleG;
  wfGr_r = new TGraph();
  wfGr_s = new TGraph();
  //nameG = "wfgr_realP";
  //nameG += "_";
  //nameG += entry;
  //titleG = "wave form graph ";
  //titleG += i;
  //titleG += "_";
  //titleG += entry;
  //wfGr[i]->SetNameTitle(nameG.Data(),titleG.Data());
  //////////////////////

  for(j = 0;j<anaConst::usbwcNsamplingPoint;j++){
    wfMyA[j] = ch[chDR][j];
    wfMyT[j] = j*SamplingPeriod/1000.0;
    wfGr_r->SetPoint( j, wfMyT[j], wfMyA[j]);
  }
  waveform *wf = new waveform( wfMyT, wfMyA, anaConst::usbwcNsamplingPoint, 5);
  wf->findParametersOftheWaveform();
  cout<<endl<<"wf->GetnPoints() = "<<wf->GetnPoints()<<endl;
  for(j = 0;j<wf->GetnPoints();j++){
    //wfGr[i]->SetPoint(j,wf->_wfTpublic[j],wf->_wfApublic[j]);
    //wfGr[i]->SetPoint(j,(j*SamplingPeriod/1000.0),ch[i][j]);
    wfGr_s->SetPoint(j, wf->_wfTpublic[j], wf->_wfApublic[j]);
  }
  delete wf;

  TString titleCh;

  TCanvas *c11 = new TCanvas("c11"," wf ch0 - ch7",10,10,1000,800);
  c11->SetFillColor(0);
  wfGr_r->SetMarkerSize(1.5);
  wfGr_r->SetMarkerStyle(20);
  wfGr_r->SetMarkerColor(kRed);
  //wfGr_r->SetLineColor(kBlack);
  wfGr_r->SetLineWidth(0.01);

  wfGr_s->SetMarkerSize(1);
  wfGr_s->SetMarkerStyle(20);
  wfGr_s->SetMarkerColor(kBlack);
  wfGr_s->SetLineColor(kBlack);
  wfGr_s->SetLineWidth(2);

  TMultiGraph *mg = new TMultiGraph("mg","");
  mg->Add(wfGr_s);
  //mg->Add(wfGr_r);
  mg->Draw("APL");
  mg->GetXaxis()->SetTitle("time, ns");
  mg->GetYaxis()->SetTitle("Amplitude, V");

  //wfGr_r->SetTitle("");
  //wfGr_r->Draw("APL");
  //wfGr_r->SetMinimum(A_min);
  //wfGr_r->SetMaximum(A_max);
  //titleCh = "time, ns    channel ";
  //titleCh += i;
  //wfGr[i]->GetXaxis()->SetTitle(titleCh.Data());
  //wfGr[i]->GetXaxis()->SetTitleSize(0.1);
  //wfGr[i]->GetXaxis()->SetTitleOffset(0.3);
}

void usbwcFileStr::initH1_F(TH1D *h[anaConst::nChannels],
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

void usbwcFileStr::initH2_F(TH2D *h[anaConst::nChannels],
			    TString hName , TString hTitle,
			    Int_t nBin1, Double_t binMin1, 
			    Double_t binMax1, 
			    Int_t nBin2, Double_t binMin2, 
			    Double_t binMax2){

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
    h[i] = new TH2D(hNameM.Data(),hTitleM.Data(), nBin1, binMin1, binMax1, nBin2, binMin2, binMax2);
  }

}    

void usbwcFileStr::GetUnixTime(Double_t *unixTimeUSB, Int_t Nmax, Int_t &nEv){
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
  cout<<" ----- usbwcFileStr::GetUnixTime ---- "<<nentries<<endl;
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
    if((UnixTime-timeBefor)<0){
      cout<<endl
	  <<" root files in root data list are not ordered in time "<<endl
	  <<fChain->GetCurrentFile()->GetName()<<endl;
      assert(0);
    }
    unixTimeUSB[jentry] = UnixTime;
    timeBefor = UnixTime;
  }
  cout<<" -------------------- "<<nentries<<endl;
  //cout<<jentry<<"  "<<time_CRT<<endl;
}

Bool_t usbwcFileStr::CheckDistInTime(){

  cout<<endl<<" Check ordering in time"<<endl
      <<"   usbwcFileStr::CheckDistInTime  "<<endl;
  Double_t timeOld = 0;

  Long64_t nentries = fChain->GetEntriesFast();
  Long64_t jentry = 0;
  LoadTree(jentry);
  GetEntry(jentry);
  //b_time->GetEntry(jentry);
  timeOld = UnixTime;
  for (jentry=0; jentry<nentries;jentry++) {
    LoadTree(jentry);
    GetEntry(jentry);
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
    if((timeOld - UnixTime)>0){
      cout<<endl
	  <<" ERROR ---> events are not ordered in time "<<endl
	  <<" file : "<<fChain->GetCurrentFile()->GetName()<<endl
	  <<"jentry    "<<jentry<<endl;
      printf(" timeOld  = %20.5f \n", timeOld);
      printf(" time_CRT = %20.5f \n", UnixTime);

      assert(0);
    }
    timeOld = UnixTime;
  }

  return true;
}

Int_t usbwcFileStr::GetDataRate(){

  ///////////////TIME//////////////
  time_t rawtime;
  tm *ptm;
  Int_t crtTimeInt;
  Int_t locTime;
  Int_t timeStart_hour;
  Int_t timeStart_min;
  /////////////////////////////////

  Long64_t nentries = fChain->GetEntriesFast();
  Int_t nentriesInt = nentries;
  LoadTree(0);
  GetEntry(0);
  Double_t timeBeg = UnixTime;
  LoadTree((nentries-1));
  GetEntry((nentries-1));
  Double_t timeEnd = UnixTime;
  
  Double_t dT = (timeEnd - timeBeg);
  Double_t Rate = -999.0;

  if(dT>0)
    Rate = nentries/dT;

  cout<<endl<<"--------- USBWC timing information ----"<<endl
      <<" RATE = "<<setw(10)<<Rate<<" 1/s"<<endl;
  printf(" timeBeg --> %20.4f \n", timeBeg);
  printf(" timeEnd --> %20.4f \n", timeEnd);
  cout<<" total run time          "<<setw(10)<<dT/3600.0<<" hours"<<endl
      <<" total number of entries "<<setw(10)<<nentries<<endl;
    
  crtTimeInt = (Int_t)timeBeg;
  rawtime = crtTimeInt;
  ptm = gmtime ( &rawtime );    
  locTime = (ptm->tm_hour);
  printf (" GMT time START run :  %2d.%2d.%4d   %2d:%02d:%02d\n", ptm->tm_mday,((ptm->tm_mon)+1),((ptm->tm_year)+1900),(ptm->tm_hour)%24, ptm->tm_min,ptm->tm_sec);

  timeStart_hour = (ptm->tm_hour)%24;
  timeStart_min  = ptm->tm_min;

  crtTimeInt = (Int_t)timeEnd;
  rawtime = crtTimeInt;
  ptm = gmtime ( &rawtime );    
  locTime = (ptm->tm_hour);
  printf (" GMT time END   run :  %2d.%2d.%4d   %2d:%02d:%02d\n", ptm->tm_mday,((ptm->tm_mon)+1),((ptm->tm_year)+1900),(ptm->tm_hour)%24, ptm->tm_min,ptm->tm_sec);

  cout<<"-----------------------------"<<endl;

  //printf("timeBeg %20.4f \n", timeBeg);
  //printf("timeEnd %20.4f \n", timeEnd);
  //cout<<"timeBeg  = "<<timeBeg<<endl
  //  <<"timeEnd  = "<<timeEnd<<endl
  //  <<"dT       = "<<dT<<endl
  //  <<"nentries = "<<nentries<<endl
  //  <<"Rate     = "<<Rate<<endl;

  _Rate = Rate;
  _unixTimeBegin = timeBeg;
  _unixTimeEnd = timeEnd;

  return nentriesInt;
}


void usbwcFileStr::saveUnixTime2File(TString fileN){
  FILE *stream;
  stream = fopen(fileN.Data(), "w");
  Long64_t nentries = fChain->GetEntriesFast();
  cout<<"======================="<<endl;
  cout<<" usbwcFileStr::saveUnixTime2File : "<<fileN<<endl;
  cout<<" total number of event "<<nentries<<endl;
  cout<<"======================="<<endl;
  Long64_t jentry = 0;
  for (jentry=0; jentry<nentries;jentry++) {
    LoadTree(jentry);
    GetEntry(jentry);
    fprintf (stream, "%20.4f \n",UnixTime);
  }
  fclose(stream);
}

/*
void usbwcFileStr::SetIdealCosmicWF(TString nam, Int_t chID){
  ifstream indata;
  indata.open(nam.Data());
  assert(indata.is_open());

  string mot;

  cout<<nam.Data()<<endl;
  
  Int_t pID;
  Double_t t;
  Double_t A;

  while (indata>>pID>>t>>A){
    //while (indata>>mot){
    //cout<<pID<<"  "<<t<<"   "<<A<<endl;
    //cout<<mot<<endl;
    idealCosmicWF_t[pID][chID] = t;
    idealCosmicWF_A[pID][chID] = A;
  }
  indata.close();

}
*/

Int_t usbwcFileStr::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

Long64_t usbwcFileStr::LoadTree(Long64_t entry)
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

void usbwcFileStr::Init(TTree *tree)
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

   fChain->SetBranchAddress("eventID", &eventID, &b_eventID);
   fChain->SetBranchAddress("SamplingPeriod", &SamplingPeriod, &b_SamplingPeriod);
   fChain->SetBranchAddress("INLCorrection", &INLCorrection, &b_INLCorrection);
   fChain->SetBranchAddress("FCR", &FCR, &b_FCR);
   fChain->SetBranchAddress("UnixTime", &UnixTime, &b_UnixTime);
   fChain->SetBranchAddress("ch", ch, &b_ch);
   fChain->SetBranchAddress("Charge", Charge, &b_Charge);
   fChain->SetBranchAddress("Time", Time, &b_Time);
   fChain->SetBranchAddress("Amplitude", Amplitude, &b_Amplitude);
}


void usbwcFileStr::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}


