// @(#)root/graf:$Name:  $:$Id: TGraphErrors.cxx,v 1.32 2003/04/10 20:12:22 brun Exp $
// Author: Rene Brun   15/09/96

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include <string.h>

#include "Riostream.h"
#include "TROOT.h"
#include "TGraphErrors.h"
#include "TStyle.h"
#include "TMath.h"
#include "TArrow.h"
#include "TVirtualPad.h"
#include "TF1.h"
#include "TStyle.h"

ClassImp(TGraphErrors)

//______________________________________________________________________________
//
//   A TGraphErrors is a TGraph with error bars.
//   The various format options to draw a TGraphErrors are explained in
//     TGraphErrors::Paint.
//
//  The picture below has been generated by the following macro:
//------------------------------------------------------------------------
//{
//   gROOT->Reset();
//   c1 = new TCanvas("c1","A Simple Graph with error bars",200,10,700,500);
//
//   c1->SetFillColor(42);
//   c1->SetGrid();
//   c1->GetFrame()->SetFillColor(21);
//   c1->GetFrame()->SetBorderSize(12);
//
//   Int_t n = 10;
//   Double_t x[n]  = {-0.22, 0.05, 0.25, 0.35, 0.5, 0.61,0.7,0.85,0.89,0.95};
//   Double_t y[n]  = {1,2.9,5.6,7.4,9,9.6,8.7,6.3,4.5,1};
//   Double_t ex[n] = {.05,.1,.07,.07,.04,.05,.06,.07,.08,.05};
//   Double_t ey[n] = {.8,.7,.6,.5,.4,.4,.5,.6,.7,.8};
//   gr = new TGraphErrors(n,x,y,ex,ey);
//   gr->SetTitle("TGraphErrors Example");
//   gr->SetMarkerColor(4);
//   gr->SetMarkerStyle(21);
//   gr->Draw("ALP");
//
//   c1->Update();
//}
//Begin_Html
/*
<img src="gif/gerrors.gif">
*/
//End_Html
//

//______________________________________________________________________________
TGraphErrors::TGraphErrors(): TGraph()
{
//*-*-*-*-*-*-*-*-*-*-*TGraphErrors default constructor*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  ================================
   fEX       = 0;
   fEY       = 0;
}


//______________________________________________________________________________
TGraphErrors::TGraphErrors(Int_t n)
             : TGraph(n)
{
//*-*-*-*-*-*-*-*-*-*-*TGraphErrors normal constructor*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  ===============================
//
//  the arrays are preset to zero

   if (n <= 0) {
      Error("TGraphErrors", "illegal number of points (%d)", n);
      return;
   }

   fEX       = new Double_t[n];
   fEY       = new Double_t[n];

   for (Int_t i=0;i<n;i++) {
      fEX[i] = 0;
      fEY[i] = 0;
   }
}


//______________________________________________________________________________
TGraphErrors::TGraphErrors(Int_t n, const Float_t *x, const Float_t *y, const Float_t *ex, const Float_t *ey)
       : TGraph(n,x,y)
{
//*-*-*-*-*-*-*-*-*-*-*TGraphErrors normal constructor*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  ===============================
//
//  if ex or ey are null, the corresponding arrays are preset to zero

   if (n <= 0) {
      Error("TGraphErrors", "illegal number of points (%d)", n);
      return;
   }

   fEX       = new Double_t[n];
   fEY       = new Double_t[n];

   for (Int_t i=0;i<n;i++) {
      if (ex) fEX[i] = ex[i];
      else    fEX[i] = 0;
      if (ey) fEY[i] = ey[i];
      else    fEY[i] = 0;
   }
}


//______________________________________________________________________________
TGraphErrors::TGraphErrors(Int_t n, const Double_t *x, const Double_t *y, const Double_t *ex, const Double_t *ey)
       : TGraph(n,x,y)
{
//*-*-*-*-*-*-*-*-*-*-*TGraphErrors normal constructor*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  ===============================
//
//  if ex or ey are null, the corresponding arrays are preset to zero

   if (n <= 0) {
      Error("TGraphErrors", "illegal number of points (%d)", n);
      return;
   }

   fEX       = new Double_t[n];
   fEY       = new Double_t[n];

   for (Int_t i=0;i<n;i++) {
      if (ex) fEX[i] = ex[i];
      else    fEX[i] = 0;
      if (ey) fEY[i] = ey[i];
      else    fEY[i] = 0;
   }
}

