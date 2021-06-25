/* FILE NAME: anim.c
* PROGRAMMER: NS6
* DATE: 19.06.2021
* PURPOSE: 3D animation primitive handle module.
*/

#include "anim.h"

/* Global animation data */
ns6ANIM NS6_Anim;

/* NS6_AnimInit */
VOID NS6_AnimInit( HWND hWnd )
{

  memset(&NS6_Anim, 0, sizeof(ns6ANIM));

  /* Fill animation context */
  NS6_Anim.hWnd = hWnd;
  NS6_RndInit(hWnd);
  NS6_Anim.W = NS6_RndFrameW;
  NS6_Anim.H = NS6_RndFrameH;

  /* Timer initialization */
  NS6_TimerInit();
}
/* End of 'NS6_AnimInit' function */

/* NS6_AnimClose */
VOID NS6_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < NS6_Anim.NumOfUnits; i++)
  {
    NS6_Anim.Units[i]->Close(NS6_Anim.Units[i], &NS6_Anim);
    free(NS6_Anim.Units[i]);
    NS6_Anim.Units[i] = NULL;
  }
  NS6_Anim.NumOfUnits = 0;
  NS6_RndClose();
}
/* End of 'NS6_AnimClose' function */

/* NS6_AnimResize */
VOID NS6_AnimResize( INT W, INT H )
{
  NS6_Anim.W = W;
  NS6_Anim.H = H;
  NS6_RndResize(W, H);
}
/* NS6_AnimResize */

/* NS6_AnimCopyFrame */
VOID NS6_AnimCopyFrame( VOID )
{
  NS6_RndCopyFrame();
}
/* End of 'NS6_AnimCopyFrame' function */

/* NS6_AnimRender */
VOID NS6_AnimRender( VOID )
{
  INT i;
  POINT pt;

  /* Timer response */
  NS6_TimerResponse();

  for (i = 0; i < NS6_Anim.NumOfUnits; i++)
    NS6_Anim.Units[i]->Response(NS6_Anim.Units[i], &NS6_Anim);

  NS6_RndStart();
  for (i = 0; i < NS6_Anim.NumOfUnits; i++)
    NS6_Anim.Units[i]->Render(NS6_Anim.Units[i], &NS6_Anim);
  NS6_RndEnd();

  /* Keyboard */
  GetKeyboardState(NS6_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    NS6_Anim.Keys[i] >>= 7;
    NS6_Anim.KeysClick[i] = NS6_Anim.Keys[i] && NS6_Anim.KeysOld[i];
  }
  memcpy(NS6_Anim.KeysOld, NS6_Anim.Keys, 256);

  /* Mouse */
  GetCursorPos(&pt);
  ScreenToClient(NS6_Anim.hWnd, &pt);

  NS6_Anim.Mx = pt.x;
  NS6_Anim.My = pt.y;  
  NS6_Anim.Mdx = pt.x - NS6_Anim.Mx;
  NS6_Anim.Mdy = pt.y - NS6_Anim.My;
}
/* End of 'NS6_AnimRender' function */


/* NS6_AnimUnitAdd */
VOID NS6_AnimAddUnit( ns6UNIT *Uni )
{
  if (NS6_Anim.NumOfUnits < NS6_MAX_UNITS)
    NS6_Anim.Units[NS6_Anim.NumOfUnits++] = Uni, Uni->Init(Uni, &NS6_Anim);
}
/* End of 'NS6_AnimUnitAdd' function */


/* NS6_FlipFullScreen */
VOID NS6_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRC;

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;
    
    /* Saving old window size */
    GetWindowRect(NS6_hRndWnd, &SaveRC);

    hmon = MonitorFromWindow(NS6_hRndWnd, MONITOR_DEFAULTTONEAREST);
    
    /* Getting monitor info */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(NS6_hRndWnd, GWL_STYLE), FALSE);

    SetWindowPos(NS6_hRndWnd, HWND_TOP,
                 rc.left, rc.top,
                 rc.right - rc.left, rc.bottom - rc.top + 201,
                 SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    SetWindowPos(NS6_hRndWnd, HWND_TOP,
    SaveRC.left, SaveRC.top,
    SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
    SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
}
/* End of 'NS6_AnimFlipFullScreen' function */

/* END OF 'anim.c' FILE */