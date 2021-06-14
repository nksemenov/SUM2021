 /* FILENAME: T06DETF.c
 * PROGRAMMER: NS6
 * DATE: 14.06.2021
 * PURPOSE: Matrix.
 */

#include <stdio.h>
#include <math.h>
#include <windows.h>

#define MAX 10

VOID Swap( DOUBLE*a, DOUBLE *b )
{
  DOUBLE t = *a;

  *a = *b;
  *b = t;
}

VOID main( VOID )
{
  INT i, j, max_row, max_col, x, y, N;
  DOUBLE det = 1, coef;
  FILE *F;
  DOUBLE A[MAX][MAX];

  N = 0;  
 
  F = fopen("matrix.txt", "r");
  fscanf(F, "%d", &N);
  if (N < 0)
    N = 0;
  else
    if (N > MAX)
      N = MAX;
  
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      fscanf(F, "%lf", &A[i][j]);
  fclose(F);
 

  for (i = 0; i < N; i++)
  
  {
    max_row = max_col = i;
    for (y = i; y < N; y++)
      for (x = i; x < N; x++)
        if (fabs(A[y][x]) > fabs(A[max_row][max_col]))
          max_row = y, max_col = x;
    if (A[max_row][max_col] == 0)
    {
      det = 0;
      printf("%d", det);
      break;
    } 

   if (max_row != i)
   {
     for (x = i; x < N; x++)
       Swap(&A[max_row][x], &A[i][x]);
     det = -det;
   }
   
   if (max_col != i)
   {
     for (x = 1; x < N; x++)
       Swap(&A[x][max_col], &A[x][i]);
     det = -det;
   }
   
   for (x = i + 1; x < N; x++)
   {
     coef = A[x][i]/A[i][i];
     A[x][i] = 0;
     for (y = 1; y < N; y++)
       A[x][y] -= A[i][y] * coef;
   }
   det *= A[i][i];           
  }

  
  if (det != 0)
    printf("%d\n", det);
  getchar();
}