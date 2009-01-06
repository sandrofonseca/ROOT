#include <typeinfo>

#include "TVirtualGL.h"
#include "KeySymbols.h"
#include "TVirtualX.h"
#include "Buttons.h"
#include "TString.h"
#include "TROOT.h"
#include "TColor.h"
#include "TMath.h"
#include "TH3.h"
#include "TF3.h"

#include "TGLMarchingCubes.h"
#include "TGLOrthoCamera.h"
#include "TGLTF3Painter.h"
#include "TGLIncludes.h"

namespace {

/*
Auxilary functions to draw iso-meshes.
*/
//______________________________________________________________________________
template<class V>
void DrawMesh(GLenum type, const std::vector<V> &vs, const std::vector<V> &ns, 
              const std::vector<UInt_t> &ts)
{
   //Surface with material and lighting.
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_NORMAL_ARRAY);
   glVertexPointer(3, type, 0, &vs[0]);
   glNormalPointer(type, 0, &ns[0]);
   glDrawElements(GL_TRIANGLES, ts.size(), GL_UNSIGNED_INT, &ts[0]);
   glDisableClientState(GL_NORMAL_ARRAY);
   glDisableClientState(GL_VERTEX_ARRAY);
}

//______________________________________________________________________________
template<class V>
void DrawMesh(GLenum type, const std::vector<V> &vs, const std::vector<UInt_t> &ts)
{
   //Only vertices, no normal (no lighting and material).
   glEnableClientState(GL_VERTEX_ARRAY);
   glVertexPointer(3, type, 0, &vs[0]);
   glDrawElements(GL_TRIANGLES, ts.size(), GL_UNSIGNED_INT, &ts[0]);
   glDisableClientState(GL_VERTEX_ARRAY);
}

//______________________________________________________________________________
template<class V, class GLN, class GLV>
void DrawMesh(GLN normal3, GLV vertex3, const std::vector<V> &vs, 
              const std::vector<V> &ns, const std::vector<UInt_t> &ts, 
              const TGLBoxCut &box)
{
   //Mesh with cut.
   //Material and lighting are enabled.
   glBegin(GL_TRIANGLES);

   for (UInt_t i = 0, e = ts.size() / 3; i < e; ++i) {
      const UInt_t * t = &ts[i * 3];
      if (box.IsInCut(&vs[t[0] * 3]))
         continue;
      if (box.IsInCut(&vs[t[1] * 3]))
         continue;
      if (box.IsInCut(&vs[t[2] * 3]))
         continue;

      normal3(&ns[t[0] * 3]);
      vertex3(&vs[t[0] * 3]);
      
      normal3(&ns[t[1] * 3]);
      vertex3(&vs[t[1] * 3]);
      
      normal3(&ns[t[2] * 3]);
      vertex3(&vs[t[2] * 3]);
   }

   glEnd();
}

//______________________________________________________________________________
template<class V, class GLV>
void DrawMesh(GLV vertex3, const std::vector<V> &vs, const std::vector<UInt_t> &ts, 
              const TGLBoxCut &box)
{
   //Mesh with cut.
   //No material and lighting.
   glBegin(GL_TRIANGLES);

   for (UInt_t i = 0, e = ts.size() / 3; i < e; ++i) {
      const UInt_t * t = &ts[i * 3];
      if (box.IsInCut(&vs[t[0] * 3]))
         continue;
      if (box.IsInCut(&vs[t[1] * 3]))
         continue;
      if (box.IsInCut(&vs[t[2] * 3]))
         continue;

      vertex3(&vs[t[0] * 3]);
      vertex3(&vs[t[1] * 3]);
      vertex3(&vs[t[2] * 3]);
   }

   glEnd();
}

//______________________________________________________________________________
void GetColor(Double_t *rfColor, const Double_t *n)
{
   //GetColor generates a color from a given normal
   const Double_t x = n[0];
   const Double_t y = n[1];
   const Double_t z = n[2];
   rfColor[0] = (x > 0. ? x : 0.) + (y < 0. ? -0.5 * y : 0.) + (z < 0. ? -0.5 * z : 0.);
   rfColor[1] = (y > 0. ? y : 0.) + (z < 0. ? -0.5 * z : 0.) + (x < 0. ? -0.5 * x : 0.);
   rfColor[2] = (z > 0. ? z : 0.) + (x < 0. ? -0.5 * x : 0.) + (y < 0. ? -0.5 * y : 0.);
}

