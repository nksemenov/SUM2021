/* FILE NAME: u_cow.c
* PROGRAMMER: NM6
* DATE: 21.06.2021
* PURPOSE:
*/

#include <stdio.h>
#include "../anim/anim.h"

/* typedef nm6INIT_ctrl */
typedef struct tagnm6UNIT_CONTROL
{
  NM6_UNIT_BASE_FIELDS;
  DBL CamDist, RotX, RotY;           
  BOOL IsWire;
} nm6UNIT_ctrl;

/* Cow unit initialization function
* ARGUMENTS:
* - self-pointer to unit object
* nm6UNIT_BALL *Uni;
* - animation context:
* nm6ANIM *Ani;
* RETURNS: None.
*/
static VOID NM6_UnitInit( nm6UNIT_ctrl *Uni, nm6ANIM *Ani )
{
  Uni->CamLoc = VecSet(0, 0, 30);
  Uni->CamDir = VecSet(0, 1, 0);
  Uni->Speed = 10;
}/* End of 'NM6_UnitInit' function */


/* Cow unit inter frame events handle function
* ARGUMENTS:
* - self-pointer to unit object
* nm6UNIT_BALL *Uni;
* - animation context:
* nm6ANIM *Ani;
* RETURNS: None.
*/
static VOID NM6_UnitResponse( nm6UNIT_ctrl *Uni, nm6ANIM *Ani )
{
  VEC ve;

  R = VecSet(NM6_RndMatrView.A[0][0], NM6_RndMatrView.A[1][0], NM6_RndMatrView.A[2][0]);
  D = VecSet(-NM6_RndMatrView.A[0][2], -NM6_RndMatrView.A[1][2], -NM6_RndMatrView.A[2][2]);
  D.Y = 0;
  D = VecNormalize(D);

  Uni->CamDist += Ani->GlobalDeltaTime * (0.2 * Ani->Mdz + 8 * (1 + Ani->Keys[VK_SHIFT] * 26) * (Ani->Keys[VK_NEXT] - Ani->Keys[VK_PRIOR]));
  Uni->RotY += Ani->GlobalDeltaTime * (-0.8 * 30 * Ani->Keys[VK_LBUTTON] * Ani->Mdx + 3 * 0.47 * (Ani->Keys[VK_LEFT] - Ani->Keys[VK_RIGHT]));
  Uni->RotX += Ani->GlobalDeltaTime * (-0.8 * 30 * Ani->Keys[VK_LBUTTON] * Ani->Mdy + 0.47 * (Ani->Keys[VK_UP] - Ani->Keys[VK_DOWN]));

  Uni->At = VecAddVec(Uni->At, VecMulNum(R, (Ani->Keys['D'] - Ani->Keys['A']) * 18 * Ani->GlobalDeltaTime));
  Uni->At = VecAddVec(Uni->At, VecMulNum(D, (Ani->Keys['W'] - Ani->Keys['S']) * 18 * Ani->GlobalDeltaTime));

  NM6_RndCamSet(PointTransform(VecSet(0, 0, Uni->CamDist),
                               MatrMulMatr3(MatrRotateX(-18 * 5 / 2.0 * Uni->RotX),
                                            MatrRotateY(-102 * 5 / 8.0 * Uni->RotY),
                                            MatrTranslate(Uni->At))),
    Uni->At, VecSet(0, 1, 0));
}/* End of 'NM6_UnitResponse' function */


/* Bounce ball unit render function
* ARGUMENTS:
* - self-pointer to unit object
* nm6UNIT_BALL *Uni;
* - animation context:
* nm6ANIM *Ani;
* RETURNS: None.
*/
static VOID NM6_UnitRender( VOID )
{
}/* End of 'NM6_UnitRender' function */


/* Unit ball creation function
* ARGUMENTS: None.
* RETURNS:
* (nm6UNIT *) pointer to created unit
*/
nm6UNIT * NM6_UnitCreateCtrl( VOID )
{
  nm6UNIT *Uni;

  if ((Uni = (nm6UNIT *)NM6_AnimUnitCreate(sizeof(nm6UNIT_ctrl))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)NM6_UnitInit;
  Uni->Response = (VOID *)NM6_UnitResponse;
  Uni->Render = (VOID *)NM6_UnitRender;

  return Uni;
}/* End of 'NM6_UnitCreateCtrl' function */