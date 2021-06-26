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
  FLT CamDist, RotX, RotY;
  VEC CamLoc, CamDir, CamUp;
} ns6UNIT_CONTROL;

static VOID NS6_UnitInit( ns6UNIT_CONTROL *Uni, ns6ANIM *Ani )
{
  Uni->CamDist = 50;
  Uni->RotX = (FLT)D2R(30);
  Uni->RotY = (FLT)D2R(30);
  Uni->CamLoc = VecSet(0, 0, 10);
  Uni->CamUp = VecSet(0, 1, 0);
  Uni->CamDir = VecSet(0, 1, 0);
}

static VOID NS6_UnitResponse( ns6UNIT_CONTROL *Uni, ns6ANIM *Ani )
{
  if (Ani->KeysClick[VK_F1])
    NS6_AnimFlipFullScreen();
  if (Ani->Keys['P'])
    Ani->IsPause = !Ani->IsPause;

  Uni->RotX += Ani->Mdx * Ani->GlobalDeltaTime * Ani->Keys[VK_LBUTTON];
  Uni->RotY += Ani->Mdy * Ani->GlobalDeltaTime * Ani->Keys[VK_LBUTTON];
  Uni->CamDist += Ani->Mdz * Ani->GlobalDeltaTime;

  Uni->CamLoc = PointTransform(Uni->CamLoc, MatrRotateX(Ani->DeltaTime * 60 * (Ani->Keys['S'] - Ani->Keys['W'])));
  Uni->CamLoc = PointTransform(Uni->CamLoc, MatrRotateY(Ani->DeltaTime * 60 * (Ani->Keys['D'] - Ani->Keys['A'])));
  if (Ani->Keys['F'] || Ani->Keys['R'])
  {
    Uni->CamLoc = VecDivNum(Uni->CamLoc, (Ani->DeltaTime * abs(Ani->Keys['F'] - Ani->Keys['R']) * 100));
    Uni->CamLoc.X *= 1;
  }
/*  Uni->CamLoc = VecAddVec(Uni->CamLoc, VecMulNum(VecSet(0, 0, 1), Ani->DeltaTime * 30 * (Ani->Keys['S'] - Ani->Keys['W'])));
  Uni->CamLoc = VecAddVec(Uni->CamLoc, VecMulNum(VecSet(0, 1, 0), Ani->DeltaTime * 30 * (Ani->Keys['R'] - Ani->Keys['F'])));
  Uni->CamLoc = VecAddVec(Uni->CamLoc, VecMulNum(VecSet(1, 0, 0), Ani->DeltaTime * 30 * (Ani->Keys['D'] - Ani->Keys['A'])));

  Uni->CamDir = VecAddVec(Uni->CamDir, VecSet(-sin(Ani->DeltaTime * 3 * (Ani->Keys['Q'] - Ani->Keys['E'])), 0, -cos(Ani->DeltaTime * 3 *(Ani->Keys['Q'] - Ani->Keys['E']))));
  Uni->CamDir.X -= 2 * (INT)Uni->CamDir.X ;
  Uni->CamDir.Y -= 2 * (INT)Uni->CamDir.Y ;
  Uni->CamDir.Z -= 2 * (INT)Uni->CamDir.Z ;*/
  NS6_RndCamSet(Uni->CamLoc, Uni->CamDir, Uni->CamUp);
}

static VOID NS6_UnitRender( ns6UNIT_CONTROL *Uni, ns6ANIM *Ani )
{
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