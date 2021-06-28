/* FILE NAME: anim.h
* PROGRAMMER: NM6
* DATE: 19.06.2021
* PURPOSE: 3D animation primitive handle module.
*/

#ifndef __anim_h_
#define __anim_h_

#include "rnd/rnd.h"

#define NM6_MAX_UNITS 3000

/* typedef */
typedef struct tagnm6UNIT nm6UNIT;
typedef struct tagnm6ANIM nm6ANIM;

#define UNIT_BASE_FIELDS \
VOID (*Init)( nm6UNIT *Uni, nm6ANIM *Ani ); \
VOID (*Close)( nm6UNIT *Uni, nm6ANIM *Ani ); \
VOID (*Response)( nm6UNIT *Uni, nm6ANIM *Ani ); \
VOID (*Render)( nm6UNIT *Uni, nm6ANIM *Ani )

/* Animation context representation type */
struct tagnm6ANIM
{
  HWND hWnd;
  HDC hDC;
  INT W, H;

  nm6UNIT *Units[NM6_MAX_UNITS];
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
struct tagnm6UNIT
{
  UNIT_BASE_FIELDS;
};

/* AnimClose */
VOID NM6_AnimClose( VOID );

/* AnimInit */
VOID NM6_AnimInit( HWND hWnd );

/* AnimRender */
VOID NM6_AnimRender( VOID );

/* AnimunitAdd */
VOID NM6_AnimAddUnit( nm6UNIT *Uni );

/* AnimCopyFrame */
VOID NM6_AnimCopyFrame( HDC hDC );

/* AnimResize */
VOID NM6_AnimResize( INT W, INT H );

/* FlipFullScreen */
VOID NM6_AnimFlipFullScreen( VOID );

/* AnimUnitCreate */
nm6UNIT * NM6_AnimUnitCreate( INT Size );



/* TimerInit */
VOID NM6_TimerInit( VOID );
/* TimerResponse */
VOID NM6_TimerResponse( VOID );



#endif /* __anim_h_ */

/* END OF 'anim.h' FILE */