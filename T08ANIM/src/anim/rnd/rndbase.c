#include "rnd.h"

VOID NS6_RndInit( HWND hWnd )
{
	HDC hDC;

	NS6_hRndWnd = hWnd;
	NS6_hRndBmFrame = NULL;
	hDC = GetDC(hWnd);
	NS6_hRndBmFrame = CreateCompatibleDC(hDC);
	ReleaseDC(hWnd, hDC);

	NS6_RndProjSize = 0.1;
	NS6_RndProjDist = NS6_RndProjSize;
	NS6_RndProjFarClip = 200;
	NS6_RndFrameW = 50;
	NS6_RndFrameH = 50;
	NS6_RndCamSet(VecSet(0, 0, 30), VecSet(0, 0, 0), VecSet(0, 1, 0));
}

VOID NS6_RndClose( VOID )
{
	if (NS6_hRndBmFrame != NULL)
	  DeleteObject(NS6_hRndBmFrame);
	DeleteDC(NS6_hRndDCFrame);
}

VOID NS6_RndResize( INT W, INT H )
{
	HDC hDC;

	if (NS6_hRndBmFrame != NULL)
	  DeleteObject(NS6_hRndBmFrame);
	DeleteObject(NS6_hRndDCFrame);

	hDC = GetDC(NS6_hRndWnd);
	NS6_hRndBmFrame = CreateCompatibleBitmap(hDC, W, H);

	SelectObject(NS6_hRndDCFrame, NS6_hRndBmFrame);

	NS6_RndFrameW = W;
	NS6_RndFrameH = H;
	NS6_RndProjSet();
}

VOID NS6_RndCopyFrame( HDC hDC )
{
	BitBlt(hDC, 0, 0, NS6_RndFrameW, NS6_RndFrameH, NS6_hRndDCFrame, 0, 0, SRCCOPY);
}

VOID NS6_RndStart( VOID )
{
	SelectObject(NS6_hRndDCFrame, GetStockObject(NULL_PEN));
	SelectObject(NS6_hRndDCFrame, GetStockObject(DC_BRUSH));
	SetDCBrushColor(NS6_hRndDCFrame, RGB(150, 150, 150));
	Rectangle(NS6_hRndDCFrame, 0, 0, NS6_hRndFrameW + 1, NS6_hRndFrameH + 1);

	SelectObject(NS6_hRndDCFrame, GetStockObject(DC_PEN));
	SelectObject(NS6_hRndDCFrame, GetStockObject(NULL_BRUSH));
	SetDCBrushColor(NS6_hRndDCFrame, RGB(255, 255, 255));
}

VOID NS6_RndEnd( VOID )
{
}

VOID NS6_RndProjSet( VOID )
{
	DBL rx, ry;

	rx = ry = NS6_RndProjSize;

	if (NS6_RndFrameW > NS6_RndFrameH)
	  rx *= (DBL)NS6_RndFrameW / NS6_RndFrameH;
	else
	  ry *= (DBL)NS6_RndFrameH / NS6_RndFrameW;

	NS6_RndMatrProj = MatrFrustum(-rx / 2, rx / 2, -ry / 2, ry / 2, NS6_RndProjDist, NS6_RndProjFarClip);
	NS6_RndMatrVP = MatrMulMatr(NS6_RndMatrView, NS6_RndMatrProj);
}

VOID NS6_RndCamSet( VEC Loc, VEC, At, VEC Up )
{
	NS6_RndMatrView = MatrView(Loc, At, Up);
	NS6_RndMatrVP = MatrMulMart(NS6_RndMatrView, NS6_RndMatrProj);
}