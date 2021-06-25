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
  Uni->CamLoc = VecSet(0, 0, 5);
  Uni->CamUp = VecSet(0, 1, 0);
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

  Uni->CamDir = VecSet(0, 0, 1);
  Uni->CamLoc = VecAddVec(Uni->CamLoc, VecMulNum(Uni->CamDir, Ani->DeltaTime * 50 * (Ani->Keys['S'] - Ani->Keys['W'])));
  NS6_RndCamSet( Uni->CamLoc, Uni->CamDir, Uni->CamUp );

  Uni->CamDir = VecSet(0, 1, 0);
  Uni->CamLoc = VecAddVec(Uni->CamLoc, VecMulNum(Uni->CamDir, Ani->DeltaTime * 50 * (Ani->Keys['E'] - Ani->Keys['Q'])));
  NS6_RndCamSet( Uni->CamLoc, Uni->CamDir, Uni->CamUp );
  
  Uni->CamDir = VecSet(1, 0, 0);
  Uni->CamLoc = VecAddVec(Uni->CamLoc, VecMulNum(Uni->CamDir, Ani->DeltaTime * 50 * (Ani->Keys['A'] - Ani->Keys['D'])));
  NS6_RndCamSet( Uni->CamLoc, Uni->CamDir, Uni->CamUp );


}

static VOID NS6_UnitRender( ns6UNIT_CONTROL *Uni, ns6ANIM *Ani )
{
  static CHAR buf[100];
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