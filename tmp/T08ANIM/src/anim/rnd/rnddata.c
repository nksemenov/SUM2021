/* FILE NAME: rnddata.c
* PROGRAMMER: NM6
* DATE: 21.06.2021
* PURPOSE: 3D animation rendering data module.
*/

#include "rnd.h"

/* Global rendering data */
HWND NM6_hRndWnd; /* Work window handle */
HDC NM6_hRndDCFrame; /* Work window memory device context */
HBITMAP NM6_hRndBmFrame; /* Work window background bitmap handle */
INT NM6_RndFrameW, NM6_RndFrameH; /* Work window size */

DBL
  NM6_RndProjSize = 0.1, /* Project plane fit square */
  NM6_RndProjDist = 0.1, /* Distance to project plane from viewer (near) */
  NM6_RndProjFarClip = 300; /* Distance to project far clip plane (far) */

MATR
  NM6_RndMatrView, /* View coordinate system matrix */
  NM6_RndMatrProj, /* Projection coordinate system matrix */
  NM6_RndMatrVP; /* Stored (View * Proj) matrix */

/* END OF 'rnddata.c' FILE */