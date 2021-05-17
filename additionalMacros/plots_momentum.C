Int_t plots_momentum(){

  TString file1;  
  file1 = "test_nn.root.root.root";
  file1 = "";
  TFile *f1 = new TFile(file1.Data());

  TCanvas *c1 = new TCanvas("c1","canva", 10, 10, 800, 700);
  c1->SetFillColor(kWhite);
  
  TH1D *hh_f1 = (TH1D*)f1->Get("h1getTrkMomID_All");
  TH1D *hh_f2 = (TH1D*)f1->Get("h1getTrkMomID");
  TH1D *hh_f3 = (TH1D*)f1->Get("h1getTrkMomID_QSC");

  Double_t NN = 
    hh_f1->GetBinContent(1) +
    hh_f1->GetBinContent(2) +
    hh_f1->GetBinContent(3) +
    hh_f1->GetBinContent(4) + 
    hh_f1->GetBinContent(5);

  Int_t Nbin  = hh_f1->GetNbinsX();
  for(Int_t i = 1;i<=5;i++){
    hh_f1->SetBinContent(i,hh_f1->GetBinContent(i)/NN*100.0);
    hh_f2->SetBinContent(i,hh_f2->GetBinContent(i)/NN*100.0);
    hh_f3->SetBinContent(i,hh_f3->GetBinContent(i)/NN*100.0);
    cout<<i<<" "<<hh_f3->GetBinContent(i)<<endl;
  }

  hh_f1->SetTitle("");
  hh_f1->SetLineWidth(3.0);
  hh_f1->SetMinimum(0.0);
  hh_f1->GetXaxis()->SetTitle("momentum ID");
  hh_f1->GetYaxis()->SetTitle("Efficiency, %");

  hh_f2->SetLineWidth(3.0);
  hh_f3->SetLineWidth(3.0);
  hh_f2->SetLineColor(kRed);
  hh_f3->SetLineColor(kBlue);

  gStyle->SetPalette(1);
  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);
 
  hh_f1->Draw();
  hh_f2->Draw("same");
  hh_f3->Draw("same");


  TLegend *leg = new TLegend(0.7,0.2,0.9,0.4,"","brNDC");
  leg->AddEntry(hh_f1, "All mouns","l");
  leg->AddEntry(hh_f2, "Reco. muons","l");
  leg->AddEntry(hh_f3, "Reco. muons + cuts","l");
  leg->Draw();

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
