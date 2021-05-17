Int_t optimaCFDval(){

  const Int_t NN = 9;

  Double_t xx[NN]    = {    0.1,    0.2,    0.3,    0.4,    0.5,    0.6,    0.7,    0.8,    0.9};
  Double_t yy[NN]    = { 0.0248, 0.0152, 0.0119, 0.0106, 0.0102, 0.0105, 0.0114, 0.0127, 0.0158};
  Double_t xxerr[NN];
  Double_t yyerr[NN];
  for(Int_t i = 0;i<NN;i++){
    xxerr[i] = 0.0;
    yyerr[i] = 0.0003;
  }

  TCanvas *c1 = new TCanvas("c1","canva", 10, 10, 800, 700);
  c1->SetFillColor(kWhite);

  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);

  TGraphErrors *gr1_pr1 = new TGraphErrors( NN, xx, yy, xxerr, yyerr);
  gr1_pr1->SetName("");
  //gr1_pr1->SetTitle("C_{1} = p1#times(Rise time) + p0");
  gr1_pr1->SetTitle("");
  gr1_pr1->SetLineWidth(3.0);
  gr1_pr1->SetMarkerStyle(20);
  gr1_pr1->GetXaxis()->SetTitle("Constant fraction value");
  gr1_pr1->GetYaxis()->SetTitle("#sigma_{waveform}");
  gr1_pr1->Draw("AP");


  return 0;
}
