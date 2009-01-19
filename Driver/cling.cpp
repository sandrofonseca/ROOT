//------------------------------------------------------------------------------
// CLING - the C++ LLVM-based InterpreterG :)
// version: $Id$
// author:  Lukasz Janyst <ljanyst@cern.ch>
//------------------------------------------------------------------------------

#include <iostream>
#include <vector>
#include <string>

#include <clang/Basic/LangOptions.h>
#include <clang/Basic/TargetInfo.h>

#include <llvm/ADT/OwningPtr.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/Module.h>
#include <llvm/Function.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/System/DynamicLibrary.h>

#include <cling/Interpreter/Interpreter.h>
#include <cling/UserInterface/UserInterface.h>

//------------------------------------------------------------------------------
// Let the show begin
//------------------------------------------------------------------------------
int main( int argc, char **argv )
{

   //---------------------------------------------------------------------------
   // Check if we should run in the "interactive" mode
   //---------------------------------------------------------------------------
   bool interactive = (argc == 1);
   if( !interactive && std::string( argv[1] ) == "-i" )
      interactive = true;

   //---------------------------------------------------------------------------
   // Set up the interpreter
   //---------------------------------------------------------------------------
   clang::LangOptions langInfo;
   langInfo.C99         = 1;
   langInfo.HexFloats   = 1;
   langInfo.BCPLComment = 1; // Only for C99/C++.
   langInfo.Digraphs    = 1; // C94, C99, C++.

   llvm::OwningPtr<clang::TargetInfo> targetInfo;
   targetInfo.reset( clang::TargetInfo::CreateTargetInfo( HOST_TARGET ) );

   cling::Interpreter interpreter( langInfo, targetInfo.get() );

   //---------------------------------------------------------------------------
   // We're supposed to parse a file
   //---------------------------------------------------------------------------
   if( !interactive ) {
      return interpreter.executeFile(argv[1]);
   }
   //----------------------------------------------------------------------------
   // We're interactive
   //----------------------------------------------------------------------------
   else {
      cling::UserInterface ui(interpreter);
      ui.runInteractively();
   }
   return 0;
}
