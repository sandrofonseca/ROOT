// @(#)root/eve:$Id$
// Authors: Matevz Tadel & Alja Mrak-Tadel: 2006, 2007

/*************************************************************************
 * Copyright (C) 1995-2007, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include "TEveTrackPropagatorEditor.h"
#include "TEveTrackPropagator.h"
// #include <TEveTrack.h>

#include "TEveGValuators.h"
#include "TEveManager.h"

#include "TVirtualPad.h"
#include "TColor.h"

#include "TGLabel.h"
#include "TG3DLine.h"
#include "TGButton.h"
#include "TGNumberEntry.h"
#include "TGColorSelect.h"
#include "TGDoubleSlider.h"
#include "TGComboBox.h"
#include "TAttMarkerEditor.h"

//______________________________________________________________________________
// TEveTrackPropagatorSubEditor
//
// Sub-editor for TEveTrackPropagator class.

ClassImp(TEveTrackPropagatorSubEditor)

//______________________________________________________________________________
TEveTrackPropagatorSubEditor::TEveTrackPropagatorSubEditor(const TGWindow *p):
   TGVerticalFrame(p),
   fM (0),

   fMaxR(0),
   fMaxZ(0),
   fMaxOrbits(0),
   fMaxAng(0),
   fDelta(0),

   fRnrFV(0),

   fPMFrame(0),
   fFitDaughters(0),
   fFitReferences(0),
   fFitDecay(0),
   fFitCluster2Ds(0),
   fRnrDaughters(0),
   fRnrReferences(0),
   fRnrDecay(0),
   fRnrCluster2Ds(0),

   fRefsCont(0),
   fPMAtt(0),
   fFVAtt(0)
{
   // Constructor.

   Int_t labelW = 51;

   // --- Limits
   fMaxR = new TEveGValuator(this, "Max R:", 90, 0);
   fMaxR->SetLabelWidth(labelW);
   fMaxR->SetNELength(6);
   fMaxR->Build();
   fMaxR->SetLimits(0.1, TEveTrackPropagator::fgEditorMaxR, 101, TGNumberFormat::kNESRealOne);
   fMaxR->SetToolTip("Maximum radius to which the tracks will be drawn.");
   fMaxR->Connect("ValueSet(Double_t)", "TEveTrackPropagatorSubEditor", this, "DoMaxR()");
   AddFrame(fMaxR, new TGLayoutHints(kLHintsTop, 1, 1, 1, 1));

   fMaxZ = new TEveGValuator(this, "Max Z:", 90, 0);
   fMaxZ->SetLabelWidth(labelW);
   fMaxZ->SetNELength(6);
   fMaxZ->Build();
   fMaxZ->SetLimits(0.1, TEveTrackPropagator::fgEditorMaxZ, 101, TGNumberFormat::kNESRealOne);
   fMaxZ->SetToolTip("Maximum z-coordinate to which the tracks will be drawn.");
   fMaxZ->Connect("ValueSet(Double_t)", "TEveTrackPropagatorSubEditor", this, "DoMaxZ()");
   AddFrame(fMaxZ, new TGLayoutHints(kLHintsTop, 1, 1, 1, 1));

   fMaxOrbits = new TEveGValuator(this, "Orbits:", 90, 0);
   fMaxOrbits->SetLabelWidth(labelW);
   fMaxOrbits->SetNELength(6);
   fMaxOrbits->Build();
   fMaxOrbits->SetLimits(0.1, 10, 101, TGNumberFormat::kNESRealOne);
   fMaxOrbits->SetToolTip("Maximal angular path of tracks' orbits (1 ~ 2Pi).");
   fMaxOrbits->Connect("ValueSet(Double_t)", "TEveTrackPropagatorSubEditor", this, "DoMaxOrbits()");
   AddFrame(fMaxOrbits, new TGLayoutHints(kLHintsTop, 1, 1, 1, 1));

   fMaxAng = new TEveGValuator(this, "Angle:", 90, 0);
   fMaxAng->SetLabelWidth(labelW);
   fMaxAng->SetNELength(6);
   fMaxAng->Build();
   fMaxAng->SetLimits(1, 160, 81, TGNumberFormat::kNESRealOne);
   fMaxAng->SetToolTip("Maximal angular step between two helix points.");
   fMaxAng->Connect("ValueSet(Double_t)", "TEveTrackPropagatorSubEditor", this, "DoMaxAng()");
   AddFrame(fMaxAng, new TGLayoutHints(kLHintsTop, 1, 1, 1, 1));

   fDelta = new TEveGValuator(this, "Delta:", 90, 0);
   fDelta->SetLabelWidth(labelW);
   fDelta->SetNELength(6);
   fDelta->Build();
   fDelta->SetLimits(0.001, 10, 101, TGNumberFormat::kNESRealThree);
   fDelta->SetToolTip("Maximal error at the mid-point of the line connecting to helix points.");
   fDelta->Connect("ValueSet(Double_t)", "TEveTrackPropagatorSubEditor", this, "DoDelta()");
   AddFrame(fDelta, new TGLayoutHints(kLHintsTop, 1, 1, 1, 1));
}

//______________________________________________________________________________
void TEveTrackPropagatorSubEditor::CreateRefsContainer(TGVerticalFrame* p)
{
   // Create a frame containing track-reference controls under parent
   // frame p.

   fRefsCont = new TGCompositeFrame(p, 80, 20, kVerticalFrame);
   fPMFrame  = new TGVerticalFrame(fRefsCont);
   // Rendering control.
   {
      TGGroupFrame* fitPM = new TGGroupFrame(fPMFrame, "PathMarks:", kLHintsTop | kLHintsCenterX);
      fitPM->SetTitlePos(TGGroupFrame::kLeft);
      fPMFrame->AddFrame( fitPM, new TGLayoutHints(kLHintsTop | kLHintsCenterX | kLHintsExpandX, 3, 3, 3, 3));

      TGMatrixLayout *ml = new TGMatrixLayout(fitPM, 0,1,6);
      fitPM->SetLayoutManager(ml);

      fFitDaughters  = new TGCheckButton(fitPM, "Fit Daughters",   TEvePathMark::kDaughter);
      fFitReferences = new TGCheckButton(fitPM, "Fit Refs",        TEvePathMark::kReference);
      fFitDecay      = new TGCheckButton(fitPM, "Fit Decay",       TEvePathMark::kDecay);
      fFitCluster2Ds = new TGCheckButton(fitPM, "Fit 2D Clusters", TEvePathMark::kCluster2D);

      fitPM->AddFrame(fFitDaughters);
      fitPM->AddFrame(fFitReferences);
      fitPM->AddFrame(fFitDecay);
      fitPM->AddFrame(fFitCluster2Ds);

      fFitDecay     ->Connect("Clicked()","TEveTrackPropagatorSubEditor", this, "DoFitPM()");
      fFitReferences->Connect("Clicked()","TEveTrackPropagatorSubEditor", this, "DoFitPM()");
      fFitDaughters ->Connect("Clicked()","TEveTrackPropagatorSubEditor", this, "DoFitPM()");
      fFitCluster2Ds->Connect("Clicked()","TEveTrackPropagatorSubEditor", this, "DoFitPM()");
   }
   // Kinematics fitting.
   {
      TGGroupFrame* rnrPM = new TGGroupFrame(fPMFrame, "PathMarks:", kLHintsTop | kLHintsCenterX);
      rnrPM->SetTitlePos(TGGroupFrame::kLeft);
      fPMFrame->AddFrame( rnrPM, new TGLayoutHints(kLHintsTop | kLHintsCenterX | kLHintsExpandX, 3, 3, 3, 3));

      TGMatrixLayout *ml = new TGMatrixLayout(rnrPM, 0,1,6);
      rnrPM->SetLayoutManager(ml);

      fRnrDaughters  = new TGCheckButton(rnrPM, "Rnr Daughters",   TEvePathMark::kDaughter);
      fRnrReferences = new TGCheckButton(rnrPM, "Rnr Refs",        TEvePathMark::kReference);
      fRnrDecay      = new TGCheckButton(rnrPM, "Rnr Decay",       TEvePathMark::kDecay);
      fRnrCluster2Ds = new TGCheckButton(rnrPM, "Rnr 2D Clusters", TEvePathMark::kCluster2D);

      rnrPM->AddFrame(fRnrDaughters);
      rnrPM->AddFrame(fRnrReferences);
      rnrPM->AddFrame(fRnrDecay);
      rnrPM->AddFrame(fRnrCluster2Ds);

      fRnrDecay     ->Connect("Clicked()","TEveTrackPropagatorSubEditor", this, "DoRnrPM()");
      fRnrReferences->Connect("Clicked()","TEveTrackPropagatorSubEditor", this, "DoRnrPM()");
      fRnrDaughters ->Connect("Clicked()","TEveTrackPropagatorSubEditor", this, "DoRnrPM()");
      fRnrCluster2Ds->Connect("Clicked()","TEveTrackPropagatorSubEditor", this, "DoRnrPM()");

      fRefsCont->AddFrame(fPMFrame, new TGLayoutHints(kLHintsTop, 1, 1, 1, 1));
   }
   // Marker attributes.
   {
      fPMAtt = new TAttMarkerEditor(fRefsCont);
      TGFrameElement *el = (TGFrameElement*) fPMAtt->GetList()->First();
      TGFrame *f = el->fFrame; fPMAtt->RemoveFrame(f);
      f->DestroyWindow(); delete f;
      fRefsCont->AddFrame(fPMAtt, new TGLayoutHints(kLHintsTop, 1, 1, 3, 1));
   }

   // First vertex.
   TGCompositeFrame *title1 = new TGCompositeFrame(fRefsCont, 145, 10,
                                                   kHorizontalFrame |
                                                   kLHintsExpandX   |
                                                   kFixedWidth      |
                                                   kOwnBackground);
   title1->AddFrame(new TGLabel(title1, "FirstVertex"),
                    new TGLayoutHints(kLHintsLeft, 1, 1, 0, 0));
   title1->AddFrame(new TGHorizontal3DLine(title1),
                    new TGLayoutHints(kLHintsExpandX, 5, 5, 7, 5));
   fRefsCont->AddFrame(title1, new TGLayoutHints(kLHintsTop, 0, 0, 2, 0));

   fRnrFV = new TGCheckButton(fRefsCont, "Rnr");
   fRnrFV->Connect("Clicked()","TEveTrackPropagatorSubEditor", this, "DoRnrFV()");
   fRefsCont->AddFrame(fRnrFV, new TGLayoutHints(kLHintsTop, 5, 1, 2, 0));
   {
      fFVAtt = new TAttMarkerEditor(fRefsCont);
      TGFrameElement *el = (TGFrameElement*) fFVAtt->GetList()->First();
      TGFrame *f = el->fFrame; fFVAtt->RemoveFrame(f);
      f->DestroyWindow(); delete f;
      fRefsCont->AddFrame(fFVAtt, new TGLayoutHints(kLHintsTop, 1, 1, 3, 1));
   }
   p->AddFrame(fRefsCont,new TGLayoutHints(kLHintsTop| kLHintsExpandX));
}

//______________________________________________________________________________
void TEveTrackPropagatorSubEditor::SetModel(TEveTrackPropagator* m)
{
   // Set model object.

   fM = m;

   fMaxR->SetValue(fM->fMaxR);
   fMaxZ->SetValue(fM->fMaxZ);
   fMaxOrbits->SetValue(fM->fMaxOrbs);
   fMaxAng->SetValue(fM->GetMaxAng());
   fDelta->SetValue(fM->GetDelta());

   if(fM->fEditPathMarks)
   {
      ShowFrame(fPMFrame);
      fRnrDaughters->SetState(fM->fRnrDaughters ? kButtonDown : kButtonUp);
      fRnrReferences->SetState(fM->fRnrReferences ? kButtonDown : kButtonUp);
      fRnrDecay->SetState(fM->fRnrDecay ? kButtonDown : kButtonUp);
      fRnrCluster2Ds->SetState(fM->fRnrCluster2Ds ? kButtonDown : kButtonUp);

      fFitDaughters->SetState(fM->fFitDaughters ? kButtonDown : kButtonUp);
      fFitReferences->SetState(fM->fFitReferences ? kButtonDown : kButtonUp);
      fFitDecay->SetState(fM->fFitDecay ? kButtonDown : kButtonUp);
      fFitCluster2Ds->SetState(fM->fFitCluster2Ds ? kButtonDown : kButtonUp);

      fPMAtt->SetModel(&fM->fPMAtt);
   }
   else
   {
      fRefsCont->HideFrame(fPMFrame);
   }

   fRnrFV->SetState(fM->fRnrFV ? kButtonDown : kButtonUp);
   fFVAtt->SetModel(&fM->fFVAtt);
}

/******************************************************************************/

