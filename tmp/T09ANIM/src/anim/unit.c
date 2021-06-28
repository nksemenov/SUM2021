/* FILE NAME: unit.c
* PROGRAMMER: NM6
* DATE: 19.06.2021
* PURPOSE: 3D animation primitive handle module.
*/

#include "anim.h"

/* Unit initialization function.
* ARGUMENTS:
* -self-pointer to unit object:
* NM6UNIT *Uni;
* -animation context:
* NM6ANIM *Ani;
* RETURNS: None
*/
static VOID NM6_UnitInit( nm6UNIT *Uni, nm6ANIM *Ani )
{
}/* End of 'NM6_UnitIit' function */


/* Unit deinitialization function.
* ARGUMENTS:
* -self-pointer to unit object:
* NM6UNIT *Uni;
* -animation context:
* NM6ANIM *Ani;
* RETURNS: None
*/
static VOID NM6_UnitClose( nm6UNIT *Uni, nm6ANIM *Ani )
{
}/* End of 'NM6_UnitResponse' function */


/* Unit inter frame events handle fucntion.
* ARGUMENTS:
* -self-pointer to unit object:
* NM6UNIT *Uni;
* -animation context:
* NM6ANIM *Ani;
* RETURNS: None
*/
static VOID NM6_UnitResponse( nm6UNIT *Uni, nm6ANIM *Ani )
{
}/* End of 'NM6_UnitResponse' function */


/* Unit render function.
* ARGUMENTS:
* -self-pointer to unit object:
* NM6UNIT *Uni;
* -animation context:
* NM6ANIM *Ani;
* RETURNS: None
*/
static VOID NM6_UnitRender( nm6UNIT *Uni, nm6ANIM *Ani )
{
}/* End of 'NM6_UnitRender' function */


/* Unit deinitialization function.
* ARGUMENTS:
* -self-pointer to unit object:
* NM6UNIT *Uni;
* -animation context:
* NM6ANIM *Ani;
* RETURNS: None
*/
static VOID NM6_Unit( nm6UNIT *Uni, nm6ANIM *Ani )
{
}/* End of 'NM6_UnitClose' function */


/* Unit creation function
* ARGUMENTS:
* -unit structure size in bytes
* INT Size;
* RETURNS:
* (NM6UNIT *) pointer to created unit
*/
nm6UNIT * NM6_AnimUnitCreate( INT Size )
{
  nm6UNIT *Uni;
  /* Memory allocation */
  if (Size < sizeof(nm6UNIT) || (Uni = malloc(Size)) == NULL)
    return NULL;
  memset(Uni, 0, Size);

  /* Setup unit methods */
  Uni->Init = NM6_UnitInit;
  Uni->Close = NM6_UnitClose;
  Uni->Response = NM6_UnitInit;
  Uni->Render = NM6_UnitRender;

  return Uni;
}/* End of 'NM6_AnimUnitCreate' function */


/* END OF 'unit.' FILE */