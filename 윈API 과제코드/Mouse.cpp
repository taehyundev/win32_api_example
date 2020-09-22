#include <windows.h>
#include"resource.h"														//�޴� ���ҽ������� �ڵ������� ������� ���� [RESOURCE.H]�� �ڵ�����

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Mouse");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);						// �޴� ����� ���� NULL�� MAKEINTRESOURCE(IDR_MENU1);�� �ٲ۴�
	WndClass.style = CS_HREDRAW | CS_VREDRAW;// | CS_DBLCLKS;
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

typedef struct tagline{		
	POINT p[1000];															// �� ��ü�� ���� ��ǥ
	int iCount;																// ���� ����
	int iwidth;
}line;
line lines[500];
int iLines = 0;																// ���� ����

int iTempCount;																// �� �Ѱ����� ���� ������ ī��Ʈ�ϱ����� ����(�� ���ʿ�¾���.)
int iCurrentWidth = 1;														// ���� ��ȯ�� ����

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int x;
	static int y;
	HPEN hpen;																// ���� ���� �ֱ����� hpen������ ����
	//static BOOL bNowDraw=FALSE;

	switch (iMessage) {

	case WM_COMMAND:														// �޴� ���ý� �Ͼ �̺�Ʈ WM_COMMAND, ���ϴ� ������ �޴��� �� or��ȭ �ڵ�
		switch (LOWORD(wParam)) {											
		case IDM_1:															// 1 �޴� ���ý�
			iCurrentWidth = 1;												// �� ���� 1�� ��ȯ
			break;
		case IDM_3:															// 1 �޴� ���ý�
			iCurrentWidth = 3;												// �� ���� 3�� ��ȯ
			break;
		case IDM_5:															// 1 �޴� ���ý�
			iCurrentWidth = 5;												// �� ���� 5�� ��ȯ
			break;
		case IDM_7:															// 1 �޴� ���ý�
			iCurrentWidth = 7;												// �� ���� 7�� ��ȯ
			break;
		
		}
		return 0;
	
	case WM_LBUTTONDOWN:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		lines[iLines].p[iTempCount].x = x;									// iLines��° ���� ��ǥ�����߿��� itmepCount��°�� x��ǥ ���� x�� �Է�. ���⼭�� buttonDown�̸��Ƿ�, itempCount���� 0�̴�.
		lines[iLines].p[iTempCount].y = y;									// iLines��° ���� ��ǥ�����߿��� itmepCount��°�� y��ǥ ���� y�� �Է�. ���⼭�� buttonDown�̸��Ƿ�, itempCount���� 0�̴�.
		lines[iLines].iwidth = iCurrentWidth;								// ���� ���� ���� ����
		return 0;

	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON) {											// wParam & MK_LBUTTON�� �ǹ� => ���콺�� �������µ�, ���ʹ�ư�� �������·� �������ٸ��̶�� �ǹ�
			hdc = GetDC(hWnd);
			hpen = CreatePen(PS_SOLID, iCurrentWidth, RGB(255, 0, 0));		//���� �������������� ���� ���� �տ������� ���� ������, ����, ����
			SelectObject(hdc, hpen);										//������ ���� �����Ѵ�.
			MoveToEx(hdc, x, y, NULL);										// ù xy��ġ�� �����ű��
			x = LOWORD(lParam);												// �̵��� ���� x��ǥ��
			y = HIWORD(lParam);												// �̵��� ���� y��ǥ��
			lines[iLines].p[iTempCount].x = x;								// ���ο� x��ǥ�� iLines��° ���� ��ǥ�����߿��� itmepCount��°�� x��ǥ ���� �Է�
			lines[iLines].p[iTempCount].y = y;								// ���ο� y��ǥ�� iLines��° ���� ��ǥ�����߿��� itmepCount��°�� y��ǥ ���� �Է�
			LineTo(hdc, x, y);												// x y���� ���� �׸���.
			ReleaseDC(hWnd, hdc);
			iTempCount++;													// �̵��Ѹ�ŭ ���� ������ �þ�Ƿ�, itempCount���� ����
		}
		return 0;

	case WM_LBUTTONUP:
		//	����ü �ϼ�
		lines[iLines].iCount = iTempCount;									// �� ����ü�� ���� ���� ���� ����
		iTempCount = 0;														// �������� iTempCount�� 0���� �ʱ�ȭ
		iLines++;															// ���� ���� ��Ÿ���� iLines �� 1����  
		//	bNowDraw=FALSE;
		return 0;

	case WM_LBUTTONDBLCLK:
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd,&ps);											// â�� �������� �ö�Ë� ����ġ�� �����ϱ����� �ٽñ׷��ش�. ���� ����ŭ �ݺ��Ҷ� �������� ���� ��-1 ����ŭ �ݺ��ϹǷ� 2��for�ݺ���
		for (int i = 0; i < iLines; i++){									// �ش� ���� ���Ⱚ�� ���� ����� ����
			hpen = CreatePen(PS_SOLID, lines[i].iwidth, RGB(255, 0, 0));		//���� �������������� ���� ���� �տ������� ���� ������, ����, ����
			SelectObject(hdc, hpen);
			for (int j = 0; j < lines[i].iCount - 1; j++){					// �ι�° for �ݺ����� �� ��ü ������ ���� ��-1 ����ŭ �ݺ��Ͽ� ���� �׸����� �Ѵ�.
				MoveToEx(hdc, lines[i].p[j].x, lines[i].p[j].y, NULL);		// lines[i].p[j].x ��, i��° ���� j��° x��ǥ���� y��ǥ���� ���� �ű��. =  ���������� ���� �ű��.
				LineTo(hdc, lines[i].p[j+1].x, lines[i].p[j+1].y);			// j+1��° ��ǥ ���� ���� �׸���.
			}																// ����ü�� ���� ��-1����ŭ �ݺ��ϴ� ������ ��2���� ���� ���� 1���� �����⶧���̴�. ��, ������������ �����ϴ� ���� ����.
			DeleteObject(hpen); // ����� �ݵ�� ���� GDI ��ü
			//�� ����������;
		}
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