//______________________________________________________________________________
void TEveTrackPropagatorSubEditor::Changed()
{
   // Emit "Changed()" signal.

   Emit("Changed()");
}

/******************************************************************************/

//______________________________________________________________________________
void TEveTrackPropagatorSubEditor::DoMaxR()
{
   // Slot for MaxR.

   fM->SetMaxR(fMaxR->GetValue());
   Changed();
}

//______________________________________________________________________________
void TEveTrackPropagatorSubEditor::DoMaxZ()
{
   // Slot for MaxZ.

   fM->SetMaxZ(fMaxZ->GetValue());
   Changed();
}

//______________________________________________________________________________
void TEveTrackPropagatorSubEditor::DoMaxOrbits()
{
   // Slot for MaxOrbits.

   fM->SetMaxOrbs(fMaxOrbits->GetValue());
   Changed();
}

//______________________________________________________________________________
void TEveTrackPropagatorSubEditor::DoMaxAng()
{
   // Slot for MaxAng.

   fM->SetMaxAng(fMaxAng->GetValue());
   Changed();
}

//______________________________________________________________________________
void TEveTrackPropagatorSubEditor::DoDelta()
{
   // Slot for Delta.

   fM->SetDelta(fDelta->GetValue());
   Changed();
}

/******************************************************************************/

