//root
#include <TH1D.h>
#include <TStyle.h>
#include <TString.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>

//C, C++
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

void convBunchDataCRT2root(TString dataFileList, 
			   TString datPreff, 
			   TString rootFileN);


int main(int argc, char *argv[]){

  // Snippet commented out to allow the code to run as a cronjob
  /*
  char* hostName = getenv( "HOSTNAME" );
  if( 0 != strcmp( "klong", hostName ) ) {
    cout << endl << "PROBLEM: you're currently using the " << hostName 
	 << " machine." << endl;
    cout << "This code must be run on the *** klong *** machine to access the CRT data!"
	 << endl;
    cout << "=> Login to klong first and then rerun this executable from there" 
	 << endl << endl;
    exit( -1 );
  }
  */

  if(argc==4){

    TString dataFileList = argv[1];
    TString datPreff = argv[2];
    TString rootFileN = argv[3];

    convBunchDataCRT2root(dataFileList, 
			  datPreff, 
			  rootFileN);
    
    //cout<<dataFileList<<endl
    //<<datPreff<<endl
    //<<rootPreff<<endl;
    
  }
  else{
    cout<<"  ERROR ---> in input arguments "<<endl
	<<"             [1] : name of a text file containing the list of dst2 files to be converted to ROOT"<<endl
	<<"             [2] : path of the directory containing the input dst2 files"<<endl
	<<"             [3] : name of the output ROOT file"<<endl;
  }

  return 0;
}


