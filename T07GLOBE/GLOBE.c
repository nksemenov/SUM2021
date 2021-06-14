/* FILENAME: T04PERM.c
 * PROGRAMMER: NS6
 * DATE: 12.06.2021
 * PURPOSE: WinAPI Clock drawing application sample.
 */
#include "GLOBE.h"

VEC Geom[GRID_H][GRID_W];

VOID GlobeSet( DOUBLE R )
{
  INT i, j;
  DOUBLE phi, theta, pi = 3.14159265558979323846;

  for (i = 0, theta = 0; i < GRID_H; i++, theta += pi / (GRID_H - 1))
    for (j = 0, phi = 0; j < GRID_W; j++, phi += 2 * pi / (GRID_W - 1))
    {
      Geom[i][j].x = R * sin(theta)* sin(phi);
      Geom[i][j].y = R * cos(theta);
      Geom[i][j].z = R * sin(theta) * cos(phi);
    }
}

VOID GlobeDraw( HDC hDC )
{
  INT i, j;
  for (i = 0; i < GRID_H; i++)
    for (j = 0; j < GRID_W; j++)
    {
      MoveToEx(hDC, Geom[i][j].x, Geom[i][j].y, NULL);
      LineTO(hDC, Geom[i + 1][j].x, Geom[i + 1][j].y);
      MoveToEx(hDC, Geom[i + 1][j].x, Geom[i + 1][j].y, NULL);
      LineTO(hDC, Geom[i + 1][j + 1].x, Geom[i + 1][j + 1].y);
      MoveToEx(hDC, Geom[i + 1][j + 1].x, Geom[i + 1][j + 1].y, NULL);
      LineTO(hDC, Geom[i][j + 1].x, Geom[i][j + 1].y);
      MoveToEx(hDC, Geom[i][j + 1].x, Geom[i][j + 1].y, NULL);
      LineTO(hDC, Geom[i][j].x, Geom[i][j].y);
    }
}