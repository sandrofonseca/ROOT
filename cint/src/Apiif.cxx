/********************************************************
* Apiif.cxx
********************************************************/
#include "Apiif.h"

#ifdef G__MEMTEST
#undef malloc
#undef free
#endif

extern "C" void G__cpp_reset_tagtableG__API();

extern "C" void G__set_cpp_environmentG__API() {
  G__add_ipath("..");
  G__add_compiledheader("Api.h");
  G__cpp_reset_tagtableG__API();
}
class G__ApiifdOcxx_tag {};

void* operator new(size_t size,G__ApiifdOcxx_tag* p) {
  if(p && G__PVOID!=G__getgvp()) return((void*)p);
#ifndef G__ROOT
  return(malloc(size));
#else
  return(::operator new(size));
#endif
}

/* dummy, for exception */
#ifdef G__EH_DUMMY_DELETE
void operator delete(void *p,G__ApiifdOcxx_tag* x) {
  if((long)p==G__getgvp() && G__PVOID!=G__getgvp()) return;
#ifndef G__ROOT
  free(p);
#else
  ::operator delete(p);
#endif
}
#endif

static void G__operator_delete(void *p) {
  if((long)p==G__getgvp() && G__PVOID!=G__getgvp()) return;
#ifndef G__ROOT
  free(p);
#else
  ::operator delete(p);
#endif
}

void G__DELDMY_ApiifdOcxx() { G__operator_delete(0); }

#include "dllrev.h"
extern "C" int G__cpp_dllrevG__API() { return(G__CREATEDLLREV); }

/*********************************************************
* Member function Interface Method
*********************************************************/

