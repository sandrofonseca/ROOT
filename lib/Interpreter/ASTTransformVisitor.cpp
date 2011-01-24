//------------------------------------------------------------------------------
// CLING - the C++ LLVM-based InterpreterG :)
// version: $Id: ASTTransformVisitor.cpp 36608 2010-11-11 18:21:02Z vvassilev $
// author:  Vassil Vassilev <vasil.georgiev.vasilev@cern.ch>
//------------------------------------------------------------------------------

#include "ASTTransformVisitor.h"

#include "llvm/ADT/SmallVector.h"

namespace llvm {
   class raw_string_ostream;
}

namespace {
 
   class StmtPrinterHelper : public PrinterHelper  {
   private:
      PrintingPolicy m_Policy;
      llvm::SmallVector<clang::DeclRefExpr*, 64> m_Environment;
   public:
      
      StmtPrinterHelper(const PrintingPolicy &Policy, llvm::SmallVector<clang::DeclRefExpr*, 64> &Environment) : 
         m_Policy(Policy), m_Environment(Environment) {}
      
      virtual ~StmtPrinterHelper() {}
      

      // Handle only DeclRefExprs since they are local and the call wrapper
      // won't "see" them. Consequently we don't need to handle:
      // * DependentScopeDeclRefExpr
      // * CallExpr
      // * MemberExpr
      // * CXXDependentScopeMemberExpr
      virtual bool handledStmt(Stmt* S, llvm::raw_ostream& OS) {
         if (DeclRefExpr *Node = dyn_cast<DeclRefExpr>(S)) {
            if (NestedNameSpecifier *Qualifier = Node->getQualifier())
               Qualifier->print(OS, m_Policy);
            m_Environment.push_back(Node);
            OS << "*("; 
            // Copy-paste from the StmtPrinter
            QualType T = Node->getType();
            SplitQualType T_split = T.split();
            OS << QualType::getAsString(T_split);
 
            if (!T.isNull()) {
               // If the type is sugared, also dump a (shallow) desugared type.
               SplitQualType D_split = T.getSplitDesugaredType();
               if (T_split != D_split)
                  OS << ":'" << QualType::getAsString(D_split) << "'";
            }
            // end
            
            OS <<"*)@";
                       
            if (Node->hasExplicitTemplateArgs())
               OS << TemplateSpecializationType::PrintTemplateArgumentList(
                                                                           Node->getTemplateArgs(),
                                                                           Node->getNumTemplateArgs(),
                                                                           m_Policy);  
            if (Node->hasExplicitTemplateArgs())
               assert((Node->getTemplateArgs() || Node->getNumTemplateArgs()) && "There shouldn't be template paramlist");

            return true;            
         }
         
         return false;
      }
   };
} // end anonymous namespace


namespace cling {

   // DeclVisitor
   
   void ASTTransformVisitor::Visit(Decl *D) {
      //if (ShouldVisit(D)) {
         Decl *PrevDecl = ASTTransformVisitor::CurrentDecl;
         ASTTransformVisitor::CurrentDecl = D;
         BaseDeclVisitor::Visit(D);
         ASTTransformVisitor::CurrentDecl = PrevDecl;
       //}
   }
   
   void ASTTransformVisitor::VisitFunctionDecl(FunctionDecl *D) {
      BaseDeclVisitor::VisitFunctionDecl(D);
     
      if (D->isThisDeclarationADefinition()) {
         Stmt *Old = D->getBody();
         Stmt *New = Visit(Old).getNewStmt();
         if (Old != New)
            D->setBody(New);
      }
   }
 
   void ASTTransformVisitor::VisitTemplateDecl(TemplateDecl *D) {     
      if (D->getNameAsString().compare("Eval") == 0) {
         CXXRecordDecl *CXX = dyn_cast<CXXRecordDecl>(D->getDeclContext());
         if (CXX && CXX->getNameAsString().compare("Interpreter") == 0) {  
            NamespaceDecl *ND = dyn_cast<NamespaceDecl>(CXX->getDeclContext());
            if (ND && ND->getNameAsString().compare("cling") == 0) {
               if (FunctionDecl *FDecl = dyn_cast<FunctionDecl>(D->getTemplatedDecl()))
                  setEvalDecl(FDecl);
            }
         }
      }
   }
  
