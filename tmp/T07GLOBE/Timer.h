/* FILE NAME: Timer.h
 * PROGRAMMER: NM6
 * DATE: 14.06.2021
 * PURPOSE: WinAPI application sample.
 */

#define __Timer_h_
#ifdef __Timer_h_

#include <windows.h>

/* Global timer data */
DOUBLE
  GLB_Time,      
  GLB_DeltaTime,  
  GLB_FPS;        
BOOL GLB_IsPause;

VOID GLB_TimerInit( VOID );

/* Timer  response function
 * ARGUMNETS: None
 * RETURNS: None
*/
VOID GLB_TimerResponse( VOID );

#endif 