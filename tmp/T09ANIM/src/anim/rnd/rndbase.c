/* FILE NAME: rndbase.c
* PROGRAMMER: NM6
* DATE: 17.06.2021
* PURPOSE: 3D animation rendering function module.
*/

#include "rnd.h"
#include <wglew.h>
#include <gl/wglext.h>

#pragma comment(lib, "opengl32")

/* NM6_RndInit */
VOID NM6_RndInit( HWND hWnd )
{
  INT i, nums;
  PIXELFORMATDESCRIPTOR pfd = {0};
  HGLRC hRC;
  INT PixelAttribs[] =
  {
    WGL_DRAW_TO_WINDOW_ARB, TRUE,
    WGL_SUPPORT_OPENGL_ARB, TRUE,
    WGL_DOUBLE_BUFFER_ARB, TRUE,
    WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
    WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
    WGL_COLOR_BITS_ARB, 32,
    WGL_DEPTH_BITS_ARB, 32,
    0
  };
  INT ContextAttribs[] =
  {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
    WGL_CONTEXT_MINOR_VERSION_ARB, 6,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
                                  /* WGL_CONTEXT_CORE_PROFILE_BIT_ARB, */
    0
  };

  NM6_hRndWnd = hWnd;
  NM6_hRndDC = GetDC(hWnd);

  /* OpenGL init: pixel format setup */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(NM6_hRndDC, &pfd);
  DescribePixelFormat(NM6_hRndDC, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  SetPixelFormat(NM6_hRndDC, i, &pfd);

  /* OpenGL init: setup rendering context */
  NM6_hRndGLRC = wglCreateContext(NM6_hRndDC);
  wglMakeCurrent(NM6_hRndDC, NM6_hRndGLRC);

  /* Initializing GLEW library */
  if (glewInit() != GLEW_OK)
  {
    MessageBox(NM6_hRndWnd, "Error extensions initializing", "Error",
      MB_ICONERROR | MB_OK);
    exit(0);
  }

  if (!(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    MessageBox(NM6_hRndWnd, "Error: no shaders support", "Error", MB_ICONERROR | MB_OK);
    exit(0);
  }

  /* Enable a new OpenGL profile support */
  wglChoosePixelFormatARB(NM6_hRndDC, PixelAttribs, NULL, 1, &i, &nums);
  hRC = wglCreateContextAttribsARB(NM6_hRndDC, NULL, ContextAttribs);

  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(NM6_hRndGLRC);

  NM6_hRndGLRC = hRC;
  wglMakeCurrent(NM6_hRndDC, NM6_hRndGLRC);
  /* Set default OpenGL parameters */
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.08, 0.16, 0.08, 1);
  NM6_RndShadersInit();

  /* Render perametrs */
  NM6_RndProjSize = 0.1;
  NM6_RndProjDist = NM6_RndProjSize;
  NM6_RndProjFarClip = 300;
  NM6_RndFrameW = 50;
  NM6_RndFrameH = 50;
  NM6_RndCamSet(VecSet(0, 0, 30), VecSet(0, 0, 0), VecSet(0, 1, 0));
}/* End of 'NM6_RndInit' function */

/* NM6_RndClose */
VOID NM6_RndClose( VOID )
{
  NM6_RndShadersClose();
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(NM6_hRndGLRC);
  ReleaseDC(NM6_hRndWnd, NM6_hRndDC);
}/* End of 'NM6_RndClose' function */

/* NM6_RndCopyFrame */
VOID NM6_RndCopyFrame( VOID )
{
  SwapBuffers(NM6_hRndDC);
}/* End of 'NM6_RndCopyFrame' function */

/* NM6_RndProjSet */
VOID NM6_RndProjSet( VOID )
{
  FLT rx, ry;

  rx = ry = NM6_RndProjSize;

  /* Correct aspect ratio */
  if (NM6_RndFrameW > NM6_RndFrameH)
    rx *= (FLT)NM6_RndFrameW / NM6_RndFrameH;
  else
    ry *= (FLT)NM6_RndFrameH / NM6_RndFrameW;

  NM6_RndMatrProj =
  MatrFrustum(-rx / 2, rx / 2, -ry / 2, ry / 2,
  NM6_RndProjDist, NM6_RndProjFarClip);
  NM6_RndMatrVP = MatrMulMatr(NM6_RndMatrView, NM6_RndMatrProj);
}/* NM6_RndProjSet */

/* NM6_RndCamset */
VOID NM6_RndCamSet( VEC Loc, VEC At, VEC Up )
{
  NM6_RndMatrView = MatrView(Loc, At, Up);
  NM6_RndMatrVP = MatrMulMatr(NM6_RndMatrView, NM6_RndMatrProj);
}/* End of 'NM6_RndCamSet' function */

/* NM6_RndStart */
VOID NM6_RndStart( VOID )
{
  NM6_RndShadersUpdate();
  /* Clear frame */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}/* End of 'NM6_RndStart' function */

/* NM6_RndEnd */
VOID NM6_RndEnd( VOID )
{
  glFinish();
}/* End of 'NM6_RndEnd' function */

/* NM6_RndResize */
VOID NM6_RndResize( INT W, INT H )
{
  glViewport(0, 0, W, H);

  NM6_RndFrameW = W;
  NM6_RndFrameH = H;

  NM6_RndProjSet();
}/* End of 'NM6_RndResize' function */


/* END OF 'rndbase.c' FILE */