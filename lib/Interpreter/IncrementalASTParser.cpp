//------------------------------------------------------------------------------
// CLING - the C++ LLVM-based InterpreterG :)
// version: $Id: Interpreter.cpp 36507 2010-11-04 21:42:17Z axel $
// author:  Axel Naumann <axel@cern.ch>
//------------------------------------------------------------------------------

#include "IncrementalASTParser.h"

#include "llvm/Support/MemoryBuffer.h"

#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclGroup.h"
#include "clang/Parse/Parser.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/Pragma.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Sema/SemaConsumer.h"

#include "cling/Interpreter/Diagnostics.h"
#include "cling/Interpreter/Interpreter.h"
#include "ASTTransformVisitor.h"
#include "ChainedASTConsumer.h"

#include <stdio.h>
#include <sstream>

namespace cling {
class MutableMemoryBuffer: public llvm::MemoryBuffer {
    std::string m_FileID;
    size_t m_Alloc;
  protected:
    void maybeRealloc(llvm::StringRef code, size_t oldlen) {
      size_t applen = code.size();
      char* B = 0;
      if (oldlen) {
        B = const_cast<char*>(getBufferStart());
        assert(!B[oldlen] && "old buffer is not 0 terminated!");
        // B + oldlen points to trailing '\0'
      }
      size_t newlen = oldlen + applen + 1;
      if (newlen > m_Alloc) {
        m_Alloc += 64*1024;
        B = (char*)realloc(B, m_Alloc);
      }
      memcpy(B + oldlen, code.data(), applen);
      B[newlen - 1] = 0;
      init(B, B + newlen - 1);
    }

  public:
    MutableMemoryBuffer(llvm::StringRef Code, llvm::StringRef Name)
    : MemoryBuffer(), m_FileID(Name), m_Alloc(0) {
      maybeRealloc(Code, 0);
    }
    
    virtual ~MutableMemoryBuffer() {
      free((void*)getBufferStart());
    }
    
    void append(llvm::StringRef code) {
      assert(getBufferSize() && "buffer is empty!");
      maybeRealloc(code, getBufferSize());
    }
    virtual const char *getBufferIdentifier() const {
      return m_FileID.c_str();
    }
  };


   IncrementalASTParser::IncrementalASTParser(clang::CompilerInstance* CI,
                                              clang::ASTConsumer* Consumer,
                                              clang::PragmaNamespace* Pragma,
                                              Interpreter* Interp):
      m_Interpreter(Interp), m_Consumer(0), m_LastTopLevelDecl(0), m_FirstTopLevelDecl(0) {
      assert(CI && "CompilerInstance is (null)!");
      m_CI.reset(CI);
      
      CI->createPreprocessor();
      clang::Preprocessor& PP = CI->getPreprocessor();
      PP.AddPragmaHandler(Pragma);
      clang::ASTContext *Ctx = new clang::ASTContext(CI->getLangOpts(),
                                                     PP.getSourceManager(), CI->getTarget(), PP.getIdentifierTable(),
                                                     PP.getSelectorTable(), PP.getBuiltinInfo(), 0);
      CI->setASTContext(Ctx);
      PP.getBuiltinInfo().InitializeBuiltins(PP.getIdentifierTable(),
                                             PP.getLangOptions());
      /*NoBuiltins = */ //true);
      
      
      m_MemoryBuffer.push_back(new MutableMemoryBuffer("//cling!\n", "CLING") );
      
      CI->getSourceManager().clearIDTables();
      m_MBFileID = CI->getSourceManager().createMainFileIDForMemBuffer(m_MemoryBuffer[0]);
      CI->getSourceManager().getBuffer(m_MBFileID, clang::SourceLocation());
      if (CI->getSourceManager().getMainFileID().isInvalid()) {
         fprintf(stderr, "Interpreter::compileString: Failed to create main "
                 "file id!\n");
         return;
      }
      
      m_Consumer = new ChainedASTConsumer();
      m_Consumer->Consumers.push_back(Consumer);
      m_Consumer->Initialize(*Ctx);
      CI->setASTConsumer(m_Consumer);
      
      bool CompleteTranslationUnit = false;
      clang::CodeCompleteConsumer* CCC = 0;
      CI->createSema(CompleteTranslationUnit, CCC);
      m_Sema = &CI->getSema();
      // Initialize the parser.
      m_Parser.reset(new clang::Parser(PP, *m_Sema));
      PP.EnterMainSourceFile();
      m_Parser->Initialize();
      m_Sema->Initialize();
      
      if (clang::SemaConsumer *SC = dyn_cast<clang::SemaConsumer>(m_Consumer))
         SC->InitializeSema(*m_Sema);
      
      // Create the visitor that will transform all dependents that are left.
      m_Transformer.reset(new ASTTransformVisitor(m_Interpreter, &CI->getSema()));      
   }
   
   IncrementalASTParser::~IncrementalASTParser() {}
   
