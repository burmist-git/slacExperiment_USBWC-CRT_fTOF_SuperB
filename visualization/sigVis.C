#define sigVis_cxx
#include "sigVis.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void sigVis::Loop(){
   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
   }

}

void sigVis::DrawToCanvas(Long64_t entry[100], TString namePS){

  Int_t i = 0;
  Int_t j = 0;

  TString nameO  = namePS+"[";
  TString nameC  = namePS+"]";

  TGraph *wfGr[16];  
  TString nameG;
  TString titleG;

  TCanvas  c11("c1"," wf ",10,10,1000,800);
  c11.Print(nameO.Data()); 
  c11.Divide(2,4);

  for(Int_t k = 0;k<100;k++){
    if(entry[k]>0){
      GetEntry((entry[k]));
      
      cout<<"entry   "<<entry[k]<<endl;
      cout<<"eventID_crt   "<<eventID_crt<<endl;
      cout<<"eventID_usb   "<<eventID_usb<<endl;
      
      for(i = 0;i<16;i++){
	wfGr[i] = new TGraph();
	nameG = "wfgr_";
	nameG += i;
	nameG += "_";
	nameG += eventID_usb;
	titleG = "wave form graph ";
	titleG += i;
	titleG += "_";
	titleG += eventID_usb;
	wfGr[i]->SetNameTitle(nameG.Data(),titleG.Data());
      }
      
      for(i = 0;i<16;i++){
	for(j = 0;j<256;j++){
	  wfGr[i]->SetPoint(j,(j*SamplingPeriod_usb/1000.0),ch_usb[i][j]);
	}
      }
            
      for(i = 0;i<8;i++){
	TMultiGraph *mg = new TMultiGraph("mg","mg");
	c11.cd(i+1);
	mg->Add(wfGr[i]);
	wfGr[i]->SetMarkerSize(0.5);
	wfGr[i]->SetMarkerStyle(20);
	wfGr[i]->Draw("APL");
	TLine *lnTime = new TLine(Time_usb[i],1.0,Time_usb[i],-1.0);
	TLine *lnAmpl = new TLine(0.0,Amplitude_usb[i],80,Amplitude_usb[i]);
	lnTime->SetLineColor(kRed);
	lnAmpl->SetLineColor(kRed);
	lnTime->Draw("same");
	lnAmpl->Draw("same");
	//wfGr_smooth[i]->Draw("APL");
	//mg->Draw("APL");
      }
      c11.Print(namePS.Data());
      
      //TCanvas *c12 = new TCanvas("c2"," wf 8 - 15",20,20,1000,800);
      //c12->Divide(2,4);
      
      for(i = 0;i<8;i++){
	TMultiGraph *mg = new TMultiGraph("mg","mg");
	//c12->cd(i+1);
	c11.cd(i+1);
	mg->Add(wfGr[i+8]);
	wfGr[i+8]->SetMarkerSize(0.5);
	wfGr[i+8]->SetMarkerStyle(20);
	wfGr[i+8]->Draw("APL same");
	TLine *lnTime = new TLine(Time_usb[i+8],1.0,Time_usb[i+8],-1.0);
	TLine *lnAmpl = new TLine(0.0,Amplitude_usb[i+8],80,Amplitude_usb[i+8]);
	lnTime->SetLineColor(kRed);
	lnAmpl->SetLineColor(kRed);
	lnTime->Draw("same");
	lnAmpl->Draw("same");
	//wfGr_smooth[i+8]->Draw("APL");
	//mg->Draw("APL");
      }
      //wfGr[0]->SaveAs("ss.C");
      c11.Print(namePS.Data());
      
      //for(i = 0;i<8;i++){
      //delete wfGr[i];
      //delete wfGr[i+8];
      //}

    }
    else{
      //cout<<"0"<<endl;
    }
  }
  c11.Print(nameC.Data());
  
}


void sigVis::Draw(Long64_t entry){

  Int_t i = 0;
  Int_t j = 0;

  TGraph *wfGr[16];  
  TString nameG;
  TString titleG;

  TCanvas *c11 = new TCanvas("c1"," wf ",10,10,1000,800);
  c11->Divide(2,4);

  GetEntry((entry));
  
  cout<<"eventID_usb   "<<eventID_usb<<endl;    
  cout<<"entry         "<<entry<<endl;    
  
  for(i = 0;i<16;i++){
    wfGr[i] = new TGraph();
    nameG = "wfgr_";
    nameG += i;
    nameG += "_";
    nameG += entry;
    titleG = "wave form graph ";
    titleG += i;
    titleG += "_";
    titleG += entry;
    wfGr[i]->SetNameTitle(nameG.Data(),titleG.Data());
  }
  
  for(i = 0;i<16;i++){
    for(j = 0;j<256;j++){
      wfGr[i]->SetPoint(j,(j*SamplingPeriod_usb/1000.0),ch_usb[i][j]);
    }
  }
  
  Double_t maxAmpl;
  Double_t minAmpl;
  
  for(i = 0;i<8;i++){
    TMultiGraph *mg = new TMultiGraph("mg","mg");
    c11->cd(i+1);
    mg->Add(wfGr[i]);
    wfGr[i]->SetMarkerSize(0.5);
    wfGr[i]->SetMarkerStyle(20);
    wfGr[i]->Draw("APL");
    // maxAmpl = wfGr[i]->GetMaximum();
    //minAmpl = wfGr[i]->GetMinimum();
    //TLine *lnTime = new TLine(Time_usb[i],1.0,Time_usb[i],-1.0);
    //TLine *lnAmpl = new TLine(0.0,Amplitude_usb[i],80,Amplitude_usb[i]);
    //lnTime->SetLineColor(kRed);
    //lnAmpl->SetLineColor(kRed);
    //lnTime->Draw("same");
    //lnAmpl->Draw("same");
    //wfGr_smooth[i]->Draw("APL");
    //mg->Draw("APL");
  }

  
  TCanvas *c12 = new TCanvas("c2"," wf 8 - 15",20,20,1000,800);
  c12->Divide(2,4);
  
  for(i = 0;i<8;i++){
    TMultiGraph *mg = new TMultiGraph("mg","mg");
    c12->cd(i+1);
    mg->Add(wfGr[i+8]);
    wfGr[i+8]->SetMarkerSize(0.5);
    wfGr[i+8]->SetMarkerStyle(20);
    wfGr[i+8]->Draw("APL same");
    //TLine *lnTime = new TLine(Time_usb[i+8],1.0,Time_usb[i+8],-1.0);
    //TLine *lnAmpl = new TLine(0.0,Amplitude_usb[i+8],80,Amplitude_usb[i+8]);
    //lnTime->SetLineColor(kRed);
    //lnAmpl->SetLineColor(kRed);
    //lnTime->Draw("same");
    //lnAmpl->Draw("same");
    //wfGr_smooth[i+8]->Draw("APL");
    //mg->Draw("APL");
  }
  //wfGr[0]->SaveAs("ss.C");
  
  
  //for(i = 0;i<8;i++){
  //delete wfGr[i];
  //delete wfGr[i+8];
  //}
  
  
}