//______________________________________________________________________________
TGraphErrors::TGraphErrors(const TH1 *h)
       : TGraph(h)
{
// TGraphErrors constructor importing its parameters from the TH1 object passed as argument

   if (fNpoints <= 0) return;
   fEX       = new Double_t[fNpoints];
   fEY       = new Double_t[fNpoints];

   for (Int_t i=0;i<fNpoints;i++) {
      fEX[i] = h->GetBinWidth(i+1)*gStyle->GetErrorX();
      fEY[i] = h->GetBinError(i+1);
   }
}   
//______________________________________________________________________________
TGraphErrors::~TGraphErrors()
{
//*-*-*-*-*-*-*-*-*-*-*TGraphErrors default destructor*-*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  ===============================

   delete [] fEX;
   delete [] fEY;
}

//______________________________________________________________________________
void TGraphErrors::Apply(TF1 *f)
{
  // apply function to all the data points
  // y = f(x,y)
  //
  // The error is calculated as ey=(f(x,y+ey)-f(x,y-ey))/2
  // This is the same as error(fy) = df/dy * ey for small errors
  //
  // For generic functions the symmetric errors might become non-symmetric
  // and are averaged here. Use TGraphAsymmErrors if desired.
  //
  // error on x doesn't change
  // function suggested/implemented by Miroslav Helbich <helbich@mail.desy.de>

  Double_t x,y,ex,ey;

  for (Int_t i=0;i<GetN();i++) {
     GetPoint(i,x,y);
     ex=GetErrorX(i);
     ey=GetErrorY(i);

     SetPoint(i,x,f->Eval(x,y));
     SetPointError(i,ex,TMath::Abs(f->Eval(x,y+ey) - f->Eval(x,y-ey))/2.);
  }
}

//______________________________________________________________________________
void TGraphErrors::ComputeRange(Double_t &xmin, Double_t &ymin, Double_t &xmax, Double_t &ymax) const
{
  for (Int_t i=0;i<fNpoints;i++) {
     if (fX[i] -fEX[i] < xmin) {
        if (gPad && gPad->GetLogx()) {
           if (fEX[i] < fX[i]) xmin = fX[i]-fEX[i];
           else                xmin = fX[i]/3;
        } else {
          xmin = fX[i]-fEX[i];
        }
     }
     if (fX[i] +fEX[i] > xmax) xmax = fX[i]+fEX[i];
     if (fY[i] -fEY[i] < ymin) {
        if (gPad && gPad->GetLogy()) {
           if (fEY[i] < fY[i]) ymin = fY[i]-fEY[i];
           else                ymin = fY[i]/3;
        } else {
          ymin = fY[i]-fEY[i];
        }
     }
     if (fY[i] +fEY[i] > ymax) ymax = fY[i]+fEY[i];
  }
}

//______________________________________________________________________________
Double_t TGraphErrors::GetErrorX(Int_t i) const
{
//    This function is called by GraphFitChisquare.
//    It returns the error along X at point i.

   if (i < 0 || i >= fNpoints) return -1;
   if (fEX) return fEX[i];
   return -1;
}

//______________________________________________________________________________
Double_t TGraphErrors::GetErrorY(Int_t i) const
{
//    This function is called by GraphFitChisquare.
//    It returns the error along Y at point i.

   if (i < 0 || i >= fNpoints) return -1;
   if (fEY) return fEY[i];
   return -1;
}

