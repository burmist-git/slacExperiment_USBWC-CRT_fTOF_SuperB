//Int_t diffConfCompData_vs_MC(  TString histName_f1_1,
//		       TString histName_f2_1,
//		       TString histName_f2_2){
  
Int_t diffConfCompData_vs_MC_Thesis(TString histName_f1_1,
				    TString histName_f2_1,
				    TString histName_f2_2){
  
  cout<<"histName_f1_1 = "<<histName_f1_1<<endl
      <<"histName_f2_1 = "<<histName_f2_1<<endl
      <<"histName_f2_2 = "<<histName_f2_2<<endl;

  TString file1;
  TString file2;
  
  //TString histName_f1_1;
  //TString histName_f2_1;
  //TString histName_f2_2;
  
  file1 = "../../Histos/3/Histos_merged_RUN2_Thesis_RiseTime.root";
  //file1 = "../../Histos/3/Histos_merged_RUN4_recotrk.root";
  //file1 = "../../Histos/3/Histos_merged_RUN4_recotrk_100phi175theta.root";
  //file1 = "../../Histos/3/Histos_merged_RUN4_recotrk_100phi165theta.root";
  //file1 = "../../Histos/3/Histos_merged_RUN5.root";
  //file2 = "../../HistosSim/3/0045/Histos_fTOFslacCRT_QUARTZ_fTOF.root";
  //file2 = "../../HistosSim/3/014/Histos_fTOFslacCRT_QUARTZ_fTOF.root";  
  //file2 = "../../HistosSim/3/042/Histos_fTOFslacCRT_QUARTZ_fTOF.root";
  file2 = "../../HistosSim/3/07/Histos_fTOFslacCRT_QUARTZ_fTOF.root";

  //file2 = "../../HistosSim/3/07/Histos_fTOFslacCRT_QUARTZ_fTOF.root";  
  //file2 = "../../HistosSim/3/014/phiMinm100phiMax100ThetaMax175/Histos_fTOFslacCRT_QUARTZ_fTOF_01.root";
  //file2 = "../../HistosSim/3/014/phiMinm100phiMax100ThetaMax165/Histos_fTOFslacCRT_QUARTZ_fTOF_01.root";

  //L
  //0 2 4 6 8 10 12
  //histName_f1_1 = "h1dTimeL_ch2";
  //histName_f2_1 = "h1dTimeL_S_ch2";
  //histName_f2_2 = "h1dTimeL_first_ch2";
 
  //L2
  //1 3 5 9 11 13	
  //histName_f1_1 = "h1dTimeL2_ch3";
  //histName_f2_1 = "h1dTimeL2_S_ch3";
  //histName_f2_2 = "h1dTimeL2_first_ch3";

  //L3
  //2 3 10 11	  
  //histName_f1_1 = "h1dTimeL3_ch3";
  //histName_f2_1 = "h1dTimeL3_S_ch3";
  //histName_f2_2 = "h1dTimeL3_first_ch3";
  
  //L4 1 2 3 4 5 6 7
  //histName_f1_1 = "h1dTimeL4_ch2";
  //histName_f2_1 = "h1dTimeL4_S_ch2";
  //histName_f2_2 = "h1dTimeL4_first_ch2";
  
  //TtB 0 1 2 3 4 5 6
  //histName_f1_1 = "h1dTimeTOPtoBOT_ch5";
  //histName_f2_1 = "h1dTimeTOPtoBOT_S_ch5";
  //histName_f2_2 = "h1dTimeTOPtoBOT_first_ch5";

  TFile *f1 = new TFile(file1.Data());
  TFile *f2 = new TFile(file2.Data());  
  TH1D *h1_f1_1 = (TH1D*)f1->Get(histName_f1_1.Data());
  TH1D *h1_f2_1 = (TH1D*)f2->Get(histName_f2_1.Data());
  TH1D *h1_f2_2 = (TH1D*)f2->Get(histName_f2_2.Data());

  //                  hh1     color    Devidor  rebin   normMax error
  normalisationH1D( h1_f1_1, kBlack,    1.0,     2,        1,    1);
  normalisationH1D( h1_f2_1, kBlue,     1.0,     2,        1,    0);
  normalisationH1D( h1_f2_2, kRed,      1.0,     2,        1,    0);
  
  alignment( h1_f1_1, 0.0, 1);
  alignment( h1_f2_1, 0.0, 0);
  alignment( h1_f2_2, 0.0, 0);
  
  TCanvas *c1 = new TCanvas("c1","plots",10,10,600,800);
  c1->Clear();
  c1->SetFillColor(kWhite);    

  gROOT->Reset();
  gStyle->SetStatColor(kWhite);
//gStyle->SetOptStat(1111111);
  //gStyle->SetOptStat(0000010);
  gStyle->SetOptStat("e");
  
  //gStyle->SetStatStyle(11);

  h1_f1_1->SetMarkerStyle(20);
  h1_f1_1->Draw();
  h1_f1_1->GetXaxis()->SetRangeUser(-2.0,2.0);
  h1_f1_1->GetXaxis()->SetTitle("Time, ns");
  //h1_f2_1->SetMarkerStyle(20);
  //h1_f2_1->Draw();
  //h1_f2_1->GetXaxis()->SetRangeUser(-2.0,2.0);
  //h1_f1_1->Draw("same");
  h1_f2_1->Draw("same");
  h1_f2_2->Draw("same");  

  TString fileN = histName_f1_1;
  fileN +="_RUN2_0.9_1.2RaseTimeCut";
  fileN +=".eps";
  c1->Print(fileN.Data());
  
  return 0;
}

