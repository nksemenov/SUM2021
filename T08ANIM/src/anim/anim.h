#ifndef __anim_h_
#define __anim_h_

#include "rnd/rnd.h"

#define NS6_UNIT_BASE_FIELDS               \
  VOID (*Init)( UNIT *Uni, ANIM *Ani );    \
  VOID (*Close)(UNIT *Uni, ANIM *Ani );    \
  VOID (*Response)(UNIT *Uni, ANIM *Ani ); \
  VOID (*Render)(UNIT *Uni, ANIM *Ani )

typedef struct tagNS6_UNIT UNIT;
struct tagNS6_UNIT
{
  NS6_UNIT_BASE_FIELDS;
};

typedef struct tafNS6_UNIT NS6_UNIT;
typedef struct tagNS6_ANIM
{
  HWND hWnd;
  HDC hDC;
  INT W, H;

  NS6_UNIT *UNITS[NS6_MAX_UNITS];
  INT NumOfUnits;
} NS6_ANIM;

VOID NS6_AnimInit( HWND hWnd );

VOID NS6_AminClose( VOID );

VOID NS6_AnimResize( INT W, INT H );

Void NS6_AnimCopyFrame( HDC hDC );

VOID NS6_AnimRender( VOID );

VOID NS6_AnimUnitAdd( NS6_UNIT *Uni );

VOID NS6_AnimFlipFullScreen( VOID );

VOID NS6_AnimDoExit( VOID );

#endif