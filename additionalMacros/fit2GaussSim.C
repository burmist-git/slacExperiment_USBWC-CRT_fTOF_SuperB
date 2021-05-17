Int_t fit2GaussSim(TString histN){

  TString file1;
  
  Double_t x1Min;
  Double_t x1Max;
  Double_t x2Min;
  Double_t x2Max;
  Double_t xMin;
  Double_t xMax;
  Int_t nRebin = 0;
  Double_t wideR;
  Double_t narrR;
  Double_t mean;

  //Int_t chID;
  TString epsName;
  
  /////////////////////////////////////////////
  //file1 = "../../HistosSim/2/Histos_fTOFslacCRT_QUARTZ_fTOF.root";
  //file1 = "../../HistosSim/2/Histos_fTOFslacCRT_QUARTZ_fTOF_thisEff.root";
  //file1 = "../../HistosSim/2/Histos_fTOFslacCRT_QUARTZ_fTOF_004collEff.root";
  file1 = "../../HistosSim/2/Histos_fTOFslacCRT_QUARTZ_fTOF_KinCut.root";

  //////TIMEDIFF L//////
  //wideR = 0.7;
  //narrR = 0.3;
  //x1Min =  meanL[chID]-wideR;
  //x1Max =  meanL[chID]+wideR;
  //x2Min =  meanL[chID]-narrR;
  //x2Max =  meanL[chID]+narrR;
  //xMin  =  meanL[chID]-wideR;
  //xMax  =  meanL[chID]+wideR;
  //nRebin = 0;
  ///------------------

  //////TIMEDIFF L2//////
  //wideR = 0.7;
  //narrR = 0.3;
  //x1Min =  meanL2[chID]-wideR;
  //x1Max =  meanL2[chID]+wideR;
  //x2Min =  meanL2[chID]-narrR;
  //x2Max =  meanL2[chID]+narrR;
  //xMin  =  meanL2[chID]-wideR;
  //xMax  =  meanL2[chID]+wideR;
  //nRebin = 0;
  ///--------------------

  //////TIMEDIFF L3//////
  //wideR = 0.7;
  //narrR = 0.3;
  //x1Min =  meanL3[chID]-wideR;
  //x1Max =  meanL3[chID]+wideR;
  //x2Min =  meanL3[chID]-narrR;
  //x2Max =  meanL3[chID]+narrR;
  //xMin  =  meanL3[chID]-wideR;
  //xMax  =  meanL3[chID]+wideR;
  //nRebin = 0;
  ///--------------------

  //////TIMEDIFF L4//////
  //wideR = 0.7;
  //narrR = 0.3;
  //x1Min =  meanL4[chID]-wideR;
  //x1Max =  meanL4[chID]+wideR;
  //x2Min =  meanL4[chID]-narrR;
  //x2Max =  meanL4[chID]+narrR;
  //xMin  =  meanL4[chID]-wideR;
  //xMax  =  meanL4[chID]+wideR;
  //nRebin = 2;
  ///--------------------

  //////TIMEDIFF TtB//////
  mean = 0.0;
  wideR = 0.7;
  narrR = 0.3;
  x1Min =  mean-wideR;
  x1Max =  mean+wideR;
  x2Min =  mean-narrR;
  x2Max =  mean+narrR;
  xMin  =  mean-wideR;
  xMax  =  mean+wideR;
  nRebin = 0;
  ///--------------------


  /////////////////////////////////////////////

  TFile *f1 = new TFile(file1.Data());
  //TString histN;
  //histN = "h1dTimeL_S_ch";
  //histN = "h1dTimeL_first_ch";
  //histN = "h1dTimeL2_ch";
  //histN = "h1dTimeL3_first_ch";
  //histN = "h1dTimeL3_S_ch";
  //histN = "h1dTimeL4_ch";
  //histN = "h1dTimeTOPtoBOT_ch";
  //histN += chID; 
  epsName = histN;
  epsName += ".eps";
  //cout<<histN<<endl;

  cout<<"file1   "<<file1<<endl
      <<"wideR   "<<wideR<<endl
      <<"narrR   "<<narrR<<endl
      <<"meanL   "<<mean<<endl
      <<"x1Min   "<<x1Min<<endl
      <<"x1Max   "<<x1Max<<endl
      <<"x2Min   "<<x2Min<<endl
      <<"x2Max   "<<x2Max<<endl
      <<"xMin    "<<xMin<<endl
      <<"xMax    "<<xMax<<endl
      <<"nRebin  "<<nRebin<<endl
    //<<"chID    "<<chID<<endl
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

  TCanvas *c2 = new TCanvas("c2","total fit",20,20,600,800);
  c2->SetFillColor(kWhite);   
  TF1 *g12 = new TF1("g12","gaus", xMin,xMax);
  TF1 *g22 = new TF1("g22","gaus", xMin,xMax);  
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

  h1dTime->GetXaxis()->SetRangeUser(mean-2.5,mean+2.5);
  h1dTime->SetLineWidth(2.0);
  h1dTime->Draw("");
  g12->Draw("same");
  g22->Draw("same");



  c2->SaveAs(epsName.Data());


  cout<<"Sigma wide   "<<parFit[2]*1000.0<<" ps"<<endl
      <<"Sigma narrow "<<parFit[5]*1000.0<<" ps"<<endl;
  
  cout<<" Integral wide   "<<g12->Integral( xMin, xMax)<<endl
      <<" Integral narrow "<<g22->Integral( xMin, xMax)<<endl;

  if((g12->Integral( xMin, xMax) + g22->Integral( xMin, xMax))!=0.0){
    cout<<" Fraction wide   "<<g12->Integral( xMin, xMax)/(g12->Integral( xMin, xMax) + g22->Integral( xMin, xMax))*100<<" %"<<endl
	<<" Fraction narrow "<<g22->Integral( xMin, xMax)/(g12->Integral( xMin, xMax) + g22->Integral( xMin, xMax))*100<<" %"<<endl;
  }  

  return 0;
}
