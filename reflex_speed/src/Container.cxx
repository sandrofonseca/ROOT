// @(#)root/reflex:$Name:  $:$Id$
// Author: Axel Naumann, 2008

// Copyright CERN, CH-1211 Geneva 23, 2004-2008, All rights reserved.
//
// Permission to use, copy, modify, and distribute this software for any
// purpose is hereby granted without fee, provided that this copyright and
// permissions notice appear in all copies and derivatives.
//
// This software is provided "as is" without express or implied warranty.

#ifndef REFLEX_BUILD
#define REFLEX_BUILD
#endif

#include "Reflex/Container.h"

//-------------------------------------------------------------------------------
Reflex::Internal::ConstIteratorBase::~ConstIteratorBase(){
//-------------------------------------------------------------------------------
// Deletes the interator implementation
   if (fDeleteImpl) delete fImpl;
}