//______________________________________________________________________________
Int_t TGraphErrors::InsertPoint()
{
// Insert a new point at the mouse position

   Int_t ipoint = TGraph::InsertPoint();

   Double_t *newEX = new Double_t[fNpoints];
   Double_t *newEY = new Double_t[fNpoints];
   Int_t i;
   for (i=0;i<ipoint;i++) {
      newEX[i] = fEX[i];
      newEY[i] = fEY[i];
   }
   newEX[ipoint] = 0;
   newEY[ipoint] = 0;
   for (i=ipoint+1;i<fNpoints;i++) {
      newEX[i] = fEX[i-1];
      newEY[i] = fEY[i-1];
   }
   delete [] fEX;
   delete [] fEY;
   fEX = newEX;
   fEY = newEY;
   return ipoint;
}

//______________________________________________________________________________
void TGraphErrors::Paint(Option_t *option)
{
   // Paint this TGraphErrors with its current attributes
   //
   // by default horizonthal and vertical small lines are drawn at
   // the end of the error bars. if option "z" or "Z" is specified,
   // these lines are not drawn.
   //
   // if option contains ">" an arrow is drawn at the end of the error bars
   // if option contains "|>" a full arrow is drawn at the end of the error bars
   // the size of the arrow is set to 2/3 of the marker size.
   //
   // By default, error bars are drawn. If option "X" is specified,
   // the errors are not drawn (TGraph::Paint equivalent).
   //
   // if option "[]" is specified only the end vertical/horizonthal lines
   // of the error bars are drawn. This option is interesting to superimpose
   // systematic errors on top of a graph with statistical errors.
   //
   // Use gStyle->SetErrorX(dx) to control the size of the error along x.
   // set dx = 0 to suppress the error along x.
   //
   // Use gStyle->SetEndErrorSize(np) to control the size of the lines
   // at the end of the error bars (when option 1 is used).
   // By default np=1. (np represents the number of pixels).

   const Int_t BASEMARKER=8;
   Double_t s2x, s2y, symbolsize, sbase;
   Double_t x, y, ex, ey, xl1, xl2, xr1, xr2, yup1, yup2, ylow1, ylow2, tx, ty;
   static Float_t cxx[11] = {1,1,0.6,0.6,1,1,0.6,0.5,1,0.6,0.6};
   static Float_t cyy[11] = {1,1,1,1,1,1,1,1,1,0.5,0.6};

   if (strchr(option,'X') || strchr(option,'x')) {TGraph::Paint(option); return;}
   Bool_t brackets = kFALSE;
   if (strstr(option,"[]")) brackets = kTRUE;
   Bool_t endLines = kTRUE;
   if (strchr(option,'z')) endLines = kFALSE;
   if (strchr(option,'Z')) endLines = kFALSE;
   const char *arrowOpt = 0;
   if (strchr(option,'>'))  arrowOpt = ">";
   if (strstr(option,"|>")) arrowOpt = "|>";

   Bool_t axis = kFALSE;
   if (strchr(option,'a')) axis = kTRUE;
   if (strchr(option,'A')) axis = kTRUE;
   if (axis) TGraph::Paint(option);

   TAttLine::Modify();

   TArrow arrow;
   arrow.SetLineWidth(GetLineWidth());
   arrow.SetLineColor(GetLineColor());
   arrow.SetFillColor(GetFillColor());

   symbolsize  = GetMarkerSize();
   sbase       = symbolsize*BASEMARKER;
   Int_t mark  = GetMarkerStyle();
   Double_t cx  = 0;
   Double_t cy  = 0;
   if (mark >= 20 && mark < 31) {
      cx = cxx[mark-20];
      cy = cyy[mark-20];
   }

//*-*-      define the offset of the error bars due to the symbol size
   s2x  = gPad->PixeltoX(Int_t(0.5*sbase)) - gPad->PixeltoX(0);
   s2y  =-gPad->PixeltoY(Int_t(0.5*sbase)) + gPad->PixeltoY(0);
   Int_t dxend = Int_t(gStyle->GetEndErrorSize());
   tx    = gPad->PixeltoX(dxend) - gPad->PixeltoX(0);
   ty    =-gPad->PixeltoY(dxend) + gPad->PixeltoY(0);
   Float_t asize = 0.6*symbolsize*BASEMARKER/gPad->GetWh();

   gPad->SetBit(kClipFrame, TestBit(kClipFrame));
   for (Int_t i=0;i<fNpoints;i++) {
      x  = gPad->XtoPad(fX[i]);
      y  = gPad->YtoPad(fY[i]);
      if (x < gPad->GetUxmin()) continue;
      if (x > gPad->GetUxmax()) continue;
      if (y < gPad->GetUymin()) continue;
      if (y > gPad->GetUymax()) continue;
      ex = fEX[i];
      ey = fEY[i];
      xl1 = x - s2x*cx;
      xl2 = gPad->XtoPad(fX[i] - ex);
      if (xl1 > xl2) {
         if (arrowOpt) {
            arrow.PaintArrow(xl1,y,xl2,y,asize,arrowOpt);
         } else {
            if (!brackets) gPad->PaintLine(xl1,y,xl2,y);
            if (endLines)  gPad->PaintLine(xl2,y-ty,xl2,y+ty);
         }
      }
      xr1 = x + s2x*cx;
      xr2 = gPad->XtoPad(fX[i] + ex);
      if (xr1 < xr2) {
         if (arrowOpt) {
            arrow.PaintArrow(xr1,y,xr2,y,asize,arrowOpt);
         } else {
            if (!brackets) gPad->PaintLine(xr1,y,xr2,y);
            if (endLines)  gPad->PaintLine(xr2,y-ty,xr2,y+ty);
         }
      }
      yup1 = y + s2y*cy;
      yup2 = gPad->YtoPad(fY[i] + ey);
      if (yup2 > gPad->GetUymax()) yup2 =  gPad->GetUymax();
      if (yup2 > yup1) {
         if (arrowOpt) {
            arrow.PaintArrow(x,yup1,x,yup2,asize,arrowOpt);
         } else {
            if (!brackets) gPad->PaintLine(x,yup1,x,yup2);
            if (endLines)  gPad->PaintLine(x-tx,yup2,x+tx,yup2);
         }
      }
      ylow1 = y - s2y*cy;
      ylow2 = gPad->YtoPad(fY[i] - ey);
      if (ylow2 < gPad->GetUymin()) ylow2 =  gPad->GetUymin();
      if (ylow2 < ylow1) {
         if (arrowOpt) {
            arrow.PaintArrow(x,ylow1,x,ylow2,asize,arrowOpt);
         } else {
            if (!brackets) gPad->PaintLine(x,ylow1,x,ylow2);
            if (endLines)  gPad->PaintLine(x-tx,ylow2,x+tx,ylow2);
         }
      }
   }
   if (!brackets && !axis) TGraph::Paint(option);
   gPad->ResetBit(kClipFrame);
}


