// @(#)root/matrix:$Name:  $:$Id: TDecompSVD.cxx,v 1.47 2003/09/05 09:21:54 brun Exp $
// Authors: Fons Rademakers, Eddy Offermann  Dec 2003

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

///////////////////////////////////////////////////////////////////////////
//                                                                       //
// Single Value Decomposition class                                      //
//                                                                       //
// For an (m x n) matrix A with m >= n, the singular value decomposition //
// is                                                                    //
// an (m x m) orthogonal matrix fU, an (m x n) diagonal matrix fS, and   //
// an (n x n) orthogonal matrix fV so that A = U*S*V'.                   //
//                                                                       //
// The singular values, fSig[k] = S[k][k], are ordered so that           //
// fSig[0] >= fSig[1] >= ... >= fSig[n-1].                               //
//                                                                       //
// The singular value decompostion always exists, so the decomposition   //
// will (as long as m >=n) never fail.                                   //
//                                                                       //
// Here fTol is used to set the thresold on the minimum allowed value of //
// the singular values:                                                  //
//  min_singular = fTol*max(fSig[i])                                     //
//                                                                       //
///////////////////////////////////////////////////////////////////////////

#include "TDecompSVD.h"
#include "TArrayD.h"

ClassImp(TDecompSVD)

//______________________________________________________________________________
TDecompSVD::TDecompSVD(const TMatrixD &a,Double_t tol)
{
  Assert(a.IsValid());
  if (a.GetNrows() < a.GetNcols()) {
    Error("TDecompSVD(const TMatrixD &","matrix rows should be >= columns");
    return;
  }

  fCondition = -1.0;
  fTol = a.GetTol();
  if (tol > 0)
    fTol = tol;

  Decompose(a);
}

//______________________________________________________________________________
TDecompSVD::TDecompSVD(const TDecompSVD &another): TDecompBase(another)
{
  *this = another;
}

//______________________________________________________________________________
Int_t TDecompSVD::Decompose(const TMatrixDBase &a)
{
  const Int_t nRow   = a.GetNrows();
  const Int_t nCol   = a.GetNcols();
  const Int_t rowLwb = a.GetRowLwb();
  const Int_t colLwb = a.GetColLwb();

  fU.ResizeTo(nRow,nRow);
  fSig.ResizeTo(nCol);
  fV.ResizeTo(nRow,nCol); // In the end we only need the nColxnCol part

  fU.UnitMatrix();
  memcpy(fV.GetElements(),a.GetElements(),nRow*nCol*sizeof(Double_t));

  TVectorD offDiag(nCol);

  // step 1: bidiagonalization of A
  if (!Bidiagonalize(fV,fU,fSig,offDiag))
    return kFALSE;

  // step 2: diagonalization of bidiagonal matrix
  if (!Diagonalize(fV,fU,fSig,offDiag))
    return kFALSE;

  // step 3: order singular values and perform permutations
  SortSingular(fV,fU,fSig);
  fV.ResizeTo(nCol,nCol); fV.Shift(0,colLwb);
  fU.Transpose(fU);       fU.Shift(rowLwb,0);
  fStatus |= kDecomposed;

  return kTRUE;
}

