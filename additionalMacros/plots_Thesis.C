Int_t plots_Thesis(){

  TString file1;  
  file1 = "test_nn.root.root";
  TFile *f1 = new TFile(file1.Data());

  TCanvas *c1 = new TCanvas("c1","canva", 10, 10, 800, 700);
  c1->SetFillColor(kWhite);
  
  //TH2D *hh_f1= (TH2D*)f1->Get("h2Hodo2YvsX");
  //TH2D *hh_f1= (TH2D*)f1->Get("h2_YintvsXint_Quartz");
  TH1D *hh_f1  = (TH1D*)f1->Get("h1_Yint_Quartz");
  Int_t  Nbin  = hh_f1->GetNbinsX();
  Double_t xL  = hh_f1->GetBinLowEdge(1);
  Double_t xR  = hh_f1->GetBinLowEdge(Nbin) + hh_f1->GetBinWidth(Nbin);
  TH1D *h1_der = new TH1D("h1_der","h1 der",Nbin,xL,xR);
  GetDerivative(hh_f1, h1_der);

  hh_f1->SetTitle("");
  hh_f1->SetLineWidth(3.0);
  //hh_f1->GetYaxis()->SetTitle("y, cm");
  hh_f1->GetXaxis()->SetTitle("x, cm");

  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);
 
  //hh_f1->Draw("ZCOLOR");
  //hh_f1->Draw();
  h1_der->Draw();

  /*
  //TLine *l_l = new TLine(-1.6, 0,  -1.6, 40000);
  //TLine *l_r = new TLine(2.59, 0,  2.59, 40000);
  TLine *l_l   = new TLine(-8.6, 0,  -8.6, 18000);
  TLine *l_r   = new TLine(12.4, 0,  12.4, 18000);

  l_l->SetLineWidth(3.0);
  l_r->SetLineWidth(3.0);

  l_l->SetLineColor(kRed);
  l_r->SetLineColor(kRed);

  l_l->Draw("same");
  l_r->Draw("same");
  */

}

void GetDerivative( TH1D *hh, TH1D *hh_f1der){
  Int_t  Nbin = hh->GetNbinsX();
  Double_t xL = hh->GetBinLowEdge(1);
  Double_t xR = hh->GetBinLowEdge(Nbin) + hh->GetBinWidth(Nbin);
  Double_t der;
  for(Int_t i = 2;i<Nbin;i++){
    //der = (hh->GetBinContent(i)-hh->GetBinContent(i-1))/(hh->GetBinCenter(i) - hh->GetBinCenter(i-1));
    der = hh->GetBinContent(i)-hh->GetBinContent(i-1);
    //der = der*(-1.0);
    hh_f1der->SetBinContent(i,der);
  }
}
