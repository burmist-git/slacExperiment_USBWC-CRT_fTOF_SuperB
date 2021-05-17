Int_t fit2Gauss_Thesis(){

  TString file1;
  
  Double_t x1Min;
  Double_t x1Max;
  Double_t x2Min;
  Double_t x2Max;
  Double_t xMin;
  Double_t xMax;
  Int_t nRebin = 0;
  
  /////////////////////////////////////////////
  //file1 = "../../HistosSim/3/007/Histos_fTOFslacCRT_QUARTZ_fTOF.root";
  //file1 = "../../HistosSim/3/042/Histos_fTOFslacCRT_QUARTZ_fTOF.root";
  file1 = "../../HistosSim/3/07/Histos_fTOFslacCRT_QUARTZ_fTOF.root";
  x1Min =  0.0-1.0;
  x1Max =  0.0+1.0;
  x2Min =  0.0-0.3;
  x2Max =  0.0+0.3;
  xMin  = -0.1-1.0;
  xMax  = -0.1+1.0;
  nRebin = 0;
  /////////////////////////////////////////////

  TFile *f1 = new TFile(file1.Data());
  //TH1D *h1dTime = (TH1D*)f1->Get("h1dTimeTOPtoBOT_first_ch3");
  TH1D *h1dTime = (TH1D*)f1->Get("h1dTimeTOPtoBOT_S_ch3");
  //TH1D *h1dTime = (TH1D*)f1->Get("h1dTimeL4_first_ch2");
  //TH1D *h1dTime = (TH1D*)f1->Get("h1dTimeL4_S_ch2");

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
   
  TF1 *g12 = new TF1("g12","gaus", xMin,xMax);
  TF1 *g22 = new TF1("g22","gaus", xMin,xMax);  
  TF1 *tripleG = new TF1("tripleG","gaus(0)+gaus(3)");
  tripleG->SetParameters(par); 
  h1dTime->Fit(tripleG,"","", xMin, xMax);
  tripleG->GetParameters(&parFit[0]);
  g12->SetParameters(parFit[0],parFit[1],parFit[2]);
  g22->SetParameters(parFit[3],parFit[4],parFit[5]);
  g12->SetLineStyle(kDashed);
  g22->SetLineStyle(kDashed);
  g12->SetLineColor(kGreen);
  g22->SetLineColor(kMagenta);
  h1dTime->Draw("");
  g12->Draw("same");
  g22->Draw("same");

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