void convBunchDataCRT2root(TString dataFileList, 
			   TString datPreff, 
			   TString rootFileN){


  ////////////////////// CRT data in one ev///////////////////////////
  Int_t i = 0;

  //Int_t time;
  Double_t time;

  const Int_t ntdc_1_3 = 8;
  Int_t tdc1[ntdc_1_3], tdc2[ntdc_1_3], tdc3[ntdc_1_3];

  const Int_t nadc_1_2 = 12;
  Int_t adc1[nadc_1_2], adc2[nadc_1_2];

  //Int_t Nstack[5];
  //Int_t Istack;

  const Int_t nscl1 = 4;
  Int_t scl1[nscl1];

  const Int_t ntdc_4_9 = 32;
  Int_t tdc4[ntdc_4_9], tdc5[ntdc_4_9], tdc6[ntdc_4_9];
  Int_t tdc7[ntdc_4_9], tdc8[ntdc_4_9], tdc9[ntdc_4_9];

  const Int_t nscl_2_3 = 12;
  Int_t scl2[nscl_2_3], scl3[nscl_2_3];

  const Int_t ntdc_10_11 = 16;
  Int_t tdc10[ntdc_10_11], tdc11[ntdc_10_11];

  const Int_t nadc3 = 12;
  Int_t adc3[nadc3];

  const Int_t ntdc12 = 8;
  Int_t tdc12[ntdc12];
  ////////////////////////////////////////////////////

  ////////////////////FDIRC data in one ev///////////////////////////
  Int_t islot = 0;
  Int_t iblab = 0;
  Int_t iRowCol = 0;
  Int_t iAdcTdcFdirc = 0;
  Int_t iScaler = 0;
  Int_t iplaceholder = 0;

  const Int_t nFiberOp = 7;
  const Int_t nBLAB2 = 4;
  const Int_t nRowCol = 16;
  const Int_t nScaler = 16;
  const Int_t nplaceholder = 8;
  const Int_t nAdcTdcFdirc = 16;
  Float_t temp[nFiberOp][nBLAB2], rovdd[nFiberOp][nBLAB2];
  Int_t row[nFiberOp][nBLAB2][nRowCol], col[nFiberOp][nBLAB2][nRowCol]; 
  Int_t rowref[nFiberOp][nBLAB2], colref[nFiberOp][nBLAB2], scalerref[nFiberOp][nBLAB2];
  Int_t scaler[nFiberOp][nBLAB2][nScaler];
  Int_t placeholder1[nFiberOp][nBLAB2][nplaceholder];
  Int_t placeholder2[nFiberOp][nBLAB2][nplaceholder];
  Float_t adc_fdirc[nFiberOp][nBLAB2][nAdcTdcFdirc], tdc_fdirc[nFiberOp][nBLAB2][nAdcTdcFdirc];
  ////////////////////////////////////////////////////


  ///////////////////Root file with data/////////////////
  TFile *hfile = new TFile( rootFileN, "RECREATE", "CRT Data processed by convertCRT2root", 1);
  if (hfile->IsZombie()) {
    cout << "PROBLEM with the initialization of the output ROOT ntuple " 
	 << rootFileN << ": check that the path is correct!!!"
	 << endl;
    exit(-1);
  }
  TTree *tree = new TTree("T", "CRT Data Tree");
  //hfile->SetCompressionLevel(2);
  tree->SetAutoSave(1000000);
  // Create new event
  TTree::SetBranchStyle(0);

  //Event CRT
  tree->Branch("time_CRT",&time,  "time/D");
  tree->Branch("tdc1_CRT", tdc1,  "tdc1[8]/I");
  tree->Branch("tdc2_CRT", tdc2,  "tdc2[8]/I");
  tree->Branch("tdc3_CRT", tdc3,  "tdc3[8]/I");
  tree->Branch("adc1_CRT", adc1,  "adc1[12]/I");
  tree->Branch("adc2_CRT", adc2,  "adc2[12]/I");
  tree->Branch("scl1_CRT", scl1,  "scl1[4]/I");
  tree->Branch("tdc4_CRT", tdc4,  "tdc4[32]/I");
  tree->Branch("tdc5_CRT", tdc5,  "tdc5[32]/I");
  tree->Branch("tdc6_CRT", tdc6,  "tdc6[32]/I");
  tree->Branch("tdc7_CRT", tdc7,  "tdc7[32]/I");
  tree->Branch("tdc8_CRT", tdc8,  "tdc8[32]/I");
  tree->Branch("tdc9_CRT", tdc9,  "tdc9[32]/I");
  tree->Branch("scl2_CRT", scl2,  "scl2[12]/I");
  tree->Branch("scl3_CRT", scl3,  "scl3[12]/I");
  tree->Branch("tdc10_CRT", tdc10, "tdc10[16]/I");
  tree->Branch("tdc11_CRT", tdc11, "tdc11[16]/I");
  tree->Branch("adc3_CRT",  adc3,  "adc3[12]/I");
  tree->Branch("tdc12_CRT", tdc12, "tdc12[8]/I");
  //Event fDIRC
  tree->Branch("temp_fDIRC",     temp, "temp[7][4]/F");
  tree->Branch("rovdd_fDIRC",   rovdd, "rovdd[7][4]/F");
  tree->Branch("row_fDIRC",       row, "row[7][4][16]/I");
  tree->Branch("col_fDIRC",       col, "col[7][4][16]/I");
  tree->Branch("rowref_fDIRC", rowref, "rowref[7][4]/I");
  tree->Branch("colref_fDIRC", colref, "colref[7][4]/I");
  tree->Branch("scaler_fDIRC", scaler, "scaler[7][4][16]/I");
  tree->Branch("scalerref_fDIRC", scalerref, "scalerref[7][4]/I");
  tree->Branch("placeholder1_fDIRC", placeholder1, "placeholder1[7][4][8]/I");
  tree->Branch("placeholder2_fDIRC", placeholder2, "placeholder2[7][4][8]/I");
  tree->Branch("adc_fdirc_fDIRC", adc_fdirc, "adc_fdirc[7][4][16]/F");
  tree->Branch("tdc_fdirc_fDIRC", tdc_fdirc, "tdc_fdirc[7][4][16]/F");
  ///////////////////////////////////////////////////////

  string mot;
  ifstream indata;
  //int nPoints = 0;

  //float xx;
  //float yy;

  Int_t nEv = 0;
  Int_t nEv_Tot = 0;
  Int_t dummyInt = 0;

  Double_t timeNpevEv = 0;

  ifstream indataList;
  string dataFileName;
  TString datF;
  indataList.open(dataFileList.Data()); 
  assert(indataList.is_open());  
  while (indataList  >> dataFileName ){
    //cout<<dataFileName<<endl;
    if(indataList.eof()){
      std::cout<<"EOF"<<std::endl;
      break;
    }
    datF = datPreff + "/" + dataFileName;
    cout<<"\t * Converting ---> "<<datF<<endl;
    indata.open(datF.Data()); 
    assert(indata.is_open());
    nEv = 0; // Event counter for a given file
    while (indata  >> mot ){
      //cout<<mot<<endl;
      nEv++;
      //if(nEv%1000==0)
      //cout<<"nEv = "<<nEv<<endl;
      
      //if(nEv>1000){
      //break;
      //}
      
      time = atof(mot.c_str());
      
      if(1200000000>time){
	cout<<endl<<"FORMAT ERROR"<<endl;
	assert(0);
      }      
      
      if((time - timeNpevEv)<0.0){
	cout<<endl<<" ERROR ---> "<<" files in data list are not ordered in time "<<endl
	    <<datF<<endl;
	printf("time      : %20.4f \n", time);
	printf("timeNpevEv: %20.4f \n", timeNpevEv);
	
	assert(0);
      }
      
      timeNpevEv = time;
      
      //cout<<time<<endl;
      
      for(i = 0;i<ntdc_1_3;i++)
	indata>>tdc1[i];
      for(i = 0;i<ntdc_1_3;i++)
	indata>>tdc2[i];
      for(i = 0;i<ntdc_1_3;i++)
	indata>>tdc3[i];
      
      for(i = 0;i<nadc_1_2;i++)
	indata>>adc1[i];
      for(i = 0;i<nadc_1_2;i++)
	indata>>adc2[i];
      
      for(i = 0;i<nscl1;i++)
	indata>>scl1[i];
      
      for(i = 0;i<ntdc_4_9;i++)
	indata>>tdc4[i];
      for(i = 0;i<ntdc_4_9;i++)
	indata>>tdc5[i];
      for(i = 0;i<ntdc_4_9;i++)
	indata>>tdc6[i];
      for(i = 0;i<ntdc_4_9;i++)
	indata>>tdc7[i];
      for(i = 0;i<ntdc_4_9;i++)
	indata>>tdc8[i];
      for(i = 0;i<ntdc_4_9;i++)
	indata>>tdc9[i];
      
      for(i = 0;i<nscl_2_3;i++)
	indata>>scl2[i];
      for(i = 0;i<nscl_2_3;i++)
	indata>>scl3[i];
      
      for(i = 0;i<ntdc_10_11;i++)
	indata>>tdc10[i];
      for(i = 0;i<ntdc_10_11;i++)
	indata>>tdc11[i];
      
      for(i = 0;i<nadc3;i++)
	indata>>adc3[i];
      
      for(i = 0;i<ntdc12;i++)
	indata>>tdc12[i];
      
      //for(i = 0;i<ntdc_4_9;i++)
      //cout<<" i "<<i <<" "<<tdc5[i]<<endl;
      //for(i = 0;i<ntdc_4_9;i++)
      //cout<<" i "<<i <<" "<<tdc9[i]<<endl;    
      
      for(i = 0;i<16;i++)
	indata>>dummyInt;
      
      for(i = 0;i<16;i++)
	indata>>dummyInt;
      
      /////////////////////////////////////////////
      //***********************************************************************************
      //***********************************************************************************
      //		FDIRC PROTOTYPE
      //
      // New proposal for DST2 file format as of (April 9, 2009):
      // 44 lines: headers & normal CAMAC data
      // (224 lines BLAB2 data in the following format)
      // (7 x fiberoptic ) { (4 x BLAB2) [ BLAB_TEMP(in degrees C) BLAB_ROVDD(in V)
      //   {             [ ROW0 ROW1 ROW2 ... ROW15 ROW_REF (sampling start rows)
      //   {             [ COL0 COL1 COL2 ... COL15 COL_REF (sampling start columns)
      //   {             [ Q_0 Q_1 ... Q_15 (charges in pC)
      //   {             [ T_0 T_1 ... T_15 (times in ns)
      //   {             [ SCALER_0 SCALER_1 ... SCALER_15 SCALER_REF (scaler values in hits/minute)
      //   {             [ PLACEHOLDER_0 PLACEHOLDER_1 ... PLACEHOLDER_7 (8 zero values, for possible use later)
      //   {             [ PLACEHOLDER_0 PLACEHOLDER_1 ... PLACEHOLDER_7 (8 more zero values, for possible use later)
      //
      // Previous DST2 file format is as follows (allowed for potentially multiple hits per channel):
      //
      // 44 lines: normal CAMAC data
      // (7 x fiberoptic ) { (4 x BLAB2) [ BLAB_TEMP(in degrees C) BLAB_ROVDD(in V)
      //   {             [ (16 x channels) HIT0_Q HIT1_Q ... HIT9_Q (charges in pC)
      //   {             [ (16 x channels) HIT0_T HIT1_T ... HIT9_T (times in ns)
      //
      // In both cases (new and old):
      //
      // Fiberoptic modules that are not connected will give zeroes for all Q,T fields. 
      // At the moment, only one hit is being collected per channel, so hits 1-9 will be 0.
      //***********************************************************************************
      //***********************************************************************************
      /////////////////////////////////////////////
      
      for(islot = 0;islot<nFiberOp;islot++){
	for(iblab = 0;iblab<nBLAB2;iblab++){
	  indata>>temp[islot][iblab];
	  indata>>rovdd[islot][iblab];
	  
	  for(iRowCol = 0;iRowCol<nRowCol;iRowCol++)
	    { indata>>row[islot][iblab][iRowCol]; }
	  indata>>rowref[islot][iblab];
	  
	  for(iRowCol = 0;iRowCol<nRowCol;iRowCol++)
	    { indata>>col[islot][iblab][iRowCol]; }
	  indata>>colref[islot][iblab];
	  
	  for(iAdcTdcFdirc = 0;iAdcTdcFdirc<nAdcTdcFdirc;iAdcTdcFdirc++)
	    indata>>adc_fdirc[islot][iblab][iAdcTdcFdirc];
	  
	  for(iAdcTdcFdirc = 0;iAdcTdcFdirc<nAdcTdcFdirc;iAdcTdcFdirc++)
	    indata>>tdc_fdirc[islot][iblab][iAdcTdcFdirc];

	  for(iScaler = 0;iScaler<nScaler;iScaler++)
	    { indata>>scaler[islot][iblab][iScaler]; }
	  indata>>scalerref[islot][iblab];
	  
	  for(iplaceholder = 0;iplaceholder<nplaceholder;iplaceholder++)
	    indata>>placeholder1[islot][iblab][iplaceholder];
	  
	  for(iplaceholder = 0;iplaceholder<nplaceholder;iplaceholder++)
	    indata>>placeholder2[islot][iblab][iplaceholder];
	  
	} // BLAB2 chip loop (iblab)
      } // Optic fiber loop (islot)
      
      
      //////////Fill tree/////////////
      tree->Fill();
      ///////////////////////    
      
      //for(iplaceholder = 0;iplaceholder<nplaceholder;iplaceholder++)
      //cout<<placeholder2[6][3][iplaceholder]<<endl;    
      //for(islot = 0;islot<nFiberOp;islot++){
      //for(iblab = 0;iblab<nBLAB2;iblab++){    
      //for(iScaler = 0;iScaler<nScaler;iScaler++){
      //cout<<scaler[6][3][iScaler]<<endl;
      //}
      //}
      //}
      //return;
      //break;
    } // Closing the loop which reads the contents of file indata
    indata.close();
    nEv_Tot += nEv; // Add the number of events read in file indata
    cout << "\t   " << nEv << " events added to the ROOT tree" << endl;
    // A DST2 file should contain 1000 events
    if( nEv < 1000 ) {
      cout << "\t  !!! This file contains less than 1000 events => it may not be complete !!!" << endl;
      TString suspectFileCmd = 
	"echo \"" + datF + "\" >> crtFilesWithLessThan1000Events.txt";
      cout << "Executing command " << suspectFileCmd << endl;
      system( suspectFileCmd.Data() );
    }
  } 
  hfile = tree->GetCurrentFile();
  hfile->Write();
  indataList.close();

  hfile->Close();

  // Make the output ROOT file rw by the group
  TString cmd = "chmod g+rw " + rootFileN;
  //cout << "Executing command " << cmd << endl;
  system( cmd.Data() );
  
  cout<<"============================"<<endl;
  cout<<"Total number of events read:"<<endl;
  cout<<"nEv_Tot = "<<nEv_Tot<<endl;
  cout<<"============================"<<endl;
}

