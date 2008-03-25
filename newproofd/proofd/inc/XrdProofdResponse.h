// @(#)root/proofd:$Id$
// Author: G. Ganis  June 2005

/*************************************************************************
 * Copyright (C) 1995-2005, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_XrdProofdResponse
#define ROOT_XrdProofdResponse

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// XrdProofdResponse                                                    //
//                                                                      //
// Authors: G. Ganis, CERN, 2005                                        //
//                                                                      //
// Utility class to handle replies to clients.                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <unistd.h>
#include <sys/uio.h>

#ifdef OLDXRDOUC
#  include "XrdSysToOuc.h"
#  include "XrdOuc/XrdOucPthread.hh"
#else
#  include "XrdSys/XrdSysPthread.hh"
#endif
#include "XrdOuc/XrdOucString.hh"
#include "XProofProtocol.h"

class XrdLink;

class XrdProofdResponse
{
 public:
   XrdProofdResponse() { fLink = 0; *fTrsid = '\0'; fSID = 0;
                         fRespIO[0].iov_base = (caddr_t)&fResp;
                         fRespIO[0].iov_len  = sizeof(fResp); }
   virtual ~XrdProofdResponse() {}

   inline const  char   *STRID() { return (const char *)fTrsid;}
   inline const char    *TraceID() const { return fTraceID.c_str(); }

   int                   LinkSend(const char *buff, int len);
   int                   LinkSend(const struct iovec *iov, int iocnt, int len);

   int                   Send(void);
   int                   Send(const char *msg);
   int                   Send(void *data, int dlen);
   int                   Send(struct iovec *, int iovcnt, int iolen=-1);
   int                   Send(XResponseType rcode);
   int                   Send(XResponseType rcode, void *data, int dlen);
   int                   Send(XErrorCode ecode, const char *msg);
   int                   Send(XPErrorCode ecode, const char *msg);
   int                   Send(XResponseType rcode, int info, char *data = 0);
   int                   Send(XResponseType rcode, XProofActionCode acode, int info);
   int                   Send(XResponseType rcode,
                              XProofActionCode acode, void *data, int dlen);
   int                   Send(XResponseType rcode, XProofActionCode acode,
                              kXR_int32 sid, void *data, int dlen);

   int                   SendI(kXR_int32 int1, void *data = 0, int dlen = 0);
   int                   SendI(kXR_int32 int1, kXR_int32 int2, void *data = 0, int dlen = 0);
   int                   SendI(kXR_int32 int1, kXR_int16 int2, kXR_int16 int3,
                               void *data = 0, int dlen = 0);

   void                  Set(XrdLink *l);
   inline void           SetTag(const char *tag) { fTag = tag; }
   void                  SetTraceID();
   void                  Set(unsigned char *stream);
   void                  Set(unsigned short streamid);

   void                  GetSID(unsigned short &sid);
   void                  SetTrsid();

   static void           SetMaxRetry(int mx) { fgMaxRetry = mx; }

   // To protect from concurrent use
   XrdSysRecMutex       fMutex;

 private:

   ServerResponseHeader fResp;
   XrdLink             *fLink;
   struct iovec         fRespIO[5];

   char                 fTrsid[8];  // sizeof() does not work here

   unsigned short       fSID;

   XrdOucString         fTraceID;
   XrdOucString         fTag;

   static int           fgMaxRetry; // Max number of retries on send failures
};
#endif
