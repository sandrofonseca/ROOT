// @(#)root/tree:$Name:  $:$Id: TNtuple.h,v 1.3 2000/10/20 15:52:49 rdm Exp $
// Author: Rene Brun   06/04/96

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TNtuple
#define ROOT_TNtuple


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TNtuple                                                              //
//                                                                      //
// A simple tree with branches of floats.                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TTree
#include "TTree.h"
#endif

class TBrowser;

class TNtuple : public TTree {

protected:
   Int_t       fNvar;            //  Number of columns
   Float_t    *fArgs;            //! [fNvar] Array of variables

   virtual Int_t  Fill();

public:
   TNtuple();
   TNtuple(const char *name,const char *title, const char *varlist, Int_t bufsize=32000);
   virtual ~TNtuple();

   virtual void    Browse(TBrowser *b);
   virtual Int_t   Fill(Float_t *x);
   virtual Int_t   Fill(Float_t x0, Float_t x1, Float_t x2=0, Float_t x3=0,
                        Float_t x4=0, Float_t x5=0, Float_t x6=0, Float_t x7=0,
                        Float_t x8=0, Float_t x9=0, Float_t x10=0,
                        Float_t x11=0, Float_t x12=0, Float_t x13=0,
                        Float_t x14=0);
   virtual Int_t   GetNvar() const { return fNvar; }
         Float_t  *GetArgs() const { return fArgs; }
            void   ResetBranchAddresses();

   ClassDef(TNtuple,2)  //A simple tree with branches of floats.
};

#endif
