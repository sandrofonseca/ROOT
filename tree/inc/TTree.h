// @(#)root/tree:$Name:  $:$Id: TTree.h,v 1.42 2002/08/17 21:38:19 brun Exp $
// Author: Rene Brun   12/01/96

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TTree
#define ROOT_TTree


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTree                                                                //
//                                                                      //
// A TTree object is a list of TBranch.                                 //
//   To Create a TTree object one must:                                 //
//    - Create the TTree header via the TTree constructor               //
//    - Call the TBranch constructor for every branch.                  //
//                                                                      //
//   To Fill this object, use member function Fill with no parameters.  //
//     The Fill function loops on all defined TBranch.                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TNamed
#include "TNamed.h"
#endif

#ifndef ROOT_TObjArray
#include "TObjArray.h"
#endif

#ifndef ROOT_TClonesArray
#include "TClonesArray.h"
#endif

#ifndef ROOT_TAttLine
#include "TAttLine.h"
#endif

#ifndef ROOT_TAttFill
#include "TAttFill.h"
#endif

#ifndef ROOT_TAttMarker
#include "TAttMarker.h"
#endif

#ifndef ROOT_TBranch
#include "TBranch.h"
#endif

#ifndef ROOT_TCut
#include "TCut.h"
#endif

#ifndef ROOT_TArrayD
#include "TArrayD.h"
#endif

#ifndef ROOT_TArrayI
#include "TArrayI.h"
#endif

#ifndef ROOT_TVirtualTreePlayer
#include "TVirtualTreePlayer.h"
#endif

class TBrowser;
class TFile;
class TDirectory;
class TLeaf;
class TH1;
class TTreeFormula;
class TPolyMarker;
class TEventList;
class TSQLResult;
class TSelector;
class TPrincipal;
class TFriendElement;

class TTree : public TNamed, public TAttLine, public TAttFill, public TAttMarker {

protected:
    Double_t      fEntries;           //  Number of entries
    Double_t      fTotBytes;          //  Total number of bytes in all branches before compression
    Double_t      fZipBytes;          //  Total number of bytes in all branches after compression
    Double_t      fSavedBytes;        //  Number of autosaved bytes
    Double_t      fWeight;            //  Tree weight (see TTree::SetWeight)
    Int_t         fTimerInterval;     //  Timer interval in milliseconds
    Int_t         fScanField;         //  Number of runs before prompting in Scan
    Int_t         fUpdate;            //  Update frequency for EntryLoop
    Int_t         fMaxEntryLoop;      //  Maximum number of entries to process
    Int_t         fMaxVirtualSize;    //  Maximum total size of buffers kept in memory
    Int_t         fAutoSave;          //  Autosave tree when fAutoSave bytes produced
    Int_t         fEstimate;          //  Number of entries to estimate histogram limits
    Int_t         fChainOffset;       //! Offset of 1st entry of this Tree in a TChain
    Int_t         fReadEntry;         //! Number of the entry being processed
    Int_t         fTotalBuffers;      //! Total number of bytes in branch buffers
    Int_t         fPacketSize;        //! Number of entries in one packet for parallel root
    Int_t         fNfill;             //! Local for EntryLoop
    Int_t         fDebug;             //! Debug level
    Int_t         fDebugMin;          //! First entry number to debug
    Int_t         fDebugMax;          //! Last entry number to debug
    Int_t         fMakeClass;         //! not zero when processing code generated by MakeClass
    Int_t         fFileNumber;        //! current file number (if file extensions)
    TObject      *fNotify;            //! Object to be notified when loading a Tree
    TDirectory   *fDirectory;         //! Pointer to directory holding this tree
    TObjArray     fBranches;          //  List of Branches
    TObjArray     fLeaves;            //  Direct pointers to individual branch leaves
    TEventList   *fEventList;         //! Pointer to event selection list (if one)
    TArrayD       fIndexValues;       //  Sorted index values
    TArrayI       fIndex;             //  Index of sorted values
    TList        *fFriends;           //  pointer to list of friend elements
    TVirtualTreePlayer *fPlayer;      //! Pointer to current Tree player
  static Int_t    fgBranchStyle;      //  Old/New branch style
  static Int_t    fgMaxTreeSize;      //  Maximum size of a file containg a Tree

protected:
    const   char    *GetNameByIndex(TString &varexp, Int_t *index,Int_t colindex) const;
    virtual void     MakeIndex(TString &varexp, Int_t *index);
    virtual TFile   *ChangeFile(TFile *file);
    
public:
    // TTree status bits
    enum {
       kForceRead   = BIT(11)
    };

