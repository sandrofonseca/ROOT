// @(#)root/geom:$Name:  $:$Id: TGeoXtru.cxx,v 1.2 2004/03/15 12:11:51 brun Exp $
// Author: Mihaela Gheata   24/01/04

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

/*************************************************************************
 * TGeoXtru - An extrusion with fixed outline shape in x-y and a sequence
 * of z extents (segments).  The overall scale of the outline scales
 * linearly between z points and the center can have an x-y offset.
 *
 * Based on the initial implementation of R. Hatcher
 *************************************************************************/

// Creation of TGeoXtru shape
//=============================
//   A TGeoXtru represents a polygonal extrusion. It is defined by the:
// a. 'Blueprint' of the arbitrary polygon representing any Z section. This
//    is an arbytrary polygon (convex or not) defined by the X/Y positions of
//    its vertices.
// b. A sequence of Z sections ordered on the Z axis. Each section defines the
//   'actual' parameters of the polygon at a given Z. The sections may be 
//    translated with respect to the blueprint and/or scaled. The TGeoXtru
//   segment in between 2 Z sections is a solid represented by the linear 
//   extrusion between the 2 polygons. Two consecutive sections may be defined
//   at same Z position.
//
// 1. TGeoXtru *xtru = TGeoXtru(Int_t nz);
//   where nz=number of Z planes
// 2. Double_t x[nvertices]; // array of X positions of blueprint polygon vertices
//    Double_t y[nvertices]; // array of Y positions of blueprint polygon vertices
// 3. xtru->DefinePolygon(nvertices,x,y);
// 4. DefineSection(0, z0, x0, y0, scale0); // Z position, offset and scale for first section
//    DefineSection(1, z1, x1, y1, scale1); // -''- secons section
//    ....
//    DefineSection(nz-1, zn, xn, yn, scalen); // parameters for last section
//
// *NOTES*
// Currently navigation functionality not fully implemented (only Contains()).
// Decomposition in concave polygons not implemented - drawing in solid mode
// within x3d produces incorrect end-faces

#include "TROOT.h"

#include "TGeoManager.h"
#include "TGeoVolume.h"
#include "TGeoPolygon.h"
#include "TVirtualGeoPainter.h"
#include "TGeoXtru.h"

ClassImp(TGeoXtru)

//_____________________________________________________________________________
TGeoXtru::TGeoXtru()
{
// dummy ctor
   SetShapeBit(TGeoShape::kGeoXtru);
   fNvert = 0;
   fNz = 0;
   fZcurrent = 0.;
   fPoly = 0;
   fX = 0;
   fY = 0;
   fXc = 0;
   fYc = 0;
   fZ = 0;
   fScale = 0;
   fX0 = 0;
   fY0 = 0;
}   

//_____________________________________________________________________________
TGeoXtru::TGeoXtru(Int_t nz)
         :TGeoBBox(0, 0, 0)
{
// Default constructor
   SetShapeBit(TGeoShape::kGeoXtru);
   if (nz<2) {
      Error("ctor", "Cannot create TGeoXtru with less than 2 Z planes");
      return;
   }
   fNvert = 0;
   fNz = nz;   
   fZcurrent = 0.;
   fPoly = 0;
   fX = 0;
   fY = 0;
   fXc = 0;
   fYc = 0;
   fZ = new Double_t[nz];
   fScale = new Double_t[nz];
   fX0 = new Double_t[nz];
   fY0 = new Double_t[nz];
}

//_____________________________________________________________________________
TGeoXtru::TGeoXtru(Double_t *param)
         :TGeoBBox(0, 0, 0)
{
// Default constructor in GEANT3 style
// param[0] = nz  // number of z planes
//
// param[1] = z1  // Z position of first plane
// param[2] = x1  // X position of first plane
// param[3] = y1  // Y position of first plane
// param[4] = scale1  // scale factor for first plane
// ...
// param[4*(nz-1]+1] = zn
// param[4*(nz-1)+2] = xn
// param[4*(nz-1)+3] = yn
// param[4*(nz-1)+4] = scalen
   SetShapeBit(TGeoShape::kGeoXtru);
   fNvert = 0;
   fNz = 0;
   fZcurrent = 0.;
   fPoly = 0;
   fX = 0;
   fY = 0;
   fXc = 0;
   fYc = 0;
   fZ = 0;
   fScale = 0;
   fX0 = 0;
   fY0 = 0;
   SetDimensions(param);
}

