{

  Long64_t eventJentry;
  gROOT->LoadMacro("sigVis.C");

  ////////////////////////
  sigVis t("./rootFileList.txt");  
  eventJentry = 1123;
  ///////////////////////
  
  t.Draw(eventJentry);
  
  /*
  //TString namePS = "plots.ps";
  //const Int_t nEvNum = 100;
  //Long64_t entry[nEvNum];
  //for(Int_t i = 0;i<nEvNum;i++){
  //entry[i] = -999;
  //}
  entry[0] = 29413;
  entry[1] = 29415;
  entry[2] = 29416;
  entry[3] = 29421;
  entry[4] = 29422;
  entry[5] = 29423;
  entry[6] = 29424;
  entry[7] = 29425;
  entry[8] = 29426;
  entry[9] = 29430;
  entry[10] = 29432;
  entry[11] = 29433;
  entry[12] = 29434;
  entry[13] = 29435;
  entry[14] = 29437;
  entry[15] = 29438;
  entry[16] = 29439;
  entry[17] = 29440;
  entry[18] = 29444;
  entry[19] = 29445;
  entry[20] = 29446;
  entry[21] = 29450;
  entry[22] = 29452;
  //t.DrawToCanvas(entry,namePS);
  */

}

