#include <stdio.h>
#include <string.h>
#include "rnd.h"

BOOL NS6_RndPrimCreate( NS6_PRIM *Pr, INT NoofV, INT NoofI)
{
  INT size;

  memset(Pr, 0, sizeof(NS6_PRIM));
  size = sizeof(NS6_VERTEX) * NoofV + sizeof(INT) * NoofI;

  if ((Pr->V = malloc(size)) == NULL)
    return FALSE;
  Pr->I = (INT *)(Pr->V + NoofV);
  Pr->NumOfI = NoofI;
  Pr->NumOfV = NoofV;
  Pr->Trans = MatrIdentity();
  memset(Pr->V, 0, size);

  return TRUE;
}

VOID NS6_RndPrimFree( NS6_PRIM *Pr)
{
  if (Pr->V != NULL)
    free(Pr->V);
  memset(Pr, 0, sizeof(NS6_PRIM));
}

VOID NS6_RndPrimDraw( NS6_PRIM *Pr, MATR World )
{
  INT i;
  MATR wvp = MatrMulMatr(MatrMulMatr(Pr->Trans, World), NS6_RndMatrVP);
  POINT *pnts;

  if ((pnts = malloc(sizeof(POINT) * Pr->NumOfV)) == NULL)
    return;

  for (i = 0; i < Pr->NumOfV; i++)
  {
    VEC p = VecMulMatr(Pr->V[i].P, wvp);

    pnts[i].x = (INT)((p.X + 1) * NS6_RndFrameW / 2);
    pnts[i].y = (INT)((-p.Y + 1) * NS6_RndFrameH / 2);
  }

  for (i = 0; i < Pr->NumOfI; i += 3)
  {
    MoveToEx(NS6_hRndDCFrame, pnts[Pr->I[i]].x, pnts[Pr->I[i]].y, NULL);
    LineTo(NS6_hRndDCFrame, pnts[Pr->I[i + 1]].x, pnts[Pr->I[i + 1]].y);
    LineTo(NS6_hRndDCFrame, pnts[Pr->I[i + 2]].x, pnts[Pr->I[i + 2]].y);
    LineTo(NS6_hRndDCFrame, pnts[Pr->I[i]].x, pnts[Pr->I[i]].y);
  }
  free(pnts);
}

VOID NS6_RndPrimCreateSphere( NS6_PRIM *Pr, VEC C, DBL R, INT SplitW, INT SplitH )
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

VOID NS6_RndPrimCreatePlane( NS6_PRIM *Pr, VEC P, VEC W, VEC L, INT SplitW, INT SplitH )
{
  INT i, j;

  for (i = 0; i < SplitH; i++)
    for (j = 0; j < SplitW; j++)
      Pr->V[i * SplitW + j].P = VecAddVec(VecAddVec(VecMulNum(W, j / (SplitW - 1.0)), VecMulNum(L, i / (SplitH - 1.0))), P);
}

/* Load primitive from '*.OBJ' file function.
 * ARGUMENTS:
 *   - pointer to primitive to load:
 *       NS6_4PRIM *Pr;
 *   - '*.OBJ' file name:
 *       CHAR *FileName;
 * RETURNS:
 *   (BOOL) TRUE if success, FALSE otherwise.
 */
BOOL NS6_RndPrimLoad( NS6_PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  INT i, nv = 0, nind = 0;
  static CHAR Buf[1000];

  memset(Pr, 0, sizeof(NS6_PRIM));
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