//______________________________________________________________________________
void DrawMapleMesh(const std::vector<Double_t> &vs, const std::vector<Double_t> &ns,
                   const std::vector<UInt_t> &ts)
{
   //Colored mesh with lighting disabled.
   Double_t color[] = {0., 0., 0., 0.15};

   glBegin(GL_TRIANGLES);

   for (UInt_t i = 0, e = ts.size() / 3; i < e; ++i) {
      const UInt_t *t = &ts[i * 3];
      const Double_t * n = &ns[t[0] * 3];
      //
      GetColor(color, n);
      glColor4dv(color);
      glVertex3dv(&vs[t[0] * 3]);
      //
      n = &ns[t[1] * 3];
      GetColor(color, n);
      glColor4dv(color);
      glVertex3dv(&vs[t[1] * 3]);
      //
      n = &ns[t[2] * 3];
      GetColor(color, n);
      glColor4dv(color);
      glVertex3dv(&vs[t[2] * 3]);
   }

   glEnd();
}

void DrawMapleMesh(const std::vector<Double_t> &vs, const std::vector<Double_t> &ns,
                   const std::vector<UInt_t> &ts, const TGLBoxCut & box)
{
   //Colored mesh with cut and disabled lighting.
   Double_t color[] = {0., 0., 0., 0.15};

   glBegin(GL_TRIANGLES);

   for (UInt_t i = 0, e = ts.size() / 3; i < e; ++i) {
      const UInt_t *t = &ts[i * 3];
      if (box.IsInCut(&vs[t[0] * 3]))
         continue;
      if (box.IsInCut(&vs[t[1] * 3]))
         continue;
      if (box.IsInCut(&vs[t[2] * 3]))
         continue;
      const Double_t * n = &ns[t[0] * 3];
      //
      GetColor(color, n);
      glColor4dv(color);
      glVertex3dv(&vs[t[0] * 3]);
      //
      n = &ns[t[1] * 3];
      GetColor(color, n);
      glColor4dv(color);
      glVertex3dv(&vs[t[1] * 3]);
      //
      n = &ns[t[2] * 3];
      GetColor(color, n);
      glColor4dv(color);
      glVertex3dv(&vs[t[2] * 3]);
   }

   glEnd();
}

}//Unnamed namespace.

//______________________________________________________________________________
//
// Plot-painter for TF3 functions.

ClassImp(TGLTF3Painter)

//______________________________________________________________________________
TGLTF3Painter::TGLTF3Painter(TF3 *fun, TH1 *hist, TGLOrthoCamera *camera,
                             TGLPlotCoordinates *coord, TGLPaintDevice *dev) :
   TGLPlotPainter(hist, camera, coord, dev, kFALSE, kFALSE, kFALSE),
   fStyle(kDefault),
   fF3(fun),
   fXOZSlice("XOZ", (TH3 *)hist, fun, coord, &fBackBox, TGLTH3Slice::kXOZ),
   fYOZSlice("YOZ", (TH3 *)hist, fun, coord, &fBackBox, TGLTH3Slice::kYOZ),
   fXOYSlice("XOY", (TH3 *)hist, fun, coord, &fBackBox, TGLTH3Slice::kXOY)
{
   // Constructor.
}

//______________________________________________________________________________
char *TGLTF3Painter::GetPlotInfo(Int_t /*px*/, Int_t /*py*/)
{
   //Coords for point on surface under cursor.
   static char mess[] = { "fun3" };
   return mess;
}

