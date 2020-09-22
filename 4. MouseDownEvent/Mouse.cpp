#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass=TEXT("Mouse");

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
		  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;
	
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;// | CS_DBLCLKS;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,
		  CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		  NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	
	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}
typedef struct tagline{
	POINT p[1000];//x y가 한번에 담긴 객체

	int iCount;//점개수
}line;
line lines[500];
int iLines =0;

int iTempCount =0; // 실질적으로 점의 갯수를 세어주는 역할을 한다.

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int x;
	static int y;
	//static BOOL bNowDraw=FALSE;

	switch (iMessage) {
	case WM_LBUTTONDOWN:
		x=LOWORD(lParam);
		y=HIWORD(lParam);
	//	bNowDraw=TRUE;
		return 0;
	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON) { // 마우스를 움직였는데 왼쪽버튼을 누른 상태로 움직였다면
			hdc=GetDC(hWnd);
			MoveToEx(hdc, x, y, NULL);
			lines[iLines].p[iTempCount].x = x;
			lines[iLines].p[iTempCount].y = y;
			x=LOWORD(lParam);
			y=HIWORD(lParam);
			LineTo(hdc,x,y);
			ReleaseDC(hWnd,hdc);
			iTempCount++;
		}
		return 0;
	case WM_LBUTTONUP:
	//	bNowDraw=FALSE;
		//마우스 클릭을 중지했을 때
		lines[iLines].iCount = iTempCount;
		iTempCount = 0;
		iLines++;
		return 0;
	case WM_LBUTTONDBLCLK:
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		for (int i = 0; i < iLines; i++){
			for (int j = 0; j < lines[i].iCount-1; j++){
				MoveToEx(hdc, lines[i].p[j].x, lines[i].p[j].y, NULL);
				LineTo(hdc, lines[i].p[j + 1].x, lines[i].p[j+1].y);
			}
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
