/* FILENAME: T04PERM.c
 * PROGRAMMER: NS6
 * DATE: 12.06.2021
 * PURPOSE: WinAPI Clock drawing application sample.
 */
#include <time.h>
#include "GLOBE.h"
#include "timer.h"
#include "mth.h"

#define GRID_H 25
#define GRID_W 25
#define pi 3.14159265558979323846

SYSTEMTIME st;

static VEC Geom[GRID_H][GRID_W];

VEC RotateZ( VEC V, DOUBLE angle )
{
  DOUBLE a = angle * pi / 180;
  VEC r;
  
  r.X = V.X * cos(a) + V.Y * sin(a);
  r.Y = V.Y * cos(a) - V.X * sin(a);
  r.Z = V.Z;
  return r;
}

VEC RotateY( VEC V, DOUBLE angle )
{
  DOUBLE a = angle * pi / 180;
  VEC r;
  
  r.Y = V.Y * cos(a) + V.Z * sin(a);
  r.Z = V.Z * cos(a) - V.Y * sin(a);
  r.X = V.X;
  return r;
} 

VEC RotateX( VEC V, DOUBLE angle )
{
  DOUBLE a = angle * pi / 180;
  VEC r;
  
  r.Z = V.Z * cos(a) + V.X * sin(a);
  r.X = V.X * cos(a) - V.Z * sin(a);
  r.Y = V.Y;
  return r;
} 

VOID GlobeSet( DOUBLE R )
{
  INT i, j;
  DOUBLE phi, theta;

  for (i = 0, theta = 0; i < GRID_H; i++, theta += pi / (GRID_H - 1))
    for (j = 0, phi = 0; j < GRID_W; j++, phi += 2 * pi / (GRID_W - 1))
    {
      Geom[i][j].X = R * sin(theta)* sin(phi);
      Geom[i][j].Y = R * cos(theta);
      Geom[i][j].Z = R * sin(theta) * cos(phi);
    }
}

VOID GlobeDraw( HDC hDC, INT W, INT H )
{
  INT i, j, s = 2, r, WinW, WinH;
  POINT pnts[GRID_H][GRID_W], p[4];
  DOUBLE t = GLB_Time, S;
  MATR m /*= MatrMulMatr(MatrMulMatr(MatrRotateX(GLB_Time * 50), MatrRotateY(GLB_Time * 50)), MatrRotate(sin(GLB_Time ) * 8, VecSet(1, 1, 1)))*/;
  DOUBLE Size = 2, Wp, Hp, ProjDist;

  WinW = W;
  WinH = H;
  
  Wp = Hp = Size;
  if (WinW > WinH)
    Wp *= (DBL) WinW / WinH;
  else
    Hp *= (DBL) WinH / WinW;
  ProjDist = Size;

  r = WinW < WinH ? WinW : WinH;
  
 /* m = MatrMulMatr(MatrMulMatr(MatrRotateZ(t * 30), MatrRotateY(t * 15)), MatrTranslate(VecSet(0, fabs(1.5 * sin(t * 3)) - 0.47, 0)));  */
  m = MatrIdentity();
  m = MatrView(VecSet(0, sin(GLB_Time), 5), VecSet(0, 0, 0), VecSet(0, 1, 0));
  GetLocalTime(&st);

  for (i = 0; i < GRID_H; i++)
    for (j = 0; j < GRID_W; j++)
    {
      VEC p = Geom[i][j];
      VEC v = PointTransform(Geom[i][j], m);

      p = RotateZ(p, t * 50);
      p = RotateY(p, t * 50);
      p = RotateX(p, t * 50);

      //pnts[i][j].x = WinW / 2 + p.X * r * 0.3;
      //pnts[i][j].y = WinH / 2 - p.Y * r * 0.3 + 150 - 200 * fabs(sin(t * 5) );
      pnts[i][j].x = WinW / 2 + v.X * r * 0.1;
      pnts[i][j].y = WinH / 2 - v.Y * r * 0.1;
    }
  
  srand(30);
  SelectObject(hDC, GetStockObject(DC_PEN));
  SetDCPenColor(hDC, RGB(255, 255, 255));
  for (i = 0; i < GRID_H - 1; i++)
    for (j = 0; j < GRID_W - 1; j++)
    {
      
      SelectObject(hDC, GetStockObject(DC_BRUSH));
      SetDCBrushColor(hDC, RGB(rand(), rand(), rand()));
      
      p[0] = pnts[i][j];
      p[1] = pnts[i][j + 1];
      p[2] = pnts[i + 1][j + 1];
      p[3] = pnts[i + 1][j];
      
      S = (p[0].x - p[1].x) * (p[0].y + p[1].y) / 2 +
          (p[1].x - p[2].x) * (p[1].y + p[2].y) / 2 +
          (p[2].x - p[3].x) * (p[2].y + p[3].y) / 2 +
          (p[3].x - p[0].x) * (p[3].y + p[0].y) / 2;

      if (S > 0)
      {
        Polygon(hDC, p, 4);
      }

      SelectObject(hDC, GetStockObject(DC_BRUSH));
      SetDCBrushColor(hDC, RGB(228, 68, 150));
    }
}