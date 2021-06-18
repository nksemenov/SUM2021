/* FILENAME: T07GLOBE.c
 * PROGRAMMER: NS6
 * DATE: 14.06.2021
 * PURPOSE: WinAPI Globe drawing application sample.
 */

#include <stdio.h>

#include "globe.h"

#define WND_CLASS_NAME "cgsgforever"

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
  //LARGEINTEGER t;
  PAINTSTRUCT ps;
  HDC hDC;
  POINT pt;
  static HDC hMemDC;
  static HBITMAP hBm;
  static INT h, w, i;
  static CHAR Buf[100];

  switch (Msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 47, 1, NULL);
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    hBm = NULL;

    GLB_TimerInit();
    return 0;
  
  case WM_SIZE:
    h = HIWORD(lParam);
    w = LOWORD(lParam);
    GlobeSet(1.0);
    if (hBm != NULL)
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
    GLB_TimerResponse();
    
    ScreenToClient(hWnd, &pt);
    SetDCPenColor(hMemDC, NULL_PEN);
    Rectangle(hMemDC, 0, 0, w, h);

    GlobeDraw(hMemDC, w, h);

    SetBkMode(hMemDC, TRANSPARENT);
    SetTextColor(hMemDC, RGB(80, 220, 100));
    TextOut(hMemDC, 10, 10, Buf, sprintf(Buf, "FPS: %.3f", GLB_FPS));

    InvalidateRect(hWnd, NULL, TRUE);
    return 0;

  case WM_KEYDOWN:
    if (wParam == 'P')
      IsPause = !IsPause;
    return 0;
  
  case WM_ERASEBKGND:
    return 1;
  
  case WM_DESTROY:
    if (hBm != NULL)
      DeleteObject(hBm);
    DeleteObject(hMemDC);
    KillTimer(hWnd, 47);
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'MyWindowFunc' function */

/* END OF 'T07GLOBE.c' FILE */
