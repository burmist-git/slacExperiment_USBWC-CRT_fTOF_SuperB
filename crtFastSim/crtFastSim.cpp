//my
#include "src/crtDetector.hh"
#include "src/muonGen.hh"
#include "src/crtBox.hh"
#include "src/crtConst.hh"
#include "src/plane.hh"

//root
#include <TString.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TGraph2D.h>

//C, C++
#include <iostream>
#include <stdlib.h>
#include <assert.h>

using namespace std;

Int_t main(int argc, char* argv[]){

  if(argc == 4){
    Int_t N = atoi(argv[1]);
    Int_t mySeed = atoi(argv[2]);
    TString histF = argv[3];
    cout<<endl
	<<" number of events to generate     "<<N<<endl
	<<" seed of the random generator     "<<mySeed<<endl
	<<" name of the file with histograms "<<histF<<endl;

    //////////
    Int_t i = 0;
    Int_t k = 0;
    //////////


    Int_t n = 1000;
    Double_t xQuartz[n];
    Double_t yQuartz[n];
    Double_t zQuartz[n];


    /////Histograms////
    //generator
    TH1D *h1muonPhi = new TH1D("h1muonPhi"," muon Phi ",100,-190.0,190.0);
    TH1D *h1muonTheta = new TH1D("h1muonTheta"," muon Theta ",100,150,190.0);
    TH1D *h1muonX = new TH1D("h1muonX"," muon X ",100,
			     -1.0*crtConst::hodoYBar_lenght/2.0 + crtConst::topHodo_xShift - 10,
			     crtConst::hodoYBar_lenght/2.0 + crtConst::topHodo_xShift + 10);
    TH1D *h1muonY = new TH1D("h1muonY"," muon Y ",100,
			     -1.0*crtConst::hodoXBar_lenght/2.0 + crtConst::topHodo_yShift - 10,
			     crtConst::hodoXBar_lenght/2.0 + crtConst::topHodo_yShift + 10);

    //Trigger 1
    TH1D *h1trig1X = new TH1D("h1trig1X"," trig1 X ",100,
			     -1.0*crtConst::hodoYBar_lenght/2.0 + crtConst::topHodo_xShift - 10,
			      crtConst::hodoYBar_lenght/2.0 + crtConst::topHodo_xShift + 10);
    TH1D *h1trig1Y = new TH1D("h1trig1Y"," trig1 Y ",100,
			      -1.0*crtConst::hodoXBar_lenght/2.0 + crtConst::topHodo_yShift - 10,
			      crtConst::hodoXBar_lenght/2.0 + crtConst::topHodo_yShift + 10);
    TH2D *h2trig1_yVSx = new TH2D("h2trig1_yVSx"," trig1 yVSx ",100,-60.0,60.0,1000,-40,40);
    TH1D *h1trig1_z = new TH1D("h1trig1_z"," trig1 z ",10000,-10.0,300.0);
    
    //Trigger 2
    TH1D *h1trig2X = new TH1D("h1trig2X"," trig2 X ",100,
			      -1.0*crtConst::hodoYBar_lenght/2.0 + crtConst::topHodo_xShift - 10,
			      crtConst::hodoYBar_lenght/2.0 + crtConst::topHodo_xShift + 10);
    TH1D *h1trig2Y = new TH1D("h1trig2Y"," trig2 Y ",100,
			      -1.0*crtConst::hodoXBar_lenght/2.0 + crtConst::topHodo_yShift - 10,
			      crtConst::hodoXBar_lenght/2.0 + crtConst::topHodo_yShift + 10);
    TH2D *h2trig2_yVSx = new TH2D("h2trig2_yVSx"," trig2 yVSx ",100,-60.0,60.0,1000,-40,40);
    TH1D *h1trig2_z = new TH1D("h1trig2_z"," trig2 z ",10000,-10.0,300.0);
    
    //Start quartz counter
    TH2D *h2quartzSt_yVSx = new TH2D("h2quartzSt_yVSx"," quartzSt yVSx ",100,-20.0,20.0,100,-10,10);
    TH1D *h1quartzSt_x = new TH1D("h1quartzSt_x"," quartzSt x ",100,-20.0,20.0);
    TH1D *h1quartzSt_y = new TH1D("h1quartzSt_y"," quartzSt y ",100,-10,10);
    TH1D *h1quartzSt_z = new TH1D("h1quartzSt_z"," quartzSt z ",100, 80,120);

    TH2D *h2fTOF_yVSx = new TH2D("h2fTOF_yVSx"," fTOF yVSx ",100,-20.0,20.0,100,-10,10);
    TH1D *h1fTOF_x = new TH1D("h1fTOF_x"," fTOF x ",100,-20.0,20.0);
    TH1D *h1fTOF_y = new TH1D("h1fTOF_y"," fTOF y ",100,-10.0,10.0);


    ////////////////////

    crtDetector *crtDet = new crtDetector();
    muonGen *muonGenerator = new muonGen(mySeed);

    for(i = 0;i<N;i++){
      if(i%1000000==0)
	cout<<i<<endl;
      
      crtDet->SetGenTrk(muonGenerator->GenerateMuon(129.22));
      //crtDet->SetGenTrk(muonGenerator->GenerateMuon(111));
      
      if(crtDet->Trigger()){
	//crtDet->recoTrk();

	//generator
	h1muonPhi->Fill(muonGenerator->GetPhi());
	h1muonTheta->Fill(muonGenerator->GetTheta());
	h1muonX->Fill(muonGenerator->GetX());
	h1muonY->Fill(muonGenerator->GetY());
      
	//tiger1
	h2trig1_yVSx->Fill(crtDet->_Trigg1->GetXintTop(),crtDet->_Trigg1->GetYintTop());
	h1trig1X->Fill(crtDet->_Trigg1->GetXintTop());
	h1trig1Y->Fill(crtDet->_Trigg1->GetYintTop());
	h1trig1_z->Fill(crtDet->_Trigg1->GetZintTop());
	h1trig1_z->Fill(crtDet->_Trigg1->GetZintBot());
	
	//triger2
	h2trig2_yVSx->Fill(crtDet->_Trigg2->GetXintTop(),crtDet->_Trigg2->GetYintTop());
	h1trig2X->Fill(crtDet->_Trigg2->GetXintTop());
	h1trig2Y->Fill(crtDet->_Trigg2->GetYintTop());
	h1trig2_z->Fill(crtDet->_Trigg2->GetZintTop());
	h1trig2_z->Fill(crtDet->_Trigg2->GetZintBot());
	
	//start quartz	
	h2quartzSt_yVSx->Fill(crtDet->_QuartzStart->GetXintTop(),crtDet->_QuartzStart->GetYintTop());
	h1quartzSt_x->Fill(crtDet->_QuartzStart->GetXintTop());
	h1quartzSt_y->Fill(crtDet->_QuartzStart->GetYintTop());
	h1quartzSt_z->Fill(crtDet->_QuartzStart->GetZintTop());

	if(k<n){
	  xQuartz[k] = crtDet->_QuartzStart->GetXintTop();
	  yQuartz[k] = crtDet->_QuartzStart->GetYintTop();
	  zQuartz[k] = crtDet->_QuartzStart->GetZintTop();
	}

	
	//fTOF
	h2fTOF_yVSx->Fill(crtDet->_fTOFprot->GetplTop()->GetXint(),
			  crtDet->_fTOFprot->GetplTop()->GetYint());
	h1fTOF_x->Fill(crtDet->_fTOFprot->GetplTop()->GetXint());
	h1fTOF_y->Fill(crtDet->_fTOFprot->GetplTop()->GetYint());

	k++;    	
      }


    }

    /////////WRITE HISTOS TO FILE//////////
    TFile* rootFile = new TFile(histF.Data(), "RECREATE", " Histograms for CRT FastSim ", 1);
    rootFile->cd();
    if (rootFile->IsZombie()){
      cout<<"  ERROR ---> file "<<histF<<" is zombi"<<endl;
      assert(0);
    }
    else
      cout<<"  Output Histos file ---> "<<histF<<endl;

    h1muonPhi->Write();
    h1muonX->Write();
    h1muonY->Write();
    h1muonTheta->Write();

    h2trig1_yVSx->Write();
    h1trig1_z->Write();
    h1trig1X->Write();
    h1trig1Y->Write();

    h2trig2_yVSx->Write();
    h1trig2X->Write();
    h1trig2Y->Write();
    h1trig2_z->Write();

    h2quartzSt_yVSx->Write();
    h1quartzSt_x->Write();
    h1quartzSt_y->Write();
    h1quartzSt_z->Write();

    h2fTOF_yVSx->Write();
    h1fTOF_x->Write();
    h1fTOF_y->Write();

    //TGraph2D *gr2DQuartz = new TGraph2D(n, xQuartz,  yQuartz, zQuartz);
    //gr2DQuartz->SaveAs("Quartz3D.C");

    cout<<" number of generaded events = "<<N<<endl
	<<" number of triggered events = "<<k<<endl;
    
    delete crtDet;
    delete muonGenerator;
  }
  else{
    cout<<"  ERROR ---> in input arguments "<<endl
	<<"             [1] : number of events to generate "<<endl
	<<"             [2] : seed of the random generator "<<endl
	<<"             [3] : name of the file with histograms "<<endl;
  }
  
  return 0;
}