//______________________________________________________________________________
Bool_t TDecompSVD::Bidiagonalize(TMatrixD &v,TMatrixD &u,TVectorD &sDiag,TVectorD &oDiag)
{
// Bidiagonalize the (m x n) - matrix a (stored in v) through a series of Householder
// transformations applied to the left (Q^T) and to the right (H) of a ,
// so that A = Q . C . H^T with matrix C bidiagonal. Q and H are orthogonal matrices .
//
// Output:
//   v     - (n x n) - matrix H in the (n x n) part of v
//   u     - (m x m) - matrix Q^T
//   sDiag - diagonal of the (m x n) C
//   oDiag - off-diagonal elements of matrix C
//
//  Test code for the output:
//    const Int_t nRow = v.GetNrows();
//    const Int_t nCol = v.GetNcols();
//    TMatrixD H(v); H.ResizeTo(nCol,nCol);
//    TMatrixD E1(nCol,nCol); E1.UnitMatrix();
//    TMatrixD Ht(TMatrixDBase::kTransposed,H);
//    Bool_t ok = kTRUE;
//    ok &= VerifyMatrixIdentity(Ht * H,E1,kTRUE,1.0e-13);
//    ok &= VerifyMatrixIdentity(H * Ht,E1,kTRUE,1.0e-13);
//    TMatrixD E2(nRow,nRow); E2.UnitMatrix();
//    TMatrixD Qt(u);
//    TMatrixD Q(TMatrixDBase::kTransposed,Qt);
//    ok &= VerifyMatrixIdentity(Q * Qt,E2,kTRUE,1.0e-13);
//    TMatrixD C(nRow,nCol);
//    TMatrixDDiag(C,0) = sDiag;
//    for (Int_t i = 0; i < nCol-1; i++)
//      C(i,i+1) = oDiag(i+1);
//    TMatrixD A = Q*C*Ht;
//    ok &= VerifyMatrixIdentity(A,a,kTRUE,1.0e-13);

  const Int_t nRow_v = v.GetNrows();
  const Int_t nCol_v = v.GetNcols();
  const Int_t nCol_u = u.GetNcols();

  TArrayD ups(nCol_v);
  TArrayD betas(nCol_v);

  for (Int_t i = 0; i < nCol_v; i++) {
    // Set up Householder Transformation q(i)

    Double_t up,beta;
    if (i < nCol_v-1 || nRow_v > nCol_v) {
      const TVectorD vc_i = TMatrixDColumn_const(v,i);
      //if (!DefHouseHolder(vc_i,i,i+1,up,beta))
      //  return kFALSE;
      DefHouseHolder(vc_i,i,i+1,up,beta);

      // Apply q(i) to v
      for (Int_t j = i; j < nCol_v; j++) {
        TMatrixDColumn vc_j = TMatrixDColumn(v,j);
        ApplyHouseHolder(vc_i,up,beta,i,i+1,vc_j);
      }

      // Apply q(i) to u
      for (Int_t j = 0; j < nCol_u; j++)
      {
        TMatrixDColumn uc_j = TMatrixDColumn(u,j);
        ApplyHouseHolder(vc_i,up,beta,i,i+1,uc_j);
      }
    }
    if (i < nCol_v-2) {
      // set up Householder Transformation h(i)
      const TVectorD vr_i = TMatrixDRow_const(v,i);

      //if (!DefHouseHolder(vr_i,i+1,i+2,up,beta))
      //  return kFALSE;
      DefHouseHolder(vr_i,i+1,i+2,up,beta);

      // save h(i)
      ups[i]   = up;
      betas[i] = beta;

      // apply h(i) to v
      for (Int_t j = i; j < nRow_v; j++) {
        TMatrixDRow vr_j = TMatrixDRow(v,j);
        ApplyHouseHolder(vr_i,up,beta,i+1,i+2,vr_j);

        // save elements i+2,...in row j of matrix v
        if (j == i) {
          for (Int_t k = i+2; k < nCol_v; k++)
            vr_j(k) = vr_i(k);
        }
      }
    }
  }

  // copy diagonal of transformed matrix v to sDiag and upper parallel v to oDiag
  if (nCol_v > 1) {
    sDiag = TMatrixDDiag(v);
    for (Int_t i = 1; i < nCol_v; i++)
      oDiag(i) = v(i-1,i);
  }
  oDiag(0) = 0.;

  // construct product matrix h = h(1)*h(2)*...*h(nCol_v-1), h(nCol_v-1) = I

  TVectorD vr_i(nCol_v);
  for (Int_t i = nCol_v-1; i >= 0; i--) {
    if (i < nCol_v-1)
      vr_i = TMatrixDRow_const(v,i);
    TMatrixDRow(v,i) = 0.0;
    v(i,i) = 1.;

    if (i < nCol_v-2) {
      for (Int_t k = i; k < nCol_v; k++) {
        // householder transformation on k-th column
        TMatrixDColumn vc_k = TMatrixDColumn(v,k);
        ApplyHouseHolder(vr_i,ups[i],betas[i],i+1,i+2,vc_k);
      }
    }
  }

  return kTRUE;
}

