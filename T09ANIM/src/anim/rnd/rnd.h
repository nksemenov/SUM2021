#ifndef __rnd_h_
#define __rnd_h_

#define GLEW_STATIC
#include <glew.h>

#include "res/rndres.h"

#include <wglew.h>

extern HWND NS6_hRndWnd;
extern HGLRC NS6_hRndGLRC;
extern HDC NS6_hRndDC;
extern HBITMAP NS6_hRndBmFrame;
extern INT NS6_RndFrameW, NS6_RndFrameH;

extern FLT
  NS6_RndProjSize,
  NS6_RndProjDist,
  NS6_RndProjFarClip;

extern MATR
  NS6_RndMatrView,
  NS6_RndMatrProj,
  NS6_RndMatrVP;

extern VEC 
  NS6_RndCamLoc,
  NS6_RndCamRight,
  NS6_RndCamAt,
  NS6_RndCamDir,
  NS6_RndCamUp;

extern VEC NS6_RndCamLoc;

typedef struct tagns6VERTEX
{
  VEC P;
  VEC2 T;
  VEC N;
  VEC4 C;
} ns6VERTEX;

typedef enum tagns6PRIM_TYPE
{
  NS6_RND_PRIM_TRIMESH,  /* Triangle mesh - array of triangles */
  NS6_RND_PRIM_TRISTRIP, /* Triangle strip - array of stripped triangles */
  NS6_RND_PRIM_LINES,    /* Line segments (by 2 points) */
  NS6_RND_PRIM_POINTS,   /* Array of points */
} ns6PRIM_TYPE;

typedef struct tagns6PRIM
{
  //ns6VERTEX *V;
  ns6PRIM_TYPE Type;
  INT NumOfV;

  INT *I;
  INT NumOfI;

  INT VA, VBuf, IBuf;
  INT NumOfElements;
  INT MtlNo;

  MATR Trans;
} ns6PRIM;

BOOL NS6_RndPrimLoad( ns6PRIM *Pr, CHAR *FileName );

VOID NS6_RndInit( HWND hWnd );

VOID NS6_RndClose( VOID );

VOID NS6_RndCopyFrame( VOID );

VOID NS6_RndProjSet( VOID );

VOID NS6_RndCamSet( VEC Loc, VEC At, VEC Up );

VOID NS6_RndPrimCreate( ns6PRIM *Pr, ns6VERTEX *V, INT NumOfV, INT *I, INT NumOfI );

VOID NS6_RndPrimDraw( ns6PRIM *Pr, MATR World );

VOID NS6_RndPrimFree( ns6PRIM *Pr );

VOID NS6_RndStart( VOID );

VOID NS6_RndEnd( VOID );

VOID NS6_RndResize( INT W, INT H );

VOID NS6_RndPrimCreateGrid( ns6PRIM *Pr, INT Grid_W, INT Grid_H, ns6VERTEX *V );


/***
 * Primitive collection support
 ***/

/* Primitive collection data type */
typedef struct tagns6PRIMS
{
  INT NumOfPrims; /* Number of primitives in array */  
  ns6PRIM *Prims; /* Array of primitives */
  MATR Trans;     /* Common transformation matrix */
} ns6PRIMS;

/* Create array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       ns6PRIMS *Prs;
 *   - number of primitives to be add:
 *       INT NumOfPrims;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL NS6_RndPrimsCreate( ns6PRIMS *Prs, INT NumOfPrims );

/* Delete array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       ns6PRIMS *Prs;
 * RETURNS: None.
 */
VOID NS6_RndPrimsFree( ns6PRIMS *Prs );

/* Draw array of primitives function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       ns6PRIMS *Prs;
 *   - global transformation matrix:
 *       MATR World;
 * RETURNS: None.
 */
VOID NS6_RndPrimsDraw( ns6PRIMS *Prs, MATR World );

/* Load array of primitives from .G3DM file function.
 * ARGUMENTS:
 *   - pointer to primitives structure:
 *       ns6PRIMS *Prs;
 *   - file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL NS6_RndPrimsLoad( ns6PRIMS *Prs, CHAR *FileName );


#endif