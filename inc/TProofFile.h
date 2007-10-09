// @(#)root/proof:$Id$
// Author: Long Tran-Thanh   14/09/07

/*************************************************************************
 * Copyright (C) 1995-2002, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TProofFile
#define ROOT_TProofFile


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TProofFile                                                           //
//                                                                      //
// Small class to steer the merging of files produced on the workers    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TNamed
#include "TNamed.h"
#endif

class TCollection;
class TProofFile;
class TString;
class TList;
class TFile;

class TProofFile : public TNamed {

friend class TProof;
friend class TProofServ;

public:

private:
   TString        fDir;         // name of the directory
   TString	  fFileName;
   TString	  fFileName1;
   TString	  fLocation;
   TString	  fMode;
   TString	  fOutputFileName;
   TString	  fWorkerOrdinal;
   Bool_t	  fMerged;

   TString	  fMasterHostName;

   const char* GetMasterHostName() const { return fMasterHostName; }

   TString GetTmpName(const char* name);

   void SetFileName(const char* name);
   void SetDir(const char* dir) { fDir = dir; }
   void SetWorkerOrdinal(const char* ordinal) { fWorkerOrdinal = ordinal; }
   void SetMasterHostName(const char* name) { fMasterHostName = name; }

protected:

public:
   TProofFile();
   TProofFile(const char* path, const char* location = "REMOTE", const char* mode = "CENTRAL");
   virtual ~TProofFile() { }

   const char* GetDir() const { return fDir; }
   const char* GetFileName( Bool_t tmpName = kTRUE ) const { return (tmpName) ? fFileName1 : fFileName; }
   const char* GetLocation() const { return fLocation; }
   const char* GetMode() const { return fMode; }
   const char* GetOutputFileName() const { return fOutputFileName; }
   const char* GetWorkerOrdinal() const { return fWorkerOrdinal; }


   TFile* OpenFile(const char* opt);             // Open a file with the specified name in fFileName1

   Long64_t Merge(TCollection* list);

   void     Print(Option_t *option="") const;

   void SetOutputFileName(const char *name);

   ClassDef(TProofFile,1) 

};

#endif