//______________________________________________________________________________
Bool_t TDecompSVD::Diagonalize(TMatrixD &v,TMatrixD &u,TVectorD &sDiag,TVectorD &oDiag)
{
// Diagonalizes in an iterativ fashion the bidiagonal matrix C as described through
// sDiag and oDiag, so that S' = U'^T . C . V' is diagonal. U' and V' are orthogonal
// matrices .
//
// Output:
//   v     - (n x n) - matrix H . V' in the (n x n) part of v
//   u     - (m x m) - matrix U'^T . Q^T     
//   sDiag - diagonal of the (m x n) S'
//
//   return convergence flag:  0 -> no convergence
//                             1 -> convergence
//
//  Test code for the output:
//    const Int_t nRow = v.GetNrows();
//    const Int_t nCol = v.GetNcols();
//    TMatrixD tmp = v; tmp.ResizeTo(nCol,nCol);
//    TMatrixD Vprime  = Ht*tmp;
//    TMatrixD Vprimet(TMatrixDBase::kTransposed,Vprime);
//    TMatrixD Uprimet = u*Q;
//    TMatrixD Uprime(TMatrixDBase::kTransposed,Uprimet);
//    TMatrixD Sprime(nRow,nCol);
//    TMatrixDDiag(Sprime,0) = sDiag;
//    ok &= VerifyMatrixIdentity(Uprimet * C * Vprime,Sprime,kTRUE,1.0e-13);
//    ok &= VerifyMatrixIdentity(Q*Uprime * Sprime * Vprimet * Ht,a,kTRUE,1.0e-13);

  Bool_t ok    = kTRUE;
  Int_t niter  = 0;
  Double_t bmx = sDiag(0);

  const Int_t nCol_v = v.GetNcols();

  if (nCol_v > 1) {
    for (Int_t i = 1; i < nCol_v; i++)
      bmx = TMath::Max(TMath::Abs(sDiag(i))+TMath::Abs(oDiag(i)),bmx);
  }

  const Int_t niterm = 10*nCol_v;
  for (Int_t k = nCol_v-1; k >= 0; k--) {
    loop:
      if (k != 0) {
        // since sDiag(k) == 0 perform Givens transform with result oDiag[k] = 0
        if ((bmx+sDiag(k))-bmx == 0.0)
          Diag_1(v,sDiag,oDiag,k);

        // find l (1 <= l <=k) so that either oDiag(l) = 0 or sDiag(l-1) = 0.
        // In the latter case transform oDiag(l) to zero. In both cases the matrix
        // splits and the bottom right minor begins with row l.
        // If no such l is found set l = 1.

        Int_t elzero = 0;
        Int_t l = 0;
        for (Int_t ll = k; ll >= 0 ; ll--) {
          l = ll;
          if (l == 0) {
            elzero = 0;
            break;
          }
          else if ((bmx-oDiag(l))-bmx == 0.0) {
            elzero = 1;
            break;
          }
          else if ((bmx+sDiag(l-1))-bmx == 0.0)
            elzero = 0;
        }
        if (l > 0 && !elzero)
          Diag_2(sDiag,oDiag,k,l);
        if (l != k) {
          // one more QR pass with order k
          Diag_3(v,u,sDiag,oDiag,k,l);
          niter++;
          if (niter <= niterm) goto loop;
          Error("Diagonalize","no convergence after %d steps",niter);
          ok = kFALSE;
        }
     }

    if (sDiag(k) < 0.) {
     // for negative singular values perform change of sign
      sDiag(k) = -sDiag(k);
      TMatrixDColumn(v,k) *= -1.0;
    }
    // order is decreased by one in next pass
  }

  return ok;
}

//______________________________________________________________________________
void TDecompSVD::Diag_1(TMatrixD &v,TVectorD &sDiag,TVectorD &oDiag,Int_t k)
{
  const Int_t nCol_v = v.GetNcols();

  TMatrixDColumn vc_k = TMatrixDColumn(v,k);
  for (Int_t i = k-1; i >= 0; i--) {
    TMatrixDColumn vc_i = TMatrixDColumn(v,i);
    Double_t h,cs,sn;
    if (i == k-1)
      DefAplGivens(sDiag[i],oDiag[i+1],cs,sn);
    else
      DefAplGivens(sDiag[i],h,cs,sn);
    if (i > 0) {
      h = 0.;
      ApplyGivens(oDiag[i],h,cs,sn);
    }
    for (Int_t j = 0; j < nCol_v; j++)
      ApplyGivens(vc_i(j),vc_k(j),cs,sn);
  }
}

