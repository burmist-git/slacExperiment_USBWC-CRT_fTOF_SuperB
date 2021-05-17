Int_t fit2Gauss(){

  TString file1;
  
  Double_t x1Min;
  Double_t x1Max;
  Double_t x2Min;
  Double_t x2Max;
  Double_t xMin;
  Double_t xMax;
  Int_t nRebin = 0;
  
  /////////////////////////////////////////////

  /*
  file1 = "../../Histos/Histos_USBWC_OldExp.root";
  x1Min = -1.0;
  x1Max =  1.7;
  x2Min =  0.0;
  x2Max =  0.6;
  xMin  = -1.0;
  xMax  =  1.7;
  nRebin = 0;
  */

  /*
  file1 = "../../Histos/Histos_USBWC_OldExp_m2.5crossTalkThreshold.root";
  x1Min = -1.0;
  x1Max =  1.7;
  x2Min =  0.0;
  x2Max =  0.6;
  xMin  = -1.0;
  xMax  =  1.7;
  nRebin = 0;
  */

  /*
  file1 = "../../Histos/Histos_USBWC_OldExp_m2.5crossTalkThreshold_cut_risetime_drisetime.root";
  x1Min = -0.5;
  x1Max =  0.5+2*0.26;
  x2Min =  0.0;
  x2Max =  0.6;
  xMin  = -0.5;
  xMax  =  0.5+2*0.26;
  nRebin = 2;
  */

  /*
  file1 = "../../Histos/Histos_USBWC_OldExp_m5crossTalkThreshold.root";
  x1Min = -1.0;
  x1Max =  1.7;
  x2Min =  0.0;
  x2Max =  0.6;
  xMin  = -1.0;
  xMax  =  1.7;
  nRebin = 0;
 */

  /*
  file1 = "../../HistosSim/Histos_fTOFslacCRT_QUARTZ_fTOF_400MeV.root";
  x1Min = -0.5;
  x1Max =  0.5;
  x2Min = -0.2;
  x2Max =  0.2;
  xMin  = -0.5;
  xMax  =  0.5;
  nRebin = 20;
  */
  
  /*
  file1 = "../../HistosSim/Histos_fTOFslacCRT1000_1500MeV_x0_y0_thata175_phi20.root";
  x1Min = -0.8;
  x1Max =  0.8;
  x2Min = -0.2;
  x2Max =  0.2;
  xMin  = -0.9;
  xMax  =  0.9;
  nRebin = 20;
  */

  /*
  file1 = "../../HistosSim/Histos_fTOFslacCRT_QUARTZ_fTOF.root";
  x1Min = -0.9;
  x1Max =  0.9;
  x2Min = -0.2;
  x2Max =  0.2;
  xMin  = -0.9;
  xMax  =  0.9;
  nRebin = 10;
  */

  /*
    file1 = "../../HistosSim/Histos_fTOFslacCRT_QUARTZ_fTOF_CT_zero.root";
    x1Min = -1.0;
    x1Max =  1.0;
    x2Min = -0.2;
    x2Max =  0.2;
    xMin  = -1.0;
    xMax  =  1.0;
    nRebin = 10;
  */

  /*
  file1 = "../../HistosSim/Histos_fTOFslacCRT_QUARTZ_fTOF_zero_cut.root";
  x1Min = -0.9;
  x1Max =  0.9;
  x2Min = -0.2;
  x2Max =  0.2;
  xMin  = -0.9;
  xMax  =  0.9;
  nRebin = 12;
  */

  /*
  file1 = "../../HistosSim/Histos_fTOFslacCRT_QUARTZ_fTOF_zero_cut_0.1collEff.root";
  x1Min = -0.9;
  x1Max =  0.9;
  x2Min = -0.2;
  x2Max =  0.2;
  xMin  = -0.9;
  xMax  =  0.9;
  nRebin = 40;
  */

  /*
  file1 = "../../HistosSim/Histos_fTOFslacCRT_QUARTZ_fTOF_zero_cut_0.42collEff.root";
  x1Min = -0.9;
  x1Max =  0.9;
  x2Min = -0.2;
  x2Max =  0.2;
  xMin  = -0.9;
  xMax  =  0.9;
  nRebin = 10;
  */

  /*
  file1 = "../../HistosSim/Histos_fTOFslacCRT_QUARTZ_fTOF_zero_cutNewRiseTime_0.42collEff.root";
  x1Min = -0.8;
  x1Max =  0.8;
  x2Min = -0.2;
  x2Max =  0.2;
  xMin  = -0.8;
  xMax  =  0.8;
  nRebin = 20;
  */

  /*
  file1 = "../../HistosSim/Histos_fTOFslacCRT_QUARTZ_fTOF_cutNewRiseTime_0.42collEff.root";
  x1Min = -0.8;
  x1Max =  0.8;
  x2Min = -0.2;
  x2Max =  0.2;
  xMin  = -0.8;
  xMax  =  0.8;
  nRebin = 20;
  */

  /*
  file1 = "../../HistosSim/Histos_QUARTZ_fTOF_cut_absorber.root";
  x1Min = -0.7;
  x1Max =  0.7;
  x2Min = -0.2;
  x2Max =  0.2;
  xMin  = -0.7;
  xMax  =  0.7;
  nRebin = 20;
  */

  /*
  file1 = "../../HistosSim/Histos_fTOFslacCRT_QUARTZ_fTOF_NOTTS.root";
  x1Min = -0.3;
  x1Max =  0.3;
  x2Min = -0.1;
  x2Max =  0.1;
  xMin  = -0.3;
  xMax  =  0.3;
  nRebin = 4;
  */

  /*
  file1 = "../../HistosSim/Histos_fTOFslacCRT_QUARTZ_fTOF_zero_cut_0.42collEff_CS.root";
  x1Min = -0.9;
  x1Max =  0.9;
  x2Min = -0.2;
  x2Max =  0.2;
  xMin  = -0.9;
  xMax  =  0.9;
  nRebin = 40;
  */

  //file1 = "../../HistosWFsim/Histo_NsplinePoint5_CT0.4ns.root";
  //file1 = "../../HistosWFsim/Histo_NsplinePoint100_CT0.4ns.root";
  //x1Min = -0.0;
  //x1Max =  0.13;
  //x2Min = -0.01;
  //x2Max =  0.06;
  //xMin  = -0.0;
  //xMax  =  0.13;
  //nRebin = 0;

  //file1 = "./Histos/fTOFslacCRT_trigg_fTOF_1500MeV_30000_WaveformSim.root";
  //x1Min = -0.2;
  //x1Max =  0.2;
  //x2Min = -0.06;
  //x2Max =  0.06;
  //xMin  = -0.21;
  //xMax  =  0.21;
  //nRebin = 4;

  //file1 = "./Histos/fTOFslacCRT30000_ftof_quartz_1.5GeV_Histos.root";
  //x1Min = -1.0;
  //x1Max =  1.0;
  //x2Min = -0.2;
  //x2Max =  0.2;
  //xMin  = -0.5;
  //xMax  =  0.5;
  //nRebin = 0;

  //file1 = "./Histos/fTOFslacCRT_trigg_fTOF_1500MeV_30000_Histos.root";  
  //x1Min = -1.0;
  //x1Max =  1.0;
  //x2Min = -0.2;
  //x2Max =  0.2;
  //xMin  = -0.5;
  //xMax  =  0.5;
  //nRebin = 0;

  //--------------------------------------------------
  //file1 = "./Histos/12122010/fTOFslacCRT1000_1500MeV_x0_y0_thata175_phi20_Histos.root";
  //x1Min = -1.0;
  //x1Max =  1.0;
  //x2Min = -0.2;
  //x2Max =  0.2;
  //xMin  = -0.5;
  //xMax  =  0.5;
  //nRebin = 2;

  //file1 = "./Histos/12122010/fTOFslacCRT1000_1500MeV_x0_ym1.5_thata175_phi180_Histos.root";
  //x1Min = -2.0;
  //x1Max =  2.0;
  //x2Min = -0.4;
  //x2Max =  0.4;
  //xMin  = -2.0;
  //xMax  =  2.0;
  //nRebin = 6;

  //file1 = "./Histos/12122010/fTOFslacCRT1000_1500MeV_x14_y0_theta170_phi0_Histos.root";
  //x1Min = -1.0;
  //x1Max =  1.0;
  //x2Min = -0.2;
  //x2Max =  0.2;
  //xMin  = -0.5;
  //xMax  =  0.5;
  //nRebin = 0;

  //file1 = "./Histos/12122010/fTOFslacCRT1000_1500MeV_x14_y0_theta170_phi180_Histos.root";
  //x1Min = -3.0;
  //x1Max =  3.0;
  //x2Min = -0.4;
  //x2Max =  0.4;
  //xMin  = -3.0;
  //xMax  =  3.0;
  //nRebin = 10;

  //file1 = "./Histos/12122010/fTOFslacCRT1000_1500MeV_x14_y0_theta170_phi90_Histos.root";
  //x1Min = -0.2;
  //x1Max =  0.2;
  //x2Min = -0.2;
  //x2Max =  0.2;
  //xMin  = -1.0;
  //xMax  =  1.0;
  //nRebin = 0;

  //file1 = "./Histos/12122010/fTOFslacCRT1000_1500MeV_x14_y0_theta180_phi180_Histos.root";
  //x1Min = -0.2;
  //x1Max =  0.2;
  //x2Min = -0.2;
  //x2Max =  0.2;
  //xMin  = -1.0;
  //xMax  =  1.0;
  //nRebin = 0;

  //file1 = "./Histos/12122010/fTOFslacCRT1000_1500MeV_x8_y0_theta180_phi0_Histos.root";
  //x1Min = -0.4;
  //x1Max =  0.4;
  //x2Min = -0.2;
  //x2Max =  0.2;
  //xMin  = -0.5;
  //xMax  =  0.5;
  //nRebin = 2;

  //file1 = "./Histos/12122010/fTOFslacCRT1000_1500MeV_x8_y1.5_theta180_phi0_Histos.root";
  //x1Min = -0.4;
  //x1Max =  0.4;
  //x2Min = -0.2;
  //x2Max =  0.2;
  //xMin  = -0.5;
  //xMax  =  0.5;
  //nRebin = 0;

  //file1 = "./Histos/12122010/fTOFslacCRT4000_1500MeV_x8_y1.5_theta175_phim10_Histos.root";
  //x1Min = -0.4;
  //x1Max =  0.4;
  //x2Min = -0.2;
  //x2Max =  0.2;
  //xMin  = -0.5;
  //xMax  =  0.5;
  //nRebin = 0;

  //file1 = "./Histos/Histos_usbwc_test_All_n.root";
  //x1Min = -0.6;
  //x1Max =  1.1;
  //x2Min =  0.15;
  //x2Max =  0.4;
  //xMin  = -0.6;
  //xMax  =  1.2;
  //nRebin = 0;

  /*
  file1 = "../ana_ftofProt/Histos_RUN3pp.root";
  x1Min = -0.3;
  x1Max =  0.8;
  x2Min =  0.15;
  x2Max =  0.4;
  xMin  = -0.3;
  xMax  =  0.8;
  nRebin = 2;
  */

  /*  
  file1 = "../ana_ftofProt/Histos_RUN4.root";
  x1Min = -0.43;
  x1Max =  0.97;
  x2Min =  0.15;
  x2Max =  0.4;
  xMin  = -0.43;
  xMax  =  0.97;
  nRebin = 2;
  */  

  /*
  file1 = "../ana_ftofProt/Histos_RUN4_4m480nSecBin_02maxdTcut_merged.root";
  x1Min = -0.43;
  x1Max =  0.97;
  x2Min =  0.15;
  x2Max =  0.4;
  xMin  = -0.43;
  xMax  =  0.97;
  nRebin = 2;
  */  

  /*
  file1 = "../ana_ftofProt/Histos_RUN4_4m480nSecBin_02maxdTcut.root";
  x1Min = -0.43;
  x1Max =  0.97;
  x2Min =  0.15;
  x2Max =  0.4;
  xMin  = -0.43;
  xMax  =  0.97;
  nRebin = 2;
*/

  /*
  file1 = "../../Histos/2/Histos_merged_RUN4_4hnSecBin_02maxdTcut.root";
  x1Min =  0.18-1.0;
  x1Max =  0.18+1.0;
  x2Min =  0.18-0.3;
  x2Max =  0.18+0.3;
  xMin  =  0.18-1.0;
  xMax  =  0.18+1.0;
  nRebin = 2;
  */

  /*  
  file1 = "../../HistosSim/Histos_fTOFslacCRT_QUARTZ_fTOF.root";
  x1Min =  0.0-1.0;
  x1Max =  0.0+1.0;
  x2Min =  0.0-0.3;
  x2Max =  0.0+0.3;
  xMin  =  0.0-1.0;
  xMax  =  0.0+1.0;
  nRebin = 2;
  */  

  file1 = "../../HistosSim/2/Histos_fTOFslacCRT_QUARTZ_fTOF.root";
  x1Min =  0.0-1.0;
  x1Max =  0.0+1.0;
  x2Min =  0.0-0.3;
  x2Max =  0.0+0.3;
  xMin  =  0.0-1.0;
  xMax  =  0.0+1.0;
  nRebin = 0;
  /////////////////////////////////////////////

  TFile *f1 = new TFile(file1.Data());
  //TH1D *h1dTime = (TH1D*)f1->Get("h1dtime_ch1");
  //TH1D *h1dTime = (TH1D*)f1->Get("h1dTime_L_ch1");
  //TH1D *h1dTime = (TH1D*)f1->Get("h1dtimeWF_ch2ch3");
  //TH1D *h1dTime = (TH1D*)f1->Get("h1dtimeFirst_ch2ch3");
  //TH1D *h1dTime = (TH1D*)f1->Get("h1dTime");
  //TH1D *h1dTime = (TH1D*)f1->Get("h1dTimeL_ch2");
  //LB 11.02.2011
  //h1dTimeBoard_ch3 -- simulation
  //h1dTimeBoardFIRST_ch3 -- simulation
  //TH1D *h1dTime = (TH1D*)f1->Get("h1dTimeBoard_ch3");
  //TH1D *h1dTime = (TH1D*)f1->Get("h1dTimeBoardFIRST_ch3");
  //TH1D *h1dTime = (TH1D*)f1->Get("h1dTimeFirstPos");
  //TH1D *h1dTime = (TH1D*)f1->Get("h1dTime_L_ch2");  
  //LB 11.02.2011
  //h1dTime_L_CUT_ch1 -- data
  //TH1D *h1dTime = (TH1D*)f1->Get("h1dTime_L_CUT_ch1");
  //TH1D *h1dTime = (TH1D*)f1->Get("h1dTimeTOPtoBOT_ch3");
  //TH1D *h1dTime = (TH1D*)f1->Get("h1dTimeL2_ch3");
  //TH1D *h1dTime = (TH1D*)f1->Get("h1dTimeL3_ch3");
  TH1D *h1dTime = (TH1D*)f1->Get("h1dTimeL3_S_ch2");

  if(nRebin>0)
    h1dTime->Rebin(nRebin);

  Int_t i = 0;
  const Int_t N = 6;
  Double_t par[N];
  Double_t parFit[N];
  for(i = 0;i<N;i++){
    par[i] = 0.0;
    parFit[i] = 0.0;
  }  

  TCanvas *c1 = new TCanvas("c1","first fit",10,10,600,800);
  
  TF1 *g1 = new TF1("g1","gaus", xMin, xMax);
  TF1 *g1x = new TF1("g1x","gaus", xMin, xMax);
  h1dTime->Fit(g1,"","", x1Min, x1Max);
  g1->GetParameters(&par[0]);
  g1x->SetParameters(par[0],par[1],par[2]);
  TF1 *g2 = new TF1("g2","gaus", xMin, xMax);
  TF1 *g2x = new TF1("g2x","gaus", xMin, xMax);
  h1dTime->Fit(g2,"","", x2Min, x2Max);
  g2->GetParameters(&par[3]);
  g2x->SetParameters(par[3],par[4],par[5]);

  for(i = 0;i<N;i++){
    cout<<"par["<<i<<"]="<<par[i]<<endl;
  }

  h1dTime->Draw("");
  g1x->SetLineStyle(kDashed);
  g2x->SetLineStyle(kDashed);
  g1x->SetLineColor(kGreen);
  g2x->SetLineColor(kMagenta);
  g1x->Draw("same");
  g2x->Draw("same");

  TCanvas *c2 = new TCanvas("c2","total fit",20,20,600,800);
   
  TF1 *g12 = new TF1("g12","gaus", xMin,xMax);
  TF1 *g22 = new TF1("g22","gaus", xMin,xMax);  
  TF1 *tripleG = new TF1("tripleG","gaus(0)+gaus(3)");
  tripleG->SetParameters(par); 
  h1dTime->Fit(tripleG,"","", xMin, xMax);
  tripleG->GetParameters(&parFit[0]);
  g12->SetParameters(parFit[0],parFit[1],parFit[2]);
  g22->SetParameters(parFit[3],parFit[4],parFit[5]);
  g12->SetLineStyle(kDashed);
  g22->SetLineStyle(kDashed);
  g12->SetLineColor(kGreen);
  g22->SetLineColor(kMagenta);
  h1dTime->Draw("");
  g12->Draw("same");
  g22->Draw("same");

  cout<<"Sigma wide   "<<parFit[2]*1000.0<<" ps"<<endl
      <<"Sigma narrow "<<parFit[5]*1000.0<<" ps"<<endl;
  
  cout<<" Integral wide   "<<g12->Integral( xMin, xMax)<<endl
      <<" Integral narrow "<<g22->Integral( xMin, xMax)<<endl;

  if((g12->Integral( xMin, xMax) + g22->Integral( xMin, xMax))!=0.0){
    cout<<" Fraction wide   "<<g12->Integral( xMin, xMax)/(g12->Integral( xMin, xMax) + g22->Integral( xMin, xMax))*100<<" %"<<endl
	<<" Fraction narrow "<<g22->Integral( xMin, xMax)/(g12->Integral( xMin, xMax) + g22->Integral( xMin, xMax))*100<<" %"<<endl;
  }


  return 0;
}