//_____________________________________________________________________________
TGeoXtru::~TGeoXtru()
{
// destructor
   if (fX)  {delete[] fX; fX = 0;}
   if (fY)  {delete[] fY; fY = 0;}
   if (fXc) {delete[] fXc; fXc = 0;}
   if (fYc) {delete[] fYc; fYc = 0;}
   if (fZ)  {delete[] fZ; fZ = 0;}
   if (fScale) {delete[] fScale; fScale = 0;}
   if (fX0)  {delete[] fX0; fX0 = 0;}
   if (fY0)  {delete[] fY0; fY0 = 0;}
}

//_____________________________________________________________________________   
void TGeoXtru::ComputeBBox()
{
// compute bounding box of the pcon
   if (!fX || !fZ || !fNvert) {
      Error("ComputeBBox", "polygon not defined");
      return;
   }   
   Double_t zmin = fZ[0];
   Double_t zmax = fZ[fNz-1];
   Double_t xmin = TGeoShape::Big();
   Double_t xmax = -TGeoShape::Big();
   Double_t ymin = TGeoShape::Big();
   Double_t ymax = -TGeoShape::Big();
   for (Int_t i=0; i<fNz; i++) {
      SetCurrentVertices(fX0[i], fY0[i], fScale[i]);
      for (Int_t j=0; j<fNvert; j++) {
         if (fXc[j]<xmin) xmin=fXc[j];
         if (fXc[j]>xmax) xmax=fXc[j];
         if (fYc[j]<ymin) ymin=fYc[j];
         if (fYc[j]>ymax) ymax=fYc[j];
      }
   }
   fOrigin[0] = 0.5*(xmin+xmax);      
   fOrigin[1] = 0.5*(ymin+ymax);      
   fOrigin[2] = 0.5*(zmin+zmax);      
   fDX = 0.5*(xmax-xmin);
   fDY = 0.5*(ymax-ymin);
   fDZ = 0.5*(zmax-zmin);
}   

//_____________________________________________________________________________   
void TGeoXtru::ComputeNormal(Double_t * /*point*/, Double_t * /*dir*/, Double_t *norm)
{
// Compute normal to closest surface from POINT. 
   memset(norm,0,3*sizeof(Double_t));
   norm[2] = 1.;
   Warning("ComputeNormal", "not implemented");
}

//_____________________________________________________________________________
Bool_t TGeoXtru::Contains(Double_t *point) const
{
// test if point is inside this shape
   // Check Z range
   TGeoXtru *xtru = (TGeoXtru*)this;
   if (point[2]<fZ[0]) return kFALSE;   
   if (point[2]>fZ[fNz-1]) return kFALSE; 
   Int_t iz = TMath::BinarySearch(fNz, fZ, point[2]);
   if (point[2]==fZ[iz]) {
      xtru->SetCurrentVertices(fX0[iz],fY0[iz], fScale[iz]);
      if (fPoly->Contains(point)) return kTRUE;
      if (iz>1 && fZ[iz]==fZ[iz-1]) {
         xtru->SetCurrentVertices(fX0[iz-1],fY0[iz-1], fScale[iz-1]);
         return fPoly->Contains(point);
      } else if (iz<fNz-2 && fZ[iz]==fZ[iz+1]) {
         xtru->SetCurrentVertices(fX0[iz+1],fY0[iz+1], fScale[iz+1]);
         return fPoly->Contains(point);
      }      
   }      
   xtru->SetCurrentZ(point[2], iz);
   // Now fXc,fYc represent the vertices of the section at point[2]
   return fPoly->Contains(point);
}

//_____________________________________________________________________________
Int_t TGeoXtru::DistancetoPrimitive(Int_t px, Int_t py)
{
// compute closest distance from point px,py to each corner
   const Int_t numPoints = fNvert*fNz;
   return ShapeDistancetoPrimitive(numPoints, px, py);
}
//_____________________________________________________________________________
Double_t TGeoXtru::DistToPlane(Double_t *point, Double_t *dir, Int_t iz, Int_t ivert, Double_t stepmax) const
{
// Compute distance to a Xtru lateral surface.
   Double_t snext;
   Double_t vert[12];
   Double_t norm[3];
   Double_t znew;
   Double_t pt[3];
   GetPlaneVertices(iz, ivert, vert);
   GetPlaneNormal(vert, norm);
   Double_t ndotd = norm[0]*dir[0]+norm[1]*dir[1]+norm[2]*dir[2];
   if (ndotd<=0) return TGeoShape::Big();
   Double_t safe = (vert[0]-point[0])*norm[0]+(vert[1]-point[1])*norm[1]+
                   (vert[2]-point[2])*norm[2];
   if (safe<0) return TGeoShape::Big(); // direction outwards plane
   snext = safe/ndotd;
   if (snext>stepmax) return TGeoShape::Big();
   if (fZ[iz]>fZ[iz+1]) {
      znew = point[2] + snext*dir[2];
      if (znew<fZ[iz]) return TGeoShape::Big();
      if (znew>fZ[iz+1]) return TGeoShape::Big();
   }
   pt[0] = point[0]+snext*dir[0];
   pt[1] = point[1]+snext*dir[1];
   pt[2] = point[2]+snext*dir[2];
   if (IsPointInsidePlane(pt, vert, norm)) return snext;
   return TGeoShape::Big();         
}