//______________________________________________________________________________
Bool_t TGLTF3Painter::InitGeometry()
{
   //Create mesh.
   fCoord->SetCoordType(kGLCartesian);

   if (!fCoord->SetRanges(fHist, kFALSE, kTRUE))
      return kFALSE;

   fBackBox.SetPlotBox(fCoord->GetXRangeScaled(), fCoord->GetYRangeScaled(), fCoord->GetZRangeScaled());
   if (fCamera) fCamera->SetViewVolume(fBackBox.Get3DBox());

   //Build mesh for TF3 surface
   fMesh.ClearMesh();

   Rgl::Mc::TMeshBuilder<TF3, Double_t> builder(kFALSE);//no averaged normals.
   //Set grid parameters.
   Rgl::Mc::TGridGeometry<Double_t> geom;
   geom.fMinX  = fXAxis->GetBinLowEdge(fXAxis->GetFirst());
   geom.fStepX = (fXAxis->GetBinUpEdge(fXAxis->GetLast()) - geom.fMinX) / (fHist->GetNbinsX());
   geom.fMinY  = fYAxis->GetBinLowEdge(fYAxis->GetFirst());
   geom.fStepY = (fYAxis->GetBinUpEdge(fYAxis->GetLast()) - geom.fMinY) / (fHist->GetNbinsY());
   geom.fMinZ  = fZAxis->GetBinLowEdge(fZAxis->GetFirst());
   geom.fStepZ = (fZAxis->GetBinUpEdge(fZAxis->GetLast()) - geom.fMinZ) / (fHist->GetNbinsZ());
   //Scale grid parameters.
   geom.fMinX *= fCoord->GetXScale(), geom.fStepX *= fCoord->GetXScale();
   geom.fMinY *= fCoord->GetYScale(), geom.fStepY *= fCoord->GetYScale();
   geom.fMinZ *= fCoord->GetZScale(), geom.fStepZ *= fCoord->GetZScale();

   builder.BuildMesh(fF3, geom, &fMesh, 0.2);

   if (fCoord->Modified()) {
      fUpdateSelection = kTRUE;
      const TGLVertex3 &vertex = fBackBox.Get3DBox()[0];
      fXOZSectionPos = vertex.Y();
      fYOZSectionPos = vertex.X();
      fXOYSectionPos = vertex.Z();
      fCoord->ResetModified();
   }

   return kTRUE;
}

//______________________________________________________________________________
void TGLTF3Painter::StartPan(Int_t px, Int_t py)
{
   //User clicks right mouse button (in a pad).
   fMousePosition.fX = px;
   fMousePosition.fY = fCamera->GetHeight() - py;
   fCamera->StartPan(px, py);
   fBoxCut.StartMovement(px, fCamera->GetHeight() - py);
}

//______________________________________________________________________________
void TGLTF3Painter::Pan(Int_t px, Int_t py)
{
   //User's moving mouse cursor, with middle mouse button pressed (for pad).
   //Calculate 3d shift related to 2d mouse movement.
   //Slicing is disabled (since somebody has broken it).
   if (!MakeGLContextCurrent())
      return;

   if (fSelectedPart >= fSelectionBase)//Pan camera.
      fCamera->Pan(px, py);
   else if (fSelectedPart > 0) {
      //Convert py into bottom-top orientation.
      //Possibly, move box here
      py = fCamera->GetHeight() - py;
      if (!fHighColor) {
         if (fBoxCut.IsActive() && (fSelectedPart >= kXAxis && fSelectedPart <= kZAxis))
            fBoxCut.MoveBox(px, py, fSelectedPart);
         else;
            //MoveSection(px, py);
      } else {
         //MoveSection(px, py);
      }
   }

   fMousePosition.fX = px, fMousePosition.fY = py;
   fUpdateSelection = kTRUE;
}

//______________________________________________________________________________
void TGLTF3Painter::AddOption(const TString &/*option*/)
{
   //No options for tf3
}

//______________________________________________________________________________
void TGLTF3Painter::ProcessEvent(Int_t event, Int_t /*px*/, Int_t py)
{
   //Change color sheme.
   if (event == kKeyPress) {
      if (py == kKey_s || py == kKey_S) {
         fStyle < kMaple2 ? fStyle = ETF3Style(fStyle + 1) : fStyle = kDefault;
      } else if (py == kKey_c || py == kKey_C) {
         if (fHighColor)
            Info("ProcessEvent", "Cut box does not work in high color, please, switch to true color");
         else {
            fBoxCut.TurnOnOff();
            fUpdateSelection = kTRUE;
         }
      }
   } else if (event == kButton1Double && (fBoxCut.IsActive() || HasSections())) {
      if (fBoxCut.IsActive())
         fBoxCut.TurnOnOff();
      const TGLVertex3 *frame = fBackBox.Get3DBox();
      fXOZSectionPos = frame[0].Y();
      fYOZSectionPos = frame[0].X();
      fXOYSectionPos = frame[0].Z();

      if (!gVirtualX->IsCmdThread())
         gROOT->ProcessLineFast(Form("((TGLPlotPainter *)0x%lx)->Paint()", this));
      else
         Paint();
   }
}

