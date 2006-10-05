// @(#)root/fitpanel:$Name:$:$Id:$
// Author:Ilka Antcheva, Lorenzo Moneta 10/08/2006

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TFitEditor                                                           //
//                                                                      //
// Allows to perform, explore and compare various fits.                 //
//                                                                      //
// To display the new Fit panel interface right click on a hostogram    //
// or a graph to pop up the context menu and then select the menu       //
// entry 'Fit Panel'. The first set of GUI elements is related to       //
// the function choice and settings.                                    //
//                                                                      //
// 'Predefined' combo box - contains a list of predefined functions     //
// in ROOT. The default one is Gaussian.                                //
//                                                                      //
// 'Operation' radio button group defines selected operational mode     //
// between functions: NOP - no operation (default); ADD - addition      //
// CONV - convolution (will be implemented in the future).              //
//                                                                      //
// Users can enter the function expression in a text entry field.       //
// The entered string is checked after Enter key was pressed. An        //
// error message shows up if the string is not accepted. This first     //
// prototype is limited and users have no freedom to enter file/user    //
// function names in this field.                                        //
//                                                                      //
// 'Set Parameters' button opens a dialog for parameters settings       //
// (still under development).                                           //
//                                                                      //
// 'Fit Settings' provides user interface elements related to the       //
// fitter. Currently there are two model choices: Chi-square and        //
// Binned Likelihood.                                                   //
//                                                                      //
// Fit options:                                                         //
// Linear Fit' check button sets the use of Linear fitter is it is      //
// selected. Otherwise the option 'F' is applied if polN is selected.   //
// 'Robust' number entry sets the robust value when fitting graphs.     //
// 'No Chi-square' check button iswitch ON/OFF option 'C' - do not      //
// calculate Chi-square (for Linear fitter).                            //
// 'Integral' check button switch ON/OFF option 'I' - use integral      //
// of function instead of value in bin center.                          //
// 'Best Errors' sets ON/OFF option 'E' - better errors estimation      //
// using Minos technique.                                               //
// "Use range" sets ON/OFF option 'R' - fit only data within the        //
// specified function range with the slider.                            //
// "All weights = 1" sets ON/OFF option 'W'- all weights set to 1,      //
// error bars are ignored.                                              //
// "Improve fit results" sets ON/OFF option 'M'- after minimum is       //
// found, search for a new one.                                         //
// "Add to list" sets On/Off option '+'- add function to the list       //
// without deleting the previous.                                       //
//                                                                      //
// Draw options:                                                        //
// "SAME" sets On/Off function drawing on the same pad.                 //
// "No drawing" sets On/Off option '0'- do not draw function graphics.  //
// "Do not store/draw" sets On/Off option 'N'- do not store the         //
// function, do not draw it.                                            //
//                                                                      //
// Print options:                                                       //
// "Default" - between Verbose and Quiet.                               //
// "Verbose" - prints results after each iteration.                     //
// "Quiet" - no fit information is printed.                             //
//                                                                      //
// Sliders settings are used if option 'R' - use range is active.       //
// Users can change min/max values by pressing the left mouse button    //
// near to the left/right slider edges. It is possible o change both    //
// values simultaneously by pressing the left mouse button near to its  //
// center and moving it to a new desire position.                       //
//                                                                      //
// Fit button - performs a fit.                                         //
// Reset - resets the GUI elements and related fit settings to the      //
// default ones.                                                        //
// Close - closes this window.                                          //
//                                                                      //
// Begin_Html                                                           //
/*
<img src="gif/TGraphEditor.gif">
*/
//End_Html
//////////////////////////////////////////////////////////////////////////

#include "TFitEditor.h"
#include "TCanvas.h"
#include "TGTab.h"
#include "TGLabel.h"
#include "TG3DLine.h"
#include "TGComboBox.h"
#include "TGTextEntry.h"
#include "TGFont.h"
#include "TGGC.h"
#include "TGButtonGroup.h"
#include "TGNumberEntry.h"
#include "TGDoubleSlider.h"
#include "TFitParametersDialog.h"
#include "TGMsgBox.h"
#include "TAxis.h"
#include "TGraph.h"
#include "TH1.h"
#include "TF1.h"
#include "TTimer.h"
#include "THStack.h"


enum EFitPanel {
   kFP_FLIST, kFP_GAUS,  kFP_GAUSN, kFP_EXPO,  kFP_LAND,  kFP_LANDN,  
   kFP_POL0,  kFP_POL1,  kFP_POL2,  kFP_POL3,  kFP_POL4,  kFP_POL5,  
   kFP_POL6,  kFP_POL7,  kFP_POL8,  kFP_POL9,  kFP_USER,
   kFP_NONE,  kFP_ADD,   kFP_CONV,  kFP_FILE,  kFP_PARS,  kFP_RBUST,
   kFP_INTEG, kFP_IMERR, kFP_USERG, kFP_ADDLS, kFP_ALLE1, kFP_IFITR, kFP_NOCHI,
   kFP_MLIST, kFP_MCHIS, kFP_MBINL, kFP_MUBIN, kFP_MUSER, kFP_MLINF, kFP_MUSR,
   kFP_DSAME, kFP_DNONE, kFP_DADVB, kFP_DNOST, kFP_PDEF,  kFP_PVER,  kFP_PQET,
   kFP_XMIN,  kFP_XMAX,  kFP_YMIN,  kFP_YMAX,  kFP_ZMIN,  kFP_ZMAX,
   kFP_FIT,   kFP_RESET, kFP_CLOSE
};

ClassImp(TFitEditor)

//______________________________________________________________________________
TFitEditor::TFitEditor(const TVirtualPad* pad, const TObject *obj) :
   TGMainFrame(gClient->GetRoot(), 20, 20),
   fParentPad   ((TPad *)pad),
   fFitObject   ((TObject *)obj),
   fDim         (0),
   fXaxis       (0),
   fYaxis       (0),
   fZaxis       (0),
   fXmin        (0),
   fXmax        (0),
   fPlus        ('+'),
   fFunction    (""),
   fFitOption   ("R"),
   fDrawOption  (""),
   fFitFunc     (0)
  
