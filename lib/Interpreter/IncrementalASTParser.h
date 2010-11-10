//------------------------------------------------------------------------------
// CLING - the C++ LLVM-based InterpreterG :)
// version: $Id$
// author:  Axel Naumann <axel@cern.ch>
//------------------------------------------------------------------------------

#ifndef CLING_INCREMENTAL_AST_PARSER_H
#define CLING_INCREMENTAL_AST_PARSER_H

#include "clang/Basic/SourceLocation.h"
#include "llvm/ADT/OwningPtr.h"
#include "llvm/ADT/StringRef.h"

namespace clang {
  class CompilerInstance;
  class Parser;
  class Sema;
  class ASTConsumer;
  class PragmaNamespace;
  class SourceLocation;
}

namespace cling {
  class MutableMemoryBuffer;
  class ChainedASTConsumer;
  class ASTPragmaHandler;

  class IncrementalASTParser {
  public:
    IncrementalASTParser(clang::CompilerInstance* CI,
                         clang::ASTConsumer* Consumer,
                         clang::PragmaNamespace* Pragma);
    ~IncrementalASTParser();
    
    clang::CompilerInstance* getCI() const { return m_CI.get(); }
    clang::CompilerInstance* parse(llvm::StringRef src,
                                   int nTopLevelDecls = -1,
                                   clang::ASTConsumer* Consumer = 0);
    void RequestParseInterrupt(clang::SourceLocation Loc) {
      m_InterruptHere = Loc; }

  private:
    llvm::OwningPtr<clang::CompilerInstance> m_CI; // compiler instance.
    llvm::OwningPtr<clang::Parser> m_Parser; // parser (incremental)
    llvm::OwningPtr<clang::Sema> m_Sema; // sema used for parsing
    llvm::OwningPtr<MutableMemoryBuffer> m_MemoryBuffer; // compiler instance
    llvm::OwningPtr<ASTPragmaHandler> m_ASTPragmaHandler; // pragma cling ast(...)
    ChainedASTConsumer* m_Consumer; // CI owns it
    clang::SourceLocation m_InterruptHere; // where to stop parsing top level decls
  };
}
#endif // CLING_INCREMENTAL_AST_PARSER_H
