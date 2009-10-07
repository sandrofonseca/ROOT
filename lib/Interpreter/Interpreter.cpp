//------------------------------------------------------------------------------
// CLING - the C++ LLVM-based InterpreterG :)
// version: $Id$
// author:  Lukasz Janyst <ljanyst@cern.ch>
//------------------------------------------------------------------------------

#include <cling/Interpreter/Interpreter.h>

#include <iostream>
#include <cstdio>

#include "Diagnostics.h"
#include "ParseEnvironment.h"
#include "Visitors.h"
#include "ClangUtils.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/System/Host.h>
#include <llvm/System/Path.h>
#include <llvm/ADT/OwningPtr.h>
#include <llvm/Linker.h>
#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/Target/TargetSelect.h>
#include <llvm/Bitcode/BitstreamWriter.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/Config/config.h>
#include <llvm/Transforms/Utils/Cloning.h>
#include <llvm/LLVMContext.h>
#include <llvm/Support/FormattedStream.h>

#include <clang/Basic/FileManager.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Basic/Version.h>
#include <clang/Lex/HeaderSearch.h>
#include <clang/Lex/Preprocessor.h>
#include <clang/Frontend/InitHeaderSearch.h>
#include <clang/Frontend/TextDiagnosticPrinter.h>
#include <clang/Frontend/CompileOptions.h>
#include "clang/Frontend/InitPreprocessor.h"
#include "clang/Frontend/ASTConsumers.h"
#include <clang/CodeGen/ModuleBuilder.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/ASTConsumer.h>
#include <clang/AST/Decl.h>
#include <clang/AST/DeclarationName.h>
#include <clang/AST/Stmt.h>
#include <clang/Parse/Parser.h>
#include <clang/Basic/TargetInfo.h>
#include <clang/Sema/ParseAST.h>
#include <clang/Lex/LexDiagnostic.h>
#include <clang/Lex/MacroInfo.h>

// Private CLANG headers
#include <Sema/Sema.h>

#include <iostream>
#include <stdexcept>

namespace cling
{
   
   //
   // MacroDetector
   //
   
   class MacroDetector : public clang::PPCallbacks
   {
      
   public:
      
      MacroDetector(const clang::LangOptions& options, unsigned int minpos,clang::SourceManager *sm) 
          : m_options(options), m_minpos(minpos), m_srcMgr(sm) {}
      
      void setSourceManager(clang::SourceManager *sm) { m_srcMgr = sm; }
      std::vector<std::string>& getMacrosVector() { return m_macros; }
      
      void MacroDefined(const clang::IdentifierInfo *II,
                        const clang::MacroInfo *MI) {
         if (MI->isBuiltinMacro() || m_srcMgr==0)
            return;
         clang::FileID mainFileID = m_srcMgr->getMainFileID();
         if (m_srcMgr->getFileID(MI->getDefinitionLoc()) == mainFileID) {
            std::pair<const char*, const char*> buf = m_srcMgr->getBufferData(mainFileID);
            SrcRange range = getMacroRange(MI, *m_srcMgr, m_options);
            if (range.first >= m_minpos) {
               std::string str(buf.first + range.first, range.second - range.first);
               fprintf(stderr,"noticing macro: %s\n",str.c_str());
               m_macros.push_back("#define " + str);
            }
         }
      }
      
      void MacroUndefined(const clang::IdentifierInfo *II,
                          const clang::MacroInfo *MI) {
         if (MI->isBuiltinMacro() || m_srcMgr==0)
            return;
         clang::FileID mainFileID = m_srcMgr->getMainFileID();
         if (m_srcMgr->getFileID(MI->getDefinitionLoc()) == mainFileID) {
            std::pair<const char*, const char*> buf = m_srcMgr->getBufferData(mainFileID);
            SrcRange range = getMacroRange(MI, *m_srcMgr, m_options);
            if (range.first >= m_minpos) {
               std::string str(buf.first + range.first, range.second - range.first);
               size_t pos = str.find(' ');
               if (pos != std::string::npos)
                  str = str.substr(0, pos);
               m_macros.push_back("#undef " + str);
            }
         }
      }
      
   private:
      
      const clang::LangOptions& m_options;
      unsigned int m_minpos;
      clang::SourceManager* m_srcMgr;
      std::vector<std::string> m_macros;
      
   };
   
