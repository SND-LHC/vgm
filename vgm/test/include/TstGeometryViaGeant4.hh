// $Id$
//
// Class TstPrimaryGeneratorAction
// -------------------------------
// The test detector construction
//
// Author: Ivana Hrivnacova; IPN Orsay


#ifndef TST_GEOMETRY_VIA_GEANT4_H
#define TST_GEOMETRY_VIA_GEANT4_H

#include "TstVGeometry.hh"

class G4LogicalVolume;
class G4Material;

class TstGeometryViaGeant4 : public TstVGeometry 
{
  public:
    TstGeometryViaGeant4();
    virtual ~TstGeometryViaGeant4();

    virtual void  DefineMaterials();

    virtual void* TestSolids(G4bool fullPhi);
    virtual void* TestPlacements();
    virtual void* TestReflections(G4bool fullPhi);
    virtual void* TestBooleanSolids1();
    virtual void* TestBooleanSolids2();
    virtual void* TestBooleanSolids3();

  private:
    G4LogicalVolume* CreateWorld();
    G4LogicalVolume* CreateBox();
    G4LogicalVolume* CreateCons(G4double sphi, G4double dphi);
    G4LogicalVolume* CreatePara();
    G4LogicalVolume* CreatePolycone(G4double sphi, G4double dphi);
    G4LogicalVolume* CreatePolyhedra(G4double sphi, G4double dphi);
    G4LogicalVolume* CreateSphere(G4double sphi, G4double dphi);
    G4LogicalVolume* CreateTorus(G4double sphi, G4double dphi);
    G4LogicalVolume* CreateTrap();
    G4LogicalVolume* CreateTrd();
    G4LogicalVolume* CreateTubs(G4double sphi, G4double dphi);
    G4LogicalVolume* PlaceSolids(G4LogicalVolume* mother,
                          G4bool fullPhi, G4bool reflect, G4double zpos);

    // data members
    G4Material* fBasicMaterial;
};

#endif //TST_GEOMETRY_VIA_GEANT4_H

