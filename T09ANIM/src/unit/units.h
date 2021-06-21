#ifndef __units_h_
#define __units_h_

#include "../anim/anim.h"

NS6_UNIT * NS6_UnitCreatBounceBall( VOID );

VOID NS6_UnitInit( NS6_BALL *Uni, NS6_ANIM * Ani );

VOID NS6_UnitClose( NS6_UNIT_BALL * Uni, NS6_ANIM *Ani );

NS6_UnitResponse( NS6_UNIT *Uni, NS6_ANIM * Ani );

NS6_UnitRender( NS6_UNIT_BALL * Uni, NS6_ANIM *Ani );

#endif