//______________________________________________________________________________
void TGLTF3Painter::InitGL() const
{
   //Initialize OpenGL state variables.
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   glDisable(GL_CULL_FACE);
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

//______________________________________________________________________________
void TGLTF3Painter::DrawToSelectionBuffer() const
{
   //Draw triangles, no normals, no lighting.
   Rgl::ObjectIDToColor(fSelectionBase, fHighColor);

   if (!fBoxCut.IsActive())
      DrawMesh(GL_DOUBLE, fMesh.fVerts, fMesh.fTris);
   else
      DrawMesh(&glVertex3dv, fMesh.fVerts, fMesh.fTris, fBoxCut);
}

//______________________________________________________________________________
void TGLTF3Painter::DrawDefaultPlot() const
{
   //Surface with material properties and lighting.
   if (HasSections()) {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDepthMask(GL_FALSE);
   }

   SetSurfaceColor();

   if (!fBoxCut.IsActive()) {
      DrawMesh(GL_DOUBLE, fMesh.fVerts, fMesh.fNorms, fMesh.fTris);
   } else {
      DrawMesh(&glNormal3dv, &glVertex3dv, fMesh.fVerts, fMesh.fNorms, 
               fMesh.fTris, fBoxCut);
   }

   if (HasSections()) {
      glDisable(GL_BLEND);
      glDepthMask(GL_TRUE);
   }
}

//______________________________________________________________________________
void TGLTF3Painter::DrawMaplePlot() const
{
   //Colored surface, without lighting and
   //material properties.
   const TGLDisableGuard lightGuard(GL_LIGHTING);

   if (HasSections() && fStyle < kMaple2) {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDepthMask(GL_FALSE);
   }

   if (fStyle == kMaple1) {//Shaded polygons and outlines.
      glEnable(GL_POLYGON_OFFSET_FILL);//[1
      glPolygonOffset(1.f, 1.f);
   } else if (fStyle == kMaple2)//Colored outlines only.
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//[2

   if(!fBoxCut.IsActive())
      DrawMapleMesh(fMesh.fVerts, fMesh.fNorms, fMesh.fTris);
   else
      DrawMapleMesh(fMesh.fVerts, fMesh.fNorms, fMesh.fTris, fBoxCut);

   if (fStyle == kMaple1) {
      //Draw outlines.
      glDisable(GL_POLYGON_OFFSET_FILL);//1]
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//[3
      glColor4d(0., 0., 0., 0.25);

      if(!fBoxCut.IsActive())
         DrawMesh(GL_DOUBLE, fMesh.fVerts, fMesh.fTris);
      else
         DrawMesh(&glVertex3dv, fMesh.fVerts, fMesh.fTris, fBoxCut);

      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//[3
   } else if (fStyle == kMaple2)
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   if (HasSections() && fStyle < kMaple2) {
      glDisable(GL_BLEND);
      glDepthMask(GL_TRUE);
   }
}

//______________________________________________________________________________
void TGLTF3Painter::DrawPlot() const
{
   //Draw mesh.
   fBackBox.DrawBox(fSelectedPart, fSelectionPass, fZLevels, fHighColor);
   DrawSections();
   
   if (fSelectionPass) {
      DrawToSelectionBuffer();
   } else if (fStyle == kDefault) {
      DrawDefaultPlot();
   } else {
      DrawMaplePlot();
   }

   if (fBoxCut.IsActive())
      fBoxCut.DrawBox(fSelectionPass, fSelectedPart);
}

//______________________________________________________________________________
void TGLTF3Painter::SetSurfaceColor() const
{
   //Set color for surface.
   Float_t diffColor[] = {0.8f, 0.8f, 0.8f, 0.15f};

   if (fF3->GetFillColor() != kWhite)
      if (const TColor *c = gROOT->GetColor(fF3->GetFillColor()))
         c->GetRGB(diffColor[0], diffColor[1], diffColor[2]);

   glMaterialfv(GL_BACK, GL_DIFFUSE, diffColor);
   diffColor[0] /= 2, diffColor[1] /= 2, diffColor[2] /= 2;
   glMaterialfv(GL_FRONT, GL_DIFFUSE, diffColor);
   const Float_t specColor[] = {1.f, 1.f, 1.f, 1.f};
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 70.f);
}

