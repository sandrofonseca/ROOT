// @(#)root/html:$Id$
// Author: Nenad Buncic   18/10/95

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_THtml
#define ROOT_THtml


////////////////////////////////////////////////////////////////////////////
//                                                                        //
// THtml                                                                  //
//                                                                        //
// Html generates documentation for all ROOT classes                      //
// using XHTML 1.0 transitional                                           //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

#ifndef ROOT_THashList
#include "THashList.h"
#endif

#ifndef ROOT_THashTable
#include "THashTable.h"
#endif

#ifndef ROOT_TExMap
#include "TExMap.h"
#endif

#include <map>

class TClass;
class TClassDocInfo;
class TVirtualMutex;

class THtml: public TObject {
public:
   //______________________________________________________________
   // Helper base class.
   class THelperBase: public TObject {
   public:
      THelperBase(): fHtml(0) {}
      virtual ~THelperBase();
      void    SetOwner(THtml* html);
      THtml*  GetOwner() const { return fHtml; }
   private:
      THtml*  fHtml; // object owning the helper
      ClassDef(THelperBase, 0); // a helper object's base class
   };

   //______________________________________________________________
   // Helper class to translate between classes and their
   // modules. Can be derived from and thus replaced by
   // the user; see THtml::SetModuleDefinition().
   class TModuleDefinition: public THelperBase {
   public:
      virtual bool GetModule(TClass* cl, TString& out_modulename) const;
      ClassDef(TModuleDefinition, 0); // helper class to determine a class's module
   };

   //______________________________________________________________
   // Helper class to translate between classes and their
   // filenames. Can be derived from and thus replaced by
   // the user; see THtml::SetFileDefinition().
   class TFileDefinition: public THelperBase {
   public:
      virtual bool GetDeclFileName(const TClass* cl, TString& out_filename, TString& out_fsys) const;
      virtual bool GetImplFileName(const TClass* cl, TString& out_filename, TString& out_fsys) const;
   protected:
      virtual bool GetFileName(const TClass* cl, bool decl, TString& out_filename, TString& out_fsys) const;

      void SplitClassIntoDirFile(const TString& clname, TString& dir, TString& filename) const;
      void ExpandSearchPath(TString& path) const;
      ClassDef(TFileDefinition, 0); // helper class to determine a class's source files
   };

   //______________________________________________________________
   // Helper class to translate between file names and their
   // version used for documentation. Can be derived from and thus
   // replaced by the user; see THtml::SetPathDefinition().
   class TPathDefinition: public THelperBase {
   public:
      virtual bool GetMacroPath(const TClass* cl, TString& out_dir) const;
      virtual bool GetIncludeAs(TClass* cl, TString& out_include_as) const;
      virtual bool GetFileNameFromInclude(const char* included, TString& out_fsname) const;
      virtual bool GetDocDir(const TString& module, TString& doc_dir) const;

      // Set the include path; first matching element (separated by ":") will be
      // removed from classes' 
      virtual void SetIncludePath(const char* include) { fIncludePath = include; }
   protected:
      TString        fIncludePath;     // include path, removed from DeclFileName() for documentation ("include" for ROOT)
      ClassDef(TPathDefinition, 0); // helper class to determine directory layouts
   };

   class TFileSysDir;
   class TFileSysDB;
   //______________________________________________________________
   // Utility class representing a directory entry
   class TFileSysEntry: public TObject {
   public:
      TFileSysEntry(const char* name, TFileSysDir* parent):
         fName(name), fParent(parent), fLevel(parent ? parent->GetLevel() + 1 : 0) {}
      const char* GetName() const { return fName; }
      void GetFullName(TString& fullname) const {
         if (fParent) {
            fParent->GetFullName(fullname);
            fullname += "/";
         }
         fullname += fName;
      }

      TFileSysDir* GetParent() const { return fParent; }
      Int_t GetLevel() const { return fLevel; }
   private:
      TString      fName; // name of the element
      TFileSysDir* fParent; // parent directory
      Int_t        fLevel; // level of directory
      ClassDef(TFileSysEntry, 0); // an entry of the local file system
   };

   //______________________________________________________________
   // Utility class representing a directory
   class TFileSysDir: public TFileSysEntry {
   public:
      TFileSysDir(const char* name, TFileSysDir* parent):
         TFileSysEntry(name, parent)
      { fFiles.SetOwner(); fDirs.SetOwner(); }
      const TList* GetFiles() const { return &fFiles; }
      const TList* GetSubDirs() const { return &fDirs; }

