// $Id$
//
// Class BooleanSolid
// ---------------------
// VGM implementation for Root Boolean solid
//
// Author: Ivana Hrivnacova; IPN Orsay

#include "TGeoCompositeShape.h"
#include "TGeoBoolNode.h"
#include "TGeoMatrix.h"

#include "BaseVGM/common/utilities.h"

#include "RootGM/solids/BooleanSolid.h"
#include "RootGM/solids/SolidMap.h"
#include "RootGM/common/utilities.h"

const char RootGM::BooleanSolid::fgkIntersectionChar = '*'; 
const char RootGM::BooleanSolid::fgkSubtractionChar = '-'; 
const char RootGM::BooleanSolid::fgkUnionChar = '+'; 
const char RootGM::BooleanSolid::fgkSeparator = ':'; 

//_____________________________________________________________________________
RootGM::BooleanSolid::BooleanSolid(
                             const std::string& name, 
                             VGM::BooleanType boolType,
                             VGM::ISolid* solidA, VGM::ISolid* solidB,
			     TGeoMatrix* displacement) 
  : BaseVGM::VBooleanSolid(),
    fCompositeShape(0) 
{
//  
  // Call common constructor function
  CreateBooleanSolid(name, boolType, solidA, solidB, displacement);  
}

//_____________________________________________________________________________
RootGM::BooleanSolid::BooleanSolid(TGeoCompositeShape* compositeShape)
  : BaseVGM::VBooleanSolid(),
    fCompositeShape(compositeShape) 
{
//  
  RootGM::SolidMap::Instance()->AddSolid(this, fCompositeShape); 
}

//_____________________________________________________________________________
RootGM::BooleanSolid::~BooleanSolid() {
//
}

// 
// private methods
//

//_____________________________________________________________________________
void RootGM::BooleanSolid::CreateBooleanSolid(
                                 const std::string& name, 
                                 VGM::BooleanType boolType,
                                 VGM::ISolid* solidA, VGM::ISolid* solidB, 
                                 TGeoMatrix* displacementB)
{
//  
  // Get solids from the volumes map
  TGeoShape* rootSolidA = RootGM::SolidMap::Instance()->GetSolid(solidA);
  TGeoShape* rootSolidB = RootGM::SolidMap::Instance()->GetSolid(solidB);

  // Register TGeo matrix
  displacementB->SetName(name.data());
  displacementB->RegisterYourself();
  
  // Expression
  // Requires unique solid names in geometry 
  std::string solidNameA = rootSolidA->GetName();
  std::string solidNameB = rootSolidB->GetName();
  std::string matrixNameB = displacementB->GetName();
  
  char sign;
  switch (boolType) {
    case VGM::kIntersection:  sign = fgkIntersectionChar; break;					 
    case VGM::kSubtraction:   sign = fgkSubtractionChar;  break;
    case VGM::kUnion:         sign = fgkUnionChar;        break;
    case VGM::kUnknownBoolean:
    default:
      std::cerr << "    RootGM::BooleanSolid::BooleanSolid: "<< std::endl;
      std::cerr << "    Unknown Boolean solid type" << std::endl;
      std::cerr << "*** Error: Aborting execution  ***" << std::endl; 
      exit(1);
  }  
  
  std::string expression 
    = solidNameA + sign + solidNameB + fgkSeparator + matrixNameB;   		

  //std::cout << "RootGM::BooleanSolid::BooleanSolid with expression: "
  //          << expression << std::endl;

  // Create composite shape
  fCompositeShape = new TGeoCompositeShape(name.data(), expression.data());

  RootGM::SolidMap::Instance()->AddSolid(this, fCompositeShape); 
}

