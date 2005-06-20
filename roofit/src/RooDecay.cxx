/*****************************************************************************
 * Project: RooFit                                                           *
 * Package: RooFitModels                                                     *
 *    File: $Id: RooDecay.cc,v 1.15 2005/06/16 09:37:27 wverkerke Exp $
 * Authors:                                                                  *
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu       *
 *   DK, David Kirkby,    UC Irvine,         dkirkby@uci.edu                 *
 *                                                                           *
 * Copyright (c) 2000-2005, Regents of the University of California          *
 *                          and Stanford University. All rights reserved.    *
 *                                                                           *
 * Redistribution and use in source and binary forms,                        *
 * with or without modification, are permitted according to the terms        *
 * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
 *****************************************************************************/

// -- CLASS DESCRIPTION [PDF] --
// 

#include "RooFitCore/RooFit.hh"

#include "Riostream.h"
#include "Riostream.h"
#include "RooFitModels/RooDecay.hh"
#include "RooFitCore/RooRealVar.hh"
#include "RooFitCore/RooRandom.hh"

ClassImp(RooDecay) 
;


RooDecay::RooDecay(const char *name, const char *title, 
		   RooRealVar& t, RooAbsReal& tau, 
		   const RooResolutionModel& model, DecayType type) :
  RooAbsAnaConvPdf(name,title,model,t), 
  _t("t","time",this,t),
  _tau("tau","decay time",this,tau),
  _type(type)
{
  // Constructor
  switch(type) {
  case SingleSided:
    _basisExp = declareBasis("exp(-@0/@1)",tau) ;
    break ;
  case Flipped:
    _basisExp = declareBasis("exp(@0/@1)",tau) ;
    break ;
  case DoubleSided:
    _basisExp = declareBasis("exp(-abs(@0)/@1)",tau) ;
    break ;
  }
}


RooDecay::RooDecay(const RooDecay& other, const char* name) : 
  RooAbsAnaConvPdf(other,name), 
  _t("t",this,other._t),
  _tau("tau",this,other._tau),
  _type(other._type),
  _basisExp(other._basisExp)
{
  // Copy constructor
}



RooDecay::~RooDecay()
{
  // Destructor
}


Double_t RooDecay::coefficient(Int_t /*basisIndex*/) const 
{
  return 1 ;
}



Int_t RooDecay::getGenerator(const RooArgSet& directVars, RooArgSet &generateVars, Bool_t /*staticInitOK*/) const
{
  if (matchArgs(directVars,generateVars,_t)) return 1 ;  
  return 0 ;
}



void RooDecay::generateEvent(Int_t code)
{
  assert(code==1) ;

  // Generate delta-t dependent
  while(1) {
    Double_t rand = RooRandom::uniform() ;
    Double_t tval(0) ;

    switch(_type) {
    case SingleSided:
      tval = -_tau*log(rand);
      break ;
    case Flipped:
      tval= +_tau*log(rand);
      break ;
    case DoubleSided:
      tval = (rand<=0.5) ? -_tau*log(2*rand) : +_tau*log(2*(rand-0.5)) ;
      break ;
    }
    
    if (tval<_t.max() && tval>_t.min()) {
      _t = tval ;
      break ;
    }
  }  
}
