//my
#include "slacTestANA.hh"
#include "../../anaConst.hh"
#include "../wfSimulation/src/wfSim.hh"
#include "../wfSimulation/src/waveform.hh"

//root
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TVector3.h>

//C, C++
#include <assert.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

const Int_t nMcpPMmax = 100000;

slacTestANA::slacTestANA(string fileName, UInt_t runSeed){
  cout<<endl<<" file "<<fileName<<endl;
  cout<<endl
      <<" ---- Init Random Gen ---- "<<endl
      <<" runSeed = "<<runSeed<<endl
      <<"--------------------------"<<endl<<endl;
  _Rand3 = new TRandom3(runSeed);
  TTree *tree = 0;
  // if parameter tree is not specified (or zero), connect the file
  // used to generate this class and read the Tree.
  if (tree == 0) {
    TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(fileName.c_str());
    if (!f) {
      f = new TFile(fileName.c_str());
    }
    tree = (TTree*)gDirectory->Get("T");    
  }
  Init(tree);
}

slacTestANA::slacTestANA(TString name, Int_t key, UInt_t runSeed){
  if(key == 1){
    cout<<"slacTestANA::slacTestANA(TString name, Int_t key, UInt_t runSeed)"<<endl;
    cout<<endl<<endl<<name<<endl;
    _Rand3 = new TRandom3(runSeed);
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
      cout<<"        adding "<<rootFileName<<endl;
      theChain->Add(rootFileName.c_str(),-1);
    }
    indata.close();
    Init(theChain);
  }
  else{
    cout<<"slacTestANA::slacTestANA(string fileName, Int_t key, UInt_t runSeed)"<<endl;
    assert(0);
  }
}

slacTestANA::~slacTestANA(){
   if (!fChain) return;
   delete fChain->GetCurrentFile();
   //delete _Rand3;
   //cout<<"sasaasa"<<endl;
}

void slacTestANA::Loop(TString histFileName){
  Int_t i = 0;
  Int_t nPe = 0;
  Int_t mYchID = -999;
  Double_t timeSmeardAndQE = -999.0;
  Double_t firstTime[anaConst::nChannels];
  Double_t firstPhotPathL[anaConst::nChannels];
  Int_t myNPE[anaConst::nChannels];
  Double_t mom;
  Double_t trkTheta;	 

  Int_t notOk = 0;
  Int_t isOk = 0;

  Double_t primMom;
  Double_t primTheta = -999.0;
  Double_t primPhi = -999.0;

  //////////////////
  TH1D *h1nPhot = new TH1D("h1nPhot","nPhot",3000,0.0,2500.0);
  TH1D *h1nPhotEl = new TH1D("h1nPhotEl","n p.e",300,0.0,300.0);

  TH1D *h1primTheta = new TH1D("h1primTheta","prim Theta",200,   0.0,190.0);
  TH1D *h1primPhi   = new TH1D("h1primPhi","prim Phi"    ,360,-180.0,180.0);

  TH1D *h1dtimeWF_ch2ch3 =  new TH1D("h1dtimeWF_ch2ch3" , "dtimeWF ch2 - ch3", 2000, -10.0, 10.0);
  TH1D *h1dtimeFirst_ch2ch3 =  new TH1D("h1dtimeFirst_ch2ch3" , "dtimeWF ch2 - ch3", 2000, -10.0, 10.0);
 
  TH2D *h2trkPosXvstrkPosY = new TH2D("h2trkPosXvstrkPosY"," trkPosX vs trkPosY",300,-600.0,600.0,300,-600.0,600.0);
  TH1D *h1trkPosX = new TH1D("h1trkPosX"," trkPosX ",300,-600.0,600.0);
  TH1D *h1trkPosZ = new TH1D("h1trkPosZ"," trkPosZ ",3000,0.0,3000.0);
  TH1D *h1trkTheta = new TH1D("h1trkTheta"," trk Theta ",200,150.0,190.0);

  TH2D *h2PosZvsPosY_ch3 = new TH2D("h2PosZvsPosY_ch3","PosZ vs PosY ch3", 300, -30.0, 30.0, 300, 1200, 1300);

  TH1D *h1time[anaConst::nChannels];
  TH1D *h1nPE[anaConst::nChannels];
  TH1D *h1Firsttime[anaConst::nChannels];
  TH1D *h1PhotPathL[anaConst::nChannels];
  TH1D *h1dtime[anaConst::nChannels];
  TH1D *h1dtimeL[anaConst::nChannels];
  TH1D *h1dL[anaConst::nChannels];
  TH1D *h1dL_L[anaConst::nChannels];

  TH2D *h2dtimeVSx = new TH2D("h2dtimeVSx","dtime vs x", 1000,-150.0,150,1000,-5.0,5.0);


  TH2D *h2dtimeLVSphi_ch1 = new TH2D("h2dtimeLVSphi_ch1","dtimeL VS phi ch1", 360.0,-180.0,180,1000,-5.0,5.0);

  //////////////////

  initH1_F(h1time, "h1time" , "time", 1000, 0.0, 20.0);
  initH1_F(h1nPE, "h1nPE" , "nPE", 20, 0.0, 20.0);
  initH1_F(h1Firsttime, "h1Firsttime" , "First time", 1000, 0.0, 20.0);
  initH1_F(h1dtime, "h1dtime" , "dtime", 2000, -10.0, 10.0);
  initH1_F(h1dtimeL, "h1dtimeL" , "dtimeL", 2000, -10.0, 10.0);
  initH1_F(h1dL,   "h1dL",   "dL", 2000, -100.0, 100.0);
  initH1_F(h1dL_L, "h1dL_L", "dL_L", 2000, -100.0, 100.0);
  initH1_F(h1PhotPathL, "h1PhotPathL" , "PhotPathL", 10000, 0.0, 2000.0);

  /////////////////////////////////////
  Int_t MySeed = 19012234;
  TRandom3 *rnd = new TRandom3(MySeed); 
  double t0;
  wfSim *wfS_ch2;
  wfSim *wfS_ch3;
  bool firstEP_ch2 = false; 
  bool firstEP_ch3 = false; 
  double twf_ch2 = -999.0;
  double twf_ch3 = -999.0;
  /////////////////////////////////////


  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  cout<<" nentries "<<nentries<<endl;
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    if(jentry%1000==0)
      cout<<jentry<<endl;

    primMom = TMath::Sqrt(primMomX*primMomX + 
			  primMomY*primMomY + 
			  primMomZ*primMomZ);

    //if(primMom>0.0){
    TVector3 vTRK( primMomX, primMomY, primMomZ);
    primTheta = vTRK.Theta()*180.0/TMath::Pi();
    primPhi   = vTRK.Phi()*180.0/TMath::Pi();

    h1primTheta->Fill(primTheta);
    h1primPhi->Fill(primPhi);
    
    h1nPhot->Fill(nPhot);
    nPe = 0;
    for(i = 0;i<anaConst::nChannels;i++){
      firstTime[i] = 999.0;
      firstPhotPathL[i] = -999.0;
      myNPE[i] = 0;
    }

    h2trkPosXvstrkPosY->Fill(trkPosX[0],trkPosY[0]);    
    
    ////////////////////////////
    firstEP_ch2 = false; 
    firstEP_ch3 = false; 
    t0 = rnd->Uniform(0.0,5.0);
    twf_ch2 = -999.0;
    twf_ch3 = -999.0;
    ////////////////////////////

    for(i = 0;i<nPhot;i++){
      //mYchID = GetChID(PosY[i],PosZ[i]);
      mYchID = chID[i];
      if(chID[i]==mYchID){
	//cout<<"chID[i]!=mYchID"<<endl
	//<<"chID[i] "<<chID[i]<<endl
	//<<"mYchID  "<<mYchID<<endl;
	isOk++;
	//return;
	//assert(0);
      }

      if(chID[i]!=mYchID){
	//cout<<"chID[i]!=mYchID"<<endl
	//<<"chID[i] "<<chID[i]<<endl
	//<<"mYchID  "<<mYchID<<endl;
	notOk++;
	//return;
	//assert(0);
      }
      //if(trkPosX[i]>100.0 && trkPosX[i]<110.0){
      //if(trkPosY[i]>-5.0 && trkPosY[i]<5.0){
      if(ifBialkali_QE(Wavelength[i])){
	timeSmeardAndQE = calculateTimeSpread(Time[i]);
	if(mYchID>=0 && mYchID<anaConst::nChannels){
	  h1time[mYchID]->Fill(timeSmeardAndQE);
	  h1PhotPathL[mYchID]->Fill(photPathLen[i]);	
	  
	  //myNPE[mYchID] = myNPE[mYchID]+1;
	  if(firstTime[mYchID] > timeSmeardAndQE){
	    firstTime[mYchID] = timeSmeardAndQE;
	    firstPhotPathL[mYchID] = photPathLen[i];
	  }
	  
	  if(mYchID == 3){
	    //cout<<"PosY[i] = "<<PosY[i]<<endl
	    //<<"PosZ[i] = "<<PosZ[i]<<endl;
	    h2PosZvsPosY_ch3->Fill(PosY[i],PosZ[i]);
	  }
	  
	  ////////////////////////
	  if(mYchID==2 && firstEP_ch2 == false){
	    //wfS_ch2 = new wfSim( (timeSmeardAndQE+10), rnd, t0);
	    firstEP_ch2 = true;
	  }
	  else if(mYchID==2 && firstEP_ch2 == true){
	    //wfS_ch2->addNextWF( (timeSmeardAndQE+10), t0);
	    //delete wfS_ch2;
	  }

	  if(mYchID==3 && firstEP_ch3 == false){
	    //wfS_ch3 = new wfSim( (timeSmeardAndQE+10), rnd, t0);
	    firstEP_ch3 = true;
	  }
	  else if(mYchID==3 && firstEP_ch3 == true){
	    //wfS_ch3->addNextWF( (timeSmeardAndQE+10), t0);
	    //delete wfS_ch2;
	  }
	  /////////////////////////////

	}
	nPe++;
      }

      //}
      //}
    }

    if(wfS_ch2!=0){
      //waveform *wf_ch2 = new waveform( wfS_ch2->_wfReal, usbConst::usbwcdTimesampling); 
      //cout<<wf_ch2->GetFirstTimePosCFD()<<endl;
      //twf_ch2 = wf_ch2->GetFirstTimePosCFD();
      //delete wf_ch2;
    }

    if(wfS_ch3!=0){
      //waveform *wf_ch3 = new waveform( wfS_ch3->_wfReal, usbConst::usbwcdTimesampling); 
      //cout<<wf_ch3->GetFirstTimePosCFD()<<endl;
      //twf_ch3 = wf_ch3->GetFirstTimePosCFD();
      //delete wf_ch3;
    }

    if(twf_ch2 != -999.0 && twf_ch3 != -999.0)
      h1dtimeWF_ch2ch3->Fill(twf_ch2 - twf_ch3);
    
    if(firstTime[2] != 999 && firstTime[3] != 999)
      h1dtimeFirst_ch2ch3->Fill(firstTime[2] - firstTime[3]);
      
    
    
    for(i = 0;i<anaConst::nChannels;i++){
      if(firstTime[i] != 999)
	h1Firsttime[i]->Fill(firstTime[i]);
      h1nPE[i]->Fill(myNPE[i]);
    }
    
    for(i = 0;i<8;i++){

      if(i<3){
	if(firstTime[2*i+1] != 999 && firstTime[2*i+2] != 999){
	  h1dtimeL[i]->Fill(firstTime[2*i+1] - firstTime[2*i+2]);
	  if(i==1)
	    h2dtimeLVSphi_ch1->Fill(primPhi,firstTime[2*i+1] - firstTime[2*i+2]);
	  if(firstPhotPathL[2*i+1] != -999.0 && firstPhotPathL[2*i+2] != -999.0){
	    h1dL_L[i]->Fill(firstPhotPathL[2*i+1] - firstPhotPathL[2*i+2]);
	  }	  
	}
      } 
      
      if(i>3 && i<7){
	if(firstTime[2*i+1] != 999 && firstTime[2*i+2] != 999){
	  h1dtimeL[i]->Fill(firstTime[2*i+1] - firstTime[2*i+2]);
	  if(firstPhotPathL[2*i+1] != -999.0 && firstPhotPathL[2*i+2] != -999.0){
	    h1dL_L[i]->Fill(firstPhotPathL[2*i+1] - firstPhotPathL[2*i+2]);
	  }	  
	}
      } 

      if(firstTime[i] != 999 && firstTime[i+8] != 999){
	h1dtime[i]->Fill(firstTime[i] - firstTime[i+8]);

	if(firstPhotPathL[i] != -999.0 && firstPhotPathL[i+8] != -999.0){
	  h1dL[i]->Fill(firstPhotPathL[i] - firstPhotPathL[i+8]);
	}

	if(i == 3){
	  h2dtimeVSx->Fill( trkPosX[0],firstTime[i] - firstTime[i+8]);
	  mom = TMath::Sqrt(primMomX*primMomX+primMomY*primMomY+primMomZ*primMomZ);
	  if(mom!=0.0)
	    trkTheta = TMath::ACos(primMomZ/mom)*180.0/TMath::Pi();//deg
	  else
	    trkTheta = -999.0;
	  h1trkTheta->Fill(trkTheta);
	}

	if((firstTime[i] - firstTime[i+8])==0.0 ){
	  cout<<"firstTime[i]    "<<firstTime[i]<<endl
	      <<"firstTime[i+8]  "<<firstTime[i+8]<<endl;
	}
      }
    }
    
    h1nPhotEl->Fill(nPe);
 
  }

  TFile *f = new TFile(histFileName.Data(),"RECREATE");

  for(i = 0;i<anaConst::nChannels;i++){
    h1time[i]->Write();
    h1PhotPathL[i]->Write();
    h1dtime[i]->Write();
    h1dtimeL[i]->Write();
    h1dL[i]->Write();
    h1dL_L[i]->Write();
    h1Firsttime[i]->Write();
    h1nPE[i]->Write();
  }
  
  h1nPhot->Write();
  h1nPhotEl->Write();
  h2trkPosXvstrkPosY->Write();
  h1trkPosX->Write();
  h1trkPosZ->Write();
  h2dtimeVSx->Write();
  h1trkTheta->Write();
  h2PosZvsPosY_ch3->Write();
  h1primTheta->Write();
  h1primPhi->Write();
  h2dtimeLVSphi_ch1->Write();
  h1dtimeWF_ch2ch3->Write();
  h1dtimeFirst_ch2ch3->Write();
  
  cout<<"notOk   "<<notOk<<endl
      <<"isOk    "<<isOk<<endl;
}

