#ifndef __timer_h_
#define __timer_h_

#include <windows.h>

DOUBLE GLB_Time, GLB_DeltaTime, GLB_FPS;
BOOL GLB_IsPause, IsPause;

VOID GLB_TimerInit( VOID );
VOID GLB_TimerResponse( VOID );


#endif