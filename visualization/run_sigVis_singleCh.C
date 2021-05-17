{
  Long64_t eventJentry;
  Int_t chID;
  gROOT->LoadMacro("sigVis.C");
  gROOT->LoadMacro("./src/waveform.cpp");
  waveform *wf;

  //////////////////////////////////
  sigVis t("../rootFileList/rootFileListUSB_laser.txt");
  //sigVis t("../rootFileList/rootFileListUSB_RUN1.txt");
  //sigVis t("../rootFileList/rootFileListUSB_RUN2.txt");
  //sigVis t("../rootFileList/rootFileListUSB_RUN3.txt");

  eventJentry = 71;
  chID = 2;
  //////////////////////////////////
  
  t.GetEntry(eventJentry);
  Double_t wfArr[usbConst::usbwcNsamplingPoint];
  for(Int_t i = 0;i<usbConst::usbwcNsamplingPoint;i++){
    wfArr[i] = t.ch_usb[chID][i];
  }

  wf = new waveform(wfArr,t.SamplingPeriod_usb/1000.0);
  wf->PrintWaveFormInfo();
  wf->Draw();
}
