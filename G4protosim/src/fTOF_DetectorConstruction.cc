//My
#include "fTOF_DetectorConstruction.hh"
#include "fTOF_SensitiveDetector.hh"
#include "MagneticField.hh"

//G4
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4Trap.hh"
#include "G4ExtrudedSolid.hh"
#include "G4VSolid.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4AssemblyVolume.hh"
#include "G4VisAttributes.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4Color.hh"
#include "G4TwoVector.hh"
#include "G4SDManager.hh"
#include "globals.hh"
//magnetic field
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4ChordFinder.hh"
#include "G4UserLimits.hh"
//GDML
//#include <G4GDMLParser.hh>

//root 
#include "TMath.h"

#include "crtConst.hh"


fTOF_DetectorConstruction::fTOF_DetectorConstruction()
{
  //magField = new MagneticField();
  worldVisAtt = new G4VisAttributes();
  quartzVisAtt = new G4VisAttributes();
  sensitiveVisAtt = new G4VisAttributes();
  pmtboxVisAtt = new G4VisAttributes();
  absVisAtt = new G4VisAttributes();
  // Define Materials to be used
  DefineMaterials();
}

fTOF_DetectorConstruction::~fTOF_DetectorConstruction()
{
  //delete magField;
  delete worldVisAtt;
  delete quartzVisAtt;
  delete sensitiveVisAtt;
  delete pmtboxVisAtt;
  delete absVisAtt;
}

