Int_t gaussDer(){

  Double_t xMin = -10;
  Double_t xMax =  10;

  TF1 *fb = new TF1("fb","gaus(0)",xMin,xMax);
  fb->SetParameters(1,0,1);
  fb->SetNpx(10000);
  //cout<<"der  "<<fb->Derivative(1.0)<<endl;

  const Int_t N = 10000;
  Double_t xx[N];
  Double_t yy[N];
  
  for(Int_t i=0;i<N;i++){
    xx[i] = xMin + (xMax-xMin)*i/(N-1);
    //cout<<"der  "<<fb->Derivative(1.0)<<endl;
    yy[i] = fb->Derivative(xx[i]);
    //cout<<"xx  =  "<<xx[i]<<"   yy = "<<yy[i]<<endl;
  }

  
  
  TGraph *gr1 = new TGraph(N,xx,yy);
  gr1->SetMaximum(1.2);
  //gr1->Draw("APL");
  fb->Draw();


  return 0;
}
