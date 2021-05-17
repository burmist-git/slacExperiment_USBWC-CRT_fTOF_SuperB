Int_t optimaNspline(){

  const Int_t NN = 9;

  Double_t xx[NN]    = {     0.001,     1,    2,    3,    4,    5,    6,    10,    20};
  Double_t tt[NN]    = {    18,    47,   60,   70,   84,   96,  113,   163,   283};
  Double_t ttRMS[NN] = {0.0114, 0.0086,0.0087,0.0087,0.0086,0.0087,0.0087, 0.0086, 0.0085};
  Double_t xxerr[NN];
  Double_t yyerr[NN];
  for(Int_t i = 0;i<NN;i++){
    xxerr[i] = 0.0;
    yyerr[i] = 0.00009;
    tt[i] = tt[i]/18.0;
  }

  TCanvas *c1 = new TCanvas("c1","canva", 10, 10, 800, 700);
  c1->SetFillColor(kWhite);

  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);

  TGraphErrors *gr1_pr1 = new TGraphErrors( NN, xx, tt, xxerr, yyerr);
  //TGraphErrors *gr1_pr1 = new TGraphErrors( NN, xx, ttRMS, xxerr, yyerr);
  gr1_pr1->SetName("");
  gr1_pr1->SetTitle("Normalized CPU time = p1#times(Number of spline points) + p0");
  //gr1_pr1->SetTitle("");
  gr1_pr1->SetLineWidth(3.0);
  gr1_pr1->SetMarkerStyle(20);
  //gr1_pr1->GetXaxis()->SetTitle("Constant fraction value");
  gr1_pr1->GetYaxis()->SetTitle("Normalized CPU time");
  gr1_pr1->GetXaxis()->SetTitle("Number of spline points");
  //gr1_pr1->GetYaxis()->SetTitle("#sigma_{waveform}, ns");
  gr1_pr1->Draw("AP");


  return 0;
}
