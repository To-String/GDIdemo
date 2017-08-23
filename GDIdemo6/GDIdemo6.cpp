#include "stdafx.h"
#include "GDIdemo6.h"
#include<tchar.h>
#include<Windows.h>
#include<mmsystem.h>

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"Msimg32.lib") //添加使用TransparentBlt函数所需的库文件

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"【致我们永不熄灭的游戏开发梦想】游戏动画技巧之 定时器动画显示 示例程序"

HDC g_hdc = nullptr, g_mdc = nullptr;   //全局设备环境句柄与全局内存DC句柄
HBITMAP g_hSprite[12];   //声明位图数组用来储存各张人物位图
int g_iNum = 0;  //"g_iNum"变量用来记录目前显示的图号

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL		Game_Init(HWND hwnd);			//在此函数中进行资源的初始化
VOID		Game_Paint(HWND hwnd);		//在此函数中进行绘图代码的书写
BOOL		Game_CleanUp(HWND hwnd);	//在此函数中进行资源的清理

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
	wndClass.hCursor = (HCURSOR)LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszClassName = nullptr;
	wndClass.lpszClassName = L"ForTheDreamOfGameDevelop";		//用一个以空终止的字符串，指定窗口类的名字。

	if (!RegisterClassEx(&wndClass))
	{
		return -1;
	}

	HWND hwnd = CreateWindow(L"ForTheDreamOfGameDevelop", WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, nullptr, nullptr, hInstance, nullptr);

	MoveWindow(hwnd, 250, 80, WINDOW_WIDTH, WINDOW_HEIGHT, true);
	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	if (!Game_Init(hwnd))
	{
		MessageBox(hwnd, L"资源初始化失败", L"消息窗口",0);
		return -1;
	}
	PlaySound(L"鸟之诗.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_LOOP); //循环播放背景音乐 

	MSG msg = { 0 };
	while ( msg.message != WM_QUIT )
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);		//将虚拟键消息转换为字符消息
			DispatchMessage(&msg);			//分发一个消息给窗口程序。
		}
	}

	UnregisterClass(L"ForTheDreamOfGameDevelop", wndClass.hInstance);

	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_TIMER:
		Game_Paint(hwnd);  //调用 Game_Paint()函数进行窗口绘图
		break;

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
	g_hdc = GetDC(hwnd);
	
	wchar_t filename[20];

	for (int i = 0; i < 12; ++i)
	{
		memset(filename, 0, sizeof(filename));
		swprintf_s(filename, L"%d.bmp", i);
		g_hSprite[i] = (HBITMAP)LoadImage(nullptr, filename, IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);
	}

	g_mdc = CreateCompatibleDC(g_hdc); //建立兼容设备环境的内存DC
	g_iNum = 0;              //设置初始的显示图号为"0"
	SetTimer(hwnd, 1, 90, nullptr);   //建立定时器，间隔0.09秒发出消息
	Game_Paint(hwnd);

	return TRUE;
}

VOID Game_Paint(HWND hwnd)
{
	if (g_iNum == 11)
	{
		g_iNum = 0;
	}
	SelectObject(g_mdc, g_hSprite[g_iNum]);//根据图号选入对应的位图
	BitBlt(g_hdc,0,0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);         //以目前图号进行窗口贴图)

																				  //图号自增
	g_iNum++;                    //将“g_iNum”值加1，为下一次要显示的图号
}

BOOL Game_CleanUp(HWND hwnd)
{
	KillTimer(hwnd, 1);   //删除所建立的定时器  
						  //释放资源对象
	for (int i = 0; i < 12; ++i)
	{
		DeleteObject(g_hSprite[i]);
	}
	DeleteDC(g_mdc);
	ReleaseDC(hwnd, g_hdc);  //释放设备环境
	return TRUE;
}