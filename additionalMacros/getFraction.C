Int_t getFraction(){

  TString file1;
  Double_t yCenter;
  Double_t ySig;

  //file1 = "../ana_ftofProt/Histos_RUN1.root";
  //file1 = "../ana_ftofProt/Histos_RUN2.root";
  file1 = "../ana_ftofProt/Histos_RUN3.root";
  yCenter = 0.3;
  ySig = 0.5;



  TFile *f1 = new TFile(file1.Data());  
  TH2D *h2hist = (TH2D*)f1->Get("h2dTLvsUnixTime");
  //TH2D *h2hist = (TH2D*)f1->Get("h2dTL2vsUnixTime");

  Int_t nxBin = h2hist->GetNbinsX();
  Int_t nyBin = h2hist->GetNbinsY();
  Double_t xMin = h2hist->GetXaxis()->GetBinLowEdge(1);
  Double_t xMax = h2hist->GetXaxis()->GetBinLowEdge(nxBin) + 
                  h2hist->GetXaxis()->GetBinWidth(nxBin);
  Double_t yMin;
  Double_t yMax;

  //printf("xMin       : %20.4f \n", xMin);

  TH1D *h1hist = new TH1D("h1hist","hist", nxBin, xMin, xMax);
  TH1D *h1hist2sig = new TH1D("h1hist2sig","hist 2 sig", nxBin, xMin, xMax);
  TH1D *h1hist3sig = new TH1D("h1hist3sig","hist 3 sig", nxBin, xMin, xMax);
  TH1D *h1histMore3sig = new TH1D("h1histMore3sig","hist more 3 sig", nxBin, xMin, xMax);

  Double_t totNev = 0.0;
  Double_t NevSig = 0.0;
  Double_t Nev2Sig = 0.0;
  Double_t Nev3Sig = 0.0;
  Double_t NevMore3Sig = 0.0;

  Int_t j = 0;
  Int_t i = 0;

  for( j=1; j<=nxBin; j++){
    totNev = 0.0;
    NevSig = 0.0;
    Nev2Sig = 0.0;
    Nev3Sig = 0.0;
    NevMore3Sig = 0.0;
    for( i=1; i<=nyBin; i++){
      totNev = totNev + h2hist->GetBinContent(j,i);
      yMin = h2hist->GetYaxis()->GetBinLowEdge(i);
      yMax = h2hist->GetYaxis()->GetBinLowEdge(i) + 
	     h2hist->GetYaxis()->GetBinWidth(i);
      if(TMath::Abs(yMin - yCenter) < ySig && TMath::Abs(yMax - yCenter) < ySig)
	NevSig = NevSig + h2hist->GetBinContent(j,i);
      
      if(TMath::Abs(yMin - yCenter) >= ySig && TMath::Abs(yMin - yCenter) < 2*ySig){
	if(TMath::Abs(yMax - yCenter) >= ySig && TMath::Abs(yMax - yCenter) < 2*ySig)
	  Nev2Sig = Nev2Sig + h2hist->GetBinContent(j,i);
      }

      if(TMath::Abs(yMin - yCenter) >= 2*ySig && TMath::Abs(yMin - yCenter) < 3*ySig){
	if(TMath::Abs(yMax - yCenter) >= 2*ySig && TMath::Abs(yMax - yCenter) < 3*ySig)
	  Nev3Sig = Nev3Sig + h2hist->GetBinContent(j,i);
      }

      if(TMath::Abs(yMin - yCenter) >= 3*ySig && TMath::Abs(yMax - yCenter) >= 3*ySig)
	NevMore3Sig = NevMore3Sig + h2hist->GetBinContent(j,i);
    }

    if(totNev > 0.0){
      h1hist->SetBinContent(j,NevSig/totNev);
      h1hist2sig->SetBinContent(j,Nev2Sig/totNev);
      h1hist3sig->SetBinContent(j,Nev3Sig/totNev);
      h1histMore3sig->SetBinContent(j,NevMore3Sig/totNev);
    }

  }

  gStyle->SetPalette(1);
  TCanvas *c1 = new TCanvas("c1","h2hist", 10,10, 1000,800);
  c1->SetFillColor(kWhite);
  h2hist->Draw("ZCOLOR");
  TLine *lCenter = new TLine( xMin, yCenter, xMax, yCenter);
  TLine *lCenterLow = new TLine( xMin, yCenter - ySig, xMax, yCenter - ySig);
  TLine *lCenterUp = new TLine( xMin, yCenter + ySig, xMax, yCenter +  ySig);
  TLine *lCenter2Low = new TLine( xMin, yCenter - 2*ySig, xMax, yCenter - 2*ySig);
  TLine *lCenter2Up = new TLine( xMin, yCenter + 2*ySig, xMax, yCenter + 2*ySig);
  lCenter->Draw();
  lCenterLow->Draw();
  lCenterUp->Draw();
  lCenter2Low->Draw();
  lCenter2Up->Draw();
  
  TCanvas *c2 = new TCanvas("c2","h1hist", 20,20, 1000,800);
  c2->SetFillColor(kWhite);
  h1hist->Draw();

  TCanvas *c3 = new TCanvas("c3","h1hist", 30,30, 1000,800);
  c3->SetFillColor(kWhite);
  h1hist2sig->Draw();

  TCanvas *c4 = new TCanvas("c4","h1hist", 40,40, 1000,800);
  c4->SetFillColor(kWhite);
  h1hist3sig->Draw();

  TCanvas *c5 = new TCanvas("c5","h1hist", 50,50, 1000,800);
  c5->SetFillColor(kWhite);
  h1histMore3sig->Draw();

  return 0;
}
