//#include "iomanip.h"
Int_t getAvWF(){
  TString file1;
  file1 = "../wfSimulation/wfData/Histos_laser_first.root";
  TString histN = "gr1SP";
  TString datN = histN;
  datN +=".dat";
  TFile *f1 = new TFile(file1);
  TGraph *gr1 = (TGraph*)f1->Get(histN.Data());
  gr1->Draw("APL");
  Int_t N = gr1->GetN();
  Double_t x;
  Double_t y;
  Double_t *xx = new Double_t[N];
  Double_t *yy = new Double_t[N];
  /*
  FILE * pFile;
  pFile = fopen (datN.Data(),"w");
  cout<<" datN = "<<datN<<endl;
  fprintf (pFile, "Npionts = %d \n",N);
  fprintf (pFile, "timeMax = wqqwqwqksqlwjlqw \n");
  for(Int_t i = 0;i<N;i++){
    gr1->GetPoint( i, x, y);
    //printf (" %d   %20.5f  %20.10f \n", i, x, y);
    if(x>13.56){
      xx[i] = x;
      yy[i] = y;
    }
    else{
      xx[i] = x;
      yy[i] = 0.0;
    }
    fprintf (pFile, " %d   %20.5f  %20.10f \n", i, xx[i], yy[i]);
  }
  TGraph *gr1InFile = new TGraph(N,xx,yy);
  //gr1InFile->Draw("APL");
  fclose (pFile);
  */
  return 0;
}
