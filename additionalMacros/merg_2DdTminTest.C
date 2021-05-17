Int_t merg_2DdTminTest(){
  
  Int_t i = 0;
  
  TString file1;
  //file1 = "../../Data/2/timing_RUN4_30minnSecBin_01maxdTcut.root";
  //file1 = "../../Data/2/timing_RUN4_30minnSecBin_02maxdTcut.root";
  //file1 = "../../Data/2/timing_RUN4_30minnSecBin_05maxdTcut.root";
  //file1 = "../../Data/2/timing_RUN4_30minnSecBin_2maxdTcut.root";
  file1 = "../../Data/2/timing_RUN4_480nSecBin_02maxdTcut.root";
  //file1 = "../../Data/2/timing_RUN4_4hnSecBin_02maxdTcut.root";
  
  TFile *f1 = new TFile(file1.Data());
  
  TH2D *h2_1 = (TH2D*)f1->Get("h2MindTvsTime");
  
  Int_t N = h2_1->GetYaxis()->GetNbins();
  
  Double_t yMin = h2_1->GetYaxis()->GetBinLowEdge(1);
  Double_t yMax = h2_1->GetYaxis()->GetBinLowEdge(N) + 
    h2_1->GetYaxis()->GetBinWidth(N);
  
  TH1D *h1dTime = new TH1D("h1dTime","dTime",N,yMin,yMax);
  
  Int_t j = 13;
  printf(" %10d   %20.5f  \n", j, h2_1->GetXaxis()->GetBinLowEdge(j));

  for(i = 0;i<N;i++){
    h1dTime->SetBinContent(i+1,h2_1->GetBinContent(j,i+1));
  }

  TCanvas *c1 = new TCanvas("c1","dTmin",10,10,800,500);
  c1->SetFillColor(kWhite);
  h1dTime->Draw();

  return 0;
}
