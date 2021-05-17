Int_t A_vs_risetime(){

  const Int_t NN = 5;

  Double_t xx[NN]    = { 0.95,   1.5,   2.0,   2.5, 3.0};
  Double_t yy[NN]    = { 2.13, 2.612, 3.117, 3.792, 4.093};
  Double_t xxerr[NN];
  Double_t yyerr[NN];
  for(Int_t i = 0;i<NN;i++){
    xxerr[i] = 0.03;
    yyerr[i] = 0.06;
  }

  TCanvas *c1 = new TCanvas("c1","canva", 10, 10, 800, 700);
  c1->SetFillColor(kWhite);

  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);

  TGraphErrors *gr1_pr1 = new TGraphErrors( NN, xx, yy, xxerr, yyerr);
  gr1_pr1->SetName("");
  gr1_pr1->SetTitle("C_{1} = p1#times(Rise time) + p0");
  gr1_pr1->SetLineWidth(3.0);
  gr1_pr1->SetMarkerStyle(20);
  gr1_pr1->GetXaxis()->SetTitle("Rise time, ns");
  gr1_pr1->GetYaxis()->SetTitle("C_{1}");
  gr1_pr1->Draw("AP");


  return 0;
}
