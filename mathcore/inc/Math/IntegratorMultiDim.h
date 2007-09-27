// @(#)root/mathcore:$Id$
// Authors: M. Slawinska   08/2007 

/**********************************************************************
 *                                                                    *
 * Copyright (c) 2007 , LCG ROOT MathLib Team                         *
 *                                                                    *
 *                                                                    *
 **********************************************************************/

// Header source file for class IntegratorMultiDim


#ifndef ROOT_Math_IntegratorMultiDim
#define ROOT_Math_IntegratorMultiDim

#ifndef ROOT_Math_IFunctionfwd
#include "Math/IFunctionfwd.h"
#endif



namespace ROOT {
namespace Math {


/**
   class for adaptive quadrature integration in multi-dimensions
   Algorithm from  A.C. Genz, A.A. Malik, 
    1.A.C. Genz and A.A. Malik, An adaptive algorithm for numerical integration over 
    an N-dimensional rectangular region, J. Comput. Appl. Math. 6 (1980) 295-302.

   Converted/adapted by R.Brun to C++ from Fortran CERNLIB routine RADMUL (D120)
   The new code features many changes compared to the Fortran version.
  
 */

class IntegratorMultiDim {
   public:
   // constructors
   explicit 
   IntegratorMultiDim(unsigned int dim, double absTol = 1.E-6, double relTol = 1E-6, size_t size = 100000);

   IntegratorMultiDim(const IMultiGenFunction &f, unsigned int dim, double absTol = 1.E-9, double relTol = 1E-6, size_t size = 100000);


   virtual ~IntegratorMultiDim() {}

   //private:
   //   Integrator(const Integrator &);
   //   Integrator & operator=(const Integrator &);


   /**
      evaluate the integral with the previously given function between xmin[] and xmax[]  
   */
   double Integral(unsigned int dim, double* xmin, double * xmax);

   /// evaluate the integral passing a new function
   double Integral(const IMultiGenFunction &f, unsigned int dim, double* xmin, double * xmax);

   void SetFunction(const IMultiGenFunction &f);
 
   /// return result of integration 
   double Result() const { return fResult; }

   // return integration error 
   double Error() const { return fError; } 

   // return number of function evaluations in calculating the integral 
   unsigned int NEval() const { return fNEval; }
 
   void SetRelTolerance(double relTol);
   void SetAbsTolerance(double absTol);


 private:

   unsigned int fdim; // dimentionality of integrand

   double fAbsTol;
   double fRelTol;
   size_t fSize;

   double fResult;
   double fError;
   unsigned int  fNEval;

   const IMultiGenFunction* fFun;

};

}//namespace Math
}//namespace ROOT

#endif /* ROOT_Math_IntegratorMultiDim */
