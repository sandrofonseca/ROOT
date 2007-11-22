// @(#)root/reve:$Id$
// Authors: Matevz Tadel & Alja Mrak-Tadel: 2006, 2007

/*************************************************************************
 * Copyright (C) 1995-2007, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TEveSceneInfo
#define ROOT_TEveSceneInfo

#include <TEveElement.h>

class TGLSceneBase;
class TGLSceneInfo;

class TEveViewer;
class TEveScene;

class TEveSceneInfo : public TEveElement,
                      public TNamed
{
private:
   TEveSceneInfo(const TEveSceneInfo&);            // Not implemented
   TEveSceneInfo& operator=(const TEveSceneInfo&); // Not implemented

protected:
   TEveViewer       *fViewer;
   TEveScene        *fScene;
   TGLSceneInfo     *fGLSceneInfo;

public:
   TEveSceneInfo(TEveViewer* viewer, TEveScene* scene, TGLSceneInfo* sinfo);
   virtual ~TEveSceneInfo();

   TEveViewer   * GetViewer()      const { return fViewer; }
   TEveScene    * GetScene()       const { return fScene;  }
   TGLSceneInfo * GetGLSceneInfo() const { return fGLSceneInfo; }
   TGLSceneBase * GetGLScene()     const;

   virtual void   SetRnrSelf(Bool_t rnr);
   virtual void   SetRnrState(Bool_t rnr);

   virtual Bool_t AcceptRenderElement(TEveElement* el);
   virtual Bool_t HandleElementPaste(TEveElement* el);

   ClassDef(TEveSceneInfo, 0); // TEveUtil representation of TGLSceneInfo.
};

#endif
