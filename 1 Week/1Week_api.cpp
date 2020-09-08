#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("Class");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}
int b_x[1000], b_y[1000];
int r_x[1000], r_y[1000];
int count = 0;
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH HBrush;
	int x, y;
	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		return 0;
	case WM_LBUTTONDOWN:
		//blue
		hdc = GetDC(hWnd);
		b_x[count] = x = LOWORD(lParam);
		b_y[count] = y = HIWORD(lParam);
		count++;
		HBrush = CreateSolidBrush(RGB(0, 0, 255));
		SelectObject(hdc, HBrush);
		Ellipse(hdc, x - 10, y - 10, x + 10, y + 10);
		return 0;
	case WM_RBUTTONDOWN:
		//red
		hdc = GetDC(hWnd);
		r_x[count] = x = LOWORD(lParam);
		r_y[count] = y = HIWORD(lParam);
		count++;
		HBrush = CreateSolidBrush(RGB(255, 0, 0));
		SelectObject(hdc, HBrush);
		Ellipse(hdc, x - 10, y - 10, x + 10, y + 10);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		for (int i = 0; i < count; i++) {
			HBrush = CreateSolidBrush(RGB(255, 0, 0));
			SelectObject(hdc, HBrush);
			Ellipse(hdc, r_x[i] - 10, r_y[i] - 10, r_x[i] + 10, r_y[i] + 10);


			HBrush = CreateSolidBrush(RGB(0, 0, 255));
			SelectObject(hdc, HBrush);
			Ellipse(hdc, b_x[i] - 10, b_y[i] - 10, b_x[i] + 10, b_y[i] + 10);
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
