/* t01fwin.c */ 
#include <windows.h>

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
  wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
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
      "Title",
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

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  switch (Msg)
  { 
  HDC hDC;
  PAINTSTRUCT ps;
  HPEN hPen, hPenOld;
  POINT pt;
  int x, y, h, w;

  case WM_CREATE:
    SetTimer(hWnd, 47, 30, NULL);
    return 0;
  case WM_LBUTTONDOWN:
    x = LOWORD(lParam);
    y = HIWORD(lParam);
  case WM_SIZE:
    w = LOWORD(lParam);
    h = HIWORD(lParam);

  case WM_TIMER:
    
    hDC = GetDC(hWnd);
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);

    MoveToEx(hDC, 30, 300, NULL);
    LineTo(hDC, pt.x, pt.y);

    InvalidateRect(hWnd, NULL, FALSE);
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
        
    hPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
    Ellipse(hDC, 100, 100, 200, 200);

    EndPaint(hWnd, &ps);
      return 0;
    
   case WM_DESTROY:
     PostQuitMessage(0);
     KillTimer(hWnd, 47);
     return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
  