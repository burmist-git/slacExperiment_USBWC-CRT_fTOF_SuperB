Int_t timeResVSsnr(){

  TString file1;
  TString file2;
  TString file3;
  TString file4;
  TString file5;
  TString file6;
 
  file1 = "../../HistosWFsim/Histo_Thesis_l.root";  
  file2 = "../../HistosWFsim/Histo_Thesis_0.5ns.root";  
  file3 = "../../HistosWFsim/Histo_Thesis_0.95ns.root"; 
  file4 = "../../HistosWFsim/Histo_Thesis_1.5ns.root";  
  file5 = "../../HistosWFsim/Histo_Thesis_2.0ns.root";  
  file6 = "../../HistosWFsim/Histo_Thesis_2.5ns.root";
  
  TFile *f1 = new TFile(file1.Data());
  TFile *f2 = new TFile(file2.Data());
  TFile *f3 = new TFile(file3.Data());
  TFile *f4 = new TFile(file4.Data());
  TFile *f5 = new TFile(file5.Data());
  TFile *f6 = new TFile(file6.Data());
 
  TProfile *prof1 = (TH1D*)f1->Get("prof_dT_vs_SNR");
  TProfile *prof2 = (TH1D*)f2->Get("prof_dT_vs_SNR");
  TProfile *prof3 = (TH1D*)f3->Get("prof_dT_vs_SNR");
  TProfile *prof4 = (TH1D*)f4->Get("prof_dT_vs_SNR");
  TProfile *prof5 = (TH1D*)f5->Get("prof_dT_vs_SNR");
  TProfile *prof6 = (TH1D*)f6->Get("prof_dT_vs_SNR");
  
  TH1D *h1_rms1 = new TH1D();
  TH1D *h1_rms2 = new TH1D();
  TH1D *h1_rms3 = new TH1D();
  TH1D *h1_rms4 = new TH1D();
  TH1D *h1_rms5 = new TH1D();
  TH1D *h1_rms6 = new TH1D();

  getRMSfromProff( h1_rms1, prof1);
  getRMSfromProff( h1_rms2, prof2);
  getRMSfromProff( h1_rms3, prof3);
  getRMSfromProff( h1_rms4, prof4);
  getRMSfromProff( h1_rms5, prof5);
  getRMSfromProff( h1_rms6, prof6);

  //cout<<pp->GetBinError(i)<<endl;

  h1_rms1->SetLineWidth(2);
  h1_rms2->SetLineWidth(2);
  h1_rms3->SetLineWidth(2);
  h1_rms4->SetLineWidth(2);
  h1_rms5->SetLineWidth(2);
  h1_rms6->SetLineWidth(2);

  h1_rms1->SetLineColor(kBlack);
  h1_rms2->SetLineColor(kBlue);
  h1_rms3->SetLineColor(kRed);
  h1_rms4->SetLineColor(kRed);
  h1_rms5->SetLineColor(kBlack);
  h1_rms6->SetLineColor(kRed);

  TF1 *ff = new TF1("ff","[0]/x^([1])",300,650);
  //ff->SetParameters(1.58141e+09,2);
  h1_rms6->Fit(ff,"","",200,1000);

  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);

  TCanvas *c1 = new TCanvas("c1","plot",10,10,600,800);
  c1->SetFillColor(kWhite);
  h1_rms6->SetMaximum(0.06);
  h1_rms6->Draw();
  h1_rms6->SetTitle("#sigma_{waveform} = #frac{C_{1}}{(S/N)^{C_{2}}}");
  //h1_rms1->Draw();
  //h1_rms2->Draw("same");
  //h1_rms3->Draw("same");
  //h1_rms4->Draw("same");
  //h1_rms5->Draw("same");

  TPaveLabel *lt1 = new TPaveLabel(500, 0.02, 650, 0.025,"C_{1} = 3.22");
  TPaveLabel *lt2 = new TPaveLabel(500, 0.015, 650, 0.02,"C_{2} = 1.02");
  lt1->Draw("same");
  lt2->Draw("same");


  TLegend *leg = new TLegend(0.7,0.2,0.9,0.4,"","brNDC");
  //leg->AddEntry(h1_rms1, "Rise time = 0.95 ns (Single p.e. shape)","l");
  //leg->AddEntry(h1_rms2, "0.5 ns","l");
  //leg->AddEntry(h1_rms3, "Rise time = 0.95 ns (Gauss shape)","l");
  //leg->AddEntry(h1_rms4, "Rise time = 1.5  ns","l");
  leg->AddEntry(h1_rms5, "Rise time = 2.0  ns","l");
  leg->Draw();

  return 0;
}

void getRMSfromProff( TH1D *hh1, TProfile *pp){
  hh1->SetBins(pp->GetNbinsX(), 
	       pp->GetBinLowEdge(1), 
	       pp->GetBinLowEdge(pp->GetNbinsX()) + pp->GetBinWidth(pp->GetNbinsX()));
  for(Int_t i = 1;i<=pp->GetNbinsX();i++){
    //cout<<pp->GetBinError(i)<<endl;
    hh1->SetBinContent(i,pp->GetBinError(i));
    //hh1->SetBinError(i,pp->GetBinError(i)*0.08);
  }
}

void normalisationH1D( TH1D *hh1, TH1D *h1f1_my,Int_t color, Double_t Devidor, Double_t aligment, Int_t normID){
  Int_t i = 0;  
  Double_t maxEn = (Double_t)hh1->GetMaximum();
  for(i = 1;i<=hh1->GetNbinsX();i++){
    if(normID==0)
      hh1->SetBinContent(i,hh1->GetBinContent(i)/hh1->GetEntries()/Devidor);
    else if(normID==1){
      if(maxEn>0)
	hh1->SetBinContent(i,hh1->GetBinContent(i)/maxEn/Devidor);
      else
	assert(0);
    }
    else
      assert(0);
  }
  hh1->SetLineWidth(2);
  hh1->SetLineColor(color);
  h1f1_my->SetLineWidth(2);
  h1f1_my->SetLineColor(color);

  //find bin id which contain 0.0
  Int_t aligmentBinID;
  for(i = 1;i<=hh1->GetNbinsX();i++){
    if( aligment>=hh1->GetBinLowEdge(i)&&
	aligment<(hh1->GetBinLowEdge(i) + hh1->GetBinWidth(i)))
      aligmentBinID = i;
  }

  for(i = 1;i<=hh1->GetNbinsX();i++){
    if((i-(aligmentBinID - hh1->GetMaximumBin()))>0 && (i-(aligmentBinID - hh1->GetMaximumBin()))<=hh1->GetNbinsX())
      h1f1_my->SetBinContent( i, hh1->GetBinContent(i-(aligmentBinID - hh1->GetMaximumBin())));
  }
  
  //cout<<"h1f1_my->GetMaximumBin()  "<<hh1->GetMaximumBin()<<endl;
  
}
