// @(#)root/gui:$Id$
// Author: Bertrand Bellenot   26/09/2007

/*************************************************************************
 * Copyright (C) 1995-2007, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TGFileBrowser
#define ROOT_TGFileBrowser

#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif

#ifndef ROOT_TBrowserImp
#include "TBrowserImp.h"
#endif

class TGCanvas;
class TGListTree;
class TGListTreeItem;
class TGPicture;
class TGComboBox;
class TContextMenu;
class TRegexp;
class TString;
class TRootBrowser;
class TSystemDirectory;
class TSystemFile;

class TGFileBrowser : public TGMainFrame, public TBrowserImp {

protected:
   TRootBrowser      *fNewBrowser;        // Pointer back to the Browser
   TGHorizontalFrame *fTopFrame;          // Top horizontal frame
   TGHorizontalFrame *fBotFrame;          // Bottom horizontal frame
   TGCanvas          *fCanvas;            // Canvas for the list tree
   TGListTree        *fListTree;          // Main list tree
   TGListTreeItem    *fListLevel;         // Current list tree level
   TGListTreeItem    *fCurrentDir;        // Current (list tree) directory
   TGListTreeItem    *fRootDir;           // Root (list tree) directory
   TGComboBox        *fDrawOption;        // Draw options combobox
   TGComboBox        *fFileType;          // File type combobox
   TContextMenu      *fContextMenu;       // pointer to context menu
   const TGPicture   *fRootIcon;          // Root files icon
   const TGPicture   *fFileIcon;          // System files icon
   const TGPicture   *fCachedPic;         // Cached picture
   TString            fCachedPicName;     // Cached picture name
   TRegexp           *fFilter;            // Regular expression used to filter files
   TSystemDirectory  *fDir;               // Actual (selected) system directory
   TSystemFile       *fFile;              // Actual (selected) system file
   Int_t              fGroupSize;         // total number of items when icon box switched to "global view" mode
   Long_t             fNKeys, fCnt;       // Counters for keys inside a Root file
   Bool_t             fGrouped;           // kTRUE if Root file content (keys) is grouped
   Bool_t             fShowHidden;        // kTRUE to display hidden files

   void CreateBrowser();

public:
   TGFileBrowser(const TGWindow *p, TBrowser* b=0, UInt_t w=200, UInt_t h=400);
   virtual ~TGFileBrowser();

   virtual void Add(TObject *obj, const char *name = 0, Int_t check = -1);
   virtual void BrowseObj(TObject *obj);
   virtual void RecursiveRemove(TObject *obj);
   virtual void Refresh(Bool_t force = kFALSE);
   virtual void Show() { MapRaised(); }
   Option_t    *GetDrawOption() const;

   TRootBrowser *GetNewBrowser() const          { return fNewBrowser; }
   void          SetNewBrowser(TRootBrowser* b) { fNewBrowser = b;    }

   void        AddFSDirectory(const char* entry, const char* path=0, Option_t *opt="");
   void        AddKey(TGListTreeItem *itm, TObject *obj, const char *name = 0);
   void        AddRemoteFile(TObject *obj);
   void        ApplyFilter(Int_t id);
   void        Chdir(TGListTreeItem *item);
   void        Checked(TObject *obj, Bool_t check);
   void        CheckRemote(TGListTreeItem *item);
   void        Clicked(TGListTreeItem *item, Int_t btn, Int_t x, Int_t y);
   TString     DirName(TGListTreeItem* item);
   TString     FullPathName(TGListTreeItem* item);
   void        DoubleClicked(TGListTreeItem *item, Int_t btn);
   Long_t      XXExecuteDefaultAction(TObject *obj);
   char       *FormatFileInfo(const char *fname, Long64_t size, Long_t modtime);
   void        GetFilePictures(const TGPicture **pic, Int_t file_type, Bool_t is_link, const char *name);
   void        GetObjPicture(const TGPicture **pic, TObject *obj);
   void        GotoDir(const char *path);
   void        Selected(char *);
   void        Update();

   ClassDef(TGFileBrowser, 0) // File browser.
};

#endif