//_____________________________________________________________________________
Double_t TGeoXtru::DistToOut(Double_t *point, Double_t *dir, Int_t iact, Double_t step, Double_t *safe) const
{
// compute distance from inside point to surface of the polycone
   // locate Z segment
   if (iact<3 && safe) {
      *safe = Safety(point, kTRUE);
      if (iact==0) return TGeoShape::Big();
      if (iact==1 && step<*safe) return TGeoShape::Big();
   }   
   Int_t iz = TMath::BinarySearch(fNz, fZ, point[2]);
   if (iz==fNz-1) {
      if (dir[2]>=0) return 0.;
      iz--;
   } else {   
      if (iz>0) {
         if (fZ[iz]==fZ[iz-1]) iz--;
      }
   }   
   TGeoXtru *xtru = (TGeoXtru*)this;
   Bool_t convex = fPoly->IsConvex();
   Double_t stepmax = step;
   Double_t snext = TGeoShape::Big();
   Double_t dist, sz;
   Double_t pt[3];
   Bool_t lookZ = kTRUE;
   Int_t iv, ipl, inext;
   Int_t incseg = (dir[2]>0)?1:-1;   
   // we treat the special case when dir[2]=0
   if (dir[2]==0) {
      for (iv=0; iv<fNvert; iv++) {
         dist = DistToPlane(point,dir,iz,iv,stepmax);
         if (dist<stepmax) {
            stepmax = dist;
            snext = dist;
            if (convex) return snext;
         }   
      }
      return snext;
   }      
   
   // normal case   
   while (lookZ && (iz>=0) && (iz<fNz-1)) {
      // find the distance  to current segment end Z surface
      ipl = iz+((incseg+1)>>1);
      inext = ipl+incseg;
      sz = (fZ[ipl]-point[2])/dir[2];
      if (sz<stepmax) {
         // we cross the next Z section before stepmax
         pt[0] = point[0]+sz*dir[0];
         pt[1] = point[1]+sz*dir[1];
         xtru->SetCurrentVertices(fX0[ipl],fY0[ipl],fScale[ipl]);
         if (!fPoly->Contains(pt)) {
            // no polygon crossing, so we do cross the lateral surfaces
            snext = TGeoShape::Big();
            stepmax = sz;
            lookZ = kFALSE;
         } else {  
            // we cross also the polygon, so there is no lateral crossing
            // except the case when we have 2 planes at same Z
            if (inext>0 && inext<fNz-1) {
               if (fZ[ipl]==fZ[inext]) {
                  // we have to check lateral crossing with min(ipl,inext)
                  Int_t imin = TMath::Min(ipl,inext);
                  for (iv=0; iv<fNvert; iv++) {
                     dist = DistToPlane(point,dir,imin,iv,stepmax);
                     if (dist<stepmax) return dist;
                  }
               }
            }            
            iz += incseg;
            snext = sz; // crossing but might be virtual
            continue;
         }   
      } else {
        // next Z section further than stepmax
        lookZ = kFALSE;
      }
      // check lateral faces of current segment up to stepmax
      for (iv=0; iv<fNvert; iv++) {
         dist = DistToPlane(point,dir,iz,iv,stepmax); 
         if (dist<stepmax) {
            stepmax = dist;
            snext = dist;
            if (convex) return snext;
         }   
      }   
   }
   return snext;  
}

