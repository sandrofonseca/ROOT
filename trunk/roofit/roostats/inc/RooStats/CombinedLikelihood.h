// @(#)root/roostats:$Id$
// Author: Ioan Gabriel Bucur   30/07/2012


#ifndef ROOSTATS_CombinedLikelihood
#define ROOSTATS_CombinedLikelihood

#include "TObject.h"
#include "RooAbsPdf.h"
#include "RooArgList.h"
#include "RooDataSet.h"
#include "RooSimultaneous.h"
#include "RooAbsCategoryLValue.h"
#include "RooArgSet.h"
#include "RooAddPdf.h"
#include "RooRealSumPdf.h"
#include "RooRealVar.h"
#include "RooSetProxy.h"
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <cmath>

class CachedArgSet {
public:
   CachedArgSet(const RooAbsCollection& set) { 
      TIterator *iter = set.createIterator();
      for(TObject *obj = iter->Next(); obj != NULL; obj = iter->Next()) {
         RooRealVar *var = dynamic_cast<RooRealVar *>(obj);
         if(var) {
            fVariables.push_back(var);
            fValues.push_back(var->getVal());
         }
      }
      delete iter;
   }
   Bool_t IsDirty(Bool_t updateIfChanged = kFALSE) {
      std::vector<RooRealVar*>::const_iterator varIter = fVariables.begin(), varEnd = fVariables.end();
      std::vector<Double_t>::iterator valIter = fValues.begin();
      Bool_t hasChanged = kFALSE;
      for( ; varIter != varEnd; ++varIter, ++valIter) {
         Double_t value = (*varIter)->getVal();
         if (value != *valIter) {
            hasChanged = kTRUE;
            if (updateIfChanged) { *valIter = value; }
            else break;
         }
      }
      return hasChanged;
   }

private:
   std::vector<RooRealVar*> fVariables;
   std::vector<Double_t> fValues;
};




class CachedPdf {
public:
   // TODO: RooAbsPdf instead of RooAbsReal
   CachedPdf(RooAbsReal *pdf, const RooArgSet* obs) : fPdf(pdf), fObs(obs), fCacheObs(*fObs) {}
   void SetDataDirty() { fLastData = NULL; }
   const std::vector<Double_t>& Eval(const RooAbsData& data) {
//      std::cout << "CachedPdf::Eval -> "; fPdf->Print("");
      bool newData = (fLastData != &data);
      if (newData) {
         fLastData = &data;
         fPdf->optimizeCacheMode(*data.get()); 
         fPdf->attachDataSet(data); // TODO: is it necessary?
         const_cast<RooAbsData*>(fLastData)->setDirtyProp(kFALSE);
         Fill(data);
      }
      // TODO: see if it is worth to have multiple caches
      // if observables have changed value, the cache is renewed
      else if (fCacheObs.IsDirty(kTRUE)) {
         Fill(data);
         std::cout << "fCacheObs gets dirty" << std::endl;
      }

      return fValues;
   }

private:
   RooAbsReal* fPdf;
   const RooArgSet* fObs;
   CachedArgSet fCacheObs;
   std::vector<RooRealVar*> fVariables;
   std::vector<Double_t> fValues;
   const RooAbsData* fLastData;   

   void Fill(const RooAbsData& data) {
//      std::cout << "CachedPdf::Fill -> "; data.Print("");
 //    fPdf->Print("");
      Int_t numEntries = data.numEntries();
      fValues.resize(numEntries);
      std::vector<Double_t>::iterator itVal = fValues.begin();

      for(Int_t i = 0; i < numEntries; ++i) {
         data.get(i);
         fValues[i] = fPdf->getVal(fObs);
      }
   }
};


class SumLikelihood : public RooAbsReal{
public:
   SumLikelihood() : RooAbsReal() { }

