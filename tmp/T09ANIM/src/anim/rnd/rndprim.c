/* FILE NAME: rndprim.c
* PROGRAMMER: nm6
* DATE: 18.06.2021
* PURPOSE: 3D animation primitive handle module.
*/

#include <stdio.h>
#include "rnd.h"

/* Primitive creation function.
 * ARGUMENTS:
 *   - primitive pointer:
 *       nm6PRIM *Pr;
 *   - vertex attributes array:
 *       nm6VERTEX *V;
 *   - number of vertices:
 *       INT NumOfV;
 *   - index array (for trimesh – by 3 ones, may be NULL)
 *       INT *I;
 *   - number of indices
 *       INT NumOfI;
 * RETURNS: None.
 */
VOID NM6_RndPrimCreate( nm6PRIM *Pr, nm6VERTEX *V, INT NumOfV, INT *I, INT NumOfI )
{
  memset(Pr, 0, sizeof(nm6PRIM));

  if (V != NULL && NumOfV != 0)
  {
    glGenBuffers(1, &Pr->VBuf);
    glGenVertexArrays(1, &Pr->VA);

    glBindVertexArray(Pr->VA);
    glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
    glBufferData(GL_ARRAY_BUFFER, sizeof(nm6VERTEX) * NumOfV, V, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(nm6VERTEX),
                          (VOID *)0); /* position */
    glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(nm6VERTEX),
                          (VOID *)sizeof(VEC)); /* texture coordinates */
    glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(nm6VERTEX),
                          (VOID *)(sizeof(VEC) + sizeof(VEC2))); /* normal */
    glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(nm6VERTEX),
                          (VOID *)(sizeof(VEC) * 2 + sizeof(VEC2))); /* color */

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
} /* End of 'NM6_RndPrimCreate' function */

/* NM6_RndPrimFree */
VOID NM6_RndPrimFree( nm6PRIM *Pr )
{
  glBindVertexArray(Pr->VA);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Pr->VBuf);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Pr->VA);

  glDeleteBuffers(1, &Pr->IBuf);

  memset(Pr, 0, sizeof(nm6PRIM));
}/* NM6_PrimFree */

/* NM6_PrimDraw */
VOID NM6_RndPrimDraw( nm6PRIM *Pr, MATR World )
{
  MATR wvp = MatrMulMatr3(Pr->Trans, World, NM6_RndMatrVP);

  INT ProgId = NM6_RndShaders[0].ProgId, loc;

  glUseProgram(ProgId);

  if ((loc = glGetUniformLocation(ProgId, "MatrWVP")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, wvp.A[0]);
  /*if ((loc = glGetUniformLocation(ProgId, "Time")) != -1)
    glUniform1f(loc, nm6ANIM.Time); */

  /* Send matrix to OpenGL /v.1.0 */
  glLoadMatrixf(wvp.A[0]);

  glBindVertexArray(Pr->VA);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
  glDrawElements(GL_TRIANGLES, Pr->NumOfElements, GL_UNSIGNED_INT, NULL);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glUseProgram(0);
}/* End of 'NM6_PrimDraw' function */

/* NM6_RndPrimLoad */
BOOL NM6_RndPrimLoad( nm6PRIM *Pr, CHAR *FileName )
{
  FILE *F;                                          
  INT i, nv = 0, nind = 0, size;
  nm6VERTEX *V;
  INT *Ind;
  static CHAR Buf[1000];

  memset(Pr, 0, sizeof(nm6PRIM));
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
        if (Buf[i - 1] == ' ' && Buf[i] != ' ')
          n++;
      nind += (n - 2) * 3;
    }
  }

  size = sizeof(nm6VERTEX) * nv + sizeof(INT) * nind;
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

      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
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
        /*for (i = 0; i < Ind; i += 3)
          {
            VEC
              p0 = V[Ind[i]].P,
              p1 = V[Ind[i + 1]].P,
              p2 = V[Ind[i + 2]].P,
              N = VecNormalize(VecCrossVec(VecSubVec(p1, p0), VecSubVec(p2, p0)));

            V[Ind[i]].N = VecAddVec(V[Ind[i]].N, N); /* VecAddVecEq(&V[Ind[i]].N, N); */
            /*V[Ind[i + 1]].N = VecAddVec(V[Ind[i + 1]].N, N);
            V[Ind[i + 2]].N = VecAddVec(V[Ind[i + 2]].N, N);
          }                                                */
    }
  }
  fclose(F);
  NM6_RndPrimCreate(Pr, V, nv, Ind, nind);
  return TRUE;
} /* End of 'NM6_RndPrimLoad' function */

/* END OF 'rndprim.c' FILE */