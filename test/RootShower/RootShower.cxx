// Author: Bertrand Bellenot   22/08/02

/*************************************************************************
 * Copyright (C) 1995-2002, Bertrand Bellenot.                           *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see the LICENSE file.                         *
 *************************************************************************/


#include <Riostream.h>
#include <time.h>

#include <TROOT.h>
#include <TStyle.h>
#include <TApplication.h>
#include <TVirtualX.h>
#include <TEnv.h>

#include <TFile.h>
#include <TTree.h>
#include <TFrame.h>
#include <TH1.h>
#include <TF1.h>

#include <TGMenu.h>
#include <TGFileDialog.h>
#include <TGTextEdit.h>
#include <TRootEmbeddedCanvas.h>
#include <TCanvas.h>
#include <TRandom.h>
#include <TSystem.h>
#include <TRootHelpDialog.h>
#include <TGStatusBar.h>
#include <TBrowser.h>
#include <TParticle.h>
#include "RootShower.h"
#include "MyParticle.h"
#include "GTitleFrame.h"
#include "GButtonFrame.h"
#include "RSMsgBox.h"
#include "RSAbout.h"
#include "SettingsDlg.h"
#include "RSHelpText.h"
#include "MyEvent.h"

#include <TGeometry.h>
#include <TBRIK.h>
#include <TMixture.h>
#include <TNode.h>
#include <TView.h>
#include <TGToolBar.h>

#include <THtml.h>

#ifndef _CONSTANTS_H_
#include "constants.h"
#endif

enum RootShowerMessageTypes {
    M_FILE_OPEN,
    M_FILE_SAVEAS,
    M_FILE_HTML,
    M_FILE_EXIT,
    M_EVENT_NEXT,
    M_EVENT_SELECT,
    M_INTERRUPT_SIMUL,
    M_ZOOM_PLUS,
    M_ZOOM_MOINS,
    M_ZOOM_PLUS2,
    M_ZOOM_MOINS2,

    M_SHOW_PROCESS,
    M_ANIMATE_GIF,
    M_SETTINGS_DLG,
    M_SETTINGS_SAVE,
    M_SHOW_INFOS,
    M_SHOW_3D,
    M_SHOW_TRACK,

    M_VIEW_TOOLBAR,
    M_INSPECT_BROWSER,

    M_HELP_PHYSICS,
    M_HELP_SIMULATION,
    M_HELP_LICENSE,
    M_HELP_ABOUT,
};

const char *xpm_names[] = {
    "open.xpm",
    "save.xpm",
    "",
    "settings.xpm",
    "",
    "infos.xpm",
    "view3d.xpm",
    "",
    "browser.xpm",
    "",
    "manual.xpm",
    "help.xpm",
    "license.xpm",
    "about.xpm",
    "",
    "quit.xpm",
    0
};

ToolBarData_t tb_data[] = {
  { "", "Open Root event file",     kFALSE, M_FILE_OPEN,        NULL },
  { "", "Save event in Root file",  kFALSE, M_FILE_SAVEAS,      NULL },
  { "",              0,             0,      -1,                 NULL },
  { "", "Event settings",           kFALSE, M_SETTINGS_DLG,     NULL },
  { "",              0,             0,      -1,                 NULL },
  { "", "Infos on current event",   kFALSE, M_SHOW_INFOS,       NULL },
  { "", "Open 3D viewer",           kFALSE, M_SHOW_3D,          NULL },
  { "",              0,             0,      -1,                 NULL },
  { "", "Start Root browser",       kFALSE, M_INSPECT_BROWSER,  NULL },
  { "",              0,             0,      -1,                 NULL },
  { "", "Physics recalls",          kFALSE, M_HELP_PHYSICS,     NULL },
  { "", "RootShower help",          kFALSE, M_HELP_SIMULATION,  NULL },
  { "", "Display license",          kFALSE, M_HELP_LICENSE,     NULL },
  { "", "About RootShower",         kFALSE, M_HELP_ABOUT,       NULL },
  { "",              0,             0,      -1,                 NULL },
  { "", "Exit Application",         kFALSE, M_FILE_EXIT,        NULL },
  { NULL,            NULL,          0,      0,                  NULL }
};

RootShower      *gRootShower;
Int_t            gColIndex;
TGListTree      *gEventListTree; // event selection TGListTree
TGListTreeItem  *gBaseLTI;
TGListTreeItem  *gTmpLTI;
TGListTreeItem  *gLTI[MAX_PARTICLE];

const TGPicture *bpic, *bspic;
const TGPicture *lpic, *lspic;

const Char_t *filetypes[] = {
    "ROOT files",    "*.root",
    "ROOT macros",   "*.C",
    "GIF  files",    "*.gif",
    "PS   files",    "*.ps",
    "EPS  files",    "*.eps",
    "All files",     "*",
    0,               0
};

enum EGeometrySettingsDialogMessageTypes {
    kM_BUTTON_OK,
    kM_BUTTON_CANCEL,
    kM_COMBOBOX_CHANNELID,
    kM_COMBOBOX_TDC
};

//_________________________________________________
// RootShower
//

Int_t RootShower::fgDefaultXPosition = 1;
Int_t RootShower::fgDefaultYPosition = 1;


