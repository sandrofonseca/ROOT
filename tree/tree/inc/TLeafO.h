// @(#)root/tree:$Id$
// Author: Philippe Canal  20/1/05

/*************************************************************************
 * Copyright (C) 1995-2005, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TLeafO
#define ROOT_TLeafO


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TLeafO                                                               //
//                                                                      //
// A TLeaf for a bool data type.                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TLeaf
#include "TLeaf.h"
#endif

class TLeafO : public TLeaf {

protected:
   Bool_t       fMinimum;         //Minimum value if leaf range is specified
   Bool_t       fMaximum;         //Maximum value if leaf range is specified
   Bool_t       *fValue;          //!Pointer to data buffer
   Bool_t       **fPointer;       //!Address of a pointer to data buffer!

public:
   TLeafO();
   TLeafO(TBranch *parent, const char *name, const char *type);
   virtual ~TLeafO();

   virtual void    Export(TClonesArray *list, Int_t n);
   virtual void    FillBasket(TBuffer &b);
   virtual Int_t   GetMaximum() const {return fMaximum;}
   virtual Int_t   GetMinimum() const {return fMinimum;}
   const char     *GetTypeName() const;
   Double_t        GetValue(Int_t i=0) const;
   virtual void   *GetValuePointer() const {return fValue;}
   virtual void    Import(TClonesArray *list, Int_t n);
   virtual void    PrintValue(Int_t i=0) const;
   virtual void    ReadBasket(TBuffer &b);
   virtual void    ReadBasketExport(TBuffer &b, TClonesArray *list, Int_t n);
   virtual void    ReadValue(istream & s);
   virtual void    SetAddress(void *add=0);
   virtual void    SetMaximum(Bool_t max) { fMaximum = max; }
   virtual void    SetMinimum(Bool_t min) { fMinimum = min; }

   ClassDef(TLeafO,1);  //A TLeaf for an 8 bit Integer data type.
};

inline Double_t TLeafO::GetValue(Int_t i) const { return fValue[i]; }

#endif
