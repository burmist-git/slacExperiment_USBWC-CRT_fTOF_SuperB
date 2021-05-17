Int_t diffPlot_Thesis(){

  TString file1;

  //file1 = "../../HistosSim/3/007/Histos_fTOFslacCRT_QUARTZ_fTOF.root";
  //file1 = "../../HistosSim/3/042/Histos_fTOFslacCRT_QUARTZ_fTOF.root";
  file1 = "../../HistosSim/3/07/Histos_fTOFslacCRT_QUARTZ_fTOF.root";
  
  TFile *f1 = new TFile(file1.Data());
  
  //TH1D *h1f1_1 = (TH1D*)f1->Get("h1dTimeTOPtoBOT_first_ch3");
  //TH1D *h1f1_2 = (TH1D*)f1->Get("h1dTimeTOPtoBOT_S_ch3");
  TH1D *h1f1_1 = (TH1D*)f1->Get("h1dTimeL4_first_ch2");
  TH1D *h1f1_2 = (TH1D*)f1->Get("h1dTimeL4_S_ch2");

  h1f1_1->SetLineColor(kRed);
  h1f1_2->SetLineColor(kBlue);
  h1f1_1->SetLineWidth(3);
  h1f1_2->SetLineWidth(3);

  h1f1_1->Rebin(2);
  h1f1_2->Rebin(2);

  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);  

  TCanvas *c1 = new TCanvas("c1"," CRT plots",10,10,800,800);
  c1->SetFillColor(kWhite);

  h1f1_1->SetTitle("");
  h1f1_1->GetXaxis()->SetTitle("time, ns");
  h1f1_1->Draw();
  h1f1_2->Draw("same");

  TLegend *leg = new TLegend(0.7,0.2,0.9,0.4,"","brNDC");
  leg->AddEntry(h1f1_1, "G4Sim. + first  p.e.","l");
  leg->AddEntry(h1f1_2, "G4Sim. + wavef. sim.","l");
  leg->Draw();

  return 0;
}
