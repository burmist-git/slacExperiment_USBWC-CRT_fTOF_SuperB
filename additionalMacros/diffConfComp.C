Int_t diffConfComp(){

  TString file1;
  TString file2;
  TString file3;
  TString file4;
  TString file5;
  TString file6;
  TString file7;
  TString file8;
  
  file1 = "../../HistosSim/Histos_fTOFslacCRT1000_1500MeV_x0_y0_thata175_phi20.root";
  file2 = "../../HistosSim/Histos_fTOFslacCRT_QUARTZ_fTOF_400MeV.root";
  file3 = "../../HistosSim/Histos_fTOFslacCRT_QUARTZ_fTOF_zero.root";
  //file4 = "../../Histos/Histos_USBWC_OldExp.root";
  file4 = "../../Histos/Histos_USBWC_OldExp_m2.5crossTalkThreshold_cut_risetime_drisetime.root";
  file5 = "../../HistosSim/Histos_fTOFslacCRT_QUARTZ_fTOF_zero_cut.root";
  //file6 = "../../HistosSim/Histos_fTOFslacCRT_QUARTZ_fTOF_zero_cut_0.42collEff.root";
  file6 = "../../HistosSim/Histos_fTOFslacCRT_QUARTZ_fTOF_zero_cutNewRiseTime_0.42collEff.root";

  file7 = "../../HistosWFsim/Histo_NsplinePoint5_SP.root";
  file8 = "../wfSimulation/wfData/Histos_laser_first.root";

  //TString namePS = "./plots_diffConfComp.ps";
  
  TFile *f1 = new TFile(file1.Data());
  TFile *f2 = new TFile(file2.Data());
  TFile *f3 = new TFile(file3.Data());
  TFile *f4 = new TFile(file4.Data());
  TFile *f5 = new TFile(file5.Data());
  TFile *f6 = new TFile(file6.Data());
  TFile *f7 = new TFile(file7.Data());
  TFile *f8 = new TFile(file8.Data());
  
  TH1D *h1_f1_1 = (TH1D*)f1->Get("h1dTimeBoard_ch3");
  TH1D *h1_f1_2 = (TH1D*)f1->Get("h1dTimeBoardFIRST_ch3");
  TH1D *h1_f2_1 = (TH1D*)f2->Get("h1dTimeBoard_ch3");
  TH1D *h1_f2_2 = (TH1D*)f2->Get("h1dTimeBoardFIRST_ch3");
  TH1D *h1_f3_1 = (TH1D*)f3->Get("h1dTimeBoard_ch3");
  TH1D *h1_f3_2 = (TH1D*)f3->Get("h1dTimeBoardFIRST_ch3");
  TH1D *h1_f4_1 = (TH1D*)f4->Get("h1dTime_L_ch1");
  TH1D *h1_f4_2 = (TH1D*)f4->Get("h1dTime_L_CUT_ch1");
  TH1D *h1_f4_3 = (TH1D*)f4->Get("h1RiseTimeSP_ch3");
  TH1D *h1_f5_1 = (TH1D*)f5->Get("h1dTimeBoard_ch3");
  TH1D *h1_f6_1 = (TH1D*)f6->Get("h1dTimeBoard_ch3");
  TH1D *h1_f6_2 = (TH1D*)f6->Get("h1RiseTimeNoCut_ch3");

  TH1D *h1_f7_1 = (TH1D*)f7->Get("h1RiseTimeFirstPos");
  TH1D *h1_f8_1 = (TH1D*)f8->Get("h1RiseTimeFirstPosSP_ch3");

  //( TH1D *hh1, Int_t color, Double_t Devidor, 
  //                 Int_t rebin, bool normMax)

  normalisationH1D( h1_f1_1, kBlack, 1.0, 30, 1);
  normalisationH1D( h1_f1_2, kRed  , 1.0, 30, 1);

  normalisationH1D( h1_f2_1, kBlack, 1.0, 10, 1);
  normalisationH1D( h1_f2_2, kBlue , 1.0, 10, 1);
  
  normalisationH1D( h1_f3_1, kMagenta,  1.0, 20, 1);
  normalisationH1D( h1_f3_2, kBlue ,  1.0, 20, 1);

  normalisationH1D( h1_f4_1, kRed ,  1.0,  2, 1);
  normalisationH1D( h1_f4_2, kBlue,  1.0,  1, 1);

  normalisationH1D( h1_f5_1, kBlack,   1.0, 20, 1);
  normalisationH1D( h1_f6_1, kMagenta, 1.0, 20, 1);
  
  normalisationH1D( h1_f4_3, kBlue,    1.0, 0, 1);
  normalisationH1D( h1_f7_1, kBlack,   1.0, 0, 1);
  normalisationH1D( h1_f6_2, kMagenta, 1.0, 0, 1);
  normalisationH1D( h1_f8_1, kRed,     1.0, 0, 1);


  TH1D *h1_f4_2_div_h1_f3_1 = divide2Histo(h1_f4_2, h1_f3_1);
  //cout<<"getBinID(h1_f3_1,0.0) "<<getBinID(h1_f3_1,0.0)<<endl;

  alignment( h1_f3_1, 0.0);
  alignment( h1_f3_2, 0.0);
  alignment( h1_f4_1, 0.0);
  alignment( h1_f4_2, 0.0);
  alignment( h1_f5_1, 0.0);
  alignment( h1_f6_1, 0.0);

  //TString nameO  = namePS+"[";
  //TString nameC  = namePS+"]";
  
  TCanvas *c1 = new TCanvas("c1"," CRT plots",10,10,600,800);
  //c1->Print(nameO.Data()); 

  c1->Clear();
  c1->SetFillColor(kWhite);    

  //h1_f1_1->Draw();
  //h1_f1_2->Draw("same");
  //h1_f2_1->Draw();
  //h1_f2_2->Draw();
  //h1_f3_1->Draw();
  //h1_f3_2->Draw("same");
  //h1_f4_1->Draw();
  h1_f4_2->Draw();
  //h1_f5_1->Draw();
  h1_f6_1->Draw("same");

  //h1_f4_3->Draw();
  //h1_f6_2->Draw("same");

  //h1_f7_1->Draw();
  //h1_f4_3->Draw("same");
  //h1_f6_2->Draw("same");
  //h1_f8_1->Draw("same");


  //h1_f4_2_div_h1_f3_1->Draw();

  //c1->Print(namePS.Data());

  //c1->Print(nameC.Data());

  return 0;
}

void normalisationH1D(TH1D *hh1,Int_t color, Double_t Devidor, 
		       Int_t rebin, bool normMax){
  if(rebin != 0)
    hh1->Rebin(rebin);
  Double_t maxEn = (Double_t)hh1->GetMaximum();
  for(Int_t i = 1;i<=hh1->GetNbinsX();i++){
    if(normMax==true)
      hh1->SetBinContent(i,hh1->GetBinContent(i)/maxEn/Devidor);
    else
      hh1->SetBinContent(i,hh1->GetBinContent(i)/hh1->GetEntries()/Devidor);
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