    TTree();
    TTree(const char *name, const char *title, Int_t splitlevel=99);
    virtual ~TTree();

    virtual TFriendElement *AddFriend(const char *treename, const char *filename="");
    virtual TFriendElement *AddFriend(const char *treename, TFile *file);
    virtual TFriendElement *AddFriend(TTree *tree, const char* alias="", Bool_t warn = kFALSE);
    virtual void      AddTotBytes(Int_t tot) {fTotBytes += tot;}
    virtual void      AddZipBytes(Int_t zip) {fZipBytes += zip;}
    virtual void      AutoSave();
    virtual Int_t     Branch(TCollection *list, Int_t bufsize=32000, Int_t splitlevel=99, const char *name="");
    virtual Int_t     Branch(TList *list, Int_t bufsize=32000, Int_t splitlevel=99);
    virtual Int_t     Branch(const char *folder, Int_t bufsize=32000, Int_t splitlevel=99);
    virtual TBranch  *Branch(const char *name, void *address, const char *leaflist, Int_t bufsize=32000);
    virtual TBranch  *Branch(const char *name, void *clonesaddress, Int_t bufsize=32000, Int_t splitlevel=1);
    virtual TBranch  *Branch(const char *name, const char *classname, void *addobj, Int_t bufsize=32000, Int_t splitlevel=99);
    virtual TBranch  *Bronch(const char *name, const char *classname, void *addobj, Int_t bufsize=32000, Int_t splitlevel=99);
    virtual TBranch  *BranchOld(const char *name, const char *classname, void *addobj, Int_t bufsize=32000, Int_t splitlevel=1);
    virtual void      Browse(TBrowser *b);
    virtual void      BuildIndex(const char *majorname, const char *minorname);
    TStreamerInfo    *BuildStreamerInfo(TClass *cl, void *pointer=0);
    virtual TTree    *CloneTree(Int_t nentries=-1, Option_t *option="");
    virtual void      CopyAddresses(TTree *tree);
    virtual Int_t     CopyEntries(TTree *tree, Int_t nentries=-1);
    virtual TTree    *CopyTree(const char *selection, Option_t *option=""
                       ,Int_t nentries=1000000000, Int_t firstentry=0);
    Int_t             Debug() const {return fDebug;}
    virtual void      Delete(Option_t *option=""); // *MENU*
    virtual void      Draw(Option_t *opt);
    virtual Int_t     Draw(const char *varexp, TCut selection, Option_t *option=""
                       ,Int_t nentries=1000000000, Int_t firstentry=0);
    virtual Int_t     Draw(const char *varexp, const char *selection, Option_t *option=""
                       ,Int_t nentries=1000000000, Int_t firstentry=0); // *MENU*
    virtual void      DropBuffers(Int_t nbytes);
    virtual Int_t     Fill();
    virtual TBranch  *FindBranch(const char *name);
    virtual TLeaf    *FindLeaf(const char *name);
    virtual Int_t     Fit(const char *funcname ,const char *varexp, const char *selection="",Option_t *option="" ,Option_t *goption=""
                       ,Int_t nentries=1000000000, Int_t firstentry=0); // *MENU*

