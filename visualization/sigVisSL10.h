#ifndef sigVisSL10_h
#define sigVisSL10_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class sigVisSL10 {
public :
   TTree          *fChain;
   Int_t           fCurrent;

   // Declaration of leaf types
   Int_t           eventID;
   Float_t         SamplingPeriod;
   Int_t           INLCorrection;
   Int_t           FCR;
   Double_t        UnixTime;
   Float_t         posX;
   Float_t         posY;
   Float_t         posZ;
   Float_t         ch[16][256];
   Float_t         Charge[16];
   Float_t         Time[16];
   Float_t         Amplitude[16];

   // List of branches
   TBranch        *b_eventID;   //!
   TBranch        *b_SamplingPeriod;   //!
   TBranch        *b_INLCorrection;   //!
   TBranch        *b_FCR;   //!
   TBranch        *b_UnixTime;   //!
   TBranch        *b_posX;   //!
   TBranch        *b_posY;   //!
   TBranch        *b_posZ;   //!
   TBranch        *b_ch;   //!
   TBranch        *b_Charge;   //!
   TBranch        *b_Time;   //!
   TBranch        *b_Amplitude;   //!

   //sigVisSL10(TTree *tree=0);
   sigVisSL10(TString nameF);
   virtual ~sigVisSL10();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef sigVisSL10_cxx
//sigVisSL10::sigVisSL10(TTree *tree){
sigVisSL10::sigVisSL10(TString nameF){
  //if parameter tree is not specified (or zero), connect the file
  //used to generate this class and read the Tree.
  //if (tree == 0) {
  //TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../../RootFiles/SL10_22.06.2011/Run_SL10_3.32kV_Data_6_22_2011/Run_SL10_3.32kV_Data_6_22_2011.root");
  //if (!f) {
  //f = new TFile("../../RootFiles/SL10_22.06.2011/Run_SL10_3.32kV_Data_6_22_2011/Run_SL10_3.32kV_Data_6_22_2011.root");
  //}
  //tree = (TTree*)gDirectory->Get("T");    
  //}
  //Init(tree);
  TTree *tree=0;
  TFile *f = new TFile(nameF.Data());
  tree = (TTree*)gDirectory->Get("T");    
  Init(tree);
}

sigVisSL10::~sigVisSL10()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t sigVisSL10::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t sigVisSL10::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void sigVisSL10::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("eventID", &eventID, &b_eventID);
   fChain->SetBranchAddress("SamplingPeriod", &SamplingPeriod, &b_SamplingPeriod);
   fChain->SetBranchAddress("INLCorrection", &INLCorrection, &b_INLCorrection);
   fChain->SetBranchAddress("FCR", &FCR, &b_FCR);
   fChain->SetBranchAddress("UnixTime", &UnixTime, &b_UnixTime);
   fChain->SetBranchAddress("posX", &posX, &b_posX);
   fChain->SetBranchAddress("posY", &posY, &b_posY);
   fChain->SetBranchAddress("posZ", &posZ, &b_posZ);
   fChain->SetBranchAddress("ch", ch, &b_ch);
   fChain->SetBranchAddress("Charge", Charge, &b_Charge);
   fChain->SetBranchAddress("Time", Time, &b_Time);
   fChain->SetBranchAddress("Amplitude", Amplitude, &b_Amplitude);
   Notify();
}

Bool_t sigVisSL10::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void sigVisSL10::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t sigVisSL10::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef sigVisSL10_cxx
