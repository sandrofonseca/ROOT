// @(#)root/mathmore:$Id$
// Author: Magdalena Slawinska 08/2007

/**********************************************************************
 *                                                                    *
 * Copyright (c) 2007 ROOT Foundation,  CERN/PH-SFT                   *
 *                                                                    *
 * This library is free software; you can redistribute it and/or      *
 * modify it under the terms of the GNU General Public License        *
 * as published by the Free Software Foundation; either version 2     *
 * of the License, or (at your option) any later version.             *
 *                                                                    *
 * This library is distributed in the hope that it will be useful,    *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   *
 * General Public License for more details.                           *
 *                                                                    *
 * You should have received a copy of the GNU General Public License  *
 * along with this library (see file COPYING); if not, write          *
 * to the Free Software Foundation, Inc., 59 Temple Place, Suite      *
 * 330, Boston, MA 02111-1307 USA, or contact the author.             *
 *                                                                    *
 **********************************************************************/
//
// Header file for class GSLMCIntegrator
// 
//

#ifndef ROOT_Math_GSLMCIntegrator
#define ROOT_Math_GSLMCIntegrator

#ifndef ROOT_Math_MCIntegrationTypes
#include "Math/MCIntegrationTypes.h"
#endif

#ifndef ROOT_Math_IFunctionfwd
#include "Math/IFunctionfwd.h"
#endif

#ifndef ROOT_Math_IFunction
#include "Math/IFunction.h"
#endif


#ifndef ROOT_Math_MCIntegrationTypes
#include "Math/MCIntegrationTypes.h"
#endif


#ifndef ROOT_Math_MCParameters
#include "Math/MCParameters.h"
#endif

#ifndef ROOT_Math_VirtualIntegrator
#include "Math/VirtualIntegrator.h"
#endif

#include <iostream>

/**

@defgroup MCIntegration Numerical Monte Carlo Integration
@ingroup NumAlgo

*/

namespace ROOT {
namespace Math {


   
   class GSLMCIntegrationWorkspace;
   class GSLMonteFunctionWrapper;
   class GSLRngWrapper;
   
   
   /**
      
    Class for performing numerical integration of a multidimensional function.
    It uses the numerical integration algorithms of GSL, which reimplements the
    algorithms used in the QUADPACK, a numerical integration package written in Fortran.
    
    Plain MC, MISER and VEGAS integration algorithms are supported for integration over finite (hypercubic) ranges.
    
    <A HREF="http://www.gnu.org/software/gsl/manual/gsl-ref_16.html#SEC248">GSL Manual</A>.
    
    @ingroup MCIntegration

   */
   
   
   class GSLMCIntegrator : public VirtualIntegratorMultiDim {
      
   public:
                  
      // constructors
      
                  
      
      /** constructor of GSL MCIntegrator. VEGAS MC is set as default integration type 
         
      @param type type of integration. The possible types are defined in the Integration::Type enumeration
      @param absTol desired absolute Error
      @param relTol desired relative Error
      @param calls maximum number of function calls
      */
      
      explicit 
      GSLMCIntegrator(MCIntegration::Type type = MCIntegration::VEGAS, double absTol = 1.E-6, double relTol = 1E-4, unsigned int calls = 500000);

      /** constructor of GSL MCIntegrator. VEGAS MC is set as default integration type 
         
      @param type type of integration using a char * (required by plug-in manager) 
      @param absTol desired absolute Error
      @param relTol desired relative Error
      @param calls maximum number of function calls
      */
      
      explicit 
      GSLMCIntegrator(const char *  type, double absTol, double relTol, unsigned int calls);
      
      

      /** destructor */ 
      virtual ~GSLMCIntegrator();
      
      // disable copy ctrs
      
private:
         
      GSLMCIntegrator(const GSLMCIntegrator &);

      GSLMCIntegrator & operator=(const GSLMCIntegrator &);
      
public:
         
         
         // template methods for generic functors
         
         /**
         method to set the a generic integration function
          
          @param f integration function. The function type must implement the assigment operator, <em>  double  operator() (  double  x ) </em>
          
          */
         
         
      void SetFunction(const IMultiGenFunction &f); 


      typedef double ( * GSLMonteFuncPointer ) ( double *, size_t, void *);    
      
      void SetFunction( GSLMonteFuncPointer f, unsigned int dim, void * p = 0 ); 
      
      // methods using GSLMonteFuncPointer
      
      /**
         evaluate the Integral of a function f over the defined hypercube (a,b)
       @param f integration function. The function type must implement the mathlib::IGenFunction interface
       @param a lower value of the integration interval
       @param b upper value of the integration interval
       */
      
      double Integral(const GSLMonteFuncPointer & f, unsigned int dim, double* a, double* b, void * p = 0);
      
      // to be added later    
      //double Integral(const GSLMonteFuncPointer & f);
        
      double Integral(const double* a, const double* b);

      
      //double Integral(GSLMonteFuncPointer f, void * p, double* a, double* b);
  
      /**
         return the type of the integration used
       */
      //MCIntegration::Type MCType() const;   
      
      /**
         return  the Result of the last Integral calculation
       */
      double Result() const;
      
      /**
         return the estimate of the absolute Error of the last Integral calculation
       */
      double Error() const;
      
      /**
         return the Error Status of the last Integral calculation
       */
      int Status() const;
      
      
      // setter for control Parameters  (getters are not needed so far )
      
      /**
         set the desired relative Error
       */
      void SetRelTolerance(double relTolerance);
      
      
      /**
         set the desired absolute Error
       */
      void SetAbsTolerance(double absTolerance);
      
      /**
	 to be added later as options for basic MC methods
       The possible rules are defined in the Integration::GKRule enumeration.
       The integration rule can be modified only for ADAPTIVE type integrations
       */
      //void SetIntegrationRule(Integration::GKRule );
      

      /**
	 set random number generator
      */
      void SetGenerator(GSLRngWrapper* r);

      /**
	 set integration method
      */
      void SetType(MCIntegration::Type type);

      /**
	 set integration mode for VEGAS method
      */

      void SetMode(MCIntegration::Mode mode);
         
      /**
	 set default parameters for VEGAS method
      */
      void SetParameters(const VegasParameters &p);

   
      /**
	 set default parameters for MISER method
      */
      void SetParameters(const MiserParameters &p);

      /**
	 set parameters for PLAIN method
      */
      //void SetPParameters(const PlainParameters &p);   

      /**
	 returns the error sigma from the last iteration of the Vegas algorithm
      */  
      double Sigma();

      /**
	 returns chi-squared per degree of freedom for the estimate of the integral in the Vegas algorithm
      */  
      double ChiSqr();


   protected:
         
      // internal method to check validity of GSL function pointer
      bool CheckFunction(); 

      // set internally the type of integration method
      void DoInitialize( );

      
   private:
      //type of intergation method   
      MCIntegration::Type fType;
       
      //mode for VEGAS integration
      MCIntegration::Mode fMode;
      GSLRngWrapper * fRng;

      double fAbsTol;
      double fRelTol;
      unsigned int fDim;
      unsigned int fCalls;
      
      // cache Error, Result and Status of integration
      
      double fResult;
      double fError;
      int fStatus;
      
      
      GSLMCIntegrationWorkspace * fWorkspace;
      GSLMonteFunctionWrapper * fFunction;
     
   };
   
   



} // namespace Math
} // namespace ROOT


#endif /* ROOT_Math_GSLMCIntegrator */
