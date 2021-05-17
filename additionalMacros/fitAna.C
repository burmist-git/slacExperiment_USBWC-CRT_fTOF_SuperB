Int_t fitAna(){

  /*
  ////////DTL///////
  const Int_t nDTL = 7;
  Double_t xDTL[nDTL]             = { 0.0,  2.0, 4.0, 6.0,
				      8.0, 10.0, 12.0};
  
  Double_t sigNarowDTL_meas[nDTL] = {58.0,  57.0, 70.1, 49.0,
				     48.0,  61.0, 63.0};
  Double_t sigWhidDTL_meas[nDTL]  = {279.0,  301.0, 297.0, 284.0,
				     304.0,  300.0, 281.0};
  Double_t frNarowDTL_meas[nDTL]  = {35.0,  35.0, 39.0, 36.0,
				     34.0,  34.0, 35.0};
  
  TGraph *gr_sigNarowDTL_meas = new TGraph( 7, xDTL, sigNarowDTL_meas);
  gr_sigNarowDTL_meas->SetMarkerStyle(20);
  gr_sigNarowDTL_meas->SetMarkerColor(kBlack);
  gr_sigNarowDTL_meas->SetLineColor(kBlack);
  gr_sigNarowDTL_meas->SetMinimum(0.0);
  gr_sigNarowDTL_meas->SetMaximum(100.0);

  TGraph *gr_sigWhidDTL_meas = new TGraph( 7, xDTL, sigWhidDTL_meas);
  gr_sigWhidDTL_meas->SetMarkerStyle(20);
  gr_sigWhidDTL_meas->SetMarkerColor(kBlack);
  gr_sigWhidDTL_meas->SetLineColor(kBlack);
  gr_sigWhidDTL_meas->SetMinimum(0.0);
  gr_sigWhidDTL_meas->SetMaximum(600.0);

  TGraph *gr_frNarowDTL_meas = new TGraph( 7, xDTL, frNarowDTL_meas);
  gr_frNarowDTL_meas->SetMarkerStyle(20);
  gr_frNarowDTL_meas->SetMarkerColor(kBlack);
  gr_frNarowDTL_meas->SetLineColor(kBlack);
  gr_frNarowDTL_meas->SetMinimum(0.0);
  gr_frNarowDTL_meas->SetMaximum(100.0);


  Double_t sigNarowDTL_Sim[nDTL]   = {69.0,  75.0, 81.0, 63.0,
				      67.0,  83.0, 80.0};
  Double_t sigNarowDTL_SimWf[nDTL] = {79.0,  87.0, 96.0, 73.0,
				      74.0,  96.0, 99.0};
  Double_t sigWhidDTL_Sim[nDTL]    = {434.0,  378.0, 410.0, 425.0,
				      413.0,  375.0, 383.0};
  Double_t sigWhidDTL_SimWf[nDTL]  = {424.0,  371.0, 408.0, 420.0,
				      409.0,  377.0, 398.0};
  Double_t frNarowDTL_Sim[nDTL]    = {31.0,  36.0, 40.0, 26.0,
				      30.0,  40.0, 41.0};
  Double_t frNarowDTL_SimWf[nDTL]  = {27.0,  32.0, 36.0, 24.0,
				      25.0,  36.0, 41.0};
  
  TGraph *gr_sigNarowDTL_Sim = new TGraph(7,xDTL,sigNarowDTL_Sim);
  gr_sigNarowDTL_Sim->SetMarkerStyle(21);
  gr_sigNarowDTL_Sim->SetMarkerColor(kRed);
  gr_sigNarowDTL_Sim->SetLineColor(kRed);
  gr_sigNarowDTL_Sim->SetMinimum(0.0);
  gr_sigNarowDTL_Sim->SetMaximum(100.0);
  TGraph *gr_sigNarowDTL_SimWf = new TGraph(7,xDTL,sigNarowDTL_SimWf);
  gr_sigNarowDTL_SimWf->SetMarkerStyle(21);
  gr_sigNarowDTL_SimWf->SetMarkerColor(kBlue);
  gr_sigNarowDTL_SimWf->SetLineColor(kBlue);
  gr_sigNarowDTL_SimWf->SetMinimum(0.0);
  gr_sigNarowDTL_SimWf->SetMaximum(100.0);
  TGraph *gr_sigWhidDTL_Sim = new TGraph(7,xDTL,sigWhidDTL_Sim);
  gr_sigWhidDTL_Sim->SetMarkerStyle(21);
  gr_sigWhidDTL_Sim->SetMarkerColor(kRed);
  gr_sigWhidDTL_Sim->SetLineColor(kRed);
  gr_sigWhidDTL_Sim->SetMinimum(0.0);
  gr_sigWhidDTL_Sim->SetMaximum(600.0);
  TGraph *gr_sigWhidDTL_SimWf = new TGraph(7,xDTL,sigWhidDTL_SimWf);
  gr_sigWhidDTL_SimWf->SetMarkerStyle(21);
  gr_sigWhidDTL_SimWf->SetMarkerColor(kBlue);
  gr_sigWhidDTL_SimWf->SetLineColor(kBlue);
  gr_sigWhidDTL_SimWf->SetMinimum(0.0);
  gr_sigWhidDTL_SimWf->SetMaximum(600.0);
  TGraph *gr_frNarowDTL_Sim = new TGraph(7,xDTL,frNarowDTL_Sim);
  gr_frNarowDTL_Sim->SetMarkerStyle(21);
  gr_frNarowDTL_Sim->SetMarkerColor(kRed);
  gr_frNarowDTL_Sim->SetLineColor(kRed);
  gr_frNarowDTL_Sim->SetMinimum(0.0);
  gr_frNarowDTL_Sim->SetMaximum(100.0);
  TGraph *gr_frNarowDTL_SimWf = new TGraph(7,xDTL,frNarowDTL_SimWf);
  gr_frNarowDTL_SimWf->SetMarkerStyle(21);
  gr_frNarowDTL_SimWf->SetMarkerColor(kBlue);
  gr_frNarowDTL_SimWf->SetLineColor(kBlue);
  gr_frNarowDTL_SimWf->SetMinimum(0.0);
  gr_frNarowDTL_SimWf->SetMaximum(100.0);


  //////////////////

  TCanvas *c1 = new TCanvas("c1","dT...",10,10,1000,800);
  c1->SetFillColor(kWhite);

  TMultiGraph *mg = new TMultiGraph("mg", "dT");
  
  //mg->Add(gr_sigNarowDTL_meas);
  //mg->Add(gr_sigNarowDTL_Sim);
  //mg->Add(gr_sigNarowDTL_SimWf);
  //mg->SetMinimum(0.0);
  //mg->SetMaximum(100.0);
  //mg->Draw("APL");

  //mg->Add(gr_sigWhidDTL_meas);
  //mg->Add(gr_sigWhidDTL_Sim);
  //mg->Add(gr_sigWhidDTL_SimWf);
  //mg->SetMinimum(0.0);
  //mg->SetMaximum(600.0);
  //mg->Draw("APL");

  mg->Add(gr_frNarowDTL_meas);
  mg->Add(gr_frNarowDTL_Sim);
  mg->Add(gr_frNarowDTL_SimWf);
  mg->SetMinimum(0.0);
  mg->SetMaximum(100.0);
  mg->Draw("APL");

  TLegend *leg = new TLegend(8.0,50.0, 12.0,100.0, "","br");
  leg->AddEntry(gr_frNarowDTL_meas,"meas","p");
  leg->AddEntry(gr_frNarowDTL_Sim,"sim","p");
  leg->AddEntry(gr_frNarowDTL_SimWf,"sim+wf","p");
  leg->SetFillColor(kWhite);
  leg->Draw("same");
  */

  /*
  ////////DTL2///////
  const Int_t nDTL2 = 6;
  Double_t xDTL2[nDTL2]             = { 1.0,   3.0,  5.0,
				       9.0,  11.0, 13.0};
  Double_t sigNarowDTL2_meas[nDTL2] = {65.0,  66.0, 87.0,
				      62.0,  81.0, 64.0};
  Double_t sigWhidDTL2_meas[nDTL2]  = {329.0,  311.0, 339.0,
				      327.0,  331.0, 305.0};
  Double_t frNarowDTL2_meas[nDTL2]  = {40.0,  39.0, 45.0,
				      35.0,  40.0, 38.0};

  TGraph *gr_sigNarowDTL2_meas = new TGraph(nDTL2,xDTL2,sigNarowDTL2_meas);
  gr_sigNarowDTL2_meas->SetMarkerStyle(20);
  gr_sigNarowDTL2_meas->SetMarkerColor(kBlack);
  gr_sigNarowDTL2_meas->SetLineColor(kBlack);
  gr_sigNarowDTL2_meas->SetMinimum(0.0);
  gr_sigNarowDTL2_meas->SetMaximum(130.0);

  TGraph *gr_sigWhidDTL2_meas = new TGraph(nDTL2,xDTL2,sigWhidDTL2_meas);
  gr_sigWhidDTL2_meas->SetMarkerStyle(20);
  gr_sigWhidDTL2_meas->SetMarkerColor(kBlack);
  gr_sigWhidDTL2_meas->SetLineColor(kBlack);
  gr_sigWhidDTL2_meas->SetMinimum(0.0);
  gr_sigWhidDTL2_meas->SetMaximum(600.0);

  TGraph *gr_frNarowDTL2_meas = new TGraph(nDTL2,xDTL2,frNarowDTL2_meas);
  gr_frNarowDTL2_meas->SetMarkerStyle(20);
  gr_frNarowDTL2_meas->SetMarkerColor(kBlack);
  gr_frNarowDTL2_meas->SetLineColor(kBlack);
  gr_frNarowDTL2_meas->SetMinimum(0.0);
  gr_frNarowDTL2_meas->SetMaximum(100.0);

  
  Double_t sigNarowDTL2_Sim[nDTL2]   = {72.0,  94.0, 73.0,
					72.0, 94.0, 80.0};
  Double_t sigNarowDTL2_SimWf[nDTL2] = {77.0,  110.0, 81.0,
					97.0, 118.0, 85.0};
  Double_t sigWhidDTL2_Sim[nDTL2]    = {399.0,  447.0, 386.0,
					377.0,  415.0, 382.0};
  Double_t sigWhidDTL2_SimWf[nDTL2]  = {382.0,  426.0, 384.0,
					408.0,  440.0, 378.0};
  Double_t frNarowDTL2_Sim[nDTL2]    = {39.0,  45.0, 39.0,
					39.0,  43.0, 40.0};
  Double_t frNarowDTL2_SimWf[nDTL2]  = {33.0,  41.0, 34.0,
					40.0,  45.0, 36.0};

  TGraph *gr_sigNarowDTL2_Sim = new TGraph(nDTL2,xDTL2,sigNarowDTL2_Sim);
  gr_sigNarowDTL2_Sim->SetMarkerStyle(21);
  gr_sigNarowDTL2_Sim->SetMarkerColor(kRed);
  gr_sigNarowDTL2_Sim->SetLineColor(kRed);
  gr_sigNarowDTL2_Sim->SetMinimum(0.0);
  gr_sigNarowDTL2_Sim->SetMaximum(130.0);
  TGraph *gr_sigNarowDTL2_SimWf = new TGraph(nDTL2,xDTL2,sigNarowDTL2_SimWf);
  gr_sigNarowDTL2_SimWf->SetMarkerStyle(21);
  gr_sigNarowDTL2_SimWf->SetMarkerColor(kBlue);
  gr_sigNarowDTL2_SimWf->SetLineColor(kBlue);
  gr_sigNarowDTL2_SimWf->SetMinimum(0.0);
  gr_sigNarowDTL2_SimWf->SetMaximum(130.0);
  TGraph *gr_sigWhidDTL2_Sim = new TGraph(nDTL2,xDTL2,sigWhidDTL2_Sim);
  gr_sigWhidDTL2_Sim->SetMarkerStyle(21);
  gr_sigWhidDTL2_Sim->SetMarkerColor(kRed);
  gr_sigWhidDTL2_Sim->SetLineColor(kRed);
  gr_sigWhidDTL2_Sim->SetMinimum(0.0);
  gr_sigWhidDTL2_Sim->SetMaximum(600.0);
  TGraph *gr_sigWhidDTL2_SimWf = new TGraph(nDTL2,xDTL2,sigWhidDTL2_SimWf);
  gr_sigWhidDTL2_SimWf->SetMarkerStyle(21);
  gr_sigWhidDTL2_SimWf->SetMarkerColor(kBlue);
  gr_sigWhidDTL2_SimWf->SetLineColor(kBlue);
  gr_sigWhidDTL2_SimWf->SetMinimum(0.0);
  gr_sigWhidDTL2_SimWf->SetMaximum(600.0);
  TGraph *gr_frNarowDTL2_Sim = new TGraph(nDTL2,xDTL2,frNarowDTL2_Sim);
  gr_frNarowDTL2_Sim->SetMarkerStyle(21);
  gr_frNarowDTL2_Sim->SetMarkerColor(kRed);
  gr_frNarowDTL2_Sim->SetLineColor(kRed);
  gr_frNarowDTL2_Sim->SetMinimum(0.0);
  gr_frNarowDTL2_Sim->SetMaximum(100.0);
  TGraph *gr_frNarowDTL2_SimWf = new TGraph(nDTL2,xDTL2,frNarowDTL2_SimWf);
  gr_frNarowDTL2_SimWf->SetMarkerStyle(21);
  gr_frNarowDTL2_SimWf->SetMarkerColor(kBlue);
  gr_frNarowDTL2_SimWf->SetLineColor(kBlue);
  gr_frNarowDTL2_SimWf->SetMinimum(0.0);
  gr_frNarowDTL2_SimWf->SetMaximum(100.0);


  TCanvas *c1 = new TCanvas("c1","dT...",10,10,1000,800);
  c1->SetFillColor(kWhite);

  TMultiGraph *mg = new TMultiGraph("mg", "DTL2");
  
  mg->Add(gr_sigNarowDTL2_meas);
  mg->Add(gr_sigNarowDTL2_Sim);
  mg->Add(gr_sigNarowDTL2_SimWf);
  mg->SetMinimum(0.0);
  mg->SetMaximum(130.0);
  mg->Draw("APL");

  //mg->Add(gr_sigWhidDTL2_meas);
  //mg->Add(gr_sigWhidDTL2_Sim);
  //mg->Add(gr_sigWhidDTL2_SimWf);
  //mg->SetMinimum(0.0);
  //mg->SetMaximum(600.0);
  //mg->Draw("APL");


  //mg->Add(gr_frNarowDTL2_meas);
  //mg->Add(gr_frNarowDTL2_Sim);
  //mg->Add(gr_frNarowDTL2_SimWf);
  //mg->SetMinimum(0.0);
  //mg->SetMaximum(100.0);
  //mg->Draw("APL");


  TLegend *leg = new TLegend(8.0,50.0, 12.0,100.0, "","br");
  leg->AddEntry(gr_frNarowDTL2_meas,"meas","p");
  leg->AddEntry(gr_frNarowDTL2_Sim,"sim","p");
  leg->AddEntry(gr_frNarowDTL2_SimWf,"sim+wf","p");
  leg->SetFillColor(kWhite);
  leg->Draw("same");
  ////////////////////////////
*/


  ////////////DTL3///////////
  const Int_t nDTL3 = 4;
  Double_t xDTL3[nDTL3]             = { 2.0,   3.0,
					10.0,  11.0};

  //All USBWC events
  //Double_t sigNarowDTL3_meas[nDTL3] = {112.0, 128.0,
  //			       114.0, 124.0};
  //Double_t sigWhidDTL3_meas[nDTL3]  = {421.0, 403.0,
  //			       416.0, 416.0};
  //Double_t frNarowDTL3_meas[nDTL3]  = {36.0, 42.0,
  //			       29.0, 33.0};

  //merged USBWC events diff ampl cuts
  //Double_t sigNarowDTL3_meas[nDTL3] = {126.0, 103.0,
  //			       133.0, 124.0};
  //merged USBWC events diff ampl cuts + nSig chanels (ampl > 100mV) < 5
  Double_t sigNarowDTL3_meas[nDTL3] = {140.0, 101.0,
				       123.0, 101.0};
  Double_t sigWhidDTL3_meas[nDTL3]  = {633.0, 427.0,
  			       495.0, 398.0};
  Double_t frNarowDTL3_meas[nDTL3]  = {57.0, 44.0,
  			       37.0, 38.0};

  //merged USBWC events diff ampl cuts

  
  //all ampl cuts = 100mV cuts on theta and phi of the track
  //Double_t sigNarowDTL3_meas[nDTL3] = {0.0, 0.0,
  //				       0.0, 0.0};
  //Double_t sigWhidDTL3_meas[nDTL3]  = {0.0, 0.0,
  //			       0.0, 0.0};
  //Double_t frNarowDTL3_meas[nDTL3]  = {0.0, 0.0,
  //			       0.0, 0.0};
  
  TGraph *gr_sigNarowDTL3_meas = new TGraph(nDTL3,xDTL3,sigNarowDTL3_meas);
  gr_sigNarowDTL3_meas->SetMarkerStyle(20);
  gr_sigNarowDTL3_meas->SetMarkerColor(kBlack);
  gr_sigNarowDTL3_meas->SetLineColor(kBlack);
  gr_sigNarowDTL3_meas->SetMinimum(0.0);
  gr_sigNarowDTL3_meas->SetMaximum(150.0);
  TGraph *gr_sigWhidDTL3_meas = new TGraph(nDTL3,xDTL3,sigWhidDTL3_meas);
  gr_sigWhidDTL3_meas->SetMarkerStyle(20);
  gr_sigWhidDTL3_meas->SetMarkerColor(kBlack);
  gr_sigWhidDTL3_meas->SetLineColor(kBlack);
  gr_sigWhidDTL3_meas->SetMinimum(0.0);
  gr_sigWhidDTL3_meas->SetMaximum(600.0);
  TGraph *gr_frNarowDTL3_meas = new TGraph(nDTL3,xDTL3,frNarowDTL3_meas);
  gr_frNarowDTL3_meas->SetMarkerStyle(20);
  gr_frNarowDTL3_meas->SetMarkerColor(kBlack);
  gr_frNarowDTL3_meas->SetLineColor(kBlack);
  gr_frNarowDTL3_meas->SetMinimum(0.0);
  gr_frNarowDTL3_meas->SetMaximum(100.0);

  Double_t sigNarowDTL3_Sim[nDTL3]   = {93.0,  95.0,
					87.0,  98.0};
  Double_t sigNarowDTL3_SimWf[nDTL3] = {112.0,  106.0,
					103.0,  102.0};
  Double_t sigWhidDTL3_Sim[nDTL3]    = {414.0,  417.0,
					395.0,  423.0};
  Double_t sigWhidDTL3_SimWf[nDTL3]  = {435.0,  402.0,
					399.0,  392.0};
  Double_t frNarowDTL3_Sim[nDTL3]    = {41.0,  42.0,
					40.0,  43.0};
  Double_t frNarowDTL3_SimWf[nDTL3]  = {41.0,  37.0,
					39.0,  37.0};

  TGraph *gr_sigNarowDTL3_Sim = new TGraph(nDTL3,xDTL3,sigNarowDTL3_Sim);
  gr_sigNarowDTL3_Sim->SetMarkerStyle(21);
  gr_sigNarowDTL3_Sim->SetMarkerColor(kRed);
  gr_sigNarowDTL3_Sim->SetLineColor(kRed);
  gr_sigNarowDTL3_Sim->SetMinimum(0.0);
  gr_sigNarowDTL3_Sim->SetMaximum(130.0);
  TGraph *gr_sigNarowDTL3_SimWf = new TGraph(nDTL3,xDTL3,sigNarowDTL3_SimWf);
  gr_sigNarowDTL3_SimWf->SetMarkerStyle(21);
  gr_sigNarowDTL3_SimWf->SetMarkerColor(kBlue);
  gr_sigNarowDTL3_SimWf->SetLineColor(kBlue);
  gr_sigNarowDTL3_SimWf->SetMinimum(0.0);
  gr_sigNarowDTL3_SimWf->SetMaximum(130.0);
  TGraph *gr_sigWhidDTL3_Sim = new TGraph(nDTL3,xDTL3,sigWhidDTL3_Sim);
  gr_sigWhidDTL3_Sim->SetMarkerStyle(21);
  gr_sigWhidDTL3_Sim->SetMarkerColor(kRed);
  gr_sigWhidDTL3_Sim->SetLineColor(kRed);
  gr_sigWhidDTL3_Sim->SetMinimum(0.0);
  gr_sigWhidDTL3_Sim->SetMaximum(600.0);
  TGraph *gr_sigWhidDTL3_SimWf = new TGraph(nDTL3,xDTL3,sigWhidDTL3_SimWf);
  gr_sigWhidDTL3_SimWf->SetMarkerStyle(21);
  gr_sigWhidDTL3_SimWf->SetMarkerColor(kBlue);
  gr_sigWhidDTL3_SimWf->SetLineColor(kBlue);
  gr_sigWhidDTL3_SimWf->SetMinimum(0.0);
  gr_sigWhidDTL3_SimWf->SetMaximum(600.0);
  TGraph *gr_frNarowDTL3_Sim = new TGraph(nDTL3,xDTL3,frNarowDTL3_Sim);
  gr_frNarowDTL3_Sim->SetMarkerStyle(21);
  gr_frNarowDTL3_Sim->SetMarkerColor(kRed);
  gr_frNarowDTL3_Sim->SetLineColor(kRed);
  gr_frNarowDTL3_Sim->SetMinimum(0.0);
  gr_frNarowDTL3_Sim->SetMaximum(130.0);
  TGraph *gr_frNarowDTL3_SimWf = new TGraph(nDTL3,xDTL3,frNarowDTL3_SimWf);
  gr_frNarowDTL3_SimWf->SetMarkerStyle(21);
  gr_frNarowDTL3_SimWf->SetMarkerColor(kBlue);
  gr_frNarowDTL3_SimWf->SetLineColor(kBlue);
  gr_frNarowDTL3_SimWf->SetMinimum(0.0);
  gr_frNarowDTL3_SimWf->SetMaximum(130.0);


  TCanvas *c1 = new TCanvas("c1","dT...",10,10,1000,800);
  c1->SetFillColor(kWhite);

  TMultiGraph *mg = new TMultiGraph("mg", "dT");
  
  mg->Add(gr_sigNarowDTL3_meas);
  mg->Add(gr_sigNarowDTL3_Sim);
  mg->Add(gr_sigNarowDTL3_SimWf);
  mg->SetMinimum(0.0);
  mg->SetMaximum(150.0);
  mg->Draw("APL");

  //mg->Add(gr_sigWhidDTL3_meas);
  //mg->Add(gr_sigWhidDTL3_Sim);
  //mg->Add(gr_sigWhidDTL3_SimWf);
  //mg->SetMinimum(0.0);
  //mg->SetMaximum(650.0);
  //mg->Draw("APL");

  //mg->Add(gr_frNarowDTL3_meas);
  //mg->Add(gr_frNarowDTL3_Sim);
  //mg->Add(gr_frNarowDTL3_SimWf);
  //mg->SetMinimum(0.0);
  //mg->SetMaximum(100.0);
  //mg->Draw("APL");

  TLegend *leg = new TLegend(8.0,50.0, 12.0,100.0, "","br");
  leg->AddEntry(gr_frNarowDTL3_meas,"meas","p");
  leg->AddEntry(gr_frNarowDTL3_Sim,"sim","p");
  leg->AddEntry(gr_frNarowDTL3_SimWf,"sim+wf","p");
  leg->SetFillColor(kWhite);
  leg->Draw("same");
  ///////////////////////////


  /*
  ////////////DTL4///////////
  const Int_t nDTL4 = 6;
  Double_t xDTL4[nDTL4]             = { 1.0,   2.0, 3.0, 4.0,
					5.0,  6.0};
  Double_t sigNarowDTL4_meas[nDTL4] = {138.0, 143.0, 165.0, 131.0,
				       129.0, 138.0};
  Double_t sigWhidDTL4_meas[nDTL4]  = {437.0, 525.0, 620.0, 451.0,
				       435.0, 509.0};
  Double_t frNarowDTL4_meas[nDTL4]  = {33.0, 38.0, 49.0, 36.0,
				       33.0, 32.0};
  
  TGraph *gr_sigNarowDTL4_meas = new TGraph(nDTL4,xDTL4,sigNarowDTL4_meas);
  gr_sigNarowDTL4_meas->SetMarkerStyle(20);
  gr_sigNarowDTL4_meas->SetMarkerColor(kBlack);
  gr_sigNarowDTL4_meas->SetLineColor(kBlack);
  gr_sigNarowDTL4_meas->SetMinimum(0.0);
  gr_sigNarowDTL4_meas->SetMaximum(200.0);
  TGraph *gr_sigWhidDTL4_meas = new TGraph(nDTL4,xDTL4,sigWhidDTL4_meas);
  gr_sigWhidDTL4_meas->SetMarkerStyle(20);
  gr_sigWhidDTL4_meas->SetMarkerColor(kBlack);
  gr_sigWhidDTL4_meas->SetLineColor(kBlack);
  gr_sigWhidDTL4_meas->SetMinimum(0.0);
  gr_sigWhidDTL4_meas->SetMaximum(700.0);
  TGraph *gr_frNarowDTL4_meas = new TGraph(nDTL4,xDTL4,frNarowDTL4_meas);
  gr_frNarowDTL4_meas->SetMarkerStyle(20);
  gr_frNarowDTL4_meas->SetMarkerColor(kBlack);
  gr_frNarowDTL4_meas->SetLineColor(kBlack);
  gr_frNarowDTL4_meas->SetMinimum(0.0);
  gr_frNarowDTL4_meas->SetMaximum(100.0);



  Double_t sigNarowDTL4_Sim[nDTL4]   = {115.0,  109.0, 103.0, 97.0,
					113.0,  123.0};
  Double_t sigNarowDTL4_SimWf[nDTL4] = {133.0,  121.0, 124.0, 116.0,
					123.0,  141.0};
  Double_t sigWhidDTL4_Sim[nDTL4]    = {452.0,  472.0, 463.0, 428.0,
					456.0,  499.0};
  Double_t sigWhidDTL4_SimWf[nDTL4]  = {471.0,  490.0, 480.0, 427.0,
					426.0,  565.0};
  Double_t frNarowDTL4_Sim[nDTL4]    = { 46.0,   44.0,  45.0, 40.0,
					 46.0,   51.0};
  Double_t frNarowDTL4_SimWf[nDTL4]  = {46.0,  43.0, 46.0, 39.0,
					42.0,  54.0};



  TGraph *gr_sigNarowDTL4_Sim = new TGraph(nDTL4,xDTL4,sigNarowDTL4_Sim);
  gr_sigNarowDTL4_Sim->SetMarkerStyle(21);
  gr_sigNarowDTL4_Sim->SetMarkerColor(kRed);
  gr_sigNarowDTL4_Sim->SetLineColor(kRed);
  gr_sigNarowDTL4_Sim->SetMinimum(0.0);
  gr_sigNarowDTL4_Sim->SetMaximum(150.0);
  TGraph *gr_sigNarowDTL4_SimWf = new TGraph(nDTL4,xDTL4,sigNarowDTL4_SimWf);
  gr_sigNarowDTL4_SimWf->SetMarkerStyle(21);
  gr_sigNarowDTL4_SimWf->SetMarkerColor(kBlue);
  gr_sigNarowDTL4_SimWf->SetLineColor(kBlue);
  gr_sigNarowDTL4_SimWf->SetMinimum(0.0);
  gr_sigNarowDTL4_SimWf->SetMaximum(150.0);
  TGraph *gr_sigWhidDTL4_Sim = new TGraph(nDTL4,xDTL4,sigWhidDTL4_Sim);
  gr_sigWhidDTL4_Sim->SetMarkerStyle(21);
  gr_sigWhidDTL4_Sim->SetMarkerColor(kRed);
  gr_sigWhidDTL4_Sim->SetLineColor(kRed);
  gr_sigWhidDTL4_Sim->SetMinimum(0.0);
  gr_sigWhidDTL4_Sim->SetMaximum(600.0);
  TGraph *gr_sigWhidDTL4_SimWf = new TGraph(nDTL4,xDTL4,sigWhidDTL4_SimWf);
  gr_sigWhidDTL4_SimWf->SetMarkerStyle(21);
  gr_sigWhidDTL4_SimWf->SetMarkerColor(kBlue);
  gr_sigWhidDTL4_SimWf->SetLineColor(kBlue);
  gr_sigWhidDTL4_SimWf->SetMinimum(0.0);
  gr_sigWhidDTL4_SimWf->SetMaximum(600.0);
  TGraph *gr_frNarowDTL4_Sim = new TGraph(nDTL4,xDTL4,frNarowDTL4_Sim);
  gr_frNarowDTL4_Sim->SetMarkerStyle(21);
  gr_frNarowDTL4_Sim->SetMarkerColor(kRed);
  gr_frNarowDTL4_Sim->SetLineColor(kRed);
  gr_frNarowDTL4_Sim->SetMinimum(0.0);
  gr_frNarowDTL4_Sim->SetMaximum(100.0);
  TGraph *gr_frNarowDTL4_SimWf = new TGraph(nDTL4,xDTL4,frNarowDTL4_SimWf);
  gr_frNarowDTL4_SimWf->SetMarkerStyle(21);
  gr_frNarowDTL4_SimWf->SetMarkerColor(kBlue);
  gr_frNarowDTL4_SimWf->SetLineColor(kBlue);
  gr_frNarowDTL4_SimWf->SetMinimum(0.0);
  gr_frNarowDTL4_SimWf->SetMaximum(100.0);


  TCanvas *c1 = new TCanvas("c1","dT...",10,10,1000,800);
  c1->SetFillColor(kWhite);

  TMultiGraph *mg = new TMultiGraph("mg", "dT");
  
  //mg->Add(gr_sigNarowDTL4_meas);
  //mg->Add(gr_sigNarowDTL4_Sim);
  //mg->Add(gr_sigNarowDTL4_SimWf);
  //mg->SetMinimum(0.0);
  //mg->SetMaximum(200.0);
  //mg->Draw("APL");

  //mg->Add(gr_sigWhidDTL4_meas);
  //mg->Add(gr_sigWhidDTL4_Sim);
  //mg->Add(gr_sigWhidDTL4_SimWf);
  //mg->SetMinimum(0.0);
  //mg->SetMaximum(650.0);
  //mg->Draw("APL");

  mg->Add(gr_frNarowDTL4_meas);
  mg->Add(gr_frNarowDTL4_Sim);
  mg->Add(gr_frNarowDTL4_SimWf);
  mg->SetMinimum(0.0);
  mg->SetMaximum(100.0);
  mg->Draw("APL");

  TLegend *leg = new TLegend(0.0,50.0, 3.0,100.0, "","br");
  leg->AddEntry(gr_frNarowDTL4_meas,"meas","p");
  leg->AddEntry(gr_frNarowDTL4_Sim,"sim","p");
  leg->AddEntry(gr_frNarowDTL4_SimWf,"sim+wf","p");
  leg->SetFillColor(kWhite);
  leg->Draw("same");

  //////////////////////////////
*/

  return 0;
}
