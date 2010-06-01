// @(#)root/io:$Id$
// Author: Markus Frank 28/10/04

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TEmulatedMapProxy
//
// Streamer around an arbitrary container, which implements basic
// functionality and iteration.
//
// In particular this is used to implement splitting and abstract
// element access of any container. Access to compiled code is necessary
// to implement the abstract iteration sequence and functionality like
// size(), clear(), resize(). resize() may be a void operation.
//
//////////////////////////////////////////////////////////////////////////

#include "TEmulatedMapProxy.h"
#include "TClassEdit.h"
#include "TStreamerInfo.h"
#include "TError.h"

TEmulatedMapProxy::TEmulatedMapProxy(const TEmulatedMapProxy& copy)
   : TEmulatedCollectionProxy(copy)
{
   // copy constructor
   if ( !(fSTL_type == TClassEdit::kMap || fSTL_type == TClassEdit::kMultiMap) )  {
      Fatal("TEmulatedMapProxy","Class %s is not a map-type!",fName.c_str());
   }
}

TEmulatedMapProxy::TEmulatedMapProxy(const char* cl_name)
   : TEmulatedCollectionProxy(cl_name)
{
   // Build a Streamer for an emulated vector whose type is 'name'.
   if ( !(fSTL_type == TClassEdit::kMap || fSTL_type == TClassEdit::kMultiMap) )  {
      Fatal("TEmulatedMapProxy","Class %s is not a map-type!",fName.c_str());
   }
}

TEmulatedMapProxy::~TEmulatedMapProxy()
{
   // Standard destructor.
}

TVirtualCollectionProxy* TEmulatedMapProxy::Generate() const
{
   // Virtual copy constructor.
   if ( !fClass ) Initialize();
   return new TEmulatedMapProxy(*this);
}

void* TEmulatedMapProxy::At(UInt_t idx)
{
   // Return the address of the value at index 'idx'.
   if ( fEnv && fEnv->fObject )   {
      PCont_t c = PCont_t(fEnv->fObject);
      return idx<(c->size()/fValDiff) ? ((char*)&(*c->begin())) + idx*fValDiff : 0;
   }
   Fatal("TEmulatedMapProxy","At> Logic error - no proxy object set.");
   return 0;
}

UInt_t TEmulatedMapProxy::Size() const
{
   // Return the current size of the container.
   if ( fEnv && fEnv->fObject )   {
      PCont_t c = PCont_t(fEnv->fObject);
      return fEnv->fSize = (c->size()/fValDiff);
   }
   Fatal("TEmulatedMapProxy","Size> Logic error - no proxy object set.");
   return 0;
}

void TEmulatedMapProxy::ReadMap(int nElements, TBuffer &b)
{
   // Map input streamer.
   Bool_t vsn3 = b.GetInfo() && b.GetInfo()->GetOldVersion()<=3;
   int    idx, loop, off[2] = {0, fValOffset };
   Value  *v, *val[2] = { fKey, fVal };
   StreamHelper* helper;
   float f;
   char* addr = 0;
   char* temp = (char*)At(0);
   for ( idx = 0; idx < nElements; ++idx )  {
      addr = temp + idx*fValDiff;
      for ( loop=0; loop<2; loop++)  {
         addr += off[loop];
         helper = (StreamHelper*)addr;
         v = val[loop];
         switch (v->fCase) {
         case G__BIT_ISFUNDAMENTAL:  // Only handle primitives this way
         case G__BIT_ISENUM:
            switch( int(v->fKind) )   {
            case kBool_t:    b >> helper->boolean;     break;
            case kChar_t:    b >> helper->s_char;      break;
            case kShort_t:   b >> helper->s_short;     break;
            case kInt_t:     b >> helper->s_int;       break;
            case kLong_t:    b >> helper->s_long;      break;
            case kLong64_t:  b >> helper->s_longlong;  break;
            case kFloat_t:   b >> helper->flt;         break;
            case kFloat16_t: b >> f;
               helper->flt = float(f);  break;
            case kDouble_t:  b >> helper->dbl;         break;
            case kBOOL_t:    b >> helper->boolean;     break;
            case kUChar_t:   b >> helper->u_char;      break;
            case kUShort_t:  b >> helper->u_short;     break;
            case kUInt_t:    b >> helper->u_int;       break;
            case kULong_t:   b >> helper->u_long;      break;
            case kULong64_t: b >> helper->u_longlong;  break;
            case kDouble32_t:b >> f;
               helper->dbl = double(f);  break;
            case kchar:
            case kNoType_t:
            case kOther_t:
               Error("TEmulatedMapProxy","fType %d is not supported yet!\n",v->fKind);
            }
            break;
         case G__BIT_ISCLASS:
            b.StreamObject(helper,v->fType);
            break;
         case kBIT_ISSTRING:
            helper->read_std_string(b);
            break;
         case G__BIT_ISPOINTER|G__BIT_ISCLASS:
            helper->set(b.ReadObjectAny(v->fType));
            break;
         case G__BIT_ISPOINTER|kBIT_ISSTRING:
            helper->read_std_string_pointer(b);
            break;
         case G__BIT_ISPOINTER|kBIT_ISTSTRING|G__BIT_ISCLASS:
            helper->read_tstring_pointer(vsn3,b);
            break;
         }
      }
   }
}