//______________________________________________________________________________
void TDecompSVD::Diag_2(TVectorD &sDiag,TVectorD &oDiag,Int_t k,Int_t l)
{
  for (Int_t i = l; i <= k; i++) {
    Double_t h,cs,sn;
    if (i == l)
      DefAplGivens(sDiag(i),oDiag(i),cs,sn);
    else
      DefAplGivens(sDiag(i),h,cs,sn);
    if (i < k) {
      h = 0.;
      ApplyGivens(oDiag(i+1),h,cs,sn);
    }
  }
}

//______________________________________________________________________________
void TDecompSVD::Diag_3(TMatrixD &v,TMatrixD &u,TVectorD &sDiag,TVectorD &oDiag,Int_t k,Int_t l)
{
  Double_t *pS = sDiag.GetElements();
  Double_t *pO = oDiag.GetElements();

  // determine shift parameter
  Double_t f = ((pS[k-1]-pS[k])*(pS[k-1]+pS[k])+(pO[k-1]-pO[k])*(pO[k-1]+pO[k]))/
                (2.*pO[k]*pS[k-1]);

  const Double_t g = (TMath::Abs(f) > 1.e+10) ? TMath::Abs(f) :TMath::Sqrt(1.+f*f);
  const Double_t t = (f >= 0.) ? f+g : f-g;

  f = ((pS[l]-pS[k])*(pS[l]+pS[k])+pO[k]*(pS[k-1]/t-pO[k]))/pS[l];

  const Int_t nCol_v = v.GetNcols();
  const Int_t nCol_u = u.GetNcols();

  Double_t h,cs,sn;
  for (Int_t i = l; i <= k-1; i++) {
    if (i == l)
      // define r[l]
      DefGivens(f,pO[i+1],cs,sn);
    else
      // define r[i]
      DefAplGivens(pO[i],h,cs,sn);

    ApplyGivens(pS[i],pO[i+1],cs,sn);
    h = 0.;
    ApplyGivens(h,pS[i+1],cs,sn);

    TMatrixDColumn vc_i  = TMatrixDColumn(v,i);
    TMatrixDColumn vc_i1 = TMatrixDColumn(v,i+1);
    for (Int_t j = 0; j < nCol_v; j++)
      ApplyGivens(vc_i(j),vc_i1(j),cs,sn);
    // define t[i]
    DefAplGivens(pS[i],h,cs,sn);
    ApplyGivens(pO[i+1],pS[i+1],cs,sn);
    if (i < k-1) {
      h = 0.;
      ApplyGivens(h,pO[i+2],cs,sn);
    }

    TMatrixDRow ur_i  = TMatrixDRow(u,i);
    TMatrixDRow ur_i1 = TMatrixDRow(u,i+1);
    for (Int_t j = 0; j < nCol_u; j++)
      ApplyGivens(ur_i(j),ur_i1(j),cs,sn);
  }
}

//______________________________________________________________________________
void TDecompSVD::SortSingular(TMatrixD &v,TMatrixD &u,TVectorD &sDiag)
{
// Perform a permutation transformation on the diagonal matrix S', so that
// matrix S'' = U''^T . S' . V''  has diagonal elements ordered such that they
// do not increase.
//
// Output:
//   v     - (n x n) - matrix H . V' . V'' in the (n x n) part of v
//   u     - (m x m) - matrix U''^T . U'^T . Q^T     
//   sDiag - diagonal of the (m x n) S''

  const Int_t nCol_v = v.GetNcols();
  const Int_t nCol_u = u.GetNcols();

  Double_t *pS = sDiag.GetElements();
  Double_t *pV = v.GetElements();
  Double_t *pU = u.GetElements();

  // order singular values

  if (nCol_v > 1) {
    while (1) {
      Bool_t found = kFALSE;
      Int_t i = 1;
      while (!found && i < nCol_v) {
        if (pS[i] > pS[i-1])
          found = kTRUE;
        else
          i++;
      }
      if (!found) break;
      for (Int_t i = 1; i < nCol_v; i++) {
        Double_t t = pS[i-1];
        Int_t k = i-1;
        for (Int_t j = i; j < nCol_v; j++) {
          if (t < pS[j]) {
            t = pS[j];
            k = j;
          }
        }
        if (k != i-1) {
          // perform permutation on singular values
          pS[k]   = pS[i-1];
          pS[i-1] = t;
          // perform permutation on matrix v
          for (Int_t j = 0; j < nCol_v; j++) {
            const Int_t off_j = j*nCol_v;
            t             = pV[off_j+k];
            pV[off_j+k]   = pV[off_j+i-1];
            pV[off_j+i-1] = t;
          }
          // perform permutation on vector u
          for (Int_t j = 0; j < nCol_u; j++) {
            const Int_t off_k  = k*nCol_u;
            const Int_t off_i1 = (i-1)*nCol_u;
            t            = pU[off_k+j];
            pU[off_k+j]  = pU[off_i1+j];
            pU[off_i1+j] = t;
          }
        }
      }
    }
  }
}

