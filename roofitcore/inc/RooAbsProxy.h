/*****************************************************************************
 * Project: BaBar detector at the SLAC PEP-II B-factory
 * Package: RooFitCore
 *    File: $Id: RooAbsProxy.rdl,v 1.4 2001/06/06 00:06:38 verkerke Exp $
 * Authors:
 *   DK, David Kirkby, Stanford University, kirkby@hep.stanford.edu
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu
 * History:
 *   07-Mar-2001 WV Created initial version
 *
 * Copyright (C) 2001 University of California
 *****************************************************************************/
#ifndef ROO_ABS_PROXY
#define ROO_ABS_PROXY

#include "TObject.h"
#include "RooFitCore/RooAbsArg.hh"

class RooAbsProxy {
public:

  // Constructors, assignment etc.
  RooAbsProxy() ;
  RooAbsProxy(const char* name, const RooAbsProxy& other) ;
  virtual ~RooAbsProxy() {} ;

  virtual const char* name() const { return "dummy" ; } ;

protected:

  RooDataSet* _dset ;

  friend class RooAbsArg ;
  virtual Bool_t changePointer(const RooArgSet& newServerSet) = 0 ;

  friend class RooAbsPdf ;
  virtual void changeDataSet(const RooDataSet* newDataSet) ;

  ClassDef(RooAbsProxy,0) // Abstract proxy interface
} ;

#endif

