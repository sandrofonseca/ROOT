// @(#)root/roostats:$Id$

/*************************************************************************
 * Project: RooStats                                                     *
 * Package: RooFit/RooStats                                              *
 * Authors:                                                              *
 *   Kyle Cranmer, Lorenzo Moneta, Gregory Schott, Wouter Verkerke       *
 * Other author of this class: Danilo Piparo                             *
 *************************************************************************
 * Copyright (C) 1995-2008, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

//_________________________________________________________________
/**
HybridCalculator class: this class is a fresh rewrite in RooStats of
	RooStatsCms/LimitCalculator developped by D. Piparo and G. Schott
Authors: D. Piparo, G. Schott - Universitaet Karlsruhe

The class is born from the need to have an implementation of the CLs 
method that could take advantage from the RooFit Package.
The basic idea is the following: 
- Instantiate an object specifying a signal+background model, a background model and a dataset.
- Perform toy MC experiments to know the distributions of -2lnQ 
- Calculate the CLsb and CLs values as "integrals" of these distributions.

The class allows the user to input models as RooAbsPdf ( TH1 object could be used 
by using the RooHistPdf class)
The pdfs must be "extended": for more information please refer to 
http://roofit.sourceforge.net). The dataset can be entered as a 
RooAbsData objects.  

Unlike the TLimit Class a complete MC generation is performed at each step 
and not a simple Poisson fluctuation of the contents of the bins.
Another innovation is the treatment of the nuisance parameters. The user 
can input in the constructor nuisance parameters.
To include the information that we have about the nuisance parameters a prior
PDF (RooAbsPdf) should be specified

Different test statistic can be used (likelihood ratio, number of events or 
profile likelihood ratio. The default is the likelihood ratio. 
See the method SetTestStatistic.

The number of toys to be generated is controlled by SetNumberOfToys(n).

The result of the calculations is returned as a HybridResult object pointer.

see also the following interesting references:
- Alex Read, "Presentation of search results: the CLs technique",
  Journal of Physics G: Nucl. Part. Phys. 28 2693-2704 (2002).
  see http://www.iop.org/EJ/abstract/0954-3899/28/10/313/

- Alex Read, "Modified Frequentist Analysis of Search Results (The CLs Method)" CERN 2000-005 (30 May 2000)

- V. Bartsch, G.Quast, "Expected signal observability at future experiments" CMS NOTE 2005/004

- http://root.cern.ch/root/html/src/TLimit.html
*/


#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooGlobalFunc.h"
#include "RooNLLVar.h"
#include "RooRealVar.h"
#include "RooAbsData.h"
#include "RooWorkspace.h"
#include "RooFunctor.h"
#include "RooRandom.h"

#include "TH1.h"

#include "RooStats/HybridCalculator.h"

#include "TF3.h"
#include "TRandom.h"
#include "Fit/UnBinData.h"
#include "Fit/BinData.h"
#include "Fit/LogLikelihoodFCN.h"
#include "Fit/PoissonLikelihoodFCN.h"
#include "Math/WrappedParamFunction.h"
#include "Math/DistSampler.h"
#include "Math/Factory.h"
// #include "TUnuran.h"
// #include "TUnuranContDist.h"

#include "TFile.h"
#include "TCanvas.h"

//#define DEBUG

ClassImp(RooStats::HybridCalculator)

using namespace RooStats;

// template<int N>
// double EvalLL(ROOT::Fit::UnBinData & data, RooFunctor * model, int dim) { 
//    ROOT::Math::WrappedParamFunction<RooFunctor *> wf(model,dim);
//    ROOT::Fit::LogLikelihoodFunction nll(data, wf);
//    return nll(0);
// }

///////////////////////////////////////////////////////////////////////////

HybridCalculator::HybridCalculator(const char *name) :
   TNamed(name,name),
   fSbModel(0),
   fBModel(0),
   fObservables(0),
   fNuisanceParameters(0),
   fPriorPdf(0),
   fData(0),
   fUsePriorPdf(false)
{
   // constructor with name and title
   // set default parameters
   SetTestStatistic(1); 
   SetNumberOfToys(1000); 
}


/// constructor without the data - is it needed ???????????
HybridCalculator::HybridCalculator( RooAbsPdf& sbModel,
                                    RooAbsPdf& bModel,
                                    RooArgList& observables,
                                    const RooArgSet* nuisance_parameters,
                                    RooAbsPdf* priorPdf ,
				    bool GenerateBinned,
                                    int testStatistics, 
                                    int numToys) :
   fSbModel(&sbModel),
   fBModel(&bModel),
   fNuisanceParameters(nuisance_parameters),
   fPriorPdf(priorPdf),
   fData(0),
   fGenerateBinned(GenerateBinned),
   fUsePriorPdf(false)
{
   /// HybridCalculator constructor without specifying a data set
   /// the user need to specify the models in the S+B case and B-only case,
   /// the list of observables of the model(s) (for MC-generation), the list of parameters 
   /// that are marginalised and the prior distribution of those parameters

   // observables are managed by the class (they are copied in) 
  fObservables = new RooArgList(observables);
  //Try to recover the informations from the pdf's
  //fObservables=new RooArgList("fObservables");
  //fNuisanceParameters=new RooArgSet("fNuisanceParameters");
  // if (priorPdf){
      

  SetTestStatistic(testStatistics); 
  SetNumberOfToys(numToys); 

  if (priorPdf) UseNuisance(true); 
  
   // this->Print();
   /* if ( _verbose ) */ //this->PrintMore("v"); /// TO DO: add the verbose mode
}


