TString file1;

file1 = "../../Histos/Histos_usbwc_test_All_n_C.root";

TFile *f1 = new TFile(file1.Data());

TH1D *h1dTime_L_ch1 = (TH1D*)f1->Get("h1dTime_L_ch1");
TH1D *h1dTime_L_ch2 = (TH1D*)f1->Get("h1dTime_L_ch2");

TH1D *h1dTime_L_ch5 = (TH1D*)f1->Get("h1dTime_L_ch5");
TH1D *h1dTime_L_ch6 = (TH1D*)f1->Get("h1dTime_L_ch6");

TH1D *h1dTime_L_TOT = new TH1D("h1dTime_L_TOT","h1dTime_L_TOT",2000,-10,10);

Int_t mergPlotsCuts(){

  Int_t i = 0;
  Int_t nn = 0;
  Double_t bC = 0;
  Double_t bC_opd = 0;

  nn = h1dTime_L_ch1->GetMaximumBin()-1000;
  for(i = 1;i<=2000;i++){
    if((i+nn)<=2000 && (i+nn)>=1){
      bC = h1dTime_L_ch1->GetBinContent(i+nn);
      bC_opd = h1dTime_L_TOT->GetBinContent(i);
      h1dTime_L_TOT->SetBinContent(i,(bC+bC_opd));
    }
  }

  nn = h1dTime_L_ch2->GetMaximumBin()-1000;
  for(i = 1;i<=2000;i++){
    if((i+nn)<=2000 && (i+nn)>=1){
      bC = h1dTime_L_ch2->GetBinContent(i+nn);
      bC_opd = h1dTime_L_TOT->GetBinContent(i);
      h1dTime_L_TOT->SetBinContent(i,(bC+bC_opd));
    }
  }

  h1dTime_L_TOT->Draw();
  h1dTime_L_TOT->SaveAs("h1dTime_L_TOT.C");
  h1dTime_L_ch1->Draw("same");
}
