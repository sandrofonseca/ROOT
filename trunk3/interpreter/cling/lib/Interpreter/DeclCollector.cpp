//------------------------------------------------------------------------------
// CLING - the C++ LLVM-based InterpreterG :)
// version: $Id$
// author:  Vassil Vassilev <vasil.georgiev.vasilev@cern.ch>
//------------------------------------------------------------------------------

#include "DeclCollector.h"

#include "cling/Interpreter/Transaction.h"

#include "clang/AST/Decl.h"
#include "clang/AST/DeclCXX.h"
#include "clang/AST/DeclGroup.h"

using namespace clang;

namespace cling {

  // pin the vtable here.
  DeclCollector::~DeclCollector() {
  }

  bool DeclCollector::HandleTopLevelDecl(DeclGroupRef DGR) {
    Transaction::DelayCallInfo DCI(DGR, Transaction::kCCIHandleTopLevelDecl);
    m_CurTransaction->append(DCI);
    return true;
  }

  void DeclCollector::HandleInterestingDecl(DeclGroupRef DGR) {
    Transaction::DelayCallInfo DCI(DGR, Transaction::kCCIHandleInterestingDecl);
    m_CurTransaction->append(DCI);
  }

  void DeclCollector::HandleTagDeclDefinition(TagDecl* TD) {
    Transaction::DelayCallInfo DCI(DeclGroupRef(TD), 
                                   Transaction::kCCIHandleTagDeclDefinition);
    m_CurTransaction->append(DCI);    
  }

  void DeclCollector::HandleVTable(CXXRecordDecl* RD, bool DefinitionRequired) {
    Transaction::DelayCallInfo DCI(DeclGroupRef(RD),
                                   Transaction::kCCIHandleVTable);
    m_CurTransaction->append(DCI);    

    // Intentional no-op. It comes through Sema::DefineUsedVTables, which
    // comes either Sema::ActOnEndOfTranslationUnit or while instantiating a
    // template. In our case we will do it on transaction commit, without 
    // keeping track of used vtables, because we have cases where we bypass the
    // clang/AST and directly ask the module so that we have to generate 
    // everything without extra smartness.
  }

  void DeclCollector::CompleteTentativeDefinition(VarDecl* VD) {
    assert(0 && "Not implemented yet!");
  }

  void DeclCollector::HandleTranslationUnit(ASTContext& Ctx) {
  }
} // namespace cling
