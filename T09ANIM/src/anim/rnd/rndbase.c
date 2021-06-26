/* FILE NAME: rndbase.c
* PROGRAMMER: ns6
* DATE: 17.06.2021
* PURPOSE: 3D animation rendering function module.
*/

#include "rnd.h"
#include <wglew.h>
#include <gl/wglext.h>

#pragma comment(lib, "opengl32")

/* NS6_RndInit */
VOID NS6_RndInit( HWND hWnd )
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

  NS6_hRndWnd = hWnd;
  NS6_hRndDC = GetDC(hWnd);

  /* OpenGL init: pixel format setup */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(NS6_hRndDC, &pfd);
  DescribePixelFormat(NS6_hRndDC, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  SetPixelFormat(NS6_hRndDC, i, &pfd);

  /* OpenGL init: setup rendering context */
  NS6_hRndGLRC = wglCreateContext(NS6_hRndDC);
  wglMakeCurrent(NS6_hRndDC, NS6_hRndGLRC);

  /* Initializing GLEW library */
  if (glewInit() != GLEW_OK)
  {
    MessageBox(NS6_hRndWnd, "Error extensions initializing", "Error", MB_ICONERROR | MB_OK);
    exit(0);
  }

  if (!(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    MessageBox(NS6_hRndWnd, "Error: no shaders support", "Error", MB_ICONERROR | MB_OK);
    exit(0);
  }

  /* Enable a new OpenGL profile support */
  wglChoosePixelFormatARB(NS6_hRndDC, PixelAttribs, NULL, 1, &i, &nums);
  hRC = wglCreateContextAttribsARB(NS6_hRndDC, NULL, ContextAttribs);

  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(NS6_hRndGLRC);

  NS6_hRndGLRC = hRC;
  wglMakeCurrent(NS6_hRndDC, NS6_hRndGLRC);

  /* Set default OpenGL parameters */
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.5, 0.5, 0.5, 1);
  NS6_RndShadersInit();
  NS6_RndTexturesInit();
  NS6_RndMtlInit();

  /* Render perametrs */
  NS6_RndProjSize = (FLT)0.1;
  NS6_RndProjDist = NS6_RndProjSize;
  NS6_RndProjFarClip = 300;
  NS6_RndFrameW = 50;
  NS6_RndFrameH = 50;
  NS6_RndCamSet(VecSet(0, 0, 5), VecSet(0, 0, 0), VecSet(0, 1, 0));
}/* End of 'NS6_RndInit' function */

/* NS6_RndClose */
VOID NS6_RndClose( VOID )
{
  NS6_RndTexturesClose();
  NS6_RndMtlClose();
  NS6_RndShadersClose();
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(NS6_hRndGLRC);
  ReleaseDC(NS6_hRndWnd, NS6_hRndDC);
}/* End of 'NS6_RndClose' function */

/* NS6_RndCopyFrame */
VOID NS6_RndCopyFrame( VOID )
{
  SwapBuffers(NS6_hRndDC);
}/* End of 'NS6_RndCopyFrame' function */

/* NS6_RndProjSet */
VOID NS6_RndProjSet( VOID )
{
  FLT rx, ry;

  rx = ry = NS6_RndProjSize;

  /* Correct aspect ratio */
  if (NS6_RndFrameW > NS6_RndFrameH)
    rx *= (FLT)NS6_RndFrameW / NS6_RndFrameH;
  else
    ry *= (FLT)NS6_RndFrameH / NS6_RndFrameW;

  NS6_RndMatrProj = MatrFrustum(-rx / 2, rx / 2, -ry / 2, ry / 2, NS6_RndProjDist, NS6_RndProjFarClip);
  NS6_RndMatrVP = MatrMulMatr(NS6_RndMatrView, NS6_RndMatrProj);
}/* NS6_RndProjSet */

/* NS6_RndCamset */
VOID NS6_RndCamSet( VEC Loc, VEC At, VEC Up )
{
  NS6_RndCamLoc = Loc;
  NS6_RndMatrView = MatrView(Loc, At, Up);
  NS6_RndMatrVP = MatrMulMatr(NS6_RndMatrView, NS6_RndMatrProj);
}/* End of 'NS6_RndCamSet' function */

/* NS6_RndStart */
VOID NS6_RndStart( VOID )
{
  NS6_RndShadersUpdate();
  /* Clear frame */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}/* End of 'NS6_RndStart' function */

/* NS6_RndEnd */
VOID NS6_RndEnd( VOID )
{
  glFinish();
}/* End of 'NS6_RndEnd' function */

/* NS6_RndResize */
VOID NS6_RndResize( INT W, INT H )
{
  glViewport(0, 0, W, H);

  NS6_RndFrameW = W;
  NS6_RndFrameH = H;

  NS6_RndProjSet();
}/* End of 'NS6_RndResize' function */


/* END OF 'rndbase.c' FILE */