void fTOF_DetectorConstruction::DefineMaterials()
{
  G4String symbol;
  G4double a, z, density;
  G4int ncomponents, natoms;
  G4double fractionmass;

  // Define elements
  //G4Element* H = 
  //new G4Element("Hydrogen", symbol = "H", z = 1., a = 1.01*g/mole);
  G4Element* C = 
    new G4Element("Carbon", symbol = "C", z = 6., a = 12.01*g/mole);
  G4Element* N = 
    new G4Element("Nitrogen", symbol = "N", z = 7., a = 14.01*g/mole);
  G4Element* O =
    new G4Element("Oxygen", symbol = "O", z = 8., a = 16.00*g/mole);
  G4Element* Si = 
    new G4Element("Silicon", symbol = "Si", z = 14., a = 28.09*g/mole);
  G4Element* Al = 
    new G4Element("Aluminum", symbol = "Al", z = 13., a = 26.98*g/mole);

  // Quartz Material (SiO2)
  G4Material* SiO2 = 
    new G4Material("quartz", density = 2.200*g/cm3, ncomponents = 2);
  SiO2->AddElement(Si, natoms = 1);
  SiO2->AddElement(O , natoms = 2);
  
  // Air
  //G4Material* Air = 
  //new G4Material("Air", density = 1.290*mg/cm3, ncomponents = 2);
  G4Material* Air = 
    new G4Material("Air", density = 0.000290*mg/cm3, ncomponents = 2);
  Air->AddElement(N, fractionmass = 0.7);
  Air->AddElement(O, fractionmass = 0.3);

  // Aluminum
  G4Material* Aluminum =
    new G4Material("Aluminum", density = 2.7*g/cm3, ncomponents = 1);
  Aluminum->AddElement(Al, fractionmass = 1.0);

  // Assign Materials
  world.material = Air;
  sec.material = SiO2;
  sensitive.material = sec.material;
  pmtWin1.material = sec.material;
  pmtWin2.material = sec.material;  
  pmtAbs1.material = Aluminum;
  abs1.material = Aluminum;
  abs2.material = Aluminum;
  //sensitive.material = Aluminum;
  pmtbox.material = Aluminum;
  //abs1.material = Aluminum;
  //abs2.material = Aluminum;

  //
  // Generate and Add Material Properties Table
  //						
  const G4int num = 36;
  G4double WaveLength[num];
  G4double Absorption[num];      // Default value for absorption
  G4double AirAbsorption[num];
  G4double AirRefractiveIndex[num];
  G4double PhotonEnergy[num];

  // Absorption of quartz per 1m
  G4double QuartzAbsorption[num] =
    {0.999572036,0.999544661,0.999515062,0.999483019,0.999448285,
     0.999410586,0.999369611,0.999325013,0.999276402,0.999223336,
     0.999165317,0.999101778,0.999032079,0.998955488,0.998871172,
     0.998778177,0.99867541 ,0.998561611,0.998435332,0.998294892,
     0.998138345,0.997963425,0.997767484,0.997547418,0.99729958 ,
     0.99701966 ,0.99670255 ,0.996342167,0.995931242,0.995461041,
     0.994921022,0.994298396,0.993577567,0.992739402,0.991760297,
     0.990610945};

  for (int i=0; i<num; i++) {
    WaveLength[i] = (300 + i*10)*nanometer;
    Absorption[i] = 100*m;      // Fake number for no absorption
    AirAbsorption[i] = 4.*cm;   // If photon hits air, kill it
    AirRefractiveIndex[i] = 1.;
    PhotonEnergy[num - (i+1)] = twopi*hbarc/WaveLength[i];
    /* Absorption is given per length and G4 needs mean free path
       length, calculate it here
       mean free path length - taken as probablility equal 1/e
       that the photon will be absorbed */
    QuartzAbsorption[i] = (-1)/log(QuartzAbsorption[i])*100*cm;
    //EpotekAbsorption[i] = (-1)/log(EpotekAbsorption[i])*
    //epotekBarJoint.thickness;
  }

  G4double QuartzRefractiveIndex[num] =
    {1.456535,1.456812,1.4571  ,1.457399,1.457712,1.458038,
     1.458378,1.458735,1.459108,1.4595  ,1.459911,1.460344,
     1.460799,1.46128 ,1.461789,1.462326,1.462897,1.463502,
     1.464146,1.464833,1.465566,1.46635 ,1.46719 ,1.468094,
     1.469066,1.470116,1.471252,1.472485,1.473826,1.475289,
     1.476891,1.478651,1.480592,1.482739,1.485127,1.487793};


  // Assign absorption and refraction to materials

  // Quartz
  G4MaterialPropertiesTable* QuartzMPT = new G4MaterialPropertiesTable();
  QuartzMPT->AddProperty("RINDEX", PhotonEnergy, QuartzRefractiveIndex, num);
  QuartzMPT->AddProperty("ABSLENGTH", PhotonEnergy, QuartzAbsorption, num);
  
  // Assign this material to the bars
  sec.material->SetMaterialPropertiesTable(QuartzMPT);

  // Air
  G4MaterialPropertiesTable* AirMPT = new G4MaterialPropertiesTable();
  AirMPT->AddProperty("RINDEX", PhotonEnergy, AirRefractiveIndex, num);
  AirMPT->AddProperty("ABSLENGTH", PhotonEnergy, AirAbsorption, num);
  
  // Assign these properties to the world volume
  world.material->SetMaterialPropertiesTable(AirMPT);

}

