/* FILE NAME: T0PERM.c
 * PROGRAMMER: NM6
 * DATE: 14.06.2021
 * PURPOSE: WinAPI application sample.
 * t06detg.c
 */

#include <windows.h>
#include <stdio.h>
#include <math.h>

#define MAX 6

DOUBLE A[MAX][MAX];
INT N;

BOOL LoadMatrix( CHAR *FileName )
{
  FILE *F;
  INT i, j;

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
}/* End of 'LoadMatrix' function */

/* swap */
VOID Swap( DOUBLE *A, DOUBLE *B )
{
  DOUBLE tmp = *A;
  *A = *B;
  *B = tmp;
}/* End of 'Swap' function */

/* go */
VOID Detg( VOID )
{
  DOUBLE kf = 1, det = 1;
  INT k, j, max_row, max_col, i, x, y;

  for (i = 0; i < N; i++)
  {
    max_row = max_col = i;
    for (y = i; y < N; y++)
      for (x = i; x < N; x++)
        if (fabs(A[y][x]) > fabs(A[max_row][max_col]))
          max_col = x, max_row = y;
        if (A[max_row][max_col] == 0)
  {
  Det = 0;
  break;
  }
  if (max_row != i)
  {
    for (x = i; x < N; x++)
    Swap(&A[i][x], &A[max_row][x]);
    Det = -Det;
  }
  if (max_col != i)
  {
    for (y = 0; y < N; y++)
    Swap(&A[y][i], &A[y][max_col]);
    Det = -Det;
  }
  for (j = i + 1; j < N; j++)
  {
    kf = A[j][i] / A[i][i];
    A[j][i] = 0;
    for (k = i + 1; k < N; k++)
    A[j][k] -= A[i][k] * kf;
  }
  Det *= A[i][i];
  }
}/* End of 'Go' function */

/* main */
VOID main( VOID )
{
  LoadMatrix("r.txt");
  Detg();
  printf("%f\n", Det);
}/* End of 'main' dunction */