   //---------------------------------------------------------------------------
   // Constructor
   //---------------------------------------------------------------------------
   Interpreter::Interpreter(clang::LangOptions language):
      m_lang( language ), m_module( 0 )
   {
      m_llvmContext = &llvm::getGlobalContext();
      m_fileMgr    = new clang::FileManager();

      // target:
      llvm::InitializeNativeTarget();
      m_target = clang::TargetInfo::CreateTargetInfo(llvm::sys::getHostTriple());
      {
         llvm::StringMap<bool> Features;
         m_target->getDefaultFeatures("", Features);
         m_target->HandleTargetFeatures(Features);
      }
      m_target->getDefaultLangOptions(language);

      // diagostics:
      m_diagClient = new clang::TextDiagnosticPrinter( llvm::errs() );
      m_diagClient->setLangOptions(&language);
   }

   //---------------------------------------------------------------------------
   // Destructor
   //---------------------------------------------------------------------------
   Interpreter::~Interpreter()
   {
      delete m_fileMgr;
      delete m_diagClient;
      delete m_target;
   }

   //---------------------------------------------------------------------------
   // Add a translation unit
   //---------------------------------------------------------------------------
   bool Interpreter::addUnit( const std::string& fileName )
   {
      //------------------------------------------------------------------------
      // Check if the unit file name was specified and if we alread have
      // an unit named like that
      //------------------------------------------------------------------------
      if( fileName.empty() )
         return false;

      std::map<UnitID_t, UnitInfo_t>::iterator it;
      it = m_units.find( fileName );
      if( it != m_units.end() )
         return false;

      //------------------------------------------------------------------------
      // Process the unit
      //------------------------------------------------------------------------
      ParseEnvironment *pEnv = parseFile( fileName );
      clang::TranslationUnitDecl* tu = pEnv->getASTContext()->getTranslationUnitDecl();
      bool res = addUnit( fileName, tu );
      delete pEnv;
      return res;
   }

   //---------------------------------------------------------------------------
   // Add a translation unit
   //---------------------------------------------------------------------------
   bool Interpreter::addUnit( const llvm::MemoryBuffer* buffer, UnitID_t& id )
   {
      return false;
   }

   //---------------------------------------------------------------------------
   // Remove the translation unit
   //---------------------------------------------------------------------------
   bool Interpreter::removeUnit( const UnitID_t& id )
   {
      return false;
   }

   //---------------------------------------------------------------------------
   // Get a compiled  module linking together all translation units
   //---------------------------------------------------------------------------
   llvm::Module* Interpreter::getModule()
   {
      return m_module;
   }

   //---------------------------------------------------------------------------
   // Compile the filename and link it to all the modules known to the
   // compiler but do not add it to the list
   //---------------------------------------------------------------------------
   llvm::Module* Interpreter::linkFile( const std::string& fileName,
                                          std::string* errMsg )
   {
      ParseEnvironment *pEnv = parseFile( fileName );
      clang::TranslationUnitDecl* tu = pEnv->getASTContext()->getTranslationUnitDecl();
      llvm::Module*           module = compile( tu );
      llvm::Module*           result = linkModule( module, errMsg );
      delete pEnv;
      delete module;
      return result;
   }

   //---------------------------------------------------------------------------
   // Compile the buffer and link it to all the modules known to the
   // compiler but do not add it to the list
   //---------------------------------------------------------------------------
   llvm::Module* Interpreter::linkSource ( const std::string& source,
                                                 std::string* errMsg )
   {
      //------------------------------------------------------------------------------
      // String constants - MOVE SOMEWHERE REASONABLE!
      //------------------------------------------------------------------------------
      static const std::string code_prefix = "#include <stdio.h>\nint imain(int argc, char** argv) {\n";
      static const std::string code_suffix = ";\nreturn 0; } ";
      
      std::vector<std::string> statements;
      splitInput(source, statements);

      //----------------------------------------------------------------------
      // Wrap the code
      //----------------------------------------------------------------------
      std::string wrapped = code_prefix + source + code_suffix;

      ParseEnvironment *pEnv = parseSource( wrapped );
      clang::TranslationUnitDecl* tu = pEnv->getASTContext()->getTranslationUnitDecl();
      llvm::Module*           module = compile( tu );
      llvm::Module*           result = linkModule( module, errMsg );
      delete tu;
      delete module;
      return result;
   }

   //---------------------------------------------------------------------------
   // Link the module to all the modules known to the compiler but do
   // not add it to the list
   //---------------------------------------------------------------------------
   llvm::Module* Interpreter::linkModule( llvm::Module *module, std::string* errMsg )
   {
      if( !module ) {
         if (errMsg) *errMsg = "Module is NULL";
         return 0;
      }

      //------------------------------------------------------------------------
      // We have some module so we should link the current one to it
      //------------------------------------------------------------------------
      llvm::Linker linker( "executable", llvm::CloneModule(module) );

      if( m_module )
         if (linker.LinkInModule( llvm::CloneModule( m_module ), errMsg ))
            return 0;

      return linker.releaseModule();
   }

