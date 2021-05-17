Int_t fit2GaussNN(){

  Int_t i;

  TString dtID;

  dtID = "L";

  //////////////////////
  if(dtID=="L"){
    const Int_t n = 7;
    Double_t x[n] = { 0.0,  2.0, 4.0, 6.0, 8.0, 10.0, 12.0};
    Double_t sigN[n];
    Double_t sigW[n];
    Double_t frN[n];
    Double_t frW[n];
  }
  else if(dtID=="L2"){
    const Int_t n = 6;
    Double_t x[n] = { 1.0, 3.0, 5.0, 9.0,  11.0, 13.0};
    Double_t sigN[n];
    Double_t sigW[n];
    Double_t frN[n];
    Double_t frW[n];
  }
  else if(dtID=="L3"){
    const Int_t n = 4;
    Double_t x[n] = { 2.0, 3.0, 10.0,  11.0};
    Double_t sigN[n];
    Double_t sigW[n];
    Double_t frN[n];
    Double_t frW[n];
  }
  else if(dtID=="L4"){
    const Int_t n = 6;
    Double_t x[n] = { 1.0, 2.0, 3.0, 4.0, 5.0,  6.0};
    Double_t sigN[n];
    Double_t sigW[n];
    Double_t frN[n];
    Double_t frW[n];
  }
  else if(dtID=="TtB"){
    const Int_t n = 7;
    Double_t x[n] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    Double_t sigN[n];
    Double_t sigW[n];
    Double_t frN[n];
    Double_t frW[n];
  }
  //////////////////////

  for(i = 0;i<n;i++){
    cout<<endl<<endl
	<<"  ****  //// 2 gauss FIT "<<dtID<<" \\\\\ ***** "<<endl;

    Int_t ddttIDID = (Int_t)x[i];

    // ALL //
    Double_t *par = fit2GaussN( ddttIDID, "../../Histos/3/Histos_merged_RUN4.root",dtID);
    sigN[i] = TMath::Abs(par[1]);
    sigW[i] = TMath::Abs(par[0]);
    frN[i]  = TMath::Abs(par[3]);
    frW[i]  = TMath::Abs(par[2]);

    //par = fit2GaussN( ddttIDID, "../../Histos/3/Histos_merged_RUN4_5nSigCut.root",dtID);
    //par = fit2GaussN( ddttIDID, "../../Histos/3/Histos_merged_RUN4_recotrk.root",dtID);
    //par = fit2GaussN( ddttIDID, "../../Histos/3/Histos_merged_RUN4_recotrk_kinemcut.root",dtID);

    cout<<"/////////////"<<endl
	<<"* i        = "<<i<<endl
	<<"* sigN  = "<<sigN[i]<<endl
	<<"* sigW  = "<<sigW[i]<<endl
	<<"* frN   = "<<frN[i]<<endl
	<<"* frW   = "<<frW[i]<<endl
	<<"/////////////"<<endl<<endl;

    cout<<" **** //// ||||||||||||| \\\\\ ***** "<<endl;
  }

  //     //
  TGraph *gr_sigN = new TGraph(n,x,sigN);
  gr_sigN->SetMarkerStyle(21);
  gr_sigN->SetMarkerColor(kRed);
  gr_sigN->SetLineColor(kRed);
  gr_sigN->SetMinimum(0.0);
  gr_sigN->SetMaximum(130.0);
  TGraph *gr_sigW = new TGraph(n,x,sigW);
  gr_sigW->SetMarkerStyle(21);
  gr_sigW->SetMarkerColor(kRed);
  gr_sigW->SetLineColor(kRed);
  gr_sigW->SetMinimum(0.0);
  gr_sigW->SetMaximum(600.0);
  TGraph *gr_frN = new TGraph(n,x,frN);
  gr_frN->SetMarkerStyle(21);
  gr_frN->SetMarkerColor(kRed);
  gr_frN->SetLineColor(kRed);
  gr_frN->SetMinimum(0.0);
  gr_frN->SetMaximum(100.0);


  TCanvas *c1_sigN = new TCanvas("c1_sigN"," sigN ",10,10,1000,800);
  c1_sigN->SetFillColor(kWhite);
  TMultiGraph *mg_sigN = new TMultiGraph("mg_sigN", "sigN");
  mg_sigN->Add(gr_sigN);
  mg_sigN->SetMinimum(0.0);
  //mg->SetMaximum(150.0);
  mg_sigN->Draw("AP");


  TCanvas *c1_sigW = new TCanvas("c1_sigW"," sigW ", 20, 20, 1000, 800);
  c1_sigW->SetFillColor(kWhite);
  TMultiGraph *mg_sigW = new TMultiGraph("mg_sigW", "sigW");
  mg_sigW->Add(gr_sigW);
  mg_sigW->SetMinimum(0.0);
  //mg->SetMaximum(150.0);
  mg_sigW->Draw("AP");


  TCanvas *c1_frN = new TCanvas("c1_frN"," frN ", 30, 30, 1000, 800);
  c1_frN->SetFillColor(kWhite);
  TMultiGraph *mg_frN = new TMultiGraph("mg_frN", "frN");
  mg_frN->Add(gr_frN);
  mg_frN->SetMinimum(0.0);
  //mg->SetMaximum(150.0);
  mg_frN->Draw("AP");


  return 0;
}