HybridCalculator::HybridCalculator( RooAbsData & data, 
                                    RooAbsPdf& sbModel,
                                    RooAbsPdf& bModel,
                                    const RooArgSet* nuisance_parameters,
                                    RooAbsPdf* priorPdf,
				    bool GenerateBinned,
                                    int testStatistics, 
                                    int numToys) :
   fSbModel(&sbModel),
   fBModel(&bModel),
   fObservables(0),
   fNuisanceParameters(nuisance_parameters),
   fPriorPdf(priorPdf),
   fData(&data),
   fGenerateBinned(GenerateBinned),
   fUsePriorPdf(false)
{
   /// HybridCalculator constructor for performing hypotesis test 
   /// the user need to specify the data set, the models in the S+B case and B-only case. 
   /// In case of treatment of nuisance parameter, the user need to specify the  
   /// the list of parameters  that are marginalised and the prior distribution of those parameters


   SetTestStatistic(testStatistics);
   SetNumberOfToys(numToys); 

   if (priorPdf) UseNuisance(true); 
}



HybridCalculator::HybridCalculator( RooAbsData& data, 
                                    const ModelConfig& sbModel, 
                                    const ModelConfig& bModel, 
				    bool GenerateBinned,
                                    int testStatistics, 
                                    int numToys) :
   fSbModel(sbModel.GetPdf()),
   fBModel(bModel.GetPdf()),
   fObservables(0),  // no need to set them - can be taken from the data
   fNuisanceParameters((sbModel.GetNuisanceParameters()) ? sbModel.GetNuisanceParameters()  :  bModel.GetNuisanceParameters()),
   fPriorPdf((sbModel.GetPriorPdf()) ? sbModel.GetPriorPdf()  :  bModel.GetPriorPdf()),
   fData(&data),
   fGenerateBinned(GenerateBinned),
   fUsePriorPdf(false)
{
  /// Constructor with a ModelConfig object representing the signal + background model and 
  /// another model config representig the background only model
  /// a Prior pdf for the nuiscane parameter of the signal and background can be specified in 
  /// the s+b model or the b model. If it is specified in the s+b model, the one of the s+b model will be used 

  if (fPriorPdf) UseNuisance(true);

  SetTestStatistic(testStatistics);
  SetNumberOfToys(numToys); 
}

///////////////////////////////////////////////////////////////////////////

HybridCalculator::~HybridCalculator()
{
   /// HybridCalculator destructor
   if (fObservables) delete fObservables; 
}

///////////////////////////////////////////////////////////////////////////

void HybridCalculator::SetNullModel(const ModelConfig& model)
{
   // Set the model describing the null hypothesis
   fBModel = model.GetPdf();
   // only if it has not been set before
   if (!fPriorPdf) fPriorPdf = model.GetPriorPdf(); 
   if (!fNuisanceParameters) fNuisanceParameters = model.GetNuisanceParameters(); 
}

void HybridCalculator::SetAlternateModel(const ModelConfig& model)
{
   // Set the model describing the alternate hypothesis
   fSbModel = model.GetPdf();
   fPriorPdf = model.GetPriorPdf(); 
   fNuisanceParameters = model.GetNuisanceParameters(); 
}

void HybridCalculator::SetTestStatistic(int index)
{
   /// set the desired test statistics:
   /// index=1 : likelihood ratio: 2 * log( L_sb / L_b )  (DEFAULT)
   /// index=2 : number of generated events
   /// index=3 : profiled likelihood ratio
   /// if the index is different to any of those values, the default is used
   fTestStatisticsIdx = index;
}

///////////////////////////////////////////////////////////////////////////

HybridResult* HybridCalculator::Calculate(TH1& data, unsigned int nToys, bool usePriors) const
{
   /// first compute the test statistics for data and then prepare and run the toy-MC experiments

   /// convert data TH1 histogram to a RooDataHist
   TString dataHistName = GetName(); dataHistName += "_roodatahist";
   RooDataHist dataHist(dataHistName,"Data distribution as RooDataHist converted from TH1",*fObservables,&data);

   HybridResult* result = Calculate(dataHist,nToys,usePriors);

   return result;
}

///////////////////////////////////////////////////////////////////////////

