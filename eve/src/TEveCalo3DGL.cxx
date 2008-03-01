// @(#)root/eve:$Id$
// Author: Matevz Tadel 2007

/*************************************************************************
 * Copyright (C) 1995-2007, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#include "TEveCalo3DGL.h"
#include "TEveCalo.h"

#include "TMath.h"

#include "TGLRnrCtx.h"
#include "TGLSelectRecord.h"
#include "TGLIncludes.h"
#include "TGLUtil.h"
#include "TEveRGBAPalette.h"

//______________________________________________________________________________
// OpenGL renderer class for TEveCalo.
//

ClassImp(TEveCalo3DGL);

//______________________________________________________________________________
TEveCalo3DGL::TEveCalo3DGL() :
   TGLObject(), fM(0)
{
   // Constructor.

}

//______________________________________________________________________________
Bool_t TEveCalo3DGL::SetModel(TObject* obj, const Option_t* /*opt*/)
{
   // Set model object.

   if (SetModelCheckClass(obj, TEveCalo3D::Class())) {
      fM = dynamic_cast<TEveCalo3D*>(obj);
      return kTRUE;
   }
   return kFALSE;
}

//______________________________________________________________________________
void TEveCalo3DGL::SetBBox()
{
   // Set bounding box.

   // !! This ok if master sub-classed from TAttBBox
   SetAxisAlignedBBox(((TEveCalo3D*)fExternalObj)->AssertBBox());
}