   //---------------------------------------------------------------------------
   // Parse memory buffer
   //---------------------------------------------------------------------------
   ParseEnvironment* Interpreter::parseSource( const std::string& source )
   {
      //------------------------------------------------------------------------
      // Create memory buffer
      //------------------------------------------------------------------------
      llvm::MemoryBuffer* buff = llvm::MemoryBuffer::getMemBufferCopy(&*source.begin(),
                                                                      &*source.end(),
                                                                      "CLING" );
      //------------------------------------------------------------------------
      // Create a file manager
      //------------------------------------------------------------------------
      llvm::OwningPtr<clang::SourceManager> srcMgr( new clang::SourceManager() );

      //------------------------------------------------------------------------
      // Register with the source manager
      //------------------------------------------------------------------------
      if( buff )
         srcMgr->createMainFileIDForMemBuffer( buff );

      if( srcMgr->getMainFileID().isInvalid() )
         return 0;

      return parse( srcMgr.get() );
   }

   //---------------------------------------------------------------------------
   // Parse file
   //---------------------------------------------------------------------------
   ParseEnvironment* Interpreter::parseFile( const std::string& fileName )
   {
      //------------------------------------------------------------------------
      // Create a file manager
      //------------------------------------------------------------------------
      llvm::OwningPtr<clang::SourceManager> srcMgr( new clang::SourceManager() );

      //------------------------------------------------------------------------
      // Feed in the file
      //------------------------------------------------------------------------
      const clang::FileEntry *file = m_fileMgr->getFile( fileName );
      if( file )
         srcMgr->createMainFileID( file, clang::SourceLocation() );

      if( srcMgr->getMainFileID().isInvalid() )
         return 0;

      return parse( srcMgr.get() );
   }
   
   //---------------------------------------------------------------------------
   // Parse
   //---------------------------------------------------------------------------
   ParseEnvironment* Interpreter::parse( clang::SourceManager* srcMgr )
   {
      //-------------------------------------------------------------------------
      // Create diagnostics
      //-------------------------------------------------------------------------
      clang::Diagnostic diag( m_diagClient );
      diag.setSuppressSystemWarnings( true );

      //------------------------------------------------------------------------
      // Setup a parse environement
      //------------------------------------------------------------------------
      ParseEnvironment *pEnv = new ParseEnvironment(m_lang, *m_target, &diag, m_fileMgr, srcMgr);
      
      //clang::ASTConsumer dummyConsumer;
      llvm::raw_stdout_ostream out;
      //llvm::raw_null_ostream out;
      clang::ASTConsumer* dummyConsumer = clang::CreateASTPrinter(&out);
      
      clang::Sema   sema(*pEnv->getPreprocessor(), *pEnv->getASTContext(), *dummyConsumer);
      clang::Parser p(*pEnv->getPreprocessor(), sema);

      pEnv->getPreprocessor()->EnterMainSourceFile();

      // The following is similar to clang::Parser::ParseTranslationUnit
      // except that we add insertDeclarations
      // and remove the ExitScope.

//      clang::TranslationUnitDecl *tu = pEnv->getASTContext()->getTranslationUnitDecl();
//
//      p.Initialize();
//
//      insertDeclarations( tu, &sema );
//
//      clang::Parser::DeclGroupPtrTy adecl;
//  
//      while( !p.ParseTopLevelDecl(adecl) ) {}
      
      p.ParseTranslationUnit();

      // dumpTU(tu);

      return pEnv;
   }

   //----------------------------------------------------------------------------
   // Compile the translation unit
   //----------------------------------------------------------------------------
   llvm::Module* Interpreter::compile( clang::TranslationUnitDecl* tu )
   {
      if( !tu )
         return 0;

      //-------------------------------------------------------------------------
      // Create diagnostics
      //-------------------------------------------------------------------------
      clang::Diagnostic diag( m_diagClient );
      diag.setSuppressSystemWarnings( true );

      //-------------------------------------------------------------------------
      // Create the code generator
      //-------------------------------------------------------------------------
      llvm::OwningPtr<clang::CodeGenerator> codeGen;
      clang::CompileOptions options;
      codeGen.reset(CreateLLVMCodeGen(diag, "SOME NAME [Interpreter::compile()]",
                                      options, *m_llvmContext));

      //-------------------------------------------------------------------------
      // Loop over the AST
      //-------------------------------------------------------------------------
      
      codeGen->Initialize(tu->getASTContext());

      for( clang::TranslationUnitDecl::decl_iterator it = tu->decls_begin(),
              itE = tu->decls_end(); it != itE; ++it )
         codeGen->HandleTopLevelDecl(clang::DeclGroupRef(*it));

      codeGen->HandleTranslationUnit(tu->getASTContext());

      //-------------------------------------------------------------------------
      // Return the module
      //-------------------------------------------------------------------------
      llvm::Module* module = codeGen->ReleaseModule();
      return module;
   }

