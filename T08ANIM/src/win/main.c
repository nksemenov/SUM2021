/* FILENAME   : main.c
 * PROGRAMMER : NS6
 * DATE       : 17.06.2021
 * PURPOSE    : 3D animation.
 */
#include "../def.h"

#include "../unit/units.h"
//#include "../anim/rnd/rnd.h"

/* Window class name */
#define NS6_WND_CLASS_NAME "My Window Class Name"

/* Forward declaration */
LRESULT CALLBACK NS6_WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/* The main program function.
 * ARGUMENTS:
 *   - handle of application instance:
 *       HINSTANCE hInstance;
 *   - dummy handle of previous application instance (not used):
 *       HINSTANCE hPrevInstance;
 *   - command line string:
 *       CHAR *CmdLine;
 *   - show window command parameter (see SW_***):
 *       INT CmdShow;
 * RETURNS:
 *   (INT) Error level for operation system (0 for success).
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT CmdShow )
{
  HWND hWnd;
  MSG msg;
  WNDCLASS wc;

  SetDbgMemHooks();

  /* Fill window class structure */
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_ASTERISK);
  wc.hInstance = hInstance;
  wc.lpszClassName = NS6_WND_CLASS_NAME;
  wc.lpszMenuName = NULL;
  wc.lpfnWndProc = NS6_WinFunc;

  /* Register window class */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error regoister window class", "Error", MB_OK | MB_ICONERROR);
    return 0;
  }

  /* Window creation */
  hWnd = CreateWindow(NS6_WND_CLASS_NAME, "My Window",
         WS_OVERLAPPEDWINDOW,
         CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
         NULL, NULL, hInstance, NULL);

  ShowWindow(hWnd, CmdShow);
  
  /* Create Units */
  NS6_AnimAddUnit(NS6_UnitCreateDeer());

  /* Message loop */
  while (TRUE)
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      DispatchMessage(&msg);
    }
    else
      SendMessage(hWnd, WM_TIMER, 30, 0);
  return 30;
} /* End of 'WinMain' function */

/* Window handle function.
 * ARGUMENTS:
 *   - window handle:
 *      HWND hWnd;
 *   - message type (see WM_***):
 *      UINT Msg;
 *   - message 'word' parameter:
 *      WPARAM wParam;
 *   - message 'long' parameter:
 *      LPARAM lParam;
 * RETURNS:
 *   (LRESULT) message depende return value.
 */
LRESULT CALLBACK NS6_WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  POINT pt;
  PAINTSTRUCT ps;
  static ns6PRIM Pr, PrS, PrF;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    ((MINMAXINFO *)lParam) -> ptMaxTrackSize.y = 
      GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + 2 * GetSystemMetrics(SM_CYBORDER);
    return 0;   
  
  case WM_CREATE:
    SetTimer(hWnd, 30, 1, NULL);
    /*NS6_RndInit(hWnd);
    NS6_RndPrimCreate(&Pr, 4, 6);
    Pr.V[0].P = VecSet(0.5, 0, 0);
    Pr.V[1].P = VecSet(0.5, -1, 0);
    Pr.V[2].P = VecSet(-0.5, -1, 0);
    Pr.V[3].P = VecSet(-0.5, 0, 0);
    Pr.I[0] = 0;
    Pr.I[1] = 1;
    Pr.I[2] = 2;
    Pr.I[3] = 0;
    Pr.I[4] = 2;
    Pr.I[5] = 3;*/
    //NS6_RndPrimCreateSphere(&PrS, VecSet(0, 0, 0), 1, 30, 30);
    //NS6_RndPrimCreatePlane(&Pr, VecSet(5, 0, 5), VecSet(10, 0, 0), VecSet(0, 0, -10), 5, 5);
    //NS6_RndPrimLoad(&PrF, "Deer.obj");
    
    NS6_AnimInit(hWnd);
    return 0;

  case WM_SIZE:
    //NS6_RndResize(LOWORD(lParam), HIWORD(lParam));
    NS6_AnimResize(LOWORD(lParam), HIWORD(lParam));
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;

  case WM_TIMER:
    //NS6_RndStart();
    //GetCursorPos(&pt);
    //ScreenToClient(hWnd, &pt);
    //NS6_RndPrimDraw(&PrS, MatrMulMatr(MatrRotateX(pt.y / 3), MatrRotateY(pt.x / 3)));
    //NS6_RndPrimDraw(&PrS, MatrMulMatr(MatrMulMatr(MatrRotateZ(clock() / 30), MatrRotateX(clock() / 10)), MatrRotateY(clock() / 20)));
    //NS6_RndPrimDraw(&PrF, MatrMulMatr(MatrScale(VecSet(0.01, 0.01, 0.01)), MatrRotateY(clock() / 10)));
    //NS6_RndEnd();

    NS6_AnimRender();
    
    hDC = GetDC(hWnd);

    //NS6_RndCopyFrame(hDC);
    NS6_AnimCopyFrame(hDC);
    ReleaseDC(hWnd, hDC);
    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    //NS6_RndCopyFrame(hDC);
    NS6_AnimCopyFrame(hDC);
    EndPaint(hWnd, &ps);
    return 0;

  case WM_DESTROY:
    /*NS6_RndPrimFree(&Pr);
    NS6_RndPrimFree(&PrS);
    NS6_RndPrimFree(&PrF);
    NS6_RndClose();*/
    NS6_AnimClose();
    KillTimer(hWnd, 30);
    PostMessage(hWnd, WM_QUIT, 0, 0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'WinFunc' function */

/* END OF 'pattern.c' FILE */


/* END OF 'main.c' file*/