{
   // Constructor of fit editor.
   
   TString name = obj->GetName();
   name.Append("::");
   name.Append(obj->ClassName());
   TGHorizontalFrame *cf2 = new TGHorizontalFrame(this, 80, 20);
   TGLabel *label = new TGLabel(cf2,"Object: ");
   cf2->AddFrame(label, new TGLayoutHints(kLHintsLeft, 1, 1, 0, 0));
   fObjLabel = new TGLabel(cf2, Form("%s", name.Data()));
   cf2->AddFrame(fObjLabel, new TGLayoutHints(kLHintsLeft, 1, 1, 0, 0));
   AddFrame(cf2, new TGLayoutHints(kLHintsTop, 1, 1, 10, 10));
   // set red color for the name
   Pixel_t color;
   gClient->GetColorByName("#ff0000", color);
   fObjLabel->SetTextColor(color, kFALSE);

   // 'General' tab
   fTab = new TGTab(this, 10, 10);
   AddFrame(fTab, new TGLayoutHints(kLHintsExpandY | kLHintsExpandX));
   fTab->SetCleanup(kDeepCleanup);
   fTab->Associate(this);
   fTabContainer = fTab->AddTab("General");
   fGeneral = new TGCompositeFrame(fTabContainer, 10, 10, kVerticalFrame);
   fTabContainer->AddFrame(fGeneral, new TGLayoutHints(kLHintsTop | 
                                                       kLHintsExpandX,
                                                       5, 5, 2, 2));

   TGGroupFrame *gf1 = new TGGroupFrame(fGeneral, "Function", kFitWidth);
   TGCompositeFrame *tf1 = new TGCompositeFrame(gf1, 350, 26, 
                                                kHorizontalFrame);
   TGVerticalFrame *tf11 = new TGVerticalFrame(tf1);
   TGLabel *label1 = new TGLabel(tf11,"Predefined:");
   tf11->AddFrame(label1, new TGLayoutHints(kLHintsNormal, 0, 0, 2, 0));
   fFuncList = BuildFunctionList(tf11, kFP_FLIST);
   fFuncList->Resize(100, 20);
   fFuncList->Select(1, kFALSE);
   
   TGListBox *lb = fFuncList->GetListBox();
   lb->Resize(lb->GetWidth(), 200);
   tf11->AddFrame(fFuncList, new TGLayoutHints(kLHintsNormal, 0, 0, 5, 0));
   fFuncList->Associate(this);
   tf1->AddFrame(tf11);

   TGHButtonGroup *bgr = new TGHButtonGroup(tf1,"Operation");
   bgr->SetRadioButtonExclusive();
   fNone = new TGRadioButton(bgr, "NOP", kFP_NONE);
   fNone->SetToolTipText("No operation defined");
   fNone->SetState(kButtonDown, kFALSE);
   fAdd = new TGRadioButton(bgr, "ADD", kFP_ADD);
   fAdd->SetToolTipText("Addition");
   fConv = new TGRadioButton(bgr, "CONV", kFP_CONV);
   fConv->SetToolTipText("Convolution (not implemented yet)");
   fConv->SetState(kButtonDisabled);
   bgr->SetLayoutHints(new TGLayoutHints(kLHintsLeft,0,5,3,-10),fNone);
   bgr->SetLayoutHints(new TGLayoutHints(kLHintsLeft,10,5,3,-10),fAdd);   
   bgr->SetLayoutHints(new TGLayoutHints(kLHintsLeft,10,0,3,-10),fConv);   
   bgr->Show();
   bgr->ChangeOptions(kFitWidth | kHorizontalFrame);
   tf1->AddFrame(bgr, new TGLayoutHints(kLHintsNormal, 15, 0, 3, 0));

   gf1->AddFrame(tf1, new TGLayoutHints(kLHintsNormal | kLHintsExpandX));

   TGCompositeFrame *tf2 = new TGCompositeFrame(gf1, 350, 26, 
                                                kHorizontalFrame);
   fEnteredFunc = new TGTextEntry(tf2, new TGTextBuffer(50), kFP_FILE); 
   fEnteredFunc->SetMaxLength(250);
   fEnteredFunc->SetAlignment(kTextLeft);
   fEnteredFunc->SetToolTipText("Enter file_name/function_name or a function expression");
   fEnteredFunc->Resize(250,fEnteredFunc->GetDefaultHeight());
   tf2->AddFrame(fEnteredFunc, new TGLayoutHints(kLHintsLeft    | 
                                                 kLHintsCenterY | 
                                                 kLHintsExpandX, 2, 2, 2, 2));
   gf1->AddFrame(tf2, new TGLayoutHints(kLHintsNormal | 
                                        kLHintsExpandX, 0, 0, 2, 0));

   TGHorizontalFrame *s1 = new TGHorizontalFrame(gf1);
   TGLabel *label21 = new TGLabel(s1, "Selected: ");
   s1->AddFrame(label21, new TGLayoutHints(kLHintsNormal | 
                                           kLHintsCenterY, 2, 2, 2, 0));
   TGHorizontal3DLine *hlines = new TGHorizontal3DLine(s1);
   s1->AddFrame(hlines, new TGLayoutHints(kLHintsCenterY | kLHintsExpandX));
   gf1->AddFrame(s1, new TGLayoutHints(kLHintsExpandX));

   TGCompositeFrame *tf4 = new TGCompositeFrame(gf1, 350, 26, 
                                                kHorizontalFrame);
   TGTextLBEntry *txt = (TGTextLBEntry *)fFuncList->GetSelectedEntry(); 
   fSelLabel = new TGLabel(tf4, Form("%s", txt->GetTitle()));
   tf4->AddFrame(fSelLabel, new TGLayoutHints(kLHintsNormal | 
                                              kLHintsCenterY, 0, 6, 2, 0));

   gClient->GetColorByName("#336666", color);
   fSelLabel->SetTextColor(color, kFALSE);
   TGCompositeFrame *tf5 = new TGCompositeFrame(tf4, 120, 20, 
                                                kHorizontalFrame | kFixedWidth);
   fSetParam = new TGTextButton(tf5, "Set Parameters...", kFP_PARS);
   tf5->AddFrame(fSetParam, new TGLayoutHints(kLHintsRight   | 
                                              kLHintsCenterY |
                                              kLHintsExpandX));
   fSetParam->SetToolTipText("Open a dialog for parameter(s) settings");
   tf4->AddFrame(tf5, new TGLayoutHints(kLHintsRight | 
                                        kLHintsTop, 5, 0, 2, 2));
   gf1->AddFrame(tf4, new TGLayoutHints(kLHintsNormal | 
                                             kLHintsExpandX, 5, 0, 0, 0));

   fGeneral->AddFrame(gf1, new TGLayoutHints(kLHintsExpandX, 5, 5, 0, 0));


   // 'options' group frame
   TGGroupFrame *gf = new TGGroupFrame(fGeneral, "Fit Settings", kFitWidth);

   // 'method' sub-group
   TGHorizontalFrame *h1 = new TGHorizontalFrame(gf);
   TGLabel *label4 = new TGLabel(h1, "Method");
   h1->AddFrame(label4, new TGLayoutHints(kLHintsNormal | 
                                          kLHintsCenterY, 2, 2, 0, 0));
   TGHorizontal3DLine *hline1 = new TGHorizontal3DLine(h1);
   h1->AddFrame(hline1, new TGLayoutHints(kLHintsCenterY | kLHintsExpandX));
   gf->AddFrame(h1, new TGLayoutHints(kLHintsExpandX));
   
   TGHorizontalFrame *h2 = new TGHorizontalFrame(gf);
   TGVerticalFrame *v1 = new TGVerticalFrame(h2);
   fMethodList = BuildMethodList(v1, kFP_MLIST);
   fMethodList->Select(1, kFALSE);
   if (fFitObject->InheritsFrom("TGraph")) {
      fType = kObjectGraph;
      fMethodList->RemoveEntry(kFP_MBINL);
   } else if (fFitObject->InheritsFrom("TGraph2D")) {
      fType = kObjectGraph2D;
      fMethodList->RemoveEntry(kFP_MUBIN);
   } else if (fFitObject->InheritsFrom("THStack")) {
      fType = kObjectHStack;
      fMethodList->RemoveEntry(kFP_MUBIN);
   } else if (fFitObject->InheritsFrom("TTree")) {
      fType = kObjectTree;
      fMethodList->RemoveEntry(kFP_MCHIS);
      fMethodList->RemoveEntry(kFP_MBINL);
      fMethodList->Select(kFP_MUBIN, kFALSE);
   } else {
      fType = kObjectHisto;
   }
   fMethodList->Resize(130, 20);
   lb = fMethodList->GetListBox();
   Int_t lbe = lb->GetNumberOfEntries();
   lb->Resize(lb->GetWidth(), lbe*16);
   v1->AddFrame(fMethodList, new TGLayoutHints(kLHintsLeft, 0, 0, 2, 5));

   fLinearFit = new TGCheckButton(v1, "Linear fit", kFP_MLINF);
   fLinearFit->Associate(this);
   fLinearFit->SetToolTipText("Perform Linear fitter if selected");
   v1->AddFrame(fLinearFit, new TGLayoutHints(kLHintsNormal, 0, 0, 2, 2));
   
   TGHorizontalFrame *v1h = new TGHorizontalFrame(v1);
   TGLabel *label41 = new TGLabel(v1h, "Robust:");
   v1h->AddFrame(label41, new TGLayoutHints(kLHintsNormal | 
                                            kLHintsCenterY, 25, 5, 5, 2));
   fRobustValue = new TGNumberEntry(v1h, 1., 5, kFP_RBUST, 
                                    TGNumberFormat::kNESRealTwo,
                                    TGNumberFormat::kNEAPositive,
                                    TGNumberFormat::kNELLimitMinMax,0.,1.);
   v1h->AddFrame(fRobustValue, new TGLayoutHints(kLHintsLeft));
   v1->AddFrame(v1h, new TGLayoutHints(kLHintsNormal));
   if (fType != kObjectGraph) {
      fRobustValue->SetState(kFALSE);
      fRobustValue->GetNumberEntry()->SetToolTipText("Available only for graphs");
   } else {
      fRobustValue->SetState(kTRUE);
      fRobustValue->GetNumberEntry()->SetToolTipText("Set robust value");
   }
   h2->AddFrame(v1, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

   TGVerticalFrame *v2 = new TGVerticalFrame(h2);
   TGCompositeFrame *v21 = new TGCompositeFrame(v2, 120, 20, 
                                                kHorizontalFrame | kFixedWidth);
   fUserButton = new TGTextButton(v21, "User-Defined...", kFP_MUSR);
   v21->AddFrame(fUserButton, new TGLayoutHints(kLHintsRight   | 
                                                kLHintsCenterY |
                                                kLHintsExpandX));
   fUserButton->SetToolTipText("Open a dialog for entering a user-defined method");
   fUserButton->SetState(kButtonDisabled);
   v2->AddFrame(v21, new TGLayoutHints(kLHintsRight | kLHintsTop));

   fNoChi2 = new TGCheckButton(v2, "No Chi-square", kFP_NOCHI);
   fNoChi2->Associate(this);
   fNoChi2->SetToolTipText("'C'- do not calculate Chi-square (for Linear fitter)");
   v2->AddFrame(fNoChi2, new TGLayoutHints(kLHintsNormal, 0, 0, 34, 2));

   h2->AddFrame(v2, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   gf->AddFrame(h2, new TGLayoutHints(kLHintsExpandX, 20, 0, 0, 0));

   // 'fit option' sub-group
   TGHorizontalFrame *h3 = new TGHorizontalFrame(gf);
   TGLabel *label5 = new TGLabel(h3, "Fit Options");
   h3->AddFrame(label5, new TGLayoutHints(kLHintsNormal | 
                                          kLHintsCenterY, 2, 2, 0, 0));
   TGHorizontal3DLine *hline2 = new TGHorizontal3DLine(h3);
   h3->AddFrame(hline2, new TGLayoutHints(kLHintsCenterY | kLHintsExpandX));
   gf->AddFrame(h3, new TGLayoutHints(kLHintsExpandX));

   TGHorizontalFrame *h = new TGHorizontalFrame(gf);
   TGVerticalFrame *v3 = new TGVerticalFrame(h);
   fIntegral = new TGCheckButton(v3, "Integral", kFP_INTEG);
   fIntegral->Associate(this);
   fIntegral->SetToolTipText("'I'- use integral of function instead of value in bin center");
   v3->AddFrame(fIntegral, new TGLayoutHints(kLHintsNormal, 0, 0, 2, 2));
   
   fBestErrors = new TGCheckButton(v3, "Best errors", kFP_IMERR);
   fBestErrors->Associate(this);
   fBestErrors->SetToolTipText("'E'- better errors estimation using Minos technique");
   v3->AddFrame(fBestErrors, new TGLayoutHints(kLHintsNormal, 0, 0, 2, 2));

   fUseRange = new TGCheckButton(v3, "Use range", kFP_USERG);
   fUseRange->Associate(this);
   fUseRange->SetToolTipText("'R'- fit only data within the specified function range");
   v3->AddFrame(fUseRange, new TGLayoutHints(kLHintsNormal, 0, 0, 2, 2));
   if (fFitOption.Contains('R'))
      fUseRange->SetState(kButtonDown);

   h->AddFrame(v3, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

   TGVerticalFrame *v4 = new TGVerticalFrame(h);
   fAllWeights1 = new TGCheckButton(v4, "All weights = 1", kFP_ALLE1);
   fAllWeights1->Associate(this);
   fAllWeights1->SetToolTipText("'W'- all weights set to 1, error bars are ignored");
   v4->AddFrame(fAllWeights1, new TGLayoutHints(kLHintsNormal, 0, 0, 2, 2));

   fImproveResults = new TGCheckButton(v4, "Improve fit results", kFP_IFITR);
   fImproveResults->Associate(this);
   fImproveResults->SetToolTipText("'M'- after minimum is found, search for a new one");
   v4->AddFrame(fImproveResults, new TGLayoutHints(kLHintsNormal, 0, 0, 2, 2));

   fAdd2FuncList = new TGCheckButton(v4, "Add to list", kFP_ADDLS);
   fAdd2FuncList->Associate(this);
   fAdd2FuncList->SetToolTipText("'+'- add function to the list without deleting the previous");
   v4->AddFrame(fAdd2FuncList, new TGLayoutHints(kLHintsNormal, 0, 0, 2, 2));

   h->AddFrame(v4, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   gf->AddFrame(h, new TGLayoutHints(kLHintsExpandX, 20, 0, 0, 0));

   // 'draw option' sub-group
   TGHorizontalFrame *h5 = new TGHorizontalFrame(gf);
   TGLabel *label6 = new TGLabel(h5, "Draw Options");
   h5->AddFrame(label6, new TGLayoutHints(kLHintsNormal | 
                                          kLHintsCenterY, 2, 2, 2, 2));
   TGHorizontal3DLine *hline3 = new TGHorizontal3DLine(h5);
   h5->AddFrame(hline3, new TGLayoutHints(kLHintsCenterY | kLHintsExpandX));
   gf->AddFrame(h5, new TGLayoutHints(kLHintsExpandX));

   TGHorizontalFrame *h6 = new TGHorizontalFrame(gf);
   TGVerticalFrame *v5 = new TGVerticalFrame(h6);

   fDrawSame = new TGCheckButton(v5, "SAME", kFP_DSAME);
   fDrawSame->Associate(this);
   fDrawSame->SetToolTipText("Superimpose on previous picture in the same pad");
   v5->AddFrame(fDrawSame, new TGLayoutHints(kLHintsNormal, 0, 0, 2, 2));

   fNoDrawing = new TGCheckButton(v5, "No drawing", kFP_DNONE);
   fNoDrawing->Associate(this);
   fNoDrawing->SetToolTipText("'0'- do not draw function graphics");
   v5->AddFrame(fNoDrawing, new TGLayoutHints(kLHintsNormal, 0, 0, 2, 2));

   fNoStoreDrawing = new TGCheckButton(v5, "Do not store/draw", kFP_DNOST);
   fNoStoreDrawing->Associate(this);
   fNoStoreDrawing->SetToolTipText("'N'- do not store the function, do not draw it");
   v5->AddFrame(fNoStoreDrawing, new TGLayoutHints(kLHintsNormal, 0, 0, 2, 2));

   h6->AddFrame(v5, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

   TGVerticalFrame *v6 = new TGVerticalFrame(h6);
   TGCompositeFrame *v61 = new TGCompositeFrame(v6, 120, 20, 
                                                kHorizontalFrame | kFixedWidth);
   fDrawAdvanced = new TGTextButton(v61, "Advanced...", kFP_DADVB);
   v61->AddFrame(fDrawAdvanced, new TGLayoutHints(kLHintsRight   | 
                                                  kLHintsCenterY |
                                                  kLHintsExpandX));
   fDrawAdvanced->SetToolTipText("Open a dialog for advanced draw options");
   fDrawAdvanced->SetState(kButtonDisabled);

   v6->AddFrame(v61, new TGLayoutHints(kLHintsRight | kLHintsTop,
                                       0, 0, (4+fDrawSame->GetHeight())*2, 0));

   h6->AddFrame(v6, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
   gf->AddFrame(h6, new TGLayoutHints(kLHintsExpandX, 20, 0, 2, 0));

   // 'draw option' sub-group
   TGHorizontalFrame *h7 = new TGHorizontalFrame(gf);
   TGLabel *label7 = new TGLabel(h7, "Print Options");
   h7->AddFrame(label7, new TGLayoutHints(kLHintsNormal | 
                                          kLHintsCenterY, 2, 2, 2, 2));
   TGHorizontal3DLine *hline4 = new TGHorizontal3DLine(h7);
   h7->AddFrame(hline4, new TGLayoutHints(kLHintsCenterY | kLHintsExpandX));
   gf->AddFrame(h7, new TGLayoutHints(kLHintsExpandX));

   TGHorizontalFrame *h8 = new TGHorizontalFrame(gf);

   fOptDefault = new TGRadioButton(h8, "Default", kFP_PDEF);
   fOptDefault->Associate(this);
   fOptDefault->SetToolTipText("Default is between Verbose and Quiet");
   h8->AddFrame(fOptDefault, new TGLayoutHints(kLHintsNormal, 0, 0, 0, 1));
   fOptDefault->SetState(kButtonDown);

   fOptVerbose = new TGRadioButton(h8, "Verbose", kFP_PVER);
   fOptVerbose->Associate(this);
   fOptVerbose->SetToolTipText("Print results after each iteration");
   h8->AddFrame(fOptVerbose, new TGLayoutHints(kLHintsNormal, 30, 0, 0, 1));

   fOptQuiet = new TGRadioButton(h8, "Quiet", kFP_PQET);
   fOptQuiet->Associate(this);
   fOptQuiet->SetToolTipText("No print");
   h8->AddFrame(fOptQuiet, new TGLayoutHints(kLHintsNormal, 30, 0, 0, 1));

   gf->AddFrame(h8, new TGLayoutHints(kLHintsExpandX, 20, 0, 1, 1));

   fGeneral->AddFrame(gf, new TGLayoutHints(kLHintsExpandX | 
                                            kLHintsExpandY, 5, 5, 0, 0));
   // sliders
   switch (fType) {
      case kObjectHisto: {
         fDim = ((TH1*)fFitObject)->GetDimension();
         break;
      }
      case kObjectGraph: {
         fDim = 1;
         break;
      }
      case kObjectGraph2D: {
         fDim = 2;
         break;
      }
      case kObjectHStack: {
         TH1 *hist = (TH1 *)((THStack *)fFitObject)->GetHists()->First();
         fDim = hist->GetDimension();
         break;
      }
      case kObjectTree:  {
         //not implemented
         break;
      }
   }
   
   if (fDim > 0) {
      TGHorizontalFrame *sf1 = new TGHorizontalFrame(fGeneral);
      TGLabel *label7 = new TGLabel(sf1, "X:");
      sf1->AddFrame(label7, new TGLayoutHints(kLHintsLeft | 
                                              kLHintsCenterY, 0, 5, 0, 0));
      fSliderX = new TGDoubleHSlider(sf1, 1, 0);
      sf1->AddFrame(fSliderX, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
      fGeneral->AddFrame(sf1, new TGLayoutHints(kLHintsExpandX, 5, 5, 0, 0));

      switch (fType) {
         case kObjectHisto: {
            fXaxis = ((TH1*)fFitObject)->GetXaxis();
            fYaxis = ((TH1*)fFitObject)->GetYaxis();
            fZaxis = ((TH1*)fFitObject)->GetZaxis();
            Int_t nx = fXaxis->GetNbins();
            Int_t nxbinmin = fXaxis->GetFirst();
            Int_t nxbinmax = fXaxis->GetLast();
            fSliderX->SetRange(1,nx);
            fSliderX->SetScale(5);
            fSliderX->SetPosition((Double_t)nxbinmin,(Double_t)nxbinmax);
            break;
         }
         case kObjectGraph: {
            TGraph *gr = (TGraph*)fFitObject; //TBV
            TH1F *hist = gr->GetHistogram();
            if (hist) {
               fXaxis = hist->GetXaxis();
               fYaxis = hist->GetYaxis();
               fZaxis = hist->GetZaxis();
               Int_t nx = fXaxis->GetNbins();
               Int_t xgrmin = fXaxis->GetFirst();
               Int_t xgrmax = fXaxis->GetLast();
               fSliderX->SetRange(1,nx);
               fSliderX->SetScale(5);
               fSliderX->SetPosition((Double_t)xgrmin,(Double_t)xgrmax);
            }
            break;
         }
         case kObjectGraph2D: {
            //not implemented
            break;
         }
         case kObjectHStack: {
            TH1 *hist = (TH1 *)((THStack *)fFitObject)->GetHists()->First();
            fXaxis = hist->GetXaxis();
            fYaxis = hist->GetYaxis();
            fZaxis = hist->GetZaxis();
            Int_t nx = fXaxis->GetNbins();
            Int_t nxbinmin = fXaxis->GetFirst();
            Int_t nxbinmax = fXaxis->GetLast();
            fSliderX->SetRange(1,nx);
            fSliderX->SetScale(5);
            fSliderX->SetPosition((Double_t)nxbinmin,(Double_t)nxbinmax);
            break;
         }
         case kObjectTree:  {
            //not implemented
            break;
         }
      }
   }

   if (fDim > 1) {
      TGHorizontalFrame *sf2 = new TGHorizontalFrame(fGeneral);
      TGLabel *label8 = new TGLabel(sf2, "Y:");
      sf2->AddFrame(label8, new TGLayoutHints(kLHintsLeft | 
                                              kLHintsCenterY, 0, 5, 0, 0));
      fSliderY = new TGDoubleHSlider(sf2, 1, 0);
      fSliderY->SetScale(5);
      sf2->AddFrame(fSliderY, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
      fGeneral->AddFrame(sf2, new TGLayoutHints(kLHintsExpandX, 5, 5, 0, 2));

      switch (fType) {
         case kObjectHisto: {
            Int_t ny = fYaxis->GetNbins();
            Int_t nybinmin = fYaxis->GetFirst();
            Int_t nybinmax = fYaxis->GetLast();
            fSliderY->SetRange(1,ny);
            fSliderY->SetPosition((Double_t)nybinmin,(Double_t)nybinmax);
            break;
         }
         case kObjectGraph: {
            //not implemented
            break;
         }
         case kObjectGraph2D: {
            //not implemented
            break;
         }
         case kObjectHStack: {
            Int_t ny = fYaxis->GetNbins();
            Int_t nybinmin = fYaxis->GetFirst();
            Int_t nybinmax = fYaxis->GetLast();
            fSliderY->SetRange(1,ny);
            fSliderY->SetPosition((Double_t)nybinmin,(Double_t)nybinmax);
            break;
         }
         case kObjectTree:  {
            //not implemented
            break;
         }
      }
   }

   if (fDim > 2) {
      TGHorizontalFrame *sf3 = new TGHorizontalFrame(fGeneral);
      TGLabel *label9 = new TGLabel(sf3, "Z:");
      sf3->AddFrame(label9, new TGLayoutHints(kLHintsLeft | 
                                              kLHintsCenterY, 0, 5, 0, 0));
      fSliderZ = new TGDoubleHSlider(sf3, 1, 0);
      fSliderZ->SetScale(5);
      sf3->AddFrame(fSliderZ, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
      fGeneral->AddFrame(sf3, new TGLayoutHints(kLHintsExpandX, 5, 5, 0, 2));

      switch (fType) {
         case kObjectHisto: {
            Int_t nz = fZaxis->GetNbins();
            Int_t nzbinmin = fZaxis->GetFirst();
            Int_t nzbinmax = fZaxis->GetLast();
            fSliderZ->SetRange(1,nz);
            fSliderZ->SetPosition((Double_t)nzbinmin,(Double_t)nzbinmax);
            break;
         }
         case kObjectGraph: {
            //not implemented
            break;
         }
         case kObjectGraph2D: {
            //not implemented
            break;
         }
         case kObjectHStack: {
            //TH1 *hist = (TH1 *)((THStack *)fFitObject)->GetHists()->First();
            Int_t nz = fZaxis->GetNbins();
            Int_t nzbinmin = fZaxis->GetFirst();
            Int_t nzbinmax = fZaxis->GetLast();
            fSliderZ->SetRange(1,nz);
            fSliderZ->SetPosition((Double_t)nzbinmin,(Double_t)nzbinmax);
            break;
         }
         case kObjectTree:  {
            //not implemented
            break;
         }
      }
   }

   // 'Minimization' tab
   fTabContainer = fTab->AddTab("Minimization");
   fMinimization = new TGCompositeFrame(fTabContainer, 10, 10, kVerticalFrame);
   fTabContainer->AddFrame(fMinimization, new TGLayoutHints(kLHintsTop | 
                                                            kLHintsExpandX, 
                                                            5, 5, 2, 2));
   fTab->SetEnabled(1, kFALSE);
   
   TGHorizontalFrame *cf1 = new TGHorizontalFrame(this, 250, 20, kFixedWidth);
   cf1->SetCleanup(kDeepCleanup);
   fFitButton = new TGTextButton(cf1, "&Fit", kFP_FIT);
   fFitButton->Associate(this);
   cf1->AddFrame(fFitButton, new TGLayoutHints(kLHintsTop | 
                                               kLHintsExpandX, 2, 2, 2, 2));

   fResetButton = new TGTextButton(cf1, "&Reset", kFP_RESET);
   fResetButton->Associate(this);
   cf1->AddFrame(fResetButton, new TGLayoutHints(kLHintsTop | 
                                                 kLHintsExpandX, 3, 2, 2, 2));

   fCloseButton = new TGTextButton(cf1, "&Close", kFP_CLOSE);
   fCloseButton->Associate(this);
   cf1->AddFrame(fCloseButton, new TGLayoutHints(kLHintsTop | 
                                                 kLHintsExpandX, 3, 2, 2, 2));
   AddFrame(cf1, new TGLayoutHints(kLHintsBottom | 
                                   kLHintsRight, 0, 5, 5, 5));
   
   MapSubwindows();
   TGDimension size = GetDefaultSize();
   Resize(size);
   // no resize
   SetWMSize(size.fWidth, size.fHeight);
   SetWMSizeHints(size.fWidth, size.fHeight, size.fWidth, size.fHeight, 0, 0);
   SetWindowName("New Fit Panel");
   SetIconName("New Fit Panel");
   SetClassHints("New Fit Panel", "New Fit Panel");

   SetMWMHints(kMWMDecorAll | kMWMDecorResizeH  | kMWMDecorMaximize |
                              kMWMDecorMinimize | kMWMDecorMenu,
               kMWMFuncAll  | kMWMFuncResize    | kMWMFuncMaximize |
                              kMWMFuncMinimize,
               kMWMInputModeless);

   MapWindow();
   Init();
   gClient->WaitFor(this);
}

//______________________________________________________________________________
TFitEditor::~TFitEditor()
{
   // Fit editor destructor.

   if (fFitFunc) delete fFitFunc;
   Cleanup();
}

//______________________________________________________________________________
void TFitEditor::Init()
{
   // Initialize the user interface.
   
   // list of predefined functions
   fFuncList->Connect("Selected(Int_t)", "TFitEditor", this, "DoFunction(Int_t)"); 
   // entered formula or function name
   fEnteredFunc->Connect("ReturnPressed()", "TFitEditor", this, "DoEnteredFunction()");
   // set parameters dialog
   fSetParam->Connect("Clicked()", "TFitEditor", this, "DoSetParameters()");
   // allowed function operations
   fNone->Connect("Toggled(Bool_t)","TFitEditor", this, "DoNoOperation(Bool_t)");
   fAdd->Connect("Toggled(Bool_t)","TFitEditor", this, "DoAddition(Bool_t)");

   // fit options 
   fIntegral->Connect("Toggled(Bool_t)","TFitEditor",this,"DoIntegral()");
   fBestErrors->Connect("Toggled(Bool_t)","TFitEditor",this,"DoBestErrors()");
   fUseRange->Connect("Toggled(Bool_t)","TFitEditor",this,"DoUseRange()");
   fAdd2FuncList->Connect("Toggled(Bool_t)","TFitEditor",this,"DoAddtoList()");
   fAllWeights1->Connect("Toggled(Bool_t)","TFitEditor",this,"DoAllWeights1()");
   fImproveResults->Connect("Toggled(Bool_t)","TFitEditor",this,"DoImproveResults()");
   
   // linear fit
   fLinearFit->Connect("Toggled(Bool_t)","TFitEditor",this,"DoLinearFit()");
   fNoChi2->Connect("Toggled(Bool_t)","TFitEditor",this,"DoNoChi2()");
   fRobustValue->Connect("ValueSet(Long_t)", "TFitEditor", this, "DoRobust()");
   (fRobustValue->GetNumberEntry())->Connect("ReturnPressed()", "TFitEditor", 
                                              this, "DoRobust()");
   
   // draw options
   fNoStoreDrawing->Connect("Toggled(Bool_t)","TFitEditor",this,"DoNoStoreDrawing()");
   fNoDrawing->Connect("Toggled(Bool_t)","TFitEditor",this,"DoNoDrawing()");
   fDrawSame->Connect("Toggled(Bool_t)","TFitEditor",this,"DoDrawSame()");

   // print options
   fOptDefault->Connect("Toggled(Bool_t)","TFitEditor",this,"DoPrintOpt(Bool_t)");
   fOptVerbose->Connect("Toggled(Bool_t)","TFitEditor",this,"DoPrintOpt(Bool_t)");
   fOptQuiet->Connect("Toggled(Bool_t)","TFitEditor",this,"DoPrintOpt(Bool_t)");

   // fit method
   fMethodList->Connect("Selected(Int_t)", "TFitEditor", this, "DoMethod(Int_t)"); 

   // fit, reset, close buttons
   fFitButton->Connect("Clicked()", "TFitEditor", this, "DoFit()");
   fResetButton->Connect("Clicked()", "TFitEditor", this, "DoReset()");
   fCloseButton->Connect("Clicked()", "TFitEditor", this, "DoClose()");
   
   // user method button
   fUserButton->Connect("Clicked()", "TFitEditor", this, "DoUserDialog()");
   // advanced draw options
   fDrawAdvanced->Connect("Clicked()", "TFitEditor", this, "DoAdvancedOptions()");
   
   // range sliders
//   fXmin = fXaxis->GetBinLowEdge((Int_t)((fSliderX->GetMinPosition())+0.5));
//   fXmax = fXaxis->GetBinUpEdge((Int_t)((fSliderX->GetMaxPosition())+0.5));

   if (fDim > 0) {
      fSliderX->Connect("PositionChanged()","TFitEditor",this, "DoSliderXMoved()");  
      fSliderX->Connect("Pressed()","TFitEditor",this, "DoSliderXPressed()"); 
      fSliderX->Connect("Released()","TFitEditor",this, "DoSliderXReleased()");     
   }
   if (fDim > 1) {
      fSliderY->Connect("PositionChanged()","TFitEditor",this, "DoSliderYMoved()");  
      fSliderY->Connect("Pressed()","TFitEditor",this, "DoSliderYPressed()"); 
      fSliderY->Connect("Released()","TFitEditor",this, "DoSliderYReleased()");     
   }
   if (fDim > 2) {
      fSliderZ->Connect("PositionChanged()","TFitEditor",this, "DoSliderZMoved()");  
      fSliderZ->Connect("Pressed()","TFitEditor",this, "DoSliderZPressed()"); 
      fSliderZ->Connect("Released()","TFitEditor",this, "DoSliderZReleased()");     
   }
   if (!gROOT->GetSelectedPrimitive())
      gROOT->SetSelectedPrimitive(fFitObject);
   if (!gROOT->GetSelectedPad())
      gROOT->SetSelectedPad(fParentPad);

   TGTextLBEntry *te = (TGTextLBEntry *)fFuncList->GetSelectedEntry();
   if (fNone->GetState() == kButtonDown)
      fFunction = te->GetTitle();
   else if (fAdd->GetState() == kButtonDown) {
      fFunction += '+';
      fFunction +=te->GetTitle();
   }
   fEnteredFunc->SetText(fFunction.Data());
   fEnteredFunc->SelectAll();
   if (!fFitFunc) {
      fFitFunc = new TF1("fitFunc",fFunction.Data(),fXmin,fXmax);
   }
   fParentPad->cd();
}

//______________________________________________________________________________
TGComboBox* TFitEditor::BuildFunctionList(TGFrame* parent, Int_t id)
{
   // Create function list combo box.

   TGComboBox *c = new TGComboBox(parent, id);

   c->AddEntry("gaus" ,  kFP_GAUS);
   c->AddEntry("gausn",  kFP_GAUSN);
   c->AddEntry("expo",   kFP_EXPO);
   c->AddEntry("landau", kFP_LAND);
   c->AddEntry("landaun",kFP_LANDN);
   c->AddEntry("pol0",   kFP_POL0);
   c->AddEntry("pol1",   kFP_POL1);
   c->AddEntry("pol2",   kFP_POL2);
   c->AddEntry("pol3",   kFP_POL3);
   c->AddEntry("pol4",   kFP_POL4);
   c->AddEntry("pol5",   kFP_POL5);
   c->AddEntry("pol6",   kFP_POL6);
   c->AddEntry("pol7",   kFP_POL7);
   c->AddEntry("pol8",   kFP_POL8);
   c->AddEntry("pol9",   kFP_POL9);
   c->AddEntry("user",   kFP_USER);

   if (!gROOT->GetFunction("gaus")) {
      Float_t xmin = 1.;
      Float_t xmax = 2.;
      new TF1("gaus","gaus",xmin,xmax);
      new TF1("landau","landau",xmin,xmax);
      new TF1("expo","expo",xmin,xmax);
      for (Int_t i=0; i<10; i++) {
         new TF1(Form("pol%d",i),Form("pol%d",i),xmin,xmax);
      }
   }
   return c;
}

//______________________________________________________________________________
TGComboBox* TFitEditor::BuildMethodList(TGFrame* parent, Int_t id)
{
   // Create method list combo box.

   TGComboBox *c = new TGComboBox(parent, id);
   c->AddEntry("Chi-square", kFP_MCHIS);
   c->AddEntry("Binned Likelihood", kFP_MBINL);
   //c->AddEntry("Unbinned Likelihood", kFP_MUBIN); //for lated use
   //c->AddEntry("User", kFP_MUSER);                //for later use
   c->Select(kFP_MCHIS);
   return c;
}

//______________________________________________________________________________
void TFitEditor::CloseWindow()
{
   // Close fit panel window.

   DeleteWindow();
}

//______________________________________________________________________________
void TFitEditor::RecursiveRemove(TObject* obj)
{
   // When obj is deleted, clear fFitObject if fFitObject = obj.  

   if (obj == fFitObject) fFitObject = 0;
}

//______________________________________________________________________________
void TFitEditor::DoAddtoList()
{
   // Slot connected to 'add to list of function' setting.

   if (fAdd2FuncList->GetState() == kButtonDown)
      fFitOption += '+';
   else {
      Int_t eq = fFitOption.First('+');
      fFitOption.Remove(eq, 1);
   }
}

//______________________________________________________________________________
void TFitEditor::DoAdvancedOptions()
{
   // Slot connected to advanced option button (opens a dialog).

   new TGMsgBox(fClient->GetRoot(), GetMainFrame(),
                "Info", "Advanced option dialog is not implemented yet",
                kMBIconAsterisk,kMBOk, 0);
}

//______________________________________________________________________________
void TFitEditor::DoAllWeights1()
{
   // Slot connected to 'set all weights to 1' setting.

   if (fAllWeights1->GetState() == kButtonDown)
      fFitOption += 'W';
   else {
      Int_t eq = fFitOption.First('W');
      fFitOption.Remove(eq, 1);
   }
}

//______________________________________________________________________________
void TFitEditor::DoClose()
{
   // Close the fit panel.

   TTimer::SingleShot(50, "TFitEditor", this, "CloseWindow()");
}

//______________________________________________________________________________
void TFitEditor::DoDrawSame()
{
   // Slot connected to 'same' draw option.
   
   fFitOption.ToUpper();
   if (fDrawOption.Contains("SAME"))
      return;
   else
      fDrawOption += "SAME";
}

//______________________________________________________________________________
void TFitEditor::DoFit()
{
   // Perform a fit with current parameters' settings.

   if (!fFitObject) return;
   if (!fParentPad) return;
   if (!fFitFunc) {
      new TGMsgBox(fClient->GetRoot(), GetMainFrame(),
                   "Error", Form("Function with name '%s' does not exist", 
                   fFunction.Data()), kMBIconExclamation, kMBClose, 0);
      return;
      }

   fParentPad->cd();
   fParentPad->GetCanvas()->SetCursor(kWatch);
   Double_t xmin, xmax;
   switch (fType) {
      case kObjectHisto: {
         TH1 *h1 = (TH1*)fFitObject;
         Float_t xmin = fXaxis->GetBinLowEdge((Int_t)(fSliderX->GetMinPosition()));
         Float_t xmax = fXaxis->GetBinUpEdge((Int_t)(fSliderX->GetMaxPosition()));
         fFitFunc->SetRange(xmin,xmax);
         h1->Fit(fFitFunc, fFitOption.Data(), fDrawOption.Data());
         break;
      }
      case kObjectGraph: {
         TGraph *gr = (TGraph*)fFitObject;
         TH1F *hist = gr->GetHistogram();
         if (hist) {
            xmin = fXaxis->GetBinLowEdge((Int_t)(fSliderX->GetMinPosition()));
            xmax = fXaxis->GetBinUpEdge((Int_t)(fSliderX->GetMaxPosition()));
            Int_t npoints = gr->GetN();
            Double_t *gx = gr->GetX();
            Double_t gxmin, gxmax;
            gxmin = gx[0];
            gxmax = gx[npoints-1];
            Double_t err0 = gr->GetErrorX(0);
            Double_t errn = gr->GetErrorX(npoints-1);
            if (err0 > 0) 
               gxmin -= 2*err0;
            if (errn > 0) 
               gxmax += 2*errn;
            for (Int_t i=0; i<npoints; i++) {
               if (gx[i] < xmin) 
                  gxmin = gx[i];
               if (gx[i] > xmax) 
                  gxmax = gx[i];
            }
            if (xmin < gxmin) xmin = gxmin;
            if (xmax > gxmax) xmax = gxmax;
         }
         fFitFunc->SetRange(xmin,xmax);
         gr->Fit(fFitFunc, (char*)fFitOption.Data(), 
                (char*)fDrawOption.Data());
         break;
      }
      case kObjectGraph2D: {
         // N/A
         break;
      }
      case kObjectHStack: {
         // N/A
         break;
      }
      case kObjectTree:  {
         // N/A
         break;
      }
   }
   fParentPad->Modified();
   fParentPad->Update();
   fParentPad->GetCanvas()->SetCursor(kPointer);
}

//______________________________________________________________________________
Int_t TFitEditor::CheckFunctionString(const char *fname)
{
   // Check entered function string.

   TFormula *form = 0;
   form = new TFormula(fname, fname);
   if (form) {
      return form->Compile();
   }
   return -1;
 }

//______________________________________________________________________________
void TFitEditor::DoAddition(Bool_t on)
{
   // Slot connected to addition of predefined functions.
   
   static Bool_t first = kFALSE;
   TString s = fEnteredFunc->GetText();
   if (on) {
      if (!first) {
         s += "(0)";
         fEnteredFunc->SetText(s.Data());
         first = kTRUE;
         fSelLabel->SetText(fFunction.Data());
         ((TGCompositeFrame *)fSelLabel->GetParent())->Layout();
      }
   } else {
      first = kFALSE;
   }
}

//______________________________________________________________________________
void TFitEditor::DoNoOperation(Bool_t on)
{
   // Slot connected to NOP of predefined functions.
   
   TGTextLBEntry *te = (TGTextLBEntry *)fFuncList->GetSelectedEntry();
   if (on) {
      fEnteredFunc->SetText(te->GetTitle());
   }    
   fFunction = fEnteredFunc->GetText();
   fSelLabel->SetText(fFunction.Data());
   ((TGCompositeFrame *)fSelLabel->GetParent())->Layout();
}

//______________________________________________________________________________
void TFitEditor::DoFunction(Int_t)
{
   // Slot connected to predefined fit function settings.

   TGTextLBEntry *te = (TGTextLBEntry *)fFuncList->GetSelectedEntry();
   if (fNone->GetState() == kButtonDown) {
      fEnteredFunc->SetText(te->GetTitle());
   } else if (fAdd->GetState() == kButtonDown) {
      TString s = fEnteredFunc->GetText();
      TFormula tmp("tmp", fFunction.Data());
      Int_t np = tmp.GetNpar();
      s += Form("+%s(%d)", te->GetTitle(), np);
      fEnteredFunc->SetText(s.Data());
   }
   fFunction = fEnteredFunc->GetText();
   
   // create TF1 with the passed string. Delete previous one if existing
   if (fFitFunc) delete fFitFunc; 
   fFitFunc = new TF1("fitFunc",fFunction.Data(),fXmin,fXmax);
   
   if (fFunction.Contains("pol"))
      fLinearFit->SetState(kButtonDown);
   else
      fLinearFit->SetState(kButtonUp);

/*   if (fFunction.Contains("user")) {
      fFitOption += 'U';
   } else if (fFitOption.Contains('U')) {
      Int_t eq = fFitOption.First('U');
      fFitOption.Remove(eq, 1);
   }*/

   fEnteredFunc->SelectAll();
   fSelLabel->SetText(fFunction.Data());
   ((TGCompositeFrame *)fSelLabel->GetParent())->Layout();
}

//______________________________________________________________________________
void TFitEditor::DoEnteredFunction()
{
   // Slot connected to entered function in text entry.

   Int_t ok = CheckFunctionString(fEnteredFunc->GetText());

   if (ok != 0) {
      new TGMsgBox(fClient->GetRoot(), GetMainFrame(),
                   "Error...", "Verify the entered function string!",
                   kMBIconStop,kMBOk, 0);   
   }
   
   fFunction = fEnteredFunc->GetText();
   if (fFitFunc) delete fFitFunc; 
   fFitFunc = new TF1("fitFunc",fFunction.Data(),fXmin,fXmax);
   
   fSelLabel->SetText(fFunction.Data());
   ((TGCompositeFrame *)fSelLabel->GetParent())->Layout();
   if (fFunction.Contains("++")) {
      fLinearFit->SetState(kButtonDown);
      fAdd->SetState(kButtonDown);
   } else if (fFunction.Contains('+')) {
      fAdd->SetState(kButtonDown);
   } else {
      fNone->SetState(kButtonDown);
   }
}

//______________________________________________________________________________
void TFitEditor::DoImproveResults()
{
   // Slot connected to 'improve fit results' option settings.

   if (fImproveResults->GetState() == kButtonDown)
      fFitOption += 'M';
   else if (fFitOption.Contains('M')) 
      fFitOption.ReplaceAll('M', "");
}

//______________________________________________________________________________
void TFitEditor::DoBestErrors()
{
   // Slot connected to 'best errors' option settings.

   if (fBestErrors->GetState() == kButtonDown)
      fFitOption += 'E';
   else if (fFitOption.Contains('E')) 
      fFitOption.ReplaceAll('E', "");
}

//______________________________________________________________________________
void TFitEditor::DoIntegral()
{
   // Slot connected to 'integral' option settings.

   if (fIntegral->GetState() == kButtonDown)
      fFitOption += 'I';
   else if (fFitOption.Contains('I')) 
      fFitOption.ReplaceAll('I', "");
}

//______________________________________________________________________________
void TFitEditor::DoLinearFit()
{
   // Slot connected to linear fit settings.

   if (fLinearFit->GetState() == kButtonDown) {
      fPlus = "++";
      if (fFitOption.Contains('F'))
         fFitOption.ReplaceAll('F', "");
   } else {
      fPlus = '+';
      if (fFunction.Contains("pol") || fFunction.Contains("++"))
         fFitOption += 'F';
   }
}

//______________________________________________________________________________
void TFitEditor::DoMethod(Int_t id)
{
   // Slot connected to fit method settings.
   
   if (id == kFP_MCHIS) {
      if (fFitOption.Contains('L')) 
         fFitOption.ReplaceAll('L', "");
   } else {
      fFitOption += 'L';
   }
}

//______________________________________________________________________________
void TFitEditor::DoNoChi2()
{
   // Slot connected to 'no chi2' option settings.

   if (fNoChi2->GetState() == kButtonDown)
      fFitOption += 'C';
   else if (fFitOption.Contains('C')) 
      fFitOption.ReplaceAll('C', "");
   
   if (fLinearFit->GetState() == kButtonUp)
      fLinearFit->SetState(kButtonDown);
}

//______________________________________________________________________________
void TFitEditor::DoNoDrawing()
{
   // Slot connected to 'no drawing' settings.

   if (fNoDrawing->GetState() == kButtonDown)
      fFitOption += '0';
   else if (fFitOption.Contains('0')) 
      fFitOption.ReplaceAll('0', "");
}

//______________________________________________________________________________
void TFitEditor::DoNoStoreDrawing()
{
   // Slot connected to 'no storing, no drawing' settings.

   if (fNoStoreDrawing->GetState() == kButtonDown)
      fFitOption += 'N';
   else if (fFitOption.Contains('N')) 
      fFitOption.ReplaceAll('N', "");
   
   if (fNoDrawing->GetState() == kButtonUp)
      fNoDrawing->SetState(kButtonDown);
}

//______________________________________________________________________________
void TFitEditor::DoPrintOpt(Bool_t on)
{
   // Slot connected to print option settings.

   TGButton *btn = (TGButton *) gTQSender;
   Int_t id = btn->WidgetId();
   switch (id) {
      case kFP_PDEF:
         if (on) {
            fOptDefault->SetState(kButtonDown);
            fOptVerbose->SetState(kButtonUp);
            fOptQuiet->SetState(kButtonUp);
            if (fFitOption.Contains('Q')) {
               fFitOption.ReplaceAll('Q', "");
            }
            if (fFitOption.Contains('V')) {
               fFitOption.ReplaceAll('V', "");
            }
         }
         break;
      case kFP_PVER:
         if (on) {
            fOptVerbose->SetState(kButtonDown);
            fOptDefault->SetState(kButtonUp);
            fOptQuiet->SetState(kButtonUp);
            if (fFitOption.Contains('Q')) {
               fFitOption.ReplaceAll('Q', "");
            }
            fFitOption += 'V';
         }
         break;
      case kFP_PQET:
         if (on) {
            fOptQuiet->SetState(kButtonDown);
            fOptDefault->SetState(kButtonUp);
            fOptVerbose->SetState(kButtonUp);
            if (fFitOption.Contains('V')) {
               fFitOption.ReplaceAll('V', "");
            }
            fFitOption += 'Q';
         }
      default:
         break;
   }
}

//______________________________________________________________________________
void TFitEditor::DoReset()
{
   // Reset all fit parameters.

   fFitOption = 'R';
   fDrawOption = "";
   fFunction = "gaus";
   fFuncList->Select(1);
   if (fFitFunc) delete fFitFunc;
   fFitFunc = new TF1("fitFunc",fFunction.Data(),fXmin,fXmax);
   fPlus = '+';
   if (fLinearFit->GetState() == kButtonDown)
      fLinearFit->SetState(kButtonUp, kFALSE);
   if (fBestErrors->GetState() == kButtonDown)
      fBestErrors->SetState(kButtonUp, kFALSE);
   if (fUseRange->GetState() == kButtonUp)
      fUseRange->SetState(kButtonDown, kFALSE);
   if (fAllWeights1->GetState() == kButtonDown)
      fAllWeights1->SetState(kButtonUp, kFALSE);
   if (fImproveResults->GetState() == kButtonDown)
      fImproveResults->SetState(kButtonUp, kFALSE);
   if (fAdd2FuncList->GetState() == kButtonDown)
      fAdd2FuncList->SetState(kButtonUp, kFALSE);
   if (fNoChi2->GetState() == kButtonDown)
      fNoChi2->SetState(kButtonUp, kFALSE);
   if (fDrawSame->GetState() == kButtonDown)
      fDrawSame->SetState(kButtonUp, kFALSE);
   if (fNoDrawing->GetState() == kButtonDown)
      fNoDrawing->SetState(kButtonUp, kFALSE);
   if (fNoStoreDrawing->GetState() == kButtonDown)
      fNoStoreDrawing->SetState(kButtonUp, kFALSE);
   if (fLinearFit->GetState() == kButtonUp)
      fRobustValue->SetState(kFALSE);
   else
      fRobustValue->SetState(kTRUE);
   fOptDefault->SetState(kButtonDown);
   fNone->SetState(kButtonDown);
}

//______________________________________________________________________________
void TFitEditor::DoRobust()
{
   // Slot connected to robust setting of linear fit.

   if (fType != kObjectGraph) return;
   
   fRobustValue->SetState(kTRUE);
   if (fFitOption.Contains("ROB")) {
      Int_t pos = fFitOption.Index("=");
      fFitOption.Replace(pos+1, 4, Form("%g",fRobustValue->GetNumber()));
   } else {
      fFitOption += Form("ROB=%g", fRobustValue->GetNumber());
   }
}

//______________________________________________________________________________
void TFitEditor::DoSetParameters()
{
   // Open set parameters dialog.

   if (!fFitFunc) {
      printf("SetParamters - create fit function %s\n",fFunction.Data());
      fFitFunc = new TF1("fitFunc",Form("%s",fFunction.Data()), fXmin, fXmax);
   }
   new TFitParametersDialog(gClient->GetDefaultRoot(), GetMainFrame(), 
                            fFitFunc, fParentPad, fXmin, fXmax);
}

//______________________________________________________________________________
void TFitEditor::DoSliderXPressed()
{
   // Slot connected to range settings on x-axis. 

   if (!fParentPad) return;
   fParentPad->cd();
   fParentPad->GetCanvas()->FeedbackMode(kFALSE);
   fParentPad->SetLineWidth(1);
   fParentPad->SetLineColor(2);
   Float_t xleft, xright;
   switch (fType) {
      case kObjectHisto: {
         //hist 1dim
         xleft  = fXaxis->GetBinLowEdge((Int_t)((fSliderX->GetMinPosition())+0.5));
         xright = fXaxis->GetBinUpEdge((Int_t)((fSliderX->GetMaxPosition())+0.5));
         break;
      }
      case kObjectGraph: {
         // graph
         xleft  = fXaxis->GetBinLowEdge((Int_t)((fSliderX->GetMinPosition())+0.5));
         xright = fXaxis->GetBinUpEdge((Int_t)((fSliderX->GetMaxPosition())+0.5));
         break;
      }
      case kObjectGraph2D: {
         // N/A
         break;
      }
      case kObjectHStack: {
         // N/A
         break;
      }
      case kObjectTree:  {
         // N/A
         break;
      }
   }
   Float_t ymin = fParentPad->GetUymin();
   Float_t ymax = fParentPad->GetUymax();
   fPx1old = fParentPad->XtoAbsPixel(xleft);
   fPy1old = fParentPad->YtoAbsPixel(ymin);
   fPx2old = fParentPad->XtoAbsPixel(xright);
   fPy2old = fParentPad->YtoAbsPixel(ymax);
   gVirtualX->DrawBox(fPx1old, fPy1old, fPx2old, fPy2old, TVirtualX::kHollow);
}

//______________________________________________________________________________
void TFitEditor::DoSliderXMoved()
{
   // Slot connected to range settings on x-axis. 
   
   Int_t px1,py1,px2,py2;
   Float_t xleft, xright;
   switch (fType) {
      case kObjectHisto: {
         xleft  = fXaxis->GetBinLowEdge((Int_t)((fSliderX->GetMinPosition())+0.5));
         xright = fXaxis->GetBinUpEdge((Int_t)((fSliderX->GetMaxPosition())+0.5));
         break; 
      }
      case kObjectGraph: {
         // graph
         xleft  = fXaxis->GetBinLowEdge((Int_t)((fSliderX->GetMinPosition())+0.5));
         xright = fXaxis->GetBinUpEdge((Int_t)((fSliderX->GetMaxPosition())+0.5));
/*        TGraph *gr = (TGraph *)fFitObject;
         Int_t np = gr->GetN();
         Double_t *x = gr->GetX();
         xleft  = x[0];
         xright = x[0];
         for(Int_t i=0; i<np; i++) {
            if (xleft > x[i])
               xleft = x[i];
            if (xright < x[i])
               xright = x[i];
         }*/
         break;
      }
      case kObjectGraph2D: {
         // N/A
         break;
      }
      case kObjectHStack: {
         // N/A
         break;
      }
      case kObjectTree:  {
         // N/A
         break;
      }
   }
   Float_t ymin = fParentPad->GetUymin();
   Float_t ymax = fParentPad->GetUymax();
   px1 = fParentPad->XtoAbsPixel(xleft);
   py1 = fParentPad->YtoAbsPixel(ymin);
   px2 = fParentPad->XtoAbsPixel(xright);
   py2 = fParentPad->YtoAbsPixel(ymax);
   fParentPad->GetCanvas()->FeedbackMode(kTRUE);
   fParentPad->cd();
   fParentPad->SetLineWidth(1);
   fParentPad->SetLineColor(2);
   gVirtualX->DrawBox(fPx1old, fPy1old, fPx2old, fPy2old, TVirtualX::kHollow);
   gVirtualX->DrawBox(px1, py1, px2, py2, TVirtualX::kHollow);
   fPx1old = px1;
   fPy1old = py1;
   fPx2old = px2 ;
   fPy2old = py2;
   gVirtualX->Update(0);
}

//______________________________________________________________________________
void TFitEditor::DoSliderXReleased()
{
   // Slot connected to range settings on x-axis. 

   fParentPad->Modified();
   fParentPad->Update();
}

//______________________________________________________________________________
void TFitEditor::DoSliderYPressed()
{
   // Slot connected to range settings on y-axis. 

   switch (fType) {
      case kObjectHisto: {
         if (!fParentPad) return;
         fParentPad->cd();
         fParentPad->GetCanvas()->FeedbackMode(kFALSE);
         fParentPad->SetLineWidth(1);
         fParentPad->SetLineColor(2);
         //hist 1dim
         Float_t ybottom = fYaxis->GetBinLowEdge((Int_t)((fSliderY->GetMinPosition())+0.5));
         Float_t ytop = fYaxis->GetBinUpEdge((Int_t)((fSliderY->GetMaxPosition())+0.5));
         Float_t xmin = fParentPad->GetUxmin();
         Float_t xmax = fParentPad->GetUxmax();
         fPx1old = fParentPad->XtoAbsPixel(xmin);
         fPy1old = fParentPad->YtoAbsPixel(ybottom);
         fPx2old = fParentPad->XtoAbsPixel(xmax);
         fPy2old = fParentPad->YtoAbsPixel(ytop);
         gVirtualX->DrawBox(fPx1old, fPy1old, fPx2old, fPy2old, TVirtualX::kHollow);
      }
      case kObjectGraph: {
         // N/A
         break;
      }
      case kObjectGraph2D: {
         // N/A
         break;
      }
      case kObjectHStack: {
         // N/A
         break;
      }
      case kObjectTree:  {
         // N/A
         break;
      }
   }
}

//______________________________________________________________________________
void TFitEditor::DoSliderYMoved()
{
   // Slot connected to range settings on y-axis. 

   switch (fType) {
      case kObjectHisto: {
         Int_t px1,py1,px2,py2;
         Float_t ybottom = fYaxis->GetBinLowEdge((Int_t)((fSliderY->GetMinPosition())+0.5));
         Float_t ytop = fYaxis->GetBinUpEdge((Int_t)((fSliderY->GetMaxPosition())+0.5));
         Float_t xmin = fParentPad->GetUxmin();
         Float_t xmax = fParentPad->GetUxmax();
         px1 = fParentPad->XtoAbsPixel(xmin);
         py1 = fParentPad->YtoAbsPixel(ybottom);
         px2 = fParentPad->XtoAbsPixel(xmax);
         py2 = fParentPad->YtoAbsPixel(ytop);
         fParentPad->GetCanvas()->FeedbackMode(kTRUE);
         fParentPad->cd();
         fParentPad->SetLineWidth(1);
         fParentPad->SetLineColor(2);
         gVirtualX->DrawBox(fPx1old, fPy1old, fPx2old, fPy2old, TVirtualX::kHollow);
         gVirtualX->DrawBox(px1, py1, px2, py2, TVirtualX::kHollow);
         fPx1old = px1;
         fPy1old = py1;
         fPx2old = px2 ;
         fPy2old = py2;
         gVirtualX->Update(0);
      }
      case kObjectGraph: {
         // N/A
         break;
      }
      case kObjectGraph2D: {
         // N/A
         break;
      }
      case kObjectHStack: {
         // N/A
         break;
      }
      case kObjectTree:  {
         // N/A
         break;
      }
   }
}

//______________________________________________________________________________
void TFitEditor::DoSliderYReleased()
{
   // Slot connected to range settings on y-axis. 

   fParentPad->Modified();
   fParentPad->Update();
}

//______________________________________________________________________________
void TFitEditor::DoSliderZPressed()
{
   // Slot connected to range settings on z-axis. 
   
}

//______________________________________________________________________________
void TFitEditor::DoSliderZMoved()
{
   // Slot connected to range settings on z-axis. 

}

//______________________________________________________________________________
void TFitEditor::DoSliderZReleased()
{
   // Slot connected to range settings on z-axis. 

}

//______________________________________________________________________________
void TFitEditor::DoUserDialog()
{
   // Open a dialog for getting a user defined method.

   new TGMsgBox(fClient->GetRoot(), GetMainFrame(),
                "Info", "Dialog of user method is not implemented yet",
                kMBIconAsterisk,kMBOk, 0);
}

//______________________________________________________________________________
void TFitEditor::DoUseRange()
{
   // Slot connected to fit range settings.

   if (fUseRange->GetState() == kButtonDown)
      fFitOption.Insert(0,'R');
   else  {
      Int_t pos = fFitOption.First('R');
      Int_t rob = fFitOption.Index("ROB");
      if (pos != rob)
         fFitOption.Remove(pos, 1);
   }
}

//______________________________________________________________________________
void TFitEditor::SetFunction(const char *function)
{
   // Set the function to be used in performed fit.

   fFunction = function;
}