void TEmulatedMapProxy::WriteMap(int nElements, TBuffer &b)
{
   // Map output streamer.
   Value  *v, *val[2] = { fKey, fVal };
   int    off[2]      = { 0, fValOffset };
   StreamHelper* i;
   char* addr = 0;
   char* temp = (char*)At(0);
   for (int loop, idx = 0; idx < nElements; ++idx )  {
      addr = temp + idx*fValDiff;
      for ( loop = 0; loop<2; ++loop )  {
         addr += off[loop];
         i = (StreamHelper*)addr;
         v = val[loop];
         switch (v->fCase) {
         case G__BIT_ISFUNDAMENTAL:  // Only handle primitives this way
         case G__BIT_ISENUM:
            switch( int(v->fKind) )   {
            case kBool_t:    b << i->boolean;     break;
            case kChar_t:    b << i->s_char;      break;
            case kShort_t:   b << i->s_short;     break;
            case kInt_t:     b << i->s_int;       break;
            case kLong_t:    b << i->s_long;      break;
            case kLong64_t:  b << i->s_longlong;  break;
            case kFloat_t:   b << i->flt;         break;
            case kFloat16_t: b << float(i->flt);  break;
            case kDouble_t:  b << i->dbl;         break;
            case kBOOL_t:    b << i->boolean;     break;
            case kUChar_t:   b << i->u_char;      break;
            case kUShort_t:  b << i->u_short;     break;
            case kUInt_t:    b << i->u_int;       break;
            case kULong_t:   b << i->u_long;      break;
            case kULong64_t: b << i->u_longlong;  break;
            case kDouble32_t:b << float(i->dbl);  break;
            case kchar:
            case kNoType_t:
            case kOther_t:
               Error("TEmulatedMapProxy","fType %d is not supported yet!\n",v->fKind);
            }
            break;
         case G__BIT_ISCLASS:
            b.StreamObject(i,v->fType);
            break;
         case kBIT_ISSTRING:
            TString(i->c_str()).Streamer(b);
            break;
         case G__BIT_ISPOINTER|G__BIT_ISCLASS:
            b.WriteObjectAny(i->ptr(),v->fType);
            break;
         case kBIT_ISSTRING|G__BIT_ISPOINTER:
            i->write_std_string_pointer(b);
            break;
         case kBIT_ISTSTRING|G__BIT_ISCLASS|G__BIT_ISPOINTER:
            i->write_tstring_pointer(b);
            break;
         }
      }
   }
}

void TEmulatedMapProxy::Streamer(TBuffer &b)
{
   // TClassStreamer IO overload.
   if ( b.IsReading() ) {  //Read mode
      int nElements = 0;
      b >> nElements;
      if ( fEnv->fObject )  {
         Resize(nElements,true);
      }
      if ( nElements > 0 )  {
         ReadMap(nElements, b);
      }
   }
   else {     // Write case
      int nElements = fEnv->fObject ? Size() : 0;
      b << nElements;
      if ( nElements > 0 )  {
         WriteMap(nElements, b);
      }
   }
}
