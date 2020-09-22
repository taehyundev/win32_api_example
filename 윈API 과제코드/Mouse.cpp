#include <windows.h>
#include"resource.h"														//메뉴 리소스에대해 자동생성된 헤더파일 선언 [RESOURCE.H]는 자동생성

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
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);						// 메뉴 사용을 위해 NULL을 MAKEINTRESOURCE(IDR_MENU1);로 바꾼다
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
	POINT p[1000];															// 선 객체에 대한 좌표
	int iCount;																// 점의 갯수
	int iwidth;
}line;
line lines[500];
int iLines = 0;																// 선의 개수

int iTempCount;																// 선 한개안의 점의 개수를 카운트하기위한 변수(꼭 쓸필요는없다.)
int iCurrentWidth = 1;														// 굵기 변환용 변수

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int x;
	static int y;
	HPEN hpen;																// 선에 색을 넣기위해 hpen변수를 생성
	//static BOOL bNowDraw=FALSE;

	switch (iMessage) {

	case WM_COMMAND:														// 메뉴 선택시 일어난 이벤트 WM_COMMAND, 이하는 각각의 메뉴별 값 or변화 코드
		switch (LOWORD(wParam)) {											
		case IDM_1:															// 1 메뉴 선택시
			iCurrentWidth = 1;												// 펜 굵기 1로 변환
			break;
		case IDM_3:															// 1 메뉴 선택시
			iCurrentWidth = 3;												// 펜 굵기 3로 변환
			break;
		case IDM_5:															// 1 메뉴 선택시
			iCurrentWidth = 5;												// 펜 굵기 5로 변환
			break;
		case IDM_7:															// 1 메뉴 선택시
			iCurrentWidth = 7;												// 펜 굵기 7로 변환
			break;
		
		}
		return 0;
	
	case WM_LBUTTONDOWN:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		lines[iLines].p[iTempCount].x = x;									// iLines번째 선의 좌표값들중에서 itmepCount번째의 x좌표 값에 x를 입력. 여기서는 buttonDown이르므로, itempCount값은 0이다.
		lines[iLines].p[iTempCount].y = y;									// iLines번째 선의 좌표값들중에서 itmepCount번째의 y좌표 값에 y를 입력. 여기서는 buttonDown이르므로, itempCount값은 0이다.
		lines[iLines].iwidth = iCurrentWidth;								// 현재 굵기 값을 저장
		return 0;

	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON) {											// wParam & MK_LBUTTON의 의미 => 마우스를 움직였는데, 왼쪽버튼을 누른상태로 움직였다면이라는 의미
			hdc = GetDC(hWnd);
			hpen = CreatePen(PS_SOLID, iCurrentWidth, RGB(255, 0, 0));		//선에 색상입히기위해 펜을 생성 앞에서부터 각각 선형태, 굵기, 색상
			SelectObject(hdc, hpen);										//생성한 펜을 선택한다.
			MoveToEx(hdc, x, y, NULL);										// 첫 xy위치에 펜을옮긴다
			x = LOWORD(lParam);												// 이동한 점의 x좌표값
			y = HIWORD(lParam);												// 이동한 점의 y좌표값
			lines[iLines].p[iTempCount].x = x;								// 새로운 x좌표를 iLines번째 선의 좌표값들중에서 itmepCount번째의 x좌표 값에 입력
			lines[iLines].p[iTempCount].y = y;								// 새로운 y좌표를 iLines번째 선의 좌표값들중에서 itmepCount번째의 y좌표 값에 입력
			LineTo(hdc, x, y);												// x y까지 선을 그린다.
			ReleaseDC(hWnd, hdc);
			iTempCount++;													// 이동한만큼 점의 개수가 늘어나므로, itempCount값이 증가
		}
		return 0;

	case WM_LBUTTONUP:
		//	선객체 완성
		lines[iLines].iCount = iTempCount;									// 한 선객체에 대한 점의 개수 저장
		iTempCount = 0;														// 갯수변소 iTempCount를 0으로 초기화
		iLines++;															// 선의 수를 나타내는 iLines 값 1증가  
		//	bNowDraw=FALSE;
		return 0;

	case WM_LBUTTONDBLCLK:
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd,&ps);											// 창이 내려갔다 올라올떄 선위치를 유지하기위해 다시그려준다. 선의 수만큼 반복할때 선내부의 점의 수-1 번만큼 반복하므로 2중for반복문
		for (int i = 0; i < iLines; i++){									// 해당 선의 굵기값을 펜의 굵기로 지정
			hpen = CreatePen(PS_SOLID, lines[i].iwidth, RGB(255, 0, 0));		//선에 색상입히기위해 펜을 생성 앞에서부터 각각 선형태, 굵기, 색상
			SelectObject(hdc, hpen);
			for (int j = 0; j < lines[i].iCount - 1; j++){					// 두번째 for 반복문은 선 객체 내부의 점의 수-1 번만큼 반복하여 선을 그리도록 한다.
				MoveToEx(hdc, lines[i].p[j].x, lines[i].p[j].y, NULL);		// lines[i].p[j].x 즉, i번째 선의 j번째 x좌표값과 y좌표값에 펜을 옮긴다. =  시작점으로 펜을 옮긴다.
				LineTo(hdc, lines[i].p[j+1].x, lines[i].p[j+1].y);			// j+1번째 좌표 까지 선을 그린다.
			}																// 선객체의 점의 수-1번만큼 반복하는 이유는 점2개에 대해 선은 1개가 나오기때문이다. 즉, 마지막점에서 시작하는 선이 없다.
			DeleteObject(hpen); // 사용후 반드시 삭제 GDI 객체
			//단 선택해제후;
		}
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
