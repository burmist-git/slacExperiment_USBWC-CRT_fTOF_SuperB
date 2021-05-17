#ifndef sigVis_h
#define sigVis_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class sigVis {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           eventID_crt;
   Double_t        UnixTime_crt;
   Double_t        x1_crt;
   Double_t        y1_crt;
   Double_t        x2_crt;
   Double_t        y2_crt;
   Int_t           eventID_usb;
   Float_t         SamplingPeriod_usb;
   Double_t        UnixTime_usb;
   Float_t         ch_usb[16][256];

   // List of branches
   TBranch        *b_eventID_crt;   //!
   TBranch        *b_UnixTime_crt;   //!
   TBranch        *b_x1_crt;   //!
   TBranch        *b_y1_crt;   //!
   TBranch        *b_x2_crt;   //!
   TBranch        *b_y2_crt;   //!
   TBranch        *b_eventID_usb;   //!   
   TBranch        *b_SamplingPeriod_usb;   //!
   TBranch        *b_UnixTime_usb;   //!
   TBranch        *b_ch_usb;   //!

   sigVis(string name);
   virtual ~sigVis();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   void DrawToCanvas(Long64_t entry[100], TString namePS);
   void Draw(Long64_t entry);

};

#endif

#ifdef sigVis_cxx
sigVis::sigVis(string name){
 ifstream indata;
  string rootFileName;
  TChain *theChain = new TChain("T");
  indata.open(name.c_str()); 
  assert(indata.is_open());  
  while (indata  >> rootFileName ){
    if(indata.eof()){
      std::cout<<"EOF"<<std::endl;
      break;
    }
    TFile *f = new TFile(rootFileName.c_str());
    cout<<"        adding "<<rootFileName<<endl;
    theChain->Add(rootFileName.c_str(),-1);
  }
  indata.close();
  Init(theChain);
  Long64_t nentries = fChain->GetEntriesFast();
  cout<<"======================="<<endl;
  cout<<" total number of event "<<nentries<<endl;
  cout<<"======================="<<endl;
}

sigVis::~sigVis()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t sigVis::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t sigVis::LoadTree(Long64_t entry)
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

void sigVis::Init(TTree *tree)
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

   fChain->SetBranchAddress("eventID_crt", &eventID_crt, &b_eventID_crt);
   fChain->SetBranchAddress("UnixTime_crt", &UnixTime_crt, &b_UnixTime_crt);
   fChain->SetBranchAddress("x1_crt", &x1_crt, &b_x1_crt);
   fChain->SetBranchAddress("y1_crt", &y1_crt, &b_y1_crt);
   fChain->SetBranchAddress("x2_crt", &x2_crt, &b_x2_crt);
   fChain->SetBranchAddress("y2_crt", &y2_crt, &b_y2_crt);
   fChain->SetBranchAddress("eventID_usb", &eventID_usb, &b_eventID_usb);
   fChain->SetBranchAddress("SamplingPeriod_usb", &SamplingPeriod_usb, &b_SamplingPeriod_usb);
   fChain->SetBranchAddress("UnixTime_usb", &UnixTime_usb, &b_UnixTime_usb);
   fChain->SetBranchAddress("ch_usb", ch_usb, &b_ch_usb);

   Notify();
}

Bool_t sigVis::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void sigVis::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t sigVis::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef sigVis_cxx
