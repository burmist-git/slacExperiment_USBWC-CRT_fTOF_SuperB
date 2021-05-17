{

  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);

  TRandom3 *rnd = new TRandom3(12431231);
  gROOT->LoadMacro("../wfSimulation/src/wfSim.cpp");
  gROOT->LoadMacro("../wfSimulation/src/waveform.cpp");
  gROOT->LoadMacro("../anaConst.hh");
  wfSim *wfS = new wfSim( rnd, anaConst::usbwcNsamplingPoint, anaConst::usbwcSamplingPeriod, 1.2);
  wfS->SetDigitTime0(4.0);
  //wfS->genGaussWF( 10.0, 0.65);
  //wfS->genGaussWFAmpl( 11.0, 1.5, 0.8);
  //wfS->genTriangleWF( 20.0, 2.3);
  wfS->genSinWF( 0.2, 0.4, 0.0);
  //wfS->genMCPPMT_SP_WF(10.0);
  wfS->generateNoiseGauss(0.013);
  wfS->Draw();
  //waveform *wf = new waveform( wfS->getWFT(), wfS->getWFA(), anaConst::usbwcNsamplingPoint, 5);
  //wf->findParametersOftheWaveform();
  //wf->Draw(0.5);
  //wf->PrintWaveFormInfo();
}