    virtual TBranch  *GetBranch(const char *name);
    virtual Bool_t    GetBranchStatus(const char *branchname) const;
    static  Int_t     GetBranchStyle();
    virtual Int_t     GetChainEntryNumber(Int_t entry) const {return entry;}
    virtual Int_t     GetChainOffset() const { return fChainOffset; }
    TFile            *GetCurrentFile() const;
            Int_t     GetDebugMax()  const {return fDebugMax;}
            Int_t     GetDebugMin()  const {return fDebugMin;}
    TDirectory       *GetDirectory() const {return fDirectory;}
    virtual Double_t  GetEntries() const   {return fEntries;}
    virtual Double_t  GetEntriesFast() const   {return fEntries;}
    virtual Double_t  GetEntriesFriend() const;
    virtual Int_t     GetEstimate() const { return fEstimate; }
    virtual Int_t     GetEntry(Int_t entry=0, Int_t getall=0);
            Int_t     GetEvent(Int_t entry=0, Int_t getall=0) {return GetEntry(entry,getall);}
    virtual Int_t     GetEntryWithIndex(Int_t major, Int_t minor);
    virtual Int_t     GetEntryNumberWithBestIndex(Int_t major, Int_t minor) const;
    virtual Int_t     GetEntryNumberWithIndex(Int_t major, Int_t minor) const;
    TEventList       *GetEventList() const {return fEventList;}
    virtual Int_t     GetEntryNumber(Int_t entry) const;
    virtual Int_t     GetFileNumber() const {return fFileNumber;}
    virtual const char *GetFriendAlias(TTree *) const;
    TH1              *GetHistogram() {return GetPlayer()->GetHistogram();}
    virtual Int_t    *GetIndex() {return &fIndex.fArray[0];}
    virtual Double_t *GetIndexValues() {return &fIndexValues.fArray[0];}
    virtual TIterator*GetIteratorOnAllLeaves(Bool_t dir = kIterForward);
    virtual TLeaf    *GetLeaf(const char *name);
    virtual TObjArray       *GetListOfBranches() {return &fBranches;}
    virtual TObjArray       *GetListOfLeaves()   {return &fLeaves;}
    virtual TList    *GetListOfFriends() const  {return fFriends;}
    virtual Int_t     GetMakeClass() const {return fMakeClass;}
    virtual Int_t     GetMaxEntryLoop() const {return fMaxEntryLoop;}
    virtual Double_t  GetMaximum(const char *columname);
    static  Int_t     GetMaxTreeSize();
    virtual Int_t     GetMaxVirtualSize() const {return fMaxVirtualSize;}
    virtual Double_t  GetMinimum(const char *columname);
    virtual Int_t     GetNbranches() {return fBranches.GetEntriesFast();}
    TObject          *GetNotify() const {return fNotify;}
    TVirtualTreePlayer  *GetPlayer();
    virtual Int_t     GetPacketSize() const {return fPacketSize;}
    virtual Int_t     GetReadEntry()  const {return fReadEntry;}
    virtual Int_t     GetReadEvent()  const {return fReadEntry;}
    virtual Int_t     GetScanField()  const {return fScanField;}
    TTreeFormula     *GetSelect()    {return GetPlayer()->GetSelect();}
    virtual Int_t     GetSelectedRows() {return GetPlayer()->GetSelectedRows();}
    virtual Int_t     GetTimerInterval() const {return fTimerInterval;}
    virtual Double_t  GetTotBytes() const {return fTotBytes;}
    virtual TTree    *GetTree() const {return (TTree*)this;}
    virtual Int_t     GetTreeNumber() const {return 0;}
    virtual Int_t     GetUpdate() const {return fUpdate;}
    TTreeFormula     *GetVar1() {return GetPlayer()->GetVar1();}
    TTreeFormula     *GetVar2() {return GetPlayer()->GetVar2();}
    TTreeFormula     *GetVar3() {return GetPlayer()->GetVar3();}
    TTreeFormula     *GetVar4() {return GetPlayer()->GetVar4();}
    virtual Double_t *GetV1()   {return GetPlayer()->GetV1();}
    virtual Double_t *GetV2()   {return GetPlayer()->GetV2();}
    virtual Double_t *GetV3()   {return GetPlayer()->GetV3();}
    virtual Double_t *GetW()    {return GetPlayer()->GetW();}
    virtual Double_t  GetWeight() const   {return fWeight;}
    virtual Stat_t    GetZipBytes() const {return fZipBytes;}
    virtual void      IncrementTotalBuffers(Int_t nbytes) {fTotalBuffers += nbytes;}
    Bool_t            IsFolder() const {return kTRUE;}
    virtual Int_t     LoadTree(Int_t entry);
    virtual Int_t     MakeClass(const char *classname=0,Option_t *option="");
    virtual Int_t     MakeCode(const char *filename=0);
    virtual Int_t     MakeSelector(const char *selector=0);
    Bool_t            MemoryFull(Int_t nbytes);
    virtual Bool_t    Notify();
    TPrincipal       *Principal(const char *varexp="", const char *selection="", Option_t *option="np"
                       ,Int_t nentries=1000000000, Int_t firstentry=0);
    virtual void      Print(Option_t *option="") const; // *MENU*
    virtual Int_t     Process(const char *filename,Option_t *option="", Int_t nentries=1000000000, Int_t firstentry=0); // *MENU*
    virtual Int_t     Process(TSelector *selector, Option_t *option="", Int_t nentries=1000000000, Int_t firstentry=0);
    virtual Int_t     Project(const char *hname, const char *varexp, const char *selection="", Option_t *option=""
                       ,Int_t nentries=1000000000, Int_t firstentry=0);
    virtual TSQLResult  *Query(const char *varexp="", const char *selection="", Option_t *option=""
                          ,Int_t nentries=1000000000, Int_t firstentry=0);
    virtual void      RemoveFriend(TTree*);
    virtual void      Reset(Option_t *option="");
    virtual Int_t     Scan(const char *varexp="", const char *selection="", Option_t *option=""
                       ,Int_t nentries=1000000000, Int_t firstentry=0); // *MENU*
    virtual void      SetAutoSave(Int_t autos=10000000) {fAutoSave=autos;}
    virtual void      SetBasketSize(const char *bname,Int_t buffsize=16000);
    virtual void      SetBranchAddress(const char *bname,void *add);
    virtual void      SetBranchStatus(const char *bname,Bool_t status=1);
    static  void      SetBranchStyle(Int_t style=1);  //style=0 for old branch, =1 for new branch style
    virtual void      SetChainOffset(Int_t offset=0) {fChainOffset=offset;}
    virtual void      SetDebug(Int_t level=1, Int_t min=0, Int_t max=9999999); // *MENU*
    virtual void      SetDirectory(TDirectory *dir);
    virtual void      SetEstimate(Int_t nentries=10000);
    virtual void      SetEventList(TEventList *list) {fEventList = list;}
    virtual void      SetMakeClass(Int_t make) {fMakeClass = make;}
    virtual void      SetMaxEntryLoop(Int_t maxev=1000000000) {fMaxEntryLoop = maxev;} // *MENU*
    static  void      SetMaxTreeSize(Int_t maxsize=1900000000);
    virtual void      SetMaxVirtualSize(Int_t size=0) {fMaxVirtualSize = size;} // *MENU*
    virtual void      SetName(const char *name); // *MENU*
    virtual void      SetNotify(TObject *obj) {fNotify = obj;}
    virtual void      SetObject(const char *name, const char *title);
    virtual void      SetScanField(Int_t n=50) {fScanField = n;} // *MENU*
    virtual void      SetTimerInterval(Int_t msec=333) {fTimerInterval=msec;}
    virtual void      SetWeight(Double_t w=1, Option_t *option="");
    virtual void      SetUpdate(Int_t freq=0) {fUpdate = freq;}
    virtual void      Show(Int_t entry=-1);
    virtual void      StartViewer(); // *MENU*
    virtual Int_t     UnbinnedFit(const char *funcname ,const char *varexp, const char *selection="",Option_t *option=""
                       ,Int_t nentries=1000000000, Int_t firstentry=0);
    void              UseCurrentStyle();

