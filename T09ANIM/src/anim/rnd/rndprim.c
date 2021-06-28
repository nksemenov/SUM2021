#include <stdio.h>
#include <string.h>
#include "../anim.h"

VOID NS6_RndPrimCreate( ns6PRIM *Pr, ns6VERTEX *V, INT NumOfV, INT *I, INT NumOfI )
{
  memset(Pr, 0, sizeof(ns6PRIM));

  if (V != NULL && NumOfV != 0)
  {
    glGenBuffers(1, &Pr->VBuf);
    glGenVertexArrays(1, &Pr->VA);

    glBindVertexArray(Pr->VA);
    glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ns6VERTEX) * NumOfV, V, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(ns6VERTEX), (VOID *)0);                                 /* position */
    glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(ns6VERTEX), (VOID *)sizeof(VEC));                       /* texture coordinates */
    glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(ns6VERTEX), (VOID *)(sizeof(VEC) + sizeof(VEC2)));      /* normal */
    glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(ns6VERTEX), (VOID *)(sizeof(VEC) * 2 + sizeof(VEC2)));  /* color */

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glBindVertexArray(0);
  }

  if (I != NULL && NumOfI != 0)
  {
    glGenBuffers(1, &Pr->IBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NumOfI, I, GL_STATIC_DRAW);
    Pr->NumOfElements = NumOfI;
  }
  else
    Pr->NumOfElements = NumOfV;
  Pr->Trans = MatrIdentity();
} /* End of 'NS6_RndPrimCreate' function */

/* NS6_RndPrimFree */
VOID NS6_RndPrimFree( ns6PRIM *Pr )
{
  glBindVertexArray(Pr->VA);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Pr->VBuf);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Pr->VA);

  glDeleteBuffers(1, &Pr->IBuf);

  memset(Pr, 0, sizeof(ns6PRIM));
}/* NS6_PrimFree */

/* NS6_PrimDraw */
VOID NS6_RndPrimDraw( ns6PRIM *Pr, MATR World )
{
  MATR
    wvp = MatrMulMatr(Pr->Trans, MatrMulMatr(World, NS6_RndMatrVP)),
    w = MatrMulMatr(Pr->Trans, World),
    winv = MatrTranspose(MatrInverse(w));
  INT ProgId = NS6_RndMtlApply(Pr->MtlNo), loc;
  INT gl_prim_type = Pr->Type == NS6_RND_PRIM_LINES ? GL_LINES :
                   Pr->Type == NS6_RND_PRIM_TRIMESH ? GL_TRIANGLES :
                   Pr->Type == NS6_RND_PRIM_TRISTRIP ? GL_TRIANGLE_STRIP :
                   GL_POINTS;
  
  if ((loc = glGetUniformLocation(ProgId, "MatrWVP")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, wvp.A[0]);
  if ((loc = glGetUniformLocation(ProgId, "MatrW")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, w.A[0]);
  if ((loc = glGetUniformLocation(ProgId, "MatrWInv")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, winv.A[0]);
  if ((loc = glGetUniformLocation(ProgId, "CamLoc")) != -1)
    glUniform3fv(loc, 1, &NS6_RndCamLoc.X);
  if ((loc = glGetUniformLocation(ProgId, "Time")) != -1)
    glUniform1f(loc, NS6_Anim.Time);

  /* Send matrix to OpenGL /v.1.0 */
  glLoadMatrixf(wvp.A[0]);

  glBindVertexArray(Pr->VA);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
  glDrawElements(GL_TRIANGLES, Pr->NumOfElements, GL_UNSIGNED_INT, NULL);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glUseProgram(0);
}/* End of 'NS6_PrimDraw' function */

VOID NS6_RndPrimCreateGrid( ns6PRIM *Pr, INT Grid_W, INT Grid_H, ns6VERTEX *V )
{
  INT i, j, k;
  INT *Ind;

  if ((Ind = malloc(sizeof(INT) * (Grid_W - 1) * (Grid_H - 1) * 6)) == NULL)
    return;

  for (k = 0, i = 0; i < Grid_H - 1; i++)
    for (j = 0; j < Grid_W - 1; j++)
    {
      Ind[k++] = i * Grid_W + j;
      Ind[k++] = i * Grid_W + j + 1;
      Ind[k++] = (i + 1) * Grid_W + j;

      Ind[k++] = (i + 1) * Grid_W + j;
      Ind[k++] = i * Grid_W + j + 1;
      Ind[k++] = (i + 1) * Grid_W + j + 1;
    }

  //NS6_RndPrimGridEvalNormals(Grid_W, Grid_H, V);
  NS6_RndPrimCreate(Pr, NS6_RND_PRIM_TRIMESH, V, Grid_W * Grid_H, Ind, (Grid_W - 1) * (Grid_H - 1) * 6);
  free(Ind);
} /* End of 'NS6_RndPrimCreateGrid' function */



/* Load primitive from '*.OBJ' file function.
 * ARGUMENTS:
 *   - pointer to primitive to load:
 *       ns6PRIM *Pr;
 *   - '*.OBJ' file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */

BOOL NS6_RndPrimLoad( ns6PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  INT i, nv = 0, nind = 0, size;
  ns6VERTEX *V;
  INT *Ind;
  static CHAR Buf[1000];

  memset(Pr, 0, sizeof(ns6PRIM));
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;

  /* Count vertexes and indexes */
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n = 0;

      for (i = 1; Buf[i] != 0; i++)
        if (isspace((UCHAR)Buf[i - 1]) && !isspace((UCHAR)Buf[i]))
          n++;
      nind += (n - 2) * 3;
    }
  }

  size = sizeof(ns6VERTEX) * nv + sizeof(INT) * nind;
  if ((V = malloc(size)) == NULL)
    return FALSE;
  Ind = (INT *)(V + nv);
  memset(V, 0, size);

  /* Load primitive */
  rewind(F);
  nv = 0;
  nind = 0;
  while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      DBL x, y, z;
      VEC4 c = {Rnd0(), Rnd0(), Rnd0(), 1};

      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      V[nv].C = c;
      V[nv++].P = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      INT n = 0, n0, n1, nc;

      for (i = 1; Buf[i] != 0; i++)
        if (isspace((UCHAR)Buf[i - 1]) && !isspace((UCHAR)Buf[i]))
        {
          sscanf(Buf + i, "%i", &nc);
          if (nc < 0)
            nc = nv + nc;
          else
            nc--;

          if (n == 0)
            n0 = nc;
          else if (n == 1)
            n1 = nc;
          else
          {
            Ind[nind++] = n0;
            Ind[nind++] = n1;
            Ind[nind++] = nc;
            n1 = nc;
          }
          n++;
        }
    }
  }
  fclose(F);
  NS6_RndPrimCreate(Pr, V, nv, Ind, nind);
  free(V);
  return TRUE;
}
/* End of 'NS6_RndPrimLoad' function */