//______________________________________________________________________________
const TMatrixD TDecompSVD::GetMatrix() const
{
  const Int_t nRows = fU.GetNrows();
  const Int_t nCols = fV.GetNcols();
  TMatrixD s(nRows,nCols);
  TMatrixDDiag(s,0) = fSig;
  const TMatrixD vt(TMatrixDBase::kTransposed,fV);
  return fU * s * vt;
}

//______________________________________________________________________________
Bool_t TDecompSVD::Solve(TVectorD &b)
{
// Solve Ax=b assuming the SVD form of A is stored . Solution returned in b.
// Remember that fU.GetNrows() (== fU.GetNcols()) >= fV.GetNrows() (== fV.GetNcols())
// If it is larger than only the first fV.GetNrows() in the returned b are meaningful .

  Assert(b.IsValid());
  Assert(fStatus & kDecomposed);

  if (fU.GetNrows() != b.GetNrows() || fU.GetRowLwb() != b.GetLwb())
  {
    Error("Solve(TVectorD &","vector and matrix incompatible");
    b.Invalidate(); 
    return kFALSE;
  }

  // We start with fU fSig fV^T x = b, and turn it into  fV^T x = fSig^-1 fU^T b
  // Form tmp = fSig^-1 fU^T b but ignore diagonal elements with
  // fSig(i) < fTol * max(fSig)

  const Int_t    lwb       = fU.GetColLwb();
  const Int_t    upb       = lwb+fV.GetNcols()-1;
  const Double_t threshold = fSig(0)*fTol;

  TVectorD tmp(lwb,upb);
  for (Int_t irow = lwb; irow < upb; irow++) {
    Double_t r = 0.0;
    if (fSig(irow-lwb) > threshold) {
      const TVectorD uc_i = TMatrixDColumn(fU,irow);
      r = uc_i * b;
      r /= fSig(irow-lwb);
    }
    tmp(irow) = r;
  }

  if (b.GetNrows() > fV.GetNrows()) {
    const TVectorD tmp2 = fV*tmp;
    b.SetSub(tmp2.GetLwb(),tmp2);
  } else
    b = fV*tmp;

  return kTRUE;
}

//______________________________________________________________________________
Bool_t TDecompSVD::Solve(TMatrixDColumn &cb)
{
  const TMatrixDBase *b = cb.GetMatrix();
  Assert(b->IsValid());
  Assert(fStatus & kDecomposed);

  if (fU.GetNrows() != b->GetNrows() || fU.GetRowLwb() != b->GetRowLwb())
  { 
    Error("Solve(TMatrixDColumn &","vector and matrix incompatible");
    return kFALSE; 
  }     

  // We start with fU fSig fV^T x = b, and turn it into  fV^T x = fSig^-1 fU^T b
  // Form tmp = fSig^-1 fU^T b but ignore diagonal elements in
  // fSig(i) < fTol * max(fSig)

  const Int_t    lwb       = fU.GetColLwb();
  const Int_t    upb       = lwb+fV.GetNcols()-1;
  const Double_t threshold = fSig(0)*fTol;

  TVectorD tmp(lwb,upb);
  const TVectorD vb = cb;
  for (Int_t irow = lwb; irow < upb; irow++) {
    Double_t r = 0.0;
    if (fSig(irow-lwb) > threshold) {
      const TVectorD uc_i = TMatrixDColumn(fU,irow);
      r = uc_i * vb;
      r /= fSig(irow-lwb);
    }
    tmp(irow) = r;
  }

  if (b->GetNrows() > fV.GetNrows()) {
    const TVectorD tmp2 = fV*tmp;
    TVectorD tmp3(cb);
    tmp3.SetSub(tmp2.GetLwb(),tmp2);
    cb = tmp3;
  } else
    cb = fV*tmp;

  return kTRUE;
}