//_____________________________________________________________________________
Double_t TGeoXtru::DistToIn(Double_t *point, Double_t *dir, Int_t iact, Double_t step, Double_t *safe) const
{
// compute distance from outside point to surface of the tube
//   Warning("DistToIn", "not implemented");
   if (iact<3 && safe) {
      *safe = Safety(point, kTRUE);
      if (iact==0) return TGeoShape::Big();
      if (iact==1 && step<*safe) return TGeoShape::Big();
   }   
   Double_t stepmax = step;
   Double_t snext = TGeoShape::Big();
   // We might get out easy with Z checks
   Int_t iz = TMath::BinarySearch(fNz, fZ, point[2]);
   if (iz<0) {
      if (dir[2]<0) return TGeoShape::Big();
      iz=0; // valid starting value
   } else {
      if (iz==fNz-1) {
         if (dir[2]>=0) return TGeoShape::Big();
         iz = fNz-2; // valid value = last segment
      }
   }      
   // Check if the bounding box is missed by the track
   if (!TGeoBBox::Contains(point)) {
      snext = TGeoBBox::DistToIn(point,dir,3);
      if (snext>stepmax) return TGeoShape::Big();
   }   
   // not the case - we have to do some work...
//   TGeoXtru *xtru = (TGeoXtru*)this;
   
   return snext;  
}

//_____________________________________________________________________________
Bool_t TGeoXtru::DefinePolygon(Int_t nvert, const Double_t *xv, const Double_t *yv)
{
// Creates the polygon representing the blueprint of any Xtru section.
//   nvert     = number of vertices >2
//   xv[nvert] = array of X vertex positions 
//   yv[nvert] = array of Y vertex positions 
// *NOTE* should be called before DefineSection or ctor with 'param'
   if (nvert<3) {
      Error("DefinePolygon","cannot create polygon with less than 3 vertices");
      return kFALSE;
   }
   fNvert = nvert;
   if (fX) delete [] fX;
   fX = new Double_t[nvert];
   if (fY) delete [] fY;
   fY = new Double_t[nvert];
   if (fXc) delete [] fXc;
   fXc = new Double_t[nvert];
   if (fYc) delete [] fYc;
   fYc = new Double_t[nvert];
   memcpy(fX,xv,nvert*sizeof(Double_t));
   memcpy(fXc,xv,nvert*sizeof(Double_t));
   memcpy(fY,yv,nvert*sizeof(Double_t));
   memcpy(fYc,yv,nvert*sizeof(Double_t));
   
   if (fPoly) delete fPoly;
   fPoly = new TGeoPolygon(nvert);
   fPoly->SetXY(fXc,fYc); // initialize with current coordinates
   fPoly->FinishPolygon();
   return kTRUE;
}

//_____________________________________________________________________________
void TGeoXtru::DefineSection(Int_t snum, Double_t z, Double_t x0, Double_t y0, Double_t scale)
{
// defines z position of a section plane, rmin and rmax at this z.
   if ((snum<0) || (snum>=fNz)) return;
   fZ[snum]  = z;
   fX0[snum] = x0;
   fY0[snum] = y0;
   fScale[snum] = scale;
   if (snum) {
      if (fZ[snum]<fZ[snum-1]) {
         Warning("DefineSection", "Z position of section %i not in increasing order",snum);
         return;
      }   
   }
   if (snum==(fNz-1)) ComputeBBox();
}
            
//_____________________________________________________________________________
Double_t TGeoXtru::GetZ(Int_t ipl) const
{
   if (ipl<0 || ipl>(fNz-1)) {
      Error("GetZ","ipl=%i out of range (0,%i)",ipl,0,fNz-1);
      return 0.;
   }
   return fZ[ipl];
}      
//_____________________________________________________________________________
void TGeoXtru::GetPlaneNormal(const Double_t *vert, Double_t *norm) const
{
// Returns normal vector to the planar quadrilateral defined by vector VERT.
   Double_t cross = 0.;
   Double_t v1[3], v2[3];
   v1[0] = vert[9]-vert[0];
   v1[1] = vert[10]-vert[1];
   v1[2] = vert[11]-vert[2];
   v2[0] = vert[3]-vert[0];
   v2[1] = vert[4]-vert[1];
   v2[2] = vert[5]-vert[2];
   norm[0] = v1[1]*v2[2]-v1[2]*v2[1];
   cross += norm[0]*norm[0];
   norm[1] = v1[2]*v2[0]-v1[0]*v2[2];
   cross += norm[1]*norm[1];
   norm[2] = v1[0]*v2[1]-v1[1]*v2[0];
   cross += norm[2]*norm[2];
   cross = 1./TMath::Sqrt(cross);
   for (Int_t i=0; i<3; i++) norm[i] *= cross;
}   

