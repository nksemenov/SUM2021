/* FILE NAME : rndmtl.c
 * PROGRAMMER: NM6
 * DATE      : 26.06.2021
 * PURPOSE   : .
 */

#include "rndres.h"
#include <glew.h>
#include <gl/glu.h>

/*INT NM6_RndMaterialsSize;
nm6MATERIAL NM6_RndMaterials[NM6_MAX_MATERIALS];
  */
VOID NM6_RndMtlInit( VOID )
{
  INT i;
  nm6MATERIAL mtl = {{0}};

  strcpy(mtl.Name, "DEFAULT");
  mtl.Ka = VecSet1(0.1);
  mtl.Kd = VecSet1(1);
  mtl.Ks = VecSet1(0.0);
  mtl.Ph = 30;
  mtl.Trans = 1;
  mtl.ShdNo = 0;
  for (i = 0; i < 8; i++)
    mtl.Tex[i] = -1;

  NM6_RndMaterialsSize = 0;
  NM6_RndMtlAdd(&mtl);
} /* End of 'NM6_RndMtlInit' function */

VOID NM6_RndMtlClose( VOID )
{
  NM6_RndMaterialsSize = 0;
} /* End of 'NM6_RndMtlClose' function */

INT NM6_RndMtlAdd( nm6MATERIAL *Mtl )
{
  /*
  INT i;
  
  for (i = 0; i < NM6_RndMaterialsSize; i++)
    if (strcmp(Mtl, NM6_RndMaterials[i].Name) == 0)
      return i;
  */
  if (NM6_RndMaterialsSize >= NM6_MAX_MATERIALS)
    return 0;
  NM6_RndMaterials[NM6_RndMaterialsSize] = *Mtl;
  return NM6_RndMaterialsSize++;
} /* End of 'NM6_RndMtlAdd' function */

/* Get default material function */
nm6MATERIAL NM6_RndMtlGetDeg( VOID )
{
  return NM6_RndMaterials[0];
} /* End of 'NM6_RndMtlGetDef' function */

/* Get material pointer function */
nm6MATERIAL * NM6_RndMtlGet( INT MtlNo )
{
  if (MtlNo < 0 || MtlNo >= NM6_RndMaterialsSize)
    MtlNo = 0;
  return &NM6_RndMaterials[MtlNo];
} /* End of 'NM6_RndMtlGet' function */

INT NM6_RndMtlApply( INT MtlNo )
{
  INT prg, loc, i;
  nm6MATERIAL *mtl;

  /* Set material pointer */
  mtl = NM6_RndMtlGet(MtlNo);

  /* Set shader program Id */
  prg = mtl->ShdNo;
  if (prg < 0 || prg >= NM6_RndShadersSize)
    prg = 0;
  prg = NM6_RndShaders[prg].ProgId;

  glUseProgram(prg);
  /* Set shading parameters */
  if ((loc = glGetUniformLocation(prg, "Time")) != -1)
    glUniform1f(loc, NM6_Anim.Time);
  if ((loc = glGetUniformLocation(prg, "GlobalTime")) != -1)
    glUniform1f(loc, NM6_Anim.GlobalTime);
  if ((loc = glGetUniformLocation(prg, "Ka")) != -1)
    glUniform3fv(loc, 1, &mtl->Ka.X);
  if ((loc = glGetUniformLocation(prg, "Kd")) != -1)
    glUniform3fv(loc, 1, &mtl->Kd.X);
  if ((loc = glGetUniformLocation(prg, "Ks")) != -1)
    glUniform3fv(loc, 1, &mtl->Ks.X);
  if ((loc = glGetUniformLocation(prg, "Ph")) != -1)
    glUniform1f(loc, mtl->Ph);
  if ((loc = glGetUniformLocation(prg, "Trans")) != -1)
    glUniform1f(loc, mtl->Trans);

  /* Set textures */
  for (i = 0; i < 8; i++)
  {
    CHAR tname[] = "IsTexture0";  /* --> shader: uniform bool IsTexture2; */

    tname[9] = '0' + i;
    if (mtl->Tex[i] != -1)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, NM6_RndTextures[mtl->Tex[i]].TexId);
    }
    if ((loc = glGetUniformLocation(prg, tname)) != -1)
      glUniform1i(loc, mtl->Tex[i] != -1);
  }
  return prg;
} /* End of 'NM6_RndMtlApply' function */

VOID NM6_RndMtlFree( INT MtlNo )
{
} /* End of 'NM6_RndMtlFree' function */

/* END OF 'rndmtl.c' FILE */