Int_t dataRateAna(){

  TString file1;  
  file1 = "../mergeCRTandUSB/timing_RUN1.root";
  TFile *f1 = new TFile(file1.Data());

  TH1D *h1r = (TH1D*)f1->Get("h1TimeUSB");
  //TH1D *h1r = (TH1D*)f1->Get("h1TimeCRT");

  //h1r->Draw();

  //cout<<"h1r->GetMaximum() = "<<h1r->GetMaximum()<<endl;
  //cout<<"h1r->GetMinimum() = "<<h1r->GetMinimum()<<endl;

  Double_t maxR = h1r->GetMaximum() + 5;
  Double_t minR = h1r->GetMinimum() - 5;
  Int_t nBinR = (Int_t)(maxR - minR);

  TH1D *h1Rate = new TH1D("h1Rate","Rate", nBinR, minR, maxR);

  for(Int_t i = 1;i<=h1r->GetNbinsX();i++){
    h1Rate->Fill(h1r->GetBinContent(i));
  }
  
  h1Rate->Draw();

  return 0;
}
