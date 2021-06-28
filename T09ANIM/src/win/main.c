/* FILENAME   : main.c
 * PROGRAMMER : NS6
 * DATE       : 17.06.2021
 * PURPOSE    : 3D animation.
 */

#include "../unit/units.h"

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
  NS6_AnimAddUnit(NS6_UnitCreateBtr());
  NS6_AnimAddUnit(NS6_UnitCreateControl());

  /* Message loop */
  while (GetMessage(&msg, NULL, 0, 0))
    DispatchMessage(&msg);
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
  PAINTSTRUCT ps;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    ((MINMAXINFO *)lParam) -> ptMaxTrackSize.y = 
      GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + 2 * GetSystemMetrics(SM_CYBORDER);
    return 0;   
  
  case WM_CREATE:
    SetTimer(hWnd, 30, 1, NULL);
    NS6_AnimInit(hWnd);
    return 0;

  case WM_SIZE:
    NS6_AnimResize(LOWORD(lParam), HIWORD(lParam));
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;

  case WM_TIMER:
    NS6_AnimRender();
    NS6_AnimCopyFrame();
    return 0;

  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
      SendMessage(hWnd, WM_CLOSE, 0, 0);
    return 0;

  case WM_LBUTTONDOWN:
    SetCapture(hWnd);
    return 0;

  case WM_LBUTTONUP:
    ReleaseCapture();
    return 0;

  case WM_MOUSEWHEEL:
    NS6_MouseWheel += (SHORT)HIWORD(wParam);
    return 0;

  case WM_PAINT:
    BeginPaint(hWnd, &ps);
    NS6_AnimCopyFrame();
    EndPaint(hWnd, &ps);
    return 0;

  case WM_DESTROY:
    NS6_AnimClose();
    KillTimer(hWnd, 30);
    PostMessage(hWnd, WM_QUIT, 0, 0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'WinFunc' function */

/* END OF 'pattern.c' FILE */


/* END OF 'main.c' file*/