//_____________________________________________________________________________
void TGeoXtru::GetPlaneVertices(Int_t iz, Int_t ivert, Double_t *vert) const
{
// Returns (x,y,z) of 3 vertices of the surface defined by Z sections (iz, iz+1)
// and polygon vertices (ivert, ivert+1). No range check.
   Double_t x,y,z1,z2;
   Int_t iv1 = (ivert+1)%fNvert;
   Int_t icrt = 0;
   z1 = fZ[iz];
   z2 = fZ[iz+1];
   x = fX[ivert]*fScale[iz]+fX0[iz];
   y = fY[ivert]*fScale[iz]+fY0[iz];
   vert[icrt++] = x;
   vert[icrt++] = y;
   vert[icrt++] = z1;
   x = fX[iv1]*fScale[iz]+fX0[iz];
   y = fY[iv1]*fScale[iz]+fY0[iz];
   vert[icrt++] = x;
   vert[icrt++] = y;
   vert[icrt++] = z1;
   x = fX[iv1]*fScale[iz+1]+fX0[iz+1];
   y = fY[iv1]*fScale[iz+1]+fY0[iz+1];
   vert[icrt++] = x;
   vert[icrt++] = y;
   vert[icrt++] = z2;
   x = fX[ivert]*fScale[iz+1]+fX0[iz+1];
   y = fY[ivert]*fScale[iz+1]+fY0[iz+1];
   vert[icrt++] = x;
   vert[icrt++] = y;
   vert[icrt++] = z2;
}
//_____________________________________________________________________________
Bool_t TGeoXtru::IsPointInsidePlane(Double_t *point, Double_t *vert, Double_t *norm) const
{
// Check if the quadrilateral defined by VERT contains a coplanar POINT.
   Double_t v1[3], v2[3];
   Double_t cross;
   Int_t j,k;
   for (Int_t i=0; i<4; i++) { // loop vertices
      j = 3*i;
      k = 3*((i+1)%4);
      v1[0] = point[0]-vert[j];
      v1[1] = point[1]-vert[j+1];
      v1[2] = point[2]-vert[j+2];
      v2[0] = vert[k]-vert[j];
      v2[1] = vert[k+1]-vert[j+1];
      v2[2] = vert[k+2]-vert[j+2];
      cross = (v1[1]*v2[2]-v1[2]*v2[1])*norm[0]+
              (v1[2]*v2[0]-v1[0]*v2[2])*norm[1]+
              (v1[0]*v2[1]-v1[1]*v2[0])*norm[2];
      if (cross<0) return kFALSE;
   }
   return kTRUE;   
}

//_____________________________________________________________________________
void TGeoXtru::InspectShape() const
{
// Print actual Xtru parameters.
   printf("*** Shape %s: TGeoXtru ***\n", GetName());
   printf("    Nz    = %i\n", fNz);
   printf("    List of (x,y) of polygon vertices:\n");
   for (Int_t ivert = 0; ivert<fNvert; ivert++)
      printf("    x = %11.5f  y = %11.5f\n", fX[ivert],fY[ivert]);
   for (Int_t ipl=0; ipl<fNz; ipl++)
      printf("     plane %i: z=%11.5f x0=%11.5f y0=%11.5f scale=%11.5f\n", ipl, fZ[ipl], fX0[ipl], fY0[ipl], fScale[ipl]);
   printf(" Bounding box:\n");
   TGeoBBox::InspectShape();
}

//_____________________________________________________________________________
void *TGeoXtru::Make3DBuffer(const TGeoVolume *vol) const
{
   TVirtualGeoPainter *painter = gGeoManager->GetGeomPainter();
   if (!painter) return 0;
   return painter->MakeXtru3DBuffer(vol);
}   

//_____________________________________________________________________________
void TGeoXtru::Paint(Option_t *option)
{
// paint this shape according to option
   TVirtualGeoPainter *painter = gGeoManager->GetGeomPainter();
   if (!painter) return;
   TGeoVolume *vol = gGeoManager->GetCurrentVolume();
   if (vol->GetShape() != (TGeoShape*)this) return;
   painter->PaintXtru(this, option);
}

//_____________________________________________________________________________
void TGeoXtru::PaintNext(TGeoHMatrix *glmat, Option_t *option)
{
// paint this shape according to option
   TVirtualGeoPainter *painter = gGeoManager->GetGeomPainter();
   if (!painter) return;
   painter->PaintXtru(this, option, glmat);
}

//_____________________________________________________________________________
Double_t TGeoXtru::Safety(Double_t * /*point*/, Bool_t /*in*/) const
{
// computes the closest distance from given point to this shape, according
// to option. The matching point on the shape is stored in spoint.
   //---> localize the Z segment
   Warning("Safety", "not implemented");
   return TGeoShape::Big();  
}

