/*****************************************************************************
 * Project: BaBar detector at the SLAC PEP-II B-factory
 * Package: RooFitCore
 *    File: $Id: RooAbsReal.cc,v 1.2 2001/03/19 15:57:30 verkerke Exp $
 * Authors:
 *   DK, David Kirkby, Stanford University, kirkby@hep.stanford.edu
 *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu
 * History:
 *   07-Mar-2001 WV Created initial version
 *
 * Copyright (C) 2001 University of California
 *****************************************************************************/

#include <iostream.h>
#include "TObjString.h"
#include "TH1.h"
#include "RooFitCore/RooAbsReal.hh"
#include "RooFitCore/RooArgSet.hh"

ClassImp(RooAbsReal) 
;


RooAbsReal::RooAbsReal(const char *name, const char *title, const char *unit= "") : 
  RooAbsArg(name,title), _unit(unit), _plotBins(100), _value(0), _plotMin(0), _plotMax(0)
{
  setValueDirty(kTRUE) ;
  setShapeDirty(kTRUE) ;
}

RooAbsReal::RooAbsReal(const char *name, const char *title, Double_t minVal, Double_t maxVal, const char *unit= "") :
  RooAbsArg(name,title), _unit(unit), _plotBins(100), _value(0), _plotMin(minVal), _plotMax(maxVal)
{
  setValueDirty(kTRUE) ;
  setShapeDirty(kTRUE) ;
}


RooAbsReal::RooAbsReal(const RooAbsReal& other) : 
  RooAbsArg(other), _unit(other._unit), _plotBins(other._plotBins), 
  _plotMin(other._plotMin), _plotMax(other._plotMax), _value(other._value)
{
  setValueDirty(kTRUE) ;
  setShapeDirty(kTRUE) ;
}


RooAbsReal::~RooAbsReal()
{
}


RooAbsArg& RooAbsReal::operator=(RooAbsArg& aother)
{
  RooAbsArg::operator=(aother) ;

  RooAbsReal& other=(RooAbsReal&)aother ;
  _value    = other._value ;
  _unit     = other._unit ;
  _plotMin  = other._plotMin ;
  _plotMax  = other._plotMax ;
  _plotBins = other._plotBins ;

  setValueDirty(kTRUE) ;
  setShapeDirty(kTRUE) ;
  return *this ;
}


Double_t RooAbsReal::getVal() const
{
  // Return value of object. Calculated if dirty, otherwise cached value is returned.
  if (isValueDirty()) {
    setValueDirty(false) ;
    _value = traceEval() ;
  } 
  
  return _value ;
}



Bool_t RooAbsReal::operator==(Double_t value) 
{
  return (getVal()==value) ;
}



const char *RooAbsReal::getPlotLabel() const {
  // Get the label associated with the variable
  return _label.IsNull() ? fName.Data() : _label.Data();
}

void RooAbsReal::setPlotLabel(const char *label) {
  // Set the label associated with this variable
  _label= label;
}



Bool_t RooAbsReal::readFromStream(istream& is, Bool_t compact, Bool_t verbose) 
{
  //Read object contents from stream (dummy for now)
} 

void RooAbsReal::writeToStream(ostream& os, Bool_t compact) const
{
  //Write object contents to stream (dummy for now)
}

void RooAbsReal::printToStream(ostream& os, PrintOption opt) const
{
  //Print object contents
  os << "RooAbsReal: " << GetName() << " = " << getVal();
  if(!_unit.IsNull()) os << ' ' << _unit;
  os << " : \"" << fTitle << "\"" ;

  printAttribList(os) ;
  os << endl ;
}


void RooAbsReal::setPlotMin(Double_t value) {
  // Set minimum value of output associated with this object

  // Check if new limit is consistent
  if (_plotMin>_plotMax) {
    cout << "RooAbsReal::setPlotMin(" << GetName() 
	 << "): Proposed new integration min. larger than max., setting min. to max." << endl ;
    _plotMin = _plotMax ;
  } else {
    _plotMin = value ;
  }

  setShapeDirty(kTRUE) ;
}

void RooAbsReal::setPlotMax(Double_t value) {
  // Set maximum value of output associated with this object

  // Check if new limit is consistent
  if (_plotMax<_plotMin) {
    cout << "RooAbsReal::setPlotMax(" << GetName() 
	 << "): Proposed new integration max. smaller than min., setting max. to min." << endl ;
    _plotMax = _plotMin ;
  } else {
    _plotMax = value ;
  }

  setShapeDirty(kTRUE) ;
}


void RooAbsReal::setPlotRange(Double_t min, Double_t max) {
  // Check if new limit is consistent
  if (min>max) {
    cout << "RooAbsReal::setPlotMinMax(" << GetName() 
	 << "): Proposed new integration max. smaller than min., setting max. to min." << endl ;
    _plotMin = min ;
    _plotMax = min ;
  } else {
    _plotMin = min ;
    _plotMax = max ;
  }

  setShapeDirty(kTRUE) ;  
}


void RooAbsReal::setPlotBins(Int_t value) {
  // Set number of histogram bins 
  _plotBins = value ;  
}


Bool_t RooAbsReal::inPlotRange(Double_t value) const {
  // Check if given value is in the min-max range for this object
  return (value >= _plotMin && value <= _plotMax) ? kTRUE : kFALSE;
}



Bool_t RooAbsReal::isValid() const {
  return isValid(getVal()) ;
}


Bool_t RooAbsReal::isValid(Double_t value) const {
  return kTRUE ;
}



Double_t RooAbsReal::traceEval() const
{
  Double_t value = evaluate() ;
  
  //Standard tracing code goes here
  if (!isValid(value)) {
    cout << "RooAbsReal::traceEval(" << GetName() << "): validation failed: " << value << endl ;
  }

  //Call optional subclass tracing code
  traceEvalHook(value) ;

  return value ;
}



TH1F *RooAbsReal::createHistogram(const char *label, const char *axis,
				  Int_t bins) {
  // Create a 1D-histogram with appropriate scale and labels for this variable
  return createHistogram(label, axis, _plotMin, _plotMax, bins);
}

TH1F *RooAbsReal::createHistogram(const char *label, const char *axis,
				  Double_t lo, Double_t hi, Int_t bins) {
  // Create a 1D-histogram with appropriate scale and labels for this variable
  char buffer[256];
  if(label) {
    sprintf(buffer, "%s:%s", label, fName.Data());
  }
  else {
    sprintf(buffer, "%s", fName.Data());
  }
  // use the default binning, if no override is specified
  if(bins <= 0) bins= getPlotBins();
  TH1F* histogram= new TH1F(buffer, fTitle, bins, lo, hi);
  if(!histogram) {
    cout << fName << ": unable to create new histogram" << endl;
    return 0;
  }
  const char *unit= getUnit();
  if(*unit) {
    sprintf(buffer, "%s (%s)", fTitle.Data(), unit);
    histogram->SetXTitle(buffer);
  }
  else {
    histogram->SetXTitle((Text_t*)fTitle.Data());
  }
  if(axis) {
    Double_t delta= (_plotMax-_plotMin)/bins;
    if(unit) {
      sprintf(buffer, "%s / %g %s", axis, delta, unit);
    }
    else {
      sprintf(buffer, "%s / %g", axis, delta);
    }
    histogram->SetYTitle(buffer);
  }
  return histogram;
}

