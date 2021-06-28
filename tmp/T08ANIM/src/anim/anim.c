/* FILE NAME: anim.c
* PROGRAMMER: NM6
* DATE: 19.06.2021
* PURPOSE: 3D animation primitive handle module.
*/

#include "anim.h"

/* Global animation data */
nm6ANIM NM6_Anim;

/* NM6_AnimInit */
VOID NM6_AnimInit( HWND hWnd )
{

  memset(&NM6_Anim, 0, sizeof(nm6ANIM));

  /* Fill animation context */
  NM6_Anim.hWnd = hWnd;
  NM6_RndInit(hWnd);
  NM6_Anim.hDC = NM6_hRndDCFrame;
  NM6_Anim.W = NM6_RndFrameW;
  NM6_Anim.H = NM6_RndFrameH;

  /* Timer initialization */
  NM6_TimerInit();

}/* End of 'NM6_AnimInit' function */

/* NM6_AnimClose */
VOID NM6_AnimClose( VOID )
{
  INT i;

  for (i = 0; i < NM6_Anim.NumOfUnits; i++)
  {
    NM6_Anim.Units[i]->Close(NM6_Anim.Units[i], &NM6_Anim);
    free(NM6_Anim.Units[i]);
    NM6_Anim.Units[i] = NULL;
  }
  NM6_Anim.NumOfUnits = 0;
  NM6_RndClose();
}/* End of '' function */

/* NM6_AnimResize */
VOID NM6_AnimResize( INT W, INT H )
{
  NM6_Anim.W = W;
  NM6_Anim.H = H;
  NM6_RndResize(W, H);
}/* NM6_AnimResize */

/* NM6_AnimCopyFrame */
VOID NM6_AnimCopyFrame( HDC hDC )
{
  NM6_RndCopyFrame(hDC);
}/* End of 'NM6_AnimCopyFrame' function */

/* NM6_AnimRender */
VOID NM6_AnimRender( VOID )
{
  INT i;

  /* Timer response */
  NM6_TimerResponse();

  for (i = 0; i < NM6_Anim.NumOfUnits; i++)
    NM6_Anim.Units[i]->Response(NM6_Anim.Units[i], &NM6_Anim);

    NM6_RndStart();
  for (i = 0; i < NM6_Anim.NumOfUnits; i++)
    NM6_Anim.Units[i]->Render(NM6_Anim.Units[i], &NM6_Anim);
    NM6_RndEnd();
}/* End of 'NM6_AnimRender' function */


/* NM6_AnimUnitAdd */
VOID NM6_AnimAddUnit( nm6UNIT *Uni )
{
if (NM6_Anim.NumOfUnits < NM6_MAX_UNITS)
  NM6_Anim.Units[NM6_Anim.NumOfUnits++] = Uni, Uni->Init(Uni, &NM6_Anim);
}/* End of 'NM6_AnimUnitAdd' function */

/* END OF 'anim.c' FILE */