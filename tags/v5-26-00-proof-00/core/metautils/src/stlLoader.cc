#include "G__ci.h"
#include "Api.h"

#ifndef __CINT__
static const char *what = WHAT;
#endif

static int stlLoader()
{
   G__ClassInfo cl("TSystem");
   if (cl.IsValid() && strlen(WHAT)<1000) {
      char buf[1024];
      sprintf(buf,"\"lib%sDict\"",what);

      G__CallFunc func;
      long offset;
      func.SetFuncProto(&cl,"Load","const char*",&offset);

      if (func.InterfaceMethod()) {
         long tmp = G__int(G__calc("gSystem")); 
         void * gsystem = (void*)tmp;

         func.SetArgs(buf);
         func.Exec(gsystem);      
      }
   }
   return 0;
}

static int sltLoad = stlLoader();
