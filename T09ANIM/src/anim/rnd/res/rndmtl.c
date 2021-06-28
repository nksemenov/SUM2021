#include "../rnd.h"

ns6MATERIAL NS6_RndMaterial[NS6_MAX_MATERIALS];
INT NS6_RndMaterialSize;

INT NS6_RndMtlAdd( ns6MATERIAL *Mtl )
{
  if (NS6_RndMaterialSize >= NS6_MAX_MATERIALS)
    return 0;
  NS6_RndMaterial[NS6_RndMaterialSize] = *Mtl;
  return NS6_RndMaterialSize++;
} 

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

INT NS6_RndMtlApply( INT MtlNo )
{
  INT prg, loc, i;
  ns6MATERIAL *mtl;

  /* Set material pointer */
  if (MtlNo < 0 || MtlNo >= NS6_RndMaterialSize)
    MtlNo = 0;
  mtl = &NS6_RndMaterial[MtlNo];

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

  /* Set textures */
  for (i = 0; i < 8; i++)
  {
    CHAR tname[] = "IsTexture0";  /* --> shader: uniform bool IsTexture2; */

    tname[9] = '0' + i;
    if (mtl->Tex[i] != -1)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      glBindTexture(GL_TEXTURE_2D, NS6_RndTextures[mtl->Tex[i]].TexId);
    }
    if ((loc = glGetUniformLocation(prg, tname)) != -1)
      glUniform1i(loc, mtl->Tex[i] != -1);
  }
  return prg;
}