// @(#)root/star:$Name:  $Id: TGenericTable.h,v 1.1 2001/07/11 06:46:19 brun Exp $
// Author: Valery Fine(fine@bnl.gov)   30/06/2001
// Copyright(c) 2001 [BNL] Brookhaven National Laboratory, Valeri Fine  (fine@bnl.gov). All right reserved",

#ifndef ROOT_TGenericTable
#define ROOT_TGenericTable

#include "TTable.h"

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//  TGenericTable                                                       //
//                                                                      //
//  This is the class to represent the array of C-struct                //
//  defined at run-time                                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
class TGenericTable : public TTable {
  protected:                                        
     TTableDescriptor *fColDescriptors;     
     virtual TTableDescriptor *GetDescriptorPointer() const { return fColDescriptors;}                 
     virtual void SetDescriptorPointer(TTableDescriptor *list) { fColDescriptors = list;}        
	 void SetGenericType(){ TTable::SetType(GetDescriptorPointer()->GetName()); }
 
  public:
    class iterator {
      protected:
	     UInt_t  fRowSize;
	     char   *fCurrentRow;
	     iterator():  fRowSize(0), fCurrentRow(0) {}
      public:
	     iterator(UInt_t size, char &rowPtr): fRowSize(size), fCurrentRow(&rowPtr){}
	     iterator(const TTable &t, char &rowPtr): fRowSize(t.GetRowSize()), fCurrentRow(&rowPtr){}
		 iterator(const iterator& iter) : fRowSize (iter.fRowSize), fCurrentRow(iter.fCurrentRow){}
	     iterator &operator++() { if (fCurrentRow) fCurrentRow+=fRowSize; return *this;}
	     iterator &operator--() { if (fCurrentRow) fCurrentRow-=fRowSize; return *this;}
	     iterator &operator+(Int_t idx) { if (fCurrentRow) fCurrentRow+=idx*fRowSize; return *this;}
	     iterator &operator-(Int_t idx) { if (fCurrentRow) fCurrentRow-=idx*fRowSize; return *this;}
	     Int_t operator-(const iterator &it) const { return (fCurrentRow-it.fCurrentRow)/fRowSize; }
         char *operator *(){ return fCurrentRow;}
		 Bool_t operator==(const iterator &t) const { return (fCurrentRow == t.fCurrentRow); }
		 Bool_t operator!=(const iterator &t) const { return !operator==(t); }
	};                                        
    TGenericTable() : TTable("TGenericTable",-1), fColDescriptors(0) {SetType("generic");}      

	// Create TGenericTable by C structure name provided
    TGenericTable(const char *structName, const Text_t *name);
    TGenericTable(const char *structName, Int_t n);
    TGenericTable(const char *structName, const Text_t *name,Int_t n);

	// Create TGenericTable by TTableDescriptor pointer
    TGenericTable(const TTableDescriptor &dsc, const Text_t *name);
    TGenericTable(const TTableDescriptor &dsc, Int_t n);
    TGenericTable(const TTableDescriptor &dsc,const Text_t *name,Int_t n);

	virtual ~TGenericTable();

                char  *GetTable(Int_t i=0)   const { return ((char *)GetArray())+i*GetRowSize();}     
    TTableDescriptor  *GetTableDescriptors() const { return GetDescriptorPointer();}
	TTableDescriptor  *GetRowDescriptors()   const { return GetDescriptorPointer();}
    char &operator[](Int_t i){ assert(i>=0 && i < GetNRows()); return *GetTable(i); }             
    const char &operator[](Int_t i) const { assert(i>=0 && i < GetNRows()); return *((const char *)(GetTable(i))); } 
    iterator begin() const  {                      return GetNRows() ? iterator(*this, *GetTable(0)):end();}
    iterator end()   { return ((const TGenericTable *)this)->end(); }
    iterator end()   const  {Long_t i = GetNRows(); return i? iterator(*this, *GetTable(i)):iterator(*this,*(char *)0);}
	ClassDef(TGenericTable,3) // Generic array of C-structure (a'la STL vector)
};
 
#endif