   SumLikelihood(RooAbsPdf* pdf, RooAbsData* data) :
      RooAbsReal("SumLikelihood", "Cached version of normal log likelihood sum"),
      fPdf(pdf),
//      fParameters("params", "parameters", this),
      fZeroPoint(0)   
   {
      if(!pdf) throw std::invalid_argument("Pdf passed is null");
      SetData(*data);
      Init();
   }

   
   SumLikelihood(const SumLikelihood& rhs, const char* newName) :
      RooAbsReal(rhs, newName),
      fPdf(rhs.fPdf),
//      fParameters("params", "parameters", this),
      fZeroPoint(rhs.fZeroPoint) // XXX: or 0?
   {
      SetData(*rhs.fData);
      Init();
   } 

   virtual TObject* clone(const char* name) const { return new SumLikelihood(*this, name); }

   virtual Double_t evaluate() const {
      std::fill( fPartialSums.begin(), fPartialSums.end(), 0.0);
      
      std::vector<RooAbsReal*>::const_iterator itCoef, begCoef = fCoefficients.begin(), endCoef = fCoefficients.end();
      std::vector<CachedPdf>::iterator itPdf = fCachedPdfs.begin();
      std::vector<Double_t>::const_iterator itWgt, begWgt = fWeights.begin();
      std::vector<Double_t>::iterator itSum, begSum = fPartialSums.begin(), endSum = fPartialSums.end();


//      std::cout << "SumLikelihood::evaluate " << fCoefficients.size() << " "
  //              << fCachedPdfs.size() << " " << fWeights.size() << " "
    //            << fPartialSums.size() << std::endl;

      Double_t sumCoef = 0.0;
      for(itCoef = begCoef ; itCoef != endCoef; ++itPdf, ++itCoef) {
         Double_t coef = (*itCoef)->getVal();
         if (fIsRooAddPdf) {
            sumCoef += coef;
//            std::cout << "coef " << coef << "sumCoef " << sumCoef << std::endl;
         } else { // RooRealSum 
            sumCoef += coef * fIntegrals[itCoef - begCoef]->getVal();
         }

         // get pdf values

         const std::vector<Double_t>& pdfVals = (*itPdf).Eval(*fData);
         std::vector<Double_t>::const_iterator itVal = pdfVals.begin();
//         std::cout << "SumLikelihood::evaluate -> fPartialSums::size = " << fPartialSums.size()
//                   << " pdfVals::size = " << pdfVals.size() << std::endl;
         for(itSum = begSum; itSum != endSum; ++itSum, ++itVal) {
//            std::cout << "pdf " << itPdf - fCachedPdfs.begin() << " value "
  //                    << itVal - pdfVals.begin() << " is " << *itVal << std::endl;
            *itSum += coef * (*itVal);
         }
      }


      // get final NLL
      Double_t result = 0.0;
      for(itSum = begSum, itWgt = begWgt; itSum != endSum; ++itSum, ++itWgt) {

         if(*itWgt == 0) continue;
         if(!std::isnormal(*itSum) || *itSum <= 0) {
            std::cerr << "WARNING: underflow to " << *itSum << " in " << GetName() << std::endl;
            // TODO: CachingSimNLL::noDeepLEE_
         }

         Double_t term = (*itSum <= 0 ? -9e9 : log( *itSum)); 
         // TODO: Determine the reason for -9e9
         // TODO: KAHAN_SUM
         //std::cout << "SumLikelihood::evaluate term " << term << " " << result <<  std::endl;
         result += term;
      }
      
      result = -result;

      Double_t expectedEvents = fIsRooAddPdf ? sumCoef : fPdf->getNorm(fData->get());
      if (expectedEvents <= 0) {
         // TODO: CachingSimNLL::noDeepLEE_ logEvalError
         expectedEvents = 1e-6;
      }

//      std::cout << expectedEvents << std::endl;

      result += expectedEvents - fSumWeights * log(expectedEvents);
//      result += fZeroPoint;
//      std::cout << "NLL " << result << "  PDF " << fPdf->getVal() << "  Data " 
  //              << fData->get()->getRealValue("CMS_hgg_mass") << " "
    //            << std::endl;

      return result;
   }