//______________________________________________________________________________
void TEveTrackPropagatorSubEditor::DoFitPM()
{
   // Slot for FitPM.

   TGButton* b = (TGButton *) gTQSender;
   TEvePathMark::EType_e type = TEvePathMark::EType_e(b->WidgetId());
   Bool_t on = b->IsOn();

   switch(type)
   {
      case TEvePathMark::kDaughter:
         fM->SetFitDaughters(on);
         break;
      case TEvePathMark::kReference:
         fM->SetFitReferences(on);
         break;
      case TEvePathMark::kDecay:
         fM->SetFitDecay(on);
         break;
      case TEvePathMark::kCluster2D:
         fM->SetFitCluster2Ds(on);
         break;

      default:
         break;
   }
   Changed();
}

//______________________________________________________________________________
void TEveTrackPropagatorSubEditor::DoRnrPM()
{
   // Slot for RnrPM.

   TGButton * b = (TGButton *) gTQSender;
   TEvePathMark::EType_e type = TEvePathMark::EType_e(b->WidgetId());
   Bool_t on = b->IsOn();
   switch(type){
      case  TEvePathMark::kDaughter:
         fM->SetRnrDaughters(on);
         break;
      case  TEvePathMark::kReference:
         fM->SetRnrReferences(on);
         break;
      case  TEvePathMark::kDecay:
         fM->SetRnrDecay(on);
         break;
      case  TEvePathMark::kCluster2D:
         fM->SetRnrCluster2Ds(on);
         break;
      default:
         break;
   }
   Changed();
}