//______________________________________________________________________________
RootShower::RootShower(const TGWindow *p, UInt_t w, UInt_t h):
  TGMainFrame(p, w, h)
{
    // Create (the) Event Display.
    //
    // p = pointer to GMainFrame (not owner)
    // w = width of RootShower frame
    // h = width of RootShower frame
    fOk           = false;
    fModified     = false;
    fShowProcess  = false;
    fCreateGIFs   = false;
    fTimer        = 0;
    fPicIndex       = 1;

    fRootShowerEnv = new TEnv(".rootshowerrc");

    fFirstParticle = fRootShowerEnv->GetValue("RootShower.fFirstParticle", PHOTON);
    fE0            = fRootShowerEnv->GetValue("RootShower.fE0", 25.0);
    fB             = fRootShowerEnv->GetValue("RootShower.fB", 5.26);
    fMaterial      = fRootShowerEnv->GetValue("RootShower.fMaterial", Polystyrene);
    fDimX          = fRootShowerEnv->GetValue("RootShower.fDimX", 150.0);
    fDimY          = fRootShowerEnv->GetValue("RootShower.fDimY", 150.0);
    fDimZ          = fRootShowerEnv->GetValue("RootShower.fDimZ", 150.0);
    fPicNumber     = fRootShowerEnv->GetValue("RootShower.fPicNumber", 24);
    fPicDelay      = fRootShowerEnv->GetValue("RootShower.fPicDelay", 100);
    fPicReset      = fRootShowerEnv->GetValue("RootShower.fPicReset", 1);

    fMaxV = TMath::Max(fDimX,TMath::Max(fDimY,fDimZ));
    fMaxV /= 3.0;
    fMinV = -1.0 * fMaxV;

    fEventNr = 0;
    fNRun    = 0;
    time( &fEventTime );

    bpic = gClient->GetPicture("branch_t.xpm");
    bspic = gClient->GetPicture("branch_t.xpm");

    lpic = gClient->GetPicture("leaf_t.xpm");
    lspic = gClient->GetPicture("leaf_t.xpm");

    // Create menubar and popup menus.
    MakeMenuBarFrame();

    //---- toolbar

    int spacing = 8;
    fToolBar = new TGToolBar(this, 60, 20, kHorizontalFrame | kRaisedFrame);
    for (int i = 0; xpm_names[i]; i++) {
        char *iconname = new char[100];
#ifdef R__WIN32
        sprintf(iconname,"%s\\icons\\%s",gProgPath,xpm_names[i]);
#else
        sprintf(iconname,"%s/icons/%s",gProgPath,xpm_names[i]);
#endif
        tb_data[i].fPixmap = iconname;
        if (strlen(xpm_names[i]) == 0) {
            spacing = 8;
            continue;
        }
        fToolBar->AddButton(this, &tb_data[i], spacing);
        spacing = 0;
    }
    AddFrame(fToolBar, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 0, 0, 0, 0));

    // Layout hints
    fL1 = new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0);
    fL2 = new TGLayoutHints(kLHintsCenterX | kLHintsExpandX, 0, 0, 0, 0);
    fL3 = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX | kLHintsExpandY,
                            0, 0, 0, 0);
    fL4 = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandY, 5, 5, 2, 2);
    fL5 = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX | kLHintsExpandY,
                            2, 2, 2, 2);
    fL6 = new TGLayoutHints(kLHintsBottom| kLHintsExpandX, 0, 0, 0, 0);
    fL7 = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX | kLHintsExpandY,
                            5, 5, 2, 2);
    fL8 = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX, 0, 0, 0, 0);

    // CREATE TITLE FRAME
    fTitleFrame = new GTitleFrame(this, "ROOT Shower Monte Carlo", "Event Display", 100, 100);
    AddFrame(fTitleFrame, fL2);

    // CREATE MAIN FRAME
    fMainFrame = new TGCompositeFrame(this, 100, 100, kHorizontalFrame | kRaisedFrame);

    // Create Selection frame (i.e. with buttons and geometry selection widgets)
    fSelectionFrame = new TGCompositeFrame(fMainFrame, 100, 100, kVerticalFrame);
    // create button frame
    fButtonFrame = new GButtonFrame (fSelectionFrame, this, M_EVENT_NEXT,
                                     M_EVENT_SELECT, M_INTERRUPT_SIMUL);
    fSelectionFrame->AddFrame(fButtonFrame, fL8);

    fTreeView = new TGCanvas(fSelectionFrame, 150, 10, kSunkenFrame | kDoubleBorder);
    fEventListTree = new TGListTree(fTreeView->GetViewPort(), 10, 10, kHorizontalFrame);
    gEventListTree = fEventListTree;
	fEventListTree->SetCanvas(fTreeView);
    fEventListTree->Associate(this);
    BuildEventTree();
    fTreeView->SetContainer(fEventListTree);
    fSelectionFrame->AddFrame(fTreeView, fL4);

    fMainFrame->AddFrame(fSelectionFrame, fL4);

    // Create Display frame
    fDisplayFrame = new TGTab(fMainFrame, 580, 360);

    // Create Display Canvas Tab (where the actual main event is displayed)
    TGCompositeFrame *tFrame = fDisplayFrame->AddTab("Main Event (Shower)");

    // Create Layout hints
    fZoomButtonsLayout = new TGLayoutHints(kLHintsBottom | kLHintsLeft | kLHintsExpandX, 5, 2, 2, 2);

    fHFrame = new TGHorizontalFrame(tFrame,0,0,0);
    tFrame->AddFrame(fHFrame, new TGLayoutHints(kLHintsBottom | kLHintsLeft | kLHintsExpandX, 5, 5, 5, 5));
    // Create Zoom Buttons
    fZoomPlusButton = new TGTextButton(fHFrame, "&Zoom Forward", M_ZOOM_PLUS);
    fZoomPlusButton->Associate(this);
    fZoomPlusButton->SetToolTipText("Zoom forward event view");
    fHFrame->AddFrame(fZoomPlusButton, fZoomButtonsLayout);
    fZoomMoinsButton = new TGTextButton(fHFrame, "Zoom &Backward", M_ZOOM_MOINS);
    fZoomMoinsButton->Associate(this);
    fZoomMoinsButton->SetToolTipText("Zoom backward event view");
    fHFrame->AddFrame(fZoomMoinsButton, fZoomButtonsLayout);

    fEmbeddedCanvas = new TRootEmbeddedCanvas("fEmbeddedCanvas", tFrame, 580, 360);
    tFrame->AddFrame(fEmbeddedCanvas, fL5);
    fEmbeddedCanvas->GetCanvas()->SetBorderMode(0);
    cA = fEmbeddedCanvas->GetCanvas();
    cA->SetFillColor(1);

    // Create Display Canvas Tab (where the selected event is displayed)
    TGCompositeFrame *tFrame2 = fDisplayFrame->AddTab("Selected Track");

    fHFrame2 = new TGHorizontalFrame(tFrame2,0,0,0);
    tFrame2->AddFrame(fHFrame2, new TGLayoutHints(kLHintsBottom | kLHintsLeft | kLHintsExpandX, 5, 5, 5, 5));
    // Create Zoom Buttons
    fZoomPlusButton2 = new TGTextButton(fHFrame2, "&Zoom Forward", M_ZOOM_PLUS2);
    fZoomPlusButton2->Associate(this);
    fZoomPlusButton2->SetToolTipText("Zoom forward event view");
    fHFrame2->AddFrame(fZoomPlusButton2, fZoomButtonsLayout);
    fZoomMoinsButton2 = new TGTextButton(fHFrame2, "Zoom &Backward", M_ZOOM_MOINS2);
    fZoomMoinsButton2->Associate(this);
    fZoomMoinsButton2->SetToolTipText("Zoom backward event view");
    fHFrame2->AddFrame(fZoomMoinsButton2, fZoomButtonsLayout);

    fEmbeddedCanvas2 = new TRootEmbeddedCanvas("fEmbeddedCanvas2", tFrame2, 580, 360);
    tFrame2->AddFrame(fEmbeddedCanvas2, fL5);
    fEmbeddedCanvas2->GetCanvas()->SetBorderMode(0);
    cB = fEmbeddedCanvas2->GetCanvas();
    cB->SetFillColor(1);
    cB->cd();
    fSelection = new TGeometry("selection","selection");
    fSelection->cd();
    sel_detect = new TBRIK("sel_detect","sel_detect","void",fDimX/2.0,fDimY/2.0,fDimZ/2.0);
    sel_detect->SetLineColor(7);
    sel_node = new TNode("SEL_NODE","SEL_NODE","sel_detect");
    sel_node->cd();
    fSelection->Draw();
    cB->GetView()->SetRange(fMinV,fMinV,fMinV,fMaxV,fMaxV,fMaxV);
    cB->GetView()->SetPerspective();
    cB->GetView()->SideView();
    cB->Update();

    // Create Display Canvas Tab (where the histogram is displayed)
    TGCompositeFrame *tFrame3 = fDisplayFrame->AddTab("Statistics");

    fEmbeddedCanvas3 = new TRootEmbeddedCanvas("fEmbeddedCanvas3", tFrame3, 580, 360);
    tFrame3->AddFrame(fEmbeddedCanvas3, fL5);
    fEmbeddedCanvas3->GetCanvas()->SetBorderMode(0);
    cC = fEmbeddedCanvas3->GetCanvas();
    cC->SetFillColor(10);
    cC->cd();
    padC = new TPad("padC","Histogram",0.0,0.0,1.0,1.0,10,3,1);
    padC->SetFillColor(10);
    padC->SetBorderMode(0);
    padC->SetBorderSize(0);
    padC->Draw();
    // Creation of histogram for particle's energy loss
    fHisto_dEdX = new TH1F("Statistics","Energy loss for each particle",100,0,0.025); // Max at 25 MeV
    fHisto_dEdX->SetFillColor(38);
    fHisto_dEdX->SetStats(kTRUE);
    fHisto_dEdX->SetXTitle("Energy Loss [GeV]");
    fHisto_dEdX->SetLabelFont(42,"X");
    fHisto_dEdX->SetLabelSize(0.03f, "X");
    fHisto_dEdX->GetXaxis()->SetTitleFont(42);
    fHisto_dEdX->SetYTitle("Number");
    fHisto_dEdX->SetLabelFont(42,"Y");
    fHisto_dEdX->SetLabelSize(0.03f, "Y");
    fHisto_dEdX->GetYaxis()->SetTitleFont(42);

    cC->Update();

    // Create text display Tab
    tFrame = fDisplayFrame->AddTab("PDG Table");
    fTextView = new TGTextEdit(tFrame, 300, 100, kSunkenFrame | kDoubleBorder);
    tFrame->AddFrame(fTextView, fL5);
    TString pdgFilename = gSystem->Getenv("ROOTSYS");
    pdgFilename.Append("/etc/pdg_table.txt");

    fTextView->LoadFile(pdgFilename);

    //  fMainFrame->AddFrame(fDisplayFrame, fL4);
    fMainFrame->AddFrame(fDisplayFrame, fL7);

    // Add Main frame to this
    AddFrame(fMainFrame, fL3);

    // Create status bar
    Int_t parts[] = {45, 45, 10};
    fStatusBar = new TGStatusBar(this, 50, 10, kHorizontalFrame);
    fStatusBar->SetParts(parts, 3);
    AddFrame(fStatusBar, fL6);
    fStatusBar->SetText("Waiting to start simulation...",0);

    // Finish RootShower for display...
    SetWindowName("Root Shower Event Display");
    SetIconName("Root Shower Event Display");
    MapSubwindows();
    Resize(GetDefaultSize()); // this is used here to init layout algoritme
    MapWindow();
    this->Move(fgDefaultXPosition, fgDefaultYPosition);
    fEvent = new MyEvent();
    fEvent->Init(0, fFirstParticle, fE0, fB, fMaterial, fDimX, fDimY, fDimZ);
    Initialize(0);
    gROOT->GetListOfBrowsables()->Add(fEvent,"RootShower Event");
    gSystem->Load("libTreeViewer");
    gRootShower = this;
}


