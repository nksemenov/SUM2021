/* FILE NAME: rndprim.c
* PROGRAMMER: nm6
* DATE: 18.06.2021
* PURPOSE: 3D animation primitive handle module.
*/

#include <stdio.h>
#include "rnd.h"

/* NM6_RndPrimCreate */
BOOL NM6_RndPrimCreate( nm6PRIM *Pr, INT NoofV, INT NoofI )
{
  INT size;
  memset(Pr, 0, sizeof(nm6PRIM));
  size = sizeof(nm6VERTEX) * NoofV + sizeof(INT) * NoofI;

  if ((Pr->V = malloc(size)) == NULL)
    return FALSE;
  Pr->I = (INT *)(Pr->V + NoofV);
  Pr->NumOfV = NoofV;
  Pr->NumOfI = NoofI;
  Pr->Trans = MatrIdentity();
  memset(Pr->V, 0, size);

  return TRUE;
}/* End of 'NM6_RndPrimCreate' function */

/* NM6_RndPrimFree */
VOID NM6_RndPrimFree( nm6PRIM *Pr )
{
  if (Pr->V != NULL)
    free(Pr->V);
  memset(Pr, 0, sizeof(nm6PRIM));
}/* NM6_PrimFree */

/* NM6_PrimDraw */
VOID NM6_RndPrimDraw( nm6PRIM *Pr, MATR World )
{
  MATR wvp = MatrMulMatr3(Pr->Trans, World, NM6_RndMatrVP);
  POINT *pnts;
  INT i;

  if ((pnts = malloc(sizeof(POINT) * Pr->NumOfV)) == NULL)
    return;

  /* build projection */
  for (i = 0; i < Pr->NumOfV; i++)
  {
    VEC p = VecMulMatr(Pr->V[i].P, wvp);

    pnts[i].x = (INT)((p.X + 1) * NM6_RndFrameW / 2);
    pnts[i].y = (INT)((-p.Y + 1) * NM6_RndFrameH / 2);
  }

  /* draw triangles */
  for (i = 0; i < Pr->NumOfI; i += 3)
  {
    MoveToEx(NM6_hRndDCFrame, pnts[Pr->I[i]].x, pnts[Pr->I[i]].y, NULL);
    LineTo(NM6_hRndDCFrame, pnts[Pr->I[i + 1]].x, pnts[Pr->I[i + 1]].y);
    LineTo(NM6_hRndDCFrame, pnts[Pr->I[i + 2]].x, pnts[Pr->I[i + 2]].y);
    LineTo(NM6_hRndDCFrame, pnts[Pr->I[i]].x, pnts[Pr->I[i]].y);
  }
  free(pnts);
}/* End of 'NM6_PrimDraw' function */

/* NM6_RndPrimLoad */
BOOL NM6_RndPrimLoad( nm6PRIM *Pr, CHAR *FileName )
{
  FILE *F;
  INT i, nv = 0, nind = 0;
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

  if (!NM6_RndPrimCreate(Pr, nv, nind))
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
    if (Buf[i - 1] == ' ' && Buf[i] != ' ')
    {
      sscanf(Buf + i, "%i", &nc);
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
} /* End of 'NM6_RndPrimLoad' function */

/* END OF 'rndprim.c' FILE */