   //----------------------------------------------------------------------------
   // Add the translation unit
   //----------------------------------------------------------------------------
   bool Interpreter::addUnit( const UnitID_t& id, clang::TranslationUnitDecl* tu )
   {
      //-------------------------------------------------------------------------
      // Check if we've got a valid translation unit
      //-------------------------------------------------------------------------
      if( !tu )
         return false;

      //-------------------------------------------------------------------------
      // Process the unit
      //-------------------------------------------------------------------------
      UnitInfo_t uinfo;
      uinfo.ast    = tu;
      uinfo.decls  = extractDeclarations( tu );
      uinfo.module = compile( tu );

      if( !uinfo.module ) {
         delete tu;
         return false;
      }

      //-------------------------------------------------------------------------
      // Linke in the module
      //-------------------------------------------------------------------------
      llvm::Module* module = linkModule( uinfo.module );
      if( !module ) {
         delete tu;
         delete module;
         return false;
      }

      //-------------------------------------------------------------------------
      // Everything went ok - register
      //-------------------------------------------------------------------------
      m_units[id] = uinfo;
      m_module = module;
      
      return true;
   }

   //----------------------------------------------------------------------------
   // Extract the function declarations
   //----------------------------------------------------------------------------
   std::vector<clang::Decl*>
   Interpreter::extractDeclarations( clang::TranslationUnitDecl* tu )
   {
      std::vector<clang::Decl*> vect;
      if( !tu )
         return vect;

      std::set<clang::Decl*> decls_before;
      std::vector<std::pair<clang::Decl*, const clang::ASTContext*> >::iterator
         dit, ditend = m_decls.end();
      for (dit = m_decls.begin(); dit != ditend; ++dit)
         decls_before.insert(dit->first);

      //-------------------------------------------------------------------------
      // Loop over the declarations
      //-------------------------------------------------------------------------
      clang::ASTContext& astContext = tu->getASTContext();
      //const clang::SourceManager& srcMgr = astContext.getSourceManager();
      for( clang::TranslationUnitDecl::decl_iterator it = tu->decls_begin(),
              itend = tu->decls_end(); it != itend; ++it ) {
         if (it->getKind() == clang::Decl::Function) {
            clang::FunctionDecl* decl = static_cast<clang::FunctionDecl*>( *it );
            if( decl && decls_before.find(decl) == decls_before.end()) {
               vect.push_back( decl );
               m_decls.push_back( std::make_pair(*it, &astContext ) );
            }
         }
      }
      return vect;
   }

   //----------------------------------------------------------------------------
   // Insert the implicit declarations to the translation unit
   //----------------------------------------------------------------------------
   void Interpreter::insertDeclarations( clang::TranslationUnitDecl* tu, clang::Sema* sema )
   {
      clang::ASTContext& astContext = tu->getASTContext();
      clang::IdentifierTable&      table    = astContext.Idents;
      clang::DeclarationNameTable& declTab  = astContext.DeclarationNames;
      std::vector<std::pair<clang::Decl*, const clang::ASTContext*> >::iterator it;
      for( it = m_decls.begin(); it != m_decls.end(); ++it ) {
         if (it->first->getKind() == clang::Decl::Function) {
            clang::FunctionDecl* func = static_cast<clang::FunctionDecl*>( it->first );
            if( func ) {
               clang::IdentifierInfo&       id       = table.get(std::string(func->getNameAsString()));
               clang::DeclarationName       dName    = declTab.getIdentifier( &id );

               clang::FunctionDecl* decl = clang::FunctionDecl::Create( astContext,
                                                                        tu,
                                                                        func->getLocation(),
                                                                        dName,
                                                                        typeCopy( func->getType(), *it->second, astContext ),
                                                                        0 /*DeclInfo*/);
               tu->addDecl( decl );
               sema->IdResolver.AddDecl( decl );
               if (sema->TUScope)
                  sema->TUScope->AddDecl( clang::Action::DeclPtrTy::make(decl) );
            }
         }
      }
   }