//______________________________________________________________________________
Bool_t TGLTF3Painter::HasSections() const
{
   //Any section exists.
   return fXOZSectionPos > fBackBox.Get3DBox()[0].Y() || 
          fYOZSectionPos > fBackBox.Get3DBox()[0].X() ||
          fXOYSectionPos > fBackBox.Get3DBox()[0].Z();
}

//______________________________________________________________________________
void TGLTF3Painter::DrawSectionXOZ() const
{
   // Draw XOZ parallel section.
   if (fSelectionPass)
      return;
   fXOZSlice.DrawSlice(fXOZSectionPos / fCoord->GetYScale());
}

//______________________________________________________________________________
void TGLTF3Painter::DrawSectionYOZ() const
{
   // Draw YOZ parallel section.
   if (fSelectionPass)
      return;
   fYOZSlice.DrawSlice(fYOZSectionPos / fCoord->GetXScale());
}

//______________________________________________________________________________
void TGLTF3Painter::DrawSectionXOY() const
{
   // Draw XOY parallel section.
   if (fSelectionPass)
      return;
   fXOYSlice.DrawSlice(fXOYSectionPos / fCoord->GetZScale());
}


//______________________________________________________________________________
//
// "gliso" option for TH3.

ClassImp(TGLIsoPainter)

//______________________________________________________________________________
TGLIsoPainter::TGLIsoPainter(TH1 *hist, TGLOrthoCamera *camera, TGLPlotCoordinates *coord, TGLPaintDevice *dev)
                  : TGLPlotPainter(hist, camera, coord, dev, kFALSE, kFALSE, kFALSE),
                    fXOZSlice("XOZ", (TH3 *)hist, coord, &fBackBox, TGLTH3Slice::kXOZ),
                    fYOZSlice("YOZ", (TH3 *)hist, coord, &fBackBox, TGLTH3Slice::kYOZ),
                    fXOYSlice("XOY", (TH3 *)hist, coord, &fBackBox, TGLTH3Slice::kXOY),
                    fInit(kFALSE)
{
   //Constructor.
   if (hist->GetDimension() < 3)
      Error("TGLIsoPainter::TGLIsoPainter", "Wrong type of histogramm, must have 3 dimensions");
}

//______________________________________________________________________________
char *TGLIsoPainter::GetPlotInfo(Int_t /*px*/, Int_t /*py*/)
{
   //Return info for plot part under cursor.

   static char mess[] = { "iso" };
   return mess;
}

