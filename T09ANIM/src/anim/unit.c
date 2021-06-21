#include "../unit/units.h"
#include "../anim/anim.h"

static VOID NS6_UnitInit( NS6_UNIT * Uni, NS6_ANIM *Ani )
{
}

static VOID NS6_UnitClose( NS6_UNIT * Uni, NS6_ANIM *Ani )
{
}

static VOID NS6_UnitResponse( NS6_UNIT * Uni, NS6_ANIM *Ani )
{
}

static VOID NS6_UnitRender( NS6_UNIT * Uni, NS6_ANIM *Ani )
{
}

NS6_UNIT * NS6_AnimUnitCreate( INT Size )
{
  NS6_UNIT *Uni;

  if (Size < sizeof(NS6_UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;

  memset(Uni, 0, Size);

  Uni->Init = NS6_UnitInit;
  Uni->Close = NS6_UnitClose;
  Uni->Response = NS6_UnitResponse;
  Uni->Render = NS6_UnitRender;

  return Uni;
}