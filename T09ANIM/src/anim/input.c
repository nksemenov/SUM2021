/* FILE NAME : input.c
 * PROGRAMMER: ns6
 * DATE      : 21.06.2021 
 * PURPOSE   : Unit declaration module
 */
#include <string.h>

#include "anim.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm")

/* Mouse wheel state global data */
INT NS6_MouseWheel;

/* Keyboard initialization function.
 * ARGUMENTS:
 *   - None.
 * RETURNS:
 *   (VOID) None.
*/
static VOID NS6_AnimKeyboardInit( VOID )
{
  INT i;

  for (i = 0; i < 256; i++)
  {
    NS6_Anim.Keys[i] = 0;
    NS6_Anim.KeysClick[i] = 0;
    NS6_Anim.KeysOld[i] = 0;
  }
} /* End of 'NS6_AnimKeyboardInit' function */

/* Keyboard response function.
 * ARGUMENTS:
 *   - None.
 * RETURNS:
 *   (VOID) None.
*/
static VOID NS6_AnimKeyboardResponse( VOID )
{
  INT i;

  GetKeyboardState(NS6_Anim.Keys);
  for (i = 0; i < 256; i++)
  {
    NS6_Anim.Keys[i] >>= 7;
    NS6_Anim.KeysClick[i] = NS6_Anim.Keys[i] && !NS6_Anim.KeysOld[i];
  }

  memcpy(NS6_Anim.KeysOld, NS6_Anim.Keys, 256);
} /* End of 'NS6_AnimKeyboardResponse' function */

/* Mouse initialization function.
 * ARGUMENTS:
 *   - None.
 * RETURNS:
 *   (VOID) None.
*/
static VOID NS6_AnimMouseInit( VOID )
{
  POINT pt;

  GetCursorPos(&pt);
  ScreenToClient(NS6_Anim.hWnd, &pt);

  NS6_Anim.Mdx = pt.x - NS6_Anim.Mx;
  NS6_Anim.Mdy = pt.y - NS6_Anim.My;

  NS6_Anim.Mx = pt.x;
  NS6_Anim.My = pt.y;

  NS6_Anim.Mdz = NS6_MouseWheel;
  NS6_Anim.Mz += NS6_MouseWheel;
  NS6_MouseWheel = 0;
} /* End of 'NS6_AnimMouseInit' function */

/* Mouse response function.
 * ARGUMENTS:
 *   - None.
 * RETURNS:
 *   (VOID) None.
*/
static VOID NS6_AnimMouseResponse( VOID )
{
  POINT pt;

  GetCursorPos(&pt);
  ScreenToClient(NS6_Anim.hWnd, &pt);

  NS6_Anim.Mdx = pt.x - NS6_Anim.Mx;
  NS6_Anim.Mdy = pt.y - NS6_Anim.My;

  NS6_Anim.Mx = pt.x;
  NS6_Anim.My = pt.y;

  NS6_Anim.Mdz = NS6_MouseWheel;
  NS6_Anim.Mz += NS6_MouseWheel;
  NS6_MouseWheel = 0;
} /* End of 'NS6_AnimMouseResponse' function */

/* Input initialization function.
 * ARGUMENTS:
 *   - None.
 * RETURNS:
 *   (VOID) None.
*/
VOID NS6_AnimInputInit( VOID )
{
  NS6_AnimKeyboardInit();
  NS6_AnimMouseInit();
} /* End of 'NS6_AnimInputInit' function */

/* Input response function.
 * ARGUMENTS:
 *   - None.
 * RETURNS:
 *   (VOID) None.
*/
VOID NS6_AnimInputResponse( VOID )
{
  NS6_AnimKeyboardResponse();
  NS6_AnimMouseResponse();
} /* End of 'NS6_AnimInputResponse' function */

/* END OF 'input.c' FILE*/