   //----------------------------------------------------------------------------
   // Dump the translation unit
   //----------------------------------------------------------------------------
   void Interpreter::dumpTU( clang::DeclContext* dc )
   {
      for (clang::DeclContext::decl_iterator it = dc->decls_begin(),
              itE = dc->decls_end(); it != itE; ++it ) {
         clang::Stmt* body = (*it) ? (*it)->getBody() : 0;
         if( body ) {
            std::cerr << "--- AST ---" << std::endl;
            body->dumpAll();
            std::cerr<<std::endl;
         }
      }
   }

   //-----------------------------------------------------------------------------
   // Copy given type to the target AST context using serializers - grr ugly :)
   //-----------------------------------------------------------------------------
   clang::QualType Interpreter::typeCopy( clang::QualType source,
                                          const clang::ASTContext& sourceContext,
                                          clang::ASTContext& targetContext )
   {
      const clang::BuiltinType*       bt;
      const clang::FunctionType*      ft1;
      const clang::FunctionProtoType* ft2;

      //--------------------------------------------------------------------------
      // Deal with a builtin type
      //--------------------------------------------------------------------------
      if( (bt = source.getTypePtr()->getAs<clang::BuiltinType>()) ) {
         return clang::QualType( bt->getCanonicalTypeInternal().getUnqualifiedType().getTypePtr () 
                                 /*was: targetContext.getBuiltinType( bt->getKind() ).getTypePtr()*/,
                                 source.getCVRQualifiers() );
      }

      //--------------------------------------------------------------------------
      // Deal with a pointer type
      //--------------------------------------------------------------------------
      else if( source.getTypePtr()->isPointerType() ) {
         const clang::PointerType* pt = source.getTypePtr()->getAs<clang::PointerType>();
         clang::QualType pointee = typeCopy( pt->getPointeeType(),
                                             sourceContext,
                                             targetContext );
         clang::QualType result = targetContext.getPointerType( pointee );
         result.setFastQualifiers( source.getFastQualifiers() );
         return result;
      }

      //--------------------------------------------------------------------------
      // Deal with a function type
      //--------------------------------------------------------------------------
      else if( source.getTypePtr()->isFunctionType() ) {
         ft1  = static_cast<clang::FunctionType*>(source.getTypePtr());

         //-----------------------------------------------------------------------
         // No parameters
         //-----------------------------------------------------------------------
         if( ft1->getTypeClass() != clang::Type::FunctionProto )
            return targetContext.getFunctionNoProtoType( typeCopy( ft1->getResultType(),
                                                                   sourceContext,
                                                                   targetContext ) );

         ft2  = static_cast<clang::FunctionProtoType*>(source.getTypePtr());
         //-----------------------------------------------------------------------
         // We have some parameters
         //-----------------------------------------------------------------------
         std::vector<clang::QualType> args;
         clang::FunctionProtoType::arg_type_iterator it;
         for( it = ft2->arg_type_begin(); it != ft2->arg_type_end(); ++it )
            args.push_back( typeCopy( *it, sourceContext, targetContext ) );

         return targetContext.getFunctionType( typeCopy( ft2->getResultType(),
                                                         sourceContext,
                                                         targetContext ),
                                               &args.front(), args.size(),
                                               ft2->isVariadic(),
                                               ft2->getTypeQuals() );
      }

      assert("Unable to convert type");
      return source;
   }

   /*
     Trying to use llvm::CloneModule() instead...

   //-----------------------------------------------------------------------------
   // Create a copy of an existing module - the linker destroys the source!!
   //-----------------------------------------------------------------------------
   llvm::Module* Interpreter::copyModule( const llvm::Module* src )
   {
      //--------------------------------------------------------------------------
      // Create the buffer and the writer
      //--------------------------------------------------------------------------
      std::vector<unsigned char> buffer;
      llvm::BitstreamWriter      stream(buffer);
      buffer.reserve(256*1024);

      //--------------------------------------------------------------------------
      // Write the module
      //--------------------------------------------------------------------------
      llvm::WriteBitcodeToStream( src, stream );

      //--------------------------------------------------------------------------
      // Create a memory buffer and read the module back
      //--------------------------------------------------------------------------
      buffer.push_back( 0 ); // required by the llvm::MemoryBuffer
      llvm::MemoryBuffer* buff = llvm::MemoryBuffer::getMemBuffer( (const char*)&buffer.front(),
                                                                   (const char*)&buffer.back(),
                                                                   src->getModuleIdentifier().c_str() );
      llvm::Module* result = ParseBitcodeFile( buff );

      //--------------------------------------------------------------------------
      // Cleanup and return
      //--------------------------------------------------------------------------
      delete buff;
      return result;
   }
   */

