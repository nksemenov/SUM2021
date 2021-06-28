/* FILE NAME: Timer.c
* PROGRAMMER: NM6
* DATE: 14.06.2021
* PURPOSE: WinAPI application sample.
*/

#include <time.h>

#include "Timer.h"

/* Local timer data */
static LONG
  StartTime,
  OldTime,
  PauseTime,
  OldFPSTime,
  FrameCount; 

/* Timer  initialixation function
 * ARGUMNETS: None
 * RETURNS: None
*/
VOID GLB_TimerInit( VOID )      
{
  StartTime = OldTime = OldFPSTime = clock();
  FrameCount = 0;
  GLB_IsPause = FALSE;
} /*End of 'GLB_TimerInit' function*/

/* Timer  response function
 * ARGUMNETS: None
 * RETURNS: None
*/
VOID GLB_TimerResponse( VOID )
{
  LONG t = clock();

  if (!GLB_IsPause)
  {
    GLB_Time = (DOUBLE)(t - PauseTime - StartTime) / CLOCKS_PER_SEC;
    GLB_DeltaTime = (DOUBLE)(t - OldTime) / CLOCKS_PER_SEC;
  }
  else
  {
    PauseTime += t - OldTime;
    GLB_DeltaTime = 0;
  }
  FrameCount++;
  if (t - OldFPSTime > CLOCKS_PER_SEC)
  {
    GLB_FPS = FrameCount / ((DOUBLE)(t - OldFPSTime) / CLOCKS_PER_SEC);
    OldFPSTime = t;
    FrameCount = 0;
  }
  OldTime = t; 
} /*End of 'GLB_TimerInit' function*/