HybridResult* HybridCalculator::Calculate(RooAbsData& data, unsigned int nToys, bool usePriors) const
{
   /// first compute the test statistics for data and then prepare and run the toy-MC experiments

   double testStatData = 0;
   if ( fTestStatisticsIdx==2 ) {
      /// number of events used as test statistics
      double nEvents = data.sumEntries();
      testStatData = nEvents;
   } else if ( fTestStatisticsIdx==3 ) {
      /// profiled likelihood ratio used as test statistics
      RooNLLVar sb_nll("sb_nll","sb_nll",*fSbModel,data,RooFit::Extended());
      fSbModel->fitTo(data);
      double sb_nll_val = sb_nll.getVal();
      RooNLLVar b_nll("b_nll","b_nll",*fBModel,data,RooFit::Extended());
      fBModel->fitTo(data);
      double b_nll_val = b_nll.getVal();
      double m2lnQ = 2*(sb_nll_val-b_nll_val);
      testStatData = m2lnQ;
   } else if ( fTestStatisticsIdx==1 ) {
      /// likelihood ratio used as test statistics (default)
      bool extLL = fUseExtendLL; 
      if (fGenerateBinned) extLL = false; 
      RooNLLVar sb_nll("sb_nll","sb_nll",*fSbModel,data,RooFit::Extended(extLL));
      RooNLLVar b_nll("b_nll","b_nll",*fBModel,data,RooFit::Extended(extLL));
      double m2lnQ = 2*(sb_nll.getVal()-b_nll.getVal());
      testStatData = m2lnQ;
#ifdef DEBUG
      double nEvents = data.sumEntries();
      std::cout << "Test Statistics on the data n = " << nEvents << std::endl;
      std::cout << "Exp: " << fBModel->extendedTerm( nEvents ) 
                << "  " << fSbModel->extendedTerm( nEvents ) << std::endl;
      std::cout << "nll: " << b_nll.getVal() << "  " << sb_nll.getVal() << std::endl;
      std::cout << "2lr " << m2lnQ << std::endl; 
#endif
   }

   HybridResult* result = Calculate(nToys,usePriors);
   if (result) result->SetDataTestStatistics(testStatData);

   return result;
}

///////////////////////////////////////////////////////////////////////////

HybridResult* HybridCalculator::Calculate(unsigned int nToys, bool usePriors) const
{
   std::vector<double> bVals;
   bVals.reserve(nToys);

   std::vector<double> sbVals;
   sbVals.reserve(nToys);

   //RunToys(bVals,sbVals,nToys,usePriors);
   RunToysFast(bVals,sbVals,nToys,usePriors);

   // check returned vectirs 
   if (bVals.size() == 0 || bVals.size() != sbVals.size() ) {
      std::cout << "HybridCalculator::Calculate - error from running toys - return empty result\n"; 
      return 0; 
   }

   HybridResult* result;

   TString name = "HybridResult_" + TString(GetName() );

   if ( fTestStatisticsIdx==2 )
     result = new HybridResult(name,sbVals,bVals,false);
   else 
     result = new HybridResult(name,sbVals,bVals);

   return result;
}

