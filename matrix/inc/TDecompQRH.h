// @(#)root/matrix:$Name:  $:$Id: TDecompQRH.h,v 1.4 2004/02/06 16:25:58 brun Exp $
// Authors: Fons Rademakers, Eddy Offermann   Dec 2003

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TDecompQRH
#define ROOT_TDecompQRH

///////////////////////////////////////////////////////////////////////////
//                                                                       //
// QR Decomposition class                                                //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TDecompBase
#include "TDecompBase.h"
#endif

class TDecompQRH : public TDecompBase
{
protected :

  //  A = fQ fR H (m x n) matrix
  TMatrixD fQ;  // (m x n) - orthogonal matrix
  TMatrixD fR;  // (n x n) - upper triangular matrix
  TVectorD fUp; // (n) - vector with Householder up's
  TVectorD fW;  // (n) - vector with Householder beta's

  static Bool_t QRH(TMatrixD &q,TVectorD &diagR,TVectorD &up,TVectorD &w,Double_t tol);

  virtual const TMatrixD &GetDecompMatrix() const { return fR; }

public :

  enum {kWorkMax = 100}; // size of work array

  TDecompQRH() {}
  TDecompQRH(const TMatrixD &m,Double_t tol = 0.0); // be careful for slicing in operator=
  TDecompQRH(const TDecompQRH &another);
  virtual ~TDecompQRH() {}

  virtual       Int_t     GetNrows () const { return fQ.GetNrows(); }
  virtual       Int_t     GetNcols () const { return fQ.GetNcols(); }
  virtual const TMatrixD &GetQ     ()       { if ( !( fStatus & kDecomposed ) ) Decompose();
                                              return fQ; }
  virtual const TMatrixD &GetR     ()       { if ( !( fStatus & kDecomposed ) ) Decompose();
                                              return fR; }
  virtual const TVectorD &GetUp    ()       { if ( !( fStatus & kDecomposed ) ) Decompose();
                                              return fUp; }
  virtual const TVectorD &GetW     ()       { if ( !( fStatus & kDecomposed ) ) Decompose();
                                              return fW; }

  virtual Int_t    Decompose  ();
  virtual Bool_t   Solve      (      TVectorD &b);
  virtual TVectorD Solve      (const TVectorD& b,Bool_t &ok);
  virtual Bool_t   Solve      (      TMatrixDColumn &b);
  virtual Bool_t   TransSolve (      TVectorD &b);
  virtual TVectorD TransSolve (const TVectorD& b,Bool_t &ok);
  virtual Bool_t   TransSolve (      TMatrixDColumn &b);
  virtual void     Det        (Double_t &d1,Double_t &d2);

  TDecompQRH &operator= (const TDecompQRH &source);

  ClassDef(TDecompQRH,1) // Matrix Decompositition QRH
};

#endif