//______________________________________________________________________________
Bool_t TDecompSVD::TransSolve(TVectorD &b)
{
// Solve A^T x=b assuming the SVD form of A is stored . Solution returned in b.

  Assert(b.IsValid());
  Assert(fStatus & kDecomposed);

  if (fU.GetNcols() != fU.GetNrows()) {
    Error("TransSolve(TVectorD &","matrix should be square");
    b.Invalidate();
    return kFALSE;
  } 

  if (fV.GetNrows() != b.GetNrows() || fV.GetRowLwb() != b.GetLwb())
  {   
    Error("TransSolve(TVectorD &","vector and matrix incompatible");
    b.Invalidate();
    return kFALSE;
  } 

  // We start with fV fSig fU^T x = b, and turn it into  fU^T x = fSig^-1 fV^T b
  // Form tmp = fSig^-1 fV^T b but ignore diagonal elements in
  // fSig(i) < fTol * max(fSig)

  const Int_t    nCol_v    = fV.GetNcols();
  const Double_t threshold = fSig(0)*fTol;

  TVectorD tmp(nCol_v);
  for (Int_t i = 0; i < nCol_v; i++) {
    Double_t r = 0.0;
    if (fSig(i) > threshold) {
      const TVectorD vc = TMatrixDColumn(fV,i);
      r = vc * b;
      r /= fSig(i);
    }
    tmp(i) = r;
  }
  b = fU*tmp;

  return kTRUE;
}

//______________________________________________________________________________
Bool_t TDecompSVD::TransSolve(TMatrixDColumn &cb)
{
  const TMatrixDBase *b = cb.GetMatrix();
  Assert(b->IsValid());
  Assert(fStatus & kDecomposed);

  if (fU.GetNcols() != fU.GetNrows()) {
    Error("TransSolve(TMatrixDColumn &","matrix should be square");
    return kFALSE;
  } 

  if (fV.GetNrows() != b->GetNrows() || fV.GetRowLwb() != b->GetRowLwb())
  {   
    Error("TransSolve(TMatrixDColumn &","vector and matrix incompatible");
    return kFALSE;
  } 

  // We start with fV fSig fU^T x = b, and turn it into  fU^T x = fSig^-1 fV^T b
  // Form tmp = fSig^-1 fV^T b but ignore diagonal elements in
  // fSig(i) < fTol * max(fSig)

  const Int_t    nCol_v    = fV.GetNcols();
  const Double_t threshold = fSig(0)*fTol;

  const TVectorD vb = cb;
  TVectorD tmp(nCol_v);
  for (Int_t i = 0; i < nCol_v; i++) {
    Double_t r = 0.0;
    if (fSig(i) > threshold) {
      const TVectorD vc = TMatrixDColumn(fV,i);
      r = vc * vb;
      r /= fSig(i);
    }
    tmp(i) = r;
  }
  cb = fU*tmp;

  return kTRUE;
}

//______________________________________________________________________________
Double_t TDecompSVD::Condition()
{
  if ( !(fStatus & kCondition) ) {
    const Double_t max = fSig(0);
    const Double_t min = fSig(fSig.GetNrows()-1);
    fCondition = (min > 0.0) ? max/min : -1.0;
    fStatus |= kCondition;
  }
  return fCondition;
}

//______________________________________________________________________________
void TDecompSVD::Det(Double_t &d1,Double_t &d2)
{
  if ( !(fStatus & kDetermined) ) {
    if ( fStatus & kSingular ) {
      fDet1 = 0.0;
      fDet2 = 0.0;
    } else {
      Assert(fSig.IsValid());
      DiagProd(fSig,fTol,fDet1,fDet2);
    }
    fStatus |= kDetermined;
  }
  d1 = fDet1;
  d2 = fDet2;
}

//______________________________________________________________________________
TDecompSVD &TDecompSVD::operator=(const TDecompSVD &source)
{
  if (this != &source) {
    TDecompBase::operator=(source);
    fU.ResizeTo(source.fU);
    fU = source.fU;
    fV.ResizeTo(source.fV);
    fV = source.fV;
    fSig.ResizeTo(source.fSig);
    fSig = source.fSig;
  }
  return *this;
}
