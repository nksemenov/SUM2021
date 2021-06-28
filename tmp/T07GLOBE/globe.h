/* FILE NAME: T07GLOBE.c
 * PROGRAMMER: NM6
 * DATE: 14.06.2021
 * PURPOSE: WinAPI application sample.
 * Globe.h  
 */
#define __globe_h_
#ifdef __globe_h_


#include <windows.h>

#include "timer.h"
#include "mth.h"

#define GRID_H 20
#define GRID_W 20
#pragma warning(disable: 4244)

/*typedef DOUBLE DBL*/

VOID GlobeSet( DBL W, DBL H, DBL R );
VOID GlobeDraw( HDC hDC );
VEC RotateZ( VEC V, DBL Angle); 

#endif