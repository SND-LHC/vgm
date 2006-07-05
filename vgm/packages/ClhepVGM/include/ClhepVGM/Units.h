// $Id$
/// \ingroup ClhepVGM
//
/// \class ClhepVGM::Units
/// 
/// Conversion from CLHEP physical units to VGM units..
///
/// Author: Ivana Hrivnacova; IPN Orsay

#ifndef CLHEP_VGM_UNITS_H
#define CLHEP_VGM_UNITS_H

#include "VGM/common/Axis.h"

namespace ClhepVGM {

  class Units
  {
    public:
      // --> protected
      // Units();  
      virtual ~Units();
  
                    /// Return CLHEP default length unit in VGM units
		    ///
      static double Length(); 

                    /// Convert CLHEP length unit in VGM units
		    ///
      static double Length(double clhepUnit); 

                    /// Return CLHEP default angle unit in VGM units
		    ///
      static double Angle(); 

                    /// Convert CLHEP angle unit in VGM units
		    ///
      static double Angle(double clhepUnit); 

                    /// Return CLHEP default mass density unit in VGM units
		    ///
      static double MassDensity(); 
      
                    /// Convert CLHEP mass density unit in VGM units
		    ///
      static double MassDensity(double clhepUnit); 

                    /// Return CLHEP default atomic weight unit in VGM units
		    ///
      static double AtomicWeight();     

                    /// Convert CLHEP  atomic weight  unit in VGM units
		    ///
      static double AtomicWeight(double clhepUnit);     
      
                    /// Convert CLHEP  default unit for given axis type
		    /// in VGM units
		    ///
      static double AxisUnit(VGM::Axis axis); 

    protected:
      Units();      
          // only static data members and methods

    private:
      // static data members  
      static const double fgkLength; 
      static const double fgkAngle;  
      static const double fgkMassDensity;
      static const double fgkAtomicWeight;
  }; 
  
}      

#endif //CLHEP_VGM_UNITS_H
