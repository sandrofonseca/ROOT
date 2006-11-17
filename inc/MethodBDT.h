// @(#)root/tmva $Id: MethodBDT.h,v 1.36 2006/11/17 14:59:23 stelzer Exp $ 
// Author: Andreas Hoecker, Joerg Stelzer, Helge Voss, Kai Voss 

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis       *
 * Package: TMVA                                                                  *
 * Class  : MethodBDT  (Boosted Decision Trees)                                   *
 * Web    : http://tmva.sourceforge.net                                           *
 *                                                                                *
 * Description:                                                                   *
 *      Analysis of Boosted Decision Trees                                        *
 *                                                                                *
 * Authors (alphabetical):                                                        *
 *      Andreas Hoecker <Andreas.Hocker@cern.ch> - CERN, Switzerland              *
 *      Xavier Prudent  <prudent@lapp.in2p3.fr>  - LAPP, France                   *
 *      Helge Voss      <Helge.Voss@cern.ch>     - MPI-K Heidelberg, Germany      *
 *      Kai Voss        <Kai.Voss@cern.ch>       - U. of Victoria, Canada         *
 *                                                                                *
 * Copyright (c) 2005:                                                            *
 *      CERN, Switzerland,                                                        * 
 *      U. of Victoria, Canada,                                                   * 
 *      MPI-K Heidelberg, Germany ,                                               * 
 *      LAPP, Annecy, France                                                      *
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in LICENSE           *
 * (http://tmva.sourceforge.net/LICENSE)                                          *
 **********************************************************************************/

#ifndef ROOT_TMVA_MethodBDT
#define ROOT_TMVA_MethodBDT

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// MethodBDT                                                            //
//                                                                      //
// Analysis of Boosted Decision Trees                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <vector>
#include "TH2.h"
#include "TTree.h"
#ifndef ROOT_TMVA_MethodBase
#include "TMVA/MethodBase.h"
#endif
#ifndef ROOT_TMVA_BinarySearchTree
#include "TMVA/BinarySearchTree.h"
#endif
#ifndef ROOT_TMVA_DecisionTree
#include "TMVA/DecisionTree.h"
#endif
#ifndef ROOT_TMVA_Event
#include "TMVA/Event.h"
#endif
#ifndef ROOT_TMVA_SeparationBase
#include "TMVA/SeparationBase.h"
#endif
#ifndef ROOT_TMVA_GiniIndex
#include "TMVA/GiniIndex.h"
#endif
#ifndef ROOT_TMVA_CrossEntropy
#include "TMVA/CrossEntropy.h"
#endif
#ifndef ROOT_TMVA_MisClassificationError
#include "TMVA/MisClassificationError.h"
#endif
#ifndef ROOT_TMVA_SdivSqrtSplusB
#include "TMVA/SdivSqrtSplusB.h"
#endif

namespace TMVA {

   class MethodBDT : public MethodBase {

   public:
      // MethodBDT (Boosted Decision Trees) options:
      // format and syntax of option string: "nTrees:BoostType:SeparationType:
      //                                      nEventsMin:dummy:
      //                                      nCuts:SignalFraction"
      // nTrees:          number of trees in the forest to be created
      // BoostType:       the boosting type for the trees in the forest (AdaBoost e.t.c..)
      // SeparationType   the separation criterion applied in the node splitting
      // nEventsMin:      the minimum number of events in a node (leaf criteria, stop splitting)
      // nCuts:  the number of steps in the optimisation of the cut for a node
      // SignalFraction:  scale parameter of the number of Bkg events  
      //                  applied to the training sample to simulate different initial purity
      //                  of your data sample. 
      // UseYesNoLeaf     decide if the classification is done simply by the node type, or the S/B
      //                  (from the training) in the leaf node
      // UseWeightedTrees use average classification from the trees, or have the individual trees
      //                  trees in the forest weighted (e.g. log(boostweight) from AdaBoost
      //
      // known SeparationTypes are:
      //    - MisClassificationError
      //    - GiniIndex
      //    - CrossEntropy
      // known BoostTypes are:
      //    - AdaBoost
      //    - Bagging

      // constructor for training and reading
      MethodBDT( TString jobName, 
                 TString methodTitle, 
                 DataSet& theData,
                 TString theOption = "",
                 TDirectory* theTargetDir = 0 );

      // constructor for calculating BDT-MVA using previously generatad decision trees
      MethodBDT( DataSet& theData, 
                 TString theWeightFile,  
                 TDirectory* theTargetDir = NULL );
  
      virtual ~MethodBDT( void );
    
      // write all Events from the Tree into a vector of Events, that are 
      // more easily manipulated 
      virtual void InitEventSample();

      // training method
      virtual void Train( void );

      // write weights to file
      virtual void WriteWeightsToStream( ostream& o ) const;

      // read weights from file
      virtual void ReadWeightsFromStream( istream& istr );

      // write method specific histos to target file
      virtual void WriteMonitoringHistosToFile( void ) const;

      // calculate the MVA value
      virtual Double_t GetMvaValue();

      // apply the boost algorithm to a tree in the collection 
      virtual Double_t Boost( std::vector<Event*>, DecisionTree *dt, Int_t iTree );

      // ranking of input variables
      const Ranking* CreateRanking();

      // the option handling methods
      virtual void DeclareOptions();
      virtual void ProcessOptions();

      // get the forest
     inline const std::vector<DecisionTree*> & GetForest() const;

      // get the forest
     inline const std::vector<Event*> & GetTrainingEvents() const;

     inline const std::vector<double> & GetBoostWeights() const;

     //return the individual relative variable importance 
     std::vector<Double_t> GetVariableImportance();
     Double_t GetVariableImportance(UInt_t ivar);

     Double_t  PruneTree( TMVA::DecisionTree *dt, Int_t itree);
     Double_t TestTreeQuality( TMVA::DecisionTree *dt );


   private:

      // boosting algorithm (adaptive boosting)
      Double_t AdaBoost( std::vector<Event*>, DecisionTree *dt );
 
      // boosting as a random re-weighting
      Double_t Bagging( std::vector<Event*>, Int_t iTree );
  
      std::vector<Event*>             fEventSample;     // the training events
      std::vector<Event*>             fValidationSample;// the Validation events
 
      Int_t                           fNTrees;          // number of decision trees requested
      std::vector<DecisionTree*>      fForest;          // the collection of decision trees
      std::vector<double>             fBoostWeights;    // the weights applied in the individual boosts
      TString                         fBoostType;       // string specifying the boost type

      //options for the decision Tree
      SeparationBase                 *fSepType;         // the separation used in node splitting
      TString                         fSepTypeS;        // the separation (option string) used in node splitting
      Int_t                           fNodeMinEvents;   // min number of events in node 
  
      Int_t                           fNCuts;           // grid used in cut applied in node splitting
      Bool_t                          fUseYesNoLeaf;    // use sig or bkg classification in leave nodes or sig/bkg
      Bool_t                          fUseWeightedTrees;// use average classification from the trees, or have the individual trees trees in the forest weighted (e.g. log(boostweight) from AdaBoost
    

      // Init used in the various constructors
      void InitBDT( void );

      //some histograms for monitoring
      TH1F*                            fBoostWeightHist; // weights applied in boosting
      TH1F*                            fBoostWeightVsTree;// weights applied in boosting vs tree number
      TH1F*                            fErrFractHist;    // error fraction vs tree number
      TH1I*                            fNodesBeforePruningVsTree;  // nNodesBeforePruning vs tree number
      TH1I*                            fNodesAfterPruningVsTree;   // nNodesAfterPruning vs tree number
      TTree*                           fMonitorNtuple;   // monitoring ntuple
      Int_t                            fITree;           // ntuple var: ith tree
      Double_t                         fBoostWeight;     // ntuple var: boost weight
      Double_t                         fErrorFraction;   // ntuple var: misclassification error fraction 
      Double_t                         fPruneStrength;   // a parameter to set the "amount" of pruning..needs to be adjusted 
      TMVA::DecisionTree::EPruneMethod fPruneMethod;     // method used for prunig 
      TString                          fPruneMethodS;    // prune method option String
      Bool_t                           fAutomatic;       // use user given prune strength or automatically determined one using a validation sample 


      std::vector<Double_t>           fVariableImportance; // the relative importance of the different variables 

      Double_t                        fDeltaPruneStrength; // step size in pruning, is adjusted according to experience of previous trees        
      // debugging flags
      static const Int_t  fgDebugLevel = 0;     // debug level determining some printout/control plots etc.


      ClassDef(MethodBDT,0)  // Analysis of Boosted Decision Trees 
         ;
   };

} // namespace TMVA

const std::vector<TMVA::DecisionTree*>& TMVA::MethodBDT::GetForest()         const { return fForest; }
const std::vector<TMVA::Event*>&        TMVA::MethodBDT::GetTrainingEvents() const { return fEventSample; }
const std::vector<double>&              TMVA::MethodBDT::GetBoostWeights()   const { return fBoostWeights; }

#endif
