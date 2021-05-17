{
  Long64_t eventJentry;
  Int_t chID;
  gROOT->LoadMacro("./sigVisSL10.C");
  gROOT->LoadMacro("../wfSimulation/src/waveform.cpp");
  gROOT->LoadMacro("../anaConst.hh");
  waveform *wf;

  //////////////////////////////////////////
  //////////////////////////////////////////
  //sigVisSL10 t("/home/leonid/theth/LAL-test-Bench/RootFiles/SL10_22.06.2011/Run_SL10_3.32kV_Data_6_22_2011/Run_SL10_3.32kV_Data_6_22_2011.root");
  sigVisSL10 t("/home/leonid/theth/LAL-test-Bench/RootFiles/SL10_27.06.2011/Run_sl10_3.4kv_Data_6_27_2011/Run_sl10_3.4kv_Data_6_27_2011.root");
  eventJentry = 94;
  chID = 0;
  //////////////////////////////////////////
  //////////////////////////////////////////
  
  t.GetEntry(eventJentry);
  Double_t wfArrT[anaConst::usbwcNsamplingPoint];
  Double_t wfArrA[anaConst::usbwcNsamplingPoint];
  for(Int_t i = 0;i<anaConst::usbwcNsamplingPoint;i++){
    wfArrA[i] = -1.0*t.ch[chID][i];
    wfArrT[i] = t.SamplingPeriod/1000.0*i;
  }

  Int_t nSplineP = 5;
  Int_t nPointBaisLine_realP = 10;
  Int_t nPointBaisLine = nPointBaisLine_realP + nSplineP*(nPointBaisLine_realP-1);
  //Double_t SignalThreshold = 5.0/1000.0;
  //Double_t crossTalkThreshold = -3.0/1000.0;
  //ch 4 (Trigger)///////////////////////////
  //Double_t SignalThreshold = 20.0/1000.0;
  //Double_t crossTalkThreshold = -10.0/1000.0;
  ///////////////////////////////////////////
  //(Signal)///////////////////////////
  Double_t SignalThreshold = 4.0/1000.0;
  Double_t crossTalkThreshold = -4.0/1000.0;
  ///////////////////////////////////////////
  Double_t mpFraction = 0.8;
  Double_t cfdVal = 0.5;
  wf = new waveform(wfArrT,wfArrA,anaConst::usbwcNsamplingPoint, nSplineP);
  wf->SetNpointBaseLine(nPointBaisLine);
  wf->SetSignalThreshold(SignalThreshold);
  wf->SetCrossTalkThreshold(crossTalkThreshold);
  wf->SetMpFraction(mpFraction);
  wf->findParametersOftheWaveform();
  wf->PrintWaveFormInfo();
  wf->Draw(cfdVal);
}
