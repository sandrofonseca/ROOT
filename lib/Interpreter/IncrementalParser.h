//------------------------------------------------------------------------------
// CLING - the C++ LLVM-based InterpreterG :)
// version: $Id$
// author:  Axel Naumann <axel@cern.ch>
//------------------------------------------------------------------------------

#ifndef CLING_INCREMENTAL_PARSER_H
#define CLING_INCREMENTAL_PARSER_H

#include "clang/Basic/SourceLocation.h"
#include "llvm/ADT/OwningPtr.h"
#include "llvm/ADT/StringRef.h"
#include <vector>

namespace clang {
  class CompilerInstance;
  class Decl;
  class Parser;
  class Sema;
  class ASTConsumer;
  class PragmaNamespace;
  class SourceLocation;
  class FileID;
}

namespace cling {
  class MutableMemoryBuffer;
  class ChainedASTConsumer;
  class DynamicExprTransformer;
  class Interpreter;
  class CIFactory;
  
  class IncrementalParser {
  public:
    IncrementalParser(Interpreter* interp,clang::PragmaNamespace* Pragma, const char* llvmdir);
    ~IncrementalParser();
    void Initialize();
    clang::CompilerInstance* getCI() const { return m_CI.get(); }
    clang::Parser* getParser() const { return m_Parser.get(); }
    clang::CompilerInstance* parse(llvm::StringRef src);
    
    MutableMemoryBuffer* getCurBuffer() {
      return m_MemoryBuffer.back();
    }
    
    void setEnabled(bool value = true);
    bool getEnabled() { return m_Enabled; }
    DynamicExprTransformer *getTransformer() const { return m_Transformer.get(); }
    
    void emptyLastFunction();
    clang::Decl* getLastTopLevelDecl() const { return m_LastTopLevelDecl; }
    clang::Decl* getFirstTopLevelDecl() const { return m_FirstTopLevelDecl; }
    
    void addConsumer(clang::ASTConsumer* consumer);
    void removeConsumer(clang::ASTConsumer* consumer);
    
  private:
    llvm::OwningPtr<clang::CompilerInstance> m_CI; // compiler instance.
    llvm::OwningPtr<clang::Parser> m_Parser; // parser (incremental)
    bool m_Enabled; // enable/disable dynamic scope
    llvm::OwningPtr<DynamicExprTransformer> m_Transformer; // delayed id lookup
    std::vector<MutableMemoryBuffer*> m_MemoryBuffer; // One buffer for each command line, owner by the source file manager
    clang::FileID m_MBFileID; // file ID of the memory buffer
    ChainedASTConsumer* m_Consumer; // CI owns it
    clang::Decl* m_LastTopLevelDecl; // last top level decl after most recent call to parse()
    clang::Decl* m_FirstTopLevelDecl; // first top level decl
  };
}
#endif // CLING_INCREMENTAL_PARSER_H
