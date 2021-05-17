//G4
#include "G4Material.hh"
#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Point3D.hh"
#include "G4TwoVector.hh"
#include "globals.hh"

//My
#include "fTOFConst.hh"

struct VolumeStruct {
  G4Material*        material;
  G4VSolid*          solid;
  G4LogicalVolume*   logical;
  G4VPhysicalVolume* physical;
  VolumeStruct() :
    material(0),
    solid(0),
    logical(0),
    physical(0)
  {;}
  ~VolumeStruct() {;}
};

struct WorldStruct : VolumeStruct {
  // Defined as a G4Box
  const G4double sizeX;
  const G4double sizeY;
  const G4double sizeZ;
  WorldStruct() :
    sizeX(5.0*m),
    sizeY(5.0*m),
    sizeZ(5.0*m)
  {;}
};

struct SecStruct : VolumeStruct {
  // Defined as a G4Trd
  const G4double sizeXmin;
  const G4double sizeXmax;
  const G4double thikness;
  const G4double length;
  const G4double tiltAngle;
  SecStruct() :
    sizeXmin(fTOFConst::detSizeXmin),
    sizeXmax(fTOFConst::detSizeXmax),
    thikness(fTOFConst::detThikness),
    length(fTOFConst::detlength),
    tiltAngle(fTOFConst::detTiltAngle)
  {;}
};

struct SensitiveStruct : VolumeStruct {
  // Defined as a G4Box
  const G4double sizeX;
  const G4double sizeY;
  const G4double thikness;
  SensitiveStruct() :
    sizeX(fTOFConst::pmtChSizeX),
    sizeY(fTOFConst::pmtChSizeY),
    thikness(fTOFConst::pmtChThikness)
  {;}
};

struct PmtBoxStruct : VolumeStruct {
  // Defined as a G4Box
  const G4double sizeX;
  const G4double sizeY;
  const G4double thikness;
  PmtBoxStruct() :
    sizeX(fTOFConst::pmtSizeX),
    sizeY(fTOFConst::pmtSizeY),
    thikness(fTOFConst::pmtThikness)
  {;}
};

struct Abs1Struct : VolumeStruct {
  // Defined as a G4Box
  const G4double sizeX;
  const G4double sizeY;
  const G4double thikness;
  const G4double angle;
  Abs1Struct() :
    sizeX(fTOFConst::abs1SizeX),
    sizeY(fTOFConst::abs1SizeY),
    thikness(fTOFConst::abs1Thikness),
    angle(fTOFConst::abs1Angle)    
  {;}
};

struct Abs2Struct : VolumeStruct {
  // Defined as a G4Box
  const G4double sizeX;
  const G4double sizeY;
  const G4double thikness;
  Abs2Struct() :
    sizeX(fTOFConst::abs2SizeX),
    sizeY(fTOFConst::abs2SizeY),
    thikness(fTOFConst::abs2Thikness)
  {;}
};

struct Abs3Struct : VolumeStruct {
  // Defined as a G4Box
  const G4double sizeX;
  const G4double sizeY;
  const G4double thikness;
  Abs3Struct() :
    sizeX(fTOFConst::abs3SizeX),
    sizeY(fTOFConst::abs3SizeY),
    thikness(fTOFConst::abs3Thikness)
  {;}
};

struct Abs4Struct : VolumeStruct {
  // Defined as a G4Box
  const G4double sizeX;
  const G4double sizeY;
  const G4double thikness;
  Abs4Struct() :
    sizeX(fTOFConst::abs4SizeX),
    sizeY(fTOFConst::abs4SizeY),
    thikness(fTOFConst::abs4Thikness)
  {;}
};

