 /* FILENAME: T05DET.c
 * PROGRAMMER: NS6
 * DATE: 12.06.2021
 * PURPOSE: WinAPI Clock drawing application sample.
 */

#include <stdio.h>
#include <windows.h>  

#define MAX 10

typedef DOUBLE DBL;

static DBL A[MAX][MAX];
static INT n, det, prod = 1;    

static INT P[MAX];
static BOOL IsParity; 

VOID Swap( INT *a, INT *b )
{
  INT t = *a;

  *a = *b;
  *b = t;
}   

VOID f( Void )
{
  INT i;
  for (i = 0; i < n; i++)
    prod *= A[i][P[i]];
  det += (IsParity * 2 - 1) * prod;
}

VOID Go( INT Pos )
{
  INT i;

  if (Pos == n)
    f();
  else
    for (i = Pos; i < n; i++)
    {
      Swap(&P[Pos], &P[i]);
      if (Pos != i)
        IsParity = !IsParity;
      Go(Pos + 1);
      Swap(&P[Pos], &P[i]);
      if (Pos != i)
        IsParity = !IsParity;
    }
}

BOOL LoadMatrix( CHAR *FileName )
{
  FILE *F;
  INT i, j;

  n = 0;
  if ((F = fopen(FileName, "r")) == NULL)
    return FALSE;  
 
  fscanf(F, "%d", &n);
  if (n < 0)
    n = 0;
  else
    if (n > n)
      n = n;
  
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      fscanf(F, "%lf", &A[i][j]);
  fclose(F);
  
  return TRUE;
}

VOID main( VOID )
{  
  if (LoadMatrix("matrix.txt") == FALSE)
    return;
  Go(0);

  printf("%d", det);
} 