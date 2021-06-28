/* FILE NAME: rnd.h
* PROGRAMMER: NM6
* DATE: 17.06.2021
* PURPOSE: 3D animation rendering declaration module.
*/

#ifndef __rnd_h_
#define __rnd_h_

#include "../../def.h"

extern HWND NM6_hRndWnd; /* Work window handle */
extern HDC NM6_hRndDCFrame; /* Work window memory device context */
extern HBITMAP NM6_hRndBmFrame; /* Work window background bitmap handle */
extern INT NM6_RndFrameW, NM6_RndFrameH; /* Work window size */

extern DBL
  NM6_RndProjSize, /* Project plane fit square */
  NM6_RndProjDist, /* Distance to project plane from viewer (near) */
  NM6_RndProjFarClip; /* Distance to project far clip plane (far) */

extern MATR
  NM6_RndMatrView, /* View coordinate system matrix */
  NM6_RndMatrProj, /* Projection coordinate system matrix */
  NM6_RndMatrVP; /* Stored (View * Proj) matrix */

/* vertex representation type */
typedef struct tagnm6VERTEX
{
  VEC P;
} nm6VERTEX;

/* primitive representation type */
typedef struct tagnm6PRIM
{
  nm6VERTEX *V;
  INT NumOfV;

  INT *I;
  INT NumOfI;

  MATR Trans;
} nm6PRIM;

/* Render base functions */
VOID NM6_RndInit( HWND hWnd );
VOID NM6_RndClose( VOID );
VOID NM6_RndCopyFrame( HDC hDC );
VOID NM6_RndProjSet( VOID );
VOID NM6_RndCamSet( VEC Loc, VEC At, VEC Up );

/* Render primitive functions */
VOID NM6_RndPrimFree( nm6PRIM *Pr );
VOID NM6_RndPrimDraw( nm6PRIM *Pr, MATR World );
BOOL NM6_RndPrimCreate( nm6PRIM *Pr, INT NoofV, INT NoofI );

/* Load Primitive from ile */
BOOL NM6_RndPrimLoad( nm6PRIM *Pr, CHAR *FileName );

/* Render start / end */
VOID NM6_RndStart( VOID );
VOID NM6_RndEnd( VOID );

/* NM6_RndResize */
VOID NM6_RndResize( INT W, INT H );

#endif /* __rnd_h_ */

/* END OF 'rnd.h' FILE */