//______________________________________________________________________________
void TEveTrackPropagatorSubEditor::DoRnrFV()
{
   // Slot for RnrFV.

   fM->SetRnrFV(fRnrFV->IsOn());
   Changed();
}


//______________________________________________________________________________
// TEveTrackPropagatorEditor
//
// GUI editor for TEveTrackPropagator.
// It's only a wrapper around a TEveTrackPropagatorSubEditor that holds actual
// widgets.

ClassImp(TEveTrackPropagatorEditor)

//______________________________________________________________________________
TEveTrackPropagatorEditor::TEveTrackPropagatorEditor(const TGWindow *p,
                                                 Int_t width, Int_t height,
                                                 UInt_t options, Pixel_t back) :
   TGedFrame(p, width, height, options | kVerticalFrame, back),
   fM(0),
   fRSSubEditor(0)
{
   // Constructor.

   MakeTitle("RenderStyle");

   fRSSubEditor = new TEveTrackPropagatorSubEditor(this);
   fRSSubEditor->Connect("Changed()", "TEveTrackPropagatorEditor", this, "Update()");
   AddFrame(fRSSubEditor, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 0,0,0));

   TGVerticalFrame* refsFrame = CreateEditorTabSubFrame("Refs");
   TGCompositeFrame *title1 = new TGCompositeFrame(refsFrame, 145, 10,
                                                   kHorizontalFrame |
                                                   kLHintsExpandX   |
                                                   kFixedWidth      |
                                                   kOwnBackground);
   title1->AddFrame(new TGLabel(title1, "PathMarks"),
                    new TGLayoutHints(kLHintsLeft, 1, 1, 0, 0));
   title1->AddFrame(new TGHorizontal3DLine(title1),
                    new TGLayoutHints(kLHintsExpandX, 5, 5, 7, 7));
   refsFrame->AddFrame(title1, new TGLayoutHints(kLHintsTop, 0, 0, 2, 0));

   // path marks
   fRSSubEditor->CreateRefsContainer(refsFrame);
   fRSSubEditor->fPMAtt->SetGedEditor((TGedEditor*)gEve->GetEditor());
   fRSSubEditor->fFVAtt->SetGedEditor((TGedEditor*)gEve->GetEditor());

   fRSSubEditor->Connect("Changed()", "TEveTrackPropagatorEditor", this, "Update()");
}

/******************************************************************************/

//______________________________________________________________________________
void TEveTrackPropagatorEditor::SetModel(TObject* obj)
{
   // Set model object.

   fM = dynamic_cast<TEveTrackPropagator*>(obj);
   fRSSubEditor->SetModel(fM);
}