/*
//_____________________________________________________________________________
HepTransform3D  RootGM::BooleanSolid::Displacement() const
{
// Returns the solid displacemnt transformation
// in the frame of the first (left) solid.
// ---


  //TGeoBoolNode* boolNode = fCompositeShape->GetBoolNode();
  //
  //TGeoMatrix* matrixA = boolNode->GetLeftMatrix();
  //TGeoMatrix* matrixB = boolNode->GetRightMatrix();
  //
  //return RootGM::Convert(matrixB, matrixA, false, true);

  TGeoBoolNode* boolNode = fCompositeShape->GetBoolNode();
  
  TGeoMatrix* matrixA = boolNode->GetLeftMatrix();
  TGeoMatrix* matrixB = boolNode->GetRightMatrix();
  
  HepTransform3D transformA = RootGM::Convert(matrixA);
  HepTransform3D transformB = RootGM::Convert(matrixB);

  // If constituents are composite shapes,
  // the displacement have to take into account the transformation
  // of left constituent not passed to the solid

  HepTransform3D totalTransformA = transformA;
  TGeoShape* shapeA = boolNode->GetLeftShape();
  while (shapeA->IsComposite()) { 
      
     TGeoBoolNode* boolNodeAC 
       = ((TGeoCompositeShape*)shapeA)->GetBoolNode();
      
     TGeoShape* shapeAC = boolNodeAC->GetLeftShape();
          // left component of the shape A 

     TGeoMatrix* matrixAC = boolNodeAC->GetLeftMatrix();
     HepTransform3D transformAC = RootGM::Convert(matrixAC);
    
     totalTransformA = totalTransformA * transformAC;
     
     shapeA = shapeAC;
  }
  
  HepTransform3D totalTransformB = transformB;
  TGeoShape* shapeB = boolNode->GetRightShape();
  while (shapeB->IsComposite()) { 
      
     TGeoBoolNode* boolNodeBC 
       = ((TGeoCompositeShape*)shapeB)->GetBoolNode();
      
     TGeoShape* shapeBC = boolNodeBC->GetLeftShape();
          // left component of the shape B 

     TGeoMatrix* matrixBC = boolNodeBC->GetLeftMatrix();
     HepTransform3D transformBC = RootGM::Convert(matrixBC);
    
     totalTransformB = totalTransformB * transformBC;
     
     shapeB = shapeBC;
  }
  
  return totalTransformA.inverse() * totalTransformB;
}  
*/

//_____________________________________________________________________________
TGeoHMatrix  RootGM::BooleanSolid::Displacement() const
{
// Returns the solid displacemnt transformation
// in the frame of the first (left) solid.
// ---

  TGeoBoolNode* boolNode = fCompositeShape->GetBoolNode();
  
  TGeoMatrix* matrixA = boolNode->GetLeftMatrix();
  TGeoMatrix* matrixB = boolNode->GetRightMatrix();
  
  TGeoHMatrix transformA(*matrixA);
  TGeoHMatrix transformB(*matrixB);

  // If constituents are composite shapes,
  // the displacement have to take into account the transformation
  // of left constituent not passed to the solid

  TGeoHMatrix totalTransformA(transformA);
  TGeoShape* shapeA = boolNode->GetLeftShape();
  while (shapeA->IsComposite()) { 
      
     TGeoBoolNode* boolNodeAC 
       = ((TGeoCompositeShape*)shapeA)->GetBoolNode();
      
     TGeoShape* shapeAC = boolNodeAC->GetLeftShape();
          // left component of the shape A 

     TGeoMatrix* matrixAC = boolNodeAC->GetLeftMatrix();
     TGeoHMatrix transformAC(*matrixAC);
    
     totalTransformA = totalTransformA * transformAC;
     
     shapeA = shapeAC;
  }
  
  TGeoHMatrix totalTransformB(transformB);
  TGeoShape* shapeB = boolNode->GetRightShape();
  while (shapeB->IsComposite()) { 
      
     TGeoBoolNode* boolNodeBC 
       = ((TGeoCompositeShape*)shapeB)->GetBoolNode();
      
     TGeoShape* shapeBC = boolNodeBC->GetLeftShape();
          // left component of the shape B 

     TGeoMatrix* matrixBC = boolNodeBC->GetLeftMatrix();
     TGeoHMatrix transformBC(*matrixBC);
    
     totalTransformB = totalTransformB * transformBC;
     
     shapeB = shapeBC;
  }
  
  return totalTransformA.Inverse() * totalTransformB;
}  

