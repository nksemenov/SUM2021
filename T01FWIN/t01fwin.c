#include <windows.h>

#define WND_CLASS_NAME "cgsgforever"

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

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

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
  PAINTSTRUCT ps;
  HDC hDC;
  HPEN hPen;
  INT x, y;
  static POINT pt;

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 47, 30, NULL);
    return 0;
  case WM_TIMER:
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    InvalidateRect(hWnd, NULL, TRUE);
    return 0;
  case WM_LBUTTONDOWN:
    x = LOWORD(lParam);
    y = HIWORD(lParam);

    hDC = GetDC(hWnd);
    Ellipse(hDC, x - 12, y - 12, x + 12, y + 12);

  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    /*InvalidateRect(hWnd, NULL, TRUE/FALSE) */
    hPen = CreatePen(PS_SOLID, 5, RGB(23, 77, 199));
    SelectObject(hDC, hPen);

    MoveToEx(hDC, 300, 200, NULL);
    LineTo(hDC, 300, 400);
    LineTo(hDC, 250, 487);
    MoveToEx(hDC, 300, 400, NULL);
    LineTo(hDC, 350, 487);
    MoveToEx(hDC, 300, 200, NULL);
    LineTo(hDC, 250, 287);
    MoveToEx(hDC, 300, 200, NULL);
    LineTo(hDC, 350, 287);
    
    Ellipse(hDC, 260, 120, 340, 200);

    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    KillTimer(hWnd, 47);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
