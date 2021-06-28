/* FILE NAME: u_btr.c
* PROGRAMMER: NS6
* DATE: 19.06.2021
* PURPOSE:
*/

#include <time.h>
#include "units.h"

/* typedef ns6UNIT_BTR */
typedef struct tagns6UNIT_BTR
{
  NS6_UNIT_BASE_FIELDS;
  ns6PRIMS Btr;
  VEC Pos;
} ns6UNIT_BTR;

/* Btr unit initialization function
* ARGUMENTS:
*   - self-pointer to unit object
*       ns6UNIT_BTR *Uni;
*   - animation context:
*       ns6ANIM *Ani;
* RETURNS: None.
*/
static VOID NS6_UnitInit( ns6UNIT_BTR *Uni, ns6ANIM *Ani )
{
  Uni->Pos = VecSet(150, 0, 0);
  NS6_RndPrimsLoad(&Uni->Btr, "BIN/MODELS/btr.g3dm");
}
/* End of 'NS6_UnitInit' function */

/* Unit deinitialization function
* ARGUMENTS:
*   - self-pointer to unit object
*       ns6UNIT_BTR *Uni;
*   - animation context:
*       ns6ANIM *Ani;
* RETURNS: None.
*/
static VOID NS6_UnitClose( ns6UNIT_BTR *Uni, ns6ANIM *Ani )
{
  NS6_RndPrimsFree(&Uni->Btr);
}
/* End of 'NS6_UnitClose' function */

/* Btr unit inter frame events handle function
* ARGUMENTS:
*   - self-pointer to unit object
*       ns6UNIT_BTR *Uni;
*   - animation context:
*       ns6ANIM *Ani;
* RETURNS: None.
*/
static VOID NS6_UnitResponse( ns6UNIT_BTR *Uni, ns6ANIM *Ani )
{
}
/* End of 'NS6_UnitResponse' function */


/* Bounce btr unit render function
* ARGUMENTS:
*   - self-pointer to unit object
*       ns6UNIT_BTR *Uni;
*   - animation context:
*       ns6ANIM *Ani;
* RETURNS: None.
*/
static VOID NS6_UnitRender( ns6UNIT_BTR *Uni, ns6ANIM *Ani )
{
  Uni->Btr.Prims[10].Trans = MatrRotateX(90 * Ani->Time);
  NS6_RndPrimsDraw(&Uni->Btr, MatrMulMatr(MatrTranslate(Uni->Pos), MatrScale(VecSet((FLT)0.01, (FLT)0.01, (FLT)0.01))));
}
/* End of 'NS6_UnitRender' function */


/* Unit btr creation function
* ARGUMENTS: None.
* RETURNS:
*   (ns6UNIT *) pointer to created unit
*/
ns6UNIT * NS6_UnitCreateBtr( VOID )
{
  ns6UNIT *Uni;

  if ((Uni = NS6_AnimUnitCreate(sizeof(ns6UNIT_BTR))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)NS6_UnitInit;
  Uni->Close = (VOID *)NS6_UnitClose;
  Uni->Response = (VOID *)NS6_UnitResponse;
  Uni->Render = (VOID *)NS6_UnitRender;

  return Uni;
}
/* End of 'NS6_UnitCreateBtr' function */