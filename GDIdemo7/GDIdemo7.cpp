#include "stdafx.h"
#include "GDIdemo7.h"
#include<tchar.h> //使用swprintf 系列字符格式化函数要包含 tchar.h  头文件
#include<Windows.h>
#include<mmsystem.h>

#pragma comment(lib,"winmm.lib")

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"【致我们永不熄灭的游戏开发梦想】游戏动画技巧之 游戏循环动画显示 示例程序"	//为窗口标题定义的宏


//	描述：全局变量的声明
HDC	 g_hdc = nullptr, g_mdc = nullptr;     //全局设备环境句柄与全局内存DC句柄
HBITMAP	 g_hSprite[12];						//声明位图数组用来储存各张人物位图
DWORD	g_tPre = 0, g_tNow = 0;          //声明l两个变量来记录时间,g_tPre记录上一次绘图的时间，g_tNow记录此次准备绘图的时间
int	g_iNum = 0;					//g_iNum变量用来记录目前显示的图号

					//	描述：全局函数声明，防止“未声明的标识”系列错误
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL Game_Init(HWND hwnd);
VOID Game_Paint(HWND hwnd);
BOOL Game_CleanUp(HWND hwnd);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSEX wndClass = { 0 };

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;
	wndClass.hIcon = (HICON)::LoadImage(nullptr, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = 0;
	wndClass.lpszClassName = L"ForTheDreamOfGameDevelop";		//用一个以空终止的字符串，指定窗口类的名字。
	
	if (!RegisterClassEx(&wndClass))
	{
		return -1;
	}

	//【3】窗口创建四步曲之三：正式创建窗口
	HWND hwnd = CreateWindow(L"ForTheDreamOfGameDevelop", WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, nullptr, nullptr, hInstance, nullptr);

	MoveWindow(hwnd, 250, 80, WINDOW_WIDTH, WINDOW_HEIGHT, true);

	ShowWindow(hwnd, nShowCmd);

	UpdateWindow(hwnd);

	//游戏资源的初始化，若初始化失败，弹出一个消息框，并返回FALSE
	if (!Game_Init(hwnd))
	{
		MessageBox(hwnd, L"资源初始化失败", L"消息窗口", 0);
		return FALSE;
	}

	PlaySound(L"仙剑·重楼.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_LOOP);

	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0,0, PM_REMOVE))
		{
			TranslateMessage(&msg);		//将虚拟键消息转换为字符消息
			DispatchMessage(&msg);			//分发一个消息给窗口程序。
		}
		else
		{
			g_tNow = GetTickCount();

			if (g_tNow - g_tPre >= 100)
			{
				Game_Paint(hwnd);
			}
		}
	}


	UnregisterClass(L"ForTheDreamOfGameDevelop", wndClass.hInstance);
	return 0;

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hwnd);
		}
		break;
	case WM_DESTROY:
		Game_CleanUp(hwnd);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

BOOL Game_Init(HWND hwnd)
{
	g_hdc = GetDC(hwnd); //获取设备环境句柄

	wchar_t filename[20];

	//载入各个人物位图

	for (int i = 0; i < 12; ++i)
	{
		memset(filename, 0,sizeof(filename));
		swprintf_s(filename, L"%d.bmp", i);
		g_hSprite[i] = (HBITMAP)LoadImage(nullptr, filename, IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);
	}

	g_mdc = CreateCompatibleDC(g_hdc);   //建立兼容设备环境的内存DC
	Game_Paint(hwnd);

	return TRUE;
}

VOID Game_Paint(HWND hwnd)
{
	if (g_iNum == 11)
	{
		g_iNum = 0;

	}
	SelectObject(g_mdc, g_hSprite[g_iNum]);
	BitBlt(g_hdc, 0, 0, 800, 600, g_mdc, 0, 0, SRCCOPY);

	g_tPre = GetTickCount();
	g_iNum++;
}

BOOL Game_CleanUp(HWND hwnd)
{
	for (int i = 0; i < 12; ++i)
	{
		DeleteObject(g_hSprite[i]);
	}
	DeleteDC(g_mdc);
	ReleaseDC(hwnd, g_hdc);
	return TRUE;
}