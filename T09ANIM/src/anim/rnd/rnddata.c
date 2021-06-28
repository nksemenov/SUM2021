#include "rnd.h"

HWND NS6_hRndWnd;
HDC NS6_hRndDC;
HGLRC NS6_hRndGLRC;
INT NS6_RndFrameW, NS6_RndFrameH;

VEC 
  NS6_RndCamLoc,
  NS6_RndCamRight,
  NS6_RndCamAt,
  NS6_RndCamDir,
  NS6_RndCamUp;

FLT
  NS6_RndProjSize,
  NS6_RndProjDist,
  NS6_RndProjFarClip;

MATR
  NS6_RndMatrView,
  NS6_RndMatrProj,
  NS6_RndMatrVP;

VEC NS6_RndCamLoc;