//______________________________________________________________________________
void RootShower::MakeMenuBarFrame()
{
    // Create menubar and popup menus.

    // layout hint items
    fMenuBarLayout = new TGLayoutHints(kLHintsTop| kLHintsLeft | kLHintsExpandX,
				     0, 0, 0, 0);
    fMenuBarItemLayout = new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0);
    fMenuBarHelpLayout = new TGLayoutHints(kLHintsTop | kLHintsRight);

    fMenuBar = new TGMenuBar(this, 1, 1, kHorizontalFrame | kRaisedFrame);

    // file popup menu
    fMenuFile = new TGPopupMenu(gClient->GetRoot());
    fMenuFile->AddEntry("&Open...", M_FILE_OPEN);
    fMenuFile->AddEntry("S&ave as...", M_FILE_SAVEAS);
    fMenuFile->AddEntry("&Close", -1);
    fMenuFile->AddSeparator();
    fMenuFile->AddEntry("&Print", -1);
    fMenuFile->AddEntry("P&rint setup...", -1);
    fMenuFile->AddSeparator();
    fMenuFile->AddEntry("E&xit", M_FILE_EXIT);
    fMenuFile->DisableEntry(M_FILE_SAVEAS);
    fMenuFile->Associate(this);

    fMenuTest = new TGPopupMenu(gClient->GetRoot());
    fMenuTest->AddLabel("Simulation Settings...");
    fMenuTest->AddSeparator();
    fMenuTest->AddEntry("&Settings...", M_SETTINGS_DLG);
    fMenuTest->AddEntry("Save &Parameters", M_SETTINGS_SAVE);
    fMenuTest->AddEntry("Show &Process", M_SHOW_PROCESS);
    fMenuTest->AddEntry("Animated &GIF", M_ANIMATE_GIF);
    fMenuTest->AddEntry("&Infos...", M_SHOW_INFOS);
    fMenuTest->AddSeparator();
    fMenuTest->AddEntry("&3D View", M_SHOW_3D);
    fMenuTest->AddEntry("&Show Selected  Track", M_SHOW_TRACK);
    fMenuTest->DisableEntry(M_SHOW_INFOS);
    fMenuTest->DisableEntry(M_SHOW_3D);
    fMenuTest->DisableEntry(M_SHOW_TRACK);

    fMenuTest->DisableEntry(M_SHOW_PROCESS);
    fMenuTest->DisableEntry(M_ANIMATE_GIF);

    fMenuTest->Associate(this);

    fMenuInspect = new TGPopupMenu(gClient->GetRoot());
    fMenuInspect->AddLabel("Simulation Tools...");
    fMenuInspect->AddSeparator();
    fMenuInspect->AddEntry("Start &Browser", M_INSPECT_BROWSER);
    fMenuInspect->AddEntry("&Create Html Doc", M_FILE_HTML);
    fMenuInspect->Associate(this);

    fMenuView = new TGPopupMenu(gClient->GetRoot());
    fMenuView->AddEntry("&Toolbar", M_VIEW_TOOLBAR);
    fMenuView->Associate(this);
    fMenuView->CheckEntry(M_VIEW_TOOLBAR);

    fMenuHelp = new TGPopupMenu(gClient->GetRoot());
    fMenuHelp->AddEntry("&Physics", M_HELP_PHYSICS);
    fMenuHelp->AddEntry("&Simulation", M_HELP_SIMULATION);
    fMenuHelp->AddSeparator();
    fMenuHelp->AddEntry("&License...", M_HELP_LICENSE);
    fMenuHelp->AddEntry("&About...", M_HELP_ABOUT);
    fMenuHelp->Associate(this);

    fMenuBar->AddPopup("&File", fMenuFile, fMenuBarItemLayout);
    fMenuBar->AddPopup("&Event", fMenuTest, fMenuBarItemLayout);
    fMenuBar->AddPopup("&Tools", fMenuInspect, fMenuBarItemLayout);
    fMenuBar->AddPopup("&View", fMenuView, fMenuBarItemLayout);
    fMenuBar->AddPopup("&Help", fMenuHelp, fMenuBarHelpLayout);

    AddFrame(fMenuBar, fMenuBarLayout);
}


