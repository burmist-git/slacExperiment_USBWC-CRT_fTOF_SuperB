Int_t diffConfCompNphot(){

  TString file1;
  TString file2;
  
  file1 = "../../Histos/3/Histos_merged_RUN4.root";
  //file2 = "../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF.root";
  file2 = "../../HistosSim/3/014/Histos_fTOFslacCRT_QUARTZ_fTOF.root";
  TFile *f1 = new TFile(file1.Data());
  TFile *f2 = new TFile(file2.Data());
  
  TH1D *h1_f1_1 = (TH1D*)f1->Get("h1nPeTotPerEv_AmpCut");
  TH1D *h1_f2_1 = (TH1D*)f2->Get("h1nChSig");
  TH1D *h1_f2_2 = (TH1D*)f2->Get("h1nPhotEl");

  //                  hh1     color    Devidor  rebin   normMax error
  normalisationH1D( h1_f1_1, kBlack,    1.0,     0,        1,    0);
  normalisationH1D( h1_f2_1, kRed,      1.0,     0,        1,    0);
  normalisationH1D( h1_f2_2, kBlue,     1.0,     0,        1,    0);
  
  TCanvas *c1 = new TCanvas("c1","plots",10,10,600,800);
  c1->Clear();
  c1->SetFillColor(kWhite);    

  h1_f2_2->Draw();
  h1_f1_1->Draw("same");
  h1_f2_1->Draw("same");
  //h1_f2_2->Draw("same");

  return 0;
}

void normalisationH1D(TH1D *hh1,Int_t color, Double_t Devidor, 
		      Int_t rebin, bool normMax, bool errYes){
  if(rebin != 0)
    hh1->Rebin(rebin);
  Double_t maxEn = (Double_t)hh1->GetMaximum();
  for(Int_t i = 1;i<=hh1->GetNbinsX();i++){
    if(normMax==true){
      hh1->SetBinContent(i,hh1->GetBinContent(i)/maxEn/Devidor);
      if(errYes ==true)
	hh1->SetBinError(i,hh1->GetBinError(i)/maxEn/Devidor);
    }
    else{
      hh1->SetBinContent(i,hh1->GetBinContent(i)/hh1->GetEntries()/Devidor);
      if(errYes ==true)
	hh1->SetBinError(i,hh1->GetBinError(i)/hh1->GetEntries()/Devidor);
    }
  }
  hh1->SetLineWidth(2);
  return hh1->SetLineColor(color);
}

void alignment( TH1D *hh1, Double_t centrate){
  Int_t binMax    = hh1->GetMaximumBin();
  Int_t binCenter = getBinID( hh1, centrate);
  TH1D *hh1copy = new TH1D();
  Int_t k = -999;
  makeCopy(hh1,hh1copy);
  Int_t nBinX = hh1->GetNbinsX();
  Double_t bC;
  for(Int_t i = 1;i<=nBinX;i++){
    k = i - binCenter + binMax;
    if(k>=1 && k<=nBinX){
      bC = hh1copy->GetBinContent(k);
      hh1->SetBinContent(i,bC);
    }
    else
      hh1->SetBinContent(i,0.0);
  }
}

Int_t getBinID( TH1D *hh1, Double_t valX){
  for(Int_t i = 1;i<=hh1->GetNbinsX();i++){
    if(valX >= hh1->GetBinLowEdge(i) && valX <= (hh1->GetBinLowEdge(i) + hh1->GetBinWidth(i))){
      //cout<<" L "<<hh1->GetBinLowEdge(i)<<endl
      //  <<" R "<<(hh1->GetBinLowEdge(i) + hh1->GetBinWidth(i))<<endl;
      return i;
    }
  }
  return -1;
}

void makeCopy(TH1D *hh1, TH1D *hh1copy){
  for(Int_t i = 1;i<=hh1->GetNbinsX();i++){
    hh1copy->SetBins(i,hh1->GetBinLowEdge(i),(hh1->GetBinLowEdge(i) + hh1->GetBinWidth(i)));
    hh1copy->SetBinContent(i,hh1->GetBinContent(i));
  }
  TString hc_name = hh1->GetName();
  hc_name += "_copy";
  hh1copy->SetName(hc_name.Data());
}


TH1D *divide2Histo(TH1D *h1_1, TH1D *h1_2){
  Int_t nBinh1_1 = h1_1->GetNbinsX();
  Int_t nBinh1_2 = h1_2->GetNbinsX();
  //cout<<"nBinh1_1 "<<nBinh1_1<<endl
  //<<"nBinh1_2 "<<nBinh1_2<<endl;
  TString hc_name = h1_1->GetName();
  hc_name += "_div_";
  hc_name += h1_2->GetName();
  TH1D *h1div = new TH1D(hc_name.Data(),hc_name.Data(),nBinh1_1,h1_1->GetBinLowEdge(1), (h1_1->GetBinLowEdge(nBinh1_1) + h1_1->GetBinWidth(nBinh1_1)));
  for(Int_t i = 1;i<=nBinh1_1;i++){
    //h1div->SetBins(i,h1_1->GetBinLowEdge(i),(h1_1->GetBinLowEdge(i) + h1_1->GetBinWidth(i)));
    //cout<<i<<"   "<<h1_1->GetBinLowEdge(i)<<"  "<<(h1_1->GetBinLowEdge(i) + h1_1->GetBinWidth(i))<<endl;
    if((h1_2->GetBinContent(i)!=0.0))
      h1div->SetBinContent(i,h1_1->GetBinContent(i)/(h1_2->GetBinContent(i)));
    else
      h1div->SetBinContent(i,0.0);
  }
  return h1div;
}
