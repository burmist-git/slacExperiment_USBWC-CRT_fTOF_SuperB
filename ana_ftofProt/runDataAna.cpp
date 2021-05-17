//my
#include "src/anaData.hh"

//root
#include <TString.h>

//C, C++
#include <iostream>
#include <stdlib.h>

using namespace std;

Int_t main(int argc, char* argv[]){

  if(argc == 4 && atoi(argv[1])==0){
    TString rootFL = argv[2];
    TString histF = argv[3];
    cout<<endl
	<<"rootFL        --> "<<rootFL<<endl
	<<"histF         --> "<<histF<<endl;
    anaData *t= new anaData(rootFL);
    cout<<endl<<"//// Data analysis ////"<<endl;
    t->Loop(histF);
    delete t;
  }
  else if(argc == 4 && atoi(argv[1])==1){
    TString rootFL = argv[2];
    TString histF = argv[3];
    cout<<endl
	<<"rootFL        --> "<<rootFL<<endl
	<<"histF         --> "<<histF<<endl;
    anaData *t= new anaData(rootFL);
    cout<<endl<<"//// Data analysis new loop////"<<endl;
    t->LoopNew(histF);
    delete t;
  }
  else{
    cout<<"  ERROR ---> in input arguments "<<endl
	<<"             [1] = 0 : run mode ID (ANALYSE)"<<endl
	<<"             [2] : input list of root files to process "<<endl
	<<"             [3] : output root file with histos "<<endl;
    cout<<"  ERROR ---> in input arguments "<<endl
	<<"             [1] = 1 : run mode ID (ANALYSE new loop)"<<endl
	<<"             [2] : input list of root files to process "<<endl
	<<"             [3] : output root file with histos "<<endl;
  }

  return 0;
}
