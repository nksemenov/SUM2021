#include "rnd.h"
#include <wglew.h>
#include <gl/wglext.h>

#pragma comment(lib, "opengl32")

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
    MessageBox(NS6_hRndWnd, "Error extensions initializing", "Error",
      MB_ICONERROR | MB_OK);
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
  glClearColor(0.08, 0.16, 0.08, 1);
  NS6_RndShadersInit();

  /* Render perametrs */
  NS6_RndProjSize = 0.1;
  NS6_RndProjDist = NS6_RndProjSize;
  NS6_RndProjFarClip = 300;
  NS6_RndFrameW = 50;
  NS6_RndFrameH = 50;
  NS6_RndCamSet(VecSet(0, 0, 30), VecSet(0, 0, 0), VecSet(0, 1, 0));
}/* End of 'NS6_RndInit' function */

VOID NS6_RndClose( VOID )
{
  if (NS6_hRndBmFrame != NULL)
    DeleteObject(NS6_hRndBmFrame);
  DeleteDC(NS6_hRndDCFrame);
  NS6_RndShadersClose();
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(NS6_hRndGLRC);
  ReleaseDC(NS6_hRndWnd, NS6_hRndDC);
}

VOID NS6_RndResize( INT W, INT H )
{
  glViewport(0, 0, W, H);

  NS6_RndFrameW = W;
  NS6_RndFrameH = H;

  NS6_RndProjSet();
}

VOID NS6_RndCopyFrame( HDC hDC )
{
  SwapBuffers(NS6_hRndDC);
}

VOID NS6_RndStart( VOID )
{
  NS6_RndShadersUpdate();
  /* Clear frame */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

VOID NS6_RndEnd( VOID )
{
  glFinish();
}

VOID NS6_RndProjSet( VOID )
{
  DBL rx, ry;

  rx = ry = NS6_RndProjSize;

  if (NS6_RndFrameW > NS6_RndFrameH)
    rx *= (DBL)NS6_RndFrameW / NS6_RndFrameH;
  else
    ry *= (DBL)NS6_RndFrameH / NS6_RndFrameW;

  NS6_RndMatrProj = MatrFrustum(-rx / 2, rx / 2, -ry / 2, ry / 2, NS6_RndProjDist, NS6_RndProjFarClip);
  NS6_RndMatrVP = MatrMulMatr(NS6_RndMatrView, NS6_RndMatrProj);
}

VOID NS6_RndCamSet( VEC Loc, VEC At, VEC Up )
{
  NS6_RndMatrView = MatrView(Loc, At, Up);
  NS6_RndMatrVP = MatrMulMatr(NS6_RndMatrView, NS6_RndMatrProj);
}