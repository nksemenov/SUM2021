/* FILENAME   : main.c
 * PROGRAMMER : NS6
 * DATE       : 17.06.2021
 * PURPOSE    : 3D animation.
 */

#include "../anim/rnd/rnd.h"

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
  wc.hIcon = LoadIcon(NULL, IDI_SHIELD);
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
  hWnd = CreateWindow(NS6_WND_CLASS_NAME, "CGSG Primary Practice Group First Window",
    WS_OVERLAPPEDWINDOW,
    /* CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, */
    1920 * 2, 0, 1100, 700,
    NULL, NULL, hInstance, NULL);

  ShowWindow(hWnd, CmdShow);

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
  PAINTSTRUCT ps;
  static INT w, h;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
	  ((MINMAXINFO *)lParam) -> ptMaxTrackSize.y = 
		  GetSystemMetrics(SM_CYMAXTRACK) + GetSystemMetrics(SM_CYCAPTION) + 2 * GetSystemMetrics(SM_CYBORDER);
	 return 0;   
  case WM_CREATE:
	  SetTimer(hWnd, 30, 12, NULL);
	  NS6_RndInit(hWnd);
    return 0;

  case WM_SIZE:
    NS6_RndResize(LOWORD(lParam), HIWORD(lParam));
	SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;

  case WM_TIMER:
    NS6_RndStart();
	Ellipse(NS6_hRndDCFrame, 30, 30, 100, 100);
	NS6_RndEnd();
	
	hDC = GetDC(hWnd);
	
	NS6_RndCopyFrame(hDC);
	ReleaseDC(hWnd, hDC);
    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
   	NS6_RndCopyFrame(hDC);
	EndPaint(hWnd, &ps);
    return 0;

  case WM_KEYDOWN:
	if (wParam == VK_ESCAPE)
		SendMessage(hWnd, WM_CLOSE, 0, 0);
	return 0;
  
  case WM_SYSKEYDOWN:
	if (wParam == VK_RETURN)
	{
	  return 0;
	}
	break;
  
  case WM_CLOSE:
	if (MessageBox(hWnd, "Are you sure?", "Exit", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1) == 1)
	  break;
	return 0;
 
  case WM_DESTROY:
    NS6_RndClose();
	KillTimer(hWnd, 30);
    PostMessage(hWnd, WM_QUIT, 0, 0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'WinFunc' function */

/* END OF 'pattern.c' FILE */


/* END OF 'main.c' file*/