//______________________________________________________________________________
void RootShower::CloseMenuBarFrame()
{
    // Destroy menubar and popup menus.
    delete fMenuHelp;
    delete fMenuTest;
    delete fMenuInspect;
    delete fMenuFile;

    delete fMenuBarItemLayout;
    delete fMenuBarHelpLayout;
    delete fMenuBar;
    delete fMenuBarLayout;
}

//______________________________________________________________________________
void RootShower::ShowToolBar(Bool_t show)
{
   // Show or hide toolbar.

   if (show) {
      ShowFrame(fToolBar);
      fMenuView->CheckEntry(M_VIEW_TOOLBAR);
   } else {
      HideFrame(fToolBar);
      fMenuView->UnCheckEntry(M_VIEW_TOOLBAR);
   }
}

//______________________________________________________________________________
RootShower::~RootShower()
{
    // Destroy RootShower object. Delete all created widgets
    // GUI MEMBERS
    CloseMenuBarFrame();

    delete fZoomPlusButton2;
    delete fZoomMoinsButton2;
    delete fZoomPlusButton;
    delete fZoomMoinsButton;
    delete fHFrame;
    delete fHFrame2;
    delete fZoomButtonsLayout;

    delete fEmbeddedCanvas;
    delete fTextView;
    delete fDisplayFrame;
    delete fEventListTree;
    delete fTreeView;
    delete fButtonFrame;
    delete fSelectionFrame;
    delete fMainFrame;
    delete fTitleFrame;

    delete fL8;
    delete fL7;
    delete fL6;
    delete fL5;
    delete fL4;
    delete fL3;
    delete fL2;
    delete fL1;
}

//______________________________________________________________________________
void RootShower::setDefaultPosition(Int_t x, Int_t y)
{
    // Set the default position on the screen of new RootShower instances.
    fgDefaultXPosition = x;
    fgDefaultYPosition = y;
}

//______________________________________________________________________________
void RootShower::Layout()
{
    //
    //  Resize(GetDefaultSize());
    TGMainFrame::Layout();
}


//______________________________________________________________________________
void RootShower::CloseWindow()
{
    // Got close message for this RootShower. The EventDislay and the
    // application will be terminated.

    if(fModified) {
        new RootShowerMsgBox(gClient->GetRoot(),this, 400, 200);
        if ( fOk ) {
            fRootShowerEnv->SetValue("RootShower.fFirstParticle",fFirstParticle);
            fRootShowerEnv->SetValue("RootShower.fE0",fE0);
            fRootShowerEnv->SetValue("RootShower.fB",fB);
            fRootShowerEnv->SetValue("RootShower.fMaterial",fMaterial);
            fRootShowerEnv->SetValue("RootShower.fDimX",fDimX);
            fRootShowerEnv->SetValue("RootShower.fDimY",fDimY);
            fRootShowerEnv->SetValue("RootShower.fDimZ",fDimZ);
            fRootShowerEnv->SaveLevel(kEnvLocal);
            cout << " Saving stuff .... " << endl;
#ifdef R__WIN32
            gSystem->Exec("del .rootshowerrc");
            gSystem->Rename(".rootshowerrc.new",".rootshowerrc");
#endif
        }
    }
    cout << "Terminating RootShower" << endl;
    gApplication->Terminate(0);
}

//______________________________________________________________________________
Bool_t RootShower::HandleConfigureNotify(Event_t *event)
{
    // This event is generated when the frame is resized.
    TGFrame* f = (TGFrame*)this;
    if ((event->fWidth != f->GetWidth()) || (event->fHeight != f->GetHeight())) {
        UInt_t w = event->fWidth;
        UInt_t h = event->fHeight;
        f->Resize(w,h);
        f->Layout();
   }
   return kTRUE;
}