void slacTestANA::LoopWfSim(TString histFileName){
  Int_t i   = 0;
  Int_t jCh = 0;
  Int_t k = 0;
  Int_t mYchID = -999;
  Int_t isOk = 0;
  Int_t notOk = 0;
  Double_t timeSmeardAndQE = -999.0;
  Int_t nPeTot = 0;
  Int_t nPe[anaConst::nChannels];
  Double_t timePe[anaConst::nChannels][anaConst::nMaxPe];
  Double_t timeWf[anaConst::nChannels];
  double t0;
  /////////////WFsim/////////////
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
  getShapeFromFile("../wfSimulation/wfData/gr1SP_first.dat", 
  	   xxMcpPM_SP, yyMcpPM_SP,
  	   nPiontsMcpPM_SP, timeMax_SP);
  //getShapeFromFile("../wfSimulation/wfData/gr1SP_first_Zero.dat", 
  //	   xxMcpPM_SP, yyMcpPM_SP,
  //	   nPiontsMcpPM_SP, timeMax_SP);
  cout<<"../wfSimulation/wfData/gr1SP_first_Zero.dat"<<endl
      <<"nPiontsMcpPM_SP "<<nPiontsMcpPM_SP<<endl
      <<"timeMax_SP      "<<timeMax_SP<<endl;
  getShapeFromFile("../wfSimulation/wfData/gr1CT_first.dat", 
		   xxMcpPM_CT, yyMcpPM_CT,
		   nPiontsMcpPM_CT, timeMax_CT);
  //getShapeFromFile("../wfSimulation/wfData/gr1CT_first_Zero.dat", 
  //	   xxMcpPM_CT, yyMcpPM_CT,
  //	   nPiontsMcpPM_CT, timeMax_CT);
  cout<<"../wfSimulation/wfData/gr1CT_first_Zero.dat"<<endl
      <<"nPiontsMcpPM_CT "<<nPiontsMcpPM_CT<<endl
      <<"timeMax_CT      "<<timeMax_CT<<endl;
  getShapeFromFile("../wfSimulation/wfData/gr1CS_first.dat", 
		   xxMcpPM_CS, yyMcpPM_CS,
		   nPiontsMcpPM_CS, timeMax_CS);
  //getShapeFromFile("../wfSimulation/wfData/gr1CS_first_Zero.dat", 
  //	   xxMcpPM_CS, yyMcpPM_CS,
  //	   nPiontsMcpPM_CS, timeMax_CS);
  cout<<"../wfSimulation/wfData/gr1CS_first_Zero.dat"<<endl
      <<"nPiontsMcpPM_CS "<<nPiontsMcpPM_CS<<endl
      <<"timeMax_CS      "<<timeMax_CS<<endl;  
  wfSim *wfS[anaConst::nChannels];
  Double_t noiseRMS = 0.0015;
  cout<<"  noiseRMS                 "<<noiseRMS<<endl
      <<"  anaConst::sigTTS         "<<anaConst::sigTTS<<endl
      <<"  anaConst::sigEle         "<<anaConst::sigEle<<endl
      <<"  anaConst::collectionEff  "<<anaConst::collectionEff<<endl;

  cout<<" cuts on muon track "<<endl
      <<"  phiMin     "<<anaConst::phiMin<<endl
      <<"  phiMax     "<<anaConst::phiMax<<endl
      <<"  thetaMin   "<<anaConst::thetaMin<<endl
      <<"  thetaMax   "<<anaConst::thetaMax<<endl;

  ///////////////////////////////
  
  /////////////WFANA////////////
  waveform *wf[anaConst::nChannels];
  const Int_t nSpline = 5;
  Double_t signalThreshold = 30.0/1000.0;
  Double_t crossTalkThreshold = -10.0/1000.0;
  Double_t cfdL = 0.5;
  Double_t mpFraction = 0.8;
  Int_t    nPointBaseLine_my = 6;
  Int_t    nPointBaseLine    = nPointBaseLine_my + nSpline*(nPointBaseLine_my-1);
  
  cout<<"nSpline             = "<<nSpline<<endl
      <<"signalThreshold     = "<<signalThreshold<<endl
      <<"crossTalkThreshold  = "<<crossTalkThreshold<<endl
      <<"cfdL                = "<<cfdL<<endl
      <<"mpFraction          = "<<mpFraction<<endl
      <<"nPointBaseLine      = "<<nPointBaseLine<<endl;
  //////////////////////////////
  
  ////////////Histos////////////
  TH1D *h1dTimeAllPe[anaConst::nChannels];
  TH1D *h1Time[anaConst::nChannels];
  TH1D *h1dTimeBoard[anaConst::nChannels];
  TH1D *h1dTimeBoardFIRST[anaConst::nChannels];
  TH1D *h1Amplitude[anaConst::nChannels];
  TH1D *h1Charge[anaConst::nChannels];
  TH1D *h1AmplitudeNeg[anaConst::nChannels];
  TH1D *h1wfID[anaConst::nChannels];
  TH1D *h1nPe[anaConst::nChannels];
  TH1D *h1RiseTimeNoCut[anaConst::nChannels];
  initH1_F(h1dTimeAllPe,"h1dTimeAllPe","dTime All Pe",1100,-0.1,10.0);
  initH1_F(h1Time,"h1Time","Time",8100,-1.0,80.0);
  initH1_F(h1dTimeBoard,"h1dTimeBoard","dTime same Board",20000,-10.0,10.0);
  initH1_F(h1dTimeBoardFIRST,"h1dTimeBoardFIRST","dTime same Board FIRST",20000,-10.0,10.0);
  initH1_F(h1Amplitude, "h1Amplitude" , "Amplitude", 10000, -5.0, 5.0);
  initH1_F(h1AmplitudeNeg, "h1AmplitudeNeg" , "AmplitudeNeg", 10000, -5.0, 5.0);
  initH1_F(h1wfID,"h1wfID","wfID",11,-5.0,5.0);
  initH1_F(h1nPe,"h1nPe","nPe",21,-1.0,20.0);
  initH1_F(h1Charge,"h1Charge","Charge",4000,-20,20);
  initH1_F(h1RiseTimeNoCut,"h1RiseTimeNoCut","Rise Time NoCut",300,0.0,2.0);

  TH1D *h1TimeAll = new TH1D("h1TimeAll","TimeAll",8100,-1.0,80.0);

  TH1D *h1time_1pe[anaConst::nChannels];
  TH1D *h1Firsttime_1pe[anaConst::nChannels];
  TH1D *h1time_2pe[anaConst::nChannels];
  TH1D *h1Firsttime_2pe[anaConst::nChannels];
  TH1D *h1time_3pe[anaConst::nChannels];
  TH1D *h1Firsttime_3pe[anaConst::nChannels];
  TH1D *h1time_4pe[anaConst::nChannels];
  TH1D *h1Firsttime_4pe[anaConst::nChannels];
  TH1D *h1time_Allpe[anaConst::nChannels];
  TH1D *h1Firsttime_Allpe[anaConst::nChannels];
  TH1D *h1Firsttime_Allpe_wfSim[anaConst::nChannels];

  TH1D *h1Amplitude_1pe[anaConst::nChannels];
  TH1D *h1Charge_1pe[anaConst::nChannels];
  TH1D *h1RiseTime_1pe[anaConst::nChannels];
  TH1D *h1Amplitude_2pe[anaConst::nChannels];
  TH1D *h1Charge_2pe[anaConst::nChannels];
  TH1D *h1RiseTime_2pe[anaConst::nChannels];
  TH1D *h1Amplitude_3pe[anaConst::nChannels];
  TH1D *h1Charge_3pe[anaConst::nChannels];
  TH1D *h1RiseTime_3pe[anaConst::nChannels];
  TH1D *h1Amplitude_4pe[anaConst::nChannels];
  TH1D *h1Charge_4pe[anaConst::nChannels];
  TH1D *h1RiseTime_4pe[anaConst::nChannels];
  TH1D *h1Amplitude_Allpe[anaConst::nChannels];
  TH1D *h1Charge_Allpe[anaConst::nChannels];
  TH1D *h1RiseTime_Allpe[anaConst::nChannels];

  ///////////////dTime measurments////////////////
  Int_t kCh1 = 0;
  Int_t kCh2 = 0;

  TH1D *h1dTimeL_S[anaConst::nChannels];       //
  TH1D *h1dTimeL2_S[anaConst::nChannels];      //
  TH1D *h1dTimeL3_S[anaConst::nChannels];      //
  TH1D *h1dTimeL4_S[anaConst::nChannels];      //
  TH1D *h1dTimeTOPtoBOT_S[anaConst::nChannels];//
  initH1_F(h1dTimeL_S,"h1dTimeL_S","dTimeL sim"                     ,  1400, -10.0, 10);
  initH1_F(h1dTimeL2_S,"h1dTimeL2_S","dTimeL2 sim"                  ,  1400, -10.0, 10);
  initH1_F(h1dTimeL3_S,"h1dTimeL3_S","dTimeL3 sim"                  ,  1400, -10.0, 10);
  initH1_F(h1dTimeL4_S,"h1dTimeL4_S","dTimeL4 sim"                  ,  1400, -10.0, 10);
  initH1_F(h1dTimeTOPtoBOT_S,"h1dTimeTOPtoBOT_S","dTimeTOPtoBOT sim",  1400, -10.0, 10);

  TH1D *h1dTimeL_first[anaConst::nChannels];           //
  TH1D *h1dTimeL_first_TrkRec[anaConst::nChannels];    //
  TH1D *h1dTimeL_first_TrkRecCut[anaConst::nChannels]; //
  TH1D *h1dTimeL2_first[anaConst::nChannels];          //
  TH1D *h1dTimeL3_first[anaConst::nChannels];          //
  TH1D *h1dTimeL4_first[anaConst::nChannels];          //
  TH1D *h1dTimeTOPtoBOT_first[anaConst::nChannels];    //

  initH1_F(h1dTimeL_first,"h1dTimeL_first","dTimeL first"                               ,  1400, -10.0, 10);
  initH1_F(h1dTimeL_first_TrkRec,"h1dTimeL_first_TrkRec","dTimeL first TrkRec"          ,  1400, -10.0, 10);
  initH1_F(h1dTimeL_first_TrkRecCut,"h1dTimeL_first_TrkRecCut","dTimeL first TrkRecCut" ,  1400, -10.0, 10);
  initH1_F(h1dTimeL2_first,"h1dTimeL2_first","dTimeL2 first"                  ,  1400, -10.0, 10);
  initH1_F(h1dTimeL3_first,"h1dTimeL3_first","dTimeL3 first"                  ,  1400, -10.0, 10);
  initH1_F(h1dTimeL4_first,"h1dTimeL4_first","dTimeL4 first"                  ,  1400, -10.0, 10);
  initH1_F(h1dTimeTOPtoBOT_first,"h1dTimeTOPtoBOT_first","dTimeTOPtoBOT first",  1400, -10.0, 10);

  TH1D *h1Charge_my_cut[anaConst::nChannels];
  TH1D *h1Amplitude_my_cut[anaConst::nChannels];
  TH1D *h1Time_my_cut[anaConst::nChannels];
  TH1D *h1Rise_cut[anaConst::nChannels];
  TH1D *h1Width_cut[anaConst::nChannels];

  initH1_F(h1Charge_my_cut,"h1Charge_my_cut","Charge my cut"         ,1000, -10.0, 10.0);
  initH1_F(h1Amplitude_my_cut,"h1Amplitude_my_cut","Amplitude my cut",1000,-1.5,1.5);
  initH1_F(h1Time_my_cut,"h1Time_my_cut","Time my cut"               ,1000,0.0,80.0);
  initH1_F(h1Rise_cut,"h1Rise_cut","Rise cut"                        ,300,0.0,3.0);
  initH1_F(h1Width_cut,"h1Width_cut","Width cut"                     ,300,0.0,10.0);

  Int_t    wfID_kCh1;
  Double_t Charge_kCh1;
  Double_t Amplitude_kCh1;
  Double_t Time_kCh1;
  Double_t Rise_kCh1;
  Double_t Width_kCh1;

  Int_t    wfID_kCh2;
  Double_t Charge_kCh2;
  Double_t Amplitude_kCh2;
  Double_t Time_kCh2;
  Double_t Rise_kCh2;
  Double_t Width_kCh2;
  ////////////////////////////////////////////////

  initH1_F(h1time_1pe, "h1time_1pe" , "time 1pe", 1000, 0.0, 20.0);
  initH1_F(h1Firsttime_1pe, "h1Firsttime_1pe" , "First time 1pe", 1000, 0.0, 20.0);
  initH1_F(h1time_2pe, "h1time_2pe" , "time 2pe", 1000, 0.0, 20.0);
  initH1_F(h1Firsttime_2pe, "h1Firsttime_2pe" , "First time 2pe", 1000, 0.0, 20.0);
  initH1_F(h1time_3pe, "h1time_3pe" , "time 3pe", 1000, 0.0, 20.0);
  initH1_F(h1Firsttime_3pe, "h1Firsttime_3pe" , "First time 3pe", 1000, 0.0, 20.0);
  initH1_F(h1time_4pe, "h1time_4pe" , "time 4pe", 1000, 0.0, 20.0);
  initH1_F(h1Firsttime_4pe, "h1Firsttime_4pe" , "First time 4pe", 1000, 0.0, 20.0);
  initH1_F(h1time_Allpe, "h1time_Allpe" , "time Allpe", 1000, 0.0, 20.0);
  initH1_F(h1Firsttime_Allpe, "h1Firsttime_Allpe" , "First time Allpe", 1000, 0.0, 20.0);
  initH1_F(h1Firsttime_Allpe_wfSim, "h1Firsttime_Allpe_wfSim" , "First time Allpe wfSim", 1000, 0.0, 20.0);

  initH1_F(h1Amplitude_1pe, "h1Amplitude_1pe" , "Amplitude 1pe", 1000, -5.0, 5.0);
  initH1_F(h1Charge_1pe,"h1Charge_1pe","Charge 1pe",4000,-20,20);
  initH1_F(h1RiseTime_1pe,"h1RiseTime_1pe","Rise Time 1pe",300,0.0,2.0);
  initH1_F(h1Amplitude_2pe, "h1Amplitude_2pe" , "Amplitude 2pe", 1000, -5.0, 5.0);
  initH1_F(h1Charge_2pe,"h1Charge_2pe","Charge 2pe",4000,-20,20);
  initH1_F(h1RiseTime_2pe,"h1RiseTime_2pe","Rise Time 2pe",300,0.0,2.0);
  initH1_F(h1Amplitude_3pe, "h1Amplitude_3pe" , "Amplitude 3pe", 1000, -5.0, 5.0);
  initH1_F(h1Charge_3pe,"h1Charge_3pe","Charge 3pe",4000,-20,20);
  initH1_F(h1RiseTime_3pe,"h1RiseTime_3pe","Rise Time 3pe",300,0.0,2.0);
  initH1_F(h1Amplitude_4pe, "h1Amplitude_4pe" , "Amplitude 4pe", 1000, -5.0, 5.0);
  initH1_F(h1Charge_4pe,"h1Charge_4pe","Charge 4pe",4000,-20,20);
  initH1_F(h1RiseTime_4pe,"h1RiseTime_4pe","Rise Time 4pe",300,0.0,2.0);
  initH1_F(h1Amplitude_Allpe, "h1Amplitude_Allpe" , "Amplitude Allpe", 1000, -5.0, 5.0);
  initH1_F(h1Charge_Allpe,"h1Charge_Allpe","Charge Allpe",4000,-20,20);
  initH1_F(h1RiseTime_Allpe,"h1RiseTime_Allpe","Rise Time Allpe",300,0.0,2.0);
  //////////////////////////////  

  TH1D *h1nPhotEl   = new TH1D("h1nPhotEl","n p.e",300,0.0,300.0);
  TH1D *h1sigDist   = new TH1D("h1sigDist","sig dist",17,0,17);
  Int_t nPeTotPerEv = 0;
  TH1D *h1nChSig    = new TH1D("h1nChSig","n sig channels",20,0.0,20.0);
  Int_t nChSig      = 0;

  TH1D *h1primMom   = new TH1D("h1primMom","prim Mom",2000, 0.0, 2000.0);
  TH1D *h1primTheta = new TH1D("h1primTheta","prim Theta",200, -10.0, 190.0);
  TH1D *h1primPhi   = new TH1D("h1primPhi","prim Phi",360,-180.0,180.0);

  Double_t primMom;
  Double_t primTheta;
  Double_t primPhi;

  TH2D *h2dTL_vs_primPhi_ch2 = new TH2D("h2dTL_vs_primPhi_ch2"," dTL vs primPhi ch2", 360, -180.0, 180.0, 1400, -10.0, 10.0);
  
  if (fChain == 0){ 
    cout<<" ERROR --> fChain == 0"<<endl;
    assert(0);
  }
  Long64_t nentries = fChain->GetEntriesFast();
  cout<<" nentries "<<nentries<<endl;
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    //cout<<jentry<<endl;
    //just for this file ../../DataSim/fTOFslacCRT_QUARTZ_fTOF.root
    if( jentry == 25780 || jentry == 13946 || jentry == 41645)
      continue;

    //return;
    
    //just for this file ../../DataSim/fTOFslacCRT_x0_y0_1500MeVmu.root
    //if( jentry == 1936)
    //continue;

    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    //if (jentry >= 5000) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    if(jentry%1000==0)
      cout<<jentry<<endl;
    //cout<<nPhot<<endl;

    primMom = TMath::Sqrt(primMomX*primMomX + 
			  primMomY*primMomY + 
			  primMomZ*primMomZ);
    h1primMom->Fill(primMom);
    TVector3 vTRK( primMomX, primMomY, primMomZ);
    primTheta = vTRK.Theta()*180.0/TMath::Pi();
    primPhi   = vTRK.Phi()*180.0/TMath::Pi();
    h1primTheta->Fill(primTheta);
    h1primPhi->Fill(primPhi);

    //cout<<"111111"<<endl;

    //t0 = _Rand3->Uniform(0.0,5.0);
    t0 = 0.0;

    //cout<<"222222"<<endl;

    for( jCh=0; jCh<anaConst::nChannels; jCh++){
      nPe[jCh] = 0;
      timeWf[jCh] = -999.0;
      for(k = 0;k<anaConst::nMaxPe;k++){
	timePe[jCh][k] = -999.0;
      }
      wfS[jCh] = new wfSim( _Rand3, 256, 0.3125, t0);
      wfS[jCh]->SetIdealShape_SP(nPiontsMcpPM_SP, xxMcpPM_SP, 
				 yyMcpPM_SP, timeMax_SP);
      wfS[jCh]->SetIdealShape_CT(nPiontsMcpPM_CT, xxMcpPM_CT, 
				 yyMcpPM_CT, timeMax_CT);
      wfS[jCh]->SetIdealShape_CS(nPiontsMcpPM_CS, xxMcpPM_CS, 
				 yyMcpPM_CS, timeMax_CS);      
    }

    ////////////////////-->
    //cout<<nPhot<<endl;
    nPeTotPerEv = 0;
    ////////////////////-->

    for(i = 0;i<nPhot;i++){
      mYchID = chID[i];
      //if(chID[i]==mYchID){
      //cout<<chID[i]<<endl;
      //isOk++;
      //}
      //if(chID[i]!=mYchID){
      //notOk++;
      //}      
      
      if(ifBialkali_QE(Wavelength[i])){
	timeSmeardAndQE = calculateTimeSpread(Time[i]);
	//cout<<timeSmeardAndQE<<endl;
	//cout<<mYchID<<endl;
	if(mYchID>=0 && mYchID<anaConst::nChannels){
	  if(nPe[mYchID]>=anaConst::nMaxPe){
	    cout<<" ERROR --> nPe[mYchID]>=nMaxPe "<<endl
		<<"mYchID                = "<<mYchID<<endl
		<<"nPe[mYchID]           = "<<nPe[mYchID]<<endl
		<<"slacTestConst::nMaxPe = "<<anaConst::nMaxPe<<endl;
	    assert(0);
	  }
	  timePe[mYchID][nPe[mYchID]] = timeSmeardAndQE;
	  nPe[mYchID] = nPe[mYchID]+1;	  
	  if(timeSmeardAndQE >= 0.0){
	    wfS[mYchID]->genMCPPMT_SP_WF(timeSmeardAndQE);
	    //for( jCh=0; jCh<anaConst::nChannels; jCh++){
	    //if(jCh!=mYchID){
	    //wfS[jCh]->genMCPPMT_CT_WF(timeSmeardAndQE+0.4);
	    //}
	    //}
	    //if((mYchID-1)>=0)
	    //wfS[mYchID-1]->genMCPPMT_CS_WF(timeSmeardAndQE);
	    //if((mYchID+1)<anaConst::nChannels)
	    //wfS[mYchID+1]->genMCPPMT_CS_WF(timeSmeardAndQE);
	  }  
	}
	if(mYchID != 15){
	  nPeTot++;
	  nPeTotPerEv++;
	}
      }
    }

    h1nPhotEl->Fill(nPeTotPerEv);

    findFirstTime( timePe, nPe);

    nChSig = 0;

    for( jCh=0; jCh<anaConst::nChannels; jCh++){
      if(nPe[jCh]==1){
	h1Firsttime_1pe[jCh]->Fill(timePe[jCh][0]);
	for(i=0;i<nPe[jCh];i++)
	  h1time_1pe[jCh]->Fill(timePe[jCh][i]);
      }      
      if(nPe[jCh]==2){
	h1Firsttime_2pe[jCh]->Fill(timePe[jCh][0]);
	for(i=0;i<nPe[jCh];i++)
	  h1time_2pe[jCh]->Fill(timePe[jCh][i]);
      }
      if(nPe[jCh]==3){
	h1Firsttime_3pe[jCh]->Fill(timePe[jCh][0]);
	for(i=0;i<nPe[jCh];i++)
	  h1time_3pe[jCh]->Fill(timePe[jCh][i]);
      }
      if(nPe[jCh]==4){
	h1Firsttime_4pe[jCh]->Fill(timePe[jCh][0]);
	for(i=0;i<nPe[jCh];i++)
	  h1time_4pe[jCh]->Fill(timePe[jCh][i]);
      }
      h1Firsttime_Allpe[jCh]->Fill(timePe[jCh][0]+10.0 - 0.66999);
      for(i=0;i<nPe[jCh];i++)
	h1time_Allpe[jCh]->Fill(timePe[jCh][i]);


      if(nPe[jCh]!=0 && jCh != 15)
	nChSig++;
      
    }
    
    h1nChSig->Fill(nChSig);

    for( jCh=0; jCh<anaConst::nChannels; jCh++){
      if(nChSig==3){
	if(nPe[jCh]!=0 && jCh != 15){
	  h1sigDist->Fill(jCh);
	}
      }
    }

    for( jCh=0; jCh<anaConst::nChannels; jCh++){
      if(nPe[jCh]>1){
	for(k = 1;k<nPe[jCh];k++){
	  h1dTimeAllPe[jCh]->Fill(timePe[jCh][k] - timePe[jCh][0]);
	}
      }
    }
    
    if(noiseRMS != 0.0){
      for( jCh=0; jCh<anaConst::nChannels; jCh++){	
	if(nPe[jCh]>0){
	  wfS[jCh]->generateNoiseGauss(noiseRMS);
	}
      }
    }
    
    for( jCh=0; jCh<anaConst::nChannels; jCh++){
      wf[jCh] = new waveform( wfS[jCh]->getWFT(), wfS[jCh]->getWFA(), 256, nSpline);
      //wf[jCh] = new waveform( wfS[jCh]->getWFT(), wfS[jCh]->getWFA(), 256);
      wf[jCh]->SetSignalThreshold(signalThreshold);
      wf[jCh]->SetCrossTalkThreshold(crossTalkThreshold);
      wf[jCh]->SetMpFraction(mpFraction);
      wf[jCh]->SetNpointBaseLine(nPointBaseLine);
      wf[jCh]->findParametersOftheWaveform();
      timeWf[jCh] = wf[jCh]->findFirstTimePosCFD(cfdL);
      h1TimeAll->Fill(timeWf[jCh]);
      h1Firsttime_Allpe_wfSim[jCh]->Fill(timeWf[jCh]);
      if(wf[jCh]->GetWfID()==1)
	h1RiseTimeNoCut[jCh]->Fill(wf[jCh]->GetFirstRiseTimePos());
    }

    for( jCh=0; jCh<anaConst::nChannels; jCh++){
      //if(jCh == 3){
      if(nPe[jCh]==1){
	h1Amplitude_1pe[jCh]->Fill(wf[jCh]->GetFirstAmplitudePos());
	h1Charge_1pe[jCh]->Fill(wf[jCh]->GetChargeTOT());
	h1RiseTime_1pe[jCh]->Fill(wf[jCh]->GetFirstRiseTimePos());
      }
      if(nPe[jCh]==2){
	h1Amplitude_2pe[jCh]->Fill(wf[jCh]->GetFirstAmplitudePos());
	h1Charge_2pe[jCh]->Fill(wf[jCh]->GetChargeTOT());
	h1RiseTime_2pe[jCh]->Fill(wf[jCh]->GetFirstRiseTimePos());
      }
      if(nPe[jCh]==3){
	h1Amplitude_3pe[jCh]->Fill(wf[jCh]->GetFirstAmplitudePos());
	h1Charge_3pe[jCh]->Fill(wf[jCh]->GetChargeTOT());
	h1RiseTime_3pe[jCh]->Fill(wf[jCh]->GetFirstRiseTimePos());
      }
      if(nPe[jCh]==4){
	h1Amplitude_4pe[jCh]->Fill(wf[jCh]->GetFirstAmplitudePos());
	h1Charge_4pe[jCh]->Fill(wf[jCh]->GetChargeTOT());
	h1RiseTime_4pe[jCh]->Fill(wf[jCh]->GetFirstRiseTimePos());
      }
      h1Amplitude_Allpe[jCh]->Fill(wf[jCh]->GetFirstAmplitudePos());
      h1Charge_Allpe[jCh]->Fill(wf[jCh]->GetChargeTOT());
      h1RiseTime_Allpe[jCh]->Fill(wf[jCh]->GetFirstRiseTimePos());
      //}
    }

    for( jCh=0; jCh<8; jCh++){
      k = 2*jCh+1;
      //if(timeWf[k]!=-999.0 && timeWf[k-1]!=-999.0){
      //if(wf[k]->GetWfID()==1 && wf[k-1]->GetWfID()==1){
      //if(TMath::Abs(wf[k]->GetFirstRiseTimePos() - wf[k-1]->GetFirstRiseTimePos())<0.15){
      //if(wf[k]->GetFirstRiseTimePos()>0.9 && wf[k]->GetFirstRiseTimePos()<1.2){
      //if(wf[k-1]->GetFirstRiseTimePos()>0.9 && wf[k-1]->GetFirstRiseTimePos()<1.2){
      h1dTimeBoard[k]->Fill(timeWf[k] - timeWf[k-1]);
      h1dTimeBoardFIRST[k]->Fill(timePe[k][0] - timePe[k-1][0]);
      h1Amplitude[k]->Fill(wf[k]->GetFirstAmplitudePos());
      h1Amplitude[k-1]->Fill(wf[k-1]->GetFirstAmplitudePos());
      h1AmplitudeNeg[k]->Fill(wf[k]->GetFirstAmplitudeNeg());
      h1AmplitudeNeg[k-1]->Fill(wf[k-1]->GetFirstAmplitudeNeg());
      h1Charge[k]->Fill(wf[k]->GetChargeTOT());
      h1Charge[k-1]->Fill(wf[k-1]->GetChargeTOT());
      h1wfID[k]->Fill(wf[k]->GetWfID());
      h1wfID[k-1]->Fill(wf[k-1]->GetWfID());
      h1nPe[k]->Fill(nPe[k]);
      h1nPe[k-1]->Fill(nPe[k-1]);
      h1Time[k]->Fill(timeWf[k]);
      h1Time[k-1]->Fill(timeWf[k-1]);
      //}
      //}
      //}
      //}
      //}
    }
    


    //////////-dTime calculation-///////////
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

      wfID_kCh1 = wf[kCh1]->GetWfID();
      Charge_kCh1 = wf[kCh1]->GetChargeTOT();
      Amplitude_kCh1 = wf[kCh1]->GetFirstAmplitudePos();
      Time_kCh1 = wf[kCh1]->findFirstTimePosCFD(cfdL);
      Rise_kCh1 = wf[kCh1]->GetFirstRiseTimePos();
      Width_kCh1 = wf[kCh1]->GetFirstWidthTimePos();

      wfID_kCh2 = wf[kCh2]->GetWfID();
      Charge_kCh2 = wf[kCh2]->GetChargeTOT();
      Amplitude_kCh2 = wf[kCh2]->GetFirstAmplitudePos();
      Time_kCh2 = wf[kCh2]->findFirstTimePosCFD(cfdL);
      Rise_kCh2 = wf[kCh2]->GetFirstRiseTimePos();
      Width_kCh2 = wf[kCh2]->GetFirstWidthTimePos();
      
      if(wfID_kCh1==1 && wfID_kCh2==1){
	if(Time_kCh1>0.0 && Time_kCh2>0.0){
	  if(Amplitude_kCh1>0.08 && Amplitude_kCh2>0.08){	    

	    if(kinemCuts( primPhi, primTheta)){
	      
	      h1dTimeL_S[kCh1]->Fill(Time_kCh1 - Time_kCh2);
	      h1dTimeL_first[kCh1]->Fill(timePe[kCh1][0] - timePe[kCh2][0]);
	      if(kCh1 == 2)
		h2dTL_vs_primPhi_ch2->Fill(primPhi,timePe[kCh1][0] - timePe[kCh2][0]);
	      
	      if(primPhi>-100 && primPhi<100){
		if(primTheta<170){
		  h1dTimeL_first_TrkRecCut[kCh1]->Fill(timePe[kCh1][0] - timePe[kCh2][0]);
		}
	      }	      
	      
	      h1Charge_my_cut[kCh1]->Fill(Charge_kCh1);
	      h1Amplitude_my_cut[kCh1]->Fill(Amplitude_kCh1);
	      h1Time_my_cut[kCh1]->Fill(Time_kCh1);
	      h1Rise_cut[kCh1]->Fill(Rise_kCh1);
	      h1Width_cut[kCh1]->Fill(Width_kCh1);
	      
	      h1Charge_my_cut[kCh2]->Fill(Charge_kCh2);
	      h1Amplitude_my_cut[kCh2]->Fill(Amplitude_kCh2);
	      h1Time_my_cut[kCh2]->Fill(Time_kCh2);
	      h1Rise_cut[kCh2]->Fill(Rise_kCh2);
	      h1Width_cut[kCh2]->Fill(Width_kCh2);

	    }

	  }
	}
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

      wfID_kCh1 = wf[kCh1]->GetWfID();
      Charge_kCh1 = wf[kCh1]->GetChargeTOT();
      Amplitude_kCh1 = wf[kCh1]->GetFirstAmplitudePos();
      Time_kCh1 = wf[kCh1]->findFirstTimePosCFD(cfdL);
      Rise_kCh1 = wf[kCh1]->GetFirstRiseTimePos();
      Width_kCh1 = wf[kCh1]->GetFirstWidthTimePos();

      wfID_kCh2 = wf[kCh2]->GetWfID();
      Charge_kCh2 = wf[kCh2]->GetChargeTOT();
      Amplitude_kCh2 = wf[kCh2]->GetFirstAmplitudePos();
      Time_kCh2 = wf[kCh2]->findFirstTimePosCFD(cfdL);
      Rise_kCh2 = wf[kCh2]->GetFirstRiseTimePos();
      Width_kCh2 = wf[kCh2]->GetFirstWidthTimePos();

      if(wfID_kCh1==1 && wfID_kCh2==1){
	if(Time_kCh1>0.0 && Time_kCh2>0.0){
	  if(Amplitude_kCh1>0.08 && Amplitude_kCh2>0.08){

	    if(kinemCuts( primPhi, primTheta)){

	      h1dTimeL2_S[kCh1]->Fill(Time_kCh1 - Time_kCh2);
	      h1dTimeL2_first[kCh1]->Fill(timePe[kCh1][0] - timePe[kCh2][0]);

	    }
	  }
	}
      }
    }
    // 9  ch9  - ch10
    // 11  ch11 - ch12
    // 13  ch13 - ch14
    for(i = 4;i<7;i++){
      kCh1 = 2*i+1;
      kCh2 = 2*i+2;
      
      wfID_kCh1 = wf[kCh1]->GetWfID();
      Charge_kCh1 = wf[kCh1]->GetChargeTOT();
      Amplitude_kCh1 = wf[kCh1]->GetFirstAmplitudePos();
      Time_kCh1 = wf[kCh1]->findFirstTimePosCFD(cfdL);
      Rise_kCh1 = wf[kCh1]->GetFirstRiseTimePos();
      Width_kCh1 = wf[kCh1]->GetFirstWidthTimePos();
      
      wfID_kCh2 = wf[kCh2]->GetWfID();
      Charge_kCh2 = wf[kCh2]->GetChargeTOT();
      Amplitude_kCh2 = wf[kCh2]->GetFirstAmplitudePos();
      Time_kCh2 = wf[kCh2]->findFirstTimePosCFD(cfdL);
      Rise_kCh2 = wf[kCh2]->GetFirstRiseTimePos();
      Width_kCh2 = wf[kCh2]->GetFirstWidthTimePos();
      
      if(wfID_kCh1==1 && wfID_kCh2==1){
	if(Time_kCh1>0.0 && Time_kCh2>0.0){
	  if(Amplitude_kCh1>0.08 && Amplitude_kCh2>0.08){

	    if(kinemCuts( primPhi, primTheta)){

	      h1dTimeL2_S[kCh1]->Fill(Time_kCh1 - Time_kCh2);
	      h1dTimeL2_first[kCh1]->Fill(timePe[kCh1][0] - timePe[kCh2][0]);

	    }

	  }
	}
      }
    }
    //---- time DIFFL2 ----

    //---- time DIFFL3 ---
    // 1  ch1 - ch2
    // 3  ch3 - ch4
    // 5  ch5 - ch6
    kCh1 = 2;
    kCh2 = 4;
    wfID_kCh1 = wf[kCh1]->GetWfID();
    Charge_kCh1 = wf[kCh1]->GetChargeTOT();
    Amplitude_kCh1 = wf[kCh1]->GetFirstAmplitudePos();
    Time_kCh1 = wf[kCh1]->findFirstTimePosCFD(cfdL);
    Rise_kCh1 = wf[kCh1]->GetFirstRiseTimePos();
    Width_kCh1 = wf[kCh1]->GetFirstWidthTimePos();
    wfID_kCh2 = wf[kCh2]->GetWfID();
    Charge_kCh2 = wf[kCh2]->GetChargeTOT();
    Amplitude_kCh2 = wf[kCh2]->GetFirstAmplitudePos();
    Time_kCh2 = wf[kCh2]->findFirstTimePosCFD(cfdL);
    Rise_kCh2 = wf[kCh2]->GetFirstRiseTimePos();
    Width_kCh2 = wf[kCh2]->GetFirstWidthTimePos();
    if(wfID_kCh1==1 && wfID_kCh2==1){
      if(Time_kCh1>0.0 && Time_kCh2>0.0){
	if(Amplitude_kCh1>0.08 && Amplitude_kCh2>0.08){

	  if(kinemCuts( primPhi, primTheta)){
	    h1dTimeL3_S[kCh1]->Fill(Time_kCh1 - Time_kCh2);
	    h1dTimeL3_first[kCh1]->Fill(timePe[kCh1][0] - timePe[kCh2][0]);
	  }
	}
      }
    }
    kCh1 = 3;
    kCh2 = 5;
    wfID_kCh1 = wf[kCh1]->GetWfID();
    Charge_kCh1 = wf[kCh1]->GetChargeTOT();
    Amplitude_kCh1 = wf[kCh1]->GetFirstAmplitudePos();
    Time_kCh1 = wf[kCh1]->findFirstTimePosCFD(cfdL);
    Rise_kCh1 = wf[kCh1]->GetFirstRiseTimePos();
    Width_kCh1 = wf[kCh1]->GetFirstWidthTimePos();
    wfID_kCh2 = wf[kCh2]->GetWfID();
    Charge_kCh2 = wf[kCh2]->GetChargeTOT();
    Amplitude_kCh2 = wf[kCh2]->GetFirstAmplitudePos();
    Time_kCh2 = wf[kCh2]->findFirstTimePosCFD(cfdL);
    Rise_kCh2 = wf[kCh2]->GetFirstRiseTimePos();
    Width_kCh2 = wf[kCh2]->GetFirstWidthTimePos();
    if(wfID_kCh1==1 && wfID_kCh2==1){
      if(Time_kCh1>0.0 && Time_kCh2>0.0){
	if(Amplitude_kCh1>0.08 && Amplitude_kCh2>0.08){

	  if(kinemCuts( primPhi, primTheta)){
	    h1dTimeL3_S[kCh1]->Fill(Time_kCh1 - Time_kCh2);
	    h1dTimeL3_first[kCh1]->Fill(timePe[kCh1][0] - timePe[kCh2][0]);
	  }

	}
      }
    }
    kCh1 = 10;
    kCh2 = 12;
    wfID_kCh1 = wf[kCh1]->GetWfID();
    Charge_kCh1 = wf[kCh1]->GetChargeTOT();
    Amplitude_kCh1 = wf[kCh1]->GetFirstAmplitudePos();
    Time_kCh1 = wf[kCh1]->findFirstTimePosCFD(cfdL);
    Rise_kCh1 = wf[kCh1]->GetFirstRiseTimePos();
    Width_kCh1 = wf[kCh1]->GetFirstWidthTimePos();
    wfID_kCh2 = wf[kCh2]->GetWfID();
    Charge_kCh2 = wf[kCh2]->GetChargeTOT();
    Amplitude_kCh2 = wf[kCh2]->GetFirstAmplitudePos();
    Time_kCh2 = wf[kCh2]->findFirstTimePosCFD(cfdL);
    Rise_kCh2 = wf[kCh2]->GetFirstRiseTimePos();
    Width_kCh2 = wf[kCh2]->GetFirstWidthTimePos();
    if(wfID_kCh1==1 && wfID_kCh2==1){
      if(Time_kCh1>0.0 && Time_kCh2>0.0){
	if(Amplitude_kCh1>0.08 && Amplitude_kCh2>0.08){

	  if(kinemCuts( primPhi, primTheta)){
	    h1dTimeL3_S[kCh1]->Fill(Time_kCh1 - Time_kCh2);
	    h1dTimeL3_first[kCh1]->Fill(timePe[kCh1][0] - timePe[kCh2][0]);
	  }

	}
      }
    }
    kCh1 = 11;
    kCh2 = 13;
    wfID_kCh1 = wf[kCh1]->GetWfID();
    Charge_kCh1 = wf[kCh1]->GetChargeTOT();
    Amplitude_kCh1 = wf[kCh1]->GetFirstAmplitudePos();
    Time_kCh1 = wf[kCh1]->findFirstTimePosCFD(cfdL);
    Rise_kCh1 = wf[kCh1]->GetFirstRiseTimePos();
    Width_kCh1 = wf[kCh1]->GetFirstWidthTimePos();
    wfID_kCh2 = wf[kCh2]->GetWfID();
    Charge_kCh2 = wf[kCh2]->GetChargeTOT();
    Amplitude_kCh2 = wf[kCh2]->GetFirstAmplitudePos();
    Time_kCh2 = wf[kCh2]->findFirstTimePosCFD(cfdL);
    Rise_kCh2 = wf[kCh2]->GetFirstRiseTimePos();
    Width_kCh2 = wf[kCh2]->GetFirstWidthTimePos();
    if(wfID_kCh1==1 && wfID_kCh2==1){
      if(Time_kCh1>0.0 && Time_kCh2>0.0){
	if(Amplitude_kCh1>0.08 && Amplitude_kCh2>0.08){

	  if(kinemCuts( primPhi, primTheta)){
	    h1dTimeL3_S[kCh1]->Fill(Time_kCh1 - Time_kCh2);
	    h1dTimeL3_first[kCh1]->Fill(timePe[kCh1][0] - timePe[kCh2][0]);
	  }
	  
	}
      }
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
      wfID_kCh1 = wf[kCh1]->GetWfID();
      Charge_kCh1 = wf[kCh1]->GetChargeTOT();
      Amplitude_kCh1 = wf[kCh1]->GetFirstAmplitudePos();
      Time_kCh1 = wf[kCh1]->findFirstTimePosCFD(cfdL);
      Rise_kCh1 = wf[kCh1]->GetFirstRiseTimePos();
      Width_kCh1 = wf[kCh1]->GetFirstWidthTimePos();
      wfID_kCh2 = wf[kCh2]->GetWfID();
      Charge_kCh2 = wf[kCh2]->GetChargeTOT();
      Amplitude_kCh2 = wf[kCh2]->GetFirstAmplitudePos();
      Time_kCh2 = wf[kCh2]->findFirstTimePosCFD(cfdL);
      Rise_kCh2 = wf[kCh2]->GetFirstRiseTimePos();
      Width_kCh2 = wf[kCh2]->GetFirstWidthTimePos();
      if(wfID_kCh1==1 && wfID_kCh2==1){
	if(Time_kCh1>0.0 && Time_kCh2>0.0){
	  if(Amplitude_kCh1>0.08 && Amplitude_kCh2>0.08){

	    if(kinemCuts( primPhi, primTheta)){
	      h1dTimeL4_S[kCh1]->Fill(Time_kCh1 - Time_kCh2);
	      h1dTimeL4_first[kCh1]->Fill(timePe[kCh1][0] - timePe[kCh2][0]);
	    }

	  }
	}
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
      wfID_kCh1 = wf[kCh1]->GetWfID();
      Charge_kCh1 = wf[kCh1]->GetChargeTOT();
      Amplitude_kCh1 = wf[kCh1]->GetFirstAmplitudePos();
      Time_kCh1 = wf[kCh1]->findFirstTimePosCFD(cfdL);
      Rise_kCh1 = wf[kCh1]->GetFirstRiseTimePos();
      Width_kCh1 = wf[kCh1]->GetFirstWidthTimePos();
      wfID_kCh2 = wf[kCh2]->GetWfID();
      Charge_kCh2 = wf[kCh2]->GetChargeTOT();
      Amplitude_kCh2 = wf[kCh2]->GetFirstAmplitudePos();
      Time_kCh2 = wf[kCh2]->findFirstTimePosCFD(cfdL);
      Rise_kCh2 = wf[kCh2]->GetFirstRiseTimePos();
      Width_kCh2 = wf[kCh2]->GetFirstWidthTimePos();
      if(wfID_kCh1==1 && wfID_kCh2==1){
	if(Time_kCh1>0.0 && Time_kCh2>0.0){
	  if(Amplitude_kCh1>0.08 && Amplitude_kCh2>0.08){      

	    if(kinemCuts( primPhi, primTheta)){
	      h1dTimeTOPtoBOT_S[kCh1]->Fill(Time_kCh1 - Time_kCh2);
	      h1dTimeTOPtoBOT_first[kCh1]->Fill(timePe[kCh1][0] - timePe[kCh2][0]);
	    }
	    
	  }
	}
      }
    }
    //---- time DIFFTOPTOBOT ---
    ////////////////////////////////////////
    


    //Generate signal
    //for( jCh=0; jCh<slacTestConst::nChannels; jCh++){
    //for( jCh=1; jCh<2; jCh++){
    //cout<<"nPe["<<jCh<<"] = "<<nPe[jCh]<<endl;
    //for(k = 0;k<nPe[jCh];k++){
    //cout<<timePe[jCh][k]<<endl;
    //}
    //}
    
    for( jCh=0; jCh<anaConst::nChannels; jCh++){
      delete wfS[jCh];
      delete wf[jCh];
    }

  }
  
  TFile *f = new TFile(histFileName.Data(),"RECREATE");
  for(jCh = 0;jCh<anaConst::nChannels;jCh++){
    h1dTimeAllPe[jCh]->Write();
    h1dTimeBoard[jCh]->Write();
    h1dTimeBoardFIRST[jCh]->Write();
    h1Amplitude[jCh]->Write();
    h1AmplitudeNeg[jCh]->Write();
    h1Charge[jCh]->Write();
    h1wfID[jCh]->Write();
    h1nPe[jCh]->Write();
    h1Time[jCh]->Write();
    h1RiseTimeNoCut[jCh]->Write();
    h1time_1pe[jCh]->Write();
    h1Firsttime_1pe[jCh]->Write();
    h1time_2pe[jCh]->Write();
    h1Firsttime_2pe[jCh]->Write();
    h1time_3pe[jCh]->Write();
    h1Firsttime_3pe[jCh]->Write();
    h1time_4pe[jCh]->Write();
    h1Firsttime_4pe[jCh]->Write();
    h1time_Allpe[jCh]->Write();
    h1Firsttime_Allpe[jCh]->Write();
    h1Firsttime_Allpe_wfSim[jCh]->Write();

    h1Amplitude_1pe[jCh]->Write();
    h1Charge_1pe[jCh]->Write();
    h1RiseTime_1pe[jCh]->Write();
    h1Amplitude_2pe[jCh]->Write();
    h1Charge_2pe[jCh]->Write();
    h1RiseTime_2pe[jCh]->Write();
    h1Amplitude_3pe[jCh]->Write();
    h1Charge_3pe[jCh]->Write();
    h1RiseTime_3pe[jCh]->Write();
    h1Amplitude_4pe[jCh]->Write();
    h1Charge_4pe[jCh]->Write();
    h1RiseTime_4pe[jCh]->Write();    
    h1Amplitude_Allpe[jCh]->Write();
    h1Charge_Allpe[jCh]->Write();
    h1RiseTime_Allpe[jCh]->Write();

    h1dTimeL_S[jCh]->Write();
    h1dTimeL2_S[jCh]->Write();
    h1dTimeL3_S[jCh]->Write();
    h1dTimeL4_S[jCh]->Write();
    h1dTimeTOPtoBOT_S[jCh]->Write();

    h1dTimeL_first[jCh]->Write();
    h1dTimeL_first_TrkRecCut[jCh]->Write();
    h1dTimeL2_first[jCh]->Write();
    h1dTimeL3_first[jCh]->Write();
    h1dTimeL4_first[jCh]->Write();
    h1dTimeTOPtoBOT_first[jCh]->Write();

    h1Charge_my_cut[jCh]->Write();
    h1Amplitude_my_cut[jCh]->Write();
    h1Time_my_cut[jCh]->Write();
    h1Rise_cut[jCh]->Write();
    h1Width_cut[jCh]->Write();

  }

  h1TimeAll->Write();
  h1nPhotEl->Write();
  h1nChSig->Write();
  h1sigDist->Write();

  h1primMom->Write();
  h1primTheta->Write();
  h1primPhi->Write();
  h2dTL_vs_primPhi_ch2->Write();
  
  //cout<<"notOk   "<<notOk<<endl
  //<<"isOk    "<<isOk<<endl
  //<<"nPeTot  "<<nPeTot<<endl;
  cout<<"nPeTot  "<<nPeTot<<endl;
}

