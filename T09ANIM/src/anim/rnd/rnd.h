#ifndef __rnd_h_
#define __rnd_h_

#define GLEW_STATIC
#include <glew.h>

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
  VEC2 T;
  VEC N;
  VEC4 C;
} ns6VERTEX;

typedef struct tgns6PRIM
{
  ns6VERTEX *V;
  INT NumOfV;

  INT *I;
  INT NumOfI;

  INT VA, VBuf, IBuf;
  INT NumOfElements;

  MATR Trans;
} ns6PRIM;

BOOL NS6_RndPrimLoad( ns6PRIM *Pr, CHAR *FileName );

VOID NS6_RndInit( HWND hWnd );/**/

VOID NS6_RndClose( VOID );/**/

VOID NS6_RndCopyFrame( VOID );/**/

VOID NS6_RndProjSet( VOID );/**/

VOID NS6_RndCamSet( VEC Loc, VEC At, VEC Up );/**/

VOID NS6_RndPrimCreate( ns6PRIM *Pr, ns6VERTEX *V, INT NumOfV, INT *I, INT NumOfI );

VOID NS6_RndPrimDraw( ns6PRIM *Pr, MATR World );

VOID NS6_RndPrimFree( ns6PRIM *Pr );

VOID NS6_RndStart( VOID );

VOID NS6_RndEnd( VOID );

VOID NS6_RndResize( INT W, INT H );

#define NS6_STR_MAX 300

typedef struct tagns6SHADER
{
  CHAR Name[NS6_STR_MAX]; /* Shader filename prefix */
  INT ProgId;             /* Shader program Id */
} ns6SHADER;

#define NS6_MAX_SHADERS 30
extern ns6SHADER NS6_RndShaders[NS6_MAX_SHADERS];
extern INT NS6_RndShadersSize;


VOID NS6_RndShadersInit( VOID );
VOID NS6_RndShadersClose( VOID );
INT NS6_RndShaderAdd( CHAR *FileNamePrefix );
VOID NS6_RndShadersUpdate( VOID );


#endif