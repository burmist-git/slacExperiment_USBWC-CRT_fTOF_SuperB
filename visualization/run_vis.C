{
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);

  gROOT->LoadMacro("../mergeCRTandUSB/src/usbwcFileStr.cpp");
  gROOT->LoadMacro("../wfSimulation/src/waveform.cpp");
  usbwcFileStr *usbStr = new usbwcFileStr();
  //usbStr->addFilesToChain("../rootFileList/rootFileListUSB_RUN4.txt");
  //usbStr->addFilesToChain("../rootFileList/rootFileListUSB_RUN2.txt");
  usbStr->addFilesToChain("../rootFileList/rootFileListUSB_laser.txt");
  usbStr->GetDataRate();

  //RUN4
  //usbStr->DrawUSBWCevent(123911);

  usbStr->DrawUSBWCevent(2);
  //usbStr->DrawUSBWCeventCh(124000,4);

  //RUN2
  //usbStr->DrawUSBWCevent(1044);
}
