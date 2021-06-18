/* FILE NAME: mth.h
 * PROGRAMMER: VG4
 * DATE: 15.06.2021
 * PURPOSE: 3D math implementation module.
 */

#ifndef __mth_h_
#define __mth_h_

#include <math.h>
#include <windows.h>

/* Pi math constant */
#define PI 3.14159265358979323846

/* Degrees to radians conversion */
#define D2R(A) ((A) * (PI / 180.0))
#define Degree2Radian(a) D2R(a)
#define UnitMatrix \
{                  \
  {                \
    {1, 0, 0, 0},  \
    {0, 1, 0, 0},  \
    {0, 0, 1, 0},  \
    {0, 0, 0, 1}   \
  }                \
}

/* Base float point types */
typedef double DBL;
typedef double FLT;

/* Space vector/point representation type */
typedef struct tagVEC
{
  DBL X, Y, Z; /* Vector coordinates */
} VEC;

typedef struct tagMATR
{
  DBL A[4][4];
} MATR;
/* Add two vectors function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC V1, V2;
 * RETURNS:
 *   (VEC) result vector.
 */
/*__inline VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
} /* End of 'VecAddVec' function */

/* END OF 'mth.h' FILE */
/* FILENAME: T08MTH.c
 * PROGRAMMER: NS6
 * DATE: 15.06.2021
 * PURPOSE: Math.
 */

  //VEC t = VecSet(0, 1, 0);
  //MATR m = MatrRotate(VecSet(1, 1, 1), 90);

__inline VEC VecSet( DBL X, DBL Y, DBL Z )
{
  VEC v = {X, Y, Z};

  return v;
} /* End of 'VecSet' function */

__inline VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
}


__inline VEC VecSubVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X - V2.X, V1.Y - V2.Y, V1.Z - V2.Z);
}

__inline VEC VecMulNum( VEC V1, DBL N )
{
  return VecSet(V1.X * N, V1.Y * N, V1.Z * N);
}

__inline VEC VecDivNum( VEC V1, DBL N )
{
  return VecSet(V1.X / N, V1.Y / N, V1.Z / N);
}

__inline VEC VecNeg( VEC V )
{
  return VecSet(-V.X, -V.Y, -V.Z);
}

__inline DBL VecDotVec( VEC V1, VEC V2 )
{
  return (V1.X * V2.X + V1.Y * V2.Y + V1.Y * V2.Y) / (sqrt(V1.X * V1.X + V1.Y * V1.Y + V1.Z * V1.Z) + sqrt(V2.X * V2.X + V2.Y * V2.Y + V2.Z * V2.Z)); 
}

__inline VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(V1.Y * V2.Z - V1.Z * V2.Y, V1.Z * V2.X - V1.X * V2.Z, V1.X * V2.Y - V1.Y * V2.X);
}

__inline DBL VecLen2( VEC V )
{
  return V.X * V.X + V.Y * V.Y + V.Z * V.Z;
}

__inline DBL VecLen( VEC V )
{
  return sqrt(V.X * V.X + V.Y * V.Y + V.Z * V.Z);
}

__inline VEC VecNormalize( VEC V )
{
  DBL len = VecDotVec(V, V);

  if (len == 1 || len == 0)
    return V;
  return VecDivNum(V, sqrt(len));
}

