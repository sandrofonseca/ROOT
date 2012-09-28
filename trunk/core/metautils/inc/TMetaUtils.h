// @(#)root/metautils:$Id$
// Author: Axel Naumann, Nov 2011

/*************************************************************************
 * Copyright (C) 1995-2011, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TMetaUtils
#define ROOT_TMetaUtils

#include "llvm/ADT/SmallSet.h"
#include "llvm/ADT/StringRef.h"

#include <string>

namespace clang {
   class ASTContext;
   class Decl;
   class QualType;
   class CompilerInstance;
   class Module;
   class SourceLocation;
   class Type;
}

namespace cling {
   class Interpreter;
   class LookupHelper;
}


namespace ROOT {
   namespace TMetaUtils {
      
      class TNormalizedCtxt {
         typedef llvm::SmallSet<const clang::Type*, 4> TypesCont_t; 
      private:
         TypesCont_t fTypeToSkip;
      public:
         TNormalizedCtxt(const cling::LookupHelper &lh);

         const TypesCont_t &GetTypeToSkip() const { return fTypeToSkip; }
      };

      // Add default template parameters.
      clang::QualType AddDefaultParameters(clang::QualType instanceType, const cling::Interpreter &interpret, const TNormalizedCtxt &normCtxt);

      // Return the ROOT include directory
      std::string GetROOTIncludeDir(bool rootbuild);

      // Return the dictionary file name for a module
      std::string GetModuleFileName(const char* moduleName);

      // Declare a virtual module.map to clang. Returns Module on success.
      clang::Module* declareModuleMap(clang::CompilerInstance* CI,
                                      const char* moduleFileName,
                                      const char* headers[]);
                                
      // Return the -I needed to find RuntimeUniverse.h
      std::string GetInterpreterExtraIncludePath(bool rootbuild);

      // Return the LLVM / clang resource directory
      std::string GetLLVMResourceDir(bool rootbuild);
      
      // Return the ROOT include directory
      std::string GetROOTIncludeDir(bool rootbuild);

      // Return (in the argument 'output') a mangled version of the C++ symbol/type (pass as 'input')
      // that can be used in C++ as a variable name.
      void GetCppName(std::string &output, const char *input);

      // Return the type name normalized for ROOT,
      // keeping only the ROOT opaque typedef (Double32_t, etc.) and
      // adding default template argument for all types except the STL collections
      // where we remove the default template argument if any.
      void GetNormalizedName(std::string &norm_name, const clang::QualType &type, const cling::Interpreter &interpreter, const TNormalizedCtxt &normCtxt);

      // Returns the comment (// striped away), annotating declaration in a meaningful
      // for ROOT IO way.
      // Takes optional out parameter clang::SourceLocation returning the source 
      // location of the comment.
      //
      // CXXMethodDecls, FieldDecls and TagDecls are annotated.
      // CXXMethodDecls declarations and FieldDecls are annotated as follows:
      // Eg. void f(); // comment1
      //     int member; // comment2
      // Inline definitions of CXXMethodDecls - after the closing ) and before {. Eg:
      // void f() // comment3
      // {...}
      // TagDecls are annotated in the end of the ClassDef macro. Eg.
      // class MyClass {
      // ...
      // ClassDef(MyClass, 1) // comment4
      //
      llvm::StringRef GetComment(const clang::Decl &decl, clang::SourceLocation *loc = 0);

   } // namespace TMetaUtils


} // namespace ROOT

#endif // ROOT_TMetaUtils
