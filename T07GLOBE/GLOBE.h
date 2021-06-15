/* FILENAME: T04PERM.c
 * PROGRAMMER: NS6
 * DATE: 12.06.2021
 * PURPOSE: WinAPI Clock drawing application sample.
 */

#include <stdio.h>
#include <windows.h>
#include <math.h>

typedef struct tagVEC
{
  DOUBLE x, y, z;
} VEC;

VOID GlobeSet( DOUBLE R );
VOID GlobeDraw( HDC hDC, INT W, INT H);
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );