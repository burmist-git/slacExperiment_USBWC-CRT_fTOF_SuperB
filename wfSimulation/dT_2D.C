Int_t dT_2D(){

  TString file1;
  file1 = "../../HistosWFsim/Histo_Thesis_CFD_vs_leadEdge.root";
  TFile *f1 = new TFile(file1.Data());

  TString histN;
  //histN = "h2dTime_VS_Ampl";
  //histN = "h2dTime_VS_Ampl_constL";
  histN = "h2dTime_VS_Ampl_DL";
  TH1D *h1dTime = (TH1D*)f1->Get(histN.Data());

  TCanvas *c1 = new TCanvas("c1","canva", 10, 10, 800, 800);
  c1->SetFillColor(kWhite);

  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);

  h1dTime->SetTitle("");
  h1dTime->SetLineWidth(3.0);
  //h1dTime->GetXaxis()->SetRangeUser(-0.5,1.1);
  h1dTime->GetYaxis()->SetTitle("time, ns");
  h1dTime->GetXaxis()->SetTitle("Amplitude, A");
  h1dTime->Draw("ZCOLOR");

  return 0;
}
