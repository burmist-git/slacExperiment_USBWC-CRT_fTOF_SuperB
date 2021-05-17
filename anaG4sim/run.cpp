//my
#include "src/slacTestANA.hh"

//root
#include "TString.h"

//C, C++
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[]){

  if(argc == 4 && atoi(argv[1])==0){    
    TString dataF = argv[2];
    TString histF = argv[3];
    cout<<"--> simulation ana <--"<<endl
	<<" runID = "<<atoi(argv[1])<<endl
	<<" dataF = "<<dataF<<endl
	<<" histF = "<<histF<<endl;    
    slacTestANA t(dataF.Data(),1231);
    t.Loop(histF.Data());
    //slacTestANA t("./Data/fTOFslacCRT10000.root");
    //t.Loop("./Histos/fTOFslacCRT10000_Histos.root");
    //slacTestANA t(`"./Data/fTOFslacCRT10000_0.4GeV.root");
    //t.Loop("./Histos/fTOFslacCRT10000_0.4GeV_Histos.root");
    //slacTestANA t("./Data/fTOFslacCRT10000_fixPoint_1500GeV.root");
    //t.Loop("./Histos/fTOFslacCRT10000_fixPoint_1500GeV_Histos.root");
    //slacTestANA t("./Data/fTOFslacCRT10000_SmearedPoint_1500GeV.root");
    //t.Loop("./Histos/fTOFslacCRT10000_SmearedPoint_1500GeV_Histos.root");
  }
  else if(argc == 5 && atoi(argv[1])==1){

    TString dataF = argv[2];
    TString histF = argv[3];
    Int_t runSeed = atoi(argv[4]);
    //Int_t i = 0;
    cout<<"--> wavefrom Sim test <--"<<endl
	<<" runID = "<<atoi(argv[1])<<endl
	<<" dataF = "<<dataF<<endl
	<<" histF = "<<histF<<endl;
    slacTestANA *t = new slacTestANA( dataF.Data(), runSeed);
    t->LoopWfSim(histF.Data());
    delete t;
    //for( i=0; i<Nwf; i++){
    //wfSim *wfS = new wfSim();
    //delete wfS;
    //}
  }
  else if(argc == 4 && atoi(argv[1])==2){    

    TString dataF = argv[2];
    Int_t runSeed = atoi(argv[3]);
    //Int_t i = 0;
    cout<<"--> simple loop <--"<<endl
	<<" runID = "<<atoi(argv[1])<<endl
	<<" dataF = "<<dataF<<endl;
    slacTestANA *t = new slacTestANA( dataF.Data(), runSeed);
    t->LoopSimpl();
    delete t;
    //for( i=0; i<Nwf; i++){
    //wfSim *wfS = new wfSim();
    //delete wfS;
    //}
  }
  else if(argc == 6 && atoi(argv[1])==3){

    TString dataF = argv[2];
    Int_t key = atoi(argv[3]);
    TString histF = argv[4];
    Int_t runSeed = atoi(argv[5]);
    //Int_t i = 0;
    cout<<"--> wavefrom Sim test <--"<<endl
	<<" runID = "<<atoi(argv[1])<<endl
	<<" dataF = "<<dataF<<endl
	<<" histF = "<<histF<<endl;
    slacTestANA *t = new slacTestANA( dataF, key,runSeed);
    t->LoopWfSim(histF.Data());
    delete t;
    //for( i=0; i<Nwf; i++){
    //wfSim *wfS = new wfSim();
    //delete wfS;
    //}
  }
  else{

    cout<<" ERROR --->  in input arguments "<<endl
	<<"  simulation ana  "<<endl
	<<"  runID [1] = 0  "<<endl
	<<"        [2] - data File "<<endl
	<<"        [3] - data hist "<<endl;
    cout<<"  wavefrom Sim test     "<<endl
	<<"  runID [1] = 1  "<<endl
	<<"        [2] - root File "<<endl
	<<"        [3] - data hist "<<endl
	<<"        [4] - runSeed"<<endl;
    cout<<"  simple loop     "<<endl
	<<"  runID [1] = 2  "<<endl
	<<"        [2] - data File "<<endl
	<<"        [4] - runSeed"<<endl;
    cout<<"  wavefrom Sim test     "<<endl
	<<"  runID [1] = 3  "<<endl
	<<"        [2] - root File list"<<endl
	<<"        [3] - 1 "<<endl
	<<"        [4] - data hist "<<endl
	<<"        [5] - runSeed"<<endl;
  }

  return 0;
}
