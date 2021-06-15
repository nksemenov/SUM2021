/* FILENAME: T08MTH.c
 * PROGRAMMER: NS6
 * DATE: 15.06.2021
 * PURPOSE: Math.
 */

/*Realisation*/
VEC VecSet( DBL X, DBL Y, DBL Z )
{
  VEC v;

  v.X = X;
  v.Y = Y;
  v.Z = Z;
  return v;
} /* End of 'VecSet' function */

  VEC t = VecSet(0, 1, 0);
  MATR m = MatrRotate(VecSet(1, 1, 1), 90);

VEC VecSet( DBL X, DBL Y, DBL Z )
{
  VEC v = {X, Y, Z};

  return v;
} /* End of 'VecSet' function */

VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
}


VEC VecSubVec( VEC V1, VEC V2 );
{
  return VecSet(V1.X - V2.X, V1.Y - V2.Y, V1.Z - V2.Z);
}

VEC VecMulNum( VEC V1, DBL N );   V1 * N
{
  return VecSet(V1.X * N, V1.Y * N, V1.Z * N);
}

VEC VecDivNum( VEC V1, DBL N );
{
  return VecSet(V1.X / N, V1.Y / N, V1.Z / N);
}

VEC VecNeg( VEC V )
{
  return VecSet(-V.X, -V.Y, -V.Z);
}

/* V1 * V2 * cos */
DBL VecDotVec( VEC V1, VEC V2 )
{
  return VecSet((V1.X * V2.X + V1.Y * V2.Y + V1.Y * V2.Y) / (sqrt(V1.X * V1.X + V1.Y * V1.Y + V1.Z * V1.Z) + sqrt(V2.X * V2.X + V2.Y * V2.Y + V2.Z * V2.Z)); 
}

VEC VecCrossVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X * V2.X, V1.Y * V2.Y, V1.Z * V2.Z);
}

DBL VecLen2( VEC V )
{
  return VecSet(V1.X * V1.X, V1.Y * V1.Y, V1.Z * V1.Z);
}

DBL VecLen( VEC V )
{
  DBL len = VecDotVec(V, V);

  if (len == 1 || len == 0)
    return len;
  return sqrt(len);
}

VEC VecNormalize( VEC V )
{
  DBL len = VecDotVec(V, V);

  if (len == 1 || len == 0)
    return V;
  return VecDivNum(V, sqrt(len));
}

VEC PointTransform( VEC V, MATR M );   /*V * M4x3*/
VEC VectorTransform( VEC V, MATR M );  /*V * M3x3*/
VEC VecMulMatr( VEC V, MATR M );       V * M4x4 / w
  w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];


–еализаци€ матриц:

/* единична€ матрица */
static MATR UnitMatrix =
{
  . . .
};
MATR MatrIdentity( VOID )
{
  return UnitMatrix;
}

#define UnitMatrix \
{                  \
  {                \
    {1, 0, 0, 0},  \
    {0, 1, 0, 0},  \
    {0, 0, 1, 0},  \
    {0, 0, 0, 1}   \
  }                \
}


MATR MatrIdentity( VOID )
{
  MATR I = UnitMatrix;
  return I;
}

MATR MatrTranslate( VEC T )
{
  MATR m = UnitMatrix;

  m.A[3][0] = T.X;
  m.A[3][1] = T.Y;
  m.A[3][2] = T.Z;
  return m;
}

MATR MatrTranslate( VEC T )
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

MATR MatrScale( VEC S );
MATR MatrRotateX( DBL AngleInDegree );
MATR MatrRotateY( DBL AngleInDegree );
MATR MatrRotateZ( DBL AngleInDegree );
MATR MatrRotate( DBL AngleInDegree, VEC V )
{
  DBL a = D2R(AngleInDegree), s = sin(a), c = cos(a);
  VEC A = VecNormalize(V);
  MATR m =
  {
    {
      {c + A.X * A.X * (1 Ц c), A.X * A.Y * (1 Ц c) + A.Z * s, A.X * A.Z * (1 Ц c) Ц A.Y * s,0},
      {... , ... , ... , 0},
      {... , ... , ... , 0},
      {0, 0, 0, 1}
    }  
  };

  return m;
}

MATR MatrMulMatr( MATR M1, MATR M2 ) 
{
  MATR r = {{{0}}};

  for (i = 0; i < 4; i++)
    for (j = 0; j < 4; j++)
      for (r.A[i][j] = 0 (???), k = 0; k < 4; k++)
        r.A[i][j] += M1.A[i][k] * M2.A[k][j];
  return r;
}

MATR MatrTranspose( MATR M );    MT

DBL MatrDeterm3x3( DBL A11, DBL A12, DBL A13,
                   DBL A21, DBL A22, DBL A23,
                   DBL A31, DBL A32, DBL A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
         A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
}

DBL MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) -
    M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                              M.A[2][0], M.A[2][2], M.A[2][3],
                              M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) -
    M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                              M.A[2][0], M.A[2][1], M.A[2][2],
                              M.A[3][0], M.A[3][1], M.A[3][2]) +
    M.A[1][0] * MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) -
    M.A[1][1] * MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                              M.A[2][0], M.A[2][2], M.A[2][3],
                              M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[1][2] * MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) -
    M.A[1][3] * MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                              M.A[2][0], M.A[2][1], M.A[2][2],
                              M.A[3][0], M.A[3][1], M.A[3][2]) +
}