////////////////////////////////////////////////////////////////
double HybridCalculator::EvalNLL(const ROOT::Fit::FitData & data, const ROOT::Math::IParamMultiFunction & func) const
{
   // fast evaluation of nll using ROOT 
   if (fGenerateBinned) { 
      ROOT::Fit::PoissonLLFunction nll((const ROOT::Fit::BinData &)data, func);
      return nll(0); 
   } 
   else { 
      ROOT::Fit::LogLikelihoodFunction nll((const ROOT::Fit::UnBinData &)data, func);
      return nll(0);
   }
         
}
///////////////////////////////////////////////////////////////////////////
void HybridCalculator::RunToysFast(std::vector<double>& bVals, std::vector<double>& sbVals, unsigned int nToys, bool usePriors) const
{ 
   // new routine to run toys using directly ROOT classes 
   /// do the actual run-MC processing
   std::cout << "HybridCalculator: run " << nToys << " toy-MC experiments\n";
   std::cout << "with test statistics index: " << fTestStatisticsIdx << "\n";
   if (usePriors) std::cout << "marginalize nuisance parameters \n";


   assert(nToys > 0);
   assert(fBModel);
   assert(fSbModel);
   if (usePriors)  { 
      assert(fPriorPdf); 
      assert(fNuisanceParameters);
   }


   /// backup the initial values of the parameters that are varied by the prior MC-integration
   int nParameters = (fNuisanceParameters) ? fNuisanceParameters->getSize() : 0;
   RooArgList parametersList("parametersList");  /// transforms the RooArgSet in a RooArgList (needed for .at())
   std::vector<double> parameterValues(nParameters); /// array to hold the initial parameter values
   if (usePriors && nParameters>0) {
      parametersList.add(*fNuisanceParameters);
      for (int iParameter=0; iParameter<nParameters; iParameter++) {
         RooRealVar* oneParam = (RooRealVar*) parametersList.at(iParameter);
         parameterValues[iParameter] = oneParam->getVal();
      }
   }

   // generate ntoys values from the prior pdf 


   int obsDim = fObservables->getSize(); 

   TRandom & rdm = *RooRandom::randomGenerator();  // needed for Poisson generation

//    // for debugging
#ifdef DEBUG
   std::vector<TH1D*> h_nuis(nParameters);
   for (int ipar = 0; ipar < nParameters; ++ipar) { 
      std::string name = "hnuis_" + std::string(parametersList[ipar].GetName() );
      RooRealVar* oneParam = (RooRealVar*) parametersList.at(ipar);
      h_nuis[ipar] = new TH1D(name.c_str(),name.c_str(),100,oneParam->getMin(),oneParam->getMax());
   }
#endif

   if (!fGenMethod.size()) fGenMethod = "Unuran";
   ROOT::Math::DistSampler * bsampler = ROOT::Math::Factory::CreateDistSampler(fGenMethod);
   ROOT::Math::DistSampler * sbsampler = ROOT::Math::Factory::CreateDistSampler(fGenMethod);
   if (!bsampler || !sbsampler ) {
      std::cerr << "HybridCalculator::RunToys: Error creating dist sampler for " 
                << fGenMethod << std::endl;
      return;
   }
   if (!fGenAlgo1.size()) { 
      //if (obsDim <=1) fGenAlgo1 = "NROU"; // for small statistics
      if (obsDim <=1) fGenAlgo1 = "PINV";  // for large generations
      else fGenAlgo1 = "VNROU";     
   }
   ROOT::Math::DistSampler * priorsampler = 0; 

   if (usePriors && nParameters>0)   { 
      // build a sampler of the nuisance parameters 
      priorsampler = ROOT::Math::Factory::CreateDistSampler(fGenMethod);
      assert(priorsampler);
      if (!fGenAlgo2.size()) { 
         if (nParameters <=1) fGenAlgo2 = "NROU";
         else fGenAlgo2 = "VNROU";
      }
      // create a functor class from prior pdf (i.e. a binding to oprerator()(double * p)
      RooFunctor * fprior = fPriorPdf->functor(*fNuisanceParameters,RooArgSet(),*fNuisanceParameters);
      priorsampler->SetFunction<RooFunctor>(*fprior,nParameters);
      ROOT::Fit::DataRange parRange(nParameters); 
      for (int i = 0; i < nParameters; ++i) {
         RooRealVar & p = (RooRealVar&) parametersList[i];
         parRange.SetRange(i, p.getMin(), p.getMax() );
      } 
      priorsampler->SetRange(parRange);

      bool ok = priorsampler->Init(fGenAlgo2.c_str());
      if (!ok) {
         std::cerr << "HybridCalculator::RunToys: Error initializing priorsampler " 
                   << " using " << fGenAlgo2 << std::endl;
         return;
      }
      // reset the parameters (Init function change them)
      for (int iParameter=0; iParameter<nParameters; iParameter++) {
         RooRealVar* oneParam = (RooRealVar*) parametersList.at(iParameter);
         oneParam->setVal(parameterValues[iParameter]);
      }

   }



   // loop on toys 
   for (unsigned int iToy=0; iToy<nToys; iToy++) {
      /// prints a progress report every 500 iterations
      /// TO DO: add a global verbose flag
     if ( /*verbose && */ iToy%500==0 ) {
       std::cout << "....... toy number " << iToy << " / " << nToys << std::endl;
     }

     

      /// vary the value of the integrated parameters according to the prior pdf
      if (usePriors && nParameters>0) {
         const double * val = priorsampler->Sample();
         for (int i = 0; i < nParameters; ++i) { 
            ( (RooRealVar*) parametersList.at(i) )->setVal( val[i] );
#ifdef DEBUG         
            h_nuis[i]->Fill(val[i]);
#endif
         }
      }

      /// generate the dataset in the B-only hypothesis
      // they have to be normalized on the observables

      // need to get the observables from the pdf 
      // fObservables are a clone of the variables inside the pdf 
      RooArgSet * b_obs = fBModel->getObservables(fData); 
      RooFunctor * bmodel = fBModel->functor(*b_obs,RooArgSet(), *b_obs); 
      RooArgSet * sb_obs = fSbModel->getObservables(fData); 
      RooFunctor * sbmodel = fSbModel->functor(*sb_obs,RooArgSet(), *sb_obs); 

      //should not be b_obs and sb_obs be the same ??
      // get observables range (this can be outside the loop )
      ROOT::Fit::DataRange obsRange(obsDim); 
      std::vector<int> nObsBins(obsDim);
      for (int i = 0; i < obsDim; ++i) {
         RooRealVar & obs = (RooRealVar&) (*fObservables)[i];
         obsRange.SetRange(i, obs.getMin(), obs.getMax() );
         if (fGenerateBinned) { 
            nObsBins[i] = obs.getBins();
         }
      } 


      // number of expected events will depend also on background fraction 
      // does need to fluctuate according to systematics - yes ! 
      double nbexp = fBModel->expectedEvents(0);
      double nsbexp = fSbModel->expectedEvents(0);
      
      // generate the poisson fluctuations 
      unsigned int nbevt  = rdm.Poisson(nbexp);
      unsigned int nsbevt = rdm.Poisson(nsbexp);
#ifdef DEBUG
      if (iToy%500==0) {
         std::cout << "generate NB = " << nbevt << "  NSB = " << nsbevt << std::endl;
         std::cout << "expected NB = " << nbexp << "  NSB = " << nsbexp << std::endl;
      }
#endif


      if (fTestStatisticsIdx == 2) { 
         bVals.push_back(nbevt); 
         sbVals.push_back(nsbevt); 
         /// restore the parameters to their initial values before evaluating
         // this should be done or NOT ? 
         if (usePriors && nParameters>0) {
            for (int iParameter=0; iParameter<nParameters; iParameter++) {
               RooRealVar* oneParam = (RooRealVar*) parametersList.at(iParameter);
               oneParam->setVal(parameterValues[iParameter]);
            }
         }

         continue; 
      }
     
      // create here  the data sets      
      ROOT::Fit::FitData * bdata = 0; 
      ROOT::Fit::FitData * sbdata = 0; 

      if (fGenerateBinned) { 
         // bin numbers will be set later when generating
         bdata = new ROOT::Fit::BinData(0, obsDim, ROOT::Fit::BinData::kNoError);
         sbdata = new ROOT::Fit::BinData(0, obsDim, ROOT::Fit::BinData::kNoError);
      } 
      else {
         bdata = new ROOT::Fit::UnBinData(nbevt, obsDim);
         sbdata = new ROOT::Fit::UnBinData(nsbevt, obsDim);
      }



      bsampler->SetFunction<RooFunctor>(*bmodel, obsDim );
      sbsampler->SetFunction<RooFunctor>(*sbmodel, obsDim );

      // set ranges have been set before (need to set only first time) 
      bsampler->SetRange(obsRange);
      sbsampler->SetRange(obsRange);

      // generate Events 
      if (fGenerateBinned) { 
         // should use expected events since sum will give POisson fluct ? 
         bsampler->Generate(nbexp,&nObsBins.front(),(ROOT::Fit::BinData&)*bdata); 
         sbsampler->Generate(nsbexp,&nObsBins.front(),(ROOT::Fit::BinData&)*sbdata); 
      }
      else { 
         // unbin generation (need to initialize)
         bool ok = bsampler->Init(fGenAlgo1.c_str());
         if (!ok) {
            std::cerr << "HybridCalculator::RunToys: Error initializing priorsampler " 
                      << " using " << fGenAlgo1 << std::endl;
            return;
      }
         
         ok = sbsampler->Init(fGenAlgo1.c_str());
         if (!ok) {
            std::cerr << "HybridCalculator::RunToys: Error initializing priorsampler " 
                      << " using " << fGenAlgo1 << std::endl;
            return;
         }

         bsampler->Generate(nbevt,(ROOT::Fit::UnBinData&)*bdata); 
         sbsampler->Generate(nsbevt,(ROOT::Fit::UnBinData&)*sbdata); 
      }

      /// restore the parameters to their initial values before evaluating
      // since test statistics must be evaluiated at known value of parameters
      if (usePriors && nParameters>0) {
         for (int iParameter=0; iParameter<nParameters; iParameter++) {
            RooRealVar* oneParam = (RooRealVar*) parametersList.at(iParameter);
            oneParam->setVal(parameterValues[iParameter]);
         }
      }
#ifdef DEBUG
      if (iToy%500==0) {
         nbexp = fBModel->expectedEvents(0);
         nsbexp = fSbModel->expectedEvents(0);
         std::cout << "expected NB = " << nbexp << "  NSB = " << nsbexp << std::endl;
      }
#endif


      // now evaluate the likelihoods 
      ROOT::Math::WrappedParamFunction<RooFunctor *> wf1(bmodel,obsDim);
      ROOT::Math::WrappedParamFunction<RooFunctor *> wf2(sbmodel,obsDim);

      // evaluate the likelihood's (need to add extended term) in case of unbinned data
      // for binned data (it is used a Poisson likelihood so no need to add it) 
      //bool extendLL = !fGenerateBinned; 
      bool extendLL = fUseExtendLL; 
      if (fGenerateBinned) extendLL = false; 

      // evaluate on the s+b data
      {
         double b_nll_val = EvalNLL(*sbdata, wf1);
         double sb_nll_val = EvalNLL(*sbdata, wf2);         
         if (extendLL) { 
            b_nll_val += fBModel->extendedTerm( nsbevt );
            sb_nll_val += fSbModel->extendedTerm( nsbevt );
         }
         
         double m2lnQ = 2*(sb_nll_val-b_nll_val);
         sbVals.push_back(m2lnQ);

#ifdef DEBUG
         if (iToy%500==0) {
            std::cout << "S+B\n";
            if (extendLL) { 
               double bnll = b_nll_val-fBModel->extendedTerm( nsbevt );
               double sbnll = sb_nll_val-fSbModel->extendedTerm( nsbevt );
               std::cout << "nll0: " << bnll << "  " << sbnll << std::endl;
               std::cout << "Exp: " << fBModel->extendedTerm( nsbevt ) 
                         << "  " << fSbModel->extendedTerm( nsbevt ) << std::endl;
            }
            std::cout << "nll" << b_nll_val << "  " << sb_nll_val << std::endl;
            std::cout << "2lr " << m2lnQ << std::endl; 
         }            
#endif


      }
      // evaluate on the b only data
      {
         double b_nll_val = EvalNLL(*bdata, wf1);
         double sb_nll_val = EvalNLL(*bdata, wf2);         
         if (extendLL) { 
            b_nll_val += fBModel->extendedTerm( nbevt );
            sb_nll_val += fSbModel->extendedTerm( nbevt );
         }
         double m2lnQ = 2*(sb_nll_val-b_nll_val);
         bVals.push_back(m2lnQ);

#ifdef DEBUG
         if (iToy%500==0) {
            std::cout << "B\n";
            if (extendLL) { 
               double bnll = b_nll_val-fBModel->extendedTerm( nbevt );
               double sbnll = sb_nll_val-fSbModel->extendedTerm( nbevt );
               std::cout << "nll: " << bnll << "  " << sbnll << std::endl;
               std::cout << "Exp: " << fBModel->extendedTerm( nbevt ) 
                         << "  " << fSbModel->extendedTerm( nbevt ) << std::endl;
            }
            std::cout << b_nll_val << "  " << sb_nll_val << std::endl;
            std::cout << "2lr " << m2lnQ << std::endl; 
         }  
#endif          
         
      }

      // debug 
#undef DEBUG
#ifdef DEBUG      
      // debug every 500 events
      if (iToy%500==0) {



         TString fileName;
         fileName.Form("hybrid_%d.root",iToy);
         TFile * file = new TFile(fileName,"RECREATE");
         // create and fill histogram for generate B and S+B data (only for 1D observables)
         RooRealVar * x = (RooRealVar *) fObservables->at(0);
         TH1D * hsb = new TH1D("h_sb","sb data",100,x->getMin(), x->getMax() );
         TH1D * hb  = new TH1D("h_b","b data",100,x->getMin(), x->getMax() );

//          RooRealVar * par0 = (RooRealVar *) parametersList->at(0); 
//          TH1D * hp  = new TH1D("prior","prior func",100,par0->getMin(),par0->getMax());


         
         for (unsigned int i = 0; i < sbdata.Size(); ++ i) {             
            hsb->Fill(*(sbdata.Coords(i)) );
         }
         for (unsigned int i = 0; i < bdata.Size(); ++ i) 
            hb->Fill(*(bdata.Coords(i)) );

         
         file->Write();
         file->Close();

         
         std::cout << "S+B parameters , nexp = " << nsbexp << std::endl;
         fSbModel->getParameters(*fObservables)->Print("v");
         std::cout << "B parameters , nexp = " << nbexp  << std::endl;
         fBModel->getParameters(*fObservables)->Print("v");
         std::cout << "nuis parameter values " << std::endl;
         parametersList.Print("v");

         // check values using RooFit
         RooDataSet roo_sb("SB roodata set","roo",*fObservables);
         RooDataSet roo_b("B roodata set","roo",*fObservables);
         for (unsigned int i = 0; i < sbdata.Size(); ++ i) {
            x->setVal(*(sbdata.Coords(i)) );
            roo_sb.add(RooArgSet(*x) );
         }
         for (unsigned int i = 0; i < bdata.Size(); ++ i) {
            x->setVal(*(bdata.Coords(i)) );
            roo_b.add(RooArgSet(*x) );
         }

         std::cout << "\nNew Values obtained\n";
         std::cout << "s+b Val = " << sbVals.back() 
                   << "\nb Val  = " << bVals.back() << std::endl;

         // evaluate now the likelihood
         std::cout << "RooFit values\n";
         {
         RooNLLVar sb_nll("sb_nll","sb_nll",*fSbModel,roo_sb,RooFit::Extended());
         RooNLLVar b_nll("b_nll","b_nll",*fBModel,roo_sb,RooFit::Extended());
          std::cout << "ROO sb data :  sb_nll = " << sb_nll.getVal() << " b_nll = " << b_nll.getVal() << std::endl;
         double roo_m2lnQ = 2*(sb_nll.getVal()-b_nll.getVal());
         std::cout << " Roofit s+b = " << roo_m2lnQ << std::endl;
         }
         {
            //RooNLLVar sb_nll("sb_nll","sb_nll",*fSbModel,roo_b);
         RooNLLVar sb_nll("sb_nll","sb_nll",*fSbModel,roo_b,RooFit::Extended());
         RooNLLVar b_nll("b_nll","b_nll",*fBModel,roo_b,RooFit::Extended());
          std::cout << "ROO b data :  sb_nll = " << sb_nll.getVal() << " b_nll = " << b_nll.getVal() << std::endl;
         double roo_m2lnQ = 2*(sb_nll.getVal()-b_nll.getVal());
         std::cout << " Roofit  b  = " << roo_m2lnQ << std::endl;
         }

      }
#endif


      // delete data clases 
      delete bdata; 
      delete sbdata; 


   } // end toy loop 

   if (bsampler) delete bsampler; 
   if (sbsampler) delete sbsampler;

#ifdef DEBUG
   TFile * file_nuis = new TFile("hybrid_nuisDistribution.root","RECREATE");
   for (unsigned int i = 0; i < h_nuis.size(); ++i) 
      h_nuis[i]->Write();
   file_nuis->Close();
#endif


   return;


}