      void Recurse(TFileSysDB* db, const char* path);

   private:
      TList fFiles;
      TList fDirs;
      ClassDef(TFileSysDir, 0); // an directory if the local file system
   };

   //______________________________________________________________
   // Utility class representing a directory
   class TFileSysDB: public TFileSysDir {
   public:
      TFileSysDB(const char* path, const char* ignore, Int_t maxdirlevel):
         TFileSysDir(path, 0), fIgnorePath(ignore), fMaxLevel(maxdirlevel)
      { Fill(); }

      TExMap& GetMapIno() { return fMapIno; }
      THashTable& GetEntries() { return fEntries; }
      const TString& GetIgnore() const { return fIgnorePath; }
      Int_t   GetMaxLevel() const { return fMaxLevel; }

   protected:
      void Fill() { Recurse(this, GetName()); }

   private:
      TExMap   fMapIno; // inode to TFileSysDir map, to detect softlinks
      THashTable fEntries; // hash map of all filenames without paths
      TString  fIgnorePath; // regexp of path to ignore while building entry tree
      Int_t    fMaxLevel; // maximum level of directory nesting
      ClassDef(TFileSysDB, 0); // instance of file system data
   };


   //______________________________________________________________
   // Configuration holder for path related settings
   struct TPathInfo {
      enum EDotAccess {
         kDotUnknown,
         kDotFound,
         kDotNotFound
      };

      TPathInfo():
         fFoundDot(kDotUnknown),
#ifdef R__WIN32
         fInputPath("./;src/;include/"),
#else
         fInputPath("./:src/:include/"),
#endif
         // .whatever implicitly ignored, no need to add .svn!
         fIgnorePath("\\b(include|CVS)\\b"),
         fDocPath("doc"),
         fMacroPath(".:doc/macros"),
         fOutputDir("htmldoc") {}

      EDotAccess     fFoundDot;        // whether dot is accessible
      TString        fInputPath;       // directories to look for classes; prepended to Decl/ImplFileName()
      TString        fIgnorePath;      // regexp pattern for directories to ignore ("\b(CVS|\.svn)\b") for ROOT
      TString        fDocPath;         // subdir to check for module documentation ("doc" for ROOT)
      TString        fMacroPath;       // subdir for macros run via the Begin/End Macro directive; ("doc/macros:." for ROOT)
      TString        fDotDir;          // directory of GraphViz's dot binary
      TString        fEtcDir;          // directory containing auxiliary files
      TString        fOutputDir;       // output directory
   };


public:
   THtml();
   virtual      ~THtml();

   static void   LoadAllLibs();

   // Functions to generate documentation
   void          Convert(const char *filename, const char *title, 
                         const char *dirname = "", const char *relpath="../");
   void          CreateHierarchy();
   void          MakeAll(Bool_t force=kFALSE, const char *filter="*",
                         int numthreads = 1);
   void          MakeClass(const char *className, Bool_t force=kFALSE);
   void          MakeIndex(const char *filter="*");
   void          MakeTree(const char *className, Bool_t force=kFALSE);