/* G__MethodInfo */
static int G__G__API_31_1_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__MethodInfo *p=NULL;
   if(G__getaryconstruct()) p=new G__MethodInfo[G__getaryconstruct()];
   else p=::new((G__ApiifdOcxx_tag*)G__getgvp()) G__MethodInfo;
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__MethodInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_2_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__MethodInfo *p=NULL;
      p=::new((G__ApiifdOcxx_tag*)G__getgvp()) G__MethodInfo(*(G__ClassInfo*)libp->para[0].ref);
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__MethodInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_3_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__MethodInfo*)(G__getstructoffset()))->Init();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_4_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__MethodInfo*)(G__getstructoffset()))->Init(*(G__ClassInfo*)libp->para[0].ref);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_5_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__MethodInfo*)(G__getstructoffset()))->Init((long)G__int(libp->para[0]),(long)G__int(libp->para[1])
,(G__ClassInfo*)G__int(libp->para[2]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_6_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__MethodInfo*)(G__getstructoffset()))->Init((G__ClassInfo*)G__int(libp->para[0]),(long)G__int(libp->para[1])
,(long)G__int(libp->para[2]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_7_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((G__MethodInfo*)(G__getstructoffset()))->Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_8_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((G__MethodInfo*)(G__getstructoffset()))->Title());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_9_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((G__MethodInfo*)(G__getstructoffset()))->Type());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_0_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,108,(long)((G__MethodInfo*)(G__getstructoffset()))->Property());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_1_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__MethodInfo*)(G__getstructoffset()))->NArg());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_2_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__MethodInfo*)(G__getstructoffset()))->NDefaultArg());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_3_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__MethodInfo*)(G__getstructoffset()))->HasVarArgs());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_4_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,89,(long)((G__MethodInfo*)(G__getstructoffset()))->InterfaceMethod());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_5_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((G__MethodInfo*)(G__getstructoffset()))->GetBytecode());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_6_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        G__DataMemberInfo *pobj,xobj=((G__MethodInfo*)(G__getstructoffset()))->GetLocalVariable();
        pobj=new G__DataMemberInfo(xobj);
        result7->obj.i=(long)((void*)pobj); result7->ref=result7->obj.i;
        G__store_tempobject(*result7);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_7_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,89,(long)((G__MethodInfo*)(G__getstructoffset()))->PointerToFunc());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_8_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((G__MethodInfo*)(G__getstructoffset()))->MemberOf());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_9_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((G__MethodInfo*)(G__getstructoffset()))->GetFriendInfo());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_0_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__MethodInfo*)(G__getstructoffset()))->SetGlobalcomp((int)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_1_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__MethodInfo*)(G__getstructoffset()))->IsValid());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_2_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__MethodInfo*)(G__getstructoffset()))->SetFilePos((const char*)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_3_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__MethodInfo*)(G__getstructoffset()))->Next());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_4_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((G__MethodInfo*)(G__getstructoffset()))->FileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_5_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__MethodInfo*)(G__getstructoffset()))->LineNumber());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_6_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__MethodInfo*)(G__getstructoffset()))->Size());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_7_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__MethodInfo*)(G__getstructoffset()))->IsBusy());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_8_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,69,(long)((G__MethodInfo*)(G__getstructoffset()))->FilePointer());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_9_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,108,(long)((G__MethodInfo*)(G__getstructoffset()))->FilePosition());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_0_3(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((G__MethodInfo*)(G__getstructoffset()))->GetPrototype());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_1_3(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((G__MethodInfo*)(G__getstructoffset()))->GetMangledName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_31_2_3(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__MethodInfo*)(G__getstructoffset()))->LoadDLLDirect((const char*)G__int(libp->para[0]),(const char*)G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__G__API_31_3_3(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash)
{
   G__MethodInfo *p;
   void *xtmp = (void*)G__int(libp->para[0]);
   p=new G__MethodInfo(*(G__MethodInfo*)xtmp);
   result7->obj.i = (long)p;
   result7->ref = (long)p;
   result7->type = 'u';
   result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__MethodInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef G__MethodInfo G__TG__MethodInfo;
static int G__G__API_31_4_3(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   if(0==G__getstructoffset()) return(1);
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (G__MethodInfo *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         ((G__MethodInfo *)((G__getstructoffset())+sizeof(G__MethodInfo)*i))->~G__TG__MethodInfo();
   else {
     long G__Xtmp=G__getgvp();
     G__setgvp(G__PVOID);
     ((G__MethodInfo *)(G__getstructoffset()))->~G__TG__MethodInfo();
     G__setgvp(G__Xtmp);
     G__operator_delete((void*)G__getstructoffset());
   }
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* G__DataMemberInfo */
static int G__G__API_32_1_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__DataMemberInfo *p=NULL;
   if(G__getaryconstruct()) p=new G__DataMemberInfo[G__getaryconstruct()];
   else p=::new((G__ApiifdOcxx_tag*)G__getgvp()) G__DataMemberInfo;
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__DataMemberInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_32_2_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__DataMemberInfo *p=NULL;
      p=::new((G__ApiifdOcxx_tag*)G__getgvp()) G__DataMemberInfo(*(G__ClassInfo*)libp->para[0].ref);
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__DataMemberInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_32_3_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__DataMemberInfo*)(G__getstructoffset()))->Init();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_32_4_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__DataMemberInfo*)(G__getstructoffset()))->Init(*(G__ClassInfo*)libp->para[0].ref);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_32_5_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__DataMemberInfo*)(G__getstructoffset()))->Init((long)G__int(libp->para[0]),(long)G__int(libp->para[1])
,(G__ClassInfo*)G__int(libp->para[2]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_32_6_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((G__DataMemberInfo*)(G__getstructoffset()))->Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_32_7_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((G__DataMemberInfo*)(G__getstructoffset()))->Title());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_32_8_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((G__DataMemberInfo*)(G__getstructoffset()))->Type());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_32_9_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,108,(long)((G__DataMemberInfo*)(G__getstructoffset()))->Property());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_32_0_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,108,(long)((G__DataMemberInfo*)(G__getstructoffset()))->Offset());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_32_1_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__DataMemberInfo*)(G__getstructoffset()))->Bitfield());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_32_2_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__DataMemberInfo*)(G__getstructoffset()))->ArrayDim());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_32_3_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__DataMemberInfo*)(G__getstructoffset()))->MaxIndex((int)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_32_4_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((G__DataMemberInfo*)(G__getstructoffset()))->MemberOf());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_32_5_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__DataMemberInfo*)(G__getstructoffset()))->SetGlobalcomp((int)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_32_6_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__DataMemberInfo*)(G__getstructoffset()))->IsValid());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_32_7_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__DataMemberInfo*)(G__getstructoffset()))->SetFilePos((const char*)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_32_8_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__DataMemberInfo*)(G__getstructoffset()))->Next());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_32_9_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   switch(libp->paran) {
   case 2:
   G__letint(result7,67,(long)((G__DataMemberInfo*)(G__getstructoffset()))->ValidArrayIndex((int*)G__int(libp->para[0]),(char**)G__int(libp->para[1])));
      break;
   case 1:
   G__letint(result7,67,(long)((G__DataMemberInfo*)(G__getstructoffset()))->ValidArrayIndex((int*)G__int(libp->para[0])));
      break;
   case 0:
   G__letint(result7,67,(long)((G__DataMemberInfo*)(G__getstructoffset()))->ValidArrayIndex());
      break;
   }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_32_0_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((G__DataMemberInfo*)(G__getstructoffset()))->FileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_32_1_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__DataMemberInfo*)(G__getstructoffset()))->LineNumber());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__G__API_32_2_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash)
{
   G__DataMemberInfo *p;
   void *xtmp = (void*)G__int(libp->para[0]);
   p=new G__DataMemberInfo(*(G__DataMemberInfo*)xtmp);
   result7->obj.i = (long)p;
   result7->ref = (long)p;
   result7->type = 'u';
   result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__DataMemberInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef G__DataMemberInfo G__TG__DataMemberInfo;
static int G__G__API_32_3_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   if(0==G__getstructoffset()) return(1);
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (G__DataMemberInfo *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         ((G__DataMemberInfo *)((G__getstructoffset())+sizeof(G__DataMemberInfo)*i))->~G__TG__DataMemberInfo();
   else {
     long G__Xtmp=G__getgvp();
     G__setgvp(G__PVOID);
     ((G__DataMemberInfo *)(G__getstructoffset()))->~G__TG__DataMemberInfo();
     G__setgvp(G__Xtmp);
     G__operator_delete((void*)G__getstructoffset());
   }
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* G__FriendInfo */
static int G__G__API_33_0_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__FriendInfo *p=NULL;
   switch(libp->paran) {
   case 1:
      p = ::new((G__ApiifdOcxx_tag*)G__getgvp()) G__FriendInfo((G__friendtag*)G__int(libp->para[0]));
      break;
   case 0:
   if(G__getaryconstruct()) p=new G__FriendInfo[G__getaryconstruct()];
   else p=::new((G__ApiifdOcxx_tag*)G__getgvp()) G__FriendInfo;
      break;
   }
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__FriendInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_33_1_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__FriendInfo*)(G__getstructoffset()))->operator=(*(G__FriendInfo*)libp->para[0].ref);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_33_2_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__FriendInfo*)(G__getstructoffset()))->Init((G__friendtag*)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_33_3_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((G__FriendInfo*)(G__getstructoffset()))->FriendOf());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_33_4_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__FriendInfo*)(G__getstructoffset()))->Next());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_33_5_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__FriendInfo*)(G__getstructoffset()))->IsValid());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__G__API_33_6_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash)
{
   G__FriendInfo *p;
   void *xtmp = (void*)G__int(libp->para[0]);
   p=new G__FriendInfo(*(G__FriendInfo*)xtmp);
   result7->obj.i = (long)p;
   result7->ref = (long)p;
   result7->type = 'u';
   result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__FriendInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef G__FriendInfo G__TG__FriendInfo;
static int G__G__API_33_7_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   if(0==G__getstructoffset()) return(1);
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (G__FriendInfo *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         ((G__FriendInfo *)((G__getstructoffset())+sizeof(G__FriendInfo)*i))->~G__TG__FriendInfo();
   else {
     long G__Xtmp=G__getgvp();
     G__setgvp(G__PVOID);
     ((G__FriendInfo *)(G__getstructoffset()))->~G__TG__FriendInfo();
     G__setgvp(G__Xtmp);
     G__operator_delete((void*)G__getstructoffset());
   }
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* G__ClassInfo */
static int G__G__API_34_1_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__ClassInfo *p=NULL;
   if(G__getaryconstruct()) p=new G__ClassInfo[G__getaryconstruct()];
   else p=::new((G__ApiifdOcxx_tag*)G__getgvp()) G__ClassInfo;
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__ClassInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_2_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__ClassInfo*)(G__getstructoffset()))->Init();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_3_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__ClassInfo *p=NULL;
      p=::new((G__ApiifdOcxx_tag*)G__getgvp()) G__ClassInfo((const char*)G__int(libp->para[0]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__ClassInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_4_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__ClassInfo*)(G__getstructoffset()))->Init((const char*)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_5_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__ClassInfo *p=NULL;
      p=::new((G__ApiifdOcxx_tag*)G__getgvp()) G__ClassInfo((int)G__int(libp->para[0]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__ClassInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_6_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__ClassInfo*)(G__getstructoffset()))->Init((int)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_7_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__ClassInfo*)(G__getstructoffset()))->operator==(*(G__ClassInfo*)libp->para[0].ref));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_8_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__ClassInfo*)(G__getstructoffset()))->operator!=(*(G__ClassInfo*)libp->para[0].ref));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_9_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((G__ClassInfo*)(G__getstructoffset()))->Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_0_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((G__ClassInfo*)(G__getstructoffset()))->Fullname());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_1_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((G__ClassInfo*)(G__getstructoffset()))->Title());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_2_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__ClassInfo*)(G__getstructoffset()))->Size());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_3_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,108,(long)((G__ClassInfo*)(G__getstructoffset()))->Property());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_4_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__ClassInfo*)(G__getstructoffset()))->NDataMembers());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_5_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__ClassInfo*)(G__getstructoffset()))->NMethods());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_6_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,108,(long)((G__ClassInfo*)(G__getstructoffset()))->IsBase((const char*)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_7_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,108,(long)((G__ClassInfo*)(G__getstructoffset()))->IsBase(*(G__ClassInfo*)libp->para[0].ref));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_8_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,108,(long)((G__ClassInfo*)(G__getstructoffset()))->Tagnum());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_9_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        G__ClassInfo *pobj,xobj=((G__ClassInfo*)(G__getstructoffset()))->EnclosingClass();
        pobj=new G__ClassInfo(xobj);
        result7->obj.i=(long)((void*)pobj); result7->ref=result7->obj.i;
        G__store_tempobject(*result7);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_0_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        G__ClassInfo *pobj,xobj=((G__ClassInfo*)(G__getstructoffset()))->EnclosingSpace();
        pobj=new G__ClassInfo(xobj);
        result7->obj.i=(long)((void*)pobj); result7->ref=result7->obj.i;
        G__store_tempobject(*result7);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_1_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((G__ClassInfo*)(G__getstructoffset()))->GetFriendInfo());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_2_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__ClassInfo*)(G__getstructoffset()))->SetGlobalcomp((int)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_3_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__ClassInfo*)(G__getstructoffset()))->SetProtectedAccess((int)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_4_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__ClassInfo*)(G__getstructoffset()))->IsValid());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_5_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__ClassInfo*)(G__getstructoffset()))->IsLoaded());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_6_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__ClassInfo*)(G__getstructoffset()))->SetFilePos((const char*)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_7_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__ClassInfo*)(G__getstructoffset()))->Next());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_8_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__ClassInfo*)(G__getstructoffset()))->Linkage());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_9_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((G__ClassInfo*)(G__getstructoffset()))->FileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_0_3(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__ClassInfo*)(G__getstructoffset()))->LineNumber());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_1_3(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__ClassInfo*)(G__getstructoffset()))->IsTmplt());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_2_3(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((G__ClassInfo*)(G__getstructoffset()))->TmpltName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_3_3(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((G__ClassInfo*)(G__getstructoffset()))->TmpltArg());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_4_3(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__ClassInfo*)(G__getstructoffset()))->SetDefFile((char*)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_5_3(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__ClassInfo*)(G__getstructoffset()))->SetDefLine((int)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_6_3(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__ClassInfo*)(G__getstructoffset()))->SetImpFile((char*)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_7_3(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__ClassInfo*)(G__getstructoffset()))->SetImpLine((int)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_8_3(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__ClassInfo*)(G__getstructoffset()))->SetVersion((int)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_9_3(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((G__ClassInfo*)(G__getstructoffset()))->DefFile());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_0_4(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__ClassInfo*)(G__getstructoffset()))->DefLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_1_4(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((G__ClassInfo*)(G__getstructoffset()))->ImpFile());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_2_4(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__ClassInfo*)(G__getstructoffset()))->ImpLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_3_4(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__ClassInfo*)(G__getstructoffset()))->Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_4_4(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,89,(long)((G__ClassInfo*)(G__getstructoffset()))->New());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_5_4(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,89,(long)((G__ClassInfo*)(G__getstructoffset()))->New((int)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_6_4(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,89,(long)((G__ClassInfo*)(G__getstructoffset()))->New((void*)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_7_4(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__ClassInfo*)(G__getstructoffset()))->InstanceCount());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_8_4(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__ClassInfo*)(G__getstructoffset()))->ResetInstanceCount();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_9_4(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__ClassInfo*)(G__getstructoffset()))->IncInstanceCount();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_0_5(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__ClassInfo*)(G__getstructoffset()))->HeapInstanceCount());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_1_5(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__ClassInfo*)(G__getstructoffset()))->IncHeapInstanceCount();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_2_5(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__ClassInfo*)(G__getstructoffset()))->ResetHeapInstanceCount();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_3_5(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__ClassInfo*)(G__getstructoffset()))->RootFlag());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_4_5(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   switch(libp->paran) {
   case 4:
   G__letint(result7,89,(long)((G__ClassInfo*)(G__getstructoffset()))->GetInterfaceMethod((const char*)G__int(libp->para[0]),(const char*)G__int(libp->para[1])
,(long*)G__int(libp->para[2]),(G__ClassInfo::MatchMode)G__int(libp->para[3])));
      break;
   case 3:
   G__letint(result7,89,(long)((G__ClassInfo*)(G__getstructoffset()))->GetInterfaceMethod((const char*)G__int(libp->para[0]),(const char*)G__int(libp->para[1])
,(long*)G__int(libp->para[2])));
      break;
   }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_5_5(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   switch(libp->paran) {
   case 4:
      {
        G__MethodInfo *pobj,xobj=((G__ClassInfo*)(G__getstructoffset()))->GetMethod((const char*)G__int(libp->para[0]),(const char*)G__int(libp->para[1])
,(long*)G__int(libp->para[2]),(G__ClassInfo::MatchMode)G__int(libp->para[3]));
        pobj=new G__MethodInfo(xobj);
        result7->obj.i=(long)((void*)pobj); result7->ref=result7->obj.i;
        G__store_tempobject(*result7);
      }
      break;
   case 3:
      {
        G__MethodInfo *pobj,xobj=((G__ClassInfo*)(G__getstructoffset()))->GetMethod((const char*)G__int(libp->para[0]),(const char*)G__int(libp->para[1])
,(long*)G__int(libp->para[2]));
        pobj=new G__MethodInfo(xobj);
        result7->obj.i=(long)((void*)pobj); result7->ref=result7->obj.i;
        G__store_tempobject(*result7);
      }
      break;
   }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_6_5(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        G__DataMemberInfo *pobj,xobj=((G__ClassInfo*)(G__getstructoffset()))->GetDataMember((const char*)G__int(libp->para[0]),(long*)G__int(libp->para[1]));
        pobj=new G__DataMemberInfo(xobj);
        result7->obj.i=(long)((void*)pobj); result7->ref=result7->obj.i;
        G__store_tempobject(*result7);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_7_5(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__ClassInfo*)(G__getstructoffset()))->HasMethod((const char*)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_8_5(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__ClassInfo*)(G__getstructoffset()))->HasDataMember((const char*)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_9_5(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__ClassInfo*)(G__getstructoffset()))->HasDefaultConstructor());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_34_1_6(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,108,(long)((G__ClassInfo*)(G__getstructoffset()))->ClassProperty());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__G__API_34_2_6(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash)
{
   G__ClassInfo *p;
   void *xtmp = (void*)G__int(libp->para[0]);
   p=new G__ClassInfo(*(G__ClassInfo*)xtmp);
   result7->obj.i = (long)p;
   result7->ref = (long)p;
   result7->type = 'u';
   result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__ClassInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef G__ClassInfo G__TG__ClassInfo;
static int G__G__API_34_3_6(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   if(0==G__getstructoffset()) return(1);
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (G__ClassInfo *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         ((G__ClassInfo *)((G__getstructoffset())+sizeof(G__ClassInfo)*i))->~G__TG__ClassInfo();
   else {
     long G__Xtmp=G__getgvp();
     G__setgvp(G__PVOID);
     ((G__ClassInfo *)(G__getstructoffset()))->~G__TG__ClassInfo();
     G__setgvp(G__Xtmp);
     G__operator_delete((void*)G__getstructoffset());
   }
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* G__BaseClassInfo */
static int G__G__API_36_1_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__BaseClassInfo *p=NULL;
      p=::new((G__ApiifdOcxx_tag*)G__getgvp()) G__BaseClassInfo(*(G__ClassInfo*)libp->para[0].ref);
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__BaseClassInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_36_2_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__BaseClassInfo*)(G__getstructoffset()))->Init(*(G__ClassInfo*)libp->para[0].ref);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_36_3_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,108,(long)((G__BaseClassInfo*)(G__getstructoffset()))->Offset());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_36_4_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,108,(long)((G__BaseClassInfo*)(G__getstructoffset()))->Property());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_36_5_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__BaseClassInfo*)(G__getstructoffset()))->IsValid());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_36_6_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__BaseClassInfo*)(G__getstructoffset()))->Next());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__G__API_36_7_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash)
{
   G__BaseClassInfo *p;
   void *xtmp = (void*)G__int(libp->para[0]);
   p=new G__BaseClassInfo(*(G__BaseClassInfo*)xtmp);
   result7->obj.i = (long)p;
   result7->ref = (long)p;
   result7->type = 'u';
   result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__BaseClassInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef G__BaseClassInfo G__TG__BaseClassInfo;
static int G__G__API_36_8_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   if(0==G__getstructoffset()) return(1);
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (G__BaseClassInfo *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         ((G__BaseClassInfo *)((G__getstructoffset())+sizeof(G__BaseClassInfo)*i))->~G__TG__BaseClassInfo();
   else {
     long G__Xtmp=G__getgvp();
     G__setgvp(G__PVOID);
     ((G__BaseClassInfo *)(G__getstructoffset()))->~G__TG__BaseClassInfo();
     G__setgvp(G__Xtmp);
     G__operator_delete((void*)G__getstructoffset());
   }
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* G__TypeInfo */
static int G__G__API_37_1_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__TypeInfo *p=NULL;
      p=::new((G__ApiifdOcxx_tag*)G__getgvp()) G__TypeInfo((const char*)G__int(libp->para[0]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__TypeInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_37_2_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__TypeInfo *p=NULL;
   if(G__getaryconstruct()) p=new G__TypeInfo[G__getaryconstruct()];
   else p=::new((G__ApiifdOcxx_tag*)G__getgvp()) G__TypeInfo;
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__TypeInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_37_3_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__TypeInfo*)(G__getstructoffset()))->Init((const char*)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_37_4_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__TypeInfo*)(G__getstructoffset()))->operator==(*(G__TypeInfo*)libp->para[0].ref));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_37_5_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__TypeInfo*)(G__getstructoffset()))->operator!=(*(G__TypeInfo*)libp->para[0].ref));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_37_6_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((G__TypeInfo*)(G__getstructoffset()))->Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_37_7_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((G__TypeInfo*)(G__getstructoffset()))->TrueName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_37_8_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__TypeInfo*)(G__getstructoffset()))->Size());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_37_9_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,108,(long)((G__TypeInfo*)(G__getstructoffset()))->Property());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_37_0_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__TypeInfo*)(G__getstructoffset()))->IsValid());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_37_1_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,89,(long)((G__TypeInfo*)(G__getstructoffset()))->New());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_37_2_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__TypeInfo*)(G__getstructoffset()))->Typenum());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_37_3_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__TypeInfo*)(G__getstructoffset()))->Type());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_37_4_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__TypeInfo*)(G__getstructoffset()))->Reftype());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_37_5_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__TypeInfo*)(G__getstructoffset()))->Isconst());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__G__API_37_7_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash)
{
   G__TypeInfo *p;
   void *xtmp = (void*)G__int(libp->para[0]);
   p=new G__TypeInfo(*(G__TypeInfo*)xtmp);
   result7->obj.i = (long)p;
   result7->ref = (long)p;
   result7->type = 'u';
   result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__TypeInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef G__TypeInfo G__TG__TypeInfo;
static int G__G__API_37_8_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   if(0==G__getstructoffset()) return(1);
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (G__TypeInfo *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         ((G__TypeInfo *)((G__getstructoffset())+sizeof(G__TypeInfo)*i))->~G__TG__TypeInfo();
   else {
     long G__Xtmp=G__getgvp();
     G__setgvp(G__PVOID);
     ((G__TypeInfo *)(G__getstructoffset()))->~G__TG__TypeInfo();
     G__setgvp(G__Xtmp);
     G__operator_delete((void*)G__getstructoffset());
   }
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* G__MethodArgInfo */
static int G__G__API_38_1_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__MethodArgInfo*)(G__getstructoffset()))->Init(*(G__MethodInfo*)libp->para[0].ref);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_38_2_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__MethodArgInfo *p=NULL;
      p=::new((G__ApiifdOcxx_tag*)G__getgvp()) G__MethodArgInfo(*(G__MethodInfo*)libp->para[0].ref);
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__MethodArgInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_38_3_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((G__MethodArgInfo*)(G__getstructoffset()))->Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_38_4_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((G__MethodArgInfo*)(G__getstructoffset()))->Type());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_38_5_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,108,(long)((G__MethodArgInfo*)(G__getstructoffset()))->Property());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_38_6_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((G__MethodArgInfo*)(G__getstructoffset()))->DefaultValue());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_38_7_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,85,(long)((G__MethodArgInfo*)(G__getstructoffset()))->ArgOf());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_38_8_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__MethodArgInfo*)(G__getstructoffset()))->IsValid());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_38_9_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__MethodArgInfo*)(G__getstructoffset()))->Next());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_38_0_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__MethodArgInfo *p=NULL;
   if(G__getaryconstruct()) p=new G__MethodArgInfo[G__getaryconstruct()];
   else p=::new((G__ApiifdOcxx_tag*)G__getgvp()) G__MethodArgInfo;
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__MethodArgInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__G__API_38_1_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash)
{
   G__MethodArgInfo *p;
   void *xtmp = (void*)G__int(libp->para[0]);
   p=new G__MethodArgInfo(*(G__MethodArgInfo*)xtmp);
   result7->obj.i = (long)p;
   result7->ref = (long)p;
   result7->type = 'u';
   result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__MethodArgInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef G__MethodArgInfo G__TG__MethodArgInfo;
static int G__G__API_38_2_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   if(0==G__getstructoffset()) return(1);
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (G__MethodArgInfo *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         ((G__MethodArgInfo *)((G__getstructoffset())+sizeof(G__MethodArgInfo)*i))->~G__TG__MethodArgInfo();
   else {
     long G__Xtmp=G__getgvp();
     G__setgvp(G__PVOID);
     ((G__MethodArgInfo *)(G__getstructoffset()))->~G__TG__MethodArgInfo();
     G__setgvp(G__Xtmp);
     G__operator_delete((void*)G__getstructoffset());
   }
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* G__CallFunc */
static int G__G__API_40_1_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__CallFunc *p=NULL;
   if(G__getaryconstruct()) p=new G__CallFunc[G__getaryconstruct()];
   else p=::new((G__ApiifdOcxx_tag*)G__getgvp()) G__CallFunc;
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__CallFunc);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_40_2_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__CallFunc*)(G__getstructoffset()))->Init();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_40_3_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   switch(libp->paran) {
   case 5:
      G__setnull(result7);
      ((G__CallFunc*)(G__getstructoffset()))->SetFunc((G__ClassInfo*)G__int(libp->para[0]),(const char*)G__int(libp->para[1])
,(const char*)G__int(libp->para[2]),(long*)G__int(libp->para[3])
,(G__CallFunc::MatchMode)G__int(libp->para[4]));
      break;
   case 4:
      G__setnull(result7);
      ((G__CallFunc*)(G__getstructoffset()))->SetFunc((G__ClassInfo*)G__int(libp->para[0]),(const char*)G__int(libp->para[1])
,(const char*)G__int(libp->para[2]),(long*)G__int(libp->para[3]));
      break;
   }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_40_4_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__CallFunc*)(G__getstructoffset()))->SetFuncProto((G__ClassInfo*)G__int(libp->para[0]),(const char*)G__int(libp->para[1])
,(const char*)G__int(libp->para[2]),(long*)G__int(libp->para[3]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_40_5_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__CallFunc*)(G__getstructoffset()))->SetFunc((G__InterfaceMethod)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_40_6_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__CallFunc*)(G__getstructoffset()))->SetFunc(*((G__MethodInfo*)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_40_7_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__CallFunc*)(G__getstructoffset()))->SetBytecode((G__bytecodefunc*)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_40_8_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__CallFunc*)(G__getstructoffset()))->IsValid());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_40_9_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   switch(libp->paran) {
   case 2:
      G__setnull(result7);
      ((G__CallFunc*)(G__getstructoffset()))->SetArgArray((long*)G__int(libp->para[0]),(int)G__int(libp->para[1]));
      break;
   case 1:
      G__setnull(result7);
      ((G__CallFunc*)(G__getstructoffset()))->SetArgArray((long*)G__int(libp->para[0]));
      break;
   }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_40_0_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__CallFunc*)(G__getstructoffset()))->ResetArg();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_40_1_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__CallFunc*)(G__getstructoffset()))->SetArg((long)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_40_2_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__CallFunc*)(G__getstructoffset()))->SetArg((double)G__double(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_40_3_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__CallFunc*)(G__getstructoffset()))->Exec((void*)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_40_4_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,108,(long)((G__CallFunc*)(G__getstructoffset()))->ExecInt((void*)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_40_5_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letdouble(result7,100,(double)((G__CallFunc*)(G__getstructoffset()))->ExecDouble((void*)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_40_6_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,89,(long)((G__CallFunc*)(G__getstructoffset()))->InterfaceMethod());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_40_7_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__CallFunc*)(G__getstructoffset()))->SetArgs((const char*)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_40_8_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        G__MethodInfo *pobj,xobj=((G__CallFunc*)(G__getstructoffset()))->GetMethodInfo();
        pobj=new G__MethodInfo(xobj);
        result7->obj.i=(long)((void*)pobj); result7->ref=result7->obj.i;
        G__store_tempobject(*result7);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__G__API_40_0_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash)
{
   G__CallFunc *p;
   void *xtmp = (void*)G__int(libp->para[0]);
   p=new G__CallFunc(*(G__CallFunc*)xtmp);
   result7->obj.i = (long)p;
   result7->ref = (long)p;
   result7->type = 'u';
   result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__CallFunc);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef G__CallFunc G__TG__CallFunc;
static int G__G__API_40_1_2(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   if(0==G__getstructoffset()) return(1);
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (G__CallFunc *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         ((G__CallFunc *)((G__getstructoffset())+sizeof(G__CallFunc)*i))->~G__TG__CallFunc();
   else {
     long G__Xtmp=G__getgvp();
     G__setgvp(G__PVOID);
     ((G__CallFunc *)(G__getstructoffset()))->~G__TG__CallFunc();
     G__setgvp(G__Xtmp);
     G__operator_delete((void*)G__getstructoffset());
   }
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* G__TypedefInfo */
static int G__G__API_42_1_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__TypedefInfo *p=NULL;
   if(G__getaryconstruct()) p=new G__TypedefInfo[G__getaryconstruct()];
   else p=::new((G__ApiifdOcxx_tag*)G__getgvp()) G__TypedefInfo;
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__TypedefInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_42_2_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__TypedefInfo*)(G__getstructoffset()))->Init();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_42_3_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__TypedefInfo *p=NULL;
      p=::new((G__ApiifdOcxx_tag*)G__getgvp()) G__TypedefInfo((const char*)G__int(libp->para[0]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__TypedefInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_42_4_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__TypedefInfo*)(G__getstructoffset()))->Init((const char*)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_42_5_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__TypedefInfo *p=NULL;
      p=::new((G__ApiifdOcxx_tag*)G__getgvp()) G__TypedefInfo((int)G__int(libp->para[0]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__TypedefInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_42_6_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__TypedefInfo*)(G__getstructoffset()))->Init((int)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_42_7_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        G__ClassInfo *pobj,xobj=((G__TypedefInfo*)(G__getstructoffset()))->EnclosingClassOfTypedef();
        pobj=new G__ClassInfo(xobj);
        result7->obj.i=(long)((void*)pobj); result7->ref=result7->obj.i;
        G__store_tempobject(*result7);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_42_8_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((G__TypedefInfo*)(G__getstructoffset()))->Title());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_42_9_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__TypedefInfo*)(G__getstructoffset()))->SetGlobalcomp((int)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_42_0_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__TypedefInfo*)(G__getstructoffset()))->IsValid());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_42_1_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__TypedefInfo*)(G__getstructoffset()))->SetFilePos((const char*)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_42_2_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__TypedefInfo*)(G__getstructoffset()))->Next());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_42_3_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((G__TypedefInfo*)(G__getstructoffset()))->FileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_42_4_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__TypedefInfo*)(G__getstructoffset()))->LineNumber());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__G__API_42_5_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash)
{
   G__TypedefInfo *p;
   void *xtmp = (void*)G__int(libp->para[0]);
   p=new G__TypedefInfo(*(G__TypedefInfo*)xtmp);
   result7->obj.i = (long)p;
   result7->ref = (long)p;
   result7->type = 'u';
   result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__TypedefInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef G__TypedefInfo G__TG__TypedefInfo;
static int G__G__API_42_6_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   if(0==G__getstructoffset()) return(1);
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (G__TypedefInfo *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         ((G__TypedefInfo *)((G__getstructoffset())+sizeof(G__TypedefInfo)*i))->~G__TG__TypedefInfo();
   else {
     long G__Xtmp=G__getgvp();
     G__setgvp(G__PVOID);
     ((G__TypedefInfo *)(G__getstructoffset()))->~G__TG__TypedefInfo();
     G__setgvp(G__Xtmp);
     G__operator_delete((void*)G__getstructoffset());
   }
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* G__TokenInfo */
static int G__G__API_43_1_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__TokenInfo *p=NULL;
   if(G__getaryconstruct()) p=new G__TokenInfo[G__getaryconstruct()];
   else p=::new((G__ApiifdOcxx_tag*)G__getgvp()) G__TokenInfo;
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__TokenInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_43_2_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__TokenInfo*)(G__getstructoffset()))->Init();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_43_3_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        G__MethodInfo *pobj,xobj=((G__TokenInfo*)(G__getstructoffset()))->MakeLocalTable(*(G__ClassInfo*)libp->para[0].ref,(const char*)G__int(libp->para[1])
,(const char*)G__int(libp->para[2]));
        pobj=new G__MethodInfo(xobj);
        result7->obj.i=(long)((void*)pobj); result7->ref=result7->obj.i;
        G__store_tempobject(*result7);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_43_4_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__TokenInfo*)(G__getstructoffset()))->Query(*(G__ClassInfo*)libp->para[0].ref,*(G__MethodInfo*)libp->para[1].ref
,(const char*)G__int(libp->para[2]),(const char*)G__int(libp->para[3])
,(const char*)G__int(libp->para[4])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_43_5_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__TokenInfo*)(G__getstructoffset()))->GetTokenType());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_43_6_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__TokenInfo*)(G__getstructoffset()))->GetTokenProperty());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_43_7_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        G__ClassInfo *pobj,xobj=((G__TokenInfo*)(G__getstructoffset()))->GetNextScope();
        pobj=new G__ClassInfo(xobj);
        result7->obj.i=(long)((void*)pobj); result7->ref=result7->obj.i;
        G__store_tempobject(*result7);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__G__API_43_5_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash)
{
   G__TokenInfo *p;
   void *xtmp = (void*)G__int(libp->para[0]);
   p=new G__TokenInfo(*(G__TokenInfo*)xtmp);
   result7->obj.i = (long)p;
   result7->ref = (long)p;
   result7->type = 'u';
   result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__TokenInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef G__TokenInfo G__TG__TokenInfo;
static int G__G__API_43_6_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   if(0==G__getstructoffset()) return(1);
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (G__TokenInfo *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         ((G__TokenInfo *)((G__getstructoffset())+sizeof(G__TokenInfo)*i))->~G__TG__TokenInfo();
   else {
     long G__Xtmp=G__getgvp();
     G__setgvp(G__PVOID);
     ((G__TokenInfo *)(G__getstructoffset()))->~G__TG__TokenInfo();
     G__setgvp(G__Xtmp);
     G__operator_delete((void*)G__getstructoffset());
   }
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* G__SourceFileInfo */
static int G__G__API_46_0_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__SourceFileInfo *p=NULL;
   if(G__getaryconstruct()) p=new G__SourceFileInfo[G__getaryconstruct()];
   else p=::new((G__ApiifdOcxx_tag*)G__getgvp()) G__SourceFileInfo;
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__SourceFileInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_46_1_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__SourceFileInfo *p=NULL;
      p=::new((G__ApiifdOcxx_tag*)G__getgvp()) G__SourceFileInfo((int)G__int(libp->para[0]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__SourceFileInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_46_2_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__SourceFileInfo *p=NULL;
      p=::new((G__ApiifdOcxx_tag*)G__getgvp()) G__SourceFileInfo((const char*)G__int(libp->para[0]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__SourceFileInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_46_4_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__SourceFileInfo*)(G__getstructoffset()))->Init();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_46_5_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__SourceFileInfo*)(G__getstructoffset()))->Init((const char*)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_46_6_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((G__SourceFileInfo*)(G__getstructoffset()))->Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_46_7_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((G__SourceFileInfo*)(G__getstructoffset()))->Prepname());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_46_8_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,69,(long)((G__SourceFileInfo*)(G__getstructoffset()))->fp());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_46_9_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__SourceFileInfo*)(G__getstructoffset()))->MaxLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_46_0_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      {
        const G__SourceFileInfo& obj=((G__SourceFileInfo*)(G__getstructoffset()))->IncludedFrom();
         result7->ref=(long)(&obj); result7->obj.i=(long)(&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_46_1_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,108,(long)((G__SourceFileInfo*)(G__getstructoffset()))->Property());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_46_2_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__SourceFileInfo*)(G__getstructoffset()))->IsValid());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_46_3_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__SourceFileInfo*)(G__getstructoffset()))->Next());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__G__API_46_4_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash)
{
   G__SourceFileInfo *p;
   void *xtmp = (void*)G__int(libp->para[0]);
   p=new G__SourceFileInfo(*(G__SourceFileInfo*)xtmp);
   result7->obj.i = (long)p;
   result7->ref = (long)p;
   result7->type = 'u';
   result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__SourceFileInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef G__SourceFileInfo G__TG__SourceFileInfo;
static int G__G__API_46_5_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   if(0==G__getstructoffset()) return(1);
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (G__SourceFileInfo *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         ((G__SourceFileInfo *)((G__getstructoffset())+sizeof(G__SourceFileInfo)*i))->~G__TG__SourceFileInfo();
   else {
     long G__Xtmp=G__getgvp();
     G__setgvp(G__PVOID);
     ((G__SourceFileInfo *)(G__getstructoffset()))->~G__TG__SourceFileInfo();
     G__setgvp(G__Xtmp);
     G__operator_delete((void*)G__getstructoffset());
   }
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* G__IncludePathInfo */
static int G__G__API_47_0_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__IncludePathInfo *p=NULL;
   if(G__getaryconstruct()) p=new G__IncludePathInfo[G__getaryconstruct()];
   else p=::new((G__ApiifdOcxx_tag*)G__getgvp()) G__IncludePathInfo;
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__IncludePathInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_47_2_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((G__IncludePathInfo*)(G__getstructoffset()))->Init();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_47_3_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   G__letint(result7,67,(long)((G__IncludePathInfo*)(G__getstructoffset()))->Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_47_4_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,108,(long)((G__IncludePathInfo*)(G__getstructoffset()))->Property());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_47_5_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__IncludePathInfo*)(G__getstructoffset()))->IsValid());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API_47_6_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((G__IncludePathInfo*)(G__getstructoffset()))->Next());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__G__API_47_7_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash)
{
   G__IncludePathInfo *p;
   void *xtmp = (void*)G__int(libp->para[0]);
   p=new G__IncludePathInfo(*(G__IncludePathInfo*)xtmp);
   result7->obj.i = (long)p;
   result7->ref = (long)p;
   result7->type = 'u';
   result7->tagnum = G__get_linked_tagnum(&G__G__APILN_G__IncludePathInfo);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef G__IncludePathInfo G__TG__IncludePathInfo;
static int G__G__API_47_8_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   if(0==G__getstructoffset()) return(1);
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (G__IncludePathInfo *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         ((G__IncludePathInfo *)((G__getstructoffset())+sizeof(G__IncludePathInfo)*i))->~G__TG__IncludePathInfo();
   else {
     long G__Xtmp=G__getgvp();
     G__setgvp(G__PVOID);
     ((G__IncludePathInfo *)(G__getstructoffset()))->~G__TG__IncludePathInfo();
     G__setgvp(G__Xtmp);
     G__operator_delete((void*)G__getstructoffset());
   }
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* Setting up global function */
static int G__G__API__8_18(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)G__SetGlobalcomp((char*)G__int(libp->para[0]),(char*)G__int(libp->para[1])
,(int)G__int(libp->para[2])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API__9_18(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)G__ForceBytecodecompilation((char*)G__int(libp->para[0]),(char*)G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API__0_19(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      G__InitGetSpecialObject((G__pMethodSpecialObject)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API__1_19(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      G__InitGetSpecialValue((G__pMethodSpecialValue)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__G__API__2_19(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      G__InitUpdateClassInfo((G__pMethodUpdateClassInfo)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}


/*********************************************************
* Member function Stub
*********************************************************/

/* G__MethodInfo */

/* G__DataMemberInfo */

/* G__FriendInfo */

/* G__ClassInfo */

/* G__BaseClassInfo */

/* G__TypeInfo */

/* G__MethodArgInfo */

/* G__CallFunc */

/* G__TypedefInfo */

/* G__TokenInfo */

/* G__SourceFileInfo */

/* G__IncludePathInfo */

/*********************************************************
* Global function Stub
*********************************************************/

/*********************************************************
* Get size of pointer to member function
*********************************************************/
class G__Sizep2memfuncG__API {
 public:
  G__Sizep2memfuncG__API() {p=&G__Sizep2memfuncG__API::sizep2memfunc;}
    size_t sizep2memfunc() { return(sizeof(p)); }
  private:
    size_t (G__Sizep2memfuncG__API::*p)();
};

size_t G__get_sizep2memfuncG__API()
{
  G__Sizep2memfuncG__API a;
  G__setsizep2memfunc((int)a.sizep2memfunc());
  return((size_t)a.sizep2memfunc());
}


/*********************************************************
* virtual base class offset calculation interface
*********************************************************/

   /* Setting up class inheritance */

/*********************************************************
* Inheritance information setup/
*********************************************************/
extern "C" void G__cpp_setup_inheritanceG__API() {

   /* Setting up class inheritance */
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__G__APILN_G__BaseClassInfo))) {
     G__BaseClassInfo *G__Lderived;
     G__Lderived=(G__BaseClassInfo*)0x1000;
     {
       G__ClassInfo *G__Lpbase=(G__ClassInfo*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__G__APILN_G__BaseClassInfo),G__get_linked_tagnum(&G__G__APILN_G__ClassInfo),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__G__APILN_G__TypeInfo))) {
     G__TypeInfo *G__Lderived;
     G__Lderived=(G__TypeInfo*)0x1000;
     {
       G__ClassInfo *G__Lpbase=(G__ClassInfo*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__G__APILN_G__TypeInfo),G__get_linked_tagnum(&G__G__APILN_G__ClassInfo),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__G__APILN_G__TypedefInfo))) {
     G__TypedefInfo *G__Lderived;
     G__Lderived=(G__TypedefInfo*)0x1000;
     {
       G__TypeInfo *G__Lpbase=(G__TypeInfo*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__G__APILN_G__TypedefInfo),G__get_linked_tagnum(&G__G__APILN_G__TypeInfo),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
     {
       G__ClassInfo *G__Lpbase=(G__ClassInfo*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__G__APILN_G__TypedefInfo),G__get_linked_tagnum(&G__G__APILN_G__ClassInfo),(long)G__Lpbase-(long)G__Lderived,1,0);
     }
   }
}

/*********************************************************
* typedef information setup/
*********************************************************/
extern "C" void G__cpp_setup_typetableG__API() {

   /* Setting up typedef entry */
   G__search_typename2("G__InterfaceMethod",89,-1,0,
-1);
   G__setnewtype(-1,"int (*G__InterfaceMethod)(G__value*,G__CONST char*,struct G__param*,int);",0);
   G__search_typename2("G__pMethodSpecialObject",89,-1,0,
-1);
   G__setnewtype(-1,"void *(*G__pMethodSpecialObject)(char *item,G__ClassInfo *typeinfo",0);
   G__search_typename2("G__pMethodSpecialValue",89,-1,0,
-1);
   G__setnewtype(-1,"void (*G__pMethodSpecialValue)(char *item,G__TypeInfo *typeinfo",0);
   G__search_typename2("G__pMethodUpdateClassInfo",89,-1,0,
-1);
   G__setnewtype(-1,"void (*G__pMethodUpdateClassInfo)(char *item,long tagnum);",0);
}

/*********************************************************
* Data Member information setup/
*********************************************************/

   /* Setting up class,struct,union tag member variable */

   /* G__MethodInfo */
static void G__setup_memvarG__MethodInfo(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__G__APILN_G__MethodInfo));
   { G__MethodInfo *p; p=(G__MethodInfo*)0x1000; if (p) { }
   }
   G__tag_memvar_reset();
}


   /* G__DataMemberInfo */
static void G__setup_memvarG__DataMemberInfo(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__G__APILN_G__DataMemberInfo));
   { G__DataMemberInfo *p; p=(G__DataMemberInfo*)0x1000; if (p) { }
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__APILN_G__DataMemberInfocLcLerror_code),-1,-2,1,"VALID=0",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__APILN_G__DataMemberInfocLcLerror_code),-1,-2,1,"NOT_INT=1",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__APILN_G__DataMemberInfocLcLerror_code),-1,-2,1,"NOT_DEF=2",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__APILN_G__DataMemberInfocLcLerror_code),-1,-2,1,"IS_PRIVATE=3",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__APILN_G__DataMemberInfocLcLerror_code),-1,-2,1,"UNKNOWN=4",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* G__FriendInfo */
static void G__setup_memvarG__FriendInfo(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__G__APILN_G__FriendInfo));
   { G__FriendInfo *p; p=(G__FriendInfo*)0x1000; if (p) { }
   }
   G__tag_memvar_reset();
}


   /* G__ClassInfo */