   void ASTTransformVisitor::VisitDecl(Decl *D) {
      if (!ShouldVisit(D))
         return;
      
      if (DeclContext *DC = dyn_cast<DeclContext>(D))
         static_cast<ASTTransformVisitor*>(this)->VisitDeclContext(DC);
   }
   
   void ASTTransformVisitor::VisitDeclContext(DeclContext *DC) {
      for (DeclContext::decl_iterator
              I = DC->decls_begin(), E = DC->decls_end(); I != E; ++I)        
         if (ShouldVisit(*I))
            Visit(*I);
   }
   
   // end DeclVisitor
   
   // StmtVisitor
   
   EvalInfo ASTTransformVisitor::VisitStmt(Stmt *Node) {
      for (Stmt::child_iterator
              I = Node->child_begin(), E = Node->child_end(); I != E; ++I) {
         if (*I) {
            EvalInfo EInfo = Visit(*I);
            if (EInfo.IsEvalNeeded) {
               if (Expr *E = dyn_cast<Expr>(EInfo.getNewStmt()))
                  // Assume void if still not escaped
                  *I = SubstituteUnknownSymbol(SemaPtr->getASTContext().VoidTy, E);
            } 
            else {
               *I = EInfo.getNewStmt();
            }
         }
      }
      
      return EvalInfo(Node, 0);
   }
   
   EvalInfo ASTTransformVisitor::VisitExpr(Expr *Node) {
      for (Stmt::child_iterator
              I = Node->child_begin(), E = Node->child_end(); I != E; ++I) {
         if (*I) {
            EvalInfo EInfo = Visit(*I);
            if (EInfo.IsEvalNeeded) {
               if (Expr *E = dyn_cast<Expr>(EInfo.getNewStmt()))
                  // Assume void if still not escaped
                  *I = SubstituteUnknownSymbol(SemaPtr->getASTContext().VoidTy, E);
            } 
            else {
               *I = EInfo.getNewStmt();
            }
         }
      }
      return EvalInfo(Node, 0);
   }

   // EvalInfo ASTTransformVisitor::VisitCompoundStmt(CompoundStmt *S) {
   //    for (CompoundStmt::body_iterator
   //            I = S->body_begin(), E = S->body_end(); I != E; ++I) {
   //       EvalInfo EInfo = Visit(*I);
   //       if (EInfo.IsEvalNeeded) {
   //          if (Expr *Exp = dyn_cast<Expr>(EInfo.getNewStmt())) {
   //             QualType T = Exp->getType();
   //             // Assume if still dependent void
   //             if (Exp->isTypeDependent() || Exp->isValueDependent())
   //                T = SemaPtr->getASTContext().VoidTy;

   //             *I = BuildEvalCallExpr(T);
   //          }
   //       } 
   //       else {
   //          *I = EInfo.getNewStmt();
   //       }
   //    }
   //    return EvalInfo(S, 0);
   // }

   EvalInfo ASTTransformVisitor::VisitCallExpr(CallExpr *E) {
      if (IsArtificiallyDependent(E)) {
         // FIXME: Handle the arguments
         // EvalInfo EInfo = Visit(E->getCallee());
         
         return EvalInfo(E, 1);
         
      }
      return EvalInfo(E, 0);
   }
   
   // EvalInfo ASTTransformVisitor::VisitImplicitCastExpr(ImplicitCastExpr *ICE) {
   //    return EvalInfo(ICE, 0);
   // }
   
   EvalInfo ASTTransformVisitor::VisitDeclRefExpr(DeclRefExpr *DRE) {
         return EvalInfo(DRE, 0);
   }
      
   EvalInfo ASTTransformVisitor::VisitDependentScopeDeclRefExpr(DependentScopeDeclRefExpr *Node) {
         return EvalInfo(Node, 1);
   }

   EvalInfo ASTTransformVisitor::VisitBinaryOperator(BinaryOperator *binOp) {
      EvalInfo rhs = Visit(binOp->getRHS());
      EvalInfo lhs = Visit(binOp->getLHS());
      /*
      if (binOp->isAssignmentOp()) {
         if (rhs.IsEvalNeeded && !lhs.IsEvalNeeded) {
            if (Expr *E = dyn_cast<Expr>(lhs.getNewStmt()))
               if (!E->isTypeDependent() || !E->isValueDependent()) {
                  const QualType returnTy = E->getType();
                  binOp->setRHS(SubstituteUnknownSymbol(returnTy, E));
               }
         }
      }
      */
      return EvalInfo(binOp, IsArtificiallyDependent(binOp));
   }
   
