// @(#)root/roostats:$Id: FeldmanCousins.h 26805 2009-01-13 17:45:57Z cranmer $
// Author: Kyle Cranmer, Lorenzo Moneta, Gregory Schott, Wouter Verkerke
/*************************************************************************
 * Copyright (C) 1995-2008, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOSTATS_FeldmanCousins
#define ROOSTATS_FeldmanCousins


#ifndef ROOT_Rtypes
#include "Rtypes.h"
#endif

#ifndef ROOSTATS_IntervalCalculator
#include "RooStats/IntervalCalculator.h"
#endif

#include "RooStats/ToyMCSampler.h"
#include "RooStats/ConfidenceBelt.h"

#include "RooAbsData.h"
//#include "RooWorkspace.h"
#include "RooAbsPdf.h"
#include "RooArgSet.h"
#include "TList.h"

class RooAbsData; 

namespace RooStats {

   class ConfInterval; 

   class FeldmanCousins : public IntervalCalculator, public TNamed {

   public:

     FeldmanCousins();

     virtual ~FeldmanCousins();
    
      // Main interface to get a ConfInterval (will be a PointSetInterval)
      virtual ConfInterval* GetInterval() const;

      // Get the size of the test (eg. rate of Type I error)
      virtual Double_t Size() const {return fSize;}
      // Get the Confidence level for the test
      virtual Double_t ConfidenceLevel()  const {return 1.-fSize;}  
      // set a workspace that owns all the necessary components for the analysis
//       virtual void SetWorkspace(RooWorkspace& ws) {
// 	if(fOwnsWorkspace && fWS) delete fWS;
// 	fOwnsWorkspace = false;
// 	fWS = &ws;
//       }

      // Set the DataSet
    virtual void SetData(RooAbsData& data) {  fData = &data; }    
//          if (!fWS) {
//             fWS = new RooWorkspace();
//             fOwnsWorkspace = true; 
//          }
//          if (! fWS->data(data.GetName()) ) {
// 	   RooMsgService::instance().setGlobalKillBelow(RooFit::ERROR) ;
//             fWS->import(data);
// 	    RooMsgService::instance().setGlobalKillBelow(RooFit::DEBUG) ;
//          }
//          SetData(data.GetName());
//       }

      // Set the Pdf
    virtual void SetPdf(RooAbsPdf& pdf) { fPdf = &pdf; }	
//          if (!fWS) 
//             fWS = new RooWorkspace();
//          if (! fWS->pdf( pdf.GetName() ))
//          {
//             RooMsgService::instance().setGlobalKillBelow(RooFit::ERROR) ;
//             fWS->import(pdf);
//             RooMsgService::instance().setGlobalKillBelow(RooFit::DEBUG) ;
//          }
//          SetPdf(pdf.GetName());
//       }

//       // specify the name of the dataset in the workspace to be used
//       virtual void SetData(const char* name) {fDataName = name;}
//       // specify the name of the PDF in the workspace to be used
//       virtual void SetPdf(const char* name) {fPdfName = name;}

      // specify the parameters of interest in the interval
      virtual void SetParameters(const RooArgSet& set) {fPOI = &set;}
      // specify the nuisance parameters (eg. the rest of the parameters)
      virtual void SetNuisanceParameters(const RooArgSet& set) {fNuisParams = &set;}
      // set the size of the test (rate of Type I error) ( Eg. 0.05 for a 95% Confidence Interval)
      virtual void SetTestSize(Double_t size) {fSize = size;}
      // set the confidence level for the interval (eg. 0.95 for a 95% Confidence Interval)
      virtual void SetConfidenceLevel(Double_t cl) {fSize = 1.-cl;}

      virtual void SetModel(const ModelConfig &); 

      RooAbsData* GetPointsToScan() {
	if(!fPointsToTest) CreateParameterPoints();	  
	return fPointsToTest;
      }

      ConfidenceBelt* GetConfidenceBelt() {return fConfBelt;}

      void UseAdaptiveSampling(bool flag=true){fAdaptiveSampling=flag;}

      void SetNBins(Int_t bins) {fNbins = bins;}

      void FluctuateNumDataEntries(bool flag=true){fFluctuateData = flag;}

      void SaveBeltToFile(bool flag=true){
	fSaveBeltToFile = flag;
	if(flag) fCreateBelt = true;
      }
      void CreateConfBelt(bool flag=true){fCreateBelt = flag;}

      
   private:

      // initializes fPointsToTest data member (mutable)
      void CreateParameterPoints() const;

      // initializes fTestStatSampler data member (mutable)
      void CreateTestStatSampler() const;

      Double_t fSize; // size of the test (eg. specified rate of Type I error)
      RooAbsPdf * fPdf; // common PDF
      RooAbsData * fData; // data set 

//       RooWorkspace* fWS; // a workspace that owns all the components to be used by the calculator
//       Bool_t fOwnsWorkspace; // flag if this object owns its workspace
//       const char* fPdfName; // name of  common PDF in workspace
//       const char* fDataName; // name of data set in workspace
      const RooArgSet* fPOI; // RooArgSet specifying  parameters of interest for interval
      const RooArgSet* fNuisParams;// RooArgSet specifying  nuisance parameters for interval
      mutable ToyMCSampler* fTestStatSampler; // the test statistic sampler
      mutable RooAbsData* fPointsToTest; // points to perform the construction
      mutable ConfidenceBelt* fConfBelt;
      bool fAdaptiveSampling; // controls use of adaptive sampling algorithm
      Int_t fNbins; // number of samples per variable
      Bool_t fFluctuateData;  // tell ToyMCSampler to fluctuate number of entries in dataset
      Bool_t fDoProfileConstruction; // instead of full construction over nuisance parametrs, do profile
      bool fSaveBeltToFile; // controls use if ConfidenceBelt should be saved to a TFile
      bool fCreateBelt; // controls use if ConfidenceBelt should be saved to a TFile

   protected:
      ClassDef(FeldmanCousins,1)   // Interface for tools setting limits (producing confidence intervals)
   };
}


#endif