Bool_t slacTestANA::kinemCuts( Double_t primPhi, Double_t primTheta){
  //if(primPhi>anaConst::phiMin && primPhi<anaConst::phiMax){
  //if(primTheta>anaConst::thetaMin && primTheta<anaConst::thetaMax){
  //return true;
  //}
  //}
  //if(primPhi<anaConst::phiMin || primPhi>anaConst::phiMax){
  //if(primTheta>anaConst::thetaMin && primTheta<anaConst::thetaMax){
  //return true;
  //}
  //}
  //return false;
  return true;
}

void slacTestANA::findFirstTime(double timePe[anaConst::nChannels][anaConst::nMaxPe], 
				Int_t nPe[anaConst::nChannels]){
  int chID = 0;
  int peID = 0;
  double aa;
  for(chID = 0;chID<anaConst::nChannels;chID++){
    if(nPe[chID]>0){
      for( peID = (nPe[chID]-1); peID>0; peID--){
	if(timePe[chID][peID]<timePe[chID][peID-1]){
	  aa = timePe[chID][peID-1];
	  timePe[chID][peID-1] = timePe[chID][peID];
	  timePe[chID][peID] = aa;
	}
      }
    }
  }
}

void slacTestANA::LoopSimpl(){
  if (fChain == 0){
    cout<<" ERROR --> fChain == 0"<<endl;
    assert(0);
  }
  Long64_t nentries = fChain->GetEntriesFast();
  cout<<" nentries "<<nentries<<endl;
  Long64_t nbytes = 0, nb = 0;
  for(Long64_t jentry=0; jentry<nentries;jentry++){
    //if(jentry != 25780 && jentry !=41645){
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      //if (jentry >= 45000) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      if(jentry%1000==0)
	cout<<jentry<<endl;
      //}
  }
}

