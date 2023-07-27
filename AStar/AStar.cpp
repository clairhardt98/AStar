// AStar.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "AStar.h"
#include "Node.h"
#include <queue>


using namespace std;
/*-------------------------------------*/
const int ROW = 10;
const int COL = 15;
vector<vector<Node*>> NVec;

struct cmp {
	bool operator()(Node a, Node b) {
		return a.GetF() < b.GetF();
	}
};
std::priority_queue<Node, vector<Node>, cmp> OpenPQ;
/*-------------------------------------*/
enum Status { INITPROGRAM, SETTINGOBSTACLE, STARTPOINTSET, ENDPOINTSET,SEARCHING };//init에서 시작점 세팅되면 다음으로, 종료점 세팅되면 다음으로, 탐색 완료되면 다음으로
Status status;

void AStar();
void InitNodes();
//도착점 -> 현재 사각형
int GetHVal();
//시작점 -> 현재 사각형
int GetGVal();
int GetFVal();
Node* FindNode();
void FindNodePos();

Node* StartNode;
POINT StartNodePos;
Node* EndNode;
POINT EndNodePos;


void SetNeighbor(int i, int j);
POINT dir[8] =
{
	POINT{0,1},
	POINT{0,-1},
	POINT{-1,0},
	POINT{1,0},

	POINT{-1,-1},
	POINT{1,-1},
	POINT{-1,1},
	POINT{1,1}
};

int cost[8] =
{
	10,10,10,10,14,14,14,14
};

/*-------------------------------------*/
POINT ptMousePos;




#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_ASTAR, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ASTAR));

	MSG msg;

	// 기본 메시지 루프입니다:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}





//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ASTAR));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_ASTAR);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static string statusStr = "CURRENT STATUS: ";
	switch (message)
	{
	case WM_CREATE:
		InitNodes();
		statusStr += "INITPROGRAM";
		status = INITPROGRAM;
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_KEYDOWN:
		if (wParam == '1' && status == INITPROGRAM)
		{
			statusStr = "CURRENT STATUS: ";
			statusStr += "SETTINGOBSTACLE";
			InvalidateRgn(hWnd, NULL, true);
			status = SETTINGOBSTACLE;
		}
		else if (wParam == '1' && status == SETTINGOBSTACLE)
		{
			statusStr = "CURRENT STATUS: ";
			statusStr += "INITPROGRAM";
			InvalidateRgn(hWnd, NULL, true);
			status = INITPROGRAM;
		}
		if (wParam == VK_SPACE && status == ENDPOINTSET)
		{
			//탐색 실행
			statusStr = "CURRENT STATUS: ";
			statusStr += "SEARCHING";
			InvalidateRgn(hWnd, NULL, true);
			status = SEARCHING;
			AStar();
		}

		break;
	case WM_LBUTTONDOWN:
		ptMousePos.x = LOWORD(lParam);
		ptMousePos.y = HIWORD(lParam);
		switch (status)
		{
		case INIT:
			//init 상태에서 클릭했을 때, 그 노드를 시작점으로 변경
			statusStr = "CURRENT STATUS: ";
			statusStr += "STARTPOINTSET";
			StartNode = FindNode();
			StartNode->SetStartPoint();
			InvalidateRgn(hWnd, NULL, true);
			status = STARTPOINTSET;
			break;
		case STARTPOINTSET:
			//startpointset 상태에서 클릭했을 때, 그 노드를 종료점으로 변경
			statusStr = "CURRENT STATUS: ";
			statusStr += "ENDPOINTSET";
			EndNode = FindNode();
			EndNode->SetEndPoint();
			InvalidateRgn(hWnd, NULL, true);
			status = ENDPOINTSET;
			break;
		case SETTINGOBSTACLE:
			//settingobstacle 상태이면 누르는 노드를 장애물로 지정

			FindNode()->SetObstacle();
			InvalidateRgn(hWnd, NULL, true);
			break;
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		for (auto e : NVec)
			for (auto k : e)
				k->Draw(hdc);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
		TextOutA(hdc, 1000, 100, statusStr.c_str(), statusStr.size());
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


void AStar()
{
	OpenPQ.push(*StartNode);
	FindNodePos();
	SetNeighbor(StartNodePos.x, StartNodePos.y);
}

void InitNodes()
{
	for (int i = 0; i < ROW; i++)
	{
		vector<Node*> temp;
		for (int j = 0; j < COL; j++)
		{
			Node* newNode = new Node(i, j);
			temp.push_back(newNode);
		}
		NVec.push_back(temp);
	}
}

Node* FindNode()
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (NVec[i][j]->IsInRect(ptMousePos))
				return NVec[i][j];
		}
	}
	return nullptr;
}

void FindNodePos()
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (NVec[i][j]->Getstat() == STARTPOINT)
			{
				StartNodePos = POINT{ i,j };
				break;
			}
			if (NVec[i][j]->Getstat() == ENDPOINT)
			{
				EndNodePos = POINT{ i,j };
				break;
			}
		}
	}
}
void SetNeighbor(int i, int j)
{
	//open pq에서 꺼내 closed 상태로
	OpenPQ.pop();
	//NVec[i][j]->SetStat(START);
	//벽 바깥일 때, 장애물일 때, closed 배열에 있을 때 무시
	for (int a = 0; a < 8; a++)
	{
		//벽 바깥일 때
		if (i + dir[a].x < 0 || j + dir[a].y < 0)
			continue;
		//장애물일 때
		if (NVec[i + dir[a].x][j + dir[a].y]->Getstat() == OBSTACLE)
			continue;
		//closed 배열에 있을 때
		if (NVec[i + dir[a].x][j + dir[a].y]->Getstat() == NOWINCLOSED || NVec[i + dir[a].x][j + dir[a].y]->Getstat() == STARTPOINT)
			continue;

		if (NVec[i + dir[a].x][j + dir[a].y]->Getstat() == NOWINOPEN)
		{

		}
		NVec[i + dir[a].x][j + dir[a].y]->SetStat(NOWINOPEN);

	}
	//open pq에 있다면 
}
