//C, C++
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <iomanip>
#include <time.h>

//root
#include "TROOT.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TFile.h"
#include "TRandom3.h"
#include "TGraph.h"
#include <TProfile.h>

const Int_t nMcpPMmax = 10000;

void getShapeFromFile(TString fileN, 
		      double *xxMcpPM, double *yyMcpPM, 
		      int &N, double &timeMax);

Int_t dataVis(){

    double xxMcpPM_SP[nMcpPMmax];
    double yyMcpPM_SP[nMcpPMmax];
    double yyDerMcpPM_SP[nMcpPMmax];
    int nPiontsMcpPM_SP;
    double timeMax_SP;

    getShapeFromFile("./gr1SP_first.dat", 
		     xxMcpPM_SP, yyMcpPM_SP,
		     nPiontsMcpPM_SP, timeMax_SP);
    cout<<" --> nPiontsMcpPM_SP "<<nPiontsMcpPM_SP<<endl
	<<" --> timeMax_SP      "<<timeMax_SP<<endl;

  TCanvas *c1 = new TCanvas("c1","wavefor",10,10,1000,800);
  c1->SetFillColor(kWhite);

  for(Int_t i = 1;i<nPiontsMcpPM_SP;i++){
    yyDerMcpPM_SP[i-1] = (yyMcpPM_SP[i] - yyMcpPM_SP[i-1])/(xxMcpPM_SP[i] - xxMcpPM_SP[i-1]);
  }

  TGraph *gr1 = new TGraph( nPiontsMcpPM_SP, xxMcpPM_SP, yyMcpPM_SP); 
  TGraph *gr2 = new TGraph( nPiontsMcpPM_SP, xxMcpPM_SP, yyDerMcpPM_SP); 

  gStyle->SetFrameBorderMode(0);
  gROOT->ForceStyle();
  gStyle->SetOptStat(kFALSE);

  TMultiGraph *mg = new TMultiGraph("mg"," wf vs wfSp");
  mg->SetTitle("");
  mg->Add(gr1);
  mg->Add(gr2);
  mg->Draw("APL");

  TLine *ln = new TLine(14.4, 0.5, 14.4, 1.0532);
  ln->Draw();
  TLine *ln2 = new TLine(8.0, 0.5, 14.4, 0.5);
  ln2->Draw();
  //gr1->Draw("APL");

  TLegend *leg = new TLegend(0.7,0.2,0.9,0.4,"","brNDC");
  leg->AddEntry(gr1, "Signal", "l");
  leg->AddEntry(gr2, "Derivative", "l");
  leg->Draw();


  return 0;
}

void getShapeFromFile(TString fileN, 
		      double *xxMcpPM, double *yyMcpPM, 
		      int &N, double &timeMax){
  ifstream indata;
  string mot;
  int kk;
  double x;
  double y;
  indata.open(fileN.Data());
  if(!indata) { // file couldn't be opened
    cout<<" ERROR --> file : "<< fileN<<" could not be opened"<<endl;
    assert(0);
  }
  indata>>mot;
  indata>>mot;
  indata>>N;
  indata>>mot;
  indata>>mot;
  indata>>timeMax;
  if(nMcpPMmax<N){
    cout<<" ERROR --> nMcpPMmax<N"<<endl
	<<"             nMcpPMmax "<<nMcpPMmax<<endl
	<<"                     N "<<N<<endl;
    assert(0);
  }
  //xxMcpPM = new double[N];
  //yyMcpPM = new double[N];
  for(Int_t i = 0;i<N;i++){
    indata>>kk>>x>>y;
    xxMcpPM[i] = x;
    yyMcpPM[i] = y;
    //cout<<setw(10)<<kk<<setw(20)<<xxMcpPM[i]<<setw(20)<<yyMcpPM[i]<<endl;
  }
  indata.close();
}