   //---------------------------------------------------------------------------
   // Call the Interpreter on a Module
   //---------------------------------------------------------------------------
   int Interpreter::executeModuleMain( llvm::Module *module,
                                       const std::string& name)
   {
      //---------------------------------------------------------------------------
      // Create the execution engine
      //---------------------------------------------------------------------------
      llvm::EngineBuilder builder(module);
      std::string errMsg;
      builder.setErrorStr(&errMsg);
      builder.setEngineKind(llvm::EngineKind::JIT);
      llvm::OwningPtr<llvm::ExecutionEngine> engine( builder.create() );

      if( !engine ) {
         std::cout << "[!] Unable to create the execution engine! (" << errMsg << ")" << std::endl;
         return 1;
      }

      //---------------------------------------------------------------------------
      // Look for the imain function
      //---------------------------------------------------------------------------
      llvm::Function* func( module->getFunction( name ) );
      if( !func ) {
         std::cerr << "[!] Cannot find the entry function " << name << "!" << std::endl;
         return 1;
      }

      // Run static constructors.
      engine->runStaticConstructorsDestructors(false);

      //---------------------------------------------------------------------------
      // Create argv
      //---------------------------------------------------------------------------
      std::vector<std::string> params;
      params.push_back( "executable" );

      return engine->runFunctionAsMain( func,  params, 0 );   
   }


   //---------------------------------------------------------------------------
   // Call the Interpreter on a File
   //---------------------------------------------------------------------------
   int Interpreter::executeFile( const std::string& filename,
                                 const std::string& funcname)
   {
      llvm::Module* module = linkFile( filename );
      if(!module) {
         std::cerr << "[!] Errors occured while parsing file " << filename << "!" << std::endl;
         return 1;
      }
      std::string myfuncname(funcname);
      if (funcname == "()") {
         size_t posSlash = filename.find_last_of('/');
         ++posSlash; // npos to 0, good!
         size_t posDot = filename.find('.'); // npos is OK, too.
         myfuncname = filename.substr(posSlash, posDot);
      }

      return executeModuleMain( module, myfuncname );
   }
   
