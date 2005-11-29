// @(#)root/minuit2:$Name:  $:$Id: AnalyticalGradientCalculator.cpp,v 1.5.6.4 2005/11/29 11:08:35 moneta Exp $
// Authors: M. Winkler, F. James, L. Moneta, A. Zsenei   2003-2005  

/**********************************************************************
 *                                                                    *
 * Copyright (c) 2005 LCG ROOT Math team,  CERN/PH-SFT                *
 *                                                                    *
 **********************************************************************/

#include "Minuit2/AnalyticalGradientCalculator.h"
#include "Minuit2/FCNGradientBase.h"
#include "Minuit2/MnUserTransformation.h"
#include "Minuit2/FunctionGradient.h"
#include "Minuit2/MinimumParameters.h"
#include "Minuit2/MnMatrix.h"

namespace ROOT {

   namespace Minuit2 {


FunctionGradient AnalyticalGradientCalculator::operator()(const MinimumParameters& par) const {

  std::vector<double> grad = fGradCalc.Gradient(fTransformation(par.Vec()));
  assert(grad.size() == fTransformation.Parameters().size());

  MnAlgebraicVector v(par.Vec().size());
  for(unsigned int i = 0; i < par.Vec().size(); i++) {
    unsigned int ext = fTransformation.ExtOfInt(i);
    if(fTransformation.Parameter(ext).HasLimits()) {
      //double dd = (fTransformation.Parameter(ext).Upper() - fTransformation.Parameter(ext).Lower())*0.5*cos(par.Vec()(i));
//       const ParameterTransformation * pt = fTransformation.transformation(ext); 
//       double dd = pt->dInt2ext(par.Vec()(i), fTransformation.Parameter(ext).Lower(), fTransformation.Parameter(ext).Upper() );       
      double dd = fTransformation.DInt2Ext(i, par.Vec()(i));
      v(i) = dd*grad[ext];
    } else {
      v(i) = grad[ext];
    }
  }

  return FunctionGradient(v);
}

FunctionGradient AnalyticalGradientCalculator::operator()(const MinimumParameters& par, const FunctionGradient&) const {

  return (*this)(par);
}

bool AnalyticalGradientCalculator::CheckGradient() const {
  return fGradCalc.CheckGradient();
}

  }  // namespace Minuit2

}  // namespace ROOT
