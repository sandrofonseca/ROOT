// @(#)root/reflex:$Id$
// Author: Stefan Roiser 2004

// Copyright CERN, CH-1211 Geneva 23, 2004-2006, All rights reserved.
//
// Permission to use, copy, modify, and distribute this software for any
// purpose is hereby granted without fee, provided that this copyright and
// permissions notice appear in all copies and derivatives.
//
// This software is provided "as is" without express or implied warranty.

#ifndef REFLEX_BUILD
#define REFLEX_BUILD
#endif

#include "Reflex/internal/TypeName.h"

#include "Reflex/Type.h"
#include "Reflex/internal/OwnedMember.h"

#include "CatalogImpl.h"

#include "stl_hash.h"
#include <vector>

//-------------------------------------------------------------------------------
Reflex::TypeName::TypeName( const char * nam,
                            TypeBase * typeBas,
                            const std::type_info * ti,
                            const Catalog& catalog)
   : fName( nam ),
     fTypeBase( typeBas ),
     fCatalog(catalog),
     fCallbacks(0)
{
//-------------------------------------------------------------------------------
// Construct a type name.
   fThisType = new Type(this);
   catalog.ToImpl()->Types().Add(*this, ti);
}


//-------------------------------------------------------------------------------
Reflex::TypeName::~TypeName() {
//-------------------------------------------------------------------------------
// Destructor.
}


//-------------------------------------------------------------------------------
void Reflex::TypeName::DeleteType() const {
//-------------------------------------------------------------------------------
// Delete the type base information.
   delete fTypeBase;
   fTypeBase = 0;
}


//-------------------------------------------------------------------------------
void Reflex::TypeName::SetTypeId( const std::type_info & ti ) const {
//-------------------------------------------------------------------------------
// Add a type_info to the map.
   fCatalog.ToImpl()->Types().UpdateTypeId(*const_cast<TypeName*>(this),
                                           ti,
                                           fThisType ? fThisType->TypeInfo()
                                           : typeid(NullType));
}


//-------------------------------------------------------------------------------
Reflex::Type
Reflex::TypeName::ByName( const std::string & key ) {
//-------------------------------------------------------------------------------
// Lookup a type by name.
   return Catalog::Instance().TypeByName(key);
}


//-------------------------------------------------------------------------------
Reflex::Type
Reflex::TypeName::ByTypeInfo( const std::type_info & ti ) {
//-------------------------------------------------------------------------------
// Lookup a type by type_info.
   return Catalog::Instance().TypeByTypeInfo(ti);
}


//-------------------------------------------------------------------------------
void Reflex::TypeName::HideName() {
//-------------------------------------------------------------------------------
// Append the string " @HIDDEN@" to a type name.
   if ( fName.length() == 0 || fName[fName.length()-1] != '@' ) {
      fCatalog.ToImpl()->Types().Remove(*this);
      fName += " @HIDDEN@";
      fCatalog.ToImpl()->Types().Add(*this, 0);
   }
}

//-------------------------------------------------------------------------------
void Reflex::TypeName::UnhideName() {
   //-------------------------------------------------------------------------------
   // Remove the string " @HIDDEN@" to a type name.
   static const unsigned int len = strlen(" @HIDDEN@");
   if ( fName.length() > len
        && fName[fName.length()-1] == '@'
        && 0==strcmp(" @HIDDEN@",fName.c_str()+fName.length()-len) ){
      fCatalog.ToImpl()->Types().Remove(*this);
      fName.erase(fName.length()-len);
      fCatalog.ToImpl()->Types().Add(*this, 0);
   }
}

//-------------------------------------------------------------------------------
Reflex::Type Reflex::TypeName::ThisType() const {
//-------------------------------------------------------------------------------
// Return Type of this TypeName.
   return *fThisType;
}


//-------------------------------------------------------------------------------
Reflex::Type Reflex::TypeName::TypeAt( size_t nth ) {
//-------------------------------------------------------------------------------
// Return nth type in Reflex.
   return Catalog::Instance().TypeAt(nth);
}


//-------------------------------------------------------------------------------
size_t Reflex::TypeName::TypeSize() {
//-------------------------------------------------------------------------------
// Return number of types in Reflex.
   return Catalog::Instance().TypeSize();
}


//-------------------------------------------------------------------------------
Reflex::Type_Iterator Reflex::TypeName::Type_Begin() {
//-------------------------------------------------------------------------------
// Return begin iterator of the type container.
   return Catalog::Instance().Type_Begin();
}


//-------------------------------------------------------------------------------
Reflex::Type_Iterator Reflex::TypeName::Type_End() {
//-------------------------------------------------------------------------------
// Return end iterator of the type container.
   return Catalog::Instance().Type_End();
}


//-------------------------------------------------------------------------------
Reflex::Reverse_Type_Iterator Reflex::TypeName::Type_RBegin() {
//-------------------------------------------------------------------------------
// Return rbegin iterator of the type container.
   return Catalog::Instance().Type_RBegin();
}


//-------------------------------------------------------------------------------
Reflex::Reverse_Type_Iterator Reflex::TypeName::Type_REnd() {
//-------------------------------------------------------------------------------
// Return rend iterator of the type container.
   return Catalog::Instance().Type_REnd();
}


//-------------------------------------------------------------------------------
void
Reflex::TypeName::RegisterCallback(Callback& cb) {
//-------------------------------------------------------------------------------
// Register a callback for this type
   if (!fCallbacks) fCallbacks = new std::set<Callback*>;
   fCallbacks->insert(&cb);
}


//-------------------------------------------------------------------------------
void
Reflex::TypeName::UnregisterCallback(Callback& cb) {
//-------------------------------------------------------------------------------
// Remove a callback for this type
   if (fCallbacks) {
      std::set<Callback*>::iterator i = fCallbacks->find(&cb);
      if (i != fCallbacks->end())
         fCallbacks->erase(i);
   }
}

