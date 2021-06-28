/* FILE NAME: T0PERM.c
 * PROGRAMMER: NM6
 * DATE: 12.06.2021
 * PURPOSE: WinAPI application sample.
 * t04perm.c
 */
#include <stdio.h>
#include <windows.h>

#define MAX 3
INT P[MAX];
BOOL IsParity;

VOID Store ( VOID )
{
  FILE *F;
  INT i;

  F = fopen("PERM.TXT", "a");
  if (F == NULL)
    return;
  for (i = 0; i < MAX - 1; i++)
    fprintf(F, "%d", P[i]);
  fprintf(F, "%d - perity : %s\n", P[i], IsParity ? "even" : "odd");
  fclose(F);
}

VOID Swap( INT *A, INT *B )
{

  INT tmp = *A;

  *A = *B;
  *B = tmp;
  IsParity = !IsParity;
}

VOID Go( INT Pos )
{
  INT i, o;
  BOOL s = FALSE;

  if (Pos == MAX)
    Store();
  else
  {
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
  }
}
 
VOID Fill( VOID )
{
  INT i;

  for (i = 0; i <= MAX; i++)
    P[i] = i + 1;
}

VOID main( VOID )
{
  Fill();
  Go(0);
  printf("COMPLETE");
}
