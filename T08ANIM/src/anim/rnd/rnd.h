#ifndef __rnd_h_
#define __rnd_h_

#include "../src/def.h"

extern HWND NS6_hRndWnd;
extern HDC NS6_hRndDCFrame;
extern BITMAP NS6_hRndBmFrame;
extern INT NS6_RndFrameW, NS6_RndFrameH;

extern DBL
  NS6_RndProjSize,
  NS6_RndProjDist,
  NS6_RndProjFarClip;

extern MATR
  NS6_RndMatrView,
  NS6_RndMatrProj,
  NS6_RndMatrVP;

VOID NS6_RndInit( HWND hWnd );

VOID NS6_RndClose( VOID );

VOID NS6_RndResize( INT W, INT H );

VOID NS6_RndCopyFrame( HDC hDC );

VOID NS6_RndStart( VOID );

VOID NS6_RndEnd( VOID );

VOID NS6_RndProjSet( VOID );

VOID NS6_RndCamSet( VEC Loc, VEC At, VEC Up );

#endif