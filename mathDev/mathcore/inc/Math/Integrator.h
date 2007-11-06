// @(#)root/mathmore:$Id$
// Authors: L. Moneta, M. Slawinska 10/2007

 /**********************************************************************
  *                                                                    *
  * Copyright (c) 2007 ROOT Foundation,  CERN/PH-SFT                   *
  *                                                                    *
  *                                                                    *
  **********************************************************************/

// Header file for class Integrator
//
//
#ifndef ROOT_Math_Integrator
#define ROOT_Math_Integrator

#ifndef ROOT_Math_IntegrationTypes
#include "Math/AllIntegrationTypes.h"
#endif

#ifndef ROOT_Math_IFunctionfwd
#include "Math/IFunctionfwd.h"
#endif

#ifndef ROOT_Math_VirtualIntegrator
#include "Math/VirtualIntegrator.h"
#endif

#ifndef ROOT_Math_WrappedFunction
#include "Math/WrappedFunction.h"
#endif





/**

@defgroup Integration Numerical Integration

*/



namespace ROOT {
namespace Math {




/**

User Class for performing numerical integration of a function in one dimension.
It uses the plug-in manager to load advanced numerical integration algorithms from GSL, which reimplements the
algorithms used in the QUADPACK, a numerical integration package written in Fortran.

Various types of adaptive and non-adaptive integration are supported. These include
integration over infinite and semi-infinite ranges and singular integrals.

The integration type is selected using the Integration::type enumeration
in the class constructor.
The default type is adaptive integration with singularity
(ADAPTIVESINGULAR or QAGS in the QUADPACK convention) applying a Gauss-Kronrod 21-point integration rule.
In the case of ADAPTIVE type, the integration rule can also be specified via the
Integration::GKRule. The default rule is 31 points.

In the case of integration over infinite and semi-infinite ranges, the type used is always
ADAPTIVESINGULAR applying a transformation from the original interval into (0,1).

The ADAPTIVESINGULAR type is the most sophicticated type. When performances are
important, it is then recommened to use the NONADAPTIVE type in case of smooth functions or
 ADAPTIVE with a lower Gauss-Kronrod rule.

For detailed description on GSL integration algorithms see the
<A HREF="http://www.gnu.org/software/gsl/manual/gsl-ref_16.html#SEC248">GSL Manual</A>.


  @ingroup Integration

*/


class IntegratorOneDim {

public:



    // constructors


    /** Constructor of one dimensional Integrator, default type is adaptive

       @param type   integration type (adaptive, non-adaptive, etc..)
       @param absTol desired absolute Error
       @param relTol desired relative Error
       @param size maximum number of sub-intervals
       @param rule  Gauss-Kronrod integration rule (only for GSL ADAPTIVE type)  
                     
       Possible rule values are GAUS15 (rule = 1), GAUS21( rule = 2), GAUS31(rule =3), GAUS41 (rule=4), GAUS51 (rule =5), GAUS61(rule =6)
       lower rules are indicated for singular funcitons while higher for smooth functions to get better accuracies
    */
    explicit
    IntegratorOneDim(IntegrationOneDim::Type type = IntegrationOneDim::ADAPTIVE, double absTol = 1.E-9, double relTol = 1E-6, unsigned int size = 1000, unsigned int rule = 3) { 
       fIntegrator = CreateIntegrator(type, absTol, relTol, size, rule); 
    }
    
    /** Constructor of one dimensional Integrator passing a function interface

       @param f      integration function (1D interface)
       @param type   integration type (adaptive, non-adaptive, etc..)
       @param absTol desired absolute Error
       @param relTol desired relative Error
       @param size maximum number of sub-intervals
       @param rule Gauss-Kronrod integration rule (only for GSL ADAPTIVE type)  
    */
   explicit 
   IntegratorOneDim(const IGenFunction &f, IntegrationOneDim::Type type = IntegrationOneDim::ADAPTIVE, double absTol = 1.E-9, double relTol = 1E-6, unsigned int size = 1000, int rule = 3) { 
      fIntegrator = CreateIntegrator(type, absTol, relTol, size, rule); 
       SetFunction(f);
   }

