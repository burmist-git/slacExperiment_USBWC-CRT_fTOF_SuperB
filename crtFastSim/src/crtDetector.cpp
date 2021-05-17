//my
#include "crtDetector.hh"
#include "crtBox.hh"
#include "crtTrk.hh"
#include "crtConst.hh"


//C, C++
#include <iostream>

using namespace std;

crtDetector::crtDetector(){

  /*
  _Trigg1 = new crtBox("Trigg1", 
		       (crtConst::hodoYBar_lenght+10),(crtConst::hodoXBar_lenght+10), 1.0, 
		       0.0, 0.0, -3.0,
		       0.0);
  _Trigg2 = new crtBox("Trigg2", 
		       (crtConst::hodoYBar_lenght+10), (crtConst::hodoXBar_lenght+10), 1.0,
		       0.0,0.0,(crtConst::hodo_hight + 3.0),
		       0.0);
  */

  _Trigg1 = new crtBox("Trigg1", 
		       (crtConst::hodoYBar_lenght),(crtConst::hodoXBar_lenght), 1.0, 
		       0.0, 0.0, 0.0,
		       0.0);
  _Trigg2 = new crtBox("Trigg2", 
		       (crtConst::hodoYBar_lenght), (crtConst::hodoXBar_lenght), 1.0,
		       0.0,0.0,(crtConst::hodo_hight),
		       0.0);
  //74.8%
  _QuartzStart = new crtBox("QuartzStart",
			    crtConst::quartzBar_lenght,crtConst::quartzBar_width,crtConst::quartzBar_high,
			    4.007,0.0,(16.367+83.5),
			    -43.0);
  

  //_QuartzStart = new crtBox("QuartzStart",
  //	    crtConst::quartzBar_lenght,crtConst::quartzBar_width,crtConst::quartzBar_high,
  //	    4.007,0.0,(16.367+83.5),
  //	    0.0);
  
  //_QuartzStart = new crtBox("QuartzStart",
  //		    crtConst::quartzBar_lenght,crtConst::quartzBar_width,3.0,
  //		    4.007,1.0,(16.367+83.5),
  //		    -43.0);

  //_fTOFprot = new crtBox("fTOFprot",
  //		 crtConst::quartzBar_lenght,crtConst::quartzBar_width,crtConst::quartzBar_high,
  //		 0.55,0.0,(83.5 + 38.1 + 1.49 +  1.5 + 1.64 + 1.5/2.0),
  //		 0.0);

  _fTOFprot = new crtBox("fTOFprot",
			 crtConst::quartzBar_lenght,crtConst::quartzBar_width,(1.5*2+1.64),
			 0.55,0.0,(83.5 + 38.1 + 1.49 +  1.5 + 1.64/2),
			 0.0);
}

crtDetector::~crtDetector(){
}

void crtDetector::recoTrk(){
}

Bool_t crtDetector::Trigger(){

  _Trigg1->GetIntersection(_genTrk);
  _Trigg2->GetIntersection(_genTrk);
  _QuartzStart->GetIntersection(_genTrk);
  _fTOFprot->GetIntersection(_genTrk);

  //cout<<endl<<"------"<<endl
  //  <<"_Trigg1->_interStatus  "<<_Trigg1->GetIntersecStatus()<<endl
  //  <<"_Trigg2->_interStatus  "<<_Trigg2->GetIntersecStatus()<<endl;
  

  //29767 10000000
  // if(_Trigg1->GetIntersecStatus() >= 0 &&
  // _Trigg2->GetIntersecStatus() >= 0 && 
  // _QuartzStart->GetIntersecStatus() >= 0 && 
  // _fTOFprot->GetIntersecStatus() >= 0){
  //return true;
  //}
  

  //48469 10000000
  //if(_Trigg1->GetIntersecStatus() >= 0 &&
  // _Trigg2->GetIntersecStatus() >= 0 && 
  // _QuartzStart->GetIntersecStatus() >= 0){
  //return true;
  //}

  //1550596 10000000
  //if(_Trigg1->GetIntersecStatus() >= 0 &&
  // _Trigg2->GetIntersecStatus() >= 0 ){
  //return true;
  //}

  //9520221 10000000
  //if(_Trigg2->GetIntersecStatus() >= 0 ){
  //return true;
  //}
  
  //7264 10000000
  if(_QuartzStart->GetIntersecStatus() >= 0 &&
     _fTOFprot->GetIntersecStatus() >= 0){
    return true;
  }


  //36108 30000000 43.0 deg
  //48244 30000000 0.0 deg
  //if(_QuartzStart->GetIntersecStatus() >= 0){
  //return true;
  //}
  
  return false;
}
