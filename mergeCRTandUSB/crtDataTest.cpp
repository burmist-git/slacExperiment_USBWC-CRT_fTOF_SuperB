#include "src/crtFileStr.hh"

//C, C++
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <assert.h>

#include <time.h>

using namespace std;

int main(int argc, char* argv[]){
  
  if(argc == 4 && atoi(argv[1])==0){

      TString rootFileListCRTstr = argv[2];
      TString outputRootHistFile = argv[3];
      cout<<endl
	  <<"   for general CRT data test "<<endl
	  <<" list of input CRT ROOT files --> "<<rootFileListCRTstr<<endl
	  <<" generic output name (w/o extension) --> "<<outputRootHistFile<<endl;
      //init root files with CRT data    
      crtFileStr   *crt = new crtFileStr(); 
      crt->addFilesToChain(rootFileListCRTstr.Data());
      //crt->CheckDistInTime();
      crt->Loop(outputRootHistFile);    
  }
  else if(argc == 4 && atoi(argv[1])==1){
      TString rootFileListCRTstr = argv[2];
      TString outputRootHistFile = argv[3];
      cout<<endl
	  <<"   for CRT hodoscope data test "<<endl
	  <<" list of input CRT ROOT files --> "<<rootFileListCRTstr<<endl
	  <<" ROOT output file --> "<<outputRootHistFile<<endl;
      //init root files with CRT data    
      crtFileStr   *crt = new crtFileStr(); 
      crt->addFilesToChain(rootFileListCRTstr.Data());
      crt->crtHodoscopeDataTest(outputRootHistFile);    
  }
  else{
    cout<<endl
	<<"  ERROR ---> in input arguments "<<endl
	<<"             [1] : 0 for general CRT data test "<<endl
	<<"             [2] : list of input CRT ROOT files"<<endl
	<<"             [3] : generic output name (w/o extension)"<<endl
	<<"----------------------------------------------------"<<endl
	<<"             [1] : 1 for CRT hodoscope data test "<<endl
	<<"             [2] : root file list with CRT data"<<endl
	<<"             [3] : output root file with histograms"<<endl;
  }
  
  return 0;
}