G4VPhysicalVolume* fTOF_DetectorConstruction::Construct()
{

  /*
  //magnetic field
  static G4bool fieldIsInitialized = false;
  if(!fieldIsInitialized){
    G4FieldManager* fieldMgr
      = G4TransportationManager::GetTransportationManager()->GetFieldManager();
    fieldMgr->SetDetectorField(magField);
    fieldMgr->CreateChordFinder(magField);
    fieldMgr->GetChordFinder()->SetDeltaChord(1.0*mm);
    fieldIsInitialized = true;    
  }
  */

  // 
  // Define World Volume
  //
  world.solid = new G4Box("World",
			  world.sizeX/2,
			  world.sizeY/2,
			  world.sizeZ/2);

  world.logical = new G4LogicalVolume(world.solid,
				      world.material,
				      "World");
  
  world.physical = new G4PVPlacement(0,
				     G4ThreeVector(),
				     world.logical,
				     "World",
				     0,
				     false,
				     0);

  //
  // Define detector
  //
  sec.solid = new G4Box("Sector",
			sec.sizeX/2.0, 
			sec.sizeY/2.0,
		        sec.sizeZ/2.0);

  sec.logical = new G4LogicalVolume(sec.solid, sec.material, "Sector");

  //pmt SiO2 window1
  pmtWin1.solid = new G4Box("pmtWin1",
			    pmtWin1.sizeX/2.0, 
			    pmtWin1.sizeY/2.0,
			    pmtWin1.sizeZ/2.0);

  pmtWin1.logical = new G4LogicalVolume(pmtWin1.solid, pmtWin1.material, "pmtWin1");

  //pmt SiO2 window2
  pmtWin2.solid = new G4Box("pmtWin2",
			    pmtWin2.sizeX/2.0, 
			    pmtWin2.sizeY/2.0,
			    pmtWin2.sizeZ/2.0);

  pmtWin2.logical = new G4LogicalVolume(pmtWin2.solid, pmtWin2.material, "pmtWin2");



  //
  // Sensitive volume (Photocathode)
  //
  sensitive.solid = new G4Box("Sensitive",
			      sensitive.sizeX/2.0,
			      sensitive.sizeY/2.0,
			      sensitive.sizeZ/2.0);
  sensitive.logical = new G4LogicalVolume(sensitive.solid, 
					  sensitive.material,"Sensitive");

  //
  // Pmt Box
  //  
  pmtbox.solid = new G4Box("PmtBox",
			   pmtbox.sizeX/2.0,
			   pmtbox.sizeY/2.0,
			   pmtbox.sizeZ/2.0);
  pmtbox.logical = new G4LogicalVolume(pmtbox.solid, 
				       pmtbox.material,"PmtBox");
  
  //
  //Pmt Abs1
  //
  pmtAbs1.solid = new G4Box("pmtAbs1",
			    pmtAbs1.sizeX/2.0,
			    pmtAbs1.sizeY/2.0,
			    pmtAbs1.sizeZ/2.0);
  pmtAbs1.logical = new G4LogicalVolume(pmtAbs1.solid, 
					pmtAbs1.material,"pmtAbs1");

  //
  // abs 1;
  //
  abs1.solid = new G4Box("abs1",
			 abs1.sizeX/2.0,
			 abs1.sizeY/2.0,
			 abs1.sizeZ/2.0);
  abs1.logical = new G4LogicalVolume(abs1.solid, 
				     abs1.material,"abs1");

  //
  // abs 2;
  //
  abs2.solid = new G4Box("abs2",
			 abs2.sizeX/2.0,
			 abs2.sizeY/2.0,
			 abs2.sizeZ/2.0);
  abs2.logical = new G4LogicalVolume(abs2.solid, 
				     abs2.material,"abs2");
  
  
  //-------------------------------------------------------

  G4AssemblyVolume* secAssembly = new G4AssemblyVolume();

  //--------------------------------------------------------


  G4RotationMatrix Ra;
  G4ThreeVector Ta;
  G4Transform3D Tr;

  G4int i = 0;

  for( i=0; i<8; i++){
    //add sensitive
    Ta.setX(pmtWin1.sizeX/2.0 + pmtWin2.sizeX + sensitive.sizeX/2.0);
    Ta.setY(pmtAbs1.sizeY/2.0 + sensitive.sizeY/2.0 + fTOFConst::pmtChGap);
    Ta.setZ( 3.5*pmtWin2.sizeZ/8.0 - pmtWin2.sizeZ/8.0*i);
    Tr = G4Transform3D(Ra, Ta);
    secAssembly->AddPlacedVolume(sensitive.logical, Tr);
  }

  for( i=0; i<8; i++){
    //add sensitive
    Ta.setX(pmtWin1.sizeX/2.0 + pmtWin2.sizeX + sensitive.sizeX/2.0);
    Ta.setY(- pmtAbs1.sizeY/2.0 - sensitive.sizeY/2.0 - fTOFConst::pmtChGap);
    Ta.setZ( 3.5*pmtWin2.sizeZ/8.0 - pmtWin2.sizeZ/8.0*i);
    Tr = G4Transform3D(Ra, Ta);
    secAssembly->AddPlacedVolume(sensitive.logical, Tr);
  }

  //add sec top
  Ta.setX(-pmtWin1.sizeX/2.0 - sec.sizeX/2.0);
  //old
  //Ta.setY(pmtWin2.sizeY/2.0 - sec.sizeY/2.0);
  //ubdate from J. Vavra
  Ta.setY(pmtWin2.sizeY/2.0 - sec.sizeY/2.0 - fTOFConst::barDeltaPosY);
  Ta.setZ(0.0);
  Tr = G4Transform3D(Ra, Ta);
  secAssembly->AddPlacedVolume(sec.logical, Tr);

  //add abs1 top
  Ta.setX(-pmtWin1.sizeX/2.0 - sec.sizeX - abs1.sizeX/2.0);
  //old
  //Ta.setY(pmtWin2.sizeY/2.0 - sec.sizeY/2.0);
  //ubdate from J. Vavra
  Ta.setY(pmtWin2.sizeY/2.0 - sec.sizeY/2.0 - fTOFConst::barDeltaPosY);
  Ta.setZ(0.0);
  Tr = G4Transform3D(Ra, Ta);
  //LB just for visual 14.12.2010
  //secAssembly->AddPlacedVolume(abs1.logical, Tr);

  //add abs2 top
  Ta.setX(-pmtWin1.sizeX/2.0 - sec.sizeX/2.0);
  //old
  //Ta.setY(pmtWin2.sizeY/2.0 - sec.sizeY/2.0);
  //ubdate from J. Vavra
  Ta.setY(pmtWin2.sizeY/2.0 - sec.sizeY/2.0 - fTOFConst::barDeltaPosY);
  Ta.setZ(sec.sizeZ/2.0 + abs2.sizeZ/2.0);
  Tr = G4Transform3D(Ra, Ta);
  //LB just for visual 14.12.2010
  //secAssembly->AddPlacedVolume(abs2.logical, Tr);

  //add abs2 bottom
  Ta.setX(-pmtWin1.sizeX/2.0 - sec.sizeX/2.0);
  //old
  //Ta.setY(pmtWin2.sizeY/2.0 - sec.sizeY/2.0);
  //ubdate from J. Vavra
  Ta.setY(pmtWin2.sizeY/2.0 - sec.sizeY/2.0 - fTOFConst::barDeltaPosY);
  Ta.setZ(-sec.sizeZ/2.0 - abs2.sizeZ/2.0);
  Tr = G4Transform3D(Ra, Ta);
  //LB just for visual 14.12.2010
  //secAssembly->AddPlacedVolume(abs2.logical, Tr);


  //add sec bottom  
  Ta.setX(-pmtWin1.sizeX/2.0 - sec.sizeX/2.0);
  //old
  //Ta.setY(-pmtWin2.sizeY/2.0 + sec.sizeY/2.0);
  //ubdate from J. Vavra
  Ta.setY(-pmtWin2.sizeY/2.0 + sec.sizeY/2.0 + fTOFConst::barDeltaPosY);
  Ta.setZ(0.0);
  Tr = G4Transform3D(Ra, Ta);
  secAssembly->AddPlacedVolume(sec.logical, Tr);

  //add abs1 bottom
  Ta.setX(-pmtWin1.sizeX/2.0 - sec.sizeX - abs1.sizeX/2.0);
  //old
  //Ta.setY(-pmtWin2.sizeY/2.0 + sec.sizeY/2.0);
  //ubdate from J. Vavra
  Ta.setY(-pmtWin2.sizeY/2.0 + sec.sizeY/2.0 + fTOFConst::barDeltaPosY);
  Ta.setZ(0.0);
  Tr = G4Transform3D(Ra, Ta);
  //LB just for visual 14.12.2010
  //secAssembly->AddPlacedVolume(abs1.logical, Tr);

  //add abs2 top
  Ta.setX(-pmtWin1.sizeX/2.0 - sec.sizeX/2.0);
  //old
  //Ta.setY(-pmtWin2.sizeY/2.0 + sec.sizeY/2.0);
  //ubdate from J. Vavra
  Ta.setY(-pmtWin2.sizeY/2.0 + sec.sizeY/2.0 + fTOFConst::barDeltaPosY);
  Ta.setZ(sec.sizeZ/2.0 + abs2.sizeZ/2.0);
  Tr = G4Transform3D(Ra, Ta);
  //LB just for visual 14.12.2010
  //secAssembly->AddPlacedVolume(abs2.logical, Tr);

  //add abs2 bottom
  Ta.setX(-pmtWin1.sizeX/2.0 - sec.sizeX/2.0);
  //old
  //Ta.setY(-pmtWin2.sizeY/2.0 + sec.sizeY/2.0);
  //ubdate from J. Vavra
  Ta.setY(-pmtWin2.sizeY/2.0 + sec.sizeY/2.0 + fTOFConst::barDeltaPosY);
  Ta.setZ(-sec.sizeZ/2.0 - abs2.sizeZ/2.0);
  Tr = G4Transform3D(Ra, Ta);
  //LB just for visual 14.12.2010
  //secAssembly->AddPlacedVolume(abs2.logical, Tr);

  //Add pmtWin1
  Ta.setX(0.0);
  Ta.setY(0.0);
  Ta.setZ(0.0);
  Tr = G4Transform3D(Ra, Ta);
  secAssembly->AddPlacedVolume(pmtWin1.logical, Tr);

  //Add pmtWin2
  Ta.setX(pmtWin1.sizeX/2.0 + pmtWin2.sizeX/2.0);
  Ta.setY(0.0);
  Ta.setZ(0.0);
  Tr = G4Transform3D(Ra, Ta);
  secAssembly->AddPlacedVolume(pmtWin2.logical, Tr);

  //add pmtAbs1
  Ta.setX(pmtWin1.sizeX/2.0 + pmtWin2.sizeX + pmtAbs1.sizeX/2.0);
  Ta.setY(0.0);
  Ta.setZ(0.0);
  Tr = G4Transform3D(Ra, Ta);
  secAssembly->AddPlacedVolume(pmtAbs1.logical, Tr);

  //Pmt box
  Ta.setX(pmtWin1.sizeX/2.0 + pmtWin2.sizeX + sensitive.sizeX + pmtbox.sizeXgapp + pmtbox.sizeX/2.0);
  Ta.setY(0.0);
  Ta.setZ(0.0);
  Tr = G4Transform3D(Ra, Ta);
  secAssembly->AddPlacedVolume(pmtbox.logical, Tr);
  


  /*
  //Add detector to the Assembly
  Ta.setX(0.0);
  Ta.setY(0.0);
  Ta.setZ(0.0);
  Tr = G4Transform3D(Ra, Ta);
  secAssembly->AddPlacedVolume(sec.logical, Tr);
  */

  /*
  //Add Sensitive detectors (PMT) from the right
  Ta.setX(sec.sizeX/2.0 - sensitive.sizeX/2.0);
  Ta.setY(sec.sizeY/2.0 + sensitive.thikness/2.0);
  //1 z>0
  Ta.setZ(fTOFConst::pmtGap/2.0 + sensitive.sizeZ + fTOFConst::pmtGap + sensitive.sizeZ/2.0);
  Tr = G4Transform3D(Ra, Ta);
  secAssembly->AddPlacedVolume(sensitive.logical,Tr);
  //2
  Ta.setZ(fTOFConst::pmtGap/2.0 + sensitive.sizeZ/2.0);
  Tr = G4Transform3D(Ra, Ta);
  secAssembly->AddPlacedVolume(sensitive.logical,Tr);
  //3 z<0
  Ta.setZ(-fTOFConst::pmtGap/2.0 - sensitive.sizeZ/2.0);
  Tr = G4Transform3D(Ra, Ta);
  secAssembly->AddPlacedVolume(sensitive.logical,Tr);
  //4
  Ta.setZ(-fTOFConst::pmtGap/2.0 - sensitive.sizeZ - fTOFConst::pmtGap - sensitive.sizeZ/2.0);
  Tr = G4Transform3D(Ra, Ta);
  secAssembly->AddPlacedVolume(sensitive.logical,Tr);

  //Add Sensitive detectors (PMT) from the right
  Ta.setX(-sec.sizeX/2.0 + sensitive.sizeX/2.0);
  Ta.setY(sec.sizeY/2.0 + sensitive.thikness/2.0);
  //1 z>0
  Ta.setZ(fTOFConst::pmtGap/2.0 + sensitive.sizeZ + fTOFConst::pmtGap + sensitive.sizeZ/2.0);
  Tr = G4Transform3D(Ra, Ta);
  secAssembly->AddPlacedVolume(sensitive.logical,Tr);
  //2
  Ta.setZ(fTOFConst::pmtGap/2.0 + sensitive.sizeZ/2.0);
  Tr = G4Transform3D(Ra, Ta);
  secAssembly->AddPlacedVolume(sensitive.logical,Tr);
  //3 z<0
  Ta.setZ(-fTOFConst::pmtGap/2.0 - sensitive.sizeZ/2.0);
  Tr = G4Transform3D(Ra, Ta);
  secAssembly->AddPlacedVolume(sensitive.logical,Tr);
  //4
  Ta.setZ(-fTOFConst::pmtGap/2.0 - sensitive.sizeZ - fTOFConst::pmtGap - sensitive.sizeZ/2.0);
  Tr = G4Transform3D(Ra, Ta);
  secAssembly->AddPlacedVolume(sensitive.logical,Tr);

  
  //Add Pmt box 
  Ta.setX(sec.sizeX/2.0 - sensitive.sizeX/2.0);
  Ta.setY(sec.sizeY/2.0 + sensitive.thikness + fTOFConst::pmtBoxGap + pmtbox.thikness/2.0);
  Ta.setZ(0.0);
  Tr = G4Transform3D(Ra, Ta);
  secAssembly->AddPlacedVolume(pmtbox.logical,Tr);

  //Add Pmt box 
  Ta.setX(-sec.sizeX/2.0 + sensitive.sizeX/2.0);
  Ta.setY(sec.sizeY/2.0 + sensitive.thikness + fTOFConst::pmtBoxGap + pmtbox.thikness/2.0);
  Ta.setZ(0.0);
  Tr = G4Transform3D(Ra, Ta);
  secAssembly->AddPlacedVolume(pmtbox.logical,Tr);

  //Add abs1 L
  Ta.setX(-sec.sizeX/2.0 - abs1.sizeX/2.0);
  Ta.setY(0.0);
  Ta.setZ(0.0);
  Tr = G4Transform3D(Ra, Ta);
  secAssembly->AddPlacedVolume(abs1.logical,Tr);

  //Add abs1 R
  Ta.setX(+sec.sizeX/2.0 +  abs1.sizeX/2.0);
  Ta.setY(0.0);
  Ta.setZ(0.0);
  Tr = G4Transform3D(Ra, Ta);
  secAssembly->AddPlacedVolume(abs1.logical,Tr);

  //Add abs2 fare
  //Ta.setX(0.0);
  //Ta.setY(0.0);
  //Ta.setZ(sec.sizeZ/2.0 + abs2.sizeZ/2.0);
  //Tr = G4Transform3D(Ra, Ta);
  //secAssembly->AddPlacedVolume(abs2.logical,Tr);

  //Add abs2 not fare
  //Ta.setX(0.0);
  //Ta.setY(0.0);
  //Ta.setZ(-sec.sizeZ/2.0 - abs2.sizeZ/2.0);
  //Tr = G4Transform3D(Ra, Ta);
  //secAssembly->AddPlacedVolume(abs2.logical,Tr);


  */
  
  //
  //make Imprint
  //
  
  Ra.rotateX(90.0*deg);
  //Ra.rotateY(20.0*deg);
  //Ra.rotateY(180.0*deg);

  //One
  Ta.setX((0.55 + crtConst::quartzBar_lenght/2.0)*cm);
  Ta.setY(0.0);
  Ta.setZ((83.5 + 38.1 + 1.49 +  1.5 + 1.64/2)*cm);
  Tr = G4Transform3D(Ra, Ta);
  secAssembly->MakeImprint(world.logical, Tr, 0, true);

  //Ta.setX(0.0);
  //Ta.setY(-5.0*cm);
  //Ta.setZ(0.0*cm);
  //Tr = G4Transform3D(Ra, Ta);
  //secAssembly->MakeImprint(world.logical, Tr, 0, true);



  //-----------------------------------------------------

  //
  // Set Visualization Attributes
  //
  G4Color blue        = G4Color(0., 0., 1.);
  G4Color green       = G4Color(0., 1., 0.);
  G4Color red         = G4Color(1., 0., 0.);
  G4Color white       = G4Color(1., 1., 1.);
  G4Color cyan        = G4Color(0., 1., 1.);
  G4Color DircColor   = G4Color(0.0, 0.0, 1.0, 0.2);
  G4Color SensColor   = G4Color(0.0, 1.0, 1.0, 0.1);

  worldVisAtt->SetColor(white);
  worldVisAtt->SetVisibility(true);
  quartzVisAtt->SetColor(DircColor);
  quartzVisAtt->SetVisibility(true);
  sensitiveVisAtt->SetColor(SensColor);
  sensitiveVisAtt->SetVisibility(true);
  pmtboxVisAtt->SetColor(red);
  pmtboxVisAtt->SetVisibility(true);
  absVisAtt->SetColor(red);
  absVisAtt->SetVisibility(true);


  world.logical->SetVisAttributes(worldVisAtt);

  sec.logical->SetVisAttributes(quartzVisAtt);
  pmtWin1.logical->SetVisAttributes(quartzVisAtt);
  pmtWin2.logical->SetVisAttributes(quartzVisAtt);

  sensitive.logical->SetVisAttributes(sensitiveVisAtt);

  pmtbox.logical->SetVisAttributes(pmtboxVisAtt);

  pmtAbs1.logical->SetVisAttributes(absVisAtt);
  abs1.logical->SetVisAttributes(absVisAtt);
  abs2.logical->SetVisAttributes(absVisAtt);


  //
  // Define Optical Borders
  //

  // Surface for killing photons at borders
  const G4int num1 = 2;
  G4double Ephoton[num1] = {1.5*eV, 5.8*eV};

  G4OpticalSurface* OpVolumeKillSurface =
    new G4OpticalSurface("VolumeKillSurface");
  OpVolumeKillSurface->SetType(dielectric_metal);
  OpVolumeKillSurface->SetFinish(polished);
  OpVolumeKillSurface->SetModel(glisur);

  G4double ReflectivityKill[num1] = {0., 0.};
  G4double EfficiencyKill[num1] = {1., 1.};
  G4MaterialPropertiesTable* VolumeKill = new G4MaterialPropertiesTable();
  VolumeKill->AddProperty("REFLECTIVITY", Ephoton, ReflectivityKill, num1);
  VolumeKill->AddProperty("EFFICIENCY",   Ephoton, EfficiencyKill,   num1);
  OpVolumeKillSurface->SetMaterialPropertiesTable(VolumeKill);
  new G4LogicalSkinSurface("SensitiveSurface", 
			   sensitive.logical, OpVolumeKillSurface);
  
  
  // 
  // Sensitive detector definition
  //
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  fTOF_SensitiveDetector* aSD = new fTOF_SensitiveDetector("fTOF");
  SDman->AddNewDetector(aSD);
  sensitive.logical->SetSensitiveDetector(aSD);
  


  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  printDetectorParameters();


  //G4GDMLParser parser;
  //parser.Write("fTOFslacCRT_r.gdml", world.physical);

  return world.physical;
}

void fTOF_DetectorConstruction::printDetectorParameters(){
  
  //fTOFConst::detTiltAngle
  //fTOFConst::det_Rmin
  //fTOFConst::det_Rmax
  //fTOFConst::det_Zmin
  //fTOFConst::det_Zmax
  //fTOFConst::N_det
  //fTOFConst::detAngleSize
  //fTOFConst::detSizeXmin
  //fTOFConst::detSizeXmax
  //fTOFConst::detlength

}
