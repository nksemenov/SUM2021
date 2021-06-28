/* FILE NAME : u_gear.c
 * PROGRAMMER: ns6
 * DATE : 21.06.2021 
 * PURPOSE : Ball unit module
 */

#ifndef __u_tex_c_
#define __u_tex_c_

#include <stdio.h>

#include "units.h"

typedef struct tagns6UNIT_TEXPRIM
{
  NS6_UNIT_BASE_FIELDS;
  ns6PRIM Pr;           /* Primitive */
  INT TexId;            /* Texture OpenGL Id */
} ns6UNIT_TEXPRIM;

/* Cow unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ns6UNIT_Gear *Uni;
 *   - animation context:
 *       ns6ANIM *Ani;
 * RETURNS: None.
 */
static VOID NS6_UnitInit( ns6UNIT_TEXPRIM *Uni, ns6ANIM *Ani )
{
  ns6VERTEX V[] =
  {
    {{0, 0, 0}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{1, 0, 0}, {1, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{0, 1, 0}, {0, 1}, {0, 0, 1}, {1, 1, 1, 1}},
    {{1, 1, 0}, {1, 1}, {0, 0, 1}, {1, 1, 1, 1}},
  };
  FLT t[2][2] = 
  {
    {0.8, 1},
    {1, 0.3}
  };

  glGenTextures(1, &Uni->TexId);

  glBindTexture(GL_TEXTURE_2D, Uni->TexId);
  glTexImage2D(GL_TEXTURE_2D, 0, 1, 2, 2, 0, GL_LUMINANCE, GL_FLOAT, t);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  NS6_RndPrimCreateGrid(&Uni->Pr, 2, 2, V); 
} /* End of 'NS6_UnitInit' function */

/* Cow unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ns6UNIT_Gear *Uni;
 *   - animation context:
 *       ns6ANIM *Ani;
 * RETURNS: None.
 */
static VOID NS6_UnitResponse( ns6UNIT_TEXPRIM *Uni, ns6ANIM *Ani )
{
} /* End of 'NS6_UnitResponse' function */

/* Cow unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ns6UNIT_Gear *Uni;
 *   - animation context:
 *       ns6ANIM *Ani;
 * RETURNS: None.
 */
static VOID NS6_UnitClose( ns6UNIT_TEXPRIM *Uni, ns6ANIM *Ani )
{
  NS6_RndPrimFree(&Uni->Pr);
} /* End of 'NS6_UnitResponse' function */

/* Cow unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       ns6UNIT_Gear *Uni;
 *   - animation context:
 *       ns6ANIM *Ani;
 * RETURNS: None.
 */
static VOID NS6_UnitRender( ns6UNIT_TEXPRIM *Uni, ns6ANIM *Ani )
{
  glActiveTexture(GL_TEXTURE0 + 1);
  glBindTexture(GL_TEXTURE_2D, Uni->TexId);

  NS6_RndPrimDraw(&Uni->Pr, MatrIdentity());
} /* End of 'NS6_UnitRender' function */

/* Unit cow creation function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (ns6UNIT *) pointer to created unit.
 */
ns6UNIT * NS6_UnitCreateTexPrim( VOID )
{
  ns6UNIT *Uni;

  if ((Uni = (ns6UNIT *)NS6_AnimUnitCreate(sizeof(ns6UNIT_TEXPRIM))) == NULL)
    return NULL;

  /* Setup unit methods */
  Uni->Init = (VOID *)NS6_UnitInit;
  Uni->Response = (VOID *)NS6_UnitResponse;
  Uni->Render = (VOID *)NS6_UnitRender;
  Uni->Close = (VOID *)NS6_UnitClose;

  return Uni;
} /* End of 'NS6_UnitCreateGear' function */


/* END OF 'u_ball.c' FUNCTION */
#endif