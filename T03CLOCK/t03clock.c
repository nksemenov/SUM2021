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
  static INT h, w, q, l; 
  static DOUBLE angle, pi = 3.14159265558979323846;
  static CHAR *WD[] =
  {
    "ВС", "ПН", "ВТ", "СР", "ЧТ", "ПТ", "СБ"
  };

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
    
    if (hMemDCFrame != NULL)
    {
      HFONT hFnt, hFntOld;
      GetLocalTime(&st);
      
      /*clock*/
      SelectObject(hMemDCFrame, GetStockObject(DC_PEN));
      SetDCPenColor(hMemDCFrame, RGB(0, 0, 255));
      MoveToEx(hMemDCFrame, w / 2, h / 2, NULL);
      angle = (st.wSecond + st.wMilliseconds / 1000.0) * 2 * pi / 60;
      LineTo(hMemDCFrame, w / 2 + 200 * sin(angle), h / 2 - 200 * cos(angle));

      SetDCPenColor(hMemDCFrame, RGB(0, 255, 0));
      MoveToEx(hMemDCFrame, w / 2, h / 2, NULL);
      angle = (st.wMinute + st.wSecond / 60.0) * 2 * pi / 60;
      LineTo(hMemDCFrame, w / 2 + 150 * sin(angle), h / 2 - 150 * cos(angle));

      SetDCPenColor(hMemDCFrame, RGB(255, 0, 0));
      MoveToEx(hMemDCFrame, w / 2, h / 2, NULL);
      angle = (st.wHour + st.wMinute / 60.0) * 2 * pi / 12;
      LineTo(hMemDCFrame, w / 2 + 100 * sin(angle), h / 2 - 100 * cos(angle));
      
      /*text*/
      hFnt = CreateFont(30, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, RUSSIAN_CHARSET,
                        OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH | FF_SWISS, "Cooper");
      
      hFntOld = SelectObject(hMemDCFrame, hFnt);
      SetTextColor(hMemDCFrame, RGB(150, 150, 150));
      SetBkMode(hMemDCFrame, TRANSPARENT);
      TextOut(hMemDCFrame, w / 2, (h + bm.bmHeight) / 2, WD[st.wDayOfWeek], 4);
      SelectObject(hMemDCFrame, hFntOld);
      DeleteObject(hFnt);

      /*polygon*/
    }
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;

  case WM_ERASEBKGND:
    return 1;
  
  case WM_DESTROY:
    if (hBmImage != NULL)
      DeleteObject(hBmImage);
    DeleteObject(hMemDCFrame);
    DeleteObject(hMemDCImage);
    KillTimer(hWnd, 47);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
VOID DrawHand(HDC hDCFrame, INT x, INT y, DOUBLE angle, INT q, INT l)
{
  /*POINT pnt[] =
  {
    {l * sin(angle), l * cos(angle)},
    {},
    {},
    {}
  };*/
}