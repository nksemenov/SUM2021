#include <windows.h>
#include <math.h>

#define WND_CLASS_NAME "cgsgforever"

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
VOID DrawEye( HDC hDC, INT x, INT y, INT r, INT r1, INT Mx, INT My );
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

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  PAINTSTRUCT ps;
  HPEN hPen;
  static HDC hDC;
  POINT pt;
  static HDC hMemDC;
  static HBITMAP hBm;
  static INT h, w, x, y, r, r1;

  x = 300;
  y = 300;
  r = 90;
  r1 = 20;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 47, 30, NULL);
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    hBm = NULL;
    return 0;
  
  case WM_SIZE:
    h = HIWORD(lParam);
    w = LOWORD(lParam);
    if(hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC(hWnd);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);
    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    
    EndPaint(hWnd, &ps);
    return 0;
  
  case WM_TIMER:
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    DrawEye(hMemDC, x, y, r, r1, pt.x, pt.y);
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;

  case WM_ERASEBKGND:
    return 1;
  
  case WM_DESTROY:
    if (hBm != NULL)
      DeleteObject(hBm);
    DeleteObject(hBm);
    KillTimer(hWnd, 47);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}

VOID DrawEye( HDC hDC, INT x, INT y, INT r, INT r1, INT Mx, INT My )
{
  INT x1, y1, k, dx, dy, h, w;
  DOUBLE l;
  dx = Mx - x;
  dy = My - y; 
  l = sqrt((float)(dx * dx) + (float)(dy * dy));
  k = (r - r1) / l;
  x1 = x + (Mx - x) * l;
  y1 = y + (My - y) * l;

  SetDCPenColor(hDC, RGB(0, 255, 0));
  Ellipse(hDC, x - r, y - r, x + r, y + r);
  SetDCPenColor(hDC, RGB(0, 0, 255));
  if (l < (r - r1))
    Ellipse(hDC, Mx - r1, My - r1, Mx + r1, My + r1);
  else
    Ellipse(hDC, x1 - r1, y1 - r1, x1 + r1, y1 + r1);
}
