/* FILE NAME: units.c
* PROGRAMMER: NM6
* DATE: 19.06.2021
* PURPOSE: 3D animation primitive handle module.
*/

#include "../anim/anim.h"

/* Unit initialization function.
* ARGUMENTS:
* -self-pointer to unit object:
* NM6UNIT *Uni;
* -animation context:
* NM6ANIM *Ani;
* RETURNS: None
*/
VOID NM6_UnitInit( nm6UNIT *Uni, nm6ANIM *Ani );

/* Unit deinitialization function.
* ARGUMENTS:
* -self-pointer to unit object:
* NM6UNIT *Uni;
* -animation context:
* NM6ANIM *Ani;
* RETURNS: None
*/
VOID NM6_UnitClose( nm6UNIT *Uni, nm6ANIM *Ani );


/* Unit inter frame events handle fucntion.
* ARGUMENTS:
* -self-pointer to unit object:
* NM6UNIT *Uni;
* -animation context:
* NM6ANIM *Ani;
* RETURNS: None
*/
VOID NM6_UnitResponse( nm6UNIT *Uni, nm6ANIM *Ani );

/* Unit render function.
* ARGUMENTS:
* -self-pointer to unit object:
* NM6UNIT *Uni;
* -animation context:
* NM6ANIM *Ani;
* RETURNS: None
*/
VOID NM6_UnitRenser( nm6UNIT *Uni, nm6ANIM *Ani );

/* Unit deinitialization function.
* ARGUMENTS:
* -self-pointer to unit object:
* NM6UNIT *Uni;
* -animation context:
* NM6ANIM *Ani;
* RETURNS: None
*/
VOID NM6_Unit( nm6UNIT *Uni, nm6ANIM *Ani );

/* Unit creation function
* ARGUMENTS:
* -unit structure size in bytes
* INT Size;
* RETURNS:
* (NM6UNIT *) pointer to created unit
*/
nm6UNIT * NM6_AnimUnitCreate( INT Size );

/* Unit ball creation function
* ARGUMENTS: None.
* RETURNS:
* (NM6UNIT *) pointer to created unit
*/
nm6UNIT * NM6_UnitCreateCow( VOID );

/* END OF 'units.c' FILE */