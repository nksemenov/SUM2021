/* FILENAME: T04PERM.c
 * PROGRAMMER: NS6
 * DATE: 12.06.2021
 * PURPOSE: WinAPI Clock drawing application sample.
 */

#include <stdio.h>
#include <windows.h>

#define MAX 3


static INT P[MAX];
static BOOL IsParity = TRUE;


VOID Swap( INT *a, INT *b )
{
  INT t = *a;

  *a = *b;
  *b = t;
}

VOID Store( VOID )
{
  FILE *F;
  INT i;

  F = fopen("PERM.TXT", "a");
  if (F == NULL)
    return;
  for (i = 0; i < MAX; i++)
    fprintf(F, "%d%s", P[i], i < MAX - 1 ? " " : "");
  fprintf(F, " - parity: %s\n", IsParity ? "even" : "odd");

  fclose(F);
}


VOID Go( INT Pos )
{
  INT i;

  if (Pos == MAX)
    Store();
  else
    for (i = Pos; i < MAX; i++)
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

VOID main( VOID )
{
  INT i;

  for (i = 0; i < MAX; i++)
    P[i] = i + 1;
  Go(0);
}