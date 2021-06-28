/* FILE NAME: u_ctrl.c
* PROGRAMMER: NS6
* DATE: 21.06.2021
* PURPOSE: 3D animation project.
*/

#include <stdio.h>
#include "units.h"

typedef struct tagns6UNIT_CONTROL
{
  NS6_UNIT_BASE_FIELDS;
  INT Speed;
  DBL AngleSpeed;
  FLT CamDist, RotX, RotY;
  VEC CamLoc, Dir, Up, At, Right;
} ns6UNIT_CONTROL;

static VOID NS6_UnitInit( ns6UNIT_CONTROL *Uni, ns6ANIM *Ani )
{
  Uni->CamLoc = VecSet(0, 0, 5);
  Uni->Up = VecSet(0, 1, 0);
  Uni->Dir = VecNormalize(VecSubVec(Uni->At, Uni->CamLoc));
  Uni->Right = VecNormalize(VecCrossVec(Uni->Dir, VecSet(0, 1, 0)));
  Uni->At = VecSet1(0);
  Uni->Speed = 5;
  Uni->AngleSpeed = 5;
}

static VOID NS6_UnitResponse( ns6UNIT_CONTROL *Uni, ns6ANIM *Ani )
{
  if (Ani->Keys['S'] || Ani->Keys['W'])
  {
    Uni->CamLoc = VecAddVec(Uni->CamLoc, VecMulNum(Uni->Dir, Ani->GlobalDeltaTime * Uni->Speed * (Ani->Keys['W'] - Ani->Keys['S'])));
  //  Uni->At = VecAddVec(Uni->At, VecMulNum(Uni->Dir, Ani->GlobalDeltaTime * Uni->Speed * (Ani->Keys['W'] - Ani->Keys['S'])));
  }
  if (Ani->Keys['A'] || Ani->Keys['D'])
    Uni->CamLoc = VecAddVec(Uni->CamLoc, VecMulNum(Uni->Right, Ani->GlobalDeltaTime * Uni->Speed * (Ani->Keys['D'] - Ani->Keys['A'])));

  if (Ani->Mdz)
    Uni->CamLoc = VecAddVec(Uni->CamLoc, VecMulNum(Uni->Dir, Ani->GlobalDeltaTime * Uni->Speed * Ani->Mdz * 0.03));

  if (Ani->Keys[VK_LBUTTON])
    Uni->CamLoc = PointTransform(Uni->CamLoc, MatrRotateY(-Ani->Keys[VK_LBUTTON] * Ani->GlobalDeltaTime * Uni->AngleSpeed * Ani->Mdx * 30));

  if (Ani->Keys[VK_RBUTTON])
    Uni->CamLoc = PointTransform(Uni->CamLoc, MatrRotateX(-Ani->Keys[VK_RBUTTON] * Ani->GlobalDeltaTime * Uni->AngleSpeed * Ani->Mdy * 30));

  if (Ani->Keys['Q'] || Ani->Keys['E'])
  {
    Uni->CamLoc.Y += Ani->GlobalDeltaTime * Uni->Speed * (Ani->Keys['E'] - Ani->Keys['Q']);
    Uni->At.Y += Ani->GlobalDeltaTime * Uni->Speed * (Ani->Keys['E'] - Ani->Keys['Q']);
  }
  if (Ani->Keys[VK_F1])
    NS6_AnimFlipFullScreen();
}

static VOID NS6_UnitRender( ns6UNIT_CONTROL *Uni, ns6ANIM *Ani )
{
  NS6_RndCamSet(Uni->CamLoc, Uni->At, Uni->Up);
}

ns6UNIT * NS6_UnitCreateControl( VOID )
{
  ns6UNIT *Uni;

  if ((Uni = NS6_AnimUnitCreate(sizeof(ns6UNIT_CONTROL))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)NS6_UnitInit;
  Uni->Response = (VOID *)NS6_UnitResponse;
  Uni->Render = (VOID *)NS6_UnitRender;

  return Uni;
}