//______________________________________________________________________________
Bool_t TGLIsoPainter::InitGeometry()
{
   //Initializes meshes for 3d iso contours.
   if (fHist->GetDimension() < 3) {
      Error("TGLIsoPainter::TGLIsoPainter", "Wrong type of histogramm, must have 3 dimensions");
      return kFALSE;
   }

   //Create mesh.
   if (fInit)
      return kTRUE;

   //Only in cartesian.
   fCoord->SetCoordType(kGLCartesian);
   if (!fCoord->SetRanges(fHist, kFALSE, kTRUE))
      return kFALSE;

   fBackBox.SetPlotBox(fCoord->GetXRangeScaled(), fCoord->GetYRangeScaled(), fCoord->GetZRangeScaled());
   if (fCamera) fCamera->SetViewVolume(fBackBox.Get3DBox());

   //Move old meshes into the cache.
   if (!fIsos.empty())
      fCache.splice(fCache.begin(), fIsos);
   //Number of contours == number of iso surfaces.
   UInt_t nContours = fHist->GetContour();

   if (nContours > 1) {
      fColorLevels.resize(nContours);
      FindMinMax();

      if (fHist->TestBit(TH1::kUserContour)) {
         //There are user defined contours (iso-levels).
         for (UInt_t i = 0; i < nContours; ++i)
            fColorLevels[i] = fHist->GetContourLevelPad(i);
      } else {
         //Equidistant iso-surfaces.
         const Double_t isoStep = (fMinMax.second - fMinMax.first) / nContours;
         for (UInt_t i = 0; i < nContours; ++i)
            fColorLevels[i] = fMinMax.first + i * isoStep;
      }

      fPalette.GeneratePalette(nContours, fMinMax, kFALSE);
   } else {
      //Only one iso (ROOT's standard).
      fColorLevels.resize(nContours = 1);
      fColorLevels[0] = fHist->GetSumOfWeights() / (fHist->GetNbinsX() * fHist->GetNbinsY() * fHist->GetNbinsZ());
   }

   MeshIter_t firstMesh = fCache.begin();
   //Initialize meshes, trying to reuse mesh from
   //mesh cache.
   for (UInt_t i = 0; i < nContours; ++i) {
      if (firstMesh != fCache.end()) {
         //There is a mesh in a chache.
         SetMesh(*firstMesh, fColorLevels[i]);
         MeshIter_t next = firstMesh;
         ++next;
         fIsos.splice(fIsos.begin(), fCache, firstMesh);
         firstMesh = next;
      } else {
         //No meshes in a cache.
         //Create new one and _swap_ data (look at Mesh_t::Swap in a header)
         //between empty mesh in a list and this mesh
         //to avoid real copying.
         Mesh_t newMesh;
         SetMesh(newMesh, fColorLevels[i]);
         fIsos.push_back(fDummyMesh);
         fIsos.back().Swap(newMesh);
      }
   }

   if (fCoord->Modified()) {
      fUpdateSelection = kTRUE;
      fXOZSectionPos = fBackBox.Get3DBox()[0].Y();
      fYOZSectionPos = fBackBox.Get3DBox()[0].X();
      fXOYSectionPos = fBackBox.Get3DBox()[0].Z();
      fCoord->ResetModified();
   }

   //Avoid rebuilding the mesh.
   fInit = kTRUE;

   return kTRUE;

}

//______________________________________________________________________________
void TGLIsoPainter::StartPan(Int_t px, Int_t py)
{
   //User clicks right mouse button (in a pad).
   fMousePosition.fX = px;
   fMousePosition.fY = fCamera->GetHeight() - py;
   fCamera->StartPan(px, py);
   fBoxCut.StartMovement(px, fCamera->GetHeight() - py);
}

//______________________________________________________________________________
void TGLIsoPainter::Pan(Int_t px, Int_t py)
{
   //User's moving mouse cursor, with middle mouse button pressed (for pad).
   //Calculate 3d shift related to 2d mouse movement.
   //User's moving mouse cursor, with middle mouse button pressed (for pad).
   //Calculate 3d shift related to 2d mouse movement.

   if (!MakeGLContextCurrent())
      return;

   if (fSelectedPart >= fSelectionBase)//Pan camera.
      fCamera->Pan(px, py);
   else if (fSelectedPart > 0) {
      //Convert py into bottom-top orientation.
      //Possibly, move box here
      py = fCamera->GetHeight() - py;
      if (!fHighColor) {
         if (fBoxCut.IsActive() && (fSelectedPart >= kXAxis && fSelectedPart <= kZAxis))
            fBoxCut.MoveBox(px, py, fSelectedPart);
         else;
            //MoveSection(px, py);
      } else {
         //MoveSection(px, py);
      }
   }

   fMousePosition.fX = px, fMousePosition.fY = py;
   fUpdateSelection = kTRUE;
}

//______________________________________________________________________________
void TGLIsoPainter::AddOption(const TString &/*option*/)
{
   //No additional options for TGLIsoPainter.
}

//______________________________________________________________________________
void TGLIsoPainter::ProcessEvent(Int_t event, Int_t /*px*/, Int_t py)
{
   //Change color sheme.
   if (event == kKeyPress) {
      if (py == kKey_c || py == kKey_C) {
         if (fHighColor)
            Info("ProcessEvent", "Cut box does not work in high color, please, switch to true color");
         else {
            fBoxCut.TurnOnOff();
            fUpdateSelection = kTRUE;
         }
      }
   } else if (event == kButton1Double && (fBoxCut.IsActive() || HasSections())) {
      if (fBoxCut.IsActive())
         fBoxCut.TurnOnOff();
      const TGLVertex3 *frame = fBackBox.Get3DBox();
      fXOZSectionPos = frame[0].Y();
      fYOZSectionPos = frame[0].X();
      fXOYSectionPos = frame[0].Z();

      if (!gVirtualX->IsCmdThread())
         gROOT->ProcessLineFast(Form("((TGLPlotPainter *)0x%lx)->Paint()", this));
      else
         Paint();
   }
}