void normalisationH1D(TH1D *hh1,Int_t color, Double_t Devidor, 
		      Int_t rebin, bool normMax, bool errYes){
  if(rebin != 0)
    hh1->Rebin(rebin);
  Double_t maxEn = (Double_t)hh1->GetMaximum();
  Double_t nEn = (Double_t)hh1->GetEntries();
  for(Int_t i = 1;i<=hh1->GetNbinsX();i++){
    if(normMax==true){
      if(maxEn !=0.0){
	if(errYes ==true){
	  //hh1->SetBinError(i,hh1->GetBinError(i)/maxEn/Devidor);
	  hh1->SetBinError(i,TMath::Sqrt(hh1->GetBinContent(i))/maxEn/Devidor);
	}
	hh1->SetBinContent(i,hh1->GetBinContent(i)/maxEn/Devidor);
      }
    }
    else{
      if(nEn !=0.0){
	if(errYes ==true){
	  //hh1->SetBinError(i,hh1->GetBinError(i)/nEn/Devidor);
	  hh1->SetBinError(i,TMath::Sqrt(hh1->GetBinContent(i))/nEn/Devidor);
	}
	hh1->SetBinContent(i,hh1->GetBinContent(i)/nEn/Devidor);
      }
    }
  }
  hh1->SetLineWidth(2);
  return hh1->SetLineColor(color);
}

//old
/*
void normalisationH1D(TH1D *hh1,Int_t color, Double_t Devidor, 
		      Int_t rebin, bool normMax, bool errYes){
  if(rebin != 0)
    hh1->Rebin(rebin);
  Double_t maxEn = (Double_t)hh1->GetMaximum();
  for(Int_t i = 1;i<=hh1->GetNbinsX();i++){
    if(normMax==true){
      hh1->SetBinContent(i,hh1->GetBinContent(i)/maxEn/Devidor);
      if(errYes ==true)
	hh1->SetBinError(i,hh1->GetBinError(i)/maxEn/Devidor);
    }
    else{
      hh1->SetBinContent(i,hh1->GetBinContent(i)/hh1->GetEntries()/Devidor);
      if(errYes ==true)
	hh1->SetBinError(i,hh1->GetBinError(i)/hh1->GetEntries()/Devidor);
    }
  }
  hh1->SetLineWidth(2);
  return hh1->SetLineColor(color);
}
*/

void alignment( TH1D *hh1, Double_t centrate, bool errYes){
  Int_t binMax    = hh1->GetMaximumBin();
  Int_t binCenter = getBinID( hh1, centrate);
  TH1D *hh1copy = new TH1D();
  Int_t k = -999;
  makeCopy(hh1,hh1copy, errYes);
  Int_t nBinX = hh1->GetNbinsX();
  Double_t bC;
  Double_t bCerr;
  for(Int_t i = 1;i<=nBinX;i++){
    k = i - binCenter + binMax;
    if(k>=1 && k<=nBinX){
      if(errYes){
	bCerr = hh1copy->GetBinError(k);
	hh1->SetBinError(i,bCerr);
      }
      bC = hh1copy->GetBinContent(k);
      hh1->SetBinContent(i,bC);
    }
    else
      hh1->SetBinContent(i,0.0);
  }
}

//old
/*
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
*/

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

void makeCopy(TH1D *hh1, TH1D *hh1copy, bool errYes){
  for(Int_t i = 1;i<=hh1->GetNbinsX();i++){
    hh1copy->SetBins(i,hh1->GetBinLowEdge(i),(hh1->GetBinLowEdge(i) + hh1->GetBinWidth(i)));
    hh1copy->SetBinContent(i,hh1->GetBinContent(i));
    hh1copy->SetBinError(i,hh1->GetBinError(i));
  }
  TString hc_name = hh1->GetName();
  hc_name += "_copy";
  hh1copy->SetName(hc_name.Data());
}


/*old
void makeCopy(TH1D *hh1, TH1D *hh1copy){
  for(Int_t i = 1;i<=hh1->GetNbinsX();i++){
    hh1copy->SetBins(i,hh1->GetBinLowEdge(i),(hh1->GetBinLowEdge(i) + hh1->GetBinWidth(i)));
    hh1copy->SetBinContent(i,hh1->GetBinContent(i));
  }
  TString hc_name = hh1->GetName();
  hc_name += "_copy";
  hh1copy->SetName(hc_name.Data());
}
*/



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