MATR MatrInverse( MATR M )
{
  DBL det = MatrDeterm(M);
  MATR r;

  if (det == 0)
    return MatrIdentity();

  /* строим присоединенную матрицу */ /* build adjoint matrix */
  r.M[0][0] =
    +MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                   M.A[2][1], M.A[2][2], M.A[2][3],
                   M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.M[1][0] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[2][0], M.A[2][2], M.A[2][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.M[2][0] =
    +MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                   M.A[2][0], M.A[2][1], M.A[2][3],
                   M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.M[3][0] =
    -MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[2][0], M.A[2][1], M.A[2][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.M[0][1] =
    -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                   M.A[2][1], M.A[2][2], M.A[2][3],
                   M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.M[1][1] =
    +MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                   M.A[2][0], M.A[2][2], M.A[2][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.M[2][1] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                   M.A[2][0], M.A[2][1], M.A[2][3],
                   M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.M[3][1] =
    +MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                   M.A[2][0], M.A[2][1], M.A[2][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]) / det;


  r.M[0][2] =
    +MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                   M.A[1][1], M.A[1][2], M.A[1][3],
                   M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.M[1][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                   M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.M[2][2] =
    +MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                   M.A[1][0], M.A[1][1], M.A[1][3],
                   M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.M[3][2] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                   M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[3][0], M.A[3][1], M.A[3][2]) / det;

  r.M[0][3] =
    -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                   M.A[1][1], M.A[1][2], M.A[1][3],
                   M.A[2][1], M.A[2][2], M.A[2][3]) / det;
  r.M[1][3] =
    +MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                   M.A[1][0], M.A[1][2], M.A[1][3],
                   M.A[2][0], M.A[2][2], M.A[2][3]) / det;
  r.M[2][3] =
    -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                   M.A[1][0], M.A[1][1], M.A[1][3],
                   M.A[2][0], M.A[2][1], M.A[2][3]) / det;
  r.M[3][3] =
    +MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                   M.A[1][0], M.A[1][1], M.A[1][2],
                   M.A[2][0], M.A[2][1], M.A[2][2]) / det;

  return r;
}

MATR MatrInverse( MATR M )
{
  MATR r;
  DBL det = MatrDeterm(M);
  INT s, i, j, P[][3] = {{1, 2, 3}, {0, 2, 3}, {0, 1, 3}, {0, 1, 2}};

  if (det == 0)
    return MatrIdentity();

  for (s = 1, i = 0; i < 4; i++)
    for (j = 0; j < 4; j++, s = -s)
      r.M[j][i] =
        s * MatrDeterm3x3(M.A[P[i][0]][P[j][0]], M.A[P[i][0]][P[j][1]], M.A[P[i][0]][P[j][2]],
                       M.A[P[i][1]][P[j][0]], M.A[P[i][1]][P[j][1]], M.A[P[i][1]][P[j][2]],
                          M.A[P[i][2]][P[j][0]], M.A[P[i][2]][P[j][1]], M.A[P[i][2]][P[j][2]]) / det;
  return r;
}


/* проверка (вариант) */
void main( void )
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
}

!!!
¬ Visual Studio существует расширение €зыка —и:
если перед функцией поставить служебное слово Ч __inline Ч функци€ становитс€ статической и разворачиваетс€ пр€мо в текст программы (как define), такие функции помещают в файлы заголовков

__inline void Swap1( double *a, double *b )
{
  double tmp = *a;

  *a = *b;
  *b = tmp;
}

тогда вызов
Swap1(&a, &b) превратитс€ в текст __t__ = a, a = b, b = __t__; где __t__ кака€-то переменна€

MTH.H

/* Add two vectors function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC V1, V2;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
} /* End of 'VecAddVec' function */

__inline MATR MatrSet( DBL A00, DBL A01, ... )
{
  MATR r =
  {
    {
      {A00, A01, ...}
    }
  };

  return r;
}

__inline MATR MatrScale( VEC S )
{
  return MatrSet(S.X, 0, 0, 0,
                 0, S.Y, 0, 0,
                 0, 0, S.Y, 0,
                 0, 0, 0, 1);
}


!!! ¬ќ ¬—≈ «ј√ќЋќ¬ќ„Ќџ≈ ‘ј…Ћџ ƒќЅј¬Ћя≈ћ »— Ћё„≈Ќ»≈ ѕќ¬“ќ–Ќќ√ќ ¬ Ћё„≈Ќ»я: (см. п.7 основного конспекта по —и)
MTH.H
/* FILE NAME: mth.h
 * PROGRAMMER: VG4
 * DATE: 15.06.2021
 * PURPOSE: 3D math implementation module.
 */

#ifndef __mth_h_
#define __mth_h_

#include <math.h>

/* Pi math constant */
#define PI 3.14159265358979323846

/* Degrees to radians conversion */
#define D2R(A) ((A) * (PI / 180.0))
#define Degree2Radian(a) D2R(a)

/* Base float point types */
typedef double DBL;
typedef double FLT;

/* Space vector/point representation type */
typedef struct tagVEC
{
  DBL X, Y, Z; /* Vector coordinates */
} VEC;

/* Add two vectors function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC V1, V2;
 * RETURNS:
 *   (VEC) result vector.
 */
__inline VEC VecAddVec( VEC V1, VEC V2 )
{
  return VecSet(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
} /* End of 'VecAddVec' function */

#endif /* __mth_h_ */

/* END OF 'mth.h' FILE */

пример в GLOBE

MATR m = MatrMulMatr(MatrRotateY(GLB_Time * 30), MatrRotate(sin(GLB_Time) * 8, VecSet(1, 1, 1)));

for (...
  for (...
  {
    VEC v = PointTransform(Geom[i][i], m);


    pnts[i][j].x = WinW / 2 + (INT)v.X;
    pnts[i][j].y = WinH / 2 - (INT)v.Y;
  }
