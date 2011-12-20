#ifndef ROOT_TGQuartz
#define ROOT_TGQuartz

#ifndef ROOT_TGCocoa
#include "TGCocoa.h"
#endif

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// This is non-GUI part of TVirtualX interface, implemented for         //
// MacOS X, using CoreGraphics (Quartz).                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
class TGQuartz : public TGCocoa {
public:
   TGQuartz();
   TGQuartz(const char *name, const char *title);
   
   //Final-overriders for TVirtualX.
   virtual void      DrawBox(Int_t x1, Int_t y1, Int_t x2, Int_t y2, EBoxMode mode);
   virtual void      DrawCellArray(Int_t x1, Int_t y1, Int_t x2, Int_t y2,
                                   Int_t nx, Int_t ny, Int_t *ic);
   virtual void      DrawFillArea(Int_t n, TPoint *xy);
   
   using TGCocoa::DrawLine;
   
   virtual void      DrawLine(Int_t x1, Int_t y1, Int_t x2, Int_t y2);
   virtual void      DrawPolyLine(Int_t n, TPoint *xy);
   virtual void      DrawPolyMarker(Int_t n, TPoint *xy);
   virtual void      DrawText(Int_t x, Int_t y, Float_t angle, Float_t mgn, const char *text,
                              ETextMode mode);
   
   virtual void      SetFillColor(Color_t cindex);
   virtual void      SetFillStyle(Style_t style);
   virtual void      SetLineColor(Color_t cindex);
   virtual void      SetLineType(Int_t n, Int_t *dash);
   virtual void      SetLineStyle(Style_t linestyle);
   virtual void      SetLineWidth(Width_t width);
   virtual void      SetMarkerColor(Color_t cindex);
   virtual void      SetMarkerSize(Float_t markersize);
   virtual void      SetMarkerStyle(Style_t markerstyle);
   virtual void      SetOpacity(Int_t percent);
   virtual void      SetTextAlign(Short_t talign=11);
   virtual void      SetTextColor(Color_t cindex);
   virtual void      SetTextFont(Font_t fontnumber);
   virtual Int_t     SetTextFont(char *fontname, ETextSetMode mode);
   virtual void      SetTextSize(Float_t textsize);
   
           void      SetFillColorIndex(Int_t ci);
           void      SetStrokeColorIndex(Int_t ci);
           void      SetStencilPattern();

private:
   TGQuartz(const TGQuartz &rhs);
   TGQuartz &operator = (const TGQuartz &rhs);
   
   Int_t fStencilNb;
   
   ClassDef(TGQuartz, 0);//2D graphics for Mac OSX.
};

#endif
