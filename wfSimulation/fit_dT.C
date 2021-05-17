Int_t fit_dT(){

  TString file1;
  
  Double_t x1Min;
  Double_t x1Max;
  Double_t x2Min;
  Double_t x2Max;
  Double_t xMin;
  Double_t xMax;
  Int_t nRebin = 0;
  Double_t mean;

  ///////////////////////////////////////////////////////
  //file1 = "../../HistosWFsim/Histo_Thesis_0spline.root";
  //file1 = "../../HistosWFsim/Histo_Thesis_1spline.root";
  //file1 = "../../HistosWFsim/Histo_Thesis_2spline.root";
  //file1 = "../../HistosWFsim/Histo_Thesis_3spline.root";
  //file1 = "../../HistosWFsim/Histo_Thesis_4spline.root";
  //file1 = "../../HistosWFsim/Histo_Thesis_5spline.root";
  //file1 = "../../HistosWFsim/Histo_Thesis_6spline.root";
  //file1 = "../../HistosWFsim/Histo_Thesis_10spline.root";
  //file1 = "../../HistosWFsim/Histo_Thesis_20spline.root";
  ///////////////////////////////////////////////////////

  //////TIMEDIFF L//////
  x1Min = -0.02;
  x1Max =  0.01;
  x2Min = -0.06;
  x2Max =  0.0;
  xMin  = -0.06;
  xMax  =  0.06;
  nRebin = 0;
  ///------------------

  /////////////////////////////////////////////

  TFile *f1 = new TFile(file1.Data());
  TString histN;
  histN = "h1dTimeFirstPos_zoom";

  cout<<"file1   "<<file1<<endl
      <<"meanL   "<<mean<<endl
      <<"x1Min   "<<x1Min<<endl
      <<"x1Max   "<<x1Max<<endl
      <<"x2Min   "<<x2Min<<endl
      <<"x2Max   "<<x2Max<<endl
      <<"xMin    "<<xMin<<endl
      <<"xMax    "<<xMax<<endl
      <<"nRebin  "<<nRebin<<endl
      <<"histN   "<<histN<<endl;

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
