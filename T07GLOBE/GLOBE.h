/* FILENAME: globe.h
 * PROGRAMMER: NS6
 * DATE: 12.06.2021
 * PURPOSE: WinAPI Clock drawing application sample.
 */

#ifndef __globe_h_
#define __globe_h_

#pragma warning(disable: 4244)

#include <windows.h>

#include "timer.h"
#include "mth.h"

VOID GlobeSet( DOUBLE R );
VOID GlobeDraw( HDC hDC, INT W, INT H);
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

#endif /* __globe_h_ */

/* END OF 'globe.h' FILE */
