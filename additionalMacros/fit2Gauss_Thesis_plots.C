Int_t fit2Gauss_Thesis_plots(){

  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);

  const int num2         = 3;
  double xx[num2]        = {0.07, 0.42, 0.7};
  double xxErr[num2]     = {0.0, 0.0, 0.0};
  double sigN_F[num2]    = { 120,  97,  86};
  double sigN_FErr[num2] = { 10,  10,  10};
  double sigW_F[num2]    = { 590, 457, 396};
  double Fn_F[num2]      = {  40,  60,  71};
  double FW_F[num2]      = {  60,  40,  29};
  double F_err[num2]     = {  10,  10,  10};

  double sigN_S[num2] = {  130,  132,  121};
  double sigW_S[num2] = {  610,  492,  439};
  double Fn_S[num2]   = {   41,   61,   70};
  double FW_S[num2]   = {   59,   39,   29};

  double xx_Meas[2]        = {0.7,0.14};
  double xxErr_Meas[2]     = { 0.0, 0.0};
  double sigN_F_Meas[2]    = { 175,  110};
  double sigN_FErr_Meas[2] = {  10,  10};
 

  TCanvas *c1 = new TCanvas("c1","canva", 10, 10, 800, 700);
  c1->SetFillColor(kWhite);

  //TGraphErrors *gr1_sigN_F = new TGraphErrors( num2, xx, sigN_F, xxErr, sigN_FErr);
  //TGraphErrors *gr1_sigN_S = new TGraphErrors( num2, xx, sigN_S, xxErr, sigN_FErr);
  //TGraphErrors *gr1_sigN_M = new TGraphErrors( 2, xx_Meas, sigN_F_Meas, xxErr_Meas, sigN_FErr_Meas);
  TGraphErrors *gr1_Fn_F = new TGraphErrors( num2, xx, Fn_F, xxErr, F_err);
  TGraphErrors *gr1_Fn_S = new TGraphErrors( num2, xx, Fn_S, xxErr, F_err);
  TGraphErrors *gr1_FW_F = new TGraphErrors( num2, xx, FW_F, xxErr, F_err);
  TGraphErrors *gr1_FW_S = new TGraphErrors( num2, xx, FW_S, xxErr, F_err);

  TMultiGraph *mg = new TMultiGraph("mg","");

  mg->Add(gr1_Fn_F);
  //mg->Add(gr1_Fn_S);
  mg->Add(gr1_FW_F);
  //mg->Add(gr1_FW_S);
  mg->SetMinimum(0.0);
  mg->SetMaximum(100.0);

  mg->Draw("APL");
  mg->GetXaxis()->SetTitle("transmission efficiency");
  mg->GetYaxis()->SetTitle("Fraction, %");
  
  TLegend *leg = new TLegend(0.7,0.2,0.9,0.4,"","brNDC");
  leg->AddEntry(gr1_Fn_F, "Fr_{narrow} G4sim. + first  p.e.","lp");
  //leg->AddEntry(gr1_Fn_S, "Fr_{narrow} G4sim. + wavef. sim.","lp");
  leg->AddEntry(gr1_FW_F, "Fr_{wide} G4sim. + first  p.e.","lp");
  //leg->AddEntry(gr1_FW_S, "Fr_{wide} G4sim. + wavef. sim.","lp");
  leg->Draw();

  /*
  TMultiGraph *mg = new TMultiGraph("mg","");

  mg->Add(gr1_sigN_F);
  mg->Add(gr1_sigN_S);
  mg->Add(gr1_sigN_M);
  mg->SetMinimum(0.0);
  mg->SetMaximum(200.0);

  mg->Draw("APL");
  mg->GetXaxis()->SetTitle("transmission efficiency");
  mg->GetYaxis()->SetTitle("#sigma_{narrow}, ps");

  TLegend *leg = new TLegend(0.7,0.2,0.9,0.4,"","brNDC");
  leg->AddEntry(gr1_sigN_F, "#sigma_{narrow} G4sim. + first  p.e.","lp");
  leg->AddEntry(gr1_sigN_S, "#sigma_{narrow} G4sim. + wavef. sim.","lp");
  leg->AddEntry(gr1_sigN_M, "#sigma_{narrow} measurements","lp");
  leg->Draw();
  */

  return 0;
}
