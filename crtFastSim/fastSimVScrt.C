TString fileS = "./Histos_crt_FastSim.root";
TString fileM = "./Histos_crt_meas.root";
TString fileR = "./Histos_crt_usb_merged.root";
TString namePS = "./plots_fastSimVScrt.ps";

TFile *fS = new TFile(fileS.Data());
TFile *fM = new TFile(fileM.Data());
TFile *fR = new TFile(fileR.Data());

TH1D *h1Theta_S = (TH1D*)fS->Get("h1muonTheta");
TH1D *h1Theta_M = (TH1D*)fM->Get("h1TrkTheta");

TH1D *h1Phi_S = (TH1D*)fS->Get("h1muonPhi");
TH1D *h1Phi_M = (TH1D*)fM->Get("h1TrkTPhi");

TH1D *h1QuartzX_S = (TH1D*)fS->Get("h1quartzSt_x");
TH1D *h1QuartzX_M = (TH1D*)fM->Get("h1_Xint_Quartz");
TH1D *h1QuartzY_S = (TH1D*)fS->Get("h1quartzSt_y");
TH1D *h1QuartzY_M = (TH1D*)fM->Get("h1_Yint_Quartz");
TH1D *h1QuartzZ_S = (TH1D*)fS->Get("h1quartzSt_z");
TH1D *h1QuartzZ_M = (TH1D*)fM->Get("h1_Zint_Quartz");

TH1D *h1fTOFx_S = (TH1D*)fS->Get("h1fTOF_x");
TH1D *h1fTOFx_M = (TH1D*)fM->Get("h1_Xint_fTOF");
TH1D *h1fTOFx_R = (TH1D*)fR->Get("h1_x_fTOF_top");

TH1D *h1fTOFy_S = (TH1D*)fS->Get("h1fTOF_y");
TH1D *h1fTOFy_M = (TH1D*)fM->Get("h1_Yint_fTOF");
TH1D *h1fTOFy_R = (TH1D*)fR->Get("h1_y_fTOF_top");

TH1D *normalisationH1D(TH1D *hh1);

Int_t fastSimVScrt(){

  normalisationH1D(h1Theta_S,kRed);
  normalisationH1D(h1Theta_M,kBlack);

  normalisationH1D(h1Phi_S,kRed);
  normalisationH1D(h1Phi_M,kBlack);

  normalisationH1D(h1fTOFx_S,kRed);
  normalisationH1D(h1fTOFx_M,kBlack);
  normalisationH1D(h1fTOFx_R,kMagenta);

  normalisationH1D(h1fTOFy_S,kRed);
  normalisationH1D(h1fTOFy_M,kBlack);
  normalisationH1D(h1fTOFy_R,kMagenta);

  normalisationH1D(h1QuartzX_S,kRed);
  normalisationH1D(h1QuartzX_M,kBlack);

  normalisationH1D(h1QuartzY_S,kRed);
  normalisationH1D(h1QuartzY_M,kBlack);

  normalisationH1D(h1QuartzZ_S,kRed);
  normalisationH1D(h1QuartzZ_M,kBlack);

  TString nameO  = namePS+"[";
  TString nameC  = namePS+"]";
  
  TCanvas *c1 = new TCanvas("c1"," CRT plots",10,10,600,800);
  c1->Print(nameO.Data()); 

  c1->Clear();
  c1->SetFillColor(kWhite);    
  h1Theta_M->Draw();
  h1Theta_S->Draw("same");
  c1->Print(namePS.Data());

  c1->Clear();
  c1->SetFillColor(kWhite);    
  h1Phi_M->Draw();
  h1Phi_S->Draw("same");
  c1->Print(namePS.Data());

  c1->Clear();
  c1->SetFillColor(kWhite);    
  h1fTOFx_R->Draw();
  h1fTOFx_M->Draw("same");
  h1fTOFx_S->Draw("same");
  c1->Print(namePS.Data());

  c1->Clear();
  c1->SetFillColor(kWhite);    
  h1fTOFy_R->Draw();
  h1fTOFy_S->Draw("same");
  h1fTOFy_M->Draw("same");
  c1->Print(namePS.Data());

  c1->Clear();
  c1->SetFillColor(kWhite);    
  h1QuartzX_M->Draw();
  h1QuartzX_S->Draw("same");
  c1->Print(namePS.Data());

  c1->Clear();
  c1->SetFillColor(kWhite);    
  h1QuartzY_M->Draw();
  h1QuartzY_S->Draw("same");
  c1->Print(namePS.Data());

  c1->Clear();
  c1->SetFillColor(kWhite);
  h1QuartzZ_M->Draw();  
  h1QuartzZ_S->Draw("same");
  c1->Print(namePS.Data());
  
  c1->Print(nameC.Data());

  return 0;
}


TH1D *normalisationH1D(TH1D *hh1,Int_t color){
  for(Int_t i = 1;i<=hh1->GetNbinsX();i++)
    hh1->SetBinContent(i,hh1->GetBinContent(i)/hh1->GetEntries());
  hh1->SetLineWidth(2);
  return hh1->SetLineColor(color);
}



