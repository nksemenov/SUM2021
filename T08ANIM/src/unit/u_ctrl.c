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
} ns6UNIT_CONTROL;

static VOID NS6_UnitInit( ns6UNIT_CONTROL *Uni, ns6ANIM *Ani )
{
  Uni->CamDist = 50;
  Uni->RotX = D2R(30);
  Uni->RotY = D2R(30);
}

static VOID NS6_UnitResponse( ns6UNIT_CONTROL *Uni, ns6ANIM *Ani )
{
  if (Ani->KeysClick[VK_F1])
    NS6_AnimFlipFullScreen();
  if (Ani->KeysClick['P'])
    Ani->IsPause = !Ani->IsPause;

  Uni->RotX += Ani->Mdx * Ani->GlobalDeltaTime * Ani->Keys[VK_LBUTTON];
  Uni->RotY += Ani->Mdy * Ani->GlobalDeltaTime * Ani->Keys[VK_LBUTTON];
  Uni->CamDist += Ani->Mdz * Ani->GlobalDeltaTime;
}

static VOID NS6_UnitRender( ns6UNIT_CONTROL *Uni, ns6ANIM *Ani )
{
  static CHAR buf[100];

  SetBkMode(Ani->hDC, TRANSPARENT);
  SetTextColor(Ani->hDC, RGB(0, 255, 0));
  TextOut(Ani->hDC, 10, 10, buf, sprintf(buf, "FPS: %3f", Ani->FPS));
  TextOut(Ani->hDC, 10, 30, buf, sprintf(buf, "X: %2d Y: %2d Z: %2d", Ani->Mx, Ani->My, Ani->Mz));

  NS6_RndCamSet(VecAddVec(VecSet(0, 0, 5), VecMulNum(VecSet(0, 0, 5), Ani->DeltaTime * 5 * (-Ani->Keys[VK_UP] + Ani->Keys[VK_DOWN]))), VecSet(0, 0, 0), VecSet(0, 1, 0));
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