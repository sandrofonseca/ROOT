/*****************************************************************************
 * Project: BaBar detector at the SLAC PEP-II B-factory
 * Package: RooFitCore
 *    File: $Id$
 * Authors:
 *   WV, Wouter Verkerke, University of California Santa Barbara, verkerke@slac.stanford.edu
 * History:
 *   05-Mar-2001 WV Created initial version
 *
 * Copyright (C) 2001 University of California
 *****************************************************************************/
#ifndef ROO_UNBLIND_PRECISION
#define ROO_UNBLIND_PRECISION

#include "RooFitCore/RooAbsHiddenReal.hh"
#include "RooFitCore/RooRealProxy.hh"
#include "RooFitModels/RooBlindTools.hh"

class RooUnblindPrecision : public RooAbsHiddenReal {
public:
  // Constructors, assignment etc
  RooUnblindPrecision() ;
  RooUnblindPrecision(const char *name, const char *title, 
		      const char *blindString, Double_t centralValue, Double_t scale, RooAbsReal& blindValue);
  RooUnblindPrecision(const RooUnblindPrecision& other, const char* name=0);
  virtual TObject* clone(const char* newname) const { return new RooUnblindPrecision(*this,newname); }  
  virtual ~RooUnblindPrecision();

protected:

  // Function evaluation
  virtual Double_t evaluate() const ;

  RooRealProxy _value ;          // Holder of the blind value
  RooBlindTools _blindEngine ;   // Blinding engine

  ClassDef(RooUnblindPrecision,1) // Precision unblinding transformation
};

#endif