//______________________________________________________________________________
void TGraphErrors::Print(Option_t *) const
{
//*-*-*-*-*-*-*-*-*-*-*Print graph and errors values*-*-*-*-*-*-*-*-*-*-*-*
//*-*                  =============================
//

   for (Int_t i=0;i<fNpoints;i++) {
      printf("x[%d]=%g, y[%d]=%g, ex[%d]=%g, ey[%d]=%g\n",i,fX[i],i,fY[i],i,fEX[i],i,fEY[i]);
   }
}

//______________________________________________________________________________
Int_t TGraphErrors::RemovePoint()
{
// Delete point close to the mouse position

   Int_t ipoint = TGraph::RemovePoint();
   if (ipoint < 0) return ipoint;

   Double_t *newEX = new Double_t[fNpoints];
   Double_t *newEY = new Double_t[fNpoints];
   Int_t i, j = -1;
   for (i=0;i<fNpoints+1;i++) {
      if (i == ipoint) continue;
      j++;
      newEX[j] = fEX[i];
      newEY[j] = fEY[i];
   }
   delete [] fEX;
   delete [] fEY;
   fEX = newEX;
   fEY = newEY;
   return ipoint;
}

//______________________________________________________________________________
Int_t TGraphErrors::RemovePoint(Int_t ipnt)
{
// Delete point number ipnt

   Int_t ipoint = TGraph::RemovePoint(ipnt);
   if (ipoint < 0) return ipoint;

   Double_t *newEX = new Double_t[fNpoints];
   Double_t *newEY = new Double_t[fNpoints];
   Int_t i, j = -1;
   for (i=0;i<fNpoints+1;i++) {
      if (i == ipoint) continue;
      j++;
      newEX[j] = fEX[i];
      newEY[j] = fEY[i];
   }
   delete [] fEX;
   delete [] fEY;
   fEX = newEX;
   fEY = newEY;
   return ipoint;
}

