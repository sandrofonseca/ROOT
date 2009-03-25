/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
  * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/

#ifndef NUMUTONUE_OSCILLATION
#define NUMUTONUE_OSCILLATION

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
 
class NuMuToNuE_Oscillation : public RooAbsPdf {
public:
  NuMuToNuE_Oscillation() {} ; 
  NuMuToNuE_Oscillation(const char *name, const char *title,
	      RooAbsReal& _L,
	      RooAbsReal& _E,
	      RooAbsReal& _sinSq2theta,
	      RooAbsReal& _deltaMSq);
  NuMuToNuE_Oscillation(const NuMuToNuE_Oscillation& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new NuMuToNuE_Oscillation(*this,newname); }
  inline virtual ~NuMuToNuE_Oscillation() { }

protected:

  RooRealProxy L ;
  RooRealProxy E ;
  RooRealProxy sinSq2theta ;
  RooRealProxy deltaMSq ;
  
  Double_t evaluate() const ;

private:

  ClassDef(NuMuToNuE_Oscillation,1) // Your description goes here...
};
 
#endif
