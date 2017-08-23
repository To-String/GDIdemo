// GDIdemo8.cpp : 定义应用程序的入口点。
//
#include "stdafx.h"
#include "GDIdemo8.h"
#include<Windows.h>
#include<tchar.h>
#include<mmsystem.h>

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"Msimg32.lib")

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"【致我们永不熄灭的游戏开发梦想】游戏动画技巧之 透明动画"	//为窗口标题定义的宏

HDC g_hdc = nullptr, g_mdc = nullptr, g_bufdc = nullptr; //全局设备环境句柄，两个全局内存DC句柄
HBITMAP g_hSprite = nullptr, g_hBackGround = nullptr; //定义两个位图句柄，一个用于存储人物图，一个用于存储背景图

DWORD g_tPre = 0, g_tNow = 0;  //声明l两个函数来记录时间,g_tPre记录上一次绘图的时间，g_tNow记录此次准备绘图的时间

int g_iNum, g_iX = 0, g_iY = 0; //g_iNum用来记录图号,g_iX,g_iY分别记录贴图的横纵坐标

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL Game_Init(HWND hwnd);
VOID Game_Paint(HWND hwnd);
BOOL Game_CleanUp(HWND hwnd);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//【1】窗口创建四步曲之一：开始设计一个完整的窗口类
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

	//【2】窗口创建四步曲之二：注册窗口类
	if (!RegisterClassEx(&wndClass))
	{
		return -1;
	}
	//【3】窗口创建四步曲之三：正式创建窗口

	HWND hwnd = CreateWindow(L"ForTheDreamOfGameDevelop", WINDOW_TITLE, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		WINDOW_WIDTH, WINDOW_HEIGHT, nullptr, nullptr, hInstance, nullptr);

	//【4】窗口创建四步曲之四：窗口的移动、显示与更新

	MoveWindow(hwnd, 250, 80, WINDOW_WIDTH, WINDOW_HEIGHT, true);

	ShowWindow(hwnd, nShowCmd);

	UpdateWindow(hwnd);

	//游戏资源的初始化，若初始化失败，弹出一个消息框，并返回FALSE
	if (!Game_Init(hwnd))
	{
		MessageBox(hwnd, L"资源初始化失败", L"消息窗口", 0);
		return FALSE;
	}


	PlaySound(L"仙剑三·原版景天.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_LOOP);

	//【5】消息循环过程
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
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
	UnregisterClass(L"ForTheDreamOfGameDevelop",wndClass.hInstance);
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
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
	HBITMAP bmp;

	g_hdc = GetDC(hwnd);
	g_mdc = CreateCompatibleDC(g_hdc);
	g_bufdc = CreateCompatibleDC(g_hdc);

	bmp = CreateCompatibleBitmap(g_hdc, WINDOW_WIDTH, WINDOW_HEIGHT);    //该函数创建与指定的设备环境相关的设备兼容的位图对象

	SelectObject(g_mdc, bmp);
	g_hSprite = (HBITMAP)LoadImage(nullptr, L"goright.bmp", IMAGE_BITMAP, 480, 108, LR_LOADFROMFILE);
	g_hBackGround = (HBITMAP)LoadImage(NULL, L"bg.bmp", IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);


	g_iNum = 0;
	g_iX = 0;
	g_iY = 350;

	Game_Paint(hwnd);

	return TRUE;
}

VOID Game_Paint(HWND hwnd)
{
	if (g_iNum == 8)
	{
		g_iNum = 0;
	}

	SelectObject(g_bufdc, g_hBackGround);
	BitBlt(g_mdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_bufdc, 0, 0, SRCCOPY);
	SelectObject(g_bufdc, g_hSprite);
	TransparentBlt(g_mdc, g_iX, g_iY, 60, 108, g_bufdc, g_iNum * 60, 0, 60, 108, RGB(255, 0, 0));//采用TransparentBlt透明色彩法


										 //将最后的画面显示在窗口中
	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);

	g_tPre = GetTickCount();     //记录此次绘图时间，供下次游戏循环中判断是否已经达到画面更新操作设定的时间间隔。
	g_iNum++;                    //将“g_iNum”值加1，为下一次要显示的图号

	g_iX += 10;					   //计算下次贴图的坐标
								   //如果贴图坐标超出窗口边缘，将其坐标置为-60
	if (g_iX >= WINDOW_WIDTH)
	{
		g_iX = -60;
	}

}

BOOL Game_CleanUp(HWND hwnd)
{
	//释放资源对象
	DeleteObject(g_hBackGround);
	DeleteObject(g_hSprite);
	DeleteDC(g_bufdc);
	DeleteDC(g_mdc);
	ReleaseDC(hwnd, g_hdc);
	return TRUE;
}