   // Configuration setters
   void          SetModuleDefinition(const TModuleDefinition& md);
   void          SetFileDefinition(const TFileDefinition& fd);
   void          SetPathDefinition(const TPathDefinition& pd);
   void          SetProductName(const char* product) { fProductName = product; }
   void          SetOutputDir(const char *dir) { fPathInfo.fOutputDir = dir; }
   void          SetInputDir(const char *dir);
   void          SetEtcDir(const char* dir) { fPathInfo.fEtcDir = dir; }
   void          SetDocPath(const char* path) { fPathInfo.fDocPath = path; }
   void          SetDotDir(const char* dir) { fPathInfo.fDotDir = dir; fPathInfo.fFoundDot = TPathInfo::kDotUnknown; }
   void          SetRootURL(const char* url) { fLinkInfo.fROOTURL = url; }
   void          SetLibURL(const char* lib, const char* url) { fLinkInfo.fLibURLs[lib] = url; }
   void          SetXwho(const char *xwho) { fLinkInfo.fXwho = xwho; }
   void          SetMacroPath(const char* path) {fPathInfo.fMacroPath = path;}
   void          AddMacroPath(const char* path);
   void          SetCounterFormat(const char* format) { fCounterFormat = format; }
   void          SetClassDocTag(const char* tag) { fDocSyntax.fClassDocTag = tag; }
   void          SetAuthorTag(const char* tag) { fDocSyntax.fAuthorTag = tag; }
   void          SetLastUpdateTag(const char* tag) { fDocSyntax.fLastUpdateTag = tag; }
   void          SetCopyrightTag(const char* tag) { fDocSyntax.fCopyrightTag = tag; }
   void          SetHeader(const char* file) { fOutputStyle.fHeader = file; }
   void          SetFooter(const char* file) { fOutputStyle.fFooter = file; }
   void          SetHomepage(const char* url) { fLinkInfo.fHomepage = url; }
   void          SetSearchStemURL(const char* url) { fLinkInfo.fSearchStemURL = url; }
   void          SetSearchEngine(const char* url) { fLinkInfo.fSearchEngine = url; }
   void          SetViewCVS(const char* url) { fLinkInfo.fViewCVS = url; }
   void          SetWikiURL(const char* url) { fLinkInfo.fWikiURL = url; }
   void          SetCharset(const char* charset) { fOutputStyle.fCharset = charset; }
   void          SetDocStyle(const char* style) { fDocSyntax.fDocStyle = style; }

   // Configuration getters
   const TModuleDefinition& GetModuleDefinition() const;
   const TFileDefinition&   GetFileDefinition() const;
   const TPathDefinition&   GetPathDefinition() const;
   const TString&      GetProductName() const { return fProductName; }
   const TString&      GetInputPath() const { return fPathInfo.fInputPath; }
   const TString&      GetOutputDir(Bool_t createDir = kTRUE) const;
   virtual const char* GetEtcDir();
   const TString&      GetModuleDocPath() const { return fPathInfo.fDocPath; }
   const TString&      GetDotDir() const { return fPathInfo.fDotDir; }
   const char*         GetURL(const char* lib = 0) const;
   const TString&      GetXwho() const { return fLinkInfo.fXwho; }
   const TString&      GetMacroPath() const { return fPathInfo.fMacroPath; }
   const char*         GetCounterFormat() const { return fCounterFormat; }
   const TString&      GetClassDocTag() const { return fDocSyntax.fClassDocTag; }
   const TString&      GetAuthorTag() const { return fDocSyntax.fAuthorTag; }
   const TString&      GetLastUpdateTag() const { return fDocSyntax.fLastUpdateTag; }
   const TString&      GetCopyrightTag() const { return fDocSyntax.fCopyrightTag; }
   const TString&      GetHeader() const { return fOutputStyle.fHeader; }
   const TString&      GetFooter() const { return fOutputStyle.fFooter; }
   const TString&      GetHomepage() const { return fLinkInfo.fHomepage; }
   const TString&      GetSearchStemURL() const { return fLinkInfo.fSearchStemURL; }
   const TString&      GetSearchEngine() const { return fLinkInfo.fSearchEngine; }
   const TString&      GetViewCVS() const { return fLinkInfo.fViewCVS; }
   const TString&      GetWikiURL() const { return fLinkInfo.fWikiURL; }
   const TString&      GetCharset() const { return fOutputStyle.fCharset; }
   const TString&      GetDocStyle() const { return fDocSyntax.fDocStyle; }

   // Functions that should only be used by TDocOutput etc.
   Bool_t              CopyFileFromEtcDir(const char* filename) const;
   virtual void        CreateAuxiliaryFiles() const;
   virtual TClass*     GetClass(const char *name) const;
   const char*         GetCounter() const { return fCounter; }
   virtual const char* GetDeclFileName(TClass* cl, Bool_t filesys) const;
   void                GetDerivedClasses(TClass* cl, std::map<TClass*, Int_t>& derived) const;
   static const char*  GetDirDelimiter() {
      // ";" on windows, ":" everywhere else
#ifdef R__WIN32
      return ";";
#else
      return ":";
#endif
   }
   virtual const char* GetImplFileName(TClass* cl, Bool_t filesys) const;
   virtual void        GetHtmlFileName(TClass *classPtr, TString& filename) const;
   virtual const char* GetHtmlFileName(const char* classname) const;
   TList*              GetLibraryDependencies() { return &fDocEntityInfo.fLibDeps; }
   const TList*        GetListOfModules() const { return &fDocEntityInfo.fModules; }
   const TList*        GetListOfClasses() const { return &fDocEntityInfo.fClasses; }
   TFileSysDB*         GetLocalFiles() const { if (!fLocalFiles) SetLocalFiles(); return fLocalFiles; }
   TVirtualMutex*      GetMakeClassMutex() const { return  fMakeClassMutex; }
   virtual void        GetModuleNameForClass(TString& module, TClass* cl) const;
   const TPathInfo&    GetPathInfo() const { return fPathInfo; }
   Bool_t              HaveDot();
   static Bool_t       IsNamespace(const TClass*cl);
   void                SetDeclFileName(TClass* cl, const char* filename);
   void                SetFoundDot(Bool_t found = kTRUE);
   void                SetImplFileName(TClass* cl, const char* filename);