   virtual Double_t defaultErrorLevel() const { return 0.5; }
   Double_t SumWeights() const { return fSumWeights; }
   void SetZeroPoint() { fZeroPoint = -this->getVal(); setValueDirty(); }
   void ClearZeroPoint() { fZeroPoint = 0.0; setValueDirty(); } 

   // TODO: implement getObservables and getParameters
   virtual RooArgSet* getParameters(const RooArgSet*, Bool_t) const {
 //     std::cout << "SumLikelihood:getParameters " << std::endl;
 //   fPdf->getParameters(fData)->Print("");
      return fPdf->getParameters(fData);
   }
  

private:
   void Init() {

      // TODO: see where to put this exactly
      setOperMode(RooAbsArg::ADirty);

      const RooArgSet *observables = fData->get();
      // FIXME: clear integrals (do not think it is necessary)
      RooAddPdf* addPdf = NULL; RooRealSumPdf* sumPdf = NULL;

      if( (addPdf = dynamic_cast<RooAddPdf*>(fPdf)) != NULL) {

         fIsRooAddPdf = kTRUE;

         Int_t numPdfs = addPdf->pdfList().getSize();
         // XXX: not such a big problem
         fCoefficients.reserve(numPdfs);
         fCachedPdfs.reserve(numPdfs);

         TIterator *itPdf  = addPdf->pdfList().createIterator();
         TIterator *itCoef = addPdf->coefList().createIterator();
         
        // std::cout << addPdf->pdfList().getSize() << " " << addPdf->coefList().getSize() << std::endl;
         
         // TODO TODO : last coefficient problem
         for(Int_t i = 0; i < numPdfs; ++i) {
            RooAbsReal* coef = dynamic_cast<RooAbsReal*>(itCoef->Next());
            RooAbsPdf*  pdf  = dynamic_cast<RooAbsPdf* >(itPdf->Next() );
            assert(coef != NULL); assert(pdf != NULL);
            fCoefficients.push_back(coef);
            fCachedPdfs.push_back(CachedPdf(pdf, observables));
         }

      } else if ( (sumPdf = dynamic_cast<RooRealSumPdf*>(fPdf)) != NULL) {

         fIsRooAddPdf = kFALSE;

         Int_t numFuncs = sumPdf->funcList().getSize();
         fCoefficients.reserve(numFuncs);
         fCachedPdfs.reserve(numFuncs);
           
         TIterator *itFunc = sumPdf->funcList().createIterator();
         TIterator *itCoef = sumPdf->coefList().createIterator();
 
         for(Int_t i = 0; i < numFuncs; ++i) {
            RooAbsReal* coef = dynamic_cast<RooAbsReal*>(itCoef->Next());
            RooAbsReal* func = dynamic_cast<RooAbsReal*>(itFunc->Next() );
            fCoefficients.push_back(coef);
            fCachedPdfs.push_back(CachedPdf(func, observables));
            // TODO: insert, allocate properly
            fIntegrals.push_back(func->createIntegral(*observables));
         }
      } else {
         throw std::invalid_argument("ERROR: SumLikelihood");
      }
/*
      TIterator *itPar = fPdf->getParameters(*fData)->createIterator();
      for(TObject *obj = itPar->Next(); obj != NULL; obj = itPar->Next()) {
         RooRealVar *var = dynamic_cast<RooRealVar*>(obj);
         if(var != NULL) fParameters.add(*var);
      }
      delete itPar;

      // For DEBUG purposes
      std::cout << "--START--SumLikelihood::fParameters--" << std::endl;
         fParameters.Print("v");      
      std::cout << "---END---SumLikelihood::fParameters--" << std::endl;
*/
   }
 

