//My
#include "fTOF_EventAction.hh"
#include "fTOF_RunAction.hh"
#include "fTOF_SteppingAction.hh"
#include "fTOF_Hit.hh"
#include "fTOF_StackingAction.hh"

//G4
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "globals.hh"

fTOF_EventAction::fTOF_EventAction(fTOF_RunAction* runact,
				     fTOF_SteppingAction* steppingAction) :
  runAction(runact), _steppingAction(steppingAction), printModulo(100)
{
  thePhotonCollectionID = -1;
}

fTOF_EventAction::~fTOF_EventAction()
{
}

void fTOF_EventAction::BeginOfEventAction(const G4Event* event)
{
  // Print number of events
  G4int eventNum = event->GetEventID();

  if (eventNum%printModulo == 0) {
    G4cout << "\n---> Begin of Event: " << eventNum << G4endl;
  }

  if (thePhotonCollectionID < 0) {
    G4String colName;
    thePhotonCollectionID = 
      G4SDManager::GetSDMpointer()->
      GetCollectionID(colName="OpticalPhotonCollection");
  }

  _steppingAction->Reset();
  _steppingAction->ResetPerEvent();
}

void fTOF_EventAction::EndOfEventAction(const G4Event* event)
{
  // Print info about end of the event
  G4int eventNum = event->GetEventID();

  if (thePhotonCollectionID < 0) return;

  // Get the Hit Collection
  G4HCofThisEvent* HCE = event->GetHCofThisEvent();
  fTOF_HitsCollection * THC = 0;

  if (HCE)
    THC = (fTOF_HitsCollection*)(HCE->GetHC(thePhotonCollectionID));

  if (0 == THC) return;

  G4int nTotPhot = _stackingAction->GetTotPhotNum();
  G4int nHit = -1;
  nHit = THC->entries();

  runAction->EventInfo.EventID = eventNum;
  runAction->EventInfo.BunchXID = _primGenerator->GetBunchXID();
  runAction->EventInfo.NTotPhot = nTotPhot;
  runAction->EventInfo.Nhits = nHit;
  runAction->EventInfo.primType = event->GetPrimaryVertex()->GetPrimary()->GetPDGcode();
  //G4cout<<"primType = "<<event->GetPrimaryVertex()->GetPrimary()->GetPDGcode()<<G4endl;
  runAction->EventInfo.primMomX = event->GetPrimaryVertex()->GetPrimary()->GetPx();
  runAction->EventInfo.primMomY = event->GetPrimaryVertex()->GetPrimary()->GetPy();
  runAction->EventInfo.primMomZ = event->GetPrimaryVertex()->GetPrimary()->GetPz();
  runAction->EventInfo.primPosX = event->GetPrimaryVertex()->GetX0();
  runAction->EventInfo.primPosY = event->GetPrimaryVertex()->GetY0();
  runAction->EventInfo.primPosZ = event->GetPrimaryVertex()->GetZ0();
  runAction->EventInfo.primTime = event->GetPrimaryVertex()->GetT0();
  for (G4int i = 0; i < nHit; i++) {
    // Fill the structure then save
    runAction->HitInfo = (*THC)[i]->myData;
    runAction->tree->Fill();
  }
 
}
