 /* FILE NAME: T04PERM.c
 * PROGRAMMER: NM6
 * DATE: 12.06.2021
 * PURPOSE: WinAPI application sample.
 * t04perm.c
 */
#include <stdio.h>
#include <windows.h>

typedef DOUBLE DBL;

#define MAX 3
INT N;
DBL A[MAX][MAX];

INT P[MAX];
BOOL IsParity;

VOID Swap( INT *A, INT *B )
{

  INT tmp = *A;

  *A = *B;
  *B = tmp;
  IsParity = !IsParity;
}

BOOL LoadMatrix( CHAR *FileName )
{
  FILE *F;
  INT N = 0, i, j;
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;
  fscanf(F, "%d", &N);
  if (N > MAX)
    N = MAX;
  else
    if (N < 0)
      N = 0;
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &A[i][j]);
  fclose(F);
  return TRUE;
}

VOID Go( INT Pos )
{
  INT i, o, prod = 1, j;
  static INT Det = 0;
  BOOL s = FALSE;
  for (i = 0; i < MAX; i++)
    prod *= (int)A[i][P[i]];
  s = IsParity;
  Go(Pos + 1);

  for(i = Pos + 1; i <= MAX; i++)
  {
    Swap(&P[Pos], &P[i]);
    Go(Pos + 1);
  }
  o = P[Pos];
  for(i = Pos + 1; i <= MAX; i++)
  {
    P[i - 1] = P[i];
    P[MAX - 1] = o;
    IsParity = s;
  }

  for (i = 0; i < MAX; i++)
      for (j = 0; j < MAX; j++)
      { 
          Det += (int)A[i][j];
      }
}

VOID main( VOID )
{
  Go();
  printf("COMPLETE");
}

/*BOOL LoadMatrix( CHAR *FileName )
{
  FILE *F;

  N = 0;
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;
  fscanf(F, "%d", &N);
  if (N > MAX)
    N = MAX;
  else
    if (N < 0)
      N = 0;
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &A[i][j]);
  fclose(F);
  return TRUE;
}*/