static void G__setup_memvarG__ClassInfo(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__G__APILN_G__ClassInfo));
   { G__ClassInfo *p; p=(G__ClassInfo*)0x1000; if (p) { }
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__APILN_G__ClassInfocLcLMatchMode),-1,-2,1,"ExactMatch=0",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__APILN_G__ClassInfocLcLMatchMode),-1,-2,1,"ConversionMatch=1",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* G__BaseClassInfo */
static void G__setup_memvarG__BaseClassInfo(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__G__APILN_G__BaseClassInfo));
   { G__BaseClassInfo *p; p=(G__BaseClassInfo*)0x1000; if (p) { }
   }
   G__tag_memvar_reset();
}


   /* G__TypeInfo */
static void G__setup_memvarG__TypeInfo(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__G__APILN_G__TypeInfo));
   { G__TypeInfo *p; p=(G__TypeInfo*)0x1000; if (p) { }
   }
   G__tag_memvar_reset();
}


   /* G__MethodArgInfo */
static void G__setup_memvarG__MethodArgInfo(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__G__APILN_G__MethodArgInfo));
   { G__MethodArgInfo *p; p=(G__MethodArgInfo*)0x1000; if (p) { }
   }
   G__tag_memvar_reset();
}


   /* G__CallFunc */
static void G__setup_memvarG__CallFunc(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__G__APILN_G__CallFunc));
   { G__CallFunc *p; p=(G__CallFunc*)0x1000; if (p) { }
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__APILN_G__CallFunccLcLMatchMode),-1,-2,1,"ExactMatch=0",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__APILN_G__CallFunccLcLMatchMode),-1,-2,1,"ConversionMatch=1",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* G__TypedefInfo */
static void G__setup_memvarG__TypedefInfo(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__G__APILN_G__TypedefInfo));
   { G__TypedefInfo *p; p=(G__TypedefInfo*)0x1000; if (p) { }
   }
   G__tag_memvar_reset();
}


   /* G__TokenInfo */
