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

int main(int argc, char* argv[]){

  if(argc==4){
    TString fileNfirst = argv[1];
    Int_t N = atoi(argv[2]);
    TString outputFileName = argv[3];

    cout<<endl
	<<"     Generic input file name: "<<fileNfirst<<endl
	<<"Number of files to be merged: "<<N<<endl
	<<"            Output file name: "<<outputFileName<<endl<<endl;    
    
    ofstream myfileWrite;
    myfileWrite.open(outputFileName.Data());
    string line;
    TString fileN;
    
    for(Int_t i = 0;i<N;i++){
      fileN = fileNfirst;
      //Convention: the N files to be merged are named
      // toto.dat, toto.dat_1, toto.dat_2, ..., toto.dat_<N-1>
      // with toto.dat being the generic name passed as 1rst argument
      if(i>0){
	fileN += "_";
	fileN += i;
      }
      cout<<"* Merging file "<<fileN<<endl;
      ifstream myfile(fileN.Data());
      if (myfile.is_open()){
	while (! myfile.eof() ){
	  getline (myfile,line);
	  myfileWrite << line;
	}
	myfile.close();
      }
      else cout << "\t -> Unable to open this file!" << endl; 
    }
    myfileWrite.close();  

    // Make the output ASCII file rw by the group
    TString cmd = "chmod g+rw " + outputFileName;
    //cout << "Executing command " << cmd << endl;
    system( cmd.Data() );
  }
  else{
    cout<<endl
	<<" ERROR ---> in input arguments "<<endl
	<<"            [1] : generic name of the files to be merged "<<endl
	<<"            [2] : number of files to be merged "<<endl
	<<"            [3] : name of the output file  "<<endl;    
  }
  
  return 0;
}
