/* t01fwin.c */ 
#pragma warning(disable: 4244)

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

VOID CLOCK( HDC hDC, INT W, INT H )
{
  SYSTEMTIME st;
  DOUBLE angleS, angleM, angleH, pi = 3.14159265358979323846;
  INT Cx = W / 2, Cy = H / 2, R = 200;
  HPEN hPen;

  GetLocalTime(&st);
  angleS = ((st.wSecond + st.wMilliseconds / 1000.0) * 2 * pi) / 60;
  hPen = CreatePen(PS_SOLID, 10, RGB(255, 0, 0));
  MoveToEx(hDC, Cx, Cy, NULL);
  LineTo(hDC, Cx + R * sin(angleS), Cy - R * cos(angleS));
  
  angleM = ((st.wMinute + st.wSecond / 60.0) * 2 * pi) / 60;
  hPen = CreatePen(PS_SOLID, 10, RGB(100, 100, 100));
  MoveToEx(hDC, Cx, Cy, NULL);
  LineTo(hDC, Cx + R * sin(angleM), Cy - R * cos(angleM));
  
  angleH = (((st.wHour + st.wMinute / 60.0) * 2 * pi) / 12);
  hPen = CreatePen(PS_DASH, 10, RGB(0, 0, 0));
  MoveToEx(hDC, Cx, Cy, NULL);
  LineTo(hDC, Cx + R * sin(angleH), Cy - R * cos(angleH));
  

}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  PAINTSTRUCT ps;
  POINT pt;
  HPEN hPen;
  static HDC hDC;
  static INT w, h, r;
  static BITMAP bm, bmlogo;
  static HBITMAP hBm, hBmClock, hBmLogoXor, hBmLogoAnd;
  static HDC hDCFrame, hDCClock, hMemDCLogo;

  switch (Msg)
  { 
  case WM_CREATE:
    hDC = GetDC(hWnd);
    hDCClock = CreateCompatibleDC(hDC);
    hDCFrame = CreateCompatibleDC(hDC);
    hMemDCLogo = CreateCompatibleDC(hDC); 
    ReleaseDC(hWnd, hDC);
    hBm = NULL;
    SetTimer(hWnd, 47, 1, NULL);
    /*load logo image*/
    hBmClock = LoadImage(NULL, "CLOCK.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmLogoXor = LoadImage(NULL, "xor.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBmLogoAnd = LoadImage(NULL, "and.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hBmLogoXor, sizeof(BITMAP), &bm);
    GetObject(hBmLogoAnd, sizeof(BITMAP), &bmlogo);
    SelectObject(hDCClock, hBmClock);
    GetObject(hBmClock, sizeof(BITMAP), &bm);

    return 0;
  case WM_SIZE:
    h = HIWORD(lParam);
    w = LOWORD(lParam);
    hDC = GetDC(hWnd);
    if (hBm != NULL)
      DeleteObject(hBm);
    hBm = CreateCompatibleBitmap(hDC, w, h);
    ReleaseDC(hWnd, hDC);

    SelectObject(hDCFrame, hBm);
    SendMessage(hWnd, WM_TIMER, 0, 0);
    return 0;
  case WM_TIMER:

    Rectangle(hDCFrame, 0, 0, w, h);

    r = w > h ? h : w;
    StretchBlt(hDCFrame, (w - r) / 2, (h - r) / 2, r, r, hDCClock, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
    SetStretchBltMode(hDCFrame, COLOROKSTRING);
    CLOCK(hDCFrame, w, h);

    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);
    SelectObject(hMemDCLogo, hBmLogoAnd);
    BitBlt(hDCFrame, pt.x - bmlogo.bmWidth / 2, pt.y - bmlogo.bmHeight / 2, bmlogo.bmWidth,
            bmlogo.bmHeight, hMemDCLogo, 0, 0, SRCAND);  
    SelectObject(hMemDCLogo, hBmLogoXor);
    BitBlt(hDCFrame, pt.x - bmlogo.bmWidth / 2, pt.y - bmlogo.bmHeight / 2, bmlogo.bmWidth,
            bmlogo.bmHeight, hMemDCLogo, 0, 0, SRCINVERT);  
    
    SelectObject(hMemDCLogo, hBmLogoAnd);
    BitBlt(hDCFrame, (w - r) / 2, (h - r) / 2, bmlogo.bmWidth,
            bmlogo.bmHeight, hMemDCLogo, 0, 0, SRCAND);  
    SelectObject(hMemDCLogo, hBmLogoXor);
    BitBlt(hDCFrame, (w - r) / 2, (h - r) / 2, bmlogo.bmWidth,
            bmlogo.bmHeight, hMemDCLogo, 0, 0, SRCINVERT);  

    InvalidateRect(hWnd, NULL, TRUE);
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    BitBlt(hDC, 0, 0, w, h, hDCFrame, 0, 0, SRCCOPY);

    EndPaint(hWnd, &ps);
    return 0;
    
   case WM_DESTROY:
    if (hBm != NULL)
      DeleteObject(hBm);
    DeleteDC(hDCFrame);
    DeleteObject(hBmClock);
    DeleteObject(hBmLogoXor);
    DeleteObject(hBmLogoAnd);
    DeleteDC(hDCClock);
    DeleteDC(hMemDCLogo);

    PostQuitMessage(0);
    KillTimer(hWnd, 47);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}                 
  