   //endregion

   //region EvalBuilder


   Expr *ASTTransformVisitor::SubstituteUnknownSymbol(const QualType InstTy, Expr *SubTree) {
      // Get the addresses
      BuildEvalEnvironment(SubTree);

      //Build the arguments for the call
      ASTOwningVector<Expr*> CallArgs(*SemaPtr);
      BuildEvalArgs(CallArgs);

      // Build the call
      CallExpr* EvalCall = BuildEvalCallExpr(InstTy, SubTree, CallArgs);

      // Add substitution mapping
      getSubstSymbolMap()[EvalCall] = SubTree;
      
      return EvalCall;
   }

   // Creates the string, which is going to be escaped.
   void ASTTransformVisitor::BuildEvalEnvironment(Expr *SubTree) {
      m_EvalExpressionBuf = "";
      llvm::raw_string_ostream OS(m_EvalExpressionBuf);
      const PrintingPolicy &Policy = SemaPtr->getASTContext().PrintingPolicy;
      
      StmtPrinterHelper *helper = new StmtPrinterHelper(Policy, m_Environment);      
      SubTree->printPretty(OS, helper, Policy);
      
      OS.flush();
   
   }
   
   // Prepare the actual arguments for the call
   // Arg list for static T Eval(size_t This, const char* expr, void* varaddr[] )
   void ASTTransformVisitor::BuildEvalArgs(ASTOwningVector<Expr*> &Result) {
      ASTContext &C = SemaPtr->getASTContext();

      // Arg 0:
      Expr *Arg0 = BuildEvalArg0(C);
      Result.push_back(Arg0);

      // Arg 1:
      Expr *Arg1 = BuildEvalArg1(C);    
      Result.push_back(Arg1);

      // Arg 2:
      Expr *Arg2 = BuildEvalArg2(C);          
      Result.push_back(Arg2);

   }
   
   // Eval Arg0: size_t This
   Expr *ASTTransformVisitor::BuildEvalArg0(ASTContext &C) {
      const llvm::APInt gClingAddr(8 * sizeof(void *), (uint64_t)gCling);
      IntegerLiteral *Arg0 = IntegerLiteral::Create(C, gClingAddr, C.UnsignedLongTy, SourceLocation());

      return Arg0;
   }

   // Eval Arg1: const char* expr
   Expr *ASTTransformVisitor::BuildEvalArg1(ASTContext &C) {
      QualType constCharArray = C.getConstantArrayType(C.getConstType(C.CharTy), llvm::APInt(8 * sizeof(void *), strlen(m_EvalExpressionBuf.c_str()) + 1), ArrayType::Normal, 0);
      Expr *Arg1 = StringLiteral::Create(C, &*m_EvalExpressionBuf.c_str(), strlen(m_EvalExpressionBuf.c_str()) + 1, false, constCharArray, SourceLocation());
      //FIXME: Figure out how handle the cast kinds in the different cases
      QualType CastTo = C.getPointerType(C.getConstType(C.CharTy));
      SemaPtr->ImpCastExprToType(Arg1, CastTo, CK_ArrayToPointerDecay);

      return Arg1;
   }

   // Eval Arg2: void* varaddr[]
   Expr *ASTTransformVisitor::BuildEvalArg2(ASTContext &C) {
      QualType VarAddrTy = SemaPtr->BuildArrayType(C.VoidPtrTy, 
                                                   ArrayType::Normal,
                                                   /*ArraySize*/0
                                                   , Qualifiers()
                                                   , SourceRange()
                                                   , DeclarationName() );
      // FIXME: Get the init list from the SubTree...
      ASTOwningVector<Expr*> Inits(*SemaPtr);

      // We need fake the SourceLocation just to avoid assert(InitList.isExplicit()....)
      SourceLocation SLoc = getEvalDecl()->getLocStart();
      SourceLocation ELoc = getEvalDecl()->getLocEnd();
      InitListExpr *ILE = SemaPtr->ActOnInitList(SLoc, move_arg(Inits), ELoc).takeAs<InitListExpr>();
      Expr *Arg2 = SemaPtr->BuildCompoundLiteralExpr(SourceLocation(), C.CreateTypeSourceInfo(VarAddrTy), SourceLocation(), ILE).takeAs<CompoundLiteralExpr>();
      SemaPtr->ImpCastExprToType(Arg2, C.getPointerType(C.VoidPtrTy), CK_ArrayToPointerDecay);

      return Arg2;
   }