    ClassDef(TTree,9)  //Tree descriptor (the main ROOT I/O class)
};

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTreeFriendLeafIter                                                  //
//                                                                      //
// Iterator on all the leaves in a TTree and its friend                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class TTreeFriendLeafIter : public TIterator {

protected:
   TTree             *fTree;         //tree being iterated
   TIterator         *fLeafIter;     //current leaf sub-iterator.
   TIterator         *fTreeIter;     //current tree sub-iterator.
   Bool_t             fDirection;    //iteration direction

   TTreeFriendLeafIter() : fTree(0), fLeafIter(0), fTreeIter(0),
       fDirection(0) {}

public:
   TTreeFriendLeafIter(const TTree *t, Bool_t dir = kIterForward);
   TTreeFriendLeafIter(const TTreeFriendLeafIter &iter);
   ~TTreeFriendLeafIter() { SafeDelete(fLeafIter); SafeDelete(fTreeIter); }
   TIterator &operator=(const TIterator &rhs);
   TTreeFriendLeafIter &operator=(const TTreeFriendLeafIter &rhs);

   const TCollection *GetCollection() const { return 0; }
   Option_t          *GetOption() const;
   TObject           *Next();
   void               Reset() { SafeDelete(fLeafIter); SafeDelete(fTreeIter); }

   ClassDef(TTreeFriendLeafIter,0)  //Linked list iterator
};


inline void TTree::Draw(Option_t *opt)
{ Draw(opt, "", "", 1000000000, 0); }

#endif
