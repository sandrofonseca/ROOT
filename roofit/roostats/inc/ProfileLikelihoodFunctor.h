// @(#)root/roostats:$Id: ProfileLikelihoodFunctor.h 26805 2009-01-13 17:45:57Z cranmer $
// Author: Kyle Cranmer, Lorenzo Moneta, Gregory Schott, Wouter Verkerke
/*************************************************************************
 * Copyright (C) 1995-2008, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOSTATS_ProfileLikelihoodFunctor
#define ROOSTATS_ProfileLikelihoodFunctor

//_________________________________________________
/*
BEGIN_HTML
<p>
ProfileLikelihoodFunctor is a simple implementation of the DistributionCreator interface used for debugging.
The sampling distribution is uniformly random between [0,1] and is INDEPENDENT of the data.  So it is not useful
for true statistical tests, but it is useful for debugging.
</p>
END_HTML
*/
//

#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif

#include "RooStats/DistributionCreator.h"
#include "RooAbsPdf.h"
#include "RooArgSet.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "SamplingDistribution.h"
#include "TRandom.h"
#include <vector>
#include "RooStats/TemplatedDistributionCreator.h"
#include "RooProfileLL.h"
#include "RooNLLVar.h"

namespace RooStats {

 class ProfileLikelihoodFunctor {

   public:
     ProfileLikelihoodFunctor(RooAbsPdf& pdf) {
       fPdf = &pdf;
       fProfile = 0;
     }
     virtual ~ProfileLikelihoodFunctor() {
       //       delete fRand;
       //       delete fTestStatistic;
     }
    
     // Main interface to evaluate the test statistic on a dataset
     virtual Double_t Evaluate(RooAbsData& data, RooArgSet& paramsOfInterest)  {       
       if(!fProfile){ 
	 RooNLLVar* nll = new RooNLLVar("nll","",*fPdf,data);
	 fProfile = new RooProfileLL("pll","",*nll, paramsOfInterest);
	 fProfile->addOwnedComponents(*nll) ;  // to avoid memory leak       
       }

       RooArgSet* paramsToChange = fProfile->getParameters(data);

       TIter it = paramsOfInterest.createIterator();
       RooRealVar *myarg; 
       RooRealVar *mytarget; 
       while ((myarg = (RooRealVar *)it.Next())) { 
	 if(!myarg) continue;
	 mytarget = (RooRealVar*) paramsToChange->find(myarg->GetName());
	 if(!mytarget) continue;
	 mytarget->setVal( myarg->getVal() );
       }


       /*       TIter      itr = fProfile->getParameters(data)->createIterator();
       while ((myarg = (RooRealVar *)itr.Next())) { 
	 cout << myarg->GetName() << myarg->getVal();
       }
       cout << " = " << fProfile->evaluate()  << endl;
       */

       return fProfile->evaluate();
     }

      // Get the TestStatistic
      virtual const RooAbsArg* GetTestStatistic()  const {return fProfile;}  
    
      
   private:
      RooProfileLL* fProfile;
      RooAbsPdf* fPdf;

   protected:
      ClassDef(ProfileLikelihoodFunctor,1)   
   };

 typedef TemplatedDistributionCreator<ProfileLikelihoodFunctor> ProfileDistributionCreator;


}


#endif