   Interpreter::InputType Interpreter::analyzeInput(const std::string& contextSource,
                                                    const std::string& line,
                                                    int& indentLevel,
                                                    std::vector<clang::FunctionDecl*> *fds)
   {
      // Check if there is an explicitation continuation character.
      if (line.length() > 1 && line[line.length() - 2] == '\\') {
         indentLevel = 1;
         return Incomplete;
      }
      
      //-------------------------------------------------------------------------
      // Create diagnostics
      //-------------------------------------------------------------------------
      ProxyDiagnosticClient tokpdc(NULL);
      clang::Diagnostic tokdiag( &tokpdc );
      tokdiag.setSuppressSystemWarnings( true );
      
      //------------------------------------------------------------------------
      // Setup a parse environement
      //------------------------------------------------------------------------
      ParseEnvironment pEnv(m_lang, *m_target, &tokdiag, m_fileMgr);
      
      //------------------------------------------------------------------------
      // Register with the source manager
      //------------------------------------------------------------------------
      llvm::MemoryBuffer* buffer = llvm::MemoryBuffer::getMemBufferCopy(&*line.begin(),
                                                                        &*line.end(),
                                                                        "CLING" );
      pEnv.getSourceManager()->createMainFileIDForMemBuffer( buffer );
      
      if( pEnv.getSourceManager()->getMainFileID().isInvalid() )
         return Incomplete;
      
      // Check the tokens.
      clang::Token lastTok;
      bool tokWasDo = false;
      int stackSize = analyzeTokens( *pEnv.getPreprocessor(), lastTok, indentLevel, tokWasDo);
      if (stackSize < 0) 
         return TopLevel;
      
      // tokWasDo is used for do { ... } while (...); loops
      if (lastTok.is(clang::tok::semi) || (lastTok.is(clang::tok::r_brace) && !tokWasDo)) {
         if (stackSize > 0) return Incomplete;

         ProxyDiagnosticClient pdc(NULL);
         clang::Diagnostic diag(&pdc);
         // Setting this ensures "foo();" is not a valid top-level declaration.
         diag.setDiagnosticMapping(clang::diag::ext_missing_type_specifier,
                                   clang::diag::MAP_ERROR);
         diag.setSuppressSystemWarnings(true);
         std::string src = contextSource + buffer->getBuffer().str();
         struct : public clang::ASTConsumer {
            bool hadIncludedDecls;
            unsigned pos;
            unsigned maxPos;
            clang::SourceManager *sm;
            std::vector<clang::FunctionDecl*> fds;
            void HandleTopLevelDecl(clang::DeclGroupRef D) {
               for (clang::DeclGroupRef::iterator I = D.begin(), E = D.end(); I != E; ++I) {
                  if (clang::FunctionDecl *FD = dyn_cast<clang::FunctionDecl>(*I)) {
                     clang::SourceLocation Loc = FD->getTypeSpecStartLoc();
                     if (!Loc.isValid())
                        continue;
                     if (sm->isFromMainFile(Loc)) {
                        unsigned offset = sm->getFileOffset(sm->getInstantiationLoc(Loc));
                        if (offset >= pos) {
                           fds.push_back(FD);
                        }
                     } else {
                        while (!sm->isFromMainFile(Loc)) {
                           const clang::SrcMgr::SLocEntry& Entry =
									sm->getSLocEntry(sm->getFileID(sm->getSpellingLoc(Loc)));
                           if (!Entry.isFile())
                              break;
                           Loc = Entry.getFile().getIncludeLoc();
                        }
                        unsigned offset = sm->getFileOffset(Loc);
                        if (offset >= pos) {
                           hadIncludedDecls = true;
                        }
                     }
                  }
               }
            }
         } consumer;
         // Need to reset the preprocessor.
         ParseEnvironment pEnvCheck(m_lang, *m_target, &diag, m_fileMgr);
         consumer.hadIncludedDecls = false;
         consumer.pos = contextSource.length();
         consumer.maxPos = consumer.pos + buffer->getBuffer().size();
         consumer.sm = pEnvCheck.getSourceManager();  
         buffer = llvm::MemoryBuffer::getMemBufferCopy(&*line.begin(),
                                                       &*line.end(),
                                                       "CLING" );
         pEnvCheck.getSourceManager()->createMainFileIDForMemBuffer( buffer );
         clang::ParseAST( *pEnvCheck.getPreprocessor(), &consumer, *pEnvCheck.getASTContext());
         if (pdc.hadError(clang::diag::err_unterminated_block_comment))
            return Incomplete;
         if (!pdc.hadErrors() && (!consumer.fds.empty() || consumer.hadIncludedDecls)) {
            if (!consumer.fds.empty())
               fds->swap(consumer.fds);
            return TopLevel;
         }
         return Stmt;
      }
      
      return Incomplete;
   }
   
   int Interpreter::analyzeTokens(clang::Preprocessor& PP,
                                  clang::Token& lastTok,
                                  int& indentLevel,
                                  bool& tokWasDo)
   {
      int result;
      std::stack<std::pair<clang::Token, clang::Token> > S; // Tok, PrevTok
      
      indentLevel = 0;
      PP.EnterMainSourceFile();
      
      clang::Token Tok;
      PP.Lex(Tok);
      while (Tok.isNot(clang::tok::eof)) {
         if (Tok.is(clang::tok::l_square)) {
            S.push(std::make_pair(Tok, lastTok)); // [
         } else if (Tok.is(clang::tok::l_paren)) {
            S.push(std::make_pair(Tok, lastTok)); // (
         } else if (Tok.is(clang::tok::l_brace)) {
            S.push(std::make_pair(Tok, lastTok)); // {
            indentLevel++;
         } else if (Tok.is(clang::tok::r_square)) {
            if (S.empty() || S.top().first.isNot(clang::tok::l_square)) {
               std::cout << "Unmatched [\n";
               return -1;
            }
            tokWasDo = false;
            S.pop();
         } else if (Tok.is(clang::tok::r_paren)) {
            if (S.empty() || S.top().first.isNot(clang::tok::l_paren)) {
               std::cout << "Unmatched (\n";
               return -1;
            }
            tokWasDo = false;
            S.pop();
         } else if (Tok.is(clang::tok::r_brace)) {
            if (S.empty() || S.top().first.isNot(clang::tok::l_brace)) {
               std::cout << "Unmatched {\n";
               return -1;
            }
            tokWasDo = S.top().second.is(clang::tok::kw_do);
            S.pop();
            indentLevel--;
         }
         lastTok = Tok;
         PP.Lex(Tok);
      }
      result = S.size();
      
      // TODO: We need to properly account for indent-level for blocks that do not
      //       have braces... such as:
      //
      //       if (X)
      //         Y;
      //
      // TODO: Do-while without braces doesn't work, e.g.:
      //
      //       do
      //         foo();
      //       while (bar());
      //
      // Both of the above could be solved by some kind of rewriter-pass that would
      // insert implicit braces (or simply a more involved analysis).
      
      // Also try to match preprocessor conditionals...
      if (result == 0) {
         clang::Lexer Lexer(PP.getSourceManager().getMainFileID(),
                            PP.getSourceManager(),
                            PP.getLangOptions());
         Lexer.LexFromRawLexer(Tok);
         while (Tok.isNot(clang::tok::eof)) {
            if (Tok.is(clang::tok::hash)) {
               Lexer.LexFromRawLexer(Tok);
               if (clang::IdentifierInfo *II = PP.LookUpIdentifierInfo(Tok)) { 
                  switch (II->getPPKeywordID()) {
                     case clang::tok::pp_if:
                     case clang::tok::pp_ifdef:
                     case clang::tok::pp_ifndef:
                        result++;
                        break;
                     case clang::tok::pp_endif:
                        if (result == 0)
                           return -1; // Nesting error.
                        result--;
                        break;
                     default:
                        break;
                  }
               }
            }
            Lexer.LexFromRawLexer(Tok);
         }
      }
      
      return result;
   }
   
