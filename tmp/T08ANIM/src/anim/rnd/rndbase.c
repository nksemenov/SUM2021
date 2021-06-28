/* FILE NAME: rndbase.c
* PROGRAMMER: NM6
* DATE: 17.06.2021
* PURPOSE: 3D animation rendering function module.
*/

#include "rnd.h"

/* NM6_RndInit */
VOID NM6_RndInit( HWND hWnd )
{

  HDC hDC;

  NM6_hRndWnd = hWnd;
  NM6_hRndBmFrame = NULL;

  hDC = GetDC(hWnd);
  NM6_hRndDCFrame = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);

  /* Render perametrs */
  NM6_RndProjSize = 0.1;
  NM6_RndProjDist = NM6_RndProjSize;
  NM6_RndProjFarClip = 300;
  NM6_RndFrameW = 50;
  NM6_RndFrameH = 50;
  NM6_RndCamSet(VecSet(0, 0, 30), VecSet(0, 0, 0), VecSet(0, 1, 0));
}/* End of 'NM6_RndInit' function */

/* NM6_RndClose */
VOID NM6_RndClose( VOID )
{
  DeleteDC(NM6_hRndDCFrame);
  DeleteObject(NM6_hRndBmFrame);
}/* End of 'NM6_RndClose' function */

/* NM6_RndCopyFrame */
VOID NM6_RndCopyFrame( HDC hDC )
{
  BitBlt(hDC, 0, 0, NM6_RndFrameW, NM6_RndFrameH,
  NM6_hRndDCFrame, 0, 0, SRCCOPY);
}/* End of 'NM6_RndCopyFrame' function */

/* NM6_RndProjSet */
VOID NM6_RndProjSet( VOID )
{
  DBL rx, ry;

  rx = ry = NM6_RndProjSize;

  /* Correct aspect ratio */
  if (NM6_RndFrameW > NM6_RndFrameH)
    rx *= (DBL)NM6_RndFrameW / NM6_RndFrameH;
  else
    ry *= (DBL)NM6_RndFrameH / NM6_RndFrameW;

  NM6_RndMatrProj =
  MatrFrustum(-rx / 2, rx / 2, -ry / 2, ry / 2,
  NM6_RndProjDist, NM6_RndProjFarClip);
  NM6_RndMatrVP = MatrMulMatr(NM6_RndMatrView, NM6_RndMatrProj);
}/* NM6_RndProjSet */

/* NM6_RndCamset */
VOID NM6_RndCamSet( VEC Loc, VEC At, VEC Up )
{
  NM6_RndMatrView = MatrView(Loc, At, Up);
  NM6_RndMatrVP = MatrMulMatr(NM6_RndMatrView, NM6_RndMatrProj);
}/* End of 'NM6_RndCamSet' function */

/* NM6_RndStart */
VOID NM6_RndStart( VOID )
{
  SelectObject(NM6_hRndDCFrame, GetStockObject(WHITE_BRUSH));
  SelectObject(NM6_hRndDCFrame, GetStockObject(NULL_PEN));
  Rectangle(NM6_hRndDCFrame, 0, 0, NM6_RndFrameW + 1, NM6_RndFrameH + 1);

  SelectObject(NM6_hRndDCFrame, GetStockObject(BLACK_PEN));
  SelectObject(NM6_hRndDCFrame, GetStockObject(WHITE_BRUSH));
  SetDCPenColor(NM6_hRndDCFrame, RGB(255, 255, 255));
}/* End of 'NM6_RndStart' function */

/* NM6_RndEnd */
VOID NM6_RndEnd( VOID )
{

}/* End of 'NM6_RndEnd' function */

/* NM6_RndResize */
VOID NM6_RndResize( INT W, INT H )
{
  HDC hDC = GetDC(NM6_hRndWnd);

  if (NM6_hRndBmFrame != NULL)
  DeleteObject(NM6_hRndBmFrame);

  NM6_hRndBmFrame = CreateCompatibleBitmap(hDC, W, H);
  ReleaseDC(NM6_hRndWnd, hDC);
  SelectObject(NM6_hRndDCFrame, NM6_hRndBmFrame);

  NM6_RndFrameW = W;
  NM6_RndFrameH = H;

  NM6_RndProjSet();
}/* End of 'NM6_RndResize' function */


/* END OF 'rndbase.c' FILE */