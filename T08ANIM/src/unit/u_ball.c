# include <time.h>
#include "units.h"

typedef struct tagNS6_UNIT_BALL
{
  NS6_UNIT_BASE_FIELDS;
  NS6_PRIM Ball;
  DBL TimeShift, TimeScale;

} NS6_UNIT_BALL

NS6_UNIT * NS6_UnitCreatBounceBall( VOID )
{
  NS6_UNIT *Uni;

  if ((Uni = NS6_AnimUnitCreate(sizeof(NS6_UNIT_BALL))) == NULL)
    return NULL;

  Uni->Init = (VOID *)NS6_UnitInit;
  Uni->Close = (VOID *)NS6_UnitClose;
  Uni->Response = (VOID *)NS6_UnitResponse;
  Uni->Render = (VOID *)NS6_UnitRender;
  return Uni;
}

static VOID NS6_UnitInit( NS6_UNIT_BALL *Uni, NS6_ANIM * Ani )
{
  NS6_RndPrimCreateSphere(&Uni->Ball, VecSet(0, 0, 0), 1, 20, 10);
  Uni->Ball.Trans = MatrTranslate(VecSet(Rnd1() * 8, 0, Rnd1() * 8));
  Uni->TimeShift = rand() % 30 + 100;
  Uni->TimeScale = 1 + 0.1 + Rnd1();
}

static VOID NS6_UnitClose( NS6_UNIT_BALL * Uni, NS6_ANIM *Ani )
{
  NS6_RndPrimFree(&Uni->Ball);
}

VOID NS6_UnitResponse( NS6_UNIT *Uni, NS6_ANIM * Ani )
{
}

VOID NS6_UnitRender( NS6_UNIT_BALL * Uni, NS6_ANIM *Ani )
{
  DBL t = clock() / 1000;

  NS6_RndPrimDraw(&Uni->Ball, MatrTranslate(VecSet(0, 3 * fabs(sin(Uni->TimeShift + Uni->TimeScale * 8 * t)), 0)));
}