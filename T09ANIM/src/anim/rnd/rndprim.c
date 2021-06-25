#include <stdio.h>
#include <string.h>
#include "rnd.h"

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
  MATR wvp = MatrMulMatr3(Pr->Trans, World, NS6_RndMatrVP);

  INT ProgId = NS6_RndShaders[0].ProgId, loc;

  glUseProgram(ProgId);

  if ((loc = glGetUniformLocation(ProgId, "MatrWVP")) != -1)
    glUniformMatrix4fv(loc, 1, FALSE, wvp.A[0]);
  /*if ((loc = glGetUniformLocation(ProgId, "Time")) != -1)
    glUniform1f(loc, ns6ANIM.Time); */

  /* Send matrix to OpenGL /v.1.0 */
  glLoadMatrixf(wvp.A[0]);

  glBindVertexArray(Pr->VA);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
  glDrawElements(GL_TRIANGLES, Pr->NumOfElements, GL_UNSIGNED_INT, NULL);
  glBindVertexArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  glUseProgram(0);
}/* End of 'NS6_PrimDraw' function */
/*
VOID NS6_RndPrimCreateSphere( ns6PRIM *Pr, VEC C, DBL R, INT SplitW, INT SplitH )
{
  INT i, j, k; 
  DOUBLE phi, theta, pi = 3.14159265558979323846;
  
  NS6_RndPrimCreate(Pr, SplitW * SplitH, (SplitW - 1) * (SplitH - 1) * 6);
  for (i = 0, theta = 0; i < SplitH; i++, theta += 2 * pi / (SplitH - 1))
    for (j = 0, phi = 0; j < SplitW; j++, phi += 2 * pi / (SplitW - 1))
      Pr->V[i * SplitW + j].P = VecSet(C.X + (R +  (R / 2) * cos(theta)) * cos(phi), C.Y + (R + (R / 2) * cos(theta)) * sin(phi), C.Z + (R / 2) *sin(theta));
  
  for (k = 0, i = 0; i < SplitH - 1; i++)
    for (j = 0; j < SplitW - 1; j++)
    {
      Pr->I[k++] = i * SplitW + j;
      Pr->I[k++] = (i + 1) * SplitW + j;
      Pr->I[k++] = i * SplitW + j + 1;

      Pr->I[k++] = (i + 1) * SplitW + j;
      Pr->I[k++] = i * SplitW + j + 1;
      Pr->I[k++] = (i + 1) * SplitW + j + 1;
    }
}
*/
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
  INT i, nv = 0, nind = 0;
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

  if (!NS6_RndPrimCreate(Pr, nv, nind))
  {
    fclose(F);
    return FALSE;
  }

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
      Pr->V[nv++].P = VecSet(x, y, z);
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
            Pr->I[nind++] = n0;
            Pr->I[nind++] = n1;
            Pr->I[nind++] = nc;
            n1 = nc;
          }
          n++;
        }
    }
  }

  fclose(F);
  return TRUE;
} /* End of 'NS6_RndPrimLoad' function */