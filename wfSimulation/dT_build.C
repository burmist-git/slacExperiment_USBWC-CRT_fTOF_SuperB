Int_t dT_build(){

  TString file1;
  
  //file1 = "../../HistosWFsim/Histo_Thesis_2G_p.root";  
  //file1 = "../../HistosWFsim/Histo_Thesis_2SP_pp.root";  
  file1 = "../../HistosWFsim/Histo_Thesis_SP_CS_pp.root";  
  
  TFile *f1 = new TFile(file1.Data());
  
  //TH1D *h1_rms1 = (TH1D*)f1->Get("h1dTimeFirstPos_zoom");
  TH1D *h1_rms1 = (TH1D*)f1->Get("h1dTimeFirstPos");
  
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);
  
  TCanvas *c1 = new TCanvas("c1","plot",10,10,800,600);
  c1->SetFillColor(kWhite);
  
  h1_rms1->SetTitle("");
  h1_rms1->Draw();
  
  return 0;
}

