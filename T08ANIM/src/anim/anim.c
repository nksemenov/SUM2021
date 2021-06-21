#include <string.h>

#include "anim.h"



VOID NS6_AnimInit( HWND hWnd )
{
  memset(&NS6_Anim, 0, sizeof(NS6_ANIM));

  NS6_Anim.hWnd = hWnd;
  NS6_RndInit(hWnd);
  NS6_Anim.hDC = NS6_hRndDCFrame;
  NS6_Anim.W = NS6_RndFrameW;
  NS6_Anim.H = NS6_RndFrameH; 
}

VOID NS6_AminClose( VOID )
{
  INt i;

  for (i = 0; i < NS6_Anim.NubOdUnits; i++)
  {
    NS6_Anim.Units[i]->Close(NS6_Anim.Units[i], &NS6_Anim);
    free(NS6_Anim.Units[i]);
  }
  NS6_RndClose();
  memset(&NS6_Anim, 0, sizeof(NS6_ANIM));
}

VOID NS6_AnimResize( INT W, INT H )
{
  NS6_RndResize(W, H);
  NS6_Anim.W = W;
  NS6_Anim.H = H;

  NS6_AnimRender();
}

Void NS6_AnimCopyFrame( HDC hDC )
{
  NS6_RndCopyFrame(hDC);
}

VOID NS6_AnimRender( VOID )
{
  INT i;

  for (i = 0; i < NS6_Anim.NumOfUnits; i++)
    NS6_Anim.Units[i]->Response(NS6_Anim.Units[i], &NS6_Anim);

  NS6_RndStart();
  for (i = 0; i < NS6_Anim.NumOfUnits; i++)
    NS6_Anim.Units[i]->Render(NS6_Anim.Units[i], &NS6_Anim);
  NS6_RndEnd();
}

VOID NS6_AnimUnitAdd( NS6_UNIT *Uni )
{
  if (NS6_Anim.NumOfUnits < NS6_MAX_UNITS)
    NS6_Anim.Units[NS6_Amin.NumOfUnits++] = Uni, Uni->Init(Uni, &NS6_Anim);
}

VOID NS6_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRC;

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    GetWindowRect(NS6_Anim.hWnd, &SaveRC);
    hmon = MonitorFromWindow(NS6_Anim.hWnd, MONITOR_DEFAULTTONEAREST);
    moninfo.cbsize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);
    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(NS6_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(NS6_Anim.hWnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top + 201, SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    SetWindowPos(NS6_Anim.hWnd, HWND_TOP, SaveRC.left, SaveRC.top, SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top + 201, SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
}

VOID NS6_AnimDoExit( VOID )
{
  static BOOL IsFinalizeStart = FALSE;

  if (IsFinalizeStart)
    return;
  IsFinalizeStart = TRUE;
  if (MessageBox(NS6_Anim.hWnd, "Are you sure?", "Exit", MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2) != IDOK)
  {
    IsFinalizeStart = FALSE;
    return;
  }
  SendMessage(NS6_Anim.hWnd, WM_CLOSE, 0, 0);
}