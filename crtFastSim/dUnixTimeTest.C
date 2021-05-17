TRandom3 *rnd = new TRandom3();

Double_t GetdT();

Int_t dUnixTimeTest(){

  Int_t i = 0;
  Int_t N = 1000000;

  Double_t t  = 0.0;
  Double_t dt = 0.0;

  TH1D *h1dT = new TH1D("h1dT","dT dist",1000,0.0,100.0);

  for(i = 0;i<N;i++){
    dT = GetdT();
    h1dT->Fill(dT);
    t = t + dT;
  }

  cout<<"t = "<<t<<endl
      <<"N = "<<N<<endl
      <<"N/t = "<<(Double_t)N/t<<endl
      <<"t/N = "<<t/((Double_t)N)<<endl;

  h1dT->Draw();

  return 0;
}

Double_t GetdT(){
  //return rnd->Uniform(0.5, 10);
  //return rnd->Gaus(20.0,0.5);
  return (rnd->Exp(3.0)+1.0);
}
