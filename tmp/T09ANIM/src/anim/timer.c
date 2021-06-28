/* FILE NAME: timer.c
* PROGRAMMER: NM6
* DATE: 19.06.2021
* PURPOSE: 3D animation project.
* Timer handle functions.
*/

#include "anim.h"

nm6ANIM NM6_Anim;

/* Local timer data */
static UINT64
  StartTime, /* Start program time */
  OldTime, /* Previous frame time */
  OldTimeFPS, /* Old time FPS measurement */
  PauseTime, /* Time during pause period */
  TimePerSec, /* Timer resolution */
  FrameCounter; /* Frames counter */

/* NM6_TimerInit */
VOID NM6_TimerInit( VOID )
{
  LARGE_INTEGER t;
  QueryPerformanceFrequency(&t);
  TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  StartTime = OldTime = OldTimeFPS = t.QuadPart;

  FrameCounter = 0;
  NM6_Anim.IsPause = FALSE;
  NM6_Anim.FPS = 30.0;
  PauseTime = 0;

}/* End of 'NM6_TimerInit' function */

/* NM6_TimerResponse */
VOID NM6_TimerResponse( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceCounter(&t);
  /* Global time */
  NM6_Anim.GlobalTime = (DBL)(t.QuadPart - StartTime) / TimePerSec;
  NM6_Anim.GlobalDeltaTime = (DBL)(t.QuadPart - OldTime) / TimePerSec;
  /* Time with pause */
  if (NM6_Anim.IsPause)
  {
    NM6_Anim.DeltaTime = 0;
    PauseTime += t.QuadPart - OldTime;
  }
  else
  {
    NM6_Anim.DeltaTime = NM6_Anim.GlobalDeltaTime;
    NM6_Anim.Time = (DBL)(t.QuadPart - PauseTime - StartTime) / TimePerSec;
  }
  /* FPS */
  FrameCounter++;
  if (t.QuadPart - OldTimeFPS > TimePerSec)
  {
    NM6_Anim.FPS = FrameCounter * TimePerSec / (DBL)(t.QuadPart - OldTimeFPS);
    OldTimeFPS = t.QuadPart;
    FrameCounter = 0;
  }
  OldTime = t.QuadPart;

}/* End of 'NM6_TimerResponse' function */


/* END OF 'timer.c' FILE */