    /** Template Constructor of one dimensional Integrator passing a generic function object

       @param f      integration function (any C++ callable object implementing operator()(double x)
       @param type   integration type (adaptive, non-adaptive, etc..)
       @param absTol desired absolute Error
       @param relTol desired relative Error
       @param size maximum number of sub-intervals
       @param rule Gauss-Kronrod integration rule (only for GSL ADAPTIVE type)  
    */
#ifdef LATER
   template<class Function>
   explicit
   IntegratorOneDim(Function f, IntegrationOneDim::Type type = IntegrationOneDim::ADAPTIVE, double absTol = 1.E-9, double relTol = 1E-6, unsigned int size = 1000, int rule = 3) { 
      fIntegrator = CreateIntegrator(type, absTol, relTol, size, rule); 
       SetFunction(f);
   }
#endif

   /// destructor (will delete contained pointer)
   virtual ~IntegratorOneDim() { 
      if (fIntegrator) delete fIntegrator;
   }

   // disable copy constructur and assignment operator 

private:
   IntegratorOneDim(const IntegratorOneDim &) {}
   IntegratorOneDim & operator=(const IntegratorOneDim &) { return *this; }

public:


   // template methods for generic functors

   /**
      method to set the a generic integration function      
      @param f integration function. The function type must implement the assigment operator, <em>  double  operator() (  double  x ) </em>

   */
   template<class Function>
   void SetFunction(Function f) { 
      ROOT::Math::WrappedFunction<Function> wf(f); 
      // need to copy the wrapper function, the instance created here will be deleted after SetFunction()
      if (fIntegrator) fIntegrator->SetFunction(wf, true);
   }


   /** 
       set one dimensional function for 1D integration
    */
   //template<>
   //void SetFunction<const ROOT::Math::IGenFunction &> (const IGenFunction &f) { 
   void SetFunction  (const IGenFunction &f) { 
      if (fIntegrator) fIntegrator->SetFunction(f);
   }
   

    // integration methods using a function 

    /**
       evaluate the Integral of a function f over the defined interval (a,b)
       @param f integration function. The function type must be a C++ callable object implementing operator()(double x)
       @param a lower value of the integration interval
       @param b upper value of the integration interval
    */

   template<class Function> 
   double Integral(Function f, double a, double b) { 
      SetFunction(f); 
      return Integral(a,b);
   }


    /**
       evaluate the Integral of a function f over the defined interval (a,b)
       @param f integration function. The function type must implement the mathlib::IGenFunction interface
       @param a lower value of the integration interval
       @param b upper value of the integration interval
    */
   double Integral(const IGenFunction & f, double a, double b) { 
      SetFunction(f); 
      return Integral(a,b);
   }



   /**
      evaluate the Integral of a function f over the infinite interval (-inf,+inf)
      @param f integration function. The function type must implement the mathlib::IGenFunction interface
   */
   double Integral(const IGenFunction & f) { 
      SetFunction(f); 
      return Integral(); 
   }

   /**
      evaluate the Integral of a function f over the semi-infinite interval (a,+inf)
      @param f integration function. The function type must implement the mathlib::IGenFunction interface
      @param a lower value of the integration interval

   */
   double IntegralUp(const IGenFunction & f, double a ) { 
      SetFunction(f); 
      return IntegralUp(a); 
   }

   /**
      evaluate the Integral of a function f over the over the semi-infinite interval (-inf,b)
      @param f integration function. The function type must implement the mathlib::IGenFunction interface
      @param b upper value of the integration interval
   */
   double IntegralLow(const IGenFunction & f, double b ) { 
      SetFunction(f); 
      return IntegralLow(b); 
   }

