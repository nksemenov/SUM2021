#include "anim.h"

static UINT64
  StartTime,    /* Start program time */
  OldTime,      /* Previous frame time */
  OldTimeFPS,   /* Old time FPS measurement */
  PauseTime,    /* Time during pause period */
  TimePerSec,   /* Timer resolution */
  FrameCounter; /* Frames counter */


VOID NS6_TimerInit ( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceFrequency(&t);
  TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  StartTime = OldTime = OldTimeFPS = t.QuadPart;
  FrameCounter = 0;
  NS6_Anim.IsPause = FALSE;
  NS6_Anim.FPS = 30.0;
  PauseTime = 0;
}

VOID NS6_TimerResponse ( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceCounter(&t);
  
  /* Global Time */
  NS6_Anim.GlobalTime = (DBL)(t.QuadPart - StartTime) / TimePerSec;
  NS6_Anim.GlobalDeltaTime = (DBL)(t.QuadPart - OldTime) / TimePerSec;
  
  /* Time with pause */
  if (NS6_Anim.IsPause)
  {
    NS6_Anim.DeltaTime = 0;
    PauseTime += t.QuadPart - OldTime;
  }
  else
  {
    NS6_Anim.DeltaTime = NS6_Anim.GlobalDeltaTime;
    NS6_Anim.Time = (DBL)(t.QuadPart - PauseTime - StartTime) / TimePerSec;
  }

  /* FPS */
  FrameCounter++;
  if (t.QuadPart - OldTimeFPS > TimePerSec)
  {
    NS6_Anim.FPS = FrameCounter * TimePerSec / (DBL)(t.QuadPart - OldTimeFPS);
    OldTimeFPS = t.QuadPart;
    FrameCounter = 0;
  }
  OldTime = t.QuadPart;
}