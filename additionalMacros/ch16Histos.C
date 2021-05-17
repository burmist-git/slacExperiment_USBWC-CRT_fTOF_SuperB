Int_t ch16Histos(  Int_t chID = 15){

  TString file1;

  file1 = "../../Histos/2/Histos_merged_laser.root";
  //file1 = "../../Histos/2/Histos_merged_laser_NOCut.root";
  //file1 = "../../Histos/2/Histos_merged_laser_CTCut.root";  
  //file1 = "../../Histos/2/Histos_merged_laser_NOamplCut.root";

  TFile *f1 = new TFile(file1.Data());

  TString nameh1;

  TH1D *h1_f1_1 = (TH1D*)f1->Get("h1Charge_my_baseCut_ch14");
  h1_f1_1->GetXaxis()->SetRangeUser(-0.5,2.5);
  h1_f1_1->Draw();

  /*
  nameh1 = "h1Amplitude_my_baseCut_ch"; nameh1 += chID;
  TH1D *h1_f1_1 = (TH1D*)f1->Get(nameh1.Data());
  nameh1 = "h1AmplitudeNeg_my_baseCut_ch"; nameh1 += chID;
  TH1D *h1_f1_2 = (TH1D*)f1->Get(nameh1.Data());
  nameh1 = "h1MaxAmplBeforeFirstChangDerAmpl_my_baseCut_ch"; nameh1 += chID;
  TH1D *h1_f1_3 = (TH1D*)f1->Get(nameh1.Data());
  nameh1 = "h1MaxBaseLineAmpl_my_baseCut_ch"; nameh1 += chID;
  TH1D *h1_f1_4 = (TH1D*)f1->Get(nameh1.Data());
  nameh1 = "h1TotAmplitudePos_my_baseCut_ch"; nameh1 += chID;
  TH1D *h1_f1_5 = (TH1D*)f1->Get(nameh1.Data());


  TCanvas *c1 = new TCanvas("c1","canvas",10,10,800,800);
  c1->SetFillColor(kWhite);
  c1->SetLogy();

  h1_f1_2->SetLineColor(kRed);
  h1_f1_3->SetLineColor(kBlue);
  h1_f1_4->SetLineColor(kMagenta);
  h1_f1_5->SetLineColor(kGreen);

  h1_f1_1->SetLineWidth(2);
  h1_f1_2->SetLineWidth(2);
  h1_f1_3->SetLineWidth(2);
  h1_f1_4->SetLineWidth(2);
  h1_f1_5->SetLineWidth(2);
  
  h1_f1_4->Draw();
  //h1_f1_4->GetXaxis()->SetRangeUser(-0.3,0.5);

  h1_f1_2->Draw("same");
  h1_f1_3->Draw("same");
  h1_f1_5->Draw("same");
  h1_f1_1->Draw("same");

  TString nameEps = "ch";
  nameEps += chID; 
  nameEps += ".eps"; 

  c1->SaveAs(nameEps.Data());
  */


  /*
  TH1D *h1_f1_1 = (TH1D*)f1->Get("h1Amplitude_my_baseCut_ch3");
  TH1D *h1_f1_2 = (TH1D*)f1->Get("h1Charge_my_baseCut_ch3");
  TH1D *h1_f1_3 = (TH1D*)f1->Get("h1Rise_my_baseCut_ch3");
  TH1D *h1_f1_4 = (TH1D*)f1->Get("h1Width_my_baseCut_ch3");
  TH1D *h1_f1_5 = (TH1D*)f1->Get("h1AmplitudeNeg_my_baseCut_ch3");
  TH1D *h1_f1_6 = (TH1D*)f1->Get("h1Fall_my_baseCut_ch3");
  TH1D *h1_f1_7 = (TH1D*)f1->Get("h1nOfPosInterAtLevel_my_baseCut_ch3");
  TH1D *h1_f1_8 = (TH1D*)f1->Get("h1nOfNegInterAtLevel_my_baseCut_ch3");  

  TCanvas *c1 = new TCanvas("c1","canvas",10,10,800,800);
  c1->SetFillColor(kWhite);
  c1->Divide(4,2);
  c1->cd(1);
  h1_f1_1->Draw();
  c1->cd(2);
  h1_f1_2->Draw();
  c1->cd(3);
  h1_f1_3->Draw();
  c1->cd(4);
  h1_f1_4->Draw();
  c1->cd(5);
  h1_f1_5->Draw();
  c1->cd(6);
  h1_f1_6->Draw();
  c1->cd(7);
  h1_f1_7->Draw();
  c1->cd(8);
  h1_f1_8->Draw();
  */

  return 0;
}