void HybridCalculator::RunToys(std::vector<double>& bVals, std::vector<double>& sbVals, unsigned int nToys, bool usePriors) const
{
   /// do the actual run-MC processing
   std::cout << "HybridCalculator: run " << nToys << " toy-MC experiments\n";
   std::cout << "with test statistics index: " << fTestStatisticsIdx << "\n";
   if (usePriors) std::cout << "marginalize nuisance parameters \n";

   assert(nToys > 0);
   assert(fBModel);
   assert(fSbModel);
   if (usePriors)  { 
      assert(fPriorPdf); 
      assert(fNuisanceParameters);
   }

   /// backup the initial values of the parameters that are varied by the prior MC-integration
   int nParameters = (fNuisanceParameters) ? fNuisanceParameters->getSize() : 0;
   std::vector<double> parameterValues(nParameters); /// array to hold the initial parameter values
   RooArgList parametersList("parametersList");  /// transforms the RooArgSet in a RooArgList (needed for .at())
   if (usePriors && nParameters>0) {
      parametersList.add(*fNuisanceParameters);
      for (int iParameter=0; iParameter<nParameters; iParameter++) {
         RooRealVar* oneParam = (RooRealVar*) parametersList.at(iParameter);
         parameterValues[iParameter] = oneParam->getVal();
      }
   }

   double nuis_value = 100;

   for (unsigned int iToy=0; iToy<nToys; iToy++) {

      /// prints a progress report every 500 iterations
      /// TO DO: add a global verbose flag
     if ( /*verbose && */ iToy%500==0 ) {
       std::cout << "....... toy number " << iToy << " / " << nToys << std::endl;
     }

      /// vary the value of the integrated parameters according to the prior pdf
      if (usePriors && nParameters>0) {
         /// generation from the prior pdf (TO DO: RooMCStudy could be used here)
         RooDataSet* tmpValues = (RooDataSet*) fPriorPdf->generate(*fNuisanceParameters,1);
         for (int iParameter=0; iParameter<nParameters; iParameter++) {
            RooRealVar* oneParam = (RooRealVar*) parametersList.at(iParameter);
            oneParam->setVal(tmpValues->get()->getRealValue(oneParam->GetName()));
            nuis_value = oneParam->getVal();
         }
         delete tmpValues;
      }

      /// generate the dataset in the B-only hypothesis
      RooAbsData* bData;
      if (fGenerateBinned)
	bData = static_cast<RooAbsData*> (fBModel->generateBinned(*fObservables,RooFit::Extended()));	
      else 
	bData = static_cast<RooAbsData*> (fBModel->generate(*fObservables,RooFit::Extended()));

      /// work-around in case of an empty dataset (TO DO: need a debug in RooFit?)
      bool bIsEmpty = false;
      if (bData==NULL) {
         bIsEmpty = true;
         // if ( _verbose ) std::cout << "empty B-only dataset!\n";
         RooDataSet* bDataDummy=new RooDataSet("bDataDummy","empty dataset",*fObservables);
         bData = static_cast<RooAbsData*>(new RooDataHist ("bDataEmpty","",*fObservables,*bDataDummy));
         delete bDataDummy;
      }

      /// generate the dataset in the S+B hypothesis
      RooAbsData* sbData;
      if (fGenerateBinned)    
	sbData = static_cast<RooAbsData*> (fSbModel->generateBinned(*fObservables,RooFit::Extended()));
      else
	sbData = static_cast<RooAbsData*> (fSbModel->generate(*fObservables,RooFit::Extended()));

      /// work-around in case of an empty dataset (TO DO: need a debug in RooFit?)
      bool sbIsEmpty = false;
      if (sbData==NULL) {
         sbIsEmpty = true;
         // if ( _verbose ) std::cout << "empty S+B dataset!\n";
         RooDataSet* sbDataDummy=new RooDataSet("sbDataDummy","empty dataset",*fObservables);
         sbData = static_cast<RooAbsData*>(new RooDataHist ("sbDataEmpty","",*fObservables,*sbDataDummy));
         delete sbDataDummy;
      }

      /// restore the parameters to their initial values
      if (usePriors && nParameters>0) {
         for (int iParameter=0; iParameter<nParameters; iParameter++) {
            RooRealVar* oneParam = (RooRealVar*) parametersList.at(iParameter);
            oneParam->setVal(parameterValues[iParameter]);
         }
      }

      /// evaluate the test statistic in the S+B case
      if ( fTestStatisticsIdx==2 ) {
         /// number of events used as test statistics
         double nEvents = 0;
         if ( !sbIsEmpty ) nEvents = sbData->numEntries();
         sbVals.push_back(nEvents);
      } else if ( fTestStatisticsIdx==3 ) {
         /// profiled likelihood ratio used as test statistics
         RooNLLVar sb_nll("sb_nll","sb_nll",*fSbModel,*sbData,RooFit::Extended());
         fSbModel->fitTo(*sbData);
         double sb_nll_val = sb_nll.getVal();
         RooNLLVar b_nll("b_nll","b_nll",*fBModel,*sbData,RooFit::Extended());
         fBModel->fitTo(*sbData);
         double b_nll_val = b_nll.getVal();
         double m2lnQ = 2*(sb_nll_val-b_nll_val);
         sbVals.push_back(m2lnQ);
      } else if ( fTestStatisticsIdx==1 ) {
         /// likelihood ratio used as test statistics (default)
         RooNLLVar sb_nll("sb_nll","sb_nll",*fSbModel,*sbData,RooFit::Extended());
         RooNLLVar b_nll("b_nll","b_nll",*fBModel,*sbData,RooFit::Extended());
         double m2lnQ = 2*(sb_nll.getVal()-b_nll.getVal());
         sbVals.push_back(m2lnQ);
      }

      /// evaluate the test statistic in the B-only case
      if ( fTestStatisticsIdx==2 ) {
         /// number of events used as test statistics
         double nEvents = 0;
         if ( !bIsEmpty ) nEvents = bData->numEntries();
         bVals.push_back(nEvents);
      } else if ( fTestStatisticsIdx==3 ) {
         /// profiled likelihood ratio used as test statistics
         RooNLLVar sb_nll("sb_nll","sb_nll",*fSbModel,*bData,RooFit::Extended());
         fSbModel->fitTo(*bData);
         double sb_nll_val = sb_nll.getVal();
         RooNLLVar b_nll("b_nll","b_nll",*fBModel,*bData,RooFit::Extended());
         fBModel->fitTo(*bData);
         double b_nll_val = b_nll.getVal();
         double m2lnQ = 2*(sb_nll_val-b_nll_val);
         bVals.push_back(m2lnQ);
      } else if ( fTestStatisticsIdx==1 ) {
         /// likelihood ratio used as test statistics (default)
         RooNLLVar sb_nll("sb_nll","sb_nll",*fSbModel,*bData,RooFit::Extended());
         RooNLLVar b_nll("b_nll","b_nll",*fBModel,*bData,RooFit::Extended());
         double m2lnQ = 2*(sb_nll.getVal()-b_nll.getVal());
         bVals.push_back(m2lnQ);

      }



      /// delete the toy-MC datasets
      delete sbData;
      delete bData;

   } /// end of loop over toy-MC experiments


   /// restore the parameters to their initial values (for safety) and delete the array of values
   if (usePriors && nParameters>0) {
      for (int iParameter=0; iParameter<nParameters; iParameter++) {
         RooRealVar* oneParam = (RooRealVar*) parametersList.at(iParameter);
         oneParam->setVal(parameterValues[iParameter]);
      }
   }

   return;
}

