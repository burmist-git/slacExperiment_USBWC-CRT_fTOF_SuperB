Int_t merg_dTminTest(){
  
  Int_t i = 0;
  
  TString file1;
  file1 = "../../Data/2/timing_RUN4_30minnSecBin_01maxdTcut.root";
  //file1 = "../../Data/2/timing_RUN4_30minnSecBin_02maxdTcut.root";
  //file1 = "../../Data/2/timing_RUN4_30minnSecBin_05maxdTcut.root";
  //file1 = "../../Data/2/timing_RUN4_30minnSecBin_2maxdTcut.root";
  //file1 = "../../Data/2/timing_RUN4_480nSecBin_02maxdTcut.root";
  //file1 = "../../Data/2/timing_RUN4_4hnSecBin_02maxdTcut.root";
  
  TFile *f1 = new TFile(file1.Data());

  TH1D *h1_1 = (TH1D*)f1->Get("h1MindT");

  Double_t yMax =  h1_1->GetMaximum();

  Int_t N = h1_1->GetNbinsX();

  Double_t tLeft  = -0.4;
  Double_t tRight =  0.25;


  TCanvas *c1 = new TCanvas("c1","dTmin",10,10,800,500);
  c1->SetFillColor(kWhite);
  h1_1->Draw();
  TLine *lLeft = new TLine( tLeft, 0.0, tLeft, yMax);
  TLine *lRight = new TLine( tRight, 0.0, tRight, yMax);
  lLeft->SetLineColor(kRed);
  lRight->SetLineColor(kRed);
  lLeft->SetLineWidth(2.0);
  lRight->SetLineWidth(2.0);
  lLeft->Draw("same");
  lRight->Draw("same");


  Double_t binCenter;
  Int_t binContent;

  Int_t nWithinCut = 0;
  Int_t nTotCut = 0;

  for(i=0;i<N;i++){
    binCenter = h1_1->GetBinCenter(i+1);
    //cout<<binCenter<<endl;
    binContent = (Int_t)h1_1->GetBinContent(i+1); 
    if( binCenter>=tLeft && binCenter<=tRight){
      nWithinCut = nWithinCut + binContent;
    }
    nTotCut = nTotCut + binContent;
  }
  
  cout<<"nTotCut      "<<nTotCut<<endl
      <<"nWithinCut   "<<nWithinCut<<endl
      <<"nWithinCut n "<<100.0*nWithinCut/nTotCut<<" %"<<endl;

  return 0;
}
