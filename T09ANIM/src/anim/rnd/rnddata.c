#include "rnd.h"

HWND NS6_hRndWnd;
HDC NS6_hRndDCFrame;
HBITMAP NS6_hRndBmFrame;
INT NS6_RndFrameW, NS6_RndFrameH;
FLT
  NS6_RndProjSize,
  NS6_RndProjDist,
  NS6_RndProjFarClip;

MATR
  NS6_RndMatrView,
  NS6_RndMatrProj,
  NS6_RndMatrVP;