//______________________________________________________________________________
void TGLIsoPainter::InitGL() const
{
   //Initialize OpenGL state variables.
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);
   glDisable(GL_CULL_FACE);
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

//______________________________________________________________________________
void TGLIsoPainter::DrawPlot() const
{
   //Draw mesh.
   fBackBox.DrawBox(fSelectedPart, fSelectionPass, fZLevels, fHighColor);
   DrawSections();
   
   if (fIsos.size() != fColorLevels.size()) {
      Error("TGLIsoPainter::DrawPlot", "Non-equal number of levels and isos");
      return;
   }

   if (!fSelectionPass && HasSections()) {
      //Surface is semi-transparent during dynamic profiling.
      //Having several complex nested surfaces, it's not easy
      //(possible?) to implement correct and _efficient_ transparency
      //drawing. So, artefacts are possbile.
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDepthMask(GL_FALSE);
   }

   UInt_t colorInd = 0;
   ConstMeshIter_t iso = fIsos.begin();

   for (; iso != fIsos.end(); ++iso, ++colorInd)
      DrawMesh(*iso, colorInd);

   if (!fSelectionPass && HasSections()) {
      glDisable(GL_BLEND);
      glDepthMask(GL_TRUE);
   }

   if (fBoxCut.IsActive())
      fBoxCut.DrawBox(fSelectionPass, fSelectedPart);
}

//______________________________________________________________________________
void TGLIsoPainter::DrawSectionXOZ() const
{
   // Draw XOZ parallel section.
   if (fSelectionPass)
      return;
   fXOZSlice.DrawSlice(fXOZSectionPos / fCoord->GetYScale());
}

//______________________________________________________________________________
void TGLIsoPainter::DrawSectionYOZ() const
{
   // Draw YOZ parallel section.
   if (fSelectionPass)
      return;
   fYOZSlice.DrawSlice(fYOZSectionPos / fCoord->GetXScale());
}

//______________________________________________________________________________
void TGLIsoPainter::DrawSectionXOY() const
{
   // Draw XOY parallel section.
   if (fSelectionPass)
      return;
   fXOYSlice.DrawSlice(fXOYSectionPos / fCoord->GetZScale());
}

//______________________________________________________________________________
Bool_t TGLIsoPainter::HasSections() const
{
   //Any section exists.
   return fXOZSectionPos > fBackBox.Get3DBox()[0].Y() || fYOZSectionPos > fBackBox.Get3DBox()[0].X() ||
          fXOYSectionPos > fBackBox.Get3DBox()[0].Z();
}

//______________________________________________________________________________
void TGLIsoPainter::SetSurfaceColor(Int_t ind) const
{
   //Set color for surface.
   Float_t diffColor[] = {0.8f, 0.8f, 0.8f, 0.25f};

   if (fColorLevels.size() == 1) {
      if (fHist->GetFillColor() != kWhite)
         if (const TColor *c = gROOT->GetColor(fHist->GetFillColor()))
            c->GetRGB(diffColor[0], diffColor[1], diffColor[2]);
   } else {
      const UChar_t *color = fPalette.GetColour(ind);
      diffColor[0] = color[0] / 255.;
      diffColor[1] = color[1] / 255.;
      diffColor[2] = color[2] / 255.;
   }

   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffColor);
   const Float_t specColor[] = {1.f, 1.f, 1.f, 1.f};
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
   diffColor[0] /= 3.5, diffColor[1] /= 3.5, diffColor[2] /= 3.5;
   glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, diffColor);
   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 30.f);
}

