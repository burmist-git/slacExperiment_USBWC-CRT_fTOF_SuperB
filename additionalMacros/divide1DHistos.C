Int_t divide1DHistos(){
  
  TString file1;
  
  file1 = "../../Data/2/merged_RUN4_4hnSecBin_02maxdTcut.root";
  
  TFile *f1 = new TFile(file1.Data());
  
  //1
  TH1D *h1hist_numerator = (TH1D*)f1->Get("h1TimeUSBCRT");
  TH1D *h1hist_denominator = (TH1D*)f1->Get("h1TimeUSB");
  
  //TH1D *h1hist_denominator = (TH1D*)f1->Get("h1TimeCRT");
  //TH1D *h1hist_numerator = (TH1D*)f1->Get("h1TimeUSBCRT");
  
  Int_t N = h1hist_numerator->GetNbinsX();
  
  Double_t xMin = h1hist_numerator->GetBinLowEdge(1);
  Double_t xMax = h1hist_numerator->GetBinLowEdge(N) + 
    h1hist_numerator->GetYaxis()->GetBinWidth(N);
  
  TH1D *h1divide = new TH1D("h1divide","divide",N,xMin,xMax);
  
  Double_t numerator;
  Double_t denominator;
  
  for(Int_t i = 0; i<N;i++){
    numerator   = h1hist_numerator->GetBinContent(i+1);
    denominator = h1hist_denominator->GetBinContent(i+1);
    
    if(denominator!=0.0)
      h1divide->SetBinContent(i+1,numerator/denominator);
    else
      h1divide->SetBinContent(i+1,0.0);
  }
  
  h1divide->Draw();
  
  //h1TimeUSBCRT
}