///////////////////////////////////////////////////////////////////////////

void HybridCalculator::PrintMore(const char* options) const
{
   /// Print out some information about the input models

   if (fSbModel) { 
      std::cout << "Signal plus background model:\n";
      fSbModel->Print(options);
   }

   if (fBModel) { 
      std::cout << "\nBackground model:\n";
      fBModel->Print(options);
   }
      
   if (fObservables) {  
      std::cout << "\nObservables:\n";
      fObservables->Print(options);
   }

   if (fNuisanceParameters) { 
      std::cout << "\nParameters being integrated:\n";
      fNuisanceParameters->Print(options);
   }

   if (fPriorPdf) { 
      std::cout << "\nPrior PDF model for integration:\n";
      fPriorPdf->Print(options);
   }

   return;
}
///////////////////////////////////////////////////////////////////////////
// implementation of inherited methods from HypoTestCalculator

HybridResult* HybridCalculator::GetHypoTest() const {  
   // perform the hypothesis test and return result of hypothesis test 

   // check first that everything needed is there 
   if (!DoCheckInputs()) return 0;  
   RooAbsData * treeData = dynamic_cast<RooAbsData *> (fData); 
   if (!treeData) { 
      std::cerr << "Error in HybridCalculator::GetHypoTest - invalid data type - return NULL" << std::endl;
      return 0; 
   }
   bool usePrior = (fUsePriorPdf && fPriorPdf ); 
   return Calculate( *treeData, fNToys, usePrior);  
}


bool HybridCalculator::DoCheckInputs() const { 
   if (!fData) { 
      std::cerr << "Error in HybridCalculator - data have not been set" << std::endl;
      return false; 
   }

   // if observable have not been set take them from data 
   if (!fObservables && fData->get() ) fObservables =  new RooArgList( *fData->get() );
   if (!fObservables) { 
      std::cerr << "Error in HybridCalculator - no observables" << std::endl;
      return false; 
   }

   if (!fSbModel) { 
      std::cerr << "Error in HybridCalculator - S+B pdf has not been set " << std::endl;
      return false; 
   }

   if (!fBModel) { 
      std::cerr << "Error in HybridCalculator - B pdf has not been set" << std::endl;
      return false; 
   }
   if (fUsePriorPdf && !fNuisanceParameters) { 
      std::cerr << "Error in HybridCalculator - nuisance parameters have not been set " << std::endl;
      return false; 
   }
   if (fUsePriorPdf && !fPriorPdf) { 
      std::cerr << "Error in HybridCalculator - prior pdf has not been set " << std::endl;
      return false; 
   }
   return true; 
}


