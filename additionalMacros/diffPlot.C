#include "assert.h"

TString file1;
TString file2;
TString file3;

file1 = "../../Histos_slacExperiment_USBWC-CRT/Hist_All.root";
//file1 = "../../Histos_slacExperiment_USBWC-CRT/Hist_mew.root";

//file2 = "./Histos/Histos_usbwc_test_All_n.root";
//file3 = "./Histos/fTOFslacCRT30000_ftof_quartz_1.5GeV_Histos.root";

//TString namePS = "./plots_diffConfComp.ps";

TFile *f1 = new TFile(file1.Data());
//TFile *f2 = new TFile(file2.Data());
//TFile *f3 = new TFile(file3.Data());


////top to bottom difference////|
//  0  1   2   3   4   5   6   7|
//------------------------------|
//  0  1   2   3   4   5   6   7|
//                              |
//  8  9  10  11  12  13  14  15|
//------------------------------|

///neighbor from one board (L)//|
//    0      1       2      3   |
//------------------------------|
//  0  1   2   3   4   5   6   7|
//                              |
//    4      5       6      7   |
//  8  9  10  11  12  13  14  15|
//------------------------------|

///neighbor from diff board (L2)|
//       0       1       2      |
//------------------------------|
//  0  1   2   3   4   5   6   7|
//                              |
//       4      5       6       |
//  8  9  10  11  12  13  14  15|
//------------------------------|

TH1D *h1f1_1 = (TH1D*)f1->Get("h1dTime_ch2");
TH1D *h1f1_2 = (TH1D*)f1->Get("h1dTime_L_ch1");
TH1D *h1f1_3 = (TH1D*)f1->Get("h1dTime_L2_ch0");

Int_t diffPlot(){

  TString  namehh1;
  TString titlehh1;
  TString   nameMy;
  TString  titleMy;

  /////////////////
  h1f1_1->Rebin(20);
  h1f1_2->Rebin(20);
  h1f1_3->Rebin(20);
  /////////////////  

  /////////////////////////////////////
  namehh1 = h1f1_1->GetName();
  titlehh1 = h1f1_1->GetTitle();
  nameMy = namehh1 + "_copy";
  titleMy = titlehh1 + " copy";
  TH1D *h1f1_my1  = new TH1D(nameMy.Data(), titleMy.Data(),h1f1_1->GetNbinsX(),
			     h1f1_1->GetBinLowEdge(1),
			     h1f1_1->GetBinLowEdge(h1f1_1->GetNbinsX()) + h1f1_1->GetBinWidth(h1f1_1->GetNbinsX()));
  namehh1 = h1f1_2->GetName();
  titlehh1 = h1f1_2->GetTitle();
  nameMy = namehh1 + "_copy";
  titleMy = titlehh1 + " copy";
  TH1D *h1f1_my2  = new TH1D(nameMy.Data(), titleMy.Data(),h1f1_2->GetNbinsX(),
			     h1f1_2->GetBinLowEdge(1),
			     h1f1_2->GetBinLowEdge(h1f1_2->GetNbinsX()) + h1f1_2->GetBinWidth(h1f1_2->GetNbinsX()));
  namehh1 = h1f1_3->GetName();
  titlehh1 = h1f1_3->GetTitle();
  nameMy = namehh1 + "_copy";
  titleMy = titlehh1 + " copy";
  TH1D *h1f1_my3  = new TH1D(nameMy.Data(), titleMy.Data(),h1f1_3->GetNbinsX(),
			     h1f1_3->GetBinLowEdge(1),
			     h1f1_3->GetBinLowEdge(h1f1_3->GetNbinsX()) + h1f1_3->GetBinWidth(h1f1_3->GetNbinsX()));
  /////////////////////////////////////
  
  normalisationH1D( h1f1_1, h1f1_my1, kBlack  , 1.0, 0.0,1);
  normalisationH1D( h1f1_2, h1f1_my2, kMagenta, 1.0, 0.0,1);
  normalisationH1D( h1f1_3, h1f1_my3, kRed,     1.0, 0.0,1);
  
  TCanvas *c1 = new TCanvas("c1"," CRT plots",10,10,600,800);
  c1->SetFillColor(kWhite);

  //h1f1_1->Draw();
  h1f1_my2->Draw();
  h1f1_my1->Draw("same");
  h1f1_my3->Draw("same");


  //TString nameO  = namePS+"[";
  //TString nameC  = namePS+"]";
  //c1->Print(nameO.Data()); 
  //c1->Clear();
  //c1->Print(namePS.Data());
  //c1->Print(nameC.Data());  

  return 0;
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
