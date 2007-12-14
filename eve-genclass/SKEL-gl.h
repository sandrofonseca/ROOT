// @(#)root/eve:$Id$
// Author: Matevz Tadel 2007

/*************************************************************************
 * Copyright (C) 1995-2007, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_CLASS
#define ROOT_CLASS

#include "TGLObject.h"

class TGLViewer;
class TGLScene;

class STEM;

class CLASS : public TGLObject
{
private:
   CLASS(const CLASS&);            // Not implemented
   CLASS& operator=(const CLASS&); // Not implemented

protected:
   STEM             *fM;  // Model object.

public:
   CLASS();
   virtual ~CLASS() {}

   virtual Bool_t SetModel(TObject* obj, const Option_t* opt=0);
   virtual void   SetBBox();

   virtual void DirectDraw(TGLRnrCtx & rnrCtx) const;

   // To support two-level selection
   // virtual Bool_t SupportsSecondarySelect() const { return kTRUE; }
   // virtual void ProcessSelection(TGLRnrCtx & rnrCtx, TGLSelectRecord & rec);

   ClassDef(CLASS, 0); // GL renderer class for STEM.
};

#endif