//______________________________________________________________________________
Bool_t RootShower::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
    // Handle messages send to the RootShower object.
    Window_t wdummy;
    int ax, ay;
    TRootHelpDialog *hd;
    Char_t  strtmp[80];

    switch (GET_MSG(msg)) {

    case kC_COMMAND:

        switch (GET_SUBMSG(msg)) {

            case kCM_BUTTON:
            case kCM_MENU:
                switch(parm1) {

                    case M_EVENT_NEXT:
                        Initialize(0);
                        fStatusBar->SetText("Simulation running, please wait...",0);
	                    fButtonFrame->SetState(GButtonFrame::kNoneActive);
                        OnShowerProduce();
                        fClient->NeedRedraw(fEventListTree);
	                    fButtonFrame->SetState(GButtonFrame::kAllActive);
                        sprintf(strtmp,"Done - Total particles : %d - Waiting for next simulation",
                            fEvent->GetTotal());
                        fStatusBar->SetText(strtmp,0);
	                    break;
                    case M_EVENT_SELECT:
                        {
                            TGListTreeItem *item;
	                        if ((item = fEventListTree->GetSelected()) != 0)
                            OnShowSelected(item);
                        }
	                    break;
                    case M_INTERRUPT_SIMUL:
                            Interrupt();
	                    break;

                    case M_ZOOM_PLUS:
                            cA->cd();
                            cA->GetView()->ZoomView(0, 1.25);
                            cA->Modified();
                            cA->Update();
	                    break;

                    case M_ZOOM_MOINS:
                            cA->cd();
                            cA->GetView()->UnzoomView(0, 1.25);
                            cA->Modified();
                            cA->Update();
	                    break;

                    case M_ZOOM_PLUS2:
                            cB->cd();
                            cB->GetView()->ZoomView(0, 1.25);
                            cB->Modified();
                            cB->Update();
	                    break;

                    case M_ZOOM_MOINS2:
                            cB->cd();
                            cB->GetView()->UnzoomView(0, 1.25);
                            cB->Modified();
                            cB->Update();
	                    break;

                    case M_FILE_OPEN:
                        {
                            TGFileInfo fi;
                            fi.fFileTypes = filetypes;
                            new TGFileDialog(fClient->GetRoot(), this, kFDOpen,&fi);
                            if (!fi.fFilename) return kTRUE;
                               OnOpenFile(fi.fFilename);
                        }
                        break;

                    case M_FILE_HTML:
                         {
                             THtml html;
                             html.SetSourceDir(gProgPath);
                             html.MakeClass("MyParticle");
                             html.MakeClass("MyDetector");
                             html.MakeClass("EventHeader");
                             html.MakeClass("MyEvent");
                             html.MakeIndex();
                         }
                        break;

                    case M_FILE_SAVEAS:
                         {
                            TGFileInfo fi;
                            fi.fFileTypes = filetypes;
                            new TGFileDialog(fClient->GetRoot(), this, kFDSave,&fi);
                            if (!fi.fFilename) return kTRUE;
                               OnSaveFile(fi.fFilename);
                         }
                        break;

                    case M_FILE_EXIT:
                        CloseWindow();   // this also terminates theApp
                        break;

                    case M_SHOW_PROCESS:
                        if(fShowProcess) {
                            fMenuTest->UnCheckEntry(M_SHOW_PROCESS);
                            fShowProcess = false;
                        }
                        else {
                            fMenuTest->CheckEntry(M_SHOW_PROCESS);
                            fShowProcess = true;
                        }
                        break;

                    case M_ANIMATE_GIF:
                        if(fCreateGIFs) {
                            fMenuTest->UnCheckEntry(M_ANIMATE_GIF);
                            fCreateGIFs = false;
                        }
                        else {
                            fMenuTest->CheckEntry(M_ANIMATE_GIF);
                            fCreateGIFs = true;
                        }
                        break;

                    case M_SETTINGS_DLG:
                        new SettingsDialog(fClient->GetRoot(), this, 400, 200);
                        if(fSettingsModified) {
                            fEvent->Init(0, fFirstParticle, fE0, fB, fMaterial, fDimX, fDimY, fDimZ);
                            Initialize(0);
                            gRootShower->Modified();
                            gRootShower->SettingsModified(kFALSE);
                        }
                        break;

                    case M_SETTINGS_SAVE:
                        fRootShowerEnv->SetValue("RootShower.fFirstParticle",fFirstParticle);
                        fRootShowerEnv->SetValue("RootShower.fE0",fE0);
                        fRootShowerEnv->SetValue("RootShower.fB",fB);
                        fRootShowerEnv->SetValue("RootShower.fMaterial",fMaterial);
                        fRootShowerEnv->SetValue("RootShower.fDimX",fDimX);
                        fRootShowerEnv->SetValue("RootShower.fDimY",fDimY);
                        fRootShowerEnv->SetValue("RootShower.fDimZ",fDimZ);
                        fRootShowerEnv->SaveLevel(kEnvLocal);
#ifdef R__WIN32
                        gSystem->Exec("del .rootshowerrc");
                        gSystem->Rename(".rootshowerrc.new",".rootshowerrc");
#endif
                        gRootShower->Modified(kFALSE);
                        break;

                    case M_SHOW_INFOS:
                        ShowInfos();
                        break;

                    case M_INSPECT_BROWSER:
                        new TBrowser;
                        break;

                    case M_VIEW_TOOLBAR:
                        if (fMenuView->IsEntryChecked(M_VIEW_TOOLBAR))
                            ShowToolBar(kFALSE);
                        else
                            ShowToolBar();
                        break;

                    case M_HELP_PHYSICS:
                        {
                            Char_t str[32];
                            sprintf(str, "Help on Physics");
                            hd = new TRootHelpDialog(this, str, 620, 350);
                            hd->SetText(physics_txt);
                            gVirtualX->TranslateCoordinates( GetId(), GetParent()->GetId(),
                                        (Int_t)(GetWidth() - 620) >> 1,
                                        (Int_t)(GetHeight() - 350) >> 1,
                                        ax, ay, wdummy);
                            hd->Move(ax, ay);
                            hd->Popup();
                        }
                        break;

                    case M_HELP_SIMULATION:
                        {
                            Char_t str[32];
                            sprintf(str, "Help on Simulation");
                            hd = new TRootHelpDialog(this, str, 620, 350);
                            hd->SetText(simulation_txt);
                            gVirtualX->TranslateCoordinates( GetId(), GetParent()->GetId(),
                                        (Int_t)(GetWidth() - 620) >> 1,
                                        (Int_t)(GetHeight() - 350) >> 1,
                                        ax, ay, wdummy);
                            hd->Move(ax, ay);
                            hd->Popup();
                        }
                        break;

                    case M_HELP_LICENSE:
                        {
                            char str[32];
                            sprintf(str, "RootShower License");
                            hd = new TRootHelpDialog(this, str, 640, 380);
                            hd->SetText(gHelpLicense);
                            gVirtualX->TranslateCoordinates( GetId(), GetParent()->GetId(),
                                        (Int_t)(GetWidth() - 640) >> 1,
                                        (Int_t)(GetHeight() - 380) >> 1,
                                        ax, ay, wdummy);
                            hd->Move(ax, ay);
                            hd->Popup();
                        }
                        break;

                    case M_HELP_ABOUT:
                        new RootShowerAbout(gClient->GetRoot(),this, 400, 200);
                        break;

                    case M_SHOW_3D:
                        cA->x3d("OpenGL");
                        break;

                    case M_SHOW_TRACK:
                        {
                            TGListTreeItem *item;
	                        if ((item = fEventListTree->GetSelected()) != 0)
                            OnShowSelected(item);
                        }
                        break;

	
                } // switch parm1
                break; // M_MENU

            } // switch submsg
            break; // case kC_COMMAND

        case kC_LISTTREE:

            switch (GET_SUBMSG(msg)) {
     
                case kCT_ITEMDBLCLICK:
                    if (parm1 == kButton1) {
                        TGListTreeItem *item;
	                    if ((item = fEventListTree->GetSelected()) != 0) {
	                        fClient->NeedRedraw(fEventListTree);
	                    }
                    }
                    break;

            } // switch submsg      
            break; // case kC_LISTTREE
    } // switch msg
  
  return kTRUE;
}


