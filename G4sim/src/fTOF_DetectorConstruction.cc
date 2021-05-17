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

fTOF_DetectorConstruction::fTOF_DetectorConstruction()
{
  magField = new MagneticField();
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
  delete magField;
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
  //sensitive.material = Aluminum;
  pmtbox.material = Aluminum;
  abs1.material = Aluminum;
  abs2.material = Aluminum;
  abs3.material = Aluminum;
  abs4.material = Aluminum;

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

  G4AssemblyVolume* secAssembly = new G4AssemblyVolume();

  //
  // Define detector
  //
  sec.solid = new G4Trd("Sector",
  		sec.sizeXmin/2.0, 
  		sec.sizeXmax/2.0,
  	        sec.thikness/2.0,
  		sec.thikness/2.0,
  		sec.length/2.0);

  sec.logical = new G4LogicalVolume(sec.solid, sec.material, "Sector");

  //geometry with tilted PMTS (we need G4Trap)
  /*
  G4double  pDz = 60.0*cm; 
  G4double  pTheta = 0.0*deg;
  G4double  pPhi = 0.0*deg; 
  G4double  pDy1 = 40.0*cm;
  G4double  pDx1 = 30.0*cm; 
  G4double  pDx2 = 30.0*cm;
  G4double  pAlp1 = 30.0*deg;
  G4double  pDy2 = 40.0*cm;
  G4double  pDx3 = 30.0*cm;
  G4double  pDx4 = 30.0*cm;
  G4double  pAlp2 = 30.0*deg;

  sec.solid = new G4Trap("Sector",
			 pDz, pTheta,
			 pPhi, pDy1,
			 pDx1, pDx2,
			 pAlp1, pDy2,
			 pDx3, pDx4,
			 pAlp2);
  
  sec.logical = new G4LogicalVolume(sec.solid, sec.material, "Sector");
  */


  //
  // Sensitive volume (Photocathode)
  //
  sensitive.solid = new G4Box("Sensitive",
			      sensitive.sizeX/2.,
			      sensitive.sizeY/2.,
			      sensitive.thikness/2.);
  sensitive.logical = new G4LogicalVolume(sensitive.solid, 
					  sensitive.material,"Sensitive");

  //
  // Pmt Boz
  //  
  pmtbox.solid = new G4Box("PmtBox",
			   pmtbox.sizeX/2.,
			   pmtbox.sizeY/2.,
			   pmtbox.thikness/2.);
  pmtbox.logical = new G4LogicalVolume(pmtbox.solid, 
				       pmtbox.material,"PmtBox");
  
  //
  // Absorber1 right and left  
  //
  abs1.solid = new G4Box("Abs1",
			 abs1.sizeX/2.,
			 abs1.sizeY/2.,
			 abs1.thikness/2.);
  abs1.logical = new G4LogicalVolume(abs1.solid, 
				     abs1.material,"Abs1");
  
  //
  // Absorber2 bottom I
  //
  abs2.solid = new G4Box("Abs2",
			 abs2.sizeX/2.,
			 abs2.sizeY/2.,
			 abs2.thikness/2.);
  abs2.logical = new G4LogicalVolume(abs2.solid, 
				     abs2.material,"Abs2");

  //
  // Absorber3 top
  //
  abs3.solid = new G4Box("Abs3",
			 abs3.sizeX/2.,
			 abs3.sizeY/2.,
			 abs3.thikness/2.);
  abs3.logical = new G4LogicalVolume(abs3.solid, 
				     abs3.material,"Abs3");

  //
  // Absorber3 top II
  //
  abs4.solid = new G4Box("Abs4",
			 abs4.sizeX/2.,
			 abs4.sizeY/2.,
			 abs4.thikness/2.);
  abs4.logical = new G4LogicalVolume(abs4.solid, 
				     abs4.material,"Abs4");

  //--------------------------------------------------------

  G4RotationMatrix Ra;
  G4ThreeVector Ta;
  G4Transform3D Tr;

  /*
  //position of the PMT with bad angle to the field
  //Add Sensitive detectors (PMT)
  Ta.setY(0.0);
  //from the TOP
  Ta.setZ(sec.length/2.0 + sensitive.thikness/2.0);
  //from the BOTTOM
  //Ta.setZ(-sec.length/2.0 - sensitive.thikness/2.0);
  for(G4int i = 0;i<fTOFConst::N_PMT;i++){
    for(G4int j = 0;j<fTOFConst::N_ch;j++){
      //for Xmin pmt from the BOTTOM
      //Ta.setX(-sec.sizeXmin/2.0 + sec.sizeXmin/fTOFConst::N_PMT*(1.0/2.0+i)
      //      -(sensitive.sizeX*1.5+fTOFConst::pmtGap*1.5) 
      //      +(sensitive.sizeX+fTOFConst::pmtGap)*j );

      //for Xmax pmt from the TOP
      Ta.setX(-sec.sizeXmax/2.0 + sec.sizeXmax/fTOFConst::N_PMT*(1.0/2.0+i)
	      -(sensitive.sizeX*1.5+fTOFConst::pmtGap*1.5) 
	      +(sensitive.sizeX+fTOFConst::pmtGap)*j );

      Tr = G4Transform3D(Ra, Ta);
      secAssembly->AddPlacedVolume(sensitive.logical,Tr);
    }
  }
  
  //Add Pmt box 
  Ta.setY(0.0);
  //from the TOP
  Ta.setZ(sec.length/2.0 + sensitive.thikness + pmtbox.thikness/2.0 + fTOFConst::pmtBoxGap);
  //from the BOTTOM
  //Ta.setZ(-sec.length/2.0 - sensitive.thikness - pmtbox.thikness/2.0 - fTOFConst::pmtBoxGap);
  for(G4int i = 0;i<fTOFConst::N_PMT;i++){

    //for Xmin  pmt from the BOTTOM
    //Ta.setX(-sec.sizeXmin/2.0 + sec.sizeXmin/fTOFConst::N_PMT*(1.0/2.0+i));

    //for Xmax  pmt from the TOP
    Ta.setX(-sec.sizeXmax/2.0 + sec.sizeXmax/fTOFConst::N_PMT*(1.0/2.0+i));

    Tr = G4Transform3D(Ra, Ta);
    secAssembly->AddPlacedVolume(pmtbox.logical,Tr);
  }
  */


  //NOT OK  
  //Add Sensitive detectors (PMT)
  Ta.setY(-sec.thikness/2.0 - sensitive.thikness/2.0);
  //from the TOP
  Ta.setZ(sec.length/2.0 - fTOFConst::pmtChSizeY/2.0);
  //from the BOTTOM
  //Ta.setZ(-sec.length/2.0 + fTOFConst::pmtChSizeY/2.0);
  Ra.rotateX(90.0*deg);
  for(G4int i = 0;i<fTOFConst::N_PMT;i++){
    for(G4int j = 0;j<fTOFConst::N_ch;j++){
      //for Xmin pmt from the BOTTOM
      //Ta.setX(-sec.sizeXmin/2.0 + sec.sizeXmin/fTOFConst::N_PMT*(1.0/2.0+i)
      //    -(sensitive.sizeX*1.5+fTOFConst::pmtGap*1.5) 
      //    +(sensitive.sizeX+fTOFConst::pmtGap)*j );
      
      //for Xmax pmt from the TOP
      Ta.setX(-sec.sizeXmax/2.0 + sec.sizeXmax/fTOFConst::N_PMT*(1.0/2.0+i)
	      -(sensitive.sizeX*1.5+fTOFConst::pmtGap*1.5) 
	      +(sensitive.sizeX+fTOFConst::pmtGap)*j );
      
      Tr = G4Transform3D(Ra, Ta);
      secAssembly->AddPlacedVolume(sensitive.logical,Tr);
    }
  }


  //Add Pmt box 
  Ta.setY(-sec.thikness/2.0 - sensitive.thikness - pmtbox.thikness/2.0);
  //from the TOP
  Ta.setZ(sec.length/2.0 - fTOFConst::pmtChSizeY/2.0);
  //from the BOTTOM
  //Ta.setZ(-sec.length/2.0 + fTOFConst::pmtChSizeY/2.0);
  for(G4int i = 0;i<fTOFConst::N_PMT;i++){
    //for Xmin  pmt from the BOTTOM
    //Ta.setX(-sec.sizeXmin/2.0 + sec.sizeXmin/fTOFConst::N_PMT*(1.0/2.0+i));

    //for Xmax  pmt from the TOP
    Ta.setX(-sec.sizeXmax/2.0 + sec.sizeXmax/fTOFConst::N_PMT*(1.0/2.0+i));

    Tr = G4Transform3D(Ra, Ta);
    secAssembly->AddPlacedVolume(pmtbox.logical,Tr);
  }
  Ra.rotateX(-90.0*deg);


  //Add detector to the Assembly
  Ta.setX(0.0);
  Ta.setY(0.0);
  Ta.setZ(0.0);
  Tr = G4Transform3D(Ra, Ta);
  secAssembly->AddPlacedVolume(sec.logical, Tr);

  //Add absorber 1 left 
  //Start from the top
  Ta.setX(-sec.sizeXmax/2.0 + abs1.sizeX*TMath::Sin(abs1.angle)/2.0 
    - abs1.thikness/2.0*cos(abs1.angle));
  Ta.setY(0.0);
  Ta.setZ(sec.length/2.0 - abs1.sizeX*TMath::Cos(abs1.angle)/2.0 
    - abs1.thikness/2.0*sin(abs1.angle));
  //Start from the bottom
  //Ta.setX(-sec.sizeXmin/2.0 - abs1.sizeX*TMath::Sin(abs1.angle)/2.0 
  // - abs1.thikness/2.0*cos(abs1.angle));
  //Ta.setY(0.0);
  //Ta.setZ(-sec.length/2.0 + abs1.sizeX*TMath::Cos(abs1.angle)/2.0 
  // - abs1.thikness/2.0*sin(abs1.angle));
  
  Ra.rotateY(-abs1.angle+90.0*deg);
  Tr = G4Transform3D(Ra, Ta);
  //left
  //secAssembly->AddPlacedVolume(abs1.logical, Tr);
  Ra.rotateY(abs1.angle-90.0*deg);
  
  //Add absorber 1 right
  //Start from the top
  Ta.setX(sec.sizeXmax/2.0 - abs1.sizeX*TMath::Sin(abs1.angle)/2.0 
	  + abs1.thikness/2.0*cos(abs1.angle));
  Ta.setY(0.0);
  Ta.setZ(sec.length/2.0 - abs1.sizeX*TMath::Cos(abs1.angle)/2.0 
	  - abs1.thikness/2.0*sin(abs1.angle));
  //Start from the bottom
  //Ta.setX(sec.sizeXmin/2.0 + abs1.sizeX*TMath::Sin(abs1.angle)/2.0 
  //  + abs1.thikness/2.0*cos(abs1.angle));
  //Ta.setY(0.0);
  //Ta.setZ(-sec.length/2.0 + abs1.sizeX*TMath::Cos(abs1.angle)/2.0 
  //  - abs1.thikness/2.0*sin(abs1.angle));

  Ra.rotateY(abs1.angle+90.0*deg);
  Tr = G4Transform3D(Ra, Ta);
  //right abs
  //secAssembly->AddPlacedVolume(abs1.logical, Tr);
  Ra.rotateY(-abs1.angle-90.0*deg);


  //Add absorber 2 bottom
  Ta.setX(0.0);
  Ta.setY(0.0);
  Ta.setZ(-sec.length/2.0 - abs2.thikness/2.0);
  Tr = G4Transform3D(Ra, Ta);
  secAssembly->AddPlacedVolume(abs2.logical, Tr);



  //Add absorber 3 top
  Ta.setX(0.0);
  Ta.setY(0.0);
  Ta.setZ(sec.length/2.0 + abs3.thikness/2.0);
  Tr = G4Transform3D(Ra, Ta);
  secAssembly->AddPlacedVolume(abs3.logical, Tr);


  /*
  //Add absorber 4 bottom
  Ta.setY(0.0);
  Ta.setZ(-sec.length/2.0 - abs4.thikness/2.0);
  for(G4int i = 0;i<fTOFConst::N_PMT;i++){
    Ta.setX(-sec.sizeXmin/2.0 + sec.sizeXmin/fTOFConst::N_PMT*(1.0/2.0+i)
	    -(sensitive.sizeX*2.0+fTOFConst::pmtGap*1.5) - abs4.sizeX/2.0 - fTOFConst::pmtGap/2.0);
    Tr = G4Transform3D(Ra, Ta);
    secAssembly->AddPlacedVolume(abs4.logical,Tr);
    
    Ta.setX(-sec.sizeXmin/2.0 + sec.sizeXmin/fTOFConst::N_PMT*(1.0/2.0+i)
	    +(sensitive.sizeX*2.0+fTOFConst::pmtGap*1.5) + abs4.sizeX/2.0 + fTOFConst::pmtGap/2.0);
    Tr = G4Transform3D(Ra, Ta);
    secAssembly->AddPlacedVolume(abs4.logical,Tr);
  }
  */
  
  //
  //make Imprint
  //

  /*
  //One
  Ta.setX(0.0);
  Ta.setY(0.0);
  Ta.setZ(0.0);
  Tr = G4Transform3D(Ra, Ta);
  secAssembly->MakeImprint(world.logical, Tr, 0, true);
  */

  //real position
  G4double r0 = (fTOFConst::det_Rmax*TMath::Cos(fTOFConst::detAngleSize*TMath::Pi()/180.0/2.0)
               + fTOFConst::det_Rmin)/2.0;
  G4double yC = 0.0;
  G4double xC = 0.0;
  for(G4int i = 0;i<fTOFConst::N_det;i++){
    Ra.rotateX(-(180.0 - fTOFConst::detTiltAngle)*deg);
    Ra.rotateZ(-twopi/fTOFConst::N_det*rad*i);
    yC = r0*cos(twopi/fTOFConst::N_det*rad*i);
    xC = r0*sin(twopi/fTOFConst::N_det*rad*i);
    Ta.setX(xC);
    Ta.setY(yC);
    Ta.setZ(190.126*cm);
    Ta.setZ((fTOFConst::det_Zmax + fTOFConst::det_Zmin)/2.0);
    Tr = G4Transform3D(Ra, Ta);
    secAssembly->MakeImprint(world.logical, Tr, 0, true);
    Ra.rotateZ(twopi/fTOFConst::N_det*rad*i);
    Ra.rotateX((180.0 - fTOFConst::detTiltAngle)*deg);
  }


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
  world.logical->SetVisAttributes(worldVisAtt);

  quartzVisAtt->SetColor(DircColor);
  quartzVisAtt->SetVisibility(true);
  sec.logical->SetVisAttributes(quartzVisAtt);
  
  sensitiveVisAtt->SetColor(SensColor);
  sensitiveVisAtt->SetVisibility(true);
  sensitive.logical->SetVisAttributes(sensitiveVisAtt);

  pmtboxVisAtt->SetColor(green);
  pmtboxVisAtt->SetVisibility(true);
  pmtbox.logical->SetVisAttributes(pmtboxVisAtt);

  absVisAtt->SetColor(red);
  absVisAtt->SetVisibility(true);
  abs1.logical->SetVisAttributes(absVisAtt);
  abs2.logical->SetVisAttributes(absVisAtt);
  abs3.logical->SetVisAttributes(absVisAtt);
  abs4.logical->SetVisAttributes(absVisAtt);


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


  return world.physical;
}

