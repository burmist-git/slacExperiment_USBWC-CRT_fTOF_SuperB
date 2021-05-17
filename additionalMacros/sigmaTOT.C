Int_t sigmaTOT(){

  Double_t sigma_ele = 10.0;
  Double_t sigma_det = 70.0;
  Double_t sigma_TTS = 40.0;

  Double_t sigma_trk = 10.0;
  Double_t sigma_t0  = 20.0;

  Double_t Npe       = 18.0;

  Double_t sigma_TOT = TMath::Sqrt(sigma_ele*sigma_ele/Npe + 
				   sigma_det*sigma_det/Npe + 
				   sigma_TTS*sigma_TTS/Npe + 
				   sigma_trk*sigma_trk     +
				   sigma_t0*sigma_t0       + 
				   sigma_ele*sigma_ele); 

  cout<<"sigma_TOT = "<<sigma_TOT<<endl;

  return 0;
}