//______________________________________________________________________________
void TGLIsoPainter::SetMesh(Mesh_t &m, Double_t isoValue)
{
   Rgl::Mc::TGridGeometry<Float_t> geom;
   //Get grid parameters.
   geom.fMinX  = fXAxis->GetBinCenter(fXAxis->GetFirst());
   geom.fStepX = (fXAxis->GetBinCenter(fXAxis->GetLast()) - geom.fMinX) / (fHist->GetNbinsX() - 1);
   geom.fMinY  = fYAxis->GetBinCenter(fYAxis->GetFirst());
   geom.fStepY = (fYAxis->GetBinCenter(fYAxis->GetLast()) - geom.fMinY) / (fHist->GetNbinsY() - 1);
   geom.fMinZ  = fZAxis->GetBinCenter(fZAxis->GetFirst());
   geom.fStepZ = (fZAxis->GetBinCenter(fZAxis->GetLast()) - geom.fMinZ) / (fHist->GetNbinsZ() - 1);
   //Scale grid parameters.
   geom.fMinX *= fCoord->GetXScale(), geom.fStepX *= fCoord->GetXScale();
   geom.fMinY *= fCoord->GetYScale(), geom.fStepY *= fCoord->GetYScale();
   geom.fMinZ *= fCoord->GetZScale(), geom.fStepZ *= fCoord->GetZScale();
   //Clear mesh if it was from cache.
   m.ClearMesh();
   //Select correct TMeshBuilder type.
   if (typeid(*fHist) == typeid(TH3C)) {
      Rgl::Mc::TMeshBuilder<TH3C, Float_t> builder(kTRUE);
      builder.BuildMesh(static_cast<TH3C *>(fHist), geom, &m, isoValue);
   } else if (typeid(*fHist) == typeid(TH3S)) {
      Rgl::Mc::TMeshBuilder<TH3S, Float_t> builder(kTRUE);
      builder.BuildMesh(static_cast<TH3S *>(fHist), geom, &m, isoValue);
   } else if (typeid(*fHist) == typeid(TH3I)) {
      Rgl::Mc::TMeshBuilder<TH3I, Float_t> builder(kTRUE);
      builder.BuildMesh(static_cast<TH3I *>(fHist), geom, &m, isoValue);
   } else if (typeid(*fHist) == typeid(TH3F)) {
      Rgl::Mc::TMeshBuilder<TH3F, Float_t> builder(kTRUE);
      builder.BuildMesh(static_cast<TH3F *>(fHist), geom, &m, isoValue);
   } else if (typeid(*fHist) == typeid(TH3D)) {
      Rgl::Mc::TMeshBuilder<TH3D, Float_t> builder(kTRUE);
      builder.BuildMesh(static_cast<TH3D *>(fHist), geom, &m, isoValue);
   }
}

//______________________________________________________________________________
void TGLIsoPainter::DrawMesh(const Mesh_t &m, Int_t level) const
{
   //Draw TF3 surface
   if (!fSelectionPass)
      SetSurfaceColor(level);

   if (!fBoxCut.IsActive()) {
      if (!fSelectionPass)
         ::DrawMesh(GL_FLOAT, m.fVerts, m.fNorms, m.fTris);
      else {
         Rgl::ObjectIDToColor(fSelectionBase, fHighColor);
         ::DrawMesh(GL_FLOAT, m.fVerts, m.fTris);
      }
   } else {
      if (!fSelectionPass)
         ::DrawMesh(&glNormal3fv, &glVertex3fv, m.fVerts, m.fNorms, m.fTris, fBoxCut);
      else {
         Rgl::ObjectIDToColor(fSelectionBase, fHighColor);
         ::DrawMesh(&glVertex3fv, m.fVerts, m.fTris, fBoxCut);
      }
   }
}

//______________________________________________________________________________
void TGLIsoPainter::FindMinMax()
{
   //Find max/min bin contents for TH3.
   fMinMax.first  = fHist->GetBinContent(fXAxis->GetFirst(), fYAxis->GetFirst(), fZAxis->GetFirst());
   fMinMax.second = fMinMax.first;

   for (Int_t i = fXAxis->GetFirst(), ei = fXAxis->GetLast(); i <= ei; ++i) {
      for (Int_t j = fYAxis->GetFirst(), ej = fYAxis->GetLast(); j <= ej; ++j) {
         for (Int_t k = fZAxis->GetFirst(), ek = fZAxis->GetLast(); k <= ek; ++k) {
            const Double_t binContent = fHist->GetBinContent(i, j, k);
            fMinMax.first  = TMath::Min(binContent, fMinMax.first);
            fMinMax.second = TMath::Max(binContent, fMinMax.second);
         }
      }
   }
}