static void G__setup_memvarG__TokenInfo(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__G__APILN_G__TokenInfo));
   { G__TokenInfo *p; p=(G__TokenInfo*)0x1000; if (p) { }
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__APILN_G__TokenInfocLcLG__TokenType),-1,-2,1,"t_invalid=0",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__APILN_G__TokenInfocLcLG__TokenType),-1,-2,1,"t_class=1",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__APILN_G__TokenInfocLcLG__TokenType),-1,-2,1,"t_typedef=2",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__APILN_G__TokenInfocLcLG__TokenType),-1,-2,1,"t_fundamental=3",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__APILN_G__TokenInfocLcLG__TokenType),-1,-2,1,"t_enum=4",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__APILN_G__TokenInfocLcLG__TokenType),-1,-2,1,"t_memberfunc=5",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__APILN_G__TokenInfocLcLG__TokenType),-1,-2,1,"t_globalfunc=6",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__APILN_G__TokenInfocLcLG__TokenType),-1,-2,1,"t_datamember=7",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__APILN_G__TokenInfocLcLG__TokenType),-1,-2,1,"t_local=8",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__APILN_G__TokenInfocLcLG__TokenType),-1,-2,1,"t_global=9",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__APILN_G__TokenInfocLcLG__TokenType),-1,-2,1,"t_enumelement=10",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__APILN_G__TokenInfocLcLG__TokenProperty),-1,-2,1,"p_invalid=0",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__APILN_G__TokenInfocLcLG__TokenProperty),-1,-2,1,"p_type=1",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__APILN_G__TokenInfocLcLG__TokenProperty),-1,-2,1,"p_data=2",0,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,105,0,1,G__get_linked_tagnum(&G__G__APILN_G__TokenInfocLcLG__TokenProperty),-1,-2,1,"p_func=3",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* G__SourceFileInfo */
static void G__setup_memvarG__SourceFileInfo(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__G__APILN_G__SourceFileInfo));
   { G__SourceFileInfo *p; p=(G__SourceFileInfo*)0x1000; if (p) { }
   }
   G__tag_memvar_reset();
}


   /* G__IncludePathInfo */
