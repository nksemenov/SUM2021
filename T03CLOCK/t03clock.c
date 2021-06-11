/* FILENAME: t03clock.c
 * PROGRAMMER: NS6
 * DATE: 11.06.2021
 * PURPOSE: WinAPI Clock drawing application sample.
 */

#include <windows.h>
#include <math.h>

#define WND_CLASS_NAME "cgsgforever"

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
VOID DrawClock( HDC hDC, INT x, INT y, INT r, INT r1, INT Mx, INT My );

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
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  /* Заполняем структуру класса окна */
  wc.style = CS_VREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MyWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  /* Регистрация класса окна */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK | MB_ICONERROR);
    return 0;
  }

  /* Создание окна */
  hWnd=
    CreateWindow(WND_CLASS_NAME,
      "Title",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT,
      NULL,
      NULL,
      hInstance,
      NULL);

  /* Показать и перерисовать окно */
  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  /* Цикл обработки сообщений, пока не будет полученно сообщение 'WM_QUIT' */
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  } 
  return msg.wParam;
}

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
LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  SYSTEMTIME st;
  static HDC hMemDCImage, hMemDCFrame; 
  static HBITMAP hBmImage, hBmFrame;
  static BITMAP bm;
  static HBITMAP hBm;
  static INT h, w, angles, anglem, angleh;
  static DOUBLE pi = 3.14;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 30, 100, NULL);
    
    hDC = GetDC(hWnd);
    hMemDCFrame = CreateCompatibleDC(hDC);
    hMemDCImage = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    hBmImage = LoadImage(NULL, "clockface.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SelectObject(hMemDCImage, hBmImage);
    GetObject(hBmImage, sizeof(bm), &bm);

    hBmFrame = NULL;
    return 0;
  
  case WM_SIZE:
    if (hBmFrame != NULL)
      DeleteObject(hBmFrame);
    h = HIWORD(lParam);
    w = LOWORD(lParam);

    hDC = GetDC(hWnd);
    hBmFrame = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);

    SelectObject(hMemDCFrame, hBmFrame);

    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDCFrame, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
  
  case WM_TIMER:
    SelectObject(hMemDCFrame, GetStockObject(NULL_PEN));
    
    Rectangle(hMemDCFrame, 0, 0, w, h);
    BitBlt(hMemDCFrame, (w - bm.bmWidth)/ 2, (h - bm.bmHeight) / 2, w, h, hMemDCImage, 0, 0, SRCCOPY);
    
    GetLocalTime(&st);
    
    angles = (st.wSecond + st.wMilliseconds / 1000.0) * pi / 60;
    anglem = (st.wMinute + st.wSecond / 60.0) * pi / 60;
    angleh = (st.wHour + st.wMinute / 60.0) * pi / 12;

    SetDCPenColor(hMemDCFrame, RGB(0, 0, 255));
    MoveToEx(hMemDCFrame, (w - bm.bmWidth)/ 2, (h - bm.bmHeight) / 2, NULL);
    LineTo(hMemDCFrame, (w - bm.bmWidth)/ 2 + 50 * sin(angles), (h - bm.bmHeight) / 2 + 50 * cos(angles));
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;

  case WM_ERASEBKGND:
    return 1;
  
  case WM_DESTROY:
    if (hBmImage != NULL)
      DeleteObject(hBmImage);
    KillTimer(hWnd, 47);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}