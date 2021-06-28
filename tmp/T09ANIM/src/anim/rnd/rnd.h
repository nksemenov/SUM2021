  /* FILE NAME: rnd.h
* PROGRAMMER: NM6
* DATE: 17.06.2021
* PURPOSE: 3D animation rendering declaration module.
*/

#ifndef __rnd_h_
#define __rnd_h_

#define GLEW_STATIC
#include <glew.h>

#include "../../def.h"

extern HWND NM6_hRndWnd; /* Work window handle */
extern HGLRC NM6_hRndGLRC; /* Open GL render devic context */
extern HDC NM6_hRndDC; /* Work window memory device context */
extern INT NM6_RndFrameW, NM6_RndFrameH; /* Work window size */

extern FLT
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
  VEC2 T;
  VEC N;
  VEC4 C;
} nm6VERTEX;

/* primitive representation type */
typedef struct tagnm6PRIM
{
  INT VA, VBuf, IBuf;
  INT NumOfElements;

  MATR Trans;
} nm6PRIM;

/*typedef struct tagnm6UNIT_CONTROL
{
  ;
}*/

/* Render base functions */
VOID NM6_RndInit( HWND hWnd );
VOID NM6_RndClose( VOID );
VOID NM6_RndCopyFrame( VOID );
VOID NM6_RndProjSet( VOID );
VOID NM6_RndCamSet( VEC Loc, VEC At, VEC Up );

/* Render primitive functions */
VOID NM6_RndPrimCreate( nm6PRIM *Pr, nm6VERTEX *V, INT NumOfV, INT *I, INT NumOfI );
VOID NM6_RndPrimDraw( nm6PRIM *Pr, MATR World );
VOID NM6_RndPrimFree( nm6PRIM *Pr );

/* Load Primitive from ile */
BOOL NM6_RndPrimLoad( nm6PRIM *Pr, CHAR *FileName );

/* Render start / end */
VOID NM6_RndStart( VOID );
VOID NM6_RndEnd( VOID );

/* NM6_RndResize */
VOID NM6_RndResize( INT W, INT H );

#define NM6_STR_MAX 300

/* Shader program store type */
typedef struct tagnm6SHADER
{
  CHAR Name[NM6_STR_MAX]; /* Shader filename prefix */
  INT ProgId;             /* Shader program Id */
} nm6SHADER;

#define NM6_MAX_SHADERS 30
extern nm6SHADER NM6_RndShaders[NM6_MAX_SHADERS];
extern INT NM6_RndShadersSize;


VOID NM6_RndShadersInit( VOID );
VOID NM6_RndShadersClose( VOID );
INT NM6_RndShaderAdd( CHAR *FileNamePrefix );
VOID NM6_RndShadersUpdate( VOID );


#endif /* __rnd_h_ */

/* END OF 'rnd.h' FILE */