   bool Interpreter::splitInput(const std::string& input, std::vector<std::string>& statements) 
   {
      // Insert the environment so that the code input
      // to insure all the appropriate symbols are defined.
      std::string src = ""; // prefix source;
      
      unsigned int newpos = src.length();
      
      // Introduce a specific synthetic function so that
      // we can find the 
      src += "void __cling_internal() {\n";
      // const unsigned pos = src.length();
      src += input;
      src += "\n}\n";
 
      fprintf(stderr,"code:\n%s\n",src.c_str());
      
      clang::Diagnostic diag( m_diagClient );
      diag.setSuppressSystemWarnings( true );
      // Set offset on the diagnostics provider.
      // diag.setOffset(pos);
      
      std::vector<clang::Stmt*> stmts;
      
      MacroDetector* macros = new MacroDetector(m_lang, newpos, 0);
      ParseEnvironment pEnv(m_lang, *m_target, &diag, m_fileMgr, 0, macros);

      clang::SourceManager *sm = pEnv.getSourceManager();
      macros->setSourceManager( sm );
      StmtSplitter splitter(src, *sm, m_lang, &stmts);
      FunctionBodyConsumer<StmtSplitter> consumer(&splitter, "__cling_internal");

      fprintf(stderr, "Parsing in splitInput()...\n");

      llvm::MemoryBuffer* buffer = llvm::MemoryBuffer::getMemBufferCopy(&*src.begin(),
                                                                        &*src.end(),
                                                                        "CLING" );
      pEnv.getSourceManager()->createMainFileIDForMemBuffer( buffer );
      clang::ParseAST( *pEnv.getPreprocessor(), &consumer, *pEnv.getASTContext());

      // TODO: In order to properly support unnamed macro which could contains
      // interleaving of #define, #undef and global and to properly support
      // #define and #undef inside class and struct declaration, we will
      // need to 'merge' this list and the list of stmts.
      for(unsigned int x = 0; x < macros->getMacrosVector().size(); ++x) {
         fprintf(stderr,"found macro: %s\n",macros->getMacrosVector()[x].c_str());
      }
      
      statements.clear();
      if (diag.hasErrorOccurred()) {
         return false;
      } else {
         for (unsigned i = 0; i < stmts.size(); i++) {
            SrcRange range = getStmtRangeWithSemicolon(stmts[i], *sm, m_lang);
            std::string s = src.substr(range.first, range.second - range.first);

            fprintf(stderr, "Split %d is: %s\n", i, s.c_str());

            const clang::Expr *E = dyn_cast<clang::Expr>(stmts[i]);
            if (E) {
               fprintf(stderr,"Has expression: %s\n",s.c_str());
            } else {
               const clang::DeclStmt *DS = dyn_cast<clang::DeclStmt>(stmts[i]);
               if (DS) {
                  fprintf(stderr,"Has declaration: %s\n",s.c_str());
               } else {
                  fprintf(stderr,"Has something else: %s\n",s.c_str());
               }
            }
            statements.push_back(s);
         }
      }
      return true;
      
   }
}
