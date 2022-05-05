//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "G4UniformMagField.hh"
#include "G4TransportationManager.hh"
#include "G4FieldManager.hh"
#include "G4GlobalMagFieldMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Envelope parameters
  //
  G4double env_sizeXY = 18*m, env_sizeZ = 14*m;
  // G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");  //World is of AIR
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

///



////

 for (G4int i=-75 ;i<=75 ;i++) 
{
  //iron plate

  G4Material* iron_plate_mat = nist->FindOrBuildMaterial("G4_Fe");

  G4Box* solidIronPlate =    
    new G4Box("IronPlate",0.5*16*m, 0.5*16*m, 0.5*5.6*cm);
      
  G4LogicalVolume* logicIronPlate =                         
    new G4LogicalVolume(solidIronPlate,iron_plate_mat,"IronPlate");
               
  new G4PVPlacement(0,G4ThreeVector(0,0,(9.6*i) *cm),logicIronPlate,"IronPlate",logicWorld,false,i,checkOverlaps); 
  //

  //glass plate

  G4Material* glass_plate_mat = nist->FindOrBuildMaterial("G4_GLASS_PLATE");

  G4Box* solidGlass =    
    new G4Box("GlassPlate",0.5*16*m, 0.5*16*m, 0.5*0.01*mm);
      
  G4LogicalVolume* logicGlass=                         
    new G4LogicalVolume(solidGlass,glass_plate_mat,"GlassPlate");
               
  new G4PVPlacement(0,G4ThreeVector(0,0,((9.6*i)+4.8)*cm),logicGlass,"GlassPlate",logicWorld,false,i,checkOverlaps); 
  
  
  fScoringVolume = logicGlass;
  }


  //
  //always return the physical World
  //
  return physWorld;
}


//adding magnetic field

void B1DetectorConstruction::ConstructSDandField()
{
  G4ThreeVector fieldValue = G4ThreeVector(0.,1.5*tesla,0.);
  G4GlobalMagFieldMessenger *fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
