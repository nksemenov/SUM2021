#include <stdio.h>
#include "MTH.H"

void main ( void )
{
  DBL i;
  VEC v1, v2, v3, v4, v5;
  
  v1 = VecSet(1, 1, 1);
  v2 = VecSet(2, 2, 2);
  v3 = VecAddVec(v1, v2);
  v4 = VecMulNum(v1, 5);
  v5 = VecDivNum(v1, 3);
}