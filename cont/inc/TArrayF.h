// @(#)root/cont:$Name:  $:$Id: TArrayF.h,v 1.6 2002/04/04 10:28:35 brun Exp $
// Author: Rene Brun   06/03/95

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TArrayF
#define ROOT_TArrayF


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TArrayF                                                              //
//                                                                      //
// Array of floats (32 bits per element).                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TArray
#include "TArray.h"
#endif


class TArrayF : public TArray {

public:
   Float_t    *fArray;       //[fN] Array of fN floats

   TArrayF();
   TArrayF(Int_t n);
   TArrayF(Int_t n, const Float_t *array);
   TArrayF(const TArrayF &array);
   TArrayF    &operator=(const TArrayF &rhs);
   virtual    ~TArrayF();

   void        Adopt(Int_t n, Float_t *array);
   void        AddAt(Float_t c, Int_t i);
   Float_t     At(Int_t i) const ;
   void        Copy(TArrayF &array) {array.Set(fN); for (Int_t i=0;i<fN;i++) array.fArray[i] = fArray[i];}
   Float_t    *GetArray() const { return fArray; }
   Stat_t      GetSum() const {Stat_t sum=0; for (Int_t i=0;i<fN;i++) sum+=fArray[i]; return sum;}
   void        Reset(Float_t val=0)  {for (Int_t i=0;i<fN;i++) fArray[i] = val;}
   void        Set(Int_t n);
   void        Set(Int_t n, const Float_t *array);
   Float_t    &operator[](Int_t i);
   Float_t     operator[](Int_t i) const;
     
   ClassDef(TArrayF,1)  //Array of floats
};

inline Float_t TArrayF::At(Int_t i) const
{
   if (!BoundsOk("TArrayF::At", i))
      i = 0;
   return fArray[i];
}

inline Float_t &TArrayF::operator[](Int_t i)
{
   if (!BoundsOk("TArrayF::operator[]", i))
      i = 0;
   return fArray[i];
}

inline Float_t TArrayF::operator[](Int_t i) const
{
   if (!BoundsOk("TArrayF::operator[]", i))
      i = 0;
   return fArray[i];
}

#endif
