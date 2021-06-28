/* FILE NAME: T07GLOBE.c
* PROGRAMMER: NM6
* DATE: 14.06.2021
* PURPOSE: WinAPI application sample.
* Globe.c
*/
#include <time.h>
#include <windows.h>
#include <math.h>

#include "globe.h"
/* Globe geometry data */
VEC Geom[GRID_H][GRID_W];

static INT WinW, WinH;

VEC RotateZ( VEC V, DBL Angle)
{
  DBL pi = 3.14159265359, a = Angle * pi / 180, si = sin(a), co = cos(a);
  VEC r;

  r.X = V.X * co + V.Y * si;
  r.Y = V.Y * co - V.X * si;
  r.Z = V.Z;
  return r;
}

VEC RotateY( VEC V, DBL Angle)
{
  DBL pi = 3.14159265359, a = Angle * pi / 180, si = sin(a), co = cos(a);
  VEC r;

  r.Z = V.Z * co + V.X * si;
  r.X = V.Z * si + V.X * co;
  r.Y = V.Y;
  return r;
}


VOID GlobeSet( DBL W, DBL H, DBL R )
{
  INT i, j;
  DOUBLE pi = 3.14159265359;
  DBL phi, theta;

  /* Store window save */
  WinW = (INT)W;
  WinH = (INT)H;
  for (i = 0, theta = 0; i < GRID_H; i++, theta += pi / (GRID_H - 1))
    for (j = 0, phi = 0; j < GRID_W; j++, phi += 2 * pi / (GRID_W - 1))
    {
      Geom[i][j].X = R * sin(theta) * sin(phi);
      Geom[i][j].Y = R * cos(theta);
      Geom[i][j].Z = R * sin(theta) * cos(phi);
    }
} /* End of 'GlobeSet' function */

VOID GlobeDraw( HDC hDC )
{
  INT i, j, r, s = 2;      
  DBL t = GLB_Time, S;
  MATR m;
  static POINT pnts[GRID_H][GRID_W];
  
  

  /*
  MART WorldMatr;
  WorldMatr = MatrMullMatr4(MatrScale(VecSet(0.5, 0.5, 0.5)),
                            MatrRotateZ(45 * DLB_Time)
                            MatrRotate(90 * DLB_Time, VecSet1(1)),
                            MatrTranslate(VecSet(c, s, 0)));
  WorldMatr = MartNullMart(WorldMatr,
                           MartView(VecSet(3, 0, 0)
                                    VecSet(0, 0, 0)
                                    VecSet(0, 1, 0)));
  projDist = size;
  Wp = Hp = size;
  if (WinW < WinH)
    Wp *= (DBL)WinW / WinH;
  else
  */



  r = WinW < WinH ? WinW :WinH;

  m = 
    MatrMulMatr3(MatrRotateZ(t * 30),
                 MatrRotateZ(t * 18),
                 MatrTranslate(VecSet(0, fabs(0.8 * sin(t * 5) - 0.47), 0)));

  for (i = 0; i < GRID_H; i++)
    for (j = 0; j < GRID_W; j++)
    {
      VEC p = Geom[i][j];

      /* Point transformation */
      p = PointTransform(p, m);
                                                      
      p = RotateZ(p, t * 30);
      p = RotateY(p, t * 100);

      pnts[i][j].x = WinW / 2 + p.X * r * 0.47;
      pnts[i][j].y = WinH / 2 + p.Y * r * 0.47;
    }
  /* Draw all points 
  for (i = 0; i < GRID_H; i++)
    for (j = 0; j < GRID_W; j++)
      Ellipse(hDC, pnts[i][j].x - s, pnts[i][j].y - s, pnts[i][j].x + s, pnts[i][j].y + s);
  */
#if 0
  /* Draw by lines */
  /* H lines*/
  for (i = 0; i < GRID_H; i++)
  {
    MoveToEx(hDC, pnts[i][0].x, pnts[i][0].y, NULL);
    for (j = 1; j < GRID_W; j++)
      LineTo(hDC, pnts[i][j].x, pnts[i][j].y);
  }
  /* W lines*/
  for (j = 1; j < GRID_W; j++)
  {
    MoveToEx(hDC, pnts[0][j].x, pnts[0][j].y, NULL);
    for (i = 0; i < GRID_H; i++)
      LineTo(hDC, pnts[i][j].x, pnts[i][j].y);
  }
#endif
  for (i = 0; i < GRID_H - 1; i++)
    for (j = 0; j < GRID_W - 1; j++)
    {               
      POINT pts[4];
      if (j < (GRID_H - 1) / 2)
      {
        SelectObject(hDC, GetStockObject(DC_BRUSH));
        SetDCBrushColor(hDC, RGB(0, 0, 255));
      }
      else
      {
        SelectObject(hDC, GetStockObject(DC_BRUSH));
        SetDCBrushColor(hDC, RGB(255, 255, 0));
      }
      
      
      pts[0] = pnts[i][j];
      pts[1] = pnts[i][j + 1];
      pts[2] = pnts[i + 1][j + 1];
      pts[3] = pnts[i + 1][j];

      S = (pts[0].x - pts[1].x) * (pts[0].y + pts[1].y) / 2 +
          (pts[1].x - pts[2].x) * (pts[1].y + pts[2].y) / 2 +
          (pts[2].x - pts[3].x) * (pts[2].y + pts[3].y) / 2 +
          (pts[3].x - pts[0].x) * (pts[3].y + pts[0].y) / 2;

      if (S > 0)
      {
        Polygon(hDC, pts, 4);
      }
      /*SelectObject(hDC, GetStockObject(DC_BRUSH));
      SetDCBrushColor(hDC, RGB(228, 68, 150)); */
    }
} /* End of 'GlobeDraw' function */ 