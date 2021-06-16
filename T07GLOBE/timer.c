#include "timer.h"
#include <time.h>

DOUBLE GLB_Time, GLB_DeltaTime, GLB_FPS;
BOOL GLB_IsPause, IsPause;
LONG StartTime, OldTime, PauseTime, OldFPSTime, FrameCount;

/*VOID Timer( VOID )
{
  long t = clock();
  static long StartTime = -1, PauseTime, OldTime;

  if (StartTime == -1)
    StartTime = OldTime = t;
  if (IsPause)
    IsPause += t - OldTime, DeltaTime = 0;
  else
  {
    SyncTime = (t - PauseTime - StartTime) / (DOUBLE)CLOCKS_PER_SEC;
    DeltaTime = (t - OldTime) / (DOUBLE)CLOCKS_PER_SEC;
  }
  OldTime = t;
}  */

VOID GLB_TimerInit( VOID )
{  
  StartTime = OldTime = OldFPSTime = clock();
  FrameCount = 0;
  GLB_IsPause = FALSE;
}
  
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
}