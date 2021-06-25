/* FILE NAME: u_deer.c
* PROGRAMMER: NS6
* DATE: 19.06.2021
* PURPOSE:
*/

#include <time.h>
#include "units.h"

/* typedef ns6UNIT_DEER */
typedef struct tagns6UNIT_DEER
{
  NS6_UNIT_BASE_FIELDS;
  ns6PRIM Deer;
  VEC Pos;
} ns6UNIT_DEER;

/* Deer unit initialization function
* ARGUMENTS:
* - self-pointer to unit object
* ns6UNIT_DEER *Uni;
* - animation context:
* ns6ANIM *Ani;
* RETURNS: None.
*/
static VOID NS6_UnitInit( ns6UNIT_DEER *Uni, ns6ANIM *Ani )
{
  Uni->Pos = VecSet(0, 0, 0);
  NS6_RndPrimLoad(&Uni->Deer, "deer.obj");
}
/* End of 'NS6_UnitInit' function */


/* Deer unit inter frame events handle function
* ARGUMENTS:
* - self-pointer to unit object
* ns6UNIT_DEER *Uni;
* - animation context:
* ns6ANIM *Ani;
* RETURNS: None.
*/
static VOID NS6_UnitResponse( ns6UNIT_DEER *Uni, ns6ANIM *Ani )
{
}
/* End of 'NS6_UnitResponse' function */


/* Bounce deer unit render function
* ARGUMENTS:
* - self-pointer to unit object
* ns6UNIT_DEER *Uni;
* - animation context:
* ns6ANIM *Ani;
* RETURNS: None.
*/
static VOID NS6_UnitRender( ns6UNIT_DEER *Uni, ns6ANIM *Ani )
{
  NS6_RndPrimDraw(&Uni->Deer, MatrScale(VecSet(0.01, 0.01, 0.01)));
}
/* End of 'NS6_UnitRender' function */


/* Unit deer creation function
* ARGUMENTS: None.
* RETURNS:
* (ns6UNIT *) pointer to created unit
*/
ns6UNIT * NS6_UnitCreateDeer( VOID )
{
  ns6UNIT *Uni;

  if ((Uni = (ns6UNIT *)NS6_AnimUnitCreate(sizeof(ns6UNIT_DEER))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)NS6_UnitInit;
  Uni->Response = (VOID *)NS6_UnitResponse;
  Uni->Render = (VOID *)NS6_UnitRender;

  return Uni;
}
/* End of 'NS6_UnitCreateDeer' function */