// 
// public methods
//

//_____________________________________________________________________________
std::string 
RootGM::BooleanSolid::Name() const
{
// Returns the Boolean solid name
// ---

  return fCompositeShape->GetName();
}  

//_____________________________________________________________________________
VGM::BooleanType 
RootGM::BooleanSolid::BoolType() const
{
// Returns the Boolean solid type name
// ---

  TGeoBoolNode* boolNode = fCompositeShape->GetBoolNode();

  if (boolNode->GetBooleanOperator() == TGeoBoolNode::kGeoIntersection) 
    return VGM::kIntersection;

  if (boolNode->GetBooleanOperator() == TGeoBoolNode::kGeoSubtraction) 
    return VGM::kSubtraction;

  if (boolNode->GetBooleanOperator() == TGeoBoolNode::kGeoUnion) 
    return VGM::kUnion;
    
  return VGM::kUnknownBoolean;
}    

//_____________________________________________________________________________
VGM::ISolid* 
RootGM::BooleanSolid::ConstituentSolidA() const
{
// Returns the first constituent solid.
// ---

  TGeoBoolNode* boolNode = fCompositeShape->GetBoolNode();

  TGeoShape* rootSolidA = boolNode->GetLeftShape();
  VGM::ISolid* solidA = RootGM::SolidMap::Instance()->GetSolid(rootSolidA);
 
  return solidA;
} 

//_____________________________________________________________________________
VGM::ISolid* 
RootGM::BooleanSolid::ConstituentSolidB() const
{
// Returns the first constituent solid.
// ---

  TGeoBoolNode* boolNode = fCompositeShape->GetBoolNode();

  TGeoShape* rootSolidB = boolNode->GetRightShape();
  VGM::ISolid* solidB = RootGM::SolidMap::Instance()->GetSolid(rootSolidB);
 
  return solidB;
} 

//_____________________________________________________________________________
VGM::Rotation 
RootGM::BooleanSolid::DisplacementObjectRotation() const
{
// Returns the displacemnt object rotation.
// ---

  return Rotation(Displacement());     
}    

//_____________________________________________________________________________
VGM::Rotation 
RootGM::BooleanSolid::DisplacementFrameRotation() const
{
// Returns the displacemnt frame rotation.
// ---

  return Rotation(Displacement().Inverse());
}    
//_____________________________________________________________________________
VGM::ThreeVector 
RootGM::BooleanSolid::DisplacementObjectTranslation() const
{
// Returns the  displacemnt object translation.
// ---

  return Translation(Displacement());     
}    

//_____________________________________________________________________________
VGM::ThreeVector 
RootGM::BooleanSolid::DisplacementFrameTranslation() const
{
// Returns the  displacemnt frame translation.
// ---

   VGM::ThreeVector objTranslation = DisplacementObjectTranslation();
   for (Int_t i=0; i<3; i++) objTranslation[i] = - objTranslation[i];
   
   return objTranslation;
}    

//_____________________________________________________________________________
bool  RootGM::BooleanSolid::DisplacementReflectionZ() const
{
// Returns true if solid displacement trasformation
// includes reflection.

  return HasReflection(Displacement());     
}  

//_____________________________________________________________________________
TGeoShape* 
RootGM::BooleanSolid::GetConstituentSolid(
                               int index, 
                               TGeoCompositeShape* compositeShape)
{
// Returns the constituent shape specified by index.
// ---

  TGeoBoolNode* boolNode = compositeShape->GetBoolNode();

  if (index == 0)
    return boolNode->GetLeftShape();
  else if (index == 1)
    return boolNode->GetRightShape();
  else  {  
    std::cerr << "    RootGM::BooleanSolid::GetConstituentSolid: " << std::endl;
    std::cerr << "    Index out of scope <0, 1>" << std::endl;
    std::cerr << "*** Error: Aborting execution  ***" << std::endl; 
    exit(1); 
  }   
}			       
			       