   void IncrementalASTParser::Initialize() {
      // Allow the interpreter to find itself.
      // OBJ first: if it exists it should be more up to date
      m_Interpreter->AddIncludePath(CLING_SRCDIR_INCL);
      m_Interpreter->AddIncludePath(CLING_INSTDIR_INCL);
      
      m_Interpreter->compileString(""); // Consume initialization.
      
      std::stringstream sstr;
      sstr << "#include <stdio.h>\n"
           << "#define __STDC_LIMIT_MACROS\n"
           << "#define __STDC_CONSTANT_MACROS\n"
           << "#include \"cling/Interpreter/Interpreter.h\"\n"
           << "#include \"cling/Interpreter/ValuePrinter.h\"\n";
      // Would like
      // namespace cling {Interpreter* gCling = (Interpreter*)0x875478643;"
      // but we can't handle namespaced decls yet :-(
      sstr << "cling::Interpreter* gCling = (cling::Interpreter*)"
           << (const void*) this << ";";
      m_Interpreter->compileString(sstr.str());
      // Attach the dynamic lookup
      getTransformer()->Initialize();
      getCI()->getSema().ExternalSource = getTransformer();
   }
   
   clang::CompilerInstance*
   IncrementalASTParser::parse(llvm::StringRef src,
                               clang::ASTConsumer* AddConsumer /* = 0 */)
   {
      // Add src to the memory buffer, parse it, and add it to
      // the AST. Returns the CompilerInstance (and thus the AST).
      // Diagnostics are reset for each call of parse: they are only covering
      // src.
      
      clang::Preprocessor& PP = m_CI->getPreprocessor();
      m_CI->getDiagnosticClient().BeginSourceFile(m_CI->getLangOpts(), &PP);
      
      if (AddConsumer) {
         m_Consumer->Consumers.push_back(AddConsumer);
      }
      
      if (src.size()) {
         std::ostringstream source_name;
         source_name << "input_line_" << (m_MemoryBuffer.size()+1);
         m_MemoryBuffer.push_back( new MutableMemoryBuffer("//cling!\n", source_name.str()) );
         MutableMemoryBuffer *currentBuffer = m_MemoryBuffer.back();
         currentBuffer->append(src);
         clang::FileID FID = m_CI->getSourceManager().createFileIDForMemBuffer(currentBuffer);
         
         PP.EnterSourceFile(FID, 0, clang::SourceLocation());     
         
         clang::Token &tok = const_cast<clang::Token&>(m_Parser->getCurToken());
         tok.setKind(clang::tok::semi);
      }
      
      DiagnosticPrinter* DC = reinterpret_cast<DiagnosticPrinter*>(&m_CI->getDiagnosticClient());
      DC->resetCounts();
      m_CI->getDiagnostics().Reset();
      
      clang::ASTConsumer* Consumer = &m_CI->getASTConsumer();
      clang::Parser::DeclGroupPtrTy ADecl;
      
      bool atEOF = false;
      if (m_Parser->getCurToken().is(clang::tok::eof)) {
         atEOF = true;
      }
      else {
         atEOF = m_Parser->ParseTopLevelDecl(ADecl);
      }
      while (!atEOF) {
         // Not end of file.
         // If we got a null return and something *was* parsed, ignore it.  This
         // is due to a top-level semicolon, an action override, or a parse error
         // skipping something.
         if (ADecl) {
            clang::DeclGroupRef DGR = ADecl.getAsVal<clang::DeclGroupRef>();
            for (clang::DeclGroupRef::iterator i=DGR.begin(); i< DGR.end(); ++i) {
               if (!m_FirstTopLevelDecl)
                  m_FirstTopLevelDecl = *i;
               
               m_LastTopLevelDecl = *i;
               getTransformer()->Visit(m_LastTopLevelDecl);
            } 
            Consumer->HandleTopLevelDecl(DGR);
         } // ADecl
         if (m_Parser->getCurToken().is(clang::tok::eof)) {
            atEOF = true;
         }
         else {
            atEOF = m_Parser->ParseTopLevelDecl(ADecl);
         }
      };
      
      m_Sema->PerformPendingInstantiations();
      
      // Process any TopLevelDecls generated by #pragma weak.
      for (llvm::SmallVector<clang::Decl*,2>::iterator
              I = m_Sema->WeakTopLevelDecls().begin(),
              E = m_Sema->WeakTopLevelDecls().end(); I != E; ++I) {
         Consumer->HandleTopLevelDecl(clang::DeclGroupRef(*I));
      }
      
      clang::ASTContext *Ctx = &m_CI->getASTContext();
      Consumer->HandleTranslationUnit(*Ctx);
      
      if (AddConsumer) {
         m_Consumer->Consumers.pop_back();
      }
      DC->EndSourceFile();
      unsigned err_count = DC->getNumErrors();
      if (err_count) {
         fprintf(stderr, "IncrementalASTParser::parse(): Parse failed!\n");
         emptyLastFunction();
         return 0;
      }
      return m_CI.get();  
   }   
   
   void IncrementalASTParser::emptyLastFunction() {
      // Given a broken AST (e.g. due to a syntax error),
      // replace the last function's body by a null statement.
      
      // Note: this does not touch the identifier table.
      clang::ASTContext& Ctx = m_CI->getASTContext();
      clang::FunctionDecl* F = dyn_cast<clang::FunctionDecl>(m_LastTopLevelDecl);
      if (F && F->getBody()) {
         clang::NullStmt* NStmt = new (Ctx) clang::NullStmt(clang::SourceLocation());
         F->setBody(NStmt);
      }
   }   
} // namespace cling
