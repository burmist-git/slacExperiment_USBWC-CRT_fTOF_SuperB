Int_t diffConfCompN(){

  TString file1;
  TString file2;
  TString file3;
  TString file4;
  
  file1 = "../../Histos/2/Histos_merged_RUN4_4hnSecBin_02maxdTcut.root";
  //file1 = "../../Histos/2/Histos_merged_RUN4_4hnSecBin_02maxdTcut_trkCut.root";
  //file1 = "../../Histos/2/Histos_merged_RUN4_KinematicCut.root";
  //file2 = "../../HistosSim/2/Histos_fTOFslacCRT_QUARTZ_fTOF.root";
  //file2 = "../../HistosSim/2/Histos_fTOFslacCRT_QUARTZ_fTOF_KinCut.root";

  file3 = "../../Histos/2/Histos_merged_laser.root";
  file4 = "../../Histos/Histos_USBWC_OldExp_m2.5crossTalkThreshold_cut_risetime_drisetime.root";
  
  //TString namePS = "./plots_diffConfComp.ps";
  
  TFile *f1 = new TFile(file1.Data());
  TFile *f2 = new TFile(file2.Data());
  TFile *f3 = new TFile(file3.Data());
  TFile *f4 = new TFile(file4.Data());
  
  //timing L ch2
  //TH1D *h1_f1_1 = (TH1D*)f1->Get("h1dTimeL_ch2");
  //TH1D *h1_f2_1 = (TH1D*)f2->Get("h1dTimeL_first_ch2");
  //TH1D *h1_f2_1 = (TH1D*)f2->Get("h1dTimeL_ch2");
  //TH1D *h1_f3_1 = (TH1D*)f3->Get("h1dTimeL_ch2");

  //timing L3 ch2
  TH1D *h1_f1_1 = (TH1D*)f1->Get("h1dTimeL3_ch10");
  TH1D *h1_f2_1 = (TH1D*)f2->Get("h1dTimeL3_S_ch10");
  TH1D *h1_f2_2 = (TH1D*)f2->Get("h1dTimeL3_first_ch10");
  //TH1D *h1_f3_1 = (TH1D*)f3->Get("h1dTimeL3_ch3");

  //amplitude cut L ch3
  //TH1D *h1_f1_1 = (TH1D*)f1->Get("h1Amplitude_my_cut_ch3");
  //TH1D *h1_f2_1 = (TH1D*)f2->Get("h1Amplitude_my_cut_ch3");
  //TH1D *h1_f3_1 = (TH1D*)f3->Get("h1Amplitude_my_cut_ch3");

  //charge cut L ch2
  //TH1D *h1_f1_1 = (TH1D*)f1->Get("h1Charge_my_cut_ch3");
  //TH1D *h1_f2_1 = (TH1D*)f2->Get("h1Charge_my_cut_ch3");
  //TH1D *h1_f3_1 = (TH1D*)f3->Get("h1Charge_my_baseCut_ch3");
  //TH1D *h1_f3_2 = (TH1D*)f3->Get("h1Charge_my_cut_ch3");

  //rise time cut L ch2
  //TH1D *h1_f1_1 = (TH1D*)f1->Get("h1Rise_cut_ch2");
  //TH1D *h1_f2_1 = (TH1D*)f2->Get("h1Rise_cut_ch2");
  //TH1D *h1_f3_1 = (TH1D*)f3->Get("h1Rise_my_baseCut_ch2");
  //TH1D *h1_f4_1 = (TH1D*)f4->Get("h1RiseTimeSP_ch2");

  //                  hh1     color    Devidor  rebin   normMax error
  normalisationH1D( h1_f1_1, kBlack,    1.0,     2,        1,       1);
  normalisationH1D( h1_f2_1, kBlue,     1.0,     2,        1,       0);
  normalisationH1D( h1_f2_2, kRed,      1.0,     2,        1,       0);
  //normalisationH1D( h1_f4_1, kBlue,    1.0,     0,    1);
  //normalisationH1D( h1_f3_2,   kRed,   1.0,     2,    0);

  //TH1D *h1_f4_2_div_h1_f3_1 = divide2Histo(h1_f4_2, h1_f3_1);
  //cout<<"getBinID(h1_f3_1,0.0) "<<getBinID(h1_f3_1,0.0)<<endl;

  alignment( h1_f1_1, 0.0, 1);
  alignment( h1_f2_1, 0.0, 0);
  alignment( h1_f2_2, 0.0, 0);

  //TString nameO  = namePS+"[";
  //TString nameC  = namePS+"]";
  
  TCanvas *c1 = new TCanvas("c1"," CRT plots",10,10,600,800);
  //c1->Print(nameO.Data()); 

  c1->Clear();
  c1->SetFillColor(kWhite);    

  //h1_f2_1->Draw();
  h1_f1_1->Draw();
  h1_f2_1->Draw("same");
  //h1_f2_2->Draw("same");

  //h1_f3_2->Draw();
  //h1_f3_2->Draw("same");


  //c1->Print(namePS.Data());

  //c1->Print(nameC.Data());

  return 0;
}

void normalisationH1D(TH1D *hh1,Int_t color, Double_t Devidor, 
		      Int_t rebin, bool normMax, bool errYes){
  if(rebin != 0)
    hh1->Rebin(rebin);
  Double_t maxEn = (Double_t)hh1->GetMaximum();
  Double_t nEn = (Double_t)hh1->GetEntries();
  for(Int_t i = 1;i<=hh1->GetNbinsX();i++){
    if(normMax==true){
      if(maxEn !=0.0){
	if(errYes ==true){
	  //hh1->SetBinError(i,hh1->GetBinError(i)/maxEn/Devidor);
	  hh1->SetBinError(i,TMath::Sqrt(hh1->GetBinContent(i))/maxEn/Devidor);
	}
	hh1->SetBinContent(i,hh1->GetBinContent(i)/maxEn/Devidor);
      }
    }
    else{
      if(nEn !=0.0){
	if(errYes ==true){
	  //hh1->SetBinError(i,hh1->GetBinError(i)/nEn/Devidor);
	  hh1->SetBinError(i,TMath::Sqrt(hh1->GetBinContent(i))/nEn/Devidor);
	}
	hh1->SetBinContent(i,hh1->GetBinContent(i)/nEn/Devidor);
      }
    }
  }
  hh1->SetLineWidth(2);
  return hh1->SetLineColor(color);
}

void alignment( TH1D *hh1, Double_t centrate, bool errYes){
  Int_t binMax    = hh1->GetMaximumBin();
  Int_t binCenter = getBinID( hh1, centrate);
  TH1D *hh1copy = new TH1D();
  Int_t k = -999;
  makeCopy(hh1,hh1copy, errYes);
  Int_t nBinX = hh1->GetNbinsX();
  Double_t bC;
  Double_t bCerr;
  for(Int_t i = 1;i<=nBinX;i++){
    k = i - binCenter + binMax;
    if(k>=1 && k<=nBinX){
      if(errYes){
	bCerr = hh1copy->GetBinError(k);
	hh1->SetBinError(i,bCerr);
      }
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

void makeCopy(TH1D *hh1, TH1D *hh1copy, bool errYes){
  for(Int_t i = 1;i<=hh1->GetNbinsX();i++){
    hh1copy->SetBins(i,hh1->GetBinLowEdge(i),(hh1->GetBinLowEdge(i) + hh1->GetBinWidth(i)));
    hh1copy->SetBinContent(i,hh1->GetBinContent(i));
    hh1copy->SetBinError(i,hh1->GetBinError(i));
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