/*
Example of one event (first event in /u3/CRT_DST/exp3/run16/exp3_run16_00000.dst2)

1285013917.888                                      // Unix event time
547 245 184 381 429 419 226 1074                    // TDC1 [8 values]
362 285 230 476 4088 350 259 464                    // TDC2 [8 values]
403 357 376 436 767 259 268 153                     // TDC3 [8 values]
399 185 202 233 1 93 127 366 75 330 
674 1997                                            // ADC1 [10+2 = 12 values]
351 138 759 231 51 793 367 41 20 298 
206 51                                              // ADC2 [10+2=12 values]
0 0 0 0                                             // SCL1 [4 values]
0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 
0 0                                                 // TDC4 [10+10+10+2=32 values]
0 43 43 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 
0 0                                                 // TDC5 [10+10+10+2=32 values]
0 0 0 43 42 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 
0 0                                                 // TDC6 [10+10+10+2=32 values]
0 0 0 0 0 30 31 0 0 0 
0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 55830 0 
0 0                                                 // TDC7 [10+10+10+2=32 values]
0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 64400 0 0 0 0 
0 0 0 0 0 0 0 30061 39476 0 
0 0                                                 // TDC8 [10+10+10+2=32 values]
0 0 37562 0 0 0 0 0 0 0 
0 40 40 0 29549 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 
0 0                                                 // TDC9 [10+10+10+2=32 values]
2987 6429 0 0 17193 12937 13038 17528 2092214 25191 
587780 17070                                        // SCL2 [10+2=12 values]
0 53628 12776551 15514 1 0 0 0 0 0 
0 0                                                 // SCL3 [10+2=12 values]
928 777 905 1130 4095 4095 4095 4095 4095 4095 
4095 4095 4095 4095 4095 824                        // TDC10 [10+6=16 values]
42 11 70 56 1 1 1 2 10 8 
1 1 1 8 1 10                                        // TDC11 [10+6=16 values]
0 0 0 0 0 0 0 0 0 0 
0 0                                                 // ADC3 [10+2=12 values]
0 0 0 0 0 0 0 0                                     // TDC12 [8 values]
4095 4095 4095 4095 4095 4095 4095 4095 4095 4095 
4095 4095 4095 4095 4095 4095                       // 10+6 = 16 dummy integer values
8 6 16 15 1 3 1 18 21 1 
9 1 45 22 1 22                                      // 10+6 = 16 dummy integer values

// From now on a recurring 8-line pattern coming
// 7 (# of optic fibers, outer loop) * 4 (# of BLAB2 chips, inner loop) = 28 times:
// * Line 1: 2 floats (temp and rovdd)
// * Line 2: 16 integers (row) + 1 integer (rowref)
// * Line 3: 16 integers (col) + 1 integer (colref)
// * Line 4: 16 floats (adc_fdirc)
// * Line 5: 16 floats (tdc_fdirc)
// * Line 6: 16 integers (scaler) + 1 scaler (scalerref)
// * Line 7: 8 integers (placeholder1)
// * Line 8: 8 integers (placeholder2)

0.0000 0.0000
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
0.0000 0.0000
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
0.0000 0.0000
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
0.0000 0.0000
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
32.6250 2.5543
1 3 3 1 2 3 2 3 3 4 2 3 1 4 3 4 3 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
14292 15031 0 22332 18380 21295 13882 20110 11795 17247 11893 13390 8225 9897 7900 6589 9 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
34.1875 2.5639
3 1 3 2 2 5 0 3 4 3 2 1 4 1 3 1 3 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
13716 18294 20952 26213 11912 21054 10880 19038 8835 12136 7011 10434 7489 9926 11420 13456 9 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
34.7812 2.5405
4 1 2 1 1 2 4 2 1 4 3 1 2 2 4 1 3 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
6156 9778 11650 12170 6272 13601 7695 6768 5942 7945 5761 4225 4240 3646 3814 4280 9 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
33.8438 2.5828
4 3 1 2 4 4 3 3 2 3 5 2 4 3 1 4 3 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
306 7399 14418 17882 10768 11972 10709 9826 7993 9595 8584 5726 7233 5711 7438 4753 9 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
30.2812 2.5422
3 2 4 3 2 1 3 3 3 4 3 4 4 3 3 3 3 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
6201 6580 11659 10149 11865 11251 10078 9246 7614 6528 5307 5410 5100 4250 5094 4897 9 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
30.3750 2.5675
0 2 1 4 2 3 2 0 4 1 2 2 1 4 3 0 3 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
6156 6341 13506 15390 13512 10844 10022 9889 8030 9684 9263 8307 7928 7673 7050 6913 9 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
30.8438 2.5262
3 0 4 2 3 1 3 4 2 2 4 3 4 5 3 4 3 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
3279 3713 8459 11161 8102 9765 5823 6786 5907 6082 5502 7158 5128 6164 4221 3956 9 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
29.8750 2.5615
2 4 4 1 5 3 3 2 3 2 1 2 2 3 2 3 3 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
2711 3210 5553 8465 4727 9823 5370 8496 5348 6985 4451 6162 4960 5591 5419 6635 9 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
30.9375 2.4935
0 3 3 3 4 4 1 3 3 0 3 1 1 1 1 4 2 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
4116 4700 7965 6778 7954 5647 6778 6942 6028 6247 7240 6765 4786 7187 6861 4446 6 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
31.8750 2.5825
4 4 3 3 2 2 1 2 4 1 3 4 3 3 4 1 2 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
5940 6299 10701 9607 9299 8754 9144 9442 7253 6106 8139 5675 8620 6230 8327 8251 6 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
31.7500 2.5116
1 2 2 2 4 5 2 2 4 3 3 3 3 4 3 4 2 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
4269 5634 8299 4975 8845 6807 5570 6917 4730 5537 4452 5768 5311 4687 4580 6355 6 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
30.6250 2.5308
2 3 3 3 3 3 4 2 4 1 4 3 3 2 2 3 2 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
31 4431 5434 7616 6923 4350 6385 4223 7307 5908 5548 3497 6136 4078 6106 4464 6 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
31.0312 2.5298
0 2 3 1 2 1 1 1 5 4 4 3 5 2 0 3 2 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
6127 5284 10846 13079 8898 14250 11241 12506 10603 13306 9154 11045 9574 10897 10947 10895 6 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
31.4688 2.5330
1 1 1 4 4 1 2 1 1 2 2 2 1 3 5 4 2 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
6129 5399 9899 11341 8581 10498 10350 5952 9809 7665 8453 7459 8938 9345 8243 7981 6 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
32.9062 2.4469
4 2 3 3 1 2 3 2 1 4 3 3 4 0 2 4 2 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
3496 7661 7324 9995 8624 9695 7813 4901 8337 8440 7474 5855 6936 7995 7446 6942 6 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
30.9375 2.6177
1 3 3 1 3 1 1 4 1 1 3 2 4 1 3 5 2 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
12049 9870 12275 12838 15559 11609 12413 12393 11318 11109 12082 10597 9372 6996 8686 9415 6 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
33.8750 2.5963
1 4 3 3 1 0 0 3 1 3 3 4 2 4 2 3 3 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
5515 6453 14860 15162 12863 12104 14595 16262 13263 13145 11394 11095 8953 11218 11923 12511 9 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
35.7188 2.6166
1 3 1 4 1 1 3 2 0 1 1 3 4 4 2 5 3 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
11204 10233 13240 16430 12742 12523 12000 14835 12999 9561 11492 11560 12116 11249 19817 14132 9 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
35.8750 2.5622
1 4 5 5 2 1 3 1 2 5 4 2 3 4 4 3 3 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
9448 8029 11848 15047 22130 12585 17302 11728 16014 11414 16665 10965 13905 14541 16718 14047 9 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
34.1250 2.6156
0 1 2 4 2 3 4 1 3 4 4 4 3 2 3 4 3 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0 13846 15596 8298 19037 16127 22936 19515 21865 21799 27817 16916 17464 8032 19012 12675 9 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
33.6875 2.5721
3 2 4 2 4 1 1 4 4 5 2 1 3 2 2 4 3 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
19221 25177 22234 24018 23166 29857 28376 24212 24533 28259 29438 17046 19613 19625 19969 21123 6 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
35.3125 2.5472
3 3 2 1 4 1 5 4 1 4 2 2 3 4 1 1 3 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
22664 25302 29605 29374 35254 27506 29216 34353 28589 28858 26318 26651 26404 23743 26690 24780 6 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
35.3438 2.5558
1 2 1 2 2 4 1 2 3 3 3 2 4 4 3 3 2 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
21569 26767 22083 27982 23968 27960 29394 24981 26384 32203 25451 27826 30504 30702 25131 25078 6 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
33.7812 2.6134
3 4 1 3 0 2 4 4 3 2 4 4 1 3 1 1 2 
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 0.0000 
20766 20200 19372 16709 18405 21474 16943 27536 20967 22680 22978 26387 20974 21601 26991 25396 6 
0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0 
*/