   // unused
   void                ReplaceSpecialChars(std::ostream&, const char*) {
      Error("ReplaceSpecialChars",
            "Removed, call TDocOutput::ReplaceSpecialChars() instead!"); }
   void                SetEscape(char /*esc*/ ='\\') {} // for backward comp

protected:
   struct TDocSyntax {
      TString        fClassDocTag;     // tag for class documentation
      TString        fAuthorTag;       // tag for author
      TString        fLastUpdateTag;   // tag for last update
      TString        fCopyrightTag;    // tag for copyright
      TString        fDocStyle;        // doc style (only "Doc++" has special treatment)
   };

   struct TLinkInfo {
      TString        fXwho;            // URL for name lookup
      TString        fROOTURL;         // Root URL for ROOT's reference guide for libs that are not in fLibURLs
      std::map<std::string, TString> fLibURLs; // URL for documentation of external libraries
      TString        fHomepage;        // URL of homepage
      TString        fSearchStemURL;   // URL stem used to build search URL
      TString        fSearchEngine;    // link to search engine
      TString        fViewCVS;         // link to ViewCVS; %f is replaced by the filename (no %f: it's appended)
      TString        fWikiURL;         // URL stem of class's wiki page, %c replaced by mangled class name (no %c: appended)
   };

   struct TOutputStyle {
      TString        fHeader;          // header file name
      TString        fFooter;          // footerer file name
      TString        fCharset;         // Charset for doc pages
   };

   struct TDocEntityInfo {
      TString        fClassFilter;     // filter used for buidling known classes
      THashList      fClasses;         // known classes
      THashList      fModules;         // known modules
      THashList      fLibDeps;         // Library dependencies
   };

protected:
   virtual void    CreateJavascript() const;
   virtual void    CreateStyleSheet() const;
   void            CreateListOfTypes();
   void            CreateListOfClasses(const char* filter);
   virtual const char* GetDeclImplFileName(TClass* cl, bool filesys, bool decl) const;
   void            MakeClass(void* cdi, Bool_t force=kFALSE);
   TClassDocInfo  *GetNextClass();
   void            SetLocalFiles() const;

   void            HelperDeleted(THelperBase* who);

   static void    *MakeClassThreaded(void* info);

protected:   
   TString        fCounter;         // counter string
   TString        fCounterFormat;   // counter printf-like format
   TString        fProductName;     // name of the product to document
   TIter         *fThreadedClassIter; // fClasses iterator for MakeClassThreaded
   Int_t          fThreadedClassCount; // counter of processed classes for MakeClassThreaded
   TVirtualMutex *fMakeClassMutex; // Mutex for MakeClassThreaded
   TDocSyntax     fDocSyntax;      // doc syntax configuration
   TLinkInfo      fLinkInfo;       // link (URL) configuration
   TOutputStyle   fOutputStyle;    // output style configuration
   TPathInfo      fPathInfo;       // path configuration
   TDocEntityInfo fDocEntityInfo;  // data for documented entities
   mutable TPathDefinition *fPathDef; // object translating classes to module names
   mutable TModuleDefinition *fModuleDef; // object translating classes to module names
   mutable TFileDefinition* fFileDef; // object translating classes to file names
   mutable TFileSysDB    *fLocalFiles; // files found locally for a given source path

   ClassDef(THtml,0)  //Convert class(es) into HTML file(s)
};

R__EXTERN THtml *gHtml;

#endif
