/* FILE NAME: u_ball.c
* PROGRAMMER: nm6
* DATE: 19.06.2021
* PURPOSE:
*/

#include "units.h"

/* typedef nm6UNIT_COW */
typedef struct
{
  NM6_UNIT_BASE_FIELDS;
  nm6PRIM Pr;
  MATR Cow;
  VEC Dir;
  VEC Pos;
} nm6UNIT_COW;

/* Cow unit initialization function
* ARGUMENTS:
* - self-pointer to unit object
* nm6UNIT_BALL *Uni;
* - animation context:
* nm6ANIM *Ani;
* RETURNS: None.
*/
static VOID NM6_UnitInit( nm6UNIT_COW *Uni, nm6ANIM *Ani )
{
  NM6_RndPrimLoad(&Uni->Pr, "BIN/MODELS/cow (2).obj");
  Uni->Pos = VecSet1(0);
  Uni->Dir = VecSet(5, 0, 0);
  Uni->Cow = MatrMulMatr(MatrScale(VecSet1(0.4)), MatrTranslate(VecSet(-0.6, -0.5, 0)));
}/* End of 'NM6_UnitInit' function */


/* Cow unit inter frame events handle function
* ARGUMENTS:
* - self-pointer to unit object
* nm6UNIT_BALL *Uni;
* - animation context:
* nm6ANIM *Ani;
* RETURNS: None.
*/
static VOID NM6_UnitResponse( nm6UNIT_COW *Uni, nm6ANIM *Ani )
{
  Uni->Cow = MatrMulMatr(Uni->Cow, MatrRotate(Ani->DeltaTime * 500 * (Ani->Keys['A'] - Ani->Keys['D']), VecSet(2, 0, 0)));
  Uni->Dir = VectorTransform(Uni->Dir, MatrRotate(Ani->DeltaTime * 100 * (Ani->Keys['A'] - Ani->Keys['D']), VecSet(2, 0, 0)));
  Uni->Pos = VecAddVec(Uni->Pos, VecMulNum(Uni->Dir, Ani->DeltaTime * 10 *(Ani->Keys['W'] - Ani->Keys['S'])));
}/* End of 'NM6_UnitResponse' function */
/* Bounce ball unit render function
* ARGUMENTS:
* - self-pointer to unit object
* nm6UNIT_BALL *Uni;
* - animation context:
* nm6ANIM *Ani;
* RETURNS: None.
*/

static VOID NM6_UnitRender( nm6UNIT_COW *Uni, nm6ANIM *Ani )
{
  NM6_RndPrimDraw(&Uni->Pr, MatrRotateZ(180 * sin(Ani->Time)));
  NM6_RndPrimDraw(&Uni->Pr, MatrRotateY(180 * sin(Ani->Time)));
  NM6_RndPrimDraw(&Uni->Pr, MatrRotateX(180 * sin(Ani->Time)));
  NM6_RndPrimDraw(&Uni->Pr, MatrRotateZ(180 * -sin(Ani->Time)));
  NM6_RndPrimDraw(&Uni->Pr, MatrRotateY(180 * -sin(Ani->Time)));
  NM6_RndPrimDraw(&Uni->Pr, MatrRotateX(180 * -sin(Ani->Time)));
}/* End of 'nm6_UnitRender' function */


/* Unit ball creation function
* ARGUMENTS: None.
* RETURNS:
* (nm6UNIT *) pointer to created unit
*/
nm6UNIT * NM6_UnitCreateCow( VOID )
{
  nm6UNIT *Uni;

  if ((Uni = NM6_AnimUnitCreate(sizeof(nm6UNIT_COW))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)NM6_UnitInit;
  Uni->Response = (VOID *)NM6_UnitResponse;
  Uni->Render = (VOID *)NM6_UnitRender;

  return Uni;
}/* End of 'nm6_UnitCreateCow' function */
