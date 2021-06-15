/* FILENAME: T04PERM.c
 * PROGRAMMER: NS6
 * DATE: 12.06.2021
 * PURPOSE: WinAPI Clock drawing application sample.
 */
#include "GLOBE.h"
#include <time.h>

#define GRID_H 25
#define GRID_W 25
#define pi 3.14159265558979323846

SYSTEMTIME st;

static VEC Geom[GRID_H][GRID_W];

VEC RotateZ( VEC V, DOUBLE angle )
{
  DOUBLE a = angle * pi / 180;
  VEC r;
  
  r.x = V.x * cos(a) + V.y * sin(a);
  r.y = V.y * cos(a) - V.x * sin(a);
  r.z = V.z;
  return r;
}

VEC RotateY( VEC V, DOUBLE angle )
{
  DOUBLE a = angle * pi / 180;
  VEC r;
  
  r.y = V.y * cos(a) + V.z * sin(a);
  r.z = V.z * cos(a) - V.y * sin(a);
  r.x = V.x;
  return r;
} 

VEC RotateX( VEC V, DOUBLE angle )
{
  DOUBLE a = angle * pi / 180;
  VEC r;
  
  r.z = V.z * cos(a) + V.x * sin(a);
  r.x = V.x * cos(a) - V.z * sin(a);
  r.y = V.y;
  return r;
} 

VOID GlobeSet( DOUBLE R )
{
  INT i, j;
  DOUBLE phi, theta;

  for (i = 0, theta = 0; i < GRID_H; i++, theta += pi / (GRID_H - 1))
    for (j = 0, phi = 0; j < GRID_W; j++, phi += 2 * pi / (GRID_W - 1))
    {
      Geom[i][j].x = R * sin(theta)* sin(phi);
      Geom[i][j].y = R * cos(theta);
      Geom[i][j].z = R * sin(theta) * cos(phi);
    }
}

VOID GlobeDraw( HDC hDC, INT W, INT H )
{
  INT i, j, s = 2, r, WinW, WinH;
  POINT pnts[GRID_H][GRID_W], p[4];
  DOUBLE t = clock() / (DOUBLE)CLOCKS_PER_SEC, S;

  WinW = W;
  WinH = H;
  
  r = WinW < WinH ? WinW : WinH;                              

  GetLocalTime(&st);

  for (i = 0; i < GRID_H; i++)
    for (j = 0; j < GRID_W; j++)
    {
      VEC p = Geom[i][j];

      p = RotateZ(p, t * 50);
      p = RotateY(p, t * 50);
      p = RotateX(p, t * 50);

      pnts[i][j].x = WinW / 2 + p.x * r * 0.3;
      pnts[i][j].y = WinH / 2 - p.y * r * 0.3 + 150 - 200 * fabs(sin(t * 5) );
    }
  
  /*for (i = 0; i < GRID_H; i++)
  {
    MoveToEx(hDC, pnts[i][0].x, pnts[i][0].y, NULL);
    for (j = 0; j < GRID_W; j++)
      LineTo(hDC, pnts[i][j].x, pnts[i][j].y);
  }
  
  for (i = 0; i < GRID_H - 1; i++)
    for (j = 0; j < GRID_W; j++)
    {
      MoveToEx(hDC, pnts[i][j].x, pnts[i][j].y, NULL);
      LineTo(hDC, pnts[i + 1][j].x, pnts[i + 1][j].y);
    }
   */
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
      SetDCBrushColor(hDC, RGB(150, 150, 150));
    }
}