void fTOF_DetectorConstruction::printDetectorParameters(){
  
  G4cout<<"detThikness  = "<<fTOFConst::detThikness<<" mm"<<G4endl
	<<"detSizeXmin  = "<<fTOFConst::detSizeXmin<<" mm"<<G4endl
	<<"detSizeXmax  = "<<fTOFConst::detSizeXmax<<" mm"<<G4endl
	<<"detlength    = "<<fTOFConst::detlength<<" mm"<<G4endl
	<<"abs1CovSur   = "<<fTOFConst::abs1CovSur<<" %"<<G4endl
	<<"abs1Angle    = "<<fTOFConst::abs1Angle/deg<<" deg"<<G4endl
	<<"abs1SizeX    = "<<fTOFConst::abs1SizeX<<" mm"<<G4endl
	<<"abs1SizeY    = "<<fTOFConst::abs1SizeY<<" mm"<<G4endl
	<<"abs1Thikness = "<<fTOFConst::abs1Thikness<<" mm"<<G4endl
	<<"abs4SizeX    = "<<fTOFConst::abs4SizeX<<" mm"<<G4endl
	<<"abs4SizeY    = "<<fTOFConst::abs4SizeY<<" mm"<<G4endl
	<<"abs4Thikness = "<<fTOFConst::abs4Thikness<<" mm"<<G4endl;


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