void slacTestANA::getShapeFromFile(TString fileN, 
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

Int_t slacTestANA::GetChID(Double_t photPosY, Double_t  photPosZ){
  Int_t mYchID = -999;
  if(photPosY>-25.0 && photPosY<-18.8 )
    mYchID = 0;
  else if(photPosY>-18.7 && photPosY<-12.6 )
    mYchID = 1;
  else if(photPosY>-12.5 && photPosY<-6.3 )
    mYchID = 2;
  else if(photPosY>-6.2 && photPosY<-0.1 )
    mYchID = 3;
  else if(photPosY> 0.1 && photPosY< 6.2 )
    mYchID = 4;
  else if(photPosY> 6.3 && photPosY< 12.4 )
    mYchID = 5;
  else if(photPosY> 12.6 && photPosY< 18.7 )
    mYchID = 6;
  else if(photPosY> 18.8 && photPosY< 25.0 )
    mYchID = 7;

  if(photPosZ >1229 && photPosZ <1249)
    mYchID = mYchID + 8;


  /*
  if(photPosY>-25.0 && photPosY<-18.8 )
    mYchID = 0;
  else if(photPosY>-18.7 && photPosY<-12.6 )
    mYchID = 1;
  else if(photPosY>-12.5 && photPosY<-6.3 )
    mYchID = 2;
  else if(photPosY>-6.2 && photPosY<-0.1 )
    mYchID = 3;
  else if(photPosY> 0.1 && photPosY< 6.2 )
    mYchID = 4;
  else if(photPosY> 6.3 && photPosY< 12.4 )
    mYchID = 5;
  else if(photPosY> 12.6 && photPosY< 18.7 )
    mYchID = 6;
  else if(photPosY> 18.8 && photPosY< 25.0 )
    mYchID = 7;
  
  if(photPosZ> 1270 && photPosZ<1276){
    mYchID = mYchID;
  }
  else if (photPosZ< 1240 && photPosZ>1234){
    mYchID = mYchID + 8;
  }
  else
    mYchID = -999;
  */  
 
  return mYchID;
}


Double_t slacTestANA::calculateTimeSpread(Double_t TimeMy){

  Double_t dT_tts = -999.0;
  Double_t TimeSpred = -999.0;

  if(anaConst::sigTTS > 0.0){
    dT_tts = _Rand3->Gaus(0.0,anaConst::sigTTS);
    TimeSpred = TimeMy + dT_tts;
  }
  else{
    TimeSpred = TimeMy;
  }

  if(anaConst::sigEle > 0.0){
    dT_tts = _Rand3->Gaus(0.0,anaConst::sigEle);
    TimeSpred = TimeSpred + dT_tts;
  }  

  return TimeSpred;
}

Bool_t  slacTestANA::ifBialkali_QE(Double_t WavelengthMy){

  Bool_t answer = false;

  int i = -999; 
  int i_min = -999;
  double acceptProb = -999.0;
  double xmin = -999.0;
  double xmax = -999.0;
  double ymin = -999.0;
  double ymax = -999.0;
  double WL_min = -999.0;
  double WL_max = -999.0;

  const int nPoint = 32;
  double Lambda[nPoint];
  double QE_Bialkali[nPoint];

  //Here are values of Burle QE of a Bialkali photocathode:
  //Lambda [nm]

  WL_min = 300.0;
  WL_max = 610.0;

  Lambda[0] = 300;
  Lambda[1] = 310;
  Lambda[2] = 320;
  Lambda[3] = 330;
  Lambda[4] = 340;
  Lambda[5] = 350;
  Lambda[6] = 360;
  Lambda[7] = 370;
  Lambda[8] = 380;
  Lambda[9] = 390;
  Lambda[10] = 400;
  Lambda[11] = 410;
  Lambda[12] = 420;
  Lambda[13] = 430;
  Lambda[14] = 440;
  Lambda[15] = 450;
  Lambda[16] = 460;
  Lambda[17] = 470;
  Lambda[18] = 480;
  Lambda[19] = 490;
  Lambda[20] = 500;
  Lambda[21] = 510;
  Lambda[22] = 520;
  Lambda[23] = 530;
  Lambda[24] = 540;
  Lambda[25] = 550;
  Lambda[26] = 560;
  Lambda[27] = 570;
  Lambda[28] = 580;
  Lambda[29] = 590;
  Lambda[30] = 600;
  Lambda[31] = 610;

  QE_Bialkali[0] = 0.085;
  QE_Bialkali[1] = 0.102;
  QE_Bialkali[2] = 0.119;
  QE_Bialkali[3] = 0.136;
  QE_Bialkali[4] = 0.153;
  QE_Bialkali[5] = 0.17;
  QE_Bialkali[6] = 0.187;
  QE_Bialkali[7] = 0.204;
  QE_Bialkali[8] = 0.221;
  QE_Bialkali[9] = 0.238;
  QE_Bialkali[10] = 0.2465;
  QE_Bialkali[11] = 0.2448;
  QE_Bialkali[12] = 0.2431;
  QE_Bialkali[13] = 0.2414;
  QE_Bialkali[14] = 0.2397;
  QE_Bialkali[15] = 0.238;
  QE_Bialkali[16] = 0.221;
  QE_Bialkali[17] = 0.1955;
  QE_Bialkali[18] = 0.17;
  QE_Bialkali[19] = 0.14875;
  QE_Bialkali[20] = 0.1275;
  QE_Bialkali[21] = 0.10625;
  QE_Bialkali[22] = 0.085;
  QE_Bialkali[23] = 0.068;
  QE_Bialkali[24] = 0.051;
  QE_Bialkali[25] = 0.04675;
  QE_Bialkali[26] = 0.0425;
  QE_Bialkali[27] = 0.034;
  QE_Bialkali[28] = 0.0255;
  QE_Bialkali[29] = 0.017;
  QE_Bialkali[30] = 0.01275;
  QE_Bialkali[31] = 0.0085;

  if(WL_min >  WavelengthMy|| WL_max < WavelengthMy)
    return false;

  for(i=0;i<nPoint;i++){
    if(Lambda[i]>WavelengthMy){
      i_min = i-1;
      break;
    }
  }

  xmin = Lambda[i_min];
  xmax = Lambda[i_min+1];
  ymin = QE_Bialkali[i_min];
  ymax = QE_Bialkali[i_min+1];

  acceptProb = interpolateAcceptProb(xmin, xmax,
				     ymin, ymax,
				     WavelengthMy);

  acceptProb = acceptProb*(anaConst::collectionEff);

  double randVal = _Rand3->Uniform();
  //double randVal = 0.5;

  //cout<<"------------"<<endl
  //<<" Wavelength  "<<WavelengthMy<<" acceptProb "<<acceptProb<<"  randVal  "<<randVal<<endl
  //<<" Wavelength min  "<<xmin<<"  Wavelength max  "<<xmax<<endl
  //<<" acceptProb left "<<ymin<<"  acceptProb lright "<<ymax<<endl
  //<<"************"<<endl;
  
  if(acceptProb>=randVal)
    answer = true;

  return answer; 
}

double slacTestANA::interpolateAcceptProb( double xmin, double xmax,
					   double ymin, double ymax, 
					   double x){
  double v = -999.0;
  double d = xmax - xmin;
  if(d>0.0)
    v = ((ymax-ymin)/d*(x-xmin)+ymin);
  else{ 
    cout<<"ERROR ---> xmax - xmin < 0.0"<<endl;
    assert(0);
  }  
  return v;
}

void slacTestANA::normalizeh1QE(TH1D *hh1,TH1D *h1norm){
  int i = -999; 
  double val = -999.0;
  double norm = -999.0;
  double newVal = -999.0;
  for(i=1;i<=100;i++){
    val  = (double)hh1->GetBinContent(i);
    norm = (double)h1norm->GetBinContent(i);
    if(norm>0.0)
      newVal = val/norm;
    else
      newVal = 0.0;
    hh1->SetBinContent(i,newVal);
  }  
}

void slacTestANA::initH1_F(TH1D *h[anaConst::nChannels],
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

Int_t slacTestANA::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

Long64_t slacTestANA::LoadTree(Long64_t entry){
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

void slacTestANA::Init(TTree *tree){
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
   fChain->SetBranchAddress("EventID", &EventID, &b_Event);
   fChain->SetBranchAddress("BunchXID", &BunchXID, &b_BunchXID);
   fChain->SetBranchAddress("NTotPhot", &NTotPhot, &b_NTotPhot);
   fChain->SetBranchAddress("Nhits", &Nhits, &b_Nhits);
   fChain->SetBranchAddress("primType", &primType, &b_primType);
   fChain->SetBranchAddress("primMomX", &primMomX, &b_primMomX);
   fChain->SetBranchAddress("primMomY", &primMomY, &b_primMomY);
   fChain->SetBranchAddress("primMomZ", &primMomZ, &b_primMomZ);
   fChain->SetBranchAddress("primPosX", &primPosX, &b_primPosX);
   fChain->SetBranchAddress("primPosY", &primPosY, &b_primPosY);
   fChain->SetBranchAddress("primPosZ", &primPosZ, &b_primPosZ);
   fChain->SetBranchAddress("primTime", &primTime, &b_primTime);
   fChain->SetBranchAddress("nPhot", &nPhot, &b_nPhot);
   fChain->SetBranchAddress("TrackID", TrackID, &b_TrackID);
   fChain->SetBranchAddress("ParentID", ParentID, &b_ParentID);
   fChain->SetBranchAddress("Energy", Energy, &b_Energy);
   fChain->SetBranchAddress("Wavelength", Wavelength, &b_Wavelength);
   fChain->SetBranchAddress("Time", Time, &b_Time);
   fChain->SetBranchAddress("photPathLen", photPathLen, &b_photPathLen);
   fChain->SetBranchAddress("SecID", SecID, &b_SecID);
   fChain->SetBranchAddress("chID", chID, &b_chID);
   fChain->SetBranchAddress("PosX", PosX, &b_PosX);
   fChain->SetBranchAddress("PosY", PosY, &b_PosY);
   fChain->SetBranchAddress("PosZ", PosZ, &b_PosZ);
   fChain->SetBranchAddress("trkMomX", trkMomX, &b_trkMomX);
   fChain->SetBranchAddress("trkMomY", trkMomY, &b_trkMomY);
   fChain->SetBranchAddress("trkMomZ", trkMomZ, &b_trkMomZ);
   fChain->SetBranchAddress("trkPosX", trkPosX, &b_trkPosX);
   fChain->SetBranchAddress("trkPosY", trkPosY, &b_trkPosY);
   fChain->SetBranchAddress("trkPosZ", trkPosZ, &b_trkPosZ);
   fChain->SetBranchAddress("trkT", trkT, &b_trkT);
   fChain->SetBranchAddress("trkLength", trkLength, &b_trkLength);
   Notify();
}

Bool_t slacTestANA::Notify(){
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.
   return kTRUE;
}

void slacTestANA::Show(Long64_t entry){
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

Int_t slacTestANA::Cut(Long64_t entry){
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