//______________________________________________________________________________
TGListTreeItem* RootShower::AddToTree(const Text_t *name)
{
    // Add item to the TGListTree of the event display. It will be connected
    // to the current TGListTreeItem (i.e. fCurEventListItem)
    TGListTreeItem *e = 0;
    e = fEventListTree->AddItem(fCurListItem, name);
    return e;
}

//______________________________________________________________________________
void RootShower::BuildEventTree()
{
    // add recursively stations and layers (and cells) in TGListTree
    fCurListItem = 0;
    TGListTreeItem *eventLTItem = AddToTree("Event");
    fCurListItem = eventLTItem;
    gBaseLTI = eventLTItem;
}

//______________________________________________________________________________
void RootShower::Initialize(Int_t set_angles)
{
    Interrupt(kFALSE);
    fEventListTree->DeleteChildren(fCurListItem);
    fClient->NeedRedraw(fEventListTree);

    fMaxV = TMath::Max(fDimX,TMath::Max(fDimY,fDimZ));
    fMaxV /= 3.0;
    fMinV = -1.0 * fMaxV;
    cB->cd();
    cB->SetFillColor(1);
    cB->Clear();
    fSelection->cd();
    delete sel_node;
    delete sel_detect;
    sel_detect = new TBRIK("sel_detect","sel_detect","void",fDimX/2.0,fDimY/2.0,fDimZ/2.0);
    sel_detect->SetLineColor(7);
    sel_node = new TNode("SEL_NODE","SEL_NODE","sel_detect");
    sel_node->cd();
    fSelection->Draw();
    cB->GetView()->SetRange(fMinV,fMinV,fMinV,fMaxV,fMaxV,fMaxV);
    cB->GetView()->SetPerspective();
    cB->GetView()->SideView();
    cB->cd();
    cB->Update();

    cA->cd();
    cA->SetFillColor(1);
    cA->Clear();
    fEvent->GetDetector()->GetGeometry()->Draw();
    cA->GetView()->SetRange(fMinV,fMinV,fMinV,fMaxV,fMaxV,fMaxV);
    cA->GetView()->SetPerspective();
    cA->GetView()->SideView();
    cA->cd();
    cA->Update();
    fStatusBar->SetText("",1);
}

//______________________________________________________________________________
void RootShower::produce()
{
    Int_t     j,local_num,local_last,local_end;
    Int_t     old_num;
    Char_t    strtmp[80];

    // Check if some Event parameters have changed
    if((fEvent->GetHeader()->GetDate() != fEventTime) ||
       (fEvent->GetDetector()->GetMaterial() != fMaterial) ||
       (fEvent->GetHeader()->GetPrimary() != fFirstParticle) ||
       (fEvent->GetHeader()->GetEnergy() != fE0) ||
       (fEvent->GetB() != fB)) {
        fEventNr++;
        fNRun = 0;
    }
    fEvent->SetHeader(fEventNr, fNRun++, fEventTime, fFirstParticle, fE0);
    fEvent->Init(0, fFirstParticle, fE0, fB, fMaterial, fDimX, fDimY, fDimZ);

    fEvent->GetDetector()->GetGeometry()->cd();
    Interrupt(kFALSE);
    old_num = -1;
    j = 1;
    // loop events until user interrupt or until all particles are dead
    while((!IsInterrupted()) && (fEvent->GetNAlives() > 0)) {
        gSystem->ProcessEvents();  // handle GUI events
        if(fEvent->GetTotal() > old_num) {
            sprintf(strtmp,"Simulation running, particles : %4d, please wait...",fEvent->GetTotal());
            old_num = fEvent->GetTotal();
            fStatusBar->SetText(strtmp,0);
            // Update display here to not slow down too much...
        }
        local_last = fEvent->GetLast();
        local_num = 0;
        local_end = kFALSE;
        while((!IsInterrupted()) && (local_end == kFALSE) && (local_num < (local_last + 1))) {
            gSystem->ProcessEvents();  // handle GUI events
            // Update display here if fast machine...
            if(fEvent->GetParticle(local_num)->GetStatus() != DEAD) {
                if(fEvent->Action(local_num) == DEAD)
                    local_end = kTRUE;
                if(fEvent->GetParticle(local_num)->GetStatus() == CREATED)
                    fEvent->GetParticle(local_num)->SetStatus(ALIVE);
            }
            local_num ++;
        }
    }
    fMenuTest->EnableEntry(M_SHOW_INFOS);
    fMenuTest->EnableEntry(M_SHOW_3D);
    fMenuFile->EnableEntry(M_FILE_SAVEAS);
}

