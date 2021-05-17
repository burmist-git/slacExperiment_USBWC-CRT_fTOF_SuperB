#include "src/usbwcFileStr.hh"

//root
#include <TString.h>

//C, C++
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <assert.h>
#include <time.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[]){
  
  if(argc == 4 && atoi(argv[1])==0){

      TString rootFileListUSBstr = argv[2];
      TString outputRootHistFile = argv[3];
      cout<<endl
	  <<" root file list with USB data --> "<<rootFileListUSBstr<<endl
	  <<" output root file with histograms --> "<<outputRootHistFile<<endl;
      //init root files with USB data    
      usbwcFileStr *usb = new usbwcFileStr(); 
      usb->addFilesToChain(rootFileListUSBstr.Data());
      usb->Loop(outputRootHistFile);
     
      //time_t rawtime;
      //struct tm * timeinfo;
      //time ( &rawtime );
      //timeinfo = localtime ( &rawtime );
      //printf ( "Current local time and date: %s", asctime (timeinfo) );
    
      //time_t rawtime;
      //tm * ptm;
      //time ( &rawtime );
      //ptm = gmtime ( &rawtime );    
      //puts ("Current time around the World:");
      //printf ("Phoenix, AZ (U.S.) :  %2d:%02d\n", (ptm->tm_hour+MST)%24, ptm->tm_min);
      //printf ("Reykjavik (Iceland) : %2d:%02d\n", (ptm->tm_hour+UTC)%24, ptm->tm_min);
      //printf ("Beijing (China) :     %2d:%02d\n", (ptm->tm_hour+CCT)%24, ptm->tm_min);    
      //printf ("Phoenix, AZ (U.S.) :  %2d\n", (ptm->tm_hour+MST));
      //printf ("Reykjavik (Iceland) : %2d\n", (ptm->tm_hour+UTC));
      //printf ("Beijing (China) :     %2d\n", (ptm->tm_hour+CCT));    
      //printf (" %ld \n", time ( &rawtime ));
    
  }
  else if(argc == 4 && atoi(argv[1])==1){
      TString rootFileListUSBstr = argv[2];
      TString outputRootHistFile = argv[3];
      cout<<endl
	  <<" root file list with USB data --> "<<rootFileListUSBstr<<endl
	  <<" output root file with histograms --> "<<outputRootHistFile<<endl;
      //init root files with USB data    
      usbwcFileStr *usb = new usbwcFileStr(); 
      usb->addFilesToChain(rootFileListUSBstr.Data());
      usb->LaserAna(outputRootHistFile); 
  }
  else if(argc == 4 && atoi(argv[1])==2){
      TString rootFileListUSBstr = argv[2];
      TString outputRootHistFile = argv[3];
      cout<<endl
	  <<" root file list with USB data --> "<<rootFileListUSBstr<<endl
	  <<" output root file with histograms --> "<<outputRootHistFile<<endl;
      //init root files with USB data    
      usbwcFileStr *usb = new usbwcFileStr(); 
      usb->addFilesToChain(rootFileListUSBstr.Data());
      usb->CTAna(outputRootHistFile); 
  }
  else{
    cout<<endl
	<<"  ERROR ---> in input arguments "<<endl
	<<"  ---- usb data test ---- "<<endl
	<<"             [1] : 0 (run mode ID)"<<endl
	<<"             [2] : root file list with USB data"<<endl
	<<"             [3] : output root file with histograms"<<endl
	<<"  ---- laser run Analise ---- "<<endl
	<<"             [1] : 1 (run mode ID)"<<endl
	<<"             [2] : root file list with USB data"<<endl
	<<"             [3] : output root file with histograms"<<endl
	<<"  ---- CT run Analise ---- "<<endl
	<<"             [1] : 2 (run mode ID)"<<endl
	<<"             [2] : root file list with USB data"<<endl
	<<"             [3] : output root file with histograms"<<endl;
  }
  
  return 0;
}