//_____________________________________________________________________________
void TGeoXtru::SetCurrentZ(Double_t z, Int_t iz)
{
// Recompute current section vertices for a given Z position within range of section iz.
   Double_t x0, y0, scale, a, b;
   Int_t ind1, ind2;
   ind1 = iz;
   ind2 = iz+1;   
   Double_t invdz = 1./(fZ[ind2]-fZ[ind1]);
   a = (fX0[ind1]*fZ[ind2]-fX0[ind2]*fZ[ind1])*invdz;
   b = (fX0[ind2]-fX0[ind1])*invdz;
   x0 = a+b*z;
   a = (fY0[ind1]*fZ[ind2]-fY0[ind2]*fZ[ind1])*invdz;
   b = (fY0[ind2]-fY0[ind1])*invdz;
   y0 = a+b*z;
   a = (fScale[ind1]*fZ[ind2]-fScale[ind2]*fZ[ind1])*invdz;
   b = (fScale[ind2]-fScale[ind1])*invdz;
   scale = a+b*z;
   SetCurrentVertices(x0,y0,scale);
}
      
//_____________________________________________________________________________
void TGeoXtru::SetCurrentVertices(Double_t x0, Double_t y0, Double_t scale)      
{
// Set current vertex coordinates according X0, Y0 and SCALE.
   for (Int_t i=0; i<fNvert; i++) {
      fXc[i] = scale*fX[i] + x0;
      fYc[i] = scale*fY[i] + y0;
   }   
}

//_____________________________________________________________________________
void TGeoXtru::SetDimensions(Double_t *param)
{
// param[0] = nz  // number of z planes
//
// param[1] = z1  // Z position of first plane
// param[2] = x1  // X position of first plane
// param[3] = y1  // Y position of first plane
// param[4] = scale1  // scale factor for first plane
// ...
// param[4*(nz-1]+1] = zn
// param[4*(nz-1)+2] = xn
// param[4*(nz-1)+3] = yn
// param[4*(nz-1)+4] = scalen
   fNz = (Int_t)param[0];   
   if (fNz<2) {
      Error("SetDimensions","Cannot create TGeoXtru with less than 2 Z planes");
      return;
   }
   if (fZ) delete [] fZ;
   if (fScale) delete [] fScale;
   if (fX0) delete [] fX0;
   if (fY0) delete [] fY0;
   fZ = new Double_t[fNz];
   fScale = new Double_t[fNz];
   fX0 = new Double_t[fNz];
   fY0 = new Double_t[fNz];
   
   for (Int_t i=0; i<fNz; i++) 
      DefineSection(i, param[1+4*i], param[2+4*i], param[3+4*i], param[4+4*i]);
}   

//_____________________________________________________________________________
void TGeoXtru::SetPoints(Double_t *buff) const
{
// create polycone mesh points
   Int_t i, j;
   Int_t indx = 0;
   TGeoXtru *xtru = (TGeoXtru*)this;
   if (buff) {
      for (i = 0; i < fNz; i++) {
         xtru->SetCurrentVertices(fX0[i], fY0[i], fScale[i]);
         for (j = 0; j < fNvert; j++) {
            buff[indx++] = fXc[j];
            buff[indx++] = fYc[j];
            buff[indx++] = fZ[i];
         }
      }
   }
}

//_____________________________________________________________________________
void TGeoXtru::SetPoints(Float_t *buff) const
{
// create polycone mesh points
   Int_t i, j;
   Int_t indx = 0;
   TGeoXtru *xtru = (TGeoXtru*)this;
   if (buff) {
      for (i = 0; i < fNz; i++) {
         xtru->SetCurrentVertices(fX0[i], fY0[i], fScale[i]);
         for (j = 0; j < fNvert; j++) {
            buff[indx++] = fXc[j];
            buff[indx++] = fYc[j];
            buff[indx++] = fZ[i];
         }
      }
   }
}

//_____________________________________________________________________________
void TGeoXtru::Sizeof3D() const
{
// fill size of this 3-D object
   TVirtualGeoPainter *painter = gGeoManager->GetGeomPainter();
   if (!painter) return;

   Int_t numPoints = fNz*fNvert;
   Int_t numSegs   = fNvert*(2*fNz-1);
   Int_t numPolys  = fNvert*(fNz-1)+2;
   painter->AddSize3D(numPoints, numSegs, numPolys);
}

