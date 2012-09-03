//===-- BlackList.h - blacklist for sanitizers ------------------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//===----------------------------------------------------------------------===//
//
// This is a utility class for instrumentation passes (like AddressSanitizer
// or ThreadSanitizer) to avoid instrumenting some functions or global
// variables based on a user-supplied blacklist.
//
// The blacklist disables instrumentation of various functions and global
// variables.  Each line contains a prefix, followed by a wild card expression.
// ---
// fun:*_ZN4base6subtle*
// global:*global_with_initialization_problems*
// src:file_with_tricky_code.cc
// ---
// Note that the wild card is in fact an llvm::Regex, but * is automatically
// replaced with .*
// This is similar to the "ignore" feature of ThreadSanitizer.
// http://code.google.com/p/data-race-test/wiki/ThreadSanitizerIgnores
//
//===----------------------------------------------------------------------===//
//

#include "llvm/ADT/StringMap.h"

namespace llvm {
class Function;
class GlobalVariable;
class Module;
class Regex;
class StringRef;

class BlackList {
 public:
  BlackList(const StringRef Path);
  // Returns whether either this function or it's source file are blacklisted.
  bool isIn(const Function &F);
  // Returns whether either this global or it's source file are blacklisted.
  bool isIn(const GlobalVariable &G);
  // Returns whether this module is blacklisted by filename.
  bool isIn(const Module &M);
 private:
  StringMap<Regex*> Entries;

  bool inSection(const StringRef Section, const StringRef Query);
};

}  // namespace llvm
