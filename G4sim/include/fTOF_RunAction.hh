#ifndef fTOF_RunAction_h
#define fTOF_RunAction_h 1

//my
#include "HitDataStructure.hh"
#include "EventDataStructure.hh"

//G4
#include "G4Timer.hh"
#include "globals.hh"
#include "G4UserRunAction.hh"

//root
#include "TTree.h"

class TFile;
class G4Run;

class fTOF_RunAction : public G4UserRunAction
{
public:

  fTOF_RunAction();
  virtual ~fTOF_RunAction();

public:
  virtual void BeginOfRunAction(const G4Run* aRun);
  virtual void EndOfRunAction(const G4Run* aRun);

public:
  void SetOutputFileName(G4String fileName) {_outputFileName = fileName;}
  G4String GetOutputFileName() { return _outputFileName;}
  TTree* tree;
  HitData HitInfo;
  EventData EventInfo;

private:
  G4Timer* timer;
  TFile* hfile;
  G4String _outputFileName;

};

#endif