__inline VEC PointTransform( VEC V, MATR M )  /*V * M4x3*/
{
  return VecSet(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0],
                V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1],
                V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]);
}
__inline VEC VectorTransform( VEC V, MATR M ) /*V * M3x3*/
{
  return VecSet(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0],
                V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1],
                V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]);
}
__inline VEC VecMulMatr( VEC V, MATR M )     /*  V * M4x4 / w */
{
  DBL w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];

  return VecSet((V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
                (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
                (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
}

/* realisation */
__inline MATR MatrIdentity( VOID )
{
  MATR I = UnitMatrix;
  return I;
}

__inline MATR MatrTranslate( VEC T )
{
  MATR m =
  {
    {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {T.X, T.Y, T.Z, 1}
    }
  };

  return m;
}

__inline MATR MatrScale( VEC S )
{
  MATR m =
  {
    {
      {S.X, 0, 0, 0},
      {0, S.Y, 0, 0},
      {0, 0, S.Z, 0},
      {0, 0, 0, 1}
    }
  };

  return m;
}

__inline MATR MatrRotateX( DBL AngleInDegree )
{
  DBL a = D2R(AngleInDegree), s = sin(a), c = cos(a);
  MATR m = 
  {
    {
      {1, 0, 0, 0},
      {0, c, s, 0},
      {0, -s, c, 0},
      {0, 0, 0, 1}
    }
  };

  return m;
}

__inline MATR MatrRotateY( DBL AngleInDegree )
{
  DBL a = D2R(AngleInDegree), s = sin(a), c = cos(a);
  MATR m = 
  {
    {
      {c, 0, -s, 0},
      {0, 1, 0, 0},
      {s, 0, c, 0},
      {0, 0, 0, 1}
    }
  };

  return m;
}
__inline MATR MatrRotateZ( DBL AngleInDegree )
{
  DBL a = D2R(AngleInDegree), s = sin(a), c = cos(a);
  MATR m = 
  {
    {
      {c, s, 0, 0},
      {-s, c, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };

  return m;
}

__inline MATR MatrRotate( DBL AngleInDegree, VEC V )
{
  DBL a = D2R(AngleInDegree), s = sin(a), c = cos(a);
  VEC A = VecNormalize(V);
  
  MATR m =
  {
    {
      {c + A.X * A.X * (1 - c), A.X * A.Y * (1 - c) + A.Z * s, A.X * A.Z * (1 - c) - A.Y * s,0},
      {A.X * A.Y * (1 - c) - A.Z * s, c + A.Y * A.Y * (1 - c), A.Y * A.Z * (1 - c) + A.X * c, 0},
      {A.X * A.Z * (1 - c) + A.Y * s, A.Z * A.Y * (1 - c) + A.X * s, c + A.Z * A.Z * (1 - c), 0},
      {0, 0, 0, 1}
    }  
  };

  return m;
}

__inline MATR MatrMulMatr( MATR M1, MATR M2 ) 
{
  MATR r = {{{0}}};
  INT i, j, k;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (r.A[i][j] = 0, k = 0; k < 4; k++)
        r.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return r;
}

__inline MATR MatrTranspose( MATR M )
{
  DBL t; 
  INT i, j;

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
    {
      t = M.A[i][j];
      M.A[i][j] = M.A[j][i];
      M.A[j][i] = t;
    }
    return M;
}
__inline DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                            DBL A21, DBL A22, DBL A23,
                            DBL A31, DBL A32, DBL A33 )
{ 
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
         A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
}

__inline DBL MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3], M.A[2][1], M.A[2][2], M.A[2][3], M.A[3][1], M.A[3][2], M.A[3][3]) -
    M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3], M.A[2][0], M.A[2][2], M.A[2][3], M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3], M.A[2][0], M.A[2][1], M.A[2][3], M.A[3][0], M.A[3][1], M.A[3][3]) -
    M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2], M.A[2][0], M.A[2][1], M.A[2][2], M.A[3][0], M.A[3][1], M.A[3][2]) +
    M.A[1][0] * MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3], M.A[2][1], M.A[2][2], M.A[2][3], M.A[3][1], M.A[3][2], M.A[3][3]) -
    M.A[1][1] * MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3], M.A[2][0], M.A[2][2], M.A[2][3], M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[1][2] * MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3], M.A[2][0], M.A[2][1], M.A[2][3], M.A[3][0], M.A[3][1], M.A[3][3]) -
    M.A[1][3] * MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2], M.A[2][0], M.A[2][1], M.A[2][2], M.A[3][0], M.A[3][1], M.A[3][2]) +
    M.A[2][0] * MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3], M.A[1][1], M.A[1][2], M.A[1][3], M.A[3][1], M.A[3][2], M.A[3][3]) -
    M.A[2][1] * MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3], M.A[1][0], M.A[1][2], M.A[1][3], M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[2][2] * MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3], M.A[1][0], M.A[1][1], M.A[1][3], M.A[3][0], M.A[3][1], M.A[3][3]) -
    M.A[2][3] * MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2], M.A[1][0], M.A[1][1], M.A[1][2], M.A[3][0], M.A[3][1], M.A[3][2]) +
    M.A[3][0] * MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3], M.A[1][1], M.A[1][2], M.A[1][3], M.A[2][1], M.A[2][2], M.A[2][3]) -
    M.A[3][1] * MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3], M.A[1][0], M.A[1][2], M.A[1][3], M.A[2][0], M.A[2][2], M.A[2][3]) +
    M.A[3][2] * MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3], M.A[1][0], M.A[1][1], M.A[1][3], M.A[3][0], M.A[2][1], M.A[2][3]) -
    M.A[3][3] * MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2], M.A[1][0], M.A[1][1], M.A[1][2], M.A[2][0], M.A[2][1], M.A[2][2]);
}

__inline MATR MatrInverse( MATR M )
{
  MATR r;
  DBL det = MatrDeterm(M);
  INT s, i, j, P[][3] = {{1, 2, 3}, {0, 2, 3}, {0, 1, 3}, {0, 1, 2}};

  if (det == 0)
    return MatrIdentity();

  for (s = 1, i = 0; i < 4; i++)
    for (j = 0; j < 4; j++, s = -s)
      r.A[j][i] = s * MatrDeterm3x3(M.A[P[i][0]][P[j][0]], M.A[P[i][0]][P[j][1]], M.A[P[i][0]][P[j][2]],
                                    M.A[P[i][1]][P[j][0]], M.A[P[i][1]][P[j][1]], M.A[P[i][1]][P[j][2]],
                                    M.A[P[i][2]][P[j][0]], M.A[P[i][2]][P[j][1]], M.A[P[i][2]][P[j][2]]) / det;
  return r;
}

__inline MATR MatrView( VEC Loc, VEC At, VEC Up1 )
{
  VEC
	Dir = VecNormalize(VecSubVec(At, Loc)),
	Right = VecNormalize(VecCrossVec(Dir, Up1)),
	Up = VecNormalize(VecCrossVec(Right, Dir));
  MATR m =
  {
	{
		{Right.X, Up.X, -Dir.X, 0},
		{Right.Y, Up.Y, -Dir.Y, 0},
		{Right.Z, Up.Z, -Dir.Z, 0},
		{-Loc.X * Right.X, -Loc.Y * Up.Y, -Loc.Z * Dir.Z, 1}
	}
  };
	return m;
}

/* �������� (�������) */
/*void main( void )
{
  MATR m =
  {
    {1, 1, 1, 0},
    {1, 1, 2, 0},
    {1, 2, 1, 1},
    {2, 1, 1, 1}
  }, m1, m2;

  m1 = MatrInverse(m);
  m2 = MatrMulMatr(m, m1);
  m2 = MatrMulMatr(m1, m);

  m = MatrRotate(90, VecSet(0, 1, 0));
  m1 = MatrInverse(m);
  . . .
}*/

#endif /* __mth_h_ */

/* END OF 'mth.h' FILE */