//______________________________________________________________________________
void TGraphErrors::SavePrimitive(ofstream &out, Option_t *option)
{
    // Save primitive as a C++ statement(s) on output stream out

   char quote = '"';
   out<<"   "<<endl;
   if (gROOT->ClassSaved(TGraphErrors::Class())) {
       out<<"   ";
   } else {
       out<<"   TGraphErrors *";
   }
   out<<"gre = new TGraphErrors("<<fNpoints<<");"<<endl;
   out<<"   gre->SetName("<<quote<<GetName()<<quote<<");"<<endl;
   out<<"   gre->SetTitle("<<quote<<GetTitle()<<quote<<");"<<endl;

   SaveFillAttributes(out,"gre",0,1001);
   SaveLineAttributes(out,"gre",1,1,1);
   SaveMarkerAttributes(out,"gre",1,1,1);

   for (Int_t i=0;i<fNpoints;i++) {
      out<<"   gre->SetPoint("<<i<<","<<fX[i]<<","<<fY[i]<<");"<<endl;
      out<<"   gre->SetPointError("<<i<<","<<fEX[i]<<","<<fEY[i]<<");"<<endl;
   }
   
   static Int_t frameNumber = 0;
   if (fHistogram) {
      frameNumber++;
      TString hname = fHistogram->GetName();
      hname += frameNumber;
      fHistogram->SetName(hname.Data());
      fHistogram->SavePrimitive(out,"nodraw");
      out<<"   gre->SetHistogram("<<fHistogram->GetName()<<");"<<endl;
      out<<"   "<<endl;
   }

   // save list of functions
   TIter next(fFunctions);
   TObject *obj;
   while ((obj=next())) {
      obj->SavePrimitive(out,"nodraw");
      out<<"   gre->GetListOfFunctions()->Add("<<obj->GetName()<<");"<<endl;
      if (obj->InheritsFrom("TPaveStats")) {
         out<<"   ptstats->SetParent(gre->GetListOfFunctions());"<<endl;
      }
   }

   if (strstr(option,"multigraph")) {
      out<<"   multigraph->Add(gre);"<<endl;
      return;
   }
   out<<"   gre->Draw("
      <<quote<<option<<quote<<");"<<endl;
}

//______________________________________________________________________________
void TGraphErrors::Set(Int_t n)
{
// Set number of points in the graph
// Existing coordinates are preserved
// New coordinates and errors above fNpoints are preset to 0.

   if (n < 0) n = 0;
   if (n == fNpoints) return;
   Double_t *x=0, *y=0, *ex=0, *ey=0;
   if (n > 0) {
      x  = new Double_t[n];
      y  = new Double_t[n];
      ex = new Double_t[n];
      ey = new Double_t[n];
   }
   Int_t i;
   for (i=0; i<fNpoints && i<n;i++) {
      if (fX)   x[i] = fX[i];
      if (fY)   y[i] = fY[i];
      if (fEX) ex[i] = fEX[i];
      if (fEY) ey[i] = fEY[i];
   }
   for (i=fNpoints; i<n;i++) {
      x[i]  = 0;
      y[i]  = 0;
      ex[i] = 0;
      ey[i] = 0;
   }
   delete [] fX;
   delete [] fY;
   delete [] fEX;
   delete [] fEY;
   fNpoints =n;
   fX  = x;
   fY  = y;
   fEX = ex;
   fEY = ey;
}