   /**
      evaluate the Integral of a function f with known singular points over the defined Integral (a,b)
      @param f integration function. The function type must implement the mathlib::IGenFunction interface
      @param pts vector containing both the function singular points and the lower/upper edges of the interval. The vector must have as first element the lower edge of the integration Integral ( \a a) and last element the upper value.

   */
   double Integral(const IGenFunction & f, const std::vector<double> & pts ) { 
      SetFunction(f); 
      return Integral(pts); 
   }

   /**
      evaluate the Cauchy principal value of the integral of  a function f over the defined interval (a,b) with a singularity at c 

   */
   double IntegralCauchy(const IGenFunction & f, double a, double b, double c) { 
      SetFunction(f); 
      return IntegralCauchy(a,b,c); 
   }



   // integration method using cached function

   /**
      evaluate the Integral over the defined interval (a,b) using the function previously set with Integrator::SetFunction method
      @param a lower value of the integration interval
      @param b upper value of the integration interval
   */

   double Integral(double a, double b) { 
      return fIntegrator == 0 ? 0 : fIntegrator->Integral(a,b);
   }


   /**
      evaluate the Integral over the infinite interval (-inf,+inf) using the function previously set with Integrator::SetFunction method.
   */

   double Integral( ) { 
      return fIntegrator == 0 ? 0 : fIntegrator->Integral();
   }

   /**
      evaluate the Integral of a function f over the semi-infinite interval (a,+inf) using the function previously set with Integrator::SetFunction method.
      @param a lower value of the integration interval
   */
   double IntegralUp(double a ) { 
      return fIntegrator == 0 ? 0 : fIntegrator->IntegralUp(a);
   }

   /**
      evaluate the Integral of a function f over the over the semi-infinite interval (-inf,b) using the function previously set with Integrator::SetFunction method.
      @param b upper value of the integration interval
   */
   double IntegralLow( double b ) { 
      return fIntegrator == 0 ? 0 : fIntegrator->IntegralLow(b);
   }

   /**
      evaluate the Integral over the defined interval (a,b) using the function previously set with Integrator::SetFunction method. The function has known singular points.
      @param pts vector containing both the function singular points and the lower/upper edges of the interval. The vector must have as first element the lower edge of the integration Integral ( \a a) and last element the upper value.

   */
   double Integral( const std::vector<double> & pts) { 
      return fIntegrator == 0 ? 0 : fIntegrator->Integral(pts);
   }

   /**
      evaluate the Cauchy principal value of the integral of  a function f over the defined interval (a,b) with a singularity at c 

   */
   double IntegralCauchy(double a, double b, double c) { 
      return fIntegrator == 0 ? 0 : fIntegrator->IntegralCauchy(a,b,c);
   }

   /**
      return  the Result of the last Integral calculation
   */
   double Result() const { return fIntegrator == 0 ? 0 : fIntegrator->Result(); }

   /**
      return the estimate of the absolute Error of the last Integral calculation
   */
   double Error() const { return fIntegrator == 0 ? 0 : fIntegrator->Error(); }

   /**
      return the Error Status of the last Integral calculation
   */
   int Status() const { return fIntegrator == 0 ? -1 : fIntegrator->Status(); }


   // setter for control Parameters  (getters are not needed so far )

   /**
      set the desired relative Error
   */
   void SetRelTolerance(double relTolerance) { if (fIntegrator) fIntegrator->SetRelTolerance(relTolerance); }


   /**
      set the desired absolute Error
   */
   void SetAbsTolerance(double absTolerance) { if (fIntegrator) fIntegrator->SetRelTolerance(absTolerance); }

   /**
      return a pointer to integrator object 
   */
   VirtualIntegratorOneDim * GetIntegrator() { return fIntegrator; }  


protected: 

   VirtualIntegratorOneDim * CreateIntegrator(IntegrationOneDim::Type type , double absTol, double relTol, unsigned int size, int rule);

private:

   VirtualIntegratorOneDim * fIntegrator;

};


   typedef IntegratorOneDim Integrator; 


} // namespace Math
} // namespace ROOT


#endif /* ROOT_Math_Integrator */