//______________________________________________________________________________
void RootShower::OnShowerProduce()
{
    Int_t     i,gifindex;
    Char_t    gifname[80];
    fStatusBar->SetText("",1);

    // animation logo handling
    if(fPicReset > 0) fPicIndex = 1;
    // animation timer
    if (!fTimer) fTimer = new TTimer(this, fPicDelay);
    fTimer->Reset();
    fTimer->TurnOn();

    fHisto_dEdX->Reset();
    produce();
    Interrupt(kFALSE);
    gifindex = 0;
    for(i=0;i<=fEvent->GetTotal();i++) {
        gSystem->ProcessEvents();  // handle GUI events
        if(IsInterrupted()) break;
        // if particle has no child, represent it by a leaf,
        // otherwise by a branch
        if(fEvent->GetParticle(i)->GetChildId(0) == 0) {
            lpic = gClient->GetPicture("leaf_t.xpm");
            lspic = gClient->GetPicture("leaf_t.xpm");
            gLTI[i]->SetPictures(lpic, lspic);
        }
        else {
            bpic = gClient->GetPicture("branch_t.xpm");
            bspic = gClient->GetPicture("branch_t.xpm");
            gLTI[i]->SetPictures(bpic, bspic);
        }
        // Show only charged and massive particles...
        if((fEvent->GetParticle(i)->GetPdgCode() != PHOTON) &&
           (fEvent->GetParticle(i)->GetPdgCode() != NEUTRINO_E) &&
           (fEvent->GetParticle(i)->GetPdgCode() != NEUTRINO_MUON) &&
           (fEvent->GetParticle(i)->GetPdgCode() != NEUTRINO_TAU) &&
           (fEvent->GetParticle(i)->GetPdgCode() != ANTINEUTRINO_E) &&
           (fEvent->GetParticle(i)->GetPdgCode() != ANTINEUTRINO_MUON) &&
           (fEvent->GetParticle(i)->GetPdgCode() != ANTINEUTRINO_TAU) ) {
            // Fill histogram for particle's energy loss
            fHisto_dEdX->Fill(fEvent->GetParticle(i)->GetELoss());
            fEvent->GetTrack(i)->Draw();
            // show track by track if "show process" has been choosen
            // into the menu
            if(fShowProcess) {
                cA->Modified();
                cA->Update();
                // create one gif image by step if "Animated GIF"
                // has been choosen into the menu
                if(fCreateGIFs) {
                    sprintf(gifname,"event_%04d.gif",gifindex);
                    cA->SaveAs(gifname);
                    gifindex++;
                }
            }
        }
    }
    AppendPad();
    cA->cd();
    cA->Modified();
    cA->Update();
    padC->cd();
//    fHisto_dEdX->Fit("gaus","W");
//    fHisto_dEdX->GetFunction("gaus")->SetLineColor(kRed);
//    fHisto_dEdX->GetFunction("gaus")->SetLineWidth(1);
    fHisto_dEdX->Fit("landau","L");
    fHisto_dEdX->GetFunction("landau")->SetLineColor(kRed);
    fHisto_dEdX->GetFunction("landau")->SetLineWidth(1);
    fHisto_dEdX->Draw();    
    padC->Modified();
    padC->Update();
    cC->Update();
    padC->cd();
    padC->SetFillColor(16);
    padC->GetFrame()->SetFillColor(10);
    padC->Draw();
    padC->Update();

    // Open first list tree items
    gEventListTree->OpenItem(gBaseLTI);
    gEventListTree->OpenItem(gLTI[0]);
    fTimer->TurnOff();
    if(fPicReset > 0)
        fTitleFrame->ChangeRightLogo(1);
}

//______________________________________________________________________________
void RootShower::HighLight(TGListTreeItem *item)
{
}

//______________________________________________________________________________
void RootShower::OnShowSelected(TGListTreeItem *item)
{
    // Shows track which has been selected into the list tree
    Int_t i,retval;

    cB->cd();
    cB->SetFillColor(1);
    cB->SetBorderMode(0);
    cB->SetBorderSize(0);
    cB->Clear();
    cB->cd();
    // draw geometry
    fSelection->Draw();
    cB->GetView()->SetRange(fMinV,fMinV,fMinV,fMaxV,fMaxV,fMaxV);
    cB->GetView()->SetPerspective();
    cB->GetView()->SideView();
    cB->cd();
    cB->Update();
    fSelection->cd();
    retval = -1;
    for(i=0;i<=fEvent->GetTotal();i++) {
        if(gLTI[i] == item) {
            retval = i;
            break;
        }
    }
    if((retval > -1) &&
       (fEvent->GetParticle(i)->GetPdgCode() != PHOTON) &&
       (fEvent->GetParticle(i)->GetPdgCode() != NEUTRINO_E) &&
       (fEvent->GetParticle(i)->GetPdgCode() != NEUTRINO_MUON) &&
       (fEvent->GetParticle(i)->GetPdgCode() != NEUTRINO_TAU) &&
       (fEvent->GetParticle(i)->GetPdgCode() != ANTINEUTRINO_E) &&
       (fEvent->GetParticle(i)->GetPdgCode() != ANTINEUTRINO_MUON) &&
       (fEvent->GetParticle(i)->GetPdgCode() != ANTINEUTRINO_TAU) ) {
        fEvent->GetTrack(retval)->Draw();
    }
    cB->cd();
    cB->Modified();
    cB->Update();
}

//______________________________________________________________________________
void RootShower::OnOpenFile(const Char_t *filename)
{
    // Opens a root file into which a previous event
    // has been saved.
    Int_t  entries = 0;
    Char_t   strtmp[80];
    Int_t  i;
    TFile *f = new TFile(filename);
    TTree *tree;
    TBranch *branch;
    fStatusBar->SetText("",1);

    fEvent->Init(0, fFirstParticle, fE0, fB, fMaterial, fDimX, fDimY, fDimZ);
    fHisto_dEdX->Reset();
    tree = (TTree *)f->Get("RootShower");
    if(tree == NULL) return;

    branch = tree->GetBranch("Event");
    branch->SetAddress(&fEvent);
    entries = (Int_t)tree->GetEntries();
    tree->GetEntry(0, 1);
    f->Close();
    // take back detector dimensions for selection geometry
    fEvent->GetDetector()->GetDimensions(&fDimX, &fDimY, &fDimZ);
    Initialize(0);

    for(i=0;i<=fEvent->GetTotal();i++) {
        gTmpLTI = gEventListTree->AddItem(gBaseLTI, fEvent->GetParticle(i)->GetName());
        sprintf(strtmp,"%1.2f GeV",fEvent->GetParticle(i)->Energy());
        gEventListTree->SetToolTipItem(gTmpLTI, strtmp);
        gLTI[i] = gTmpLTI;

        if(fEvent->GetParticle(i)->GetChildId(0) == 0) {
            lpic = gClient->GetPicture("leaf_t.xpm");
            lspic = gClient->GetPicture("leaf_t.xpm");
            gLTI[i]->SetPictures(lpic, lspic);
        }
        else {
            bpic = gClient->GetPicture("branch_t.xpm");
            bspic = gClient->GetPicture("branch_t.xpm");
            gLTI[i]->SetPictures(bpic, bspic);
        }

        if((fEvent->GetParticle(i)->GetPdgCode() != PHOTON) &&
           (fEvent->GetParticle(i)->GetPdgCode() != NEUTRINO_E) &&
           (fEvent->GetParticle(i)->GetPdgCode() != NEUTRINO_MUON) &&
           (fEvent->GetParticle(i)->GetPdgCode() != NEUTRINO_TAU) &&
           (fEvent->GetParticle(i)->GetPdgCode() != ANTINEUTRINO_E) &&
           (fEvent->GetParticle(i)->GetPdgCode() != ANTINEUTRINO_MUON) &&
           (fEvent->GetParticle(i)->GetPdgCode() != ANTINEUTRINO_TAU) ) {
            // Fill histogram for particle's energy loss
            fHisto_dEdX->Fill(fEvent->GetParticle(i)->GetELoss());
            fEvent->GetTrack(i)->Draw();
        }
    }
    // Reparent each list tree item regarding the
    // corresponding particle relations
    for(i=1;i<=fEvent->GetTotal();i++) {
        gEventListTree->Reparent(gLTI[i],
            gLTI[fEvent->GetParticle(i)->GetFirstMother()]);
    }
    fEventListTree->OpenItem(gBaseLTI);
    fEventListTree->OpenItem(gLTI[0]);
    fClient->NeedRedraw(fEventListTree);
    AppendPad();

    sprintf(strtmp,"Done - Total particles : %d - Waiting for next simulation",
                    fEvent->GetTotal());
    fStatusBar->SetText(strtmp,0);
    padC->cd();
    fHisto_dEdX->Fit("landau","L");
    fHisto_dEdX->GetFunction("landau")->SetLineColor(kRed);
    fHisto_dEdX->GetFunction("landau")->SetLineWidth(1);
    fHisto_dEdX->Draw();    
    padC->Modified();
    padC->Update();
    cC->Update();
    padC->cd();
    padC->SetFillColor(16);
    padC->GetFrame()->SetFillColor(10);
    padC->Draw();
    padC->Update();

    cA->cd();
    cA->Modified();
    cA->Update();
    fMenuTest->EnableEntry(M_SHOW_INFOS);
    fMenuTest->EnableEntry(M_SHOW_3D);
    fMenuFile->EnableEntry(M_FILE_SAVEAS);
    fButtonFrame->SetState(GButtonFrame::kAllActive);
}

