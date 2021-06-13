/* FILENAME: t03clock.c
 * PROGRAMMER: NS6
 * DATE: 11.06.2021
 * PURPOSE: WinAPI Clock drawing application sample.
 */

#include <windows.h>
#include <math.h>
#include <time.h>


#define WND_CLASS_NAME "cgsgforever"
VOID DrawHand(HDC hDCFrame, INT x, INT y, DOUBLE a, INT q, INT l);

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

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
  POINT pt;
  static HDC hMemDCImage, hMemDCFrame, hMemDCLogo;
  static HBITMAP hBmImage, hBmFrame, hBmLogoAnd, hBmLogoXor;
  static BITMAP bm, bmLogo;
  HBITMAP hBm;
  static INT w, h, r;
  static DOUBLE angle, pi = 3.14159265558979323846;
  CHAR *WD[] =
  {
    "ВС", "ПН", "ВТ", "СР", "ЧТ", "ПТ", "СБ"
  };

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 30, 1, NULL);
    
    hDC = GetDC(hWnd);
    hMemDCFrame = CreateCompatibleDC(hDC);
    hMemDCImage = CreateCompatibleDC(hDC);
    hMemDCLogo= CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);

    hBmImage = LoadImage(NULL, "clockface.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmLogoAnd = LoadImage(NULL, "and.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmLogoXor = LoadImage(NULL, "xor.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    SelectObject(hMemDCImage, hBmImage);
    GetObject(hBmImage, sizeof(bm), &bm);
    GetObject(hBmLogoXor, sizeof(bm), &bmLogo);

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
    SelectObject(hMemDCFrame, GetStockObject(WHITE_BRUSH));
    
    r = w < h ? w : h;

    Rectangle(hMemDCFrame, 0, 0, w + 1, h + 1);
    //BitBlt(hMemDCFrame, (w - bm.bmWidth)/ 2, (h - bm.bmHeight) / 2, w, h, hMemDCImage, 0, 0, SRCCOPY);
    SetStretchBltMode(hMemDCFrame, COLORONCOLOR);
    StretchBlt(hMemDCFrame, (w - r) / 2, (h - r) / 2, r, r, hMemDCImage, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

    if (hMemDCFrame != NULL)
    {
      HFONT hFnt, hFntOld;
      GetLocalTime(&st);
      
      /*clock*/
      SelectObject(hMemDCFrame, GetStockObject(DC_PEN));
      SelectObject(hMemDCFrame, GetStockObject(DC_BRUSH));
      
      SetDCPenColor(hMemDCFrame, RGB(255, 0, 0));
      SetDCBrushColor(hMemDCFrame, RGB(255, 0, 0));
      //MoveToEx(hMemDCFrame, w / 2, h / 2, NULL);
      angle = (st.wHour + st.wMinute / 60.0) * 2 * pi / 12;
      //LineTo(hMemDCFrame, w / 2 + 100 * sin(angle), h / 2 - 100 * cos(angle));
      DrawHand(hMemDCFrame, w / 2, h / 2, angle, r / 40, r / 8);
            
      SetDCPenColor(hMemDCFrame, RGB(0, 255, 0));
      SetDCBrushColor(hMemDCFrame, RGB(0, 255, 0));
      //MoveToEx(hMemDCFrame, w / 2, h / 2, NULL);
      angle = (st.wMinute + st.wSecond / 60.0) * 2 * pi / 60;
      //LineTo(hMemDCFrame, w / 2 + 150 * sin(angle), h / 2 - 150 * cos(angle));
      DrawHand(hMemDCFrame, w / 2, h / 2, angle, r / 43, r / 4);

      SetDCPenColor(hMemDCFrame, RGB(0, 0, 255));
      SetDCBrushColor(hMemDCFrame, RGB(0, 0, 255));
      //MoveToEx(hMemDCFrame, w / 2, h / 2, NULL);
      angle = (st.wSecond + st.wMilliseconds / 1000.0) * 2 * pi / 60;
      //LineTo(hMemDCFrame, w / 2 + 200 * sin(angle), h / 2 - 200 * cos(angle));
      DrawHand(hMemDCFrame, w / 2, h / 2, angle, r / 65, r / 3);
      SelectObject(hMemDCFrame, GetStockObject(NULL_BRUSH));

      /*text*/
      hFnt = CreateFont(30, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, RUSSIAN_CHARSET,
                        OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH | FF_SWISS, "Cooper");
      
      hFntOld = SelectObject(hMemDCFrame, hFnt);
      SetTextColor(hMemDCFrame, RGB(150, 150, 150));
      SetBkMode(hMemDCFrame, TRANSPARENT);
      TextOut(hMemDCFrame, w / 2, (h + bm.bmHeight) / 2, WD[st.wDayOfWeek], 4);
      SelectObject(hMemDCFrame, hFntOld);
      DeleteObject(hFnt);

      GetCursorPos(&pt);
      ScreenToClient(hWnd, &pt);
      SelectObject(hMemDCLogo, hBmLogoAnd);
      BitBlt(hMemDCFrame, pt.x - bmLogo.bmWidth / 2, pt.y - bmLogo.bmWidth / 2, bmLogo.bmWidth, bmLogo.bmHeight, hMemDCLogo, 0, 0, SRCAND);
      SelectObject(hMemDCLogo, hBmLogoXor);
      BitBlt(hMemDCFrame, pt.x - bmLogo.bmWidth / 2, pt.y - bmLogo.bmWidth / 2, bmLogo.bmWidth, bmLogo.bmHeight, hMemDCLogo, 0, 0, SRCINVERT);
      

      for (r = 0; r < 30; r++)
      {
          pt.x = w / 2 + w / 3 * sin(2 * clock() / 1000.0 - r / 4.8);
          pt.y = h / 2 + h / 3 * cos(clock() / 1000.0 - r / 4.8);

          SelectObject(hMemDCLogo, hBmLogoAnd);
          BitBlt(hMemDCFrame, pt.x - bmLogo.bmWidth / 2, pt.y - bmLogo.bmWidth / 2, bmLogo.bmWidth, bmLogo.bmHeight, hMemDCLogo, 0, 0, SRCAND);
          SelectObject(hMemDCLogo, hBmLogoXor);
          BitBlt(hMemDCFrame, pt.x - bmLogo.bmWidth / 2, pt.y - bmLogo.bmWidth / 2, bmLogo.bmWidth, bmLogo.bmHeight, hMemDCLogo, 0, 0, SRCINVERT);
      }
    }
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;

  case WM_ERASEBKGND:
    return 1;
  
  case WM_DESTROY:
    if (hBmFrame != NULL)
      DeleteObject(hBmFrame);
    DeleteObject(hBmImage);
    DeleteObject(hBmLogoAnd);
    DeleteObject(hBmLogoXor);
    DeleteObject(hMemDCImage);
    DeleteObject(hMemDCLogo);
    KillTimer(hWnd, 47);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
VOID DrawHand(HDC hDCFrame, INT x, INT y, DOUBLE a, INT q, INT l)
{
  POINT pnt[] =
  {
    {x + l * sin(a), y - l * cos(a)},
    {x + q * cos(a), y + q * sin(a)},
    {x - q * sin(a), y + q * cos(a)},
    {x - q * cos(a), y - q * sin(a)}
  };
  Polygon(hDCFrame, pnt, sizeof(pnt) / sizeof(pnt[0]));
}