Double_t *fit2GaussN(Int_t chID, TString file1, TString dTID){

  Double_t meanL[16] = { 0.12, 0.0,  0.28, 0.0,
			 0.01, 0.0,  0.02, 0.0,
			 -0.02, 0.0, -0.03, 0.0,
			 -0.03, 0.0,   0.0, 0.0};
  
  Double_t meanL2[16] = { 0.0, -0.26,   0.0, -0.15,
			  0.0, -0.68,   0.0,  0.0,
			  0.0,  0.24,   0.0, -0.8,
			  0.0, -0.03,   0.0,  0.0};
  
  Double_t meanL3[16] = { 0.0,  0.0,   0.2, -0.08,
			  0.0,  0.0,   0.0,  0.0,
			  0.0,  0.0, -0.84, -0.83,
			  0.0,  0.0,   0.0,  0.0};
  
  Double_t meanL4[16] = { 0.0, -0.79,  -0.45, -0.71,
			  0.2,   0.2,    0.6,   0.5,
			  0.0,   0.0,    0.0,   0.0,
			  0.0,   0.0,    0.0,   0.0};
  
  Double_t meanTtB[16] = { 0.1,  -0.1,  0.45,  0.1,
			   -0.6, -0.66, -0.05,  0.0,
			   0.0,   0.0,   0.0,  0.0,
			   0.0,   0.0,   0.0,  0.0};
  
  Double_t x1Min;
  Double_t x1Max;
  Double_t x2Min;
  Double_t x2Max;
  Double_t xMin;
  Double_t xMax;
  Int_t nRebin = 0;
  Double_t wideR;
  Double_t narrR;

  //Int_t chID;
  TString epsName;
  
  /////////////////////////////////////////////
  //file1 = "../../Histos/2/Histos_merged_RUN4_4hnSecBin_02maxdTcut.root";
  //file1 = "../../Histos/2/Histos_merged_RUN4_4hnSecBin_02maxdTcut_trkCut.root";
  //file1 = "../../Histos/2/Histos_merged_RUN4_KinematicCut.root";
  //file1 = "../../Histos/2/Histos_merged_RUN4_KinematicCut_nAmplCut.root";
  
  //file1 = "../../Histos/2/Histos_merged_RUN4_CRTMERG.root";
  //file1 = "../../Histos/2/Histos_merged_RUN4_CRTMERG_4nAmpl.root";
  //file1 = "../../Histos/2/Histos_merged_RUN4_4nAmpl.root";
  //file1 = "../../Histos/2/Histos_merged_RUN4.root";
  //file1 = "../../Histos/2/Histos_merged_RUN4_NOCUTS.root";

  //file1 = "../../Histos/3/Histos_merged_RUN4_trkCut.root";


  if(dTID == "L"){
    //////TIMEDIFF L//////
    wideR = 0.7;
    narrR = 0.3;
    x1Min =  meanL[chID]-wideR;
    x1Max =  meanL[chID]+wideR;
    x2Min =  meanL[chID]-narrR;
    x2Max =  meanL[chID]+narrR;
    xMin  =  meanL[chID]-wideR;
    xMax  =  meanL[chID]+wideR;
    nRebin = 0;
    ///------------------
  }

  if(dTID == "L2"){
    //////TIMEDIFF L2//////
    wideR = 0.7;
    narrR = 0.3;
    x1Min =  meanL2[chID]-wideR;
    x1Max =  meanL2[chID]+wideR;
    x2Min =  meanL2[chID]-narrR;
    x2Max =  meanL2[chID]+narrR;
    xMin  =  meanL2[chID]-wideR;
    xMax  =  meanL2[chID]+wideR;
    nRebin = 0;
    ///--------------------
  }

  if(dTID == "L3"){
    //////TIMEDIFF L3//////
    wideR = 0.7;
    narrR = 0.3;
    x1Min =  meanL3[chID]-wideR;
    x1Max =  meanL3[chID]+wideR;
    x2Min =  meanL3[chID]-narrR;
    x2Max =  meanL3[chID]+narrR;
    xMin  =  meanL3[chID]-wideR;
    xMax  =  meanL3[chID]+wideR;
    nRebin = 2;
    ///--------------------
  }

  if(dTID == "L4"){
    //////TIMEDIFF L4//////
    wideR = 0.7;
    narrR = 0.3;
    x1Min =  meanL4[chID]-wideR;
    x1Max =  meanL4[chID]+wideR;
    x2Min =  meanL4[chID]-narrR;
    x2Max =  meanL4[chID]+narrR;
    xMin  =  meanL4[chID]-wideR;
    xMax  =  meanL4[chID]+wideR;
    nRebin = 2;
    ///--------------------
  }
  
  if(dTID == "TtB"){
    //////TIMEDIFF TtB//////
    wideR = 0.7;
    narrR = 0.3;
    x1Min =  meanTtB[chID]-wideR;
    x1Max =  meanTtB[chID]+wideR;
    x2Min =  meanTtB[chID]-narrR;
    x2Max =  meanTtB[chID]+narrR;
    xMin  =  meanTtB[chID]-wideR;
    xMax  =  meanTtB[chID]+wideR;
    nRebin = 2;
    ///--------------------
  }


  /////////////////////////////////////////////

  TFile *f1 = new TFile(file1.Data());
  TString histN;
  if(dTID == "L")
    histN = "h1dTimeL_ch";
  if(dTID == "L2")
    histN = "h1dTimeL2_ch";
  if(dTID == "L3")
    histN = "h1dTimeL3_ch";
  if(dTID == "L4")
    histN = "h1dTimeL4_ch";
  if(dTID == "TtB")
    histN = "h1dTimeTOPtoBOT_ch";
  histN += chID; 
  epsName = histN;
  epsName += ".eps";
  //cout<<histN<<endl;

  cout<<"file1   "<<file1<<endl
      <<"wideR   "<<wideR<<endl
      <<"narrR   "<<narrR<<endl
      <<"meanL   "<<meanL[chID]<<endl
      <<"x1Min   "<<x1Min<<endl
      <<"x1Max   "<<x1Max<<endl
      <<"x2Min   "<<x2Min<<endl
      <<"x2Max   "<<x2Max<<endl
      <<"xMin    "<<xMin<<endl
      <<"xMax    "<<xMax<<endl
      <<"nRebin  "<<nRebin<<endl
      <<"chID    "<<chID<<endl
      <<"histN   "<<histN<<endl
      <<"epsName "<<epsName<<endl;

  TH1D *h1dTime = (TH1D*)f1->Get(histN.Data());

  if(nRebin>0)
    h1dTime->Rebin(nRebin);

  Int_t i = 0;
  const Int_t N = 6;
  Double_t par[N];
  Double_t parFit[N];
  for(i = 0;i<N;i++){
    par[i] = 0.0;
    parFit[i] = 0.0;
  }  



  TCanvas *c1 = new TCanvas("c1","first fit",10,10,600,800);
  
  TF1 *g1 = new TF1("g1","gaus", xMin, xMax);
  TF1 *g1x = new TF1("g1x","gaus", xMin, xMax);
  h1dTime->Fit(g1,"","", x1Min, x1Max);
  g1->GetParameters(&par[0]);
  g1x->SetParameters(par[0],par[1],par[2]);
  TF1 *g2 = new TF1("g2","gaus", xMin, xMax);
  TF1 *g2x = new TF1("g2x","gaus", xMin, xMax);
  h1dTime->Fit(g2,"","", x2Min, x2Max);
  g2->GetParameters(&par[3]);
  g2x->SetParameters(par[3],par[4],par[5]);

  for(i = 0;i<N;i++){
    cout<<"par["<<i<<"]="<<par[i]<<endl;
  }

  h1dTime->Draw("");
  g1x->SetLineStyle(kDashed);
  g2x->SetLineStyle(kDashed);
  g1x->SetLineColor(kGreen);
  g2x->SetLineColor(kMagenta);
  g1x->Draw("same");
  g2x->Draw("same");


  TString c2name = "c2_";
  c2name += chID;
  Int_t x00 = 10;
  Int_t y00 = 10;

  TString c2title = "c2title_";
  c2title += chID;

  TCanvas *c2 = new TCanvas(c2name.Data(),c2title.Data(),20,20,600,800);
  c2->SetFillColor(kWhite);   
  TF1 *g12 = new TF1("g12","gaus", xMin,xMax);
  TF1 *g22 = new TF1("g22","gaus", xMin,xMax);  

  cout<<endl<<endl<<"            FIT           "<<endl;
  TF1 *tripleG = new TF1("tripleG","gaus(0)+gaus(3)");
  tripleG->SetParameters(par); 
  tripleG->SetLineColor(kRed);
  h1dTime->Fit(tripleG,"","", xMin, xMax);
  tripleG->GetParameters(&parFit[0]);
  g12->SetParameters(parFit[0],parFit[1],parFit[2]);
  g22->SetParameters(parFit[3],parFit[4],parFit[5]);
  g12->SetLineStyle(kDashed);
  g22->SetLineStyle(kDashed);
  g12->SetLineColor(kGreen);
  g22->SetLineColor(kMagenta);
  cout<<endl<<endl<<endl;

  if(dTID == "L")
    h1dTime->GetXaxis()->SetRangeUser(meanL[chID]-1.5,meanL[chID]+1.5);
  if(dTID == "L2")
    h1dTime->GetXaxis()->SetRangeUser(meanL2[chID]-1.5,meanL2[chID]+1.5);
  if(dTID == "L3")
    h1dTime->GetXaxis()->SetRangeUser(meanL3[chID]-1.5,meanL3[chID]+1.5);
  if(dTID == "L4")
    h1dTime->GetXaxis()->SetRangeUser(meanL4[chID]-1.5,meanL4[chID]+1.5);
  if(dTID == "TtB")
    h1dTime->GetXaxis()->SetRangeUser(meanTtB[chID]-1.5,meanTtB[chID]+1.5);

  h1dTime->SetLineWidth(2.0);
  h1dTime->Draw("");
  g12->Draw("same");
  g22->Draw("same");

  c2->SaveAs(epsName.Data());

  Double_t *parrrr;
  parrrr = new Double_t[4]; 

  cout<<"Sigma wide   "<<parFit[2]*1000.0<<" ps"<<endl
      <<"Sigma narrow "<<parFit[5]*1000.0<<" ps"<<endl;
  
  cout<<" Integral wide   "<<g12->Integral( xMin, xMax)<<endl
      <<" Integral narrow "<<g22->Integral( xMin, xMax)<<endl;

  if((g12->Integral( xMin, xMax) + g22->Integral( xMin, xMax))!=0.0){
    cout<<" Fraction wide   "<<g12->Integral( xMin, xMax)/(g12->Integral( xMin, xMax) + g22->Integral( xMin, xMax))*100<<" %"<<endl
	<<" Fraction narrow "<<g22->Integral( xMin, xMax)/(g12->Integral( xMin, xMax) + g22->Integral( xMin, xMax))*100<<" %"<<endl;
  }  

  parrrr[0] = parFit[2]*1000.0;
  parrrr[1] = parFit[5]*1000.0;
  parrrr[2] = (g12->Integral( xMin, xMax)/(g12->Integral( xMin, xMax) + g22->Integral( xMin, xMax))*100);
  parrrr[3] = (g22->Integral( xMin, xMax)/(g12->Integral( xMin, xMax) + g22->Integral( xMin, xMax))*100);


  return parrrr;
}