//______________________________________________________________________________
inline void TEveCalo3DGL::CrossProduct(const Float_t a[3], const Float_t b[3],
                                      const Float_t c[3], Float_t out[3]) const
{
   const Float_t v1[3] = { a[0] - c[0], a[1] - c[1], a[2] - c[2] };
   const Float_t v2[3] = { b[0] - c[0], b[1] - c[1], b[2] - c[2] };

   out[0] = v1[1] * v2[2] - v1[2] * v2[1];
   out[1] = v1[2] * v2[0] - v1[0] * v2[2];
   out[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

//______________________________________________________________________________
void  TEveCalo3DGL::RenderBox(Float_t pnts[8]) const
{
   Float_t *p = pnts;
   Float_t cross[3];

   // bottom: 0123
   glBegin(GL_POLYGON);
   CrossProduct(p+3, p+9, p, cross);
   glNormal3fv(cross);
   glVertex3fv(p);
   glVertex3fv(p+3);
   glVertex3fv(p+6);
   glVertex3fv(p+9);
   glEnd();
   // top:    7654
   glBegin(GL_POLYGON);
   CrossProduct(p+21, p+15, p+12, cross);
   glNormal3fv(cross);
   glVertex3fv(p+21);
   glVertex3fv(p+18);
   glVertex3fv(p+15);
   glVertex3fv(p+12);
   glEnd();
   // back:   0451
   glBegin(GL_POLYGON);
   CrossProduct(p+12, p+3, p, cross);
   glNormal3fv(cross);
   glVertex3fv(p);
   glVertex3fv(p+12);
   glVertex3fv(p+15);
   glVertex3fv(p+3);
   glEnd();
   //front :  3267
   glBegin(GL_POLYGON);
   CrossProduct(p+6, p+21, p+9, cross);
   glNormal3fv(cross);
   glVertex3fv(p+9);
   glVertex3fv(p+6);
   glVertex3fv(p+18);
   glVertex3fv(p+21);
   glEnd();
   // left:    0374
   glBegin(GL_POLYGON);
   CrossProduct(p+21, p, p+9, cross);
   glNormal3fv(cross);
   glVertex3fv(p);
   glVertex3fv(p+9);
   glVertex3fv(p+21);
   glVertex3fv(p+12);
   glEnd();
   // right:   1562
   glBegin(GL_POLYGON);
   CrossProduct(p+15, p+6, p+3, cross);
   glNormal3fv(cross);
   glVertex3fv(p+3);
   glVertex3fv(p+15);
   glVertex3fv(p+18);
   glVertex3fv(p+6);
   glEnd();
}

//______________________________________________________________________________
Float_t TEveCalo3DGL::RenderBarrelCell(const TEveCaloData::CellData_t &cellData, Float_t towerH, Float_t offset ) const
{
   using namespace TMath;

   //   printf("RenderBarrel "); cellData.Dump();

   Float_t r1 = fM->GetBarrelRadius() + offset;
   Float_t r2 = r1 + towerH*Sin(cellData.ThetaMin());
   // printf("r1 %f r2 %f \n", r1, r2);

   Float_t z1In, z1Out, z2In, z2Out;

   if(cellData.ZSideSign() == 1)
   {
      z1In  = r1/Tan(cellData.ThetaMax());
      z1Out = r2/Tan(cellData.ThetaMax());
      z2In  = r1/Tan(cellData.ThetaMin());
      z2Out = r2/Tan(cellData.ThetaMin());

   } else {
      z1In  = -r1/Tan(cellData.ThetaMin());
      z1Out = -r2/Tan(cellData.ThetaMin());
      z2In  = -r1/Tan(cellData.ThetaMax());
      z2Out = -r2/Tan(cellData.ThetaMax());
   }

   Float_t cos1 = Cos(cellData.PhiMin());
   Float_t sin1 = Sin(cellData.PhiMin());
   Float_t cos2 = Cos(cellData.PhiMax());
   Float_t sin2 = Sin(cellData.PhiMax());

   Float_t box[24];
   Float_t* pnts = box;
   // 0
   pnts[0] = r1*cos2;
   pnts[1] = r1*sin2;
   pnts[2] = z1In;
   pnts += 3;
   // 1
   pnts[0] = r1*cos1;
   pnts[1] = r1*sin1;
   pnts[2] = z1In;
   pnts += 3;
   // 2
   pnts[0] = r1*cos1;
   pnts[1] = r1*sin1;
   pnts[2] = z2In;
   pnts += 3;
   // 3
   pnts[0] = r1*cos2;
   pnts[1] = r1*sin2;
   pnts[2] = z2In;
   pnts += 3;
   //---------------------------------------------------
   // 4
   pnts[0] = r2*cos2;
   pnts[1] = r2*sin2;
   pnts[2] = z1Out;
   pnts += 3;
   // 5
   pnts[0] = r2*cos1;
   pnts[1] = r2*sin1;
   pnts[2] = z1Out;
   pnts += 3;
   // 6
   pnts[0] = r2*cos1;
   pnts[1] = r2*sin1;
   pnts[2] = z2Out;
   pnts += 3;
   // 7
   pnts[0] = r2*cos2;
   pnts[1] = r2*sin2;
   pnts[2] = z2Out;

   RenderBox(box);
   return offset+towerH*Sin(cellData.ThetaMin());
}// end RenderBarrelCell

//______________________________________________________________________________
Float_t TEveCalo3DGL::RenderEndCapCell(const TEveCaloData::CellData_t &cellData, Float_t towerH, Float_t offset ) const
{
   using namespace TMath;

   //   printf("render endcap %f, %f ", z1, z2); cellData.Dump();

   Float_t z1, r1In, r1Out, z2, r2In, r2Out;
   if (cellData.ZSideSign() == 1)
   {
      z1    = fM->fEndCapPos + offset;
      r1In  = z1*Tan(cellData.ThetaMin());
      r1Out = z1*Tan(cellData.ThetaMax());

      z2    = z1 + towerH;
      r2In  = z2*Tan(cellData.ThetaMin());
      r2Out = z2*Tan(cellData.ThetaMax());
   } else
   {
      z2    = fM->fEndCapPos + offset;
      r2In  = z2*Tan(cellData.ThetaMin());
      r2Out = z2*Tan(cellData.ThetaMax());

      z1    = z2 + towerH;
      r1In  = z1*Tan(cellData.ThetaMin());
      r1Out = z1*Tan(cellData.ThetaMax());

      z1 *= -1;
      z2 *= -1;
   }

   Float_t cos1 = Cos(cellData.PhiMin());
   Float_t sin1 = Sin(cellData.PhiMin());
   Float_t cos2 = Cos(cellData.PhiMax());
   Float_t sin2 = Sin(cellData.PhiMax());

   Float_t box[24];
   Float_t* pnts = box;
   // 0
   pnts[0] = r1In*cos2;
   pnts[1] = r1In*sin2;
   pnts[2] = z1;
   pnts += 3;
   // 1
   pnts[0] = r1In*cos1;
   pnts[1] = r1In*sin1;
   pnts[2] = z1;
   pnts += 3;
   // 2
   pnts[0] = r2In*cos1;
   pnts[1] = r2In*sin1;
   pnts[2] = z2;
   pnts += 3;
   // 3
   pnts[0] = r2In*cos2;
   pnts[1] = r2In*sin2;
   pnts[2] = z2;
   pnts += 3;
   //---------------------------------------------------
   // 4
   pnts[0] = r1Out*cos2;
   pnts[1] = r1Out*sin2;
   pnts[2] = z1;
   pnts += 3;
   // 5
   pnts[0] = r1Out*cos1;
   pnts[1] = r1Out*sin1;
   pnts[2] = z1;
   pnts += 3;
   // 6
   pnts[0] = r2Out*cos1;
   pnts[1] = r2Out*sin1;
   pnts[2] = z2;
   pnts += 3;
   // 7
   pnts[0] = r2Out*cos2;
   pnts[1] = r2Out*sin2;
   pnts[2] = z2;

   RenderBox(box);
   return offset+towerH*Cos(cellData.ThetaMin());
} // end RenderEndCapCell

 //______________________________________________________________________________
void TEveCalo3DGL::DirectDraw(TGLRnrCtx &rnrCtx) const
{
   // printf("TEveCalo3D::DirectDraw\n");
   fM->AssertPalette();  

   if (fM->fCacheOK == kFALSE) 
   {
      fM->ResetCache();
      fM->fData->GetCellList((fM->fEtaMin+fM->fEtaMax)*0.5f, fM->fEtaMax -fM->fEtaMin,
                             fM->fPhi, fM->fPhiRng, fM->fThreshold, fM->fCellList);
      fM->fCacheOK= kTRUE;
   }

   glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT);
   glEnable(GL_NORMALIZE);

   TEveCaloData::CellData_t cellData;  
   Float_t transTheta = fM->GetTransitionTheta();
   Float_t towerH;
   Bool_t  visible;
   Int_t   prevTower;
   Float_t offset = 0;

   if (rnrCtx.SecSelection()) glPushName(0);
   for(UInt_t i=0; i<fM->fCellList.size(); i++) 
   {
      fM->fData->GetCellData(fM->fCellList[i], cellData);

      if (fM->fCellList[i].fTower != prevTower) 
      {
         offset = 0;
         prevTower = fM->fCellList[i].fTower;
      }
      fM->SetupColorHeight(cellData.Value(), fM->fCellList[i].fSlice, towerH, visible);

      if (visible)
      {
         if (rnrCtx.SecSelection()) glLoadName(i);
         if (cellData.ThetaMax() > transTheta)
            offset = RenderBarrelCell(cellData, towerH, offset);
         else
            offset = RenderEndCapCell(cellData, towerH, offset);
      }
   }
   if (rnrCtx.SecSelection()) glPopName();

   glPopAttrib();
}

//______________________________________________________________________________
void TEveCalo3DGL::ProcessSelection(TGLRnrCtx & /*rnrCtx*/, TGLSelectRecord & rec)
{
   // Processes secondary selection from TGLViewer.

   if (rec.GetN() < 2) return;

   Int_t cellID = rec.GetItem(1);
   TEveCaloData::CellData_t cellData;  
   fM->fData->GetCellData(fM->fCellList[cellID], cellData);

   printf("Tower selected in slice %d \n",fM->fCellList[cellID].fSlice);
   cellData.Dump();
}
