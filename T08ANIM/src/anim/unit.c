#include "../unit/units.h"
//#include "anim.h"


static VOID NS6_UnitInit( ns6UNIT *Uni, ns6ANIM *Ani )
{
}

static VOID NS6_UnitClose( ns6UNIT *Uni, ns6ANIM *Ani )
{
}

static VOID NS6_UnitResponse( ns6UNIT *Uni, ns6ANIM *Ani )
{
}

static VOID NS6_UnitRender( ns6UNIT *Uni, ns6ANIM *Ani )
{
}

ns6UNIT * NS6_AnimUnitCreate( INT Size )
{
  ns6UNIT *Uni;

  if (Size < sizeof(ns6UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;

  memset(Uni, 0, Size);

  Uni->Init = NS6_UnitInit;
  Uni->Close = NS6_UnitClose;
  Uni->Response = NS6_UnitResponse;
  Uni->Render = NS6_UnitRender;

  return Uni;
}