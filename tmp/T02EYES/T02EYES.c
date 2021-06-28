/* t01fwin.c */ 
#include <windows.h>
#include <math.h>

#define WND_CLASS_NAME "MNN"

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR *CmdLine, INT ShowCmd)
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  /**/
  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_SHIELD);
  wc.lpszMenuName = NULL;
  wc.lpfnWndProc = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MyWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;  
  /**/
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "ERROR", MB_OK);
    return 0;
  }
  /**/
  hWnd =
    CreateWindow(WND_CLASS_NAME,
      "Kola07ah",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT,
      NULL,
      NULL,
      hInstance,                                                                        
      NULL);
  /**/
  ShowWindow(hWnd, SW_SHOWNORMAL);
  UpdateWindow(hWnd);

  while (GetMessage(&msg, NULL, 0, 0))
  {  
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.lParam;
}

VOID DrawEye( HDC hDC, INT X, INT Y, INT R, INT R1, INT Mx, INT My )
{
  DOUBLE s, p;
  Ellipse(hDC, X - R, Y - R, X + R, Y + R);
  s = sqrt((Mx - X) * (Mx - X) + (My - Y) * (My - Y));
  p = (R - R1) / s;
  if (s < R - R1)
    Ellipse(hDC, Mx - R1, My - R1, Mx + R1, My + R1);
  else
    Ellipse(hDC, (int)(p * (Mx - X) + X - R1), (int)(p * (My - Y) - R1 + Y), (int)(p * (Mx - X) + R1 + X), (int)(p * (My - Y) + R1 + Y));
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  POINT pt;
  static HDC hMemDC;
  static INT w, h;
  static HBITMAP hBm;
  
  switch (Msg)
  { 
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hMemDC = CreateCompatibleDC(hDC);
    ReleaseDC(hWnd, hDC);
    hBm = NULL;
    SetTimer(hWnd, 47, 1, NULL);
    return 0;  
  case WM_TIMER:
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);

    Rectangle(hMemDC, 0, 0, w, h);
    DrawEye(hMemDC, 150, 150, 50, 20, pt.x, pt.y);
    DrawEye(hMemDC, 300, 150, 50, 20, pt.x, pt.y);  
    InvalidateRect(hWnd, NULL, FALSE);
    return 0;
  case WM_SIZE:
    h = HIWORD(lParam);
    w = LOWORD(lParam);
    hDC = GetDC(hWnd);
    if (hBm != NULL) 
       DeleteObject(hBm);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    DeleteDC(hDC);
    SelectObject(hMemDC, hBm);
    SendMessage(hWnd, WM_TIMER, 0, 0);   
    return 0;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
    EndPaint(hWnd, &ps);
    return 0;
    
   case WM_DESTROY:
     if (hBm != NULL) 
       DeleteObject(hBm);
     PostQuitMessage(0);
     KillTimer(hWnd, 47);
     return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
  