static void G__setup_memvarG__IncludePathInfo(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__G__APILN_G__IncludePathInfo));
   { G__IncludePathInfo *p; p=(G__IncludePathInfo*)0x1000; if (p) { }
   }
   G__tag_memvar_reset();
}

extern "C" void G__cpp_setup_memvarG__API() {
}
/***********************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
***********************************************************/

/*********************************************************
* Member function information setup for each class
*********************************************************/
static void G__setup_memfuncG__MethodInfo(void) {
   /* G__MethodInfo */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__G__APILN_G__MethodInfo));
   G__memfunc_setup("G__MethodInfo",1266,G__G__API_31_1_0,105,G__get_linked_tagnum(&G__G__APILN_G__MethodInfo),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("G__MethodInfo",1266,G__G__API_31_2_0,105,G__get_linked_tagnum(&G__G__APILN_G__MethodInfo),-1,0,1,1,1,0,"u 'G__ClassInfo' - 1 - a",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Init",404,G__G__API_31_3_0,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Init",404,G__G__API_31_4_0,121,-1,-1,0,1,1,1,0,"u 'G__ClassInfo' - 1 - a",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Init",404,G__G__API_31_5_0,121,-1,-1,0,3,1,1,0,
"l - - 0 - handlein l - - 0 - indexin "
"U 'G__ClassInfo' - 0 - belongingclassin",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Init",404,G__G__API_31_6_0,121,-1,-1,0,3,1,1,0,
"U 'G__ClassInfo' - 0 - belongingclassin l - - 0 - funcpage "
"l - - 0 - indexin",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Name",385,G__G__API_31_7_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Title",514,G__G__API_31_8_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Type",418,G__G__API_31_9_0,85,G__get_linked_tagnum(&G__G__APILN_G__TypeInfo),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Property",869,G__G__API_31_0_1,108,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("NArg",360,G__G__API_31_1_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("NDefaultArg",1069,G__G__API_31_2_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("HasVarArgs",978,G__G__API_31_3_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("InterfaceMethod",1522,G__G__API_31_4_1,89,-1,G__defined_typename("G__InterfaceMethod"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("GetBytecode",1103,G__G__API_31_5_1,85,G__get_linked_tagnum(&G__G__APILN_G__bytecodefunc),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("GetLocalVariable",1585,G__G__API_31_6_1,117,G__get_linked_tagnum(&G__G__APILN_G__DataMemberInfo),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("PointerToFunc",1328,G__G__API_31_7_1,89,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("MemberOf",781,G__G__API_31_8_1,85,G__get_linked_tagnum(&G__G__APILN_G__ClassInfo),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("GetFriendInfo",1284,G__G__API_31_9_1,85,G__get_linked_tagnum(&G__G__APILN_G__friendtag),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("SetGlobalcomp",1324,G__G__API_31_0_2,121,-1,-1,0,1,1,1,0,"i - - 0 - globalcomp",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsValid",684,G__G__API_31_1_2,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("SetFilePos",990,G__G__API_31_2_2,105,-1,-1,0,1,1,1,0,"C - - 10 - fname",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Next",415,G__G__API_31_3_2,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("FileName",769,G__G__API_31_4_2,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("LineNumber",1009,G__G__API_31_5_2,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Size",411,G__G__API_31_6_2,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsBusy",607,G__G__API_31_7_2,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("FilePointer",1121,G__G__API_31_8_2,69,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("FilePosition",1237,G__G__API_31_9_2,108,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("GetPrototype",1270,G__G__API_31_0_3,67,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("GetMangledName",1369,G__G__API_31_1_3,67,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("LoadDLLDirect",1207,G__G__API_31_2_3,105,-1,-1,0,2,1,1,0,
"C - - 10 - filename C - - 10 - funcname",(char*)NULL,(void*)NULL,0);
   // automatic copy constructor
   G__memfunc_setup("G__MethodInfo",1266,G__G__API_31_3_3,(int)('i'),G__get_linked_tagnum(&G__G__APILN_G__MethodInfo),-1,0,1,1,1,0,"u 'G__MethodInfo' - 11 - -",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~G__MethodInfo",1392,G__G__API_31_4_3,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncG__DataMemberInfo(void) {
   /* G__DataMemberInfo */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__G__APILN_G__DataMemberInfo));
   G__memfunc_setup("G__DataMemberInfo",1635,G__G__API_32_1_0,105,G__get_linked_tagnum(&G__G__APILN_G__DataMemberInfo),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("G__DataMemberInfo",1635,G__G__API_32_2_0,105,G__get_linked_tagnum(&G__G__APILN_G__DataMemberInfo),-1,0,1,1,1,0,"u 'G__ClassInfo' - 1 - a",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Init",404,G__G__API_32_3_0,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Init",404,G__G__API_32_4_0,121,-1,-1,0,1,1,1,0,"u 'G__ClassInfo' - 1 - a",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Init",404,G__G__API_32_5_0,121,-1,-1,0,3,1,1,0,
"l - - 0 - handlinin l - - 0 - indexin "
"U 'G__ClassInfo' - 0 - belongingclassin",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Name",385,G__G__API_32_6_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Title",514,G__G__API_32_7_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Type",418,G__G__API_32_8_0,85,G__get_linked_tagnum(&G__G__APILN_G__TypeInfo),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Property",869,G__G__API_32_9_0,108,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Offset",615,G__G__API_32_0_1,108,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Bitfield",803,G__G__API_32_1_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ArrayDim",793,G__G__API_32_2_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("MaxIndex",798,G__G__API_32_3_1,105,-1,-1,0,1,1,1,0,"i - - 0 - dim",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("MemberOf",781,G__G__API_32_4_1,85,G__get_linked_tagnum(&G__G__APILN_G__ClassInfo),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("SetGlobalcomp",1324,G__G__API_32_5_1,121,-1,-1,0,1,1,1,0,"i - - 0 - globalcomp",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsValid",684,G__G__API_32_6_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("SetFilePos",990,G__G__API_32_7_1,105,-1,-1,0,1,1,1,0,"C - - 10 - fname",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Next",415,G__G__API_32_8_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ValidArrayIndex",1511,G__G__API_32_9_1,67,-1,-1,0,2,1,1,1,
"I - - 0 0 errnum C - - 2 0 errstr",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("FileName",769,G__G__API_32_0_2,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("LineNumber",1009,G__G__API_32_1_2,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   // automatic copy constructor
   G__memfunc_setup("G__DataMemberInfo",1635,G__G__API_32_2_2,(int)('i'),G__get_linked_tagnum(&G__G__APILN_G__DataMemberInfo),-1,0,1,1,1,0,"u 'G__DataMemberInfo' - 11 - -",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~G__DataMemberInfo",1761,G__G__API_32_3_2,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncG__FriendInfo(void) {
   /* G__FriendInfo */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__G__APILN_G__FriendInfo));
   G__memfunc_setup("G__FriendInfo",1257,G__G__API_33_0_0,105,G__get_linked_tagnum(&G__G__APILN_G__FriendInfo),-1,0,1,1,1,0,"U 'G__friendtag' - 0 0 pin",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator=",937,G__G__API_33_1_0,121,-1,-1,0,1,1,1,0,"u 'G__FriendInfo' - 11 - x",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Init",404,G__G__API_33_2_0,121,-1,-1,0,1,1,1,0,"U 'G__friendtag' - 0 - pin",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("FriendOf",781,G__G__API_33_3_0,85,G__get_linked_tagnum(&G__G__APILN_G__ClassInfo),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Next",415,G__G__API_33_4_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsValid",684,G__G__API_33_5_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   // automatic copy constructor
   G__memfunc_setup("G__FriendInfo",1257,G__G__API_33_6_0,(int)('i'),G__get_linked_tagnum(&G__G__APILN_G__FriendInfo),-1,0,1,1,1,0,"u 'G__FriendInfo' - 11 - -",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~G__FriendInfo",1383,G__G__API_33_7_0,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncG__ClassInfo(void) {
   /* G__ClassInfo */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__G__APILN_G__ClassInfo));
   G__memfunc_setup("G__ClassInfo",1159,G__G__API_34_1_0,105,G__get_linked_tagnum(&G__G__APILN_G__ClassInfo),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Init",404,G__G__API_34_2_0,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("G__ClassInfo",1159,G__G__API_34_3_0,105,G__get_linked_tagnum(&G__G__APILN_G__ClassInfo),-1,0,1,1,1,0,"C - - 10 - classname",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Init",404,G__G__API_34_4_0,121,-1,-1,0,1,1,1,0,"C - - 10 - classname",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("G__ClassInfo",1159,G__G__API_34_5_0,105,G__get_linked_tagnum(&G__G__APILN_G__ClassInfo),-1,0,1,1,1,0,"i - - 0 - tagnumin",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Init",404,G__G__API_34_6_0,121,-1,-1,0,1,1,1,0,"i - - 0 - tagnumin",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator==",998,G__G__API_34_7_0,105,-1,-1,0,1,1,1,0,"u 'G__ClassInfo' - 11 - a",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator!=",970,G__G__API_34_8_0,105,-1,-1,0,1,1,1,0,"u 'G__ClassInfo' - 11 - a",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Name",385,G__G__API_34_9_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Fullname",820,G__G__API_34_0_1,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Title",514,G__G__API_34_1_1,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Size",411,G__G__API_34_2_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Property",869,G__G__API_34_3_1,108,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("NDataMembers",1171,G__G__API_34_4_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("NMethods",802,G__G__API_34_5_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsBase",567,G__G__API_34_6_1,108,-1,-1,0,1,1,1,0,"C - - 10 - classname",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsBase",567,G__G__API_34_7_1,108,-1,-1,0,1,1,1,0,"u 'G__ClassInfo' - 1 - a",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Tagnum",620,G__G__API_34_8_1,108,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("EnclosingClass",1432,G__G__API_34_9_1,117,G__get_linked_tagnum(&G__G__APILN_G__ClassInfo),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("EnclosingSpace",1422,G__G__API_34_0_2,117,G__get_linked_tagnum(&G__G__APILN_G__ClassInfo),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("GetFriendInfo",1284,G__G__API_34_1_2,85,G__get_linked_tagnum(&G__G__APILN_G__friendtag),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("SetGlobalcomp",1324,G__G__API_34_2_2,121,-1,-1,0,1,1,1,0,"i - - 0 - globalcomp",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("SetProtectedAccess",1832,G__G__API_34_3_2,121,-1,-1,0,1,1,1,0,"i - - 0 - protectedaccess",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsValid",684,G__G__API_34_4_2,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsLoaded",773,G__G__API_34_5_2,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("SetFilePos",990,G__G__API_34_6_2,105,-1,-1,0,1,1,1,0,"C - - 10 - fname",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Next",415,G__G__API_34_7_2,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Linkage",699,G__G__API_34_8_2,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("FileName",769,G__G__API_34_9_2,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("LineNumber",1009,G__G__API_34_0_3,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsTmplt",717,G__G__API_34_1_3,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("TmpltName",914,G__G__API_34_2_3,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("TmpltArg",811,G__G__API_34_3_3,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("SetDefFile",955,G__G__API_34_4_3,121,-1,-1,0,1,1,1,0,"C - - 0 - deffilein",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("SetDefLine",963,G__G__API_34_5_3,121,-1,-1,0,1,1,1,0,"i - - 0 - deflinein",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("SetImpFile",978,G__G__API_34_6_3,121,-1,-1,0,1,1,1,0,"C - - 0 - impfilein",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("SetImpLine",986,G__G__API_34_7_3,121,-1,-1,0,1,1,1,0,"i - - 0 - implinein",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("SetVersion",1042,G__G__API_34_8_3,121,-1,-1,0,1,1,1,0,"i - - 0 - versionin",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("DefFile",655,G__G__API_34_9_3,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("DefLine",663,G__G__API_34_0_4,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ImpFile",678,G__G__API_34_1_4,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ImpLine",686,G__G__API_34_2_4,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Version",742,G__G__API_34_3_4,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("New",298,G__G__API_34_4_4,89,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("New",298,G__G__API_34_5_4,89,-1,-1,0,1,1,1,0,"i - - 0 - n",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("New",298,G__G__API_34_6_4,89,-1,-1,0,1,1,1,0,"Y - - 0 - arena",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("InstanceCount",1342,G__G__API_34_7_4,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ResetInstanceCount",1857,G__G__API_34_8_4,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IncInstanceCount",1624,G__G__API_34_9_4,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("HeapInstanceCount",1724,G__G__API_34_0_5,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IncHeapInstanceCount",2006,G__G__API_34_1_5,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ResetHeapInstanceCount",2239,G__G__API_34_2_5,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("RootFlag",798,G__G__API_34_3_5,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("GetInterfaceMethod",1810,G__G__API_34_4_5,89,-1,G__defined_typename("G__InterfaceMethod"),0,4,1,1,0,
"C - - 10 - fname C - - 10 - arg "
"L - - 0 - poffset i 'G__ClassInfo::MatchMode' - 0 ConversionMatch mode",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("GetMethod",897,G__G__API_34_5_5,117,G__get_linked_tagnum(&G__G__APILN_G__MethodInfo),-1,0,4,1,1,0,
"C - - 10 - fname C - - 10 - arg "
"L - - 0 - poffset i 'G__ClassInfo::MatchMode' - 0 ConversionMatch mode",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("GetDataMember",1266,G__G__API_34_6_5,117,G__get_linked_tagnum(&G__G__APILN_G__DataMemberInfo),-1,0,2,1,1,0,
"C - - 10 - name L - - 0 - poffset",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("HasMethod",893,G__G__API_34_7_5,105,-1,-1,0,1,1,1,0,"C - - 10 - fname",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("HasDataMember",1262,G__G__API_34_8_5,105,-1,-1,0,1,1,1,0,"C - - 10 - name",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("HasDefaultConstructor",2183,G__G__API_34_9_5,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ClassProperty",1371,G__G__API_34_1_6,108,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   // automatic copy constructor
   G__memfunc_setup("G__ClassInfo",1159,G__G__API_34_2_6,(int)('i'),G__get_linked_tagnum(&G__G__APILN_G__ClassInfo),-1,0,1,1,1,0,"u 'G__ClassInfo' - 11 - -",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~G__ClassInfo",1285,G__G__API_34_3_6,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncG__BaseClassInfo(void) {
   /* G__BaseClassInfo */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__G__APILN_G__BaseClassInfo));
   G__memfunc_setup("G__BaseClassInfo",1538,G__G__API_36_1_0,105,G__get_linked_tagnum(&G__G__APILN_G__BaseClassInfo),-1,0,1,1,1,0,"u 'G__ClassInfo' - 1 - a",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Init",404,G__G__API_36_2_0,121,-1,-1,0,1,1,1,0,"u 'G__ClassInfo' - 1 - a",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Offset",615,G__G__API_36_3_0,108,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Property",869,G__G__API_36_4_0,108,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsValid",684,G__G__API_36_5_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Next",415,G__G__API_36_6_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   // automatic copy constructor
   G__memfunc_setup("G__BaseClassInfo",1538,G__G__API_36_7_0,(int)('i'),G__get_linked_tagnum(&G__G__APILN_G__BaseClassInfo),-1,0,1,1,1,0,"u 'G__BaseClassInfo' - 11 - -",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~G__BaseClassInfo",1664,G__G__API_36_8_0,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncG__TypeInfo(void) {
   /* G__TypeInfo */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__G__APILN_G__TypeInfo));
   G__memfunc_setup("G__TypeInfo",1075,G__G__API_37_1_0,105,G__get_linked_tagnum(&G__G__APILN_G__TypeInfo),-1,0,1,1,1,0,"C - - 10 - typenamein",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("G__TypeInfo",1075,G__G__API_37_2_0,105,G__get_linked_tagnum(&G__G__APILN_G__TypeInfo),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Init",404,G__G__API_37_3_0,121,-1,-1,0,1,1,1,0,"C - - 10 - typenamein",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator==",998,G__G__API_37_4_0,105,-1,-1,0,1,1,1,0,"u 'G__TypeInfo' - 11 - a",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("operator!=",970,G__G__API_37_5_0,105,-1,-1,0,1,1,1,0,"u 'G__TypeInfo' - 11 - a",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Name",385,G__G__API_37_6_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("TrueName",801,G__G__API_37_7_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Size",411,G__G__API_37_8_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Property",869,G__G__API_37_9_0,108,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsValid",684,G__G__API_37_0_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("New",298,G__G__API_37_1_1,89,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Typenum",754,G__G__API_37_2_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Type",418,G__G__API_37_3_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Reftype",735,G__G__API_37_4_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Isconst",739,G__G__API_37_5_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   // automatic copy constructor
   G__memfunc_setup("G__TypeInfo",1075,G__G__API_37_7_1,(int)('i'),G__get_linked_tagnum(&G__G__APILN_G__TypeInfo),-1,0,1,1,1,0,"u 'G__TypeInfo' - 11 - -",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~G__TypeInfo",1201,G__G__API_37_8_1,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncG__MethodArgInfo(void) {
   /* G__MethodArgInfo */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__G__APILN_G__MethodArgInfo));
   G__memfunc_setup("Init",404,G__G__API_38_1_0,121,-1,-1,0,1,1,1,0,"u 'G__MethodInfo' - 1 - a",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("G__MethodArgInfo",1548,G__G__API_38_2_0,105,G__get_linked_tagnum(&G__G__APILN_G__MethodArgInfo),-1,0,1,1,1,0,"u 'G__MethodInfo' - 1 - a",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Name",385,G__G__API_38_3_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Type",418,G__G__API_38_4_0,85,G__get_linked_tagnum(&G__G__APILN_G__TypeInfo),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Property",869,G__G__API_38_5_0,108,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("DefaultValue",1218,G__G__API_38_6_0,67,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ArgOf",463,G__G__API_38_7_0,85,G__get_linked_tagnum(&G__G__APILN_G__MethodInfo),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsValid",684,G__G__API_38_8_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Next",415,G__G__API_38_9_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("G__MethodArgInfo",1548,G__G__API_38_0_1,105,G__get_linked_tagnum(&G__G__APILN_G__MethodArgInfo),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   // automatic copy constructor
   G__memfunc_setup("G__MethodArgInfo",1548,G__G__API_38_1_1,(int)('i'),G__get_linked_tagnum(&G__G__APILN_G__MethodArgInfo),-1,0,1,1,1,0,"u 'G__MethodArgInfo' - 11 - -",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~G__MethodArgInfo",1674,G__G__API_38_2_1,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncG__CallFunc(void) {
   /* G__CallFunc */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__G__APILN_G__CallFunc));
   G__memfunc_setup("G__CallFunc",1037,G__G__API_40_1_0,105,G__get_linked_tagnum(&G__G__APILN_G__CallFunc),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Init",404,G__G__API_40_2_0,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("SetFunc",696,G__G__API_40_3_0,121,-1,-1,0,5,1,1,0,
"U 'G__ClassInfo' - 0 - cls C - - 10 - fname "
"C - - 10 - args L - - 0 - poffset "
"i 'G__CallFunc::MatchMode' - 0 ConversionMatch mode",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("SetFuncProto",1228,G__G__API_40_4_0,121,-1,-1,0,4,1,1,0,
"U 'G__ClassInfo' - 0 - cls C - - 10 - fname "
"C - - 10 - argtype L - - 0 - poffset",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("SetFunc",696,G__G__API_40_5_0,121,-1,-1,0,1,1,1,0,"Y - 'G__InterfaceMethod' 0 - f",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("SetFunc",696,G__G__API_40_6_0,121,-1,-1,0,1,1,1,0,"u 'G__MethodInfo' - 0 - m",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("SetBytecode",1115,G__G__API_40_7_0,121,-1,-1,0,1,1,1,0,"U 'G__bytecodefunc' - 0 - bc",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsValid",684,G__G__API_40_8_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("SetArgArray",1093,G__G__API_40_9_0,121,-1,-1,0,2,1,1,0,
"L - - 0 - p i - - 0 -1 narg",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ResetArg",797,G__G__API_40_0_1,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("SetArg",582,G__G__API_40_1_1,121,-1,-1,0,1,1,1,0,"l - - 0 - l",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("SetArg",582,G__G__API_40_2_1,121,-1,-1,0,1,1,1,0,"d - - 0 - d",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Exec",389,G__G__API_40_3_1,121,-1,-1,0,1,1,1,0,"Y - - 0 - pobject",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ExecInt",688,G__G__API_40_4_1,108,-1,-1,0,1,1,1,0,"Y - - 0 - pobject",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ExecDouble",992,G__G__API_40_5_1,100,-1,-1,0,1,1,1,0,"Y - - 0 - pobject",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("InterfaceMethod",1522,G__G__API_40_6_1,89,-1,G__defined_typename("G__InterfaceMethod"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("SetArgs",697,G__G__API_40_7_1,121,-1,-1,0,1,1,1,0,"C - - 10 - args",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("GetMethodInfo",1293,G__G__API_40_8_1,117,G__get_linked_tagnum(&G__G__APILN_G__MethodInfo),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   // automatic copy constructor
   G__memfunc_setup("G__CallFunc",1037,G__G__API_40_0_2,(int)('i'),G__get_linked_tagnum(&G__G__APILN_G__CallFunc),-1,0,1,1,1,0,"u 'G__CallFunc' - 11 - -",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~G__CallFunc",1163,G__G__API_40_1_2,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncG__TypedefInfo(void) {
   /* G__TypedefInfo */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__G__APILN_G__TypedefInfo));
   G__memfunc_setup("G__TypedefInfo",1378,G__G__API_42_1_0,105,G__get_linked_tagnum(&G__G__APILN_G__TypedefInfo),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Init",404,G__G__API_42_2_0,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("G__TypedefInfo",1378,G__G__API_42_3_0,105,G__get_linked_tagnum(&G__G__APILN_G__TypedefInfo),-1,0,1,1,1,0,"C - - 10 - typenamein",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Init",404,G__G__API_42_4_0,121,-1,-1,0,1,1,1,0,"C - - 10 - typenamein",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("G__TypedefInfo",1378,G__G__API_42_5_0,105,G__get_linked_tagnum(&G__G__APILN_G__TypedefInfo),-1,0,1,1,1,0,"i - - 0 - typenumin",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Init",404,G__G__API_42_6_0,121,-1,-1,0,1,1,1,0,"i - - 0 - typenumin",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("EnclosingClassOfTypedef",2334,G__G__API_42_7_0,117,G__get_linked_tagnum(&G__G__APILN_G__ClassInfo),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Title",514,G__G__API_42_8_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("SetGlobalcomp",1324,G__G__API_42_9_0,121,-1,-1,0,1,1,1,0,"i - - 0 - globalcomp",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsValid",684,G__G__API_42_0_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("SetFilePos",990,G__G__API_42_1_1,105,-1,-1,0,1,1,1,0,"C - - 10 - fname",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Next",415,G__G__API_42_2_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("FileName",769,G__G__API_42_3_1,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("LineNumber",1009,G__G__API_42_4_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   // automatic copy constructor
   G__memfunc_setup("G__TypedefInfo",1378,G__G__API_42_5_1,(int)('i'),G__get_linked_tagnum(&G__G__APILN_G__TypedefInfo),-1,0,1,1,1,0,"u 'G__TypedefInfo' - 11 - -",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~G__TypedefInfo",1504,G__G__API_42_6_1,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncG__TokenInfo(void) {
   /* G__TokenInfo */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__G__APILN_G__TokenInfo));
   G__memfunc_setup("G__TokenInfo",1170,G__G__API_43_1_0,105,G__get_linked_tagnum(&G__G__APILN_G__TokenInfo),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Init",404,G__G__API_43_2_0,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("MakeLocalTable",1361,G__G__API_43_3_0,117,G__get_linked_tagnum(&G__G__APILN_G__MethodInfo),-1,0,3,1,1,0,
"u 'G__ClassInfo' - 1 - tag_scope C - - 10 - fname "
"C - - 10 - paramtype",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Query",534,G__G__API_43_4_0,105,-1,-1,0,5,1,1,0,
"u 'G__ClassInfo' - 1 - tag_scope u 'G__MethodInfo' - 1 - func_scope "
"C - - 10 - preopr C - - 10 - name "
"C - - 10 - postopr",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("GetTokenType",1219,G__G__API_43_5_0,105,G__get_linked_tagnum(&G__G__APILN_G__TokenInfocLcLG__TokenType),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("GetTokenProperty",1670,G__G__API_43_6_0,105,G__get_linked_tagnum(&G__G__APILN_G__TokenInfocLcLG__TokenProperty),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("GetNextScope",1209,G__G__API_43_7_0,117,G__get_linked_tagnum(&G__G__APILN_G__ClassInfo),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   // automatic copy constructor
   G__memfunc_setup("G__TokenInfo",1170,G__G__API_43_5_1,(int)('i'),G__get_linked_tagnum(&G__G__APILN_G__TokenInfo),-1,0,1,1,1,0,"u 'G__TokenInfo' - 11 - -",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~G__TokenInfo",1296,G__G__API_43_6_1,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncG__SourceFileInfo(void) {
   /* G__SourceFileInfo */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__G__APILN_G__SourceFileInfo));
   G__memfunc_setup("G__SourceFileInfo",1666,G__G__API_46_0_0,105,G__get_linked_tagnum(&G__G__APILN_G__SourceFileInfo),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("G__SourceFileInfo",1666,G__G__API_46_1_0,105,G__get_linked_tagnum(&G__G__APILN_G__SourceFileInfo),-1,0,1,1,1,0,"i - - 0 - filenin",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("G__SourceFileInfo",1666,G__G__API_46_2_0,105,G__get_linked_tagnum(&G__G__APILN_G__SourceFileInfo),-1,0,1,1,1,0,"C - - 10 - fname",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Init",404,G__G__API_46_4_0,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Init",404,G__G__API_46_5_0,121,-1,-1,0,1,1,1,0,"C - - 10 - fname",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Name",385,G__G__API_46_6_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Prepname",824,G__G__API_46_7_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("fp",214,G__G__API_46_8_0,69,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("MaxLine",686,G__G__API_46_9_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IncludedFrom",1212,G__G__API_46_0_1,117,G__get_linked_tagnum(&G__G__APILN_G__SourceFileInfo),-1,1,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Property",869,G__G__API_46_1_1,108,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsValid",684,G__G__API_46_2_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Next",415,G__G__API_46_3_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   // automatic copy constructor
   G__memfunc_setup("G__SourceFileInfo",1666,G__G__API_46_4_1,(int)('i'),G__get_linked_tagnum(&G__G__APILN_G__SourceFileInfo),-1,0,1,1,1,0,"u 'G__SourceFileInfo' - 11 - -",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~G__SourceFileInfo",1792,G__G__API_46_5_1,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncG__IncludePathInfo(void) {
   /* G__IncludePathInfo */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__G__APILN_G__IncludePathInfo));
   G__memfunc_setup("G__IncludePathInfo",1762,G__G__API_47_0_0,105,G__get_linked_tagnum(&G__G__APILN_G__IncludePathInfo),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Init",404,G__G__API_47_2_0,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Name",385,G__G__API_47_3_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Property",869,G__G__API_47_4_0,108,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsValid",684,G__G__API_47_5_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Next",415,G__G__API_47_6_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   // automatic copy constructor
   G__memfunc_setup("G__IncludePathInfo",1762,G__G__API_47_7_0,(int)('i'),G__get_linked_tagnum(&G__G__APILN_G__IncludePathInfo),-1,0,1,1,1,0,"u 'G__IncludePathInfo' - 11 - -",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~G__IncludePathInfo",1888,G__G__API_47_8_0,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__tag_memfunc_reset();
}


/*********************************************************
* Member function information setup
*********************************************************/
extern "C" void G__cpp_setup_memfuncG__API() {
}

/*********************************************************
* Global variable information setup for each class
*********************************************************/
static void G__cpp_setup_global0() {

   /* Setting up global variables */
   G__resetplocal();

}

static void G__cpp_setup_global1() {
}

static void G__cpp_setup_global2() {
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__INFO_BUFLEN=50",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__INFO_TITLELEN=256",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__PROPERTY_H=0",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISTAGNUM=15",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISCLASS=1",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISSTRUCT=2",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISUNION=4",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISENUM=8",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISTYPEDEF=16",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISFUNDAMENTAL=32",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISABSTRACT=64",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISVIRTUAL=128",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISPUREVIRTUAL=256",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISPUBLIC=512",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISPROTECTED=1024",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISPRIVATE=2048",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISPOINTER=4096",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISARRAY=8192",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISSTATIC=16384",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISDEFAULT=32768",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISREFERENCE=65536",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISDIRECTINHERIT=131072",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISCCOMPILED=262144",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISCPPCOMPILED=524288",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISCOMPILED=786432",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISBYTECODE=33554432",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISCONSTANT=1048576",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISVIRTUALBASE=2097152",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISPCONSTANT=4194304",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISGLOBALVAR=8388608",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISLOCALVAR=16777216",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISEXPLICIT=67108864",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BIT_ISNAMESPACE=134217728",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__CLS_VALID=1",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__CLS_HASEXPLICITCTOR=16",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__CLS_HASIMPLICITCTOR=32",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__CLS_HASCTOR=48",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__CLS_HASDEFAULTCTOR=64",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__CLS_HASEXPLICITDTOR=256",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__CLS_HASIMPLICITDTOR=512",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__CLS_HASDTOR=768",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__CLS_HASVIRTUAL=4096",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__CLS_ISABSTRACT=8192",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__CLASSINFO_H=0",1,(char*)NULL);
}

static void G__cpp_setup_global3() {
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__BaseClassInfo_H=0",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__TYPEINFOX_H=0",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__METHODINFO_H=0",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__METHODARGINFO_H=0",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__DATAMEMBER_H=0",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__CALLFUNC_H=0",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__TYPEDEFINFO_H=0",1,(char*)NULL);
   G__memvar_setup((void*)G__PVOID,112,0,0,-1,-1,-1,1,"G__TOKENINFO_H=0",1,(char*)NULL);

   G__resetglobalenv();
}
extern "C" void G__cpp_setup_globalG__API() {
  G__cpp_setup_global0();
  G__cpp_setup_global1();
  G__cpp_setup_global2();
  G__cpp_setup_global3();
}

/*********************************************************
* Global function information setup for each class
*********************************************************/
static void G__cpp_setup_func0() {
   G__lastifuncposition();

}

static void G__cpp_setup_func1() {
   G__memfunc_setup("G__SetGlobalcomp",1585,G__G__API__8_18,105,-1,-1,0,3,1,1,0,
"C - - 0 - funcname C - - 0 - param "
"i - - 0 - globalcomp",(char*)NULL
,(void*)NULL,0);
   G__memfunc_setup("G__ForceBytecodecompilation",2754,G__G__API__9_18,105,-1,-1,0,2,1,1,0,
"C - - 0 - funcname C - - 0 - param",(char*)NULL
,(void*)NULL,0);
   G__memfunc_setup("G__InitGetSpecialObject",2257,G__G__API__0_19,121,-1,-1,0,1,1,1,0,"Y - 'G__pMethodSpecialObject' 0 - pmethod",(char*)NULL
,(void*)NULL,0);
   G__memfunc_setup("G__InitGetSpecialValue",2167,G__G__API__1_19,121,-1,-1,0,1,1,1,0,"Y - 'G__pMethodSpecialValue' 0 - pmethod",(char*)NULL
,(void*)NULL,0);
   G__memfunc_setup("G__InitUpdateClassInfo",2174,G__G__API__2_19,121,-1,-1,0,1,1,1,0,"Y - 'G__pMethodUpdateClassInfo' 0 - pmethod",(char*)NULL
,(void*)NULL,0);

   G__resetifuncposition();
}

extern "C" void G__cpp_setup_funcG__API() {
  G__cpp_setup_func0();
  G__cpp_setup_func1();
}

/*********************************************************
* Class,struct,union,enum tag information setup
*********************************************************/
/* Setup class/struct taginfo */
G__linked_taginfo G__G__APILN_G__friendtag = { "G__friendtag" , 115 , -1 };
G__linked_taginfo G__G__APILN_G__bytecodefunc = { "G__bytecodefunc" , 115 , -1 };
G__linked_taginfo G__G__APILN_G__MethodInfo = { "G__MethodInfo" , 99 , -1 };
G__linked_taginfo G__G__APILN_G__DataMemberInfo = { "G__DataMemberInfo" , 99 , -1 };
G__linked_taginfo G__G__APILN_G__FriendInfo = { "G__FriendInfo" , 99 , -1 };
G__linked_taginfo G__G__APILN_G__ClassInfo = { "G__ClassInfo" , 99 , -1 };
G__linked_taginfo G__G__APILN_G__ClassInfocLcLMatchMode = { "G__ClassInfo::MatchMode" , 101 , -1 };
G__linked_taginfo G__G__APILN_G__BaseClassInfo = { "G__BaseClassInfo" , 99 , -1 };
G__linked_taginfo G__G__APILN_G__TypeInfo = { "G__TypeInfo" , 99 , -1 };
G__linked_taginfo G__G__APILN_G__MethodArgInfo = { "G__MethodArgInfo" , 99 , -1 };
G__linked_taginfo G__G__APILN_G__DataMemberInfocLcLerror_code = { "G__DataMemberInfo::error_code" , 101 , -1 };
G__linked_taginfo G__G__APILN_G__CallFunc = { "G__CallFunc" , 99 , -1 };
G__linked_taginfo G__G__APILN_G__CallFunccLcLMatchMode = { "G__CallFunc::MatchMode" , 101 , -1 };
G__linked_taginfo G__G__APILN_G__TypedefInfo = { "G__TypedefInfo" , 99 , -1 };
G__linked_taginfo G__G__APILN_G__TokenInfo = { "G__TokenInfo" , 99 , -1 };
G__linked_taginfo G__G__APILN_G__TokenInfocLcLG__TokenType = { "G__TokenInfo::G__TokenType" , 101 , -1 };
G__linked_taginfo G__G__APILN_G__TokenInfocLcLG__TokenProperty = { "G__TokenInfo::G__TokenProperty" , 101 , -1 };
G__linked_taginfo G__G__APILN_G__SourceFileInfo = { "G__SourceFileInfo" , 99 , -1 };
G__linked_taginfo G__G__APILN_G__IncludePathInfo = { "G__IncludePathInfo" , 99 , -1 };

/* Reset class/struct taginfo */
extern "C" void G__cpp_reset_tagtableG__API() {
  G__G__APILN_G__friendtag.tagnum = -1 ;
  G__G__APILN_G__bytecodefunc.tagnum = -1 ;
  G__G__APILN_G__MethodInfo.tagnum = -1 ;
  G__G__APILN_G__DataMemberInfo.tagnum = -1 ;
  G__G__APILN_G__FriendInfo.tagnum = -1 ;
  G__G__APILN_G__ClassInfo.tagnum = -1 ;
  G__G__APILN_G__ClassInfocLcLMatchMode.tagnum = -1 ;
  G__G__APILN_G__BaseClassInfo.tagnum = -1 ;
  G__G__APILN_G__TypeInfo.tagnum = -1 ;
  G__G__APILN_G__MethodArgInfo.tagnum = -1 ;
  G__G__APILN_G__DataMemberInfocLcLerror_code.tagnum = -1 ;
  G__G__APILN_G__CallFunc.tagnum = -1 ;
  G__G__APILN_G__CallFunccLcLMatchMode.tagnum = -1 ;
  G__G__APILN_G__TypedefInfo.tagnum = -1 ;
  G__G__APILN_G__TokenInfo.tagnum = -1 ;
  G__G__APILN_G__TokenInfocLcLG__TokenType.tagnum = -1 ;
  G__G__APILN_G__TokenInfocLcLG__TokenProperty.tagnum = -1 ;
  G__G__APILN_G__SourceFileInfo.tagnum = -1 ;
  G__G__APILN_G__IncludePathInfo.tagnum = -1 ;
}


extern "C" void G__cpp_setup_tagtableG__API() {

   /* Setting up class,struct,union tag entry */
   G__get_linked_tagnum(&G__G__APILN_G__friendtag);
   G__get_linked_tagnum(&G__G__APILN_G__bytecodefunc);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__APILN_G__MethodInfo),sizeof(G__MethodInfo),-1,34048,(char*)NULL,G__setup_memvarG__MethodInfo,G__setup_memfuncG__MethodInfo);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__APILN_G__DataMemberInfo),sizeof(G__DataMemberInfo),-1,34048,(char*)NULL,G__setup_memvarG__DataMemberInfo,G__setup_memfuncG__DataMemberInfo);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__APILN_G__FriendInfo),sizeof(G__FriendInfo),-1,2304,(char*)NULL,G__setup_memvarG__FriendInfo,G__setup_memfuncG__FriendInfo);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__APILN_G__ClassInfo),sizeof(G__ClassInfo),-1,34048,(char*)NULL,G__setup_memvarG__ClassInfo,G__setup_memfuncG__ClassInfo);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__APILN_G__ClassInfocLcLMatchMode),0,-1,0,(char*)NULL,NULL,NULL);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__APILN_G__BaseClassInfo),sizeof(G__BaseClassInfo),-1,33792,(char*)NULL,G__setup_memvarG__BaseClassInfo,G__setup_memfuncG__BaseClassInfo);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__APILN_G__TypeInfo),sizeof(G__TypeInfo),-1,34048,(char*)NULL,G__setup_memvarG__TypeInfo,G__setup_memfuncG__TypeInfo);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__APILN_G__MethodArgInfo),sizeof(G__MethodArgInfo),-1,34048,(char*)NULL,G__setup_memvarG__MethodArgInfo,G__setup_memfuncG__MethodArgInfo);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__APILN_G__DataMemberInfocLcLerror_code),0,-1,0,(char*)NULL,NULL,NULL);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__APILN_G__CallFunc),sizeof(G__CallFunc),-1,1280,(char*)NULL,G__setup_memvarG__CallFunc,G__setup_memfuncG__CallFunc);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__APILN_G__CallFunccLcLMatchMode),0,-1,0,(char*)NULL,NULL,NULL);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__APILN_G__TypedefInfo),sizeof(G__TypedefInfo),-1,34048,(char*)NULL,G__setup_memvarG__TypedefInfo,G__setup_memfuncG__TypedefInfo);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__APILN_G__TokenInfo),sizeof(G__TokenInfo),-1,1280,(char*)NULL,G__setup_memvarG__TokenInfo,G__setup_memfuncG__TokenInfo);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__APILN_G__TokenInfocLcLG__TokenType),0,-1,0,(char*)NULL,NULL,NULL);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__APILN_G__TokenInfocLcLG__TokenProperty),0,-1,0,(char*)NULL,NULL,NULL);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__APILN_G__SourceFileInfo),sizeof(G__SourceFileInfo),-1,34048,(char*)NULL,G__setup_memvarG__SourceFileInfo,G__setup_memfuncG__SourceFileInfo);
   G__tagtable_setup(G__get_linked_tagnum(&G__G__APILN_G__IncludePathInfo),sizeof(G__IncludePathInfo),-1,1280,(char*)NULL,G__setup_memvarG__IncludePathInfo,G__setup_memfuncG__IncludePathInfo);
}
extern "C" void G__cpp_setupG__API(void) {
  G__check_setup_version(G__CREATEDLLREV,"G__cpp_setupG__API()");
  G__set_cpp_environmentG__API();
  G__cpp_setup_tagtableG__API();

  G__cpp_setup_inheritanceG__API();

  G__cpp_setup_typetableG__API();

  G__cpp_setup_memvarG__API();

  G__cpp_setup_memfuncG__API();
  G__cpp_setup_globalG__API();
  G__cpp_setup_funcG__API();

   if(0==G__getsizep2memfunc()) G__get_sizep2memfuncG__API();
  return;
}
