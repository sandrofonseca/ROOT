// @(#)root/base:$Name:  $:$Id: TRandom.h,v 1.3 2002/03/28 09:22:36 brun Exp $
// Author: Rene Brun   15/12/95

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TRandom
#define ROOT_TRandom



//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TRandom                                                              //
//                                                                      //
// Simple prototype random number generator class.                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TNamed
#include "TNamed.h"
#endif

class TRandom : public TNamed {

protected:
   UInt_t   fSeed;  //Random number generator seed

public:
   TRandom(UInt_t seed=65539);
   virtual ~TRandom();
   virtual  Int_t    Binomial(Int_t ntot, Double_t prob);
   virtual  Double_t Exp(Double_t tau);
   virtual  Double_t Gaus(Double_t mean=0, Double_t sigma=1);
   virtual  UInt_t   GetSeed() {return fSeed;}
   virtual  UInt_t   Integer(UInt_t imax);
   virtual  Double_t Landau(Double_t mean=0, Double_t sigma=1);
   virtual  Int_t    Poisson(Double_t mean);
   virtual  Double_t PoissonD(Double_t mean);
   virtual  void     Rannor(Float_t &a, Float_t &b);
   virtual  void     Rannor(Double_t &a, Double_t &b);
   virtual  void     ReadRandom(const char *filename);
   virtual  void     SetSeed(UInt_t seed=65539);
   virtual  Double_t Rndm(Int_t i=0);
   virtual  void     RndmArray(Int_t n, Double_t *array);
   virtual  Double_t Uniform(Double_t x1=1);
   virtual  Double_t Uniform(Double_t x1, Double_t x2);
   virtual  void     WriteRandom(const char *filename);

   ClassDef(TRandom,1)  // Random number generators
};

R__EXTERN TRandom *gRandom;

#endif