//______________________________________________________________________________
void TGraphErrors::SetPoint(Int_t i, Double_t x, Double_t y)
{
//*-*-*-*-*-*-*-*-*-*-*Set x and y values for point number i*-*-*-*-*-*-*-*-*
//*-*                  =====================================

   if (i < 0) return;
   if (i >= fNpoints) {
   // re-allocate the object
      Double_t *savex  = new Double_t[i+1];
      Double_t *savey  = new Double_t[i+1];
      Double_t *saveex = new Double_t[i+1];
      Double_t *saveey = new Double_t[i+1];
      if (fNpoints > 0) {
         memcpy(savex, fX, fNpoints*sizeof(Double_t));
         memcpy(savey, fY, fNpoints*sizeof(Double_t));
         memcpy(saveex,fEX,fNpoints*sizeof(Double_t));
         memcpy(saveey,fEY,fNpoints*sizeof(Double_t));
      }
      if (fX)  delete [] fX;
      if (fY)  delete [] fY;
      if (fEX) delete [] fEX;
      if (fEY) delete [] fEY;
      fX  = savex;
      fY  = savey;
      fEX = saveex;
      fEY = saveey;
      fNpoints = i+1;
   }
   fX[i] = x;
   fY[i] = y;
   if (fHistogram) {
      delete fHistogram;
      fHistogram = 0;
   }
}

//______________________________________________________________________________
void TGraphErrors::SetPointError(Double_t ex, Double_t ey)
{
//*-*-*-*-*-*-*Set ex and ey values for point pointed by the mouse*-*-*-*
//*-*          ===================================================

   Int_t px = gPad->GetEventX();
   Int_t py = gPad->GetEventY();

   //localize point to be deleted
   Int_t ipoint = -2;
   Int_t i;
   // start with a small window (in case the mouse is very close to one point)
   for (i=0;i<fNpoints;i++) {
      Int_t dpx = px - gPad->XtoAbsPixel(gPad->XtoPad(fX[i]));
      Int_t dpy = py - gPad->YtoAbsPixel(gPad->YtoPad(fY[i]));
      if (dpx*dpx+dpy*dpy < 25) {ipoint = i; break;}
   }
   if (ipoint == -2) return;

   fEX[ipoint] = ex;
   fEY[ipoint] = ey;
   gPad->Modified();
}

//______________________________________________________________________________
void TGraphErrors::SetPointError(Int_t i, Double_t ex, Double_t ey)
{
//*-*-*-*-*-*-*-*-*-*-*Set ex and ey values for point number i*-*-*-*-*-*-*-*
//*-*                  =======================================

   if (i < 0) return;
   if (i >= fNpoints) {
   // re-allocate the object
      TGraphErrors::SetPoint(i,0,0);
   }
   fEX[i] = ex;
   fEY[i] = ey;
}

//______________________________________________________________________________
void TGraphErrors::Streamer(TBuffer &b)
{
   // Stream an object of class TGraphErrors.

   if (b.IsReading()) {
      UInt_t R__s, R__c;
      Version_t R__v = b.ReadVersion(&R__s, &R__c);
      if (R__v > 2) {
         TGraphErrors::Class()->ReadBuffer(b, this, R__v, R__s, R__c);
         return;
      }
      //====process old versions before automatic schema evolution
      TGraph::Streamer(b);
      fEX = new Double_t[fNpoints];
      fEY = new Double_t[fNpoints];
      if (R__v < 2) {
         Float_t *ex = new Float_t[fNpoints];
         Float_t *ey = new Float_t[fNpoints];
         b.ReadFastArray(ex,fNpoints);
         b.ReadFastArray(ey,fNpoints);
         for (Int_t i=0;i<fNpoints;i++) {
            fEX[i] = ex[i];
            fEY[i] = ey[i];
         }
         delete [] ey;
         delete [] ex;
      } else {
         b.ReadFastArray(fEX,fNpoints);
         b.ReadFastArray(fEY,fNpoints);
      }
      b.CheckByteCount(R__s, R__c, TGraphErrors::IsA());
      //====end of old versions

   } else {
      TGraphErrors::Class()->WriteBuffer(b,this);
   }
}

//______________________________________________________________________________
void TGraphErrors::SwapPoints(Int_t pos1, Int_t pos2) {
   SwapValues(fEX, pos1, pos2);
   SwapValues(fEY, pos1, pos2);
   TGraph::SwapPoints(pos1, pos2);
}
