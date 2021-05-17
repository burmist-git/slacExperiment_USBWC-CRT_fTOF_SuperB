Int_t QE_Bialkali(){

  const Int_t NN = 32;

  Double_t Lambda[NN];
  Double_t QE_Bialkali[NN];

  Lambda[0] = 300;
  Lambda[1] = 310;
  Lambda[2] = 320;
  Lambda[3] = 330;
  Lambda[4] = 340;
  Lambda[5] = 350;
  Lambda[6] = 360;
  Lambda[7] = 370;
  Lambda[8] = 380;
  Lambda[9] = 390;
  Lambda[10] = 400;
  Lambda[11] = 410;
  Lambda[12] = 420;
  Lambda[13] = 430;
  Lambda[14] = 440;
  Lambda[15] = 450;
  Lambda[16] = 460;
  Lambda[17] = 470;
  Lambda[18] = 480;
  Lambda[19] = 490;
  Lambda[20] = 500;
  Lambda[21] = 510;
  Lambda[22] = 520;
  Lambda[23] = 530;
  Lambda[24] = 540;
  Lambda[25] = 550;
  Lambda[26] = 560;
  Lambda[27] = 570;
  Lambda[28] = 580;
  Lambda[29] = 590;
  Lambda[30] = 600;
  Lambda[31] = 610;

  QE_Bialkali[0] = 0.085;
  QE_Bialkali[1] = 0.102;
  QE_Bialkali[2] = 0.119;
  QE_Bialkali[3] = 0.136;
  QE_Bialkali[4] = 0.153;
  QE_Bialkali[5] = 0.17;
  QE_Bialkali[6] = 0.187;
  QE_Bialkali[7] = 0.204;
  QE_Bialkali[8] = 0.221;
  QE_Bialkali[9] = 0.238;
  QE_Bialkali[10] = 0.2465;
  QE_Bialkali[11] = 0.2448;
  QE_Bialkali[12] = 0.2431;
  QE_Bialkali[13] = 0.2414;
  QE_Bialkali[14] = 0.2397;
  QE_Bialkali[15] = 0.238;
  QE_Bialkali[16] = 0.221;
  QE_Bialkali[17] = 0.1955;
  QE_Bialkali[18] = 0.17;
  QE_Bialkali[19] = 0.14875;
  QE_Bialkali[20] = 0.1275;
  QE_Bialkali[21] = 0.10625;
  QE_Bialkali[22] = 0.085;
  QE_Bialkali[23] = 0.068;
  QE_Bialkali[24] = 0.051;
  QE_Bialkali[25] = 0.04675;
  QE_Bialkali[26] = 0.0425;
  QE_Bialkali[27] = 0.034;
  QE_Bialkali[28] = 0.0255;
  QE_Bialkali[29] = 0.017;
  QE_Bialkali[30] = 0.01275;
  QE_Bialkali[31] = 0.0085;

  TCanvas *c1 = new TCanvas("c1","canva", 10, 10, 800, 700);
  c1->SetFillColor(kWhite);

  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);

  TGraph *gr1_pr1 = new TGraph( NN, Lambda, QE_Bialkali);
  gr1_pr1->SetName("");
  gr1_pr1->SetTitle("");
  gr1_pr1->SetLineWidth(3.0);
  gr1_pr1->GetXaxis()->SetTitle("#lambda, nm");
  gr1_pr1->GetYaxis()->SetTitle("Quantum efficiency");
  gr1_pr1->Draw("APL");


}
