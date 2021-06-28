/* FILE NAME: main.c
* PROGRAMMER: NM6
* DATE: 17.06.2021
* PURPOSE: 3D animation startup module.
*/

#include "../anim/rnd/rnd.h"

#include "../anim/unit/units.h"

/* Window class name */
#define NM6_WND_CLASS_NAME "My Window Class Name"

/* Forward declaration */
LRESULT CALLBACK NM6_WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

/* The main program function.
* ARGUMENTS:
* - handle of application instance:
* HINSTANCE hInstance;
* - dummy handle of previous application instance (not used):
* HINSTANCE hPrevInstance;
* - command line string:
* CHAR *CmdLine;
* - show window command parameter (see SW_***):
* INT CmdShow;
* RETURNS:
* (INT) Error level for operation system (0 for success).
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
  wc.hIcon = LoadIcon(NULL, IDI_SHIELD);
  wc.hInstance = hInstance;
  wc.lpszClassName = NM6_WND_CLASS_NAME;
  wc.lpszMenuName = NULL;
  wc.lpfnWndProc = NM6_WinFunc;

  /* Register window class */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "Error", MB_OK | MB_ICONERROR);
    return 0;
  }

  hWnd =
  CreateWindow(NM6_WND_CLASS_NAME,
  "KOLA)&AH",
  WS_OVERLAPPEDWINDOW,
  CW_USEDEFAULT, CW_USEDEFAULT,
  CW_USEDEFAULT, CW_USEDEFAULT,
  NULL,
  NULL,
  hInstance,
  NULL);

  /* Show window */
  ShowWindow(hWnd, CmdShow);
  UpdateWindow(hWnd);



  /* Message loop */
  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);

  return 30;
} /* End of 'WinMain' function */

/* Window handle function.
* ARGUMENTS:
* - window handle:
* HWND hWnd;
* - message type (see WM_***):
* UINT Msg;
* - message 'word' parameter:
* WPARAM wParam;
* - message 'long' parameter:
* LPARAM lParam;
* RETURNS:
* (LRESULT) message depende return value.
*/
LRESULT CALLBACK NM6_WinFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC = GetDC(hWnd);

  switch (Msg)
  {
  /* create */
  case WM_CREATE:
    NM6_AnimInit(hWnd);
    SetTimer(hWnd, 47, 1, NULL);
    return 0;

  /* size */
  case WM_SIZE:
    NM6_AnimResize(LOWORD(lParam), HIWORD(lParam));
    SendMessage(hWnd, WM_TIMER, 47, 0);
    return 0;

  /* timer */
  case WM_TIMER:
    NM6_AnimRender();
    return 0;

  /* paint */
  case WM_PAINT:
    NM6_AnimCopyFrame(hDC);
    return 0;

  /* destroy */
  case WM_DESTROY:
    NM6_AnimClose();
    KillTimer(hWnd, 30);
    PostQuitMessage(45);
    return 0;

  /* erase */
  case WM_ERASEBKGND:
    return 1;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}/* End of 'NM6_WinFunc' function */

/* END OF 'main.c' FILE */