//______________________________________________________________________________
void RootShower::OnSaveFile(const Char_t *filename)
{
    // Saves current event into a Root file
    TFile *hfile;

    hfile = new TFile(filename,"RECREATE","Root Shower file");
    hfile->SetCompressionLevel(9);

    TTree *hTree = new TTree("RootShower","Root Shower tree");
    hTree->Branch("Event", "MyEvent", &fEvent, 8000, 2);
    hTree->Fill();  //fill the tree
    hTree->Write();
    hTree->Print();
    hfile->Close();
}

//______________________________________________________________________________
void RootShower::ShowInfos()
{
    // Gives infos on current event
    Window_t wdummy;
    int ax, ay;
    TRootHelpDialog *hd;
    Char_t str[32];
    Char_t Msg[500];
    Double_t dimx,dimy,dimz;

    fEvent->GetDetector()->GetDimensions(&dimx, &dimy, &dimz);

    sprintf(Msg, "  Some information about the current shower\n");
    sprintf(Msg, "%s  Target material ...... : %s\n", Msg,
            fEvent->GetDetector()->GetMaterialName());
    sprintf(Msg, "%s  Dimensions of the target\n", Msg);
    sprintf(Msg, "%s  X .................... : %1.2e [cm]    \n", Msg, dimx);
    sprintf(Msg, "%s  Y .................... : %1.2e [cm]    \n", Msg, dimy);
    sprintf(Msg, "%s  Z .................... : %1.2e [cm]    \n", Msg, dimz);
    sprintf(Msg, "%s  Magnetic field ....... : %1.2e [kGauss]\n", Msg,
            fEvent->GetB()*17.4/Prop_B);
    sprintf(Msg, "%s  Initial particle ..... : %s \n", Msg,
            fEvent->GetParticle(0)->GetName());
    sprintf(Msg, "%s  Initial energy ....... : %1.2e [GeV] \n", Msg,
            fEvent->GetHeader()->GetEnergy());
    sprintf(Msg, "%s  Total Energy loss .... : %1.2e [GeV]", Msg,
            fEvent->GetDetector()->GetTotalELoss());

    sprintf(str, "Infos on current shower");
    hd = new TRootHelpDialog(this, str, 420, 155);
    hd->SetText(Msg);
    gVirtualX->TranslateCoordinates( GetId(), GetParent()->GetId(),
              (Int_t)(GetWidth() - 420) >> 1,(Int_t)(GetHeight() - 155) >> 1,
              ax, ay, wdummy);
    hd->Move(ax, ay);
    hd->Popup();
}

//______________________________________________________________________________
Bool_t RootShower::HandleTimer(TTimer *)
{
   // Logo animation timer handling.

   if(fPicIndex > fPicNumber) fPicIndex = 1;
   fTitleFrame->ChangeRightLogo(fPicIndex);
   fPicIndex++;
   fTimer->Reset();
   return kTRUE;
}

//______________________________________________________________________________
Int_t RootShower::DistancetoPrimitive(Int_t px, Int_t py)
{
    // Compute distance from point px,py to objects in event
    Int_t i;
    Int_t dist = 9999;

    if(fEvent->GetTotal() <= 0) return 0;
    // Browse every track and get related particle infos.
    for(i=0;i<fEvent->GetTotal();i++) {
        dist = fEvent->GetTrack(i)->DistancetoPrimitive(px, py);
        if (dist < 2) {
            gPad->SetSelected((TObject*)fEvent->GetParticle(i));
            fStatusBar->SetText(fEvent->GetParticle(i)->GetObjectInfo(px, py),1);
            gPad->SetCursor(kPointer);
            return 0;
        }
    }
    gPad->SetSelected((TObject*)gPad->GetView());
    return 0;
}

int main(int argc, char **argv)
{
    TApplication theApp("App", &argc, argv);

    gStyle->SetOptStat(1111);
    gStyle->SetOptFit(1111);
    gStyle->SetStatFont(42);

    gRandom->SetSeed( (UInt_t)time( NULL ) );
    const Int_t NRGBs = 5;
    Double_t Stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t Red[NRGBs] = { 1.00, 0.75, 0.50, 0.25, 0.00 };
    Double_t Green[NRGBs] = { 0.00, 0.00, 0.00, 0.00, 0.00 };
    Double_t Blue[NRGBs] = { 0.00, 0.25, 0.50, 0.75, 1.00 };
    gColIndex = gStyle->CreateGradientColorTable(NRGBs, Stops, Red, Green, Blue, 11);

    // Create RootShower
    RootShower *theShower = new RootShower(gClient->GetRoot(), 400, 200);

    // run ROOT application
    theApp.Run();

    delete theShower;
    return 0;
}

