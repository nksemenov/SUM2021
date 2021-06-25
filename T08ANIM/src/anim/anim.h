/* FILE NAME: anim.h
* PROGRAMMER: NS6
* DATE: 19.06.2021
* PURPOSE: 3D animation primitive handle module.
*/

#ifndef __anim_h_
#define __anim_h_

#include "rnd/rnd.h"

#define NS6_MAX_UNITS 3000

/* typedef */
typedef struct tagns6UNIT ns6UNIT;
typedef struct tagns6ANIM ns6ANIM;

#define NS6_UNIT_BASE_FIELDS                    \
VOID (*Init)( ns6UNIT *Uni, ns6ANIM *Ani );     \
VOID (*Close)( ns6UNIT *Uni, ns6ANIM *Ani );    \
VOID (*Response)( ns6UNIT *Uni, ns6ANIM *Ani ); \
VOID (*Render)( ns6UNIT *Uni, ns6ANIM *Ani )

/* Animation context representation type */
struct tagns6ANIM
{
  HWND hWnd;
  HDC hDC;
  INT W, H;

  ns6UNIT *Units[NS6_MAX_UNITS];
  INT NumOfUnits;

  /* Timer data */
  DBL
  GlobalTime, GlobalDeltaTime, /* Global time and interframe interval */
  Time, DeltaTime, /* Time with pause and interframe interval */
  FPS; /* Frames per second value */
  BOOL
  IsPause; /* Pause flag */
};

/* struct tagUNIT */
struct tagns6UNIT
{
  NS6_UNIT_BASE_FIELDS;
};

extern ns6ANIM NS6_Anim;

/* AnimClose */
VOID NS6_AnimClose( VOID );

/* AnimInit */
VOID NS6_AnimInit( HWND hWnd );

/* AnimRender */
VOID NS6_AnimRender( VOID );

/* AnimunitAdd */
VOID NS6_AnimAddUnit( ns6UNIT *Uni );

/* AnimCopyFrame */
VOID NS6_AnimCopyFrame( HDC hDC );

/* AnimResize */
VOID NS6_AnimResize( INT W, INT H );

/* FlipFullScreen */
VOID NS6_AnimFlipFullScreen( VOID );

/* AnimUnitCreate */
ns6UNIT * NS6_AnimUnitCreate( INT Size );

/* TimerInit */
VOID NS6_TimerInit( VOID );

/* TimerResponse */
VOID NS6_TimerResponse( VOID );

#endif /* __anim_h_ */

/* END OF 'anim.h' FILE */