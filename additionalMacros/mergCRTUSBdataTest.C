#include "assert.h"

Int_t mergCRTUSBdataTest(){

  Int_t i = 0;

  TString file1;
  TString file2;

  Int_t nTOTusbwc = 140993;
  
  file1 = "../../Data/2/mergedTest_RUN4_4hnSecBin_02maxdTcut.root";
  //file2 = "../../Data/2/mergedTest_RUN4_480nSecBin_02maxdTcut.root";
  //file2 = "../../Data/2/mergedTest_RUN4_30minnSecBin_01maxdTcut.root";
  //file2 = "../../Data/2/mergedTest_RUN4_30minnSecBin_02maxdTcut.root";
  //file2 = "../../Data/2/mergedTest_RUN4_30minnSecBin_05maxdTcut.root";
  file2 = "../../Data/2/mergedTest_RUN4_30minnSecBin_2maxdTcut.root";


  TFile *f1 = new TFile(file1.Data());
  TFile *f2 = new TFile(file2.Data());

  TH1D *h1_1 = (TH1D*)f1->Get("h1crtEvIDMerg");
  TH1D *h1_2 = (TH1D*)f2->Get("h1crtEvIDMerg");

  Int_t N = h1_1->GetNbinsX();

  cout<<N<<endl;

  TH1I *h1diffcrtEvIDMerg = new TH1I("h1diffcrtEvIDMerg","diff crt EvID Merg",N,0,N);

  Int_t nMer_1 = 0;
  Int_t nMer_2 = 0;
  Int_t nCRT_1 = 0;
  Int_t nCRT_2 = 0;
  Int_t ndiff = 0;
  Int_t ndiffh1_0 = 0;

  ///// difference /////
  for(i = 0;i<N;i++){
    h1diffcrtEvIDMerg->SetBinContent(i+1,(h1_1->GetBinContent(i+1) - h1_2->GetBinContent(i+1)));
  }
  //////////////////////

  h1diffcrtEvIDMerg->Draw();

  //////number of nerger events//////
  for(i = 0;i<N;i++){
    if(h1diffcrtEvIDMerg->GetBinContent(i+1)!=0.0)
      ndiff++;
    
    if(h1diffcrtEvIDMerg->GetBinContent(i+1)!=0.0 && h1_1->GetBinContent(i+1) == 0)
      ndiffh1_0++;
	
  }
  /////////////////////////////////

  //////number of nerger events//////
  for(i = 0;i<N;i++){
    if(h1_1->GetBinContent(i+1)>0.0)
      nMer_1++;
    if(h1_2->GetBinContent(i+1)>0.0)
      nMer_2++;
  }
  ///////////////////////////////////


  /// number of CRT events used 2 - times ///

  const Int_t Nc = 1000000;
  if(Nc!=N)
    assert(0);
  
  Int_t crtIDarr[Nc];
  Int_t crtID;

  for(i = 0;i<N;i++)
    crtIDarr[i] = 0;
  for(i = 0;i<N;i++){
    if(h1_1->GetBinContent(i+1)>0.0){
      crtID = (Int_t)h1_1->GetBinContent(i+1);
      if(crtID>=0 && crtID<N)
	crtIDarr[crtID]++;
    }
  }
  for(i = 0;i<N;i++){
    if(crtIDarr[i]>1)
      nCRT_1++;
  }

  for(i = 0;i<N;i++)
    crtIDarr[i] = 0;
  for(i = 0;i<N;i++){
    if(h1_2->GetBinContent(i+1)>0.0){
      crtID = (Int_t)h1_2->GetBinContent(i+1);
      if(crtID>=0 && crtID<N)
	crtIDarr[crtID]++;
    }
  }
  for(i = 0;i<N;i++){
    if(crtIDarr[i]>2)
      nCRT_2++;
  }


  ///////////////////////////////////////////
  
  cout<<"nTOTusbwc "<<nTOTusbwc<<endl
      <<"nMer_1 n    "<<(100.0*nMer_1)/nTOTusbwc<<endl
      <<"nMer_2 n    "<<(100.0*nMer_2)/nTOTusbwc<<endl
      <<"nMer_1      "<<nMer_1<<endl
      <<"nMer_2      "<<nMer_2<<endl
      <<"nCRT_1 n    "<<100.0*nCRT_1/nTOTusbwc<<endl
      <<"nCRT_2 n    "<<100.0*nCRT_2/nTOTusbwc<<endl
      <<"ndiff  n    "<<100.0*ndiff/nTOTusbwc<<endl
      <<"ndiffh1_0 n "<<100.0*ndiffh1_0/nTOTusbwc<<endl;
  
  //h1_2->SetLineColor(kRed);
  //h1_1->Draw();
  //h1_2->Draw("same");




  return 0;
}