   void SetData(const RooAbsData& data) {
      fData = &data;
      setValueDirty();
      fSumWeights = 0.0;

      Double_t numEntries = data.numEntries();
      fWeights.resize(numEntries);
      fPartialSums.resize(numEntries);
      
      for(Int_t i = 0; i < numEntries; ++i) {
         data.get(i);
         // TODO: check if double call to weight is costly
         fWeights[i] = data.weight();
         fPartialSums[i] = 0.0;
         fSumWeights += fWeights[i];
      }
/*
      std::cout << "SumLikelihood::SetData fPartialSums size " << fPartialSums.size() << std::endl;
      std::cout << "SumLikelihood::SetData fWeights size " << fWeights.size() << std::endl;
      std::cout << "SumLikelihood::SetData fSumWeights " << fSumWeights << std::endl;
      std::cout << "SumLikelihood::SetData numEntries " << numEntries << " fWeights " << fWeights.size() << std::endl; 
*/
      for(std::vector<CachedPdf>::iterator itPdf = fCachedPdfs.begin(), endPdf = fCachedPdfs.end();
         itPdf != endPdf; ++itPdf) (*itPdf).SetDataDirty();
      
   }

  // TODO: implement properly 
  // virtual RooArgSet* getObservables(const RooArgSet*, Bool_t) const {
  // }
   
   RooAbsPdf* fPdf;
   const RooAbsData *fData;
   
   //RooSetProxy fParameters;

   std::vector<Double_t> fWeights;
   mutable std::vector<CachedPdf> fCachedPdfs;
   std::vector<RooAbsReal*> fIntegrals;
   mutable std::vector<Double_t> fPartialSums;
   std::vector<RooAbsReal*> fCoefficients;
   Double_t fZeroPoint;
   Double_t fSumWeights;
   Bool_t   fIsRooAddPdf;
};


namespace RooStats {

   class CombinedLikelihood : public RooAbsReal {
   public:
      // Constructors, destructors, assignment
      CombinedLikelihood(const char* name, const char* title, RooArgList& pdfList);
      CombinedLikelihood(const CombinedLikelihood& rhs, const char *newName = NULL);
      CombinedLikelihood(const RooSimultaneous& simPdf, const RooAbsData& data, const RooLinkedList& cmdList);
      virtual TObject *clone(const char *newName) const { return new CombinedLikelihood(*this, newName); }
      virtual ~CombinedLikelihood();

      RooAbsPdf* GetPdf(const std::string& catName) const; // considering deleting it
      // returns kTRUE if adding the pdf is succesful, kFALSE otherwise
      Bool_t AddPdf(RooAbsPdf& pdf, const std::string& catLabel);
      RooDataSet *GenerateGlobalObs(const RooArgSet& vars, Int_t nEvents);
      virtual Double_t evaluate() const;

      //void SetData(const RooAbsData& data) {}
      
      // TODO: implement properly
      virtual RooArgSet* getParameters(const RooArgSet*, Bool_t) const {
        // std::cout << "CombinedLikelihood::getParameters" << std::endl;
       //  fPdf->getParameters(fData)->Print("v");
         return fPdf->getParameters(fData);
      }

   private:
      CombinedLikelihood& operator=(const CombinedLikelihood& rhs); // disallow default assignment operator

      const RooSimultaneous* fPdf;
      const RooAbsData* fData;
      const RooArgSet* fNuisanceParameters;

      Int_t fNumberOfChannels;
      Int_t fNumberOfConstraints;

      std::vector<RooAbsPdf*> fChannels;
      std::vector<RooAbsData*> fDataSets;
    
      std::vector<RooAbsPdf*> fConstraints;
      RooSetProxy fConstraintParameters;
      std::vector<Double_t> fConstraintZeroPoints;
      std::vector<std::string> fChannelNames;
      std::vector<RooAbsReal*> fChannelLikelihoods;

   protected:
      ClassDef(CombinedLikelihood,1)
   };
}

#endif // ROOSTATS_CombinedLikelihood

