// @(#)root/html:$Id$
// Author: Axel Naumann 2009-10-26

/*************************************************************************
 * Copyright (C) 1995-2009, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TDocStringTable
#define ROOT_TDocStringTable

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// TDocStringTable                                                            //
//                                                                        //
// Base class for documentation                                           //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TObjArray
#include "TObjArray.h"
#endif
#ifndef ROOT_THashTable
#include "THashTable.h"
#endif

namespace Doc {
class TDocStringTable: public TObject {
public:
   TDocStringTable() {fIndexedStrings.SetOwner();}
   virtual ~TDocStringTable();
   void Insert(const char* str) { GetIndexOrAdd(str); }
   UInt_t GetIndexOrAdd(const char* str);
   const TString& GetString(UInt_t idx) const;

   const TObjArray* GetArray() const { return &fIndexedStrings; }
   Bool_t Contains(const char* str) const { return GetIndex(str) != -1; }
   Int_t GetIndex(const char* str) const;

   void Remove(const char* str);

private:
   THashTable fHashedStrings; // strings accessible by their hashes
   TObjArray  fIndexedStrings;// strings accessible by their index

   ClassDef(TDocStringTable, 1); // Container of class names
};
} // namespace Doc

#endif // ROOT_TDocStringTable
