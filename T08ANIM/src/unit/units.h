#ifndef __units_h_
#define __units_h_

#include "../anim/anim.h"

ns6UNIT * NS6_UnitCreatDeer( VOID );

VOID NS6_UnitInit( ns6UNIT *Uni, ns6ANIM *Ani );

VOID NS6_UnitClose( ns6UNIT *Uni, ns6ANIM *Ani );

NS6_UnitResponse( ns6UNIT *Uni, ns6ANIM *Ani );

NS6_UnitRender( ns6UNIT *Uni, ns6ANIM *Ani );

#endif