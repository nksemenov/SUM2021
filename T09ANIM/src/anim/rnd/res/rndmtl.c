#include "../rnd.h"

VOID NS6_RndMtlInit( VOID )
{
  ns6MATERIAL mtl = {{0}};

  strcpy(mtl.Name, "DEFAULT");
  mtl.Ka = VecSet1(1.0);
  mtl.Kd = VecSet1(1.0);
  mtl.Ks = VecSet1(0.0);
  mtl.Ph = 50;
  mtl.Trans = 1;
  mtl.ShdNo = 0;

  NS6_RndMaterialSize = 0;
  NS6_RndMtlAdd(&mtl);
}

VOID NS6_RndMtlClose( VOID )
{
  NS6_RndMaterialSize = 0;
}

INT NS6_RndMtlAdd( ns6MATERIAL *mtl )
{
  if (NS6_RndMaterialSize >= NS6_MAX_MATERIALS)
    return -1;
  NS6_RndMaterials[NS6_RndMaterialSize] = *mtl;
  return NS6_RndMaterialSize++;
}

INT NS6_RndMtlApply( INT MtlNo )
{
  INT prg, loc;
  ns6MATERIAL *mtl;

  /* Set material pointer */
  if (MtlNo < 0 || MtlNo >= NS6_RndMaterialSize)
    MtlNo = 0;
  mtl = &NS6_RndMaterials[MtlNo];

  /* Set program id */
  prg = mtl->ShdNo;
  if (prg < 0 || MtlNo >= NS6_RndShadersSize)
    prg = NS6_RndShaders[0].ProgId;
  else
    prg = NS6_RndShaders[prg].ProgId;

  glUseProgram(prg);
  if ((loc = glGetUniformLocation(prg, "Ka")) != -1)
    glUniform3fv(loc, 1, &mtl->Ka.X);
  if ((loc = glGetUniformLocation(prg, "Kd")) != -1)
    glUniform3fv(loc, 1, &mtl->Kd.X);
  if ((loc = glGetUniformLocation(prg, "Ks")) != -1)
    glUniform3fv(loc, 1, &mtl->Ks.X);
  if ((loc = glGetUniformLocation(prg, "Trans")) != -1)
    glUniform1f(loc, mtl->Trans);
  if ((loc = glGetUniformLocation(prg, "Ph")) != -1)
    glUniform1f(loc, mtl->Ph);


  return prg;
}