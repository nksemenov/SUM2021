#ifndef __rnd_h_
#define __rnd_h_

#include "../../def.h"

extern HWND NS6_hRndWnd;
extern HDC NS6_hRndDCFrame;
extern HBITMAP NS6_hRndBmFrame;
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

typedef struct tgns6VERTEX
{
  VEC P;
} ns6VERTEX;

typedef struct tgns6PRIM
{
  ns6VERTEX *V;
  INT NumOfV;

  INT *I;
  INT NumOfI;

  MATR Trans;
} ns6PRIM;

BOOL NS6_RndPrimCreate( ns6PRIM *Pr, INT NoofV, INT NoofI);

VOID NS6_RndPrimFree( ns6PRIM *Pr);

VOID NS6_RndPrimDraw( ns6PRIM *Pr, MATR World );

VOID NS6_RndPrimCreateSphere( ns6PRIM *Pr, VEC C, DBL R, INT SplitW, INT SplitH );

VOID NS6_RndPrimCreatePlane( ns6PRIM *Pr, VEC P, VEC W, VEC L, INT SplitW, INT SplitH );

BOOL NS6_RndPrimLoad( ns6PRIM *Pr, CHAR *FileName );

#endif