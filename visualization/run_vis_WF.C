{
  gROOT->LoadMacro("../mergeCRTandUSB/src/usbwcFileStr.cpp");
  gROOT->LoadMacro("../wfSimulation/src/waveform.cpp");
  //gROOT->LoadMacro("../anaConst.hh");
  usbwcFileStr *usbStr = new usbwcFileStr();
  Long64_t entry;

  entry = 12;
  chID  = 10;
  usbStr->addFilesToChain("../rootFileList/rootFileListUSB_RUN4.txt");
  usbStr->GetDataRate();
  usbStr->GetEntry((entry));

  Double_t wfMyA[anaConst::usbwcNsamplingPoint];
  Double_t wfMyT[anaConst::usbwcNsamplingPoint];

  for(Int_t j = 0; j<anaConst::usbwcNsamplingPoint; j++){
    wfMyA[j] = usbStr->ch[chID][j];
    wfMyT[j] = j*usbStr->SamplingPeriod/1000.0;
  }

  Double_t signalThreshold = 30.0/1000.0;//V
  Double_t crossTalkThreshold = -10.0/1000.0;//V
  Double_t mpFraction = 0.8;
  Int_t nPointBaseLineR = 6;
  Int_t nSplinePoints = 0;
  Int_t nPointBaseLine = nPointBaseLineR + (nPointBaseLineR-1)*nSplinePoints;
  Double_t cfdVal = 0.5;
  Double_t level = -0.4;//V
  waveform *wf = new waveform( wfMyT, wfMyA, anaConst::usbwcNsamplingPoint, nSplinePoints);
  //waveform *wf = new waveform( wfMyT, wfMyA, anaConst::usbwcNsamplingPoint);
  wf->SetSignalThreshold(signalThreshold);
  wf->SetCrossTalkThreshold(crossTalkThreshold);
  wf->SetMpFraction(mpFraction);
  wf->SetNpointBaseLine(nPointBaseLine);
  wf->findParametersOftheWaveform();
  wf->PrintWaveFormInfo();
  wf->Draw(cfdVal);
  //wf->Draw15(level);
  //usbStr->DrawUSBWCevent(12);
}
