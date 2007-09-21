// @(#)root/mathcore:$Name:  $:$Id: inc/Math/NumGradFunction.h,v 1.0 2006/01/01 12:00:00 moneta Exp $
// Author: L. Moneta Wed Dec 20 14:36:31 2006

/**********************************************************************
 *                                                                    *
 * Copyright (c) 2006  LCG ROOT Math Team, CERN/PH-SFT                *
 *                                                                    *
 *                                                                    *
 **********************************************************************/

// Header file for class NumGradFunction

#ifndef ROOT_Math_NumGradFunction
#define ROOT_Math_NumGradFunction


#ifndef ROOT_Math_IFunction
#include "Math/IFunction.h"
#endif

#ifndef ROOT_Math_WrappedFunction
#include "Math/WrappedFunction.h"
#endif

#ifndef ROOT_Math_Derivator
#include "Math/Derivator.h"
#endif

namespace ROOT { 

   namespace Math { 


/** 
   NumGradMultiFunction class to wrap a normal function in  a
   gradient function using numerical gradient calculation


   @ingroup MultiMin
*/ 
class MultiNumGradFunction : public IMultiGradFunction { 

public: 


   /** 
     Constructor from a IMultiGenFunction interface
   */ 
   MultiNumGradFunction (const IMultiGenFunction & f) : 
      fFunc(&f), 
      fDim(f.NDim() ),
      fOwner(false)
   {}

   /** 
     Constructor from a generic function (pointer or reference) and number of dimension
     implementiong operator () (double * x)
   */ 

   template<class FuncType> 
   MultiNumGradFunction (FuncType f, int n) : 
      fDim( n ), 
      fOwner(true)
   {
      // create a wrapped function
      fFunc = new ROOT::Math::WrappedMultiFunction<FuncType> (f, n);
   }

   /** 
      Destructor (no operations)
   */ 
   ~MultiNumGradFunction ()  {
      if (fOwner) delete fFunc; 
   }  


   // method inheritaed from IFunction interface 

   unsigned int NDim() const { return fDim; } 

   IMultiGenFunction * Clone() const { 
      if (!fOwner) 
         return new MultiNumGradFunction(*fFunc); 
      else { 
         // we need to copy the pointer to the wrapped function
         MultiNumGradFunction * f =  new MultiNumGradFunction(*(fFunc->Clone()) );
         f->fOwner = true; 
         return f;
      }
   }


private: 


   double DoEval(const double * x) const { 
      return (*fFunc)(x); 
   }

   // calculate derivative using mathcore derivator 
   double DoDerivative (const double * x, unsigned int icoord  ) const { 
      static double step = 1.0E-8; 
      return ROOT::Math::Derivator::Eval(*fFunc, x, icoord, step); 
   }  

   // adapat internal function type to IMultiGenFunction needed by derivative calculation
   const IMultiGenFunction * fFunc;  
   unsigned int fDim; 
   bool fOwner; 
}; 

   } // end namespace Math

} // end namespace ROOT


#endif /* ROOT_Math_NumGradFunction */
