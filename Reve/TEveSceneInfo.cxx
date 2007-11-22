// @(#)root/reve:$Id$
// Authors: Matevz Tadel & Alja Mrak-Tadel: 2006, 2007

/*************************************************************************
 * Copyright (C) 1995-2007, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include <TEveSceneInfo.h>
#include <TEveScene.h>
#include <TEveManager.h>

#include <TGLSceneInfo.h>

//______________________________________________________________________________
// TEveSceneInfo
//
// TEveUtil representation of TGLSceneInfo.

ClassImp(TEveSceneInfo)

//______________________________________________________________________________
TEveSceneInfo::TEveSceneInfo(TEveViewer* viewer, TEveScene* scene, TGLSceneInfo* sinfo) :
   TEveElement (),
   TNamed        (Form("SI - %s", scene->GetName()),
                  Form("TEveSceneInfo of scene '%s'", scene->GetName())),
   fViewer       (viewer),
   fScene        (scene),
   fGLSceneInfo  (sinfo)
{}

//______________________________________________________________________________
TEveSceneInfo::~TEveSceneInfo()
{}

/******************************************************************************/

//______________________________________________________________________________
TGLSceneBase* TEveSceneInfo::GetGLScene() const
{
   return fGLSceneInfo->GetScene();
}

/******************************************************************************/

//______________________________________________________________________________
void TEveSceneInfo::SetRnrSelf(Bool_t rnr)
{
   TEveElement::SetRnrSelf(rnr);
   fGLSceneInfo->SetActive(fRnrSelf);
}

//______________________________________________________________________________
void TEveSceneInfo::SetRnrState(Bool_t rnr)
{
   TEveElement::SetRnrState(rnr);
   fGLSceneInfo->SetActive(fRnrSelf);
}

/******************************************************************************/

//______________________________________________________________________________
Bool_t TEveSceneInfo::AcceptRenderElement(TEveElement* /*el*/)
{
   static const TEveException eH("TEveSceneInfo::AcceptRenderElement ");

   gReve->SetStatusLine(eH + "this class does not accept children.");
   return kFALSE;
}

//______________________________________________________________________________
Bool_t TEveSceneInfo::HandleElementPaste(TEveElement* /*el*/)
{
   static const TEveException eH("TEveSceneInfo::HandleElementPaste ");

   gReve->SetStatusLine(eH + "this class does not accept children.");
   return kFALSE;
}