   // Here is the test Eval function specialization. Here the CallExpr to the function
   // is created.
   CallExpr *ASTTransformVisitor::BuildEvalCallExpr(const QualType InstTy, Expr *SubTree, 
                                                    ASTOwningVector<Expr*> &CallArgs) {      
      // Set up new context for the new FunctionDecl
      DeclContext *PrevContext = SemaPtr->CurContext;
      FunctionDecl *FDecl = getEvalDecl();
      
      assert(FDecl && "The Eval function not found!");

      SemaPtr->CurContext = FDecl->getDeclContext();
      
      // Create template arguments
      Sema::InstantiatingTemplate Inst(*SemaPtr, SourceLocation(), FDecl);
      TemplateArgument Arg(InstTy);
      TemplateArgumentList TemplateArgs(TemplateArgumentList::OnStack, &Arg, 1U);
      
      // Substitute the declaration of the templated function, with the 
      // specified template argument
      Decl *D = SemaPtr->SubstDecl(FDecl, FDecl->getDeclContext(), MultiLevelTemplateArgumentList(TemplateArgs));
      
      FunctionDecl *Fn = dyn_cast<FunctionDecl>(D);
      // Creates new body of the substituted declaration
      SemaPtr->InstantiateFunctionDefinition(Fn->getLocation(), Fn, true, true);
      
      SemaPtr->CurContext = PrevContext;                            
      
      const FunctionProtoType *Proto = Fn->getType()->getAs<FunctionProtoType>();

      //Walk the params and prepare them for building a new function type
      llvm::SmallVectorImpl<QualType> ParamTypes(FDecl->getNumParams());
      for (FunctionDecl::param_iterator P = FDecl->param_begin(), PEnd = FDecl->param_end();
           P != PEnd;
           ++P) {
         ParamTypes.push_back((*P)->getType());
         
      }
      
      // Build function type, needed by BuildDeclRefExpr 
      QualType FuncT = SemaPtr->BuildFunctionType(Fn->getResultType()
                                                  , ParamTypes.data()
                                                  , ParamTypes.size()
                                                  , Proto->isVariadic()
                                                  , Proto->getTypeQuals()
                                                  , Fn->getLocation()
                                                  , Fn->getDeclName()
                                                  , Proto->getExtInfo());                  
      
      DeclRefExpr *DRE = SemaPtr->BuildDeclRefExpr(Fn, FuncT, VK_RValue, SourceLocation()).takeAs<DeclRefExpr>();
      
      CallExpr *EvalCall = SemaPtr->ActOnCallExpr(SemaPtr->getScopeForContext(SemaPtr->CurContext)
                                                  , DRE
                                                  , SourceLocation()
                                                  //,MultiExprArg(CallArgs.take() , 1U)
                                                  , move_arg(CallArgs)
                                                  , SourceLocation()
                                                  ).takeAs<CallExpr>();
      assert (EvalCall && "Cannot create call to Eval");
      return EvalCall;                  
      
   }
   
   bool ASTTransformVisitor::ShouldVisit(Decl *D) {
      while (true) {
         if (isa<TemplateTemplateParmDecl>(D))
            return false;
         if (isa<ClassTemplateDecl>(D))
            return false;
         if (isa<FriendTemplateDecl>(D))
            return false;
         if (isa<ClassTemplatePartialSpecializationDecl>(D))
            return false;
         if (CXXRecordDecl *CXX = dyn_cast<CXXRecordDecl>(D)) {
            if (CXX->getDescribedClassTemplate())
               return false;
         }
         if (CXXMethodDecl *CXX = dyn_cast<CXXMethodDecl>(D)) {
            if (CXX->getDescribedFunctionTemplate())
               return false;
         }
         if (isa<TranslationUnitDecl>(D)) {
            break;
         }
         
         if (DeclContext* DC = D->getDeclContext())
            if (!(D = dyn_cast<Decl>(DC)))
                break;
      }
      
      return true;
   }

   bool ASTTransformVisitor::IsArtificiallyDependent(Expr *Node) {
      if (!Node->isValueDependent() || !Node->isTypeDependent())
          return false;     
      return true;
   }

   
// end StmtVisitor

}//end cling
