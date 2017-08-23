#include<iostream>
#include "stdafx.h"
#include "GDIdemoCore.h"
#include<Windows.h>
using namespace std;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"【致我们永不熄灭的游戏开发梦想】GDI程序核心框架" // 为窗口标题定义的宏

//全局变量声明
HDC g_hdc = NULL;  //全局环境句柄

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);  //窗口过程函数的声明

BOOL Game_Init(HWND hwnd); //在此函数中进行资源的初始化
VOID Game_Paint(HWND hwnd); //在此函数中进行绘图代码的书写
BOOL Game_CleanUp(HWND hwnd); //在此函数中进行资源的清理


//-----------------------------------【WinMain( )函数】--------------------------------------

//	描述：Windows应用程序的入口函数，我们的程序从这里开始

//------------------------------------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//【1】窗口创建四步曲之一：开始设计一个完整的窗口类

	WNDCLASSEX wndClass = { 0 };  //用WNDCLASSEX 定义了一个窗口类
	wndClass.cbSize = sizeof(WNDCLASSEX);   //设置结构体的字节数大小
	wndClass.style = CS_HREDRAW | CS_VREDRAW;  //设置窗口的样式
	wndClass.lpfnWndProc = WndProc;//设置指向窗口过程函数的指针
	wndClass.cbClsExtra = 0; //窗口类的附加内存，取0就可以了
	wndClass.cbWndExtra = 0;//窗口类的附加内存，依然取0就可以了
	wndClass.hInstance = hInstance;//指定包含窗口过程的程序的实例句柄，填写WinMain函数的第一个参数，也就是该程序当前运行的实例句柄传递给它
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);  //用于指定窗口类的图标句柄，这个成员变量必须是个图标资源的句柄
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //指定窗口类的光标句柄,一般这个参数设置为默认的箭头光标
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);//为hbrBackground成员指定一个灰色画刷句柄
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"ForTheDreamOfGameDevelop"; //用一个以空终止的字符串，指定窗口类的名字，名字是自定义的，后面会用到，用来标识我们正在定义的这个窗口类


														  //【2】 窗口创建四部曲之二，  注册窗口类
	if (!RegisterClassEx(&wndClass)) //设计窗口类之后，需要对窗口类进行注册，这样才能创建该类型的窗口，填的就是我们窗口类的对象（实例）
	{
		return -1;
	}

	//【3】  窗口创建四部之三，正式创建窗口

	//hwnd 就是创建该窗口的句柄
	HWND hwnd = CreateWindow
	(L"ForTheDreamOfGameDevelop", WINDOW_TITLE,  //喜闻乐见的创建窗口函数CreateWindow
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
		NULL, NULL, hInstance, NULL
	);

	//【4】 窗口创建四部曲之四：窗口的移动、显示与更新
	//填true表示会重画窗口
	MoveWindow(hwnd, 250, 80, WINDOW_WIDTH, WINDOW_HEIGHT, true);//调整窗口显示时的位置，使窗口左上角位于（250,80）处

	ShowWindow(hwnd, nShowCmd); //因为这个函数是在WinMain 函数内部调用的，直接取WinMain 函数的参数就可以了，调用ShowWindow函数来显示窗口

	UpdateWindow(hwnd); UpdateWindow(hwnd);//填的是创建成功后的窗口的句柄

										   //游戏资源的初始化，若初始化失败，弹出一个消息框，并返回FALSE

	if (!Game_Init(hwnd))
	{
		MessageBox(hwnd, L"资源初始化失败", L"消息窗口", 0); //使用MessageBox函数， 创建一个消息窗口
		return FALSE;
	}

	//【5】消息循环过程

	MSG msg = { 0 };   //定义并初始化msg

	while (msg.message != WM_QUIT)  //使用while循环，如果消息不是WM_QUIT消息，就继续循环
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))  //查看应用程序消息队列，有消息时将队列中的消息派发出去。
		{
			TranslateMessage(&msg);		//将虚拟键消息转换为字符消息
			DispatchMessage(&msg);   //分发一个消息给窗口程序。
		}

	}

	UnregisterClass(L"ForTheDreamOfGameDevelop", wndClass.hInstance);  //程序准备结束，注销窗口类

	return 0;

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;			//定义一个PAINTSTRUCT结构体来记录一些绘制信息

	switch (message)						//switch语句开始
	{
	case WM_PAINT:						// 若是客户区重绘消息
		g_hdc = BeginPaint(hwnd, &paintStruct);  //指定窗口进行绘图工作的准备，并用将和绘图有关的信息填充到paintStruct结构体中。
		Game_Paint(hwnd);
		EndPaint(hwnd, &paintStruct);			//EndPaint函数标记指定窗口的绘画过程结束
		ValidateRect(hwnd, NULL);		// 更新客户区的显示
		break;									//跳出该switch语句

	case WM_KEYDOWN:					// 若是键盘按下消息
		if (wParam == VK_ESCAPE)    // 如果被按下的键是ESC
			DestroyWindow(hwnd);		// 销毁窗口, 并发送一条WM_DESTROY消息
		break;									//跳出该switch语句

	case WM_DESTROY:					//若是窗口销毁消息
		Game_CleanUp(hwnd);			//调用自定义的资源清理函数Game_CleanUp（）进行退出前的资源清理
		PostQuitMessage(0);			//向系统表明有个线程有终止请求。用来响应WM_DESTROY消息
		break;									//跳出该switch语句

	default:										//若上述case条件都不符合，则执行该default语句
		return DefWindowProc(hwnd, message, wParam, lParam);		//调用缺省的窗口过程
	}

	return 0;									//正常退出
}

//-----------------------------------【Game_Init( )函数】--------------------------------------
//	描述：初始化函数，进行一些简单的初始化
//------------------------------------------------------------------------------------------------
BOOL Game_Init(HWND hwnd)
{
	g_hdc = GetDC(hwnd);
	Game_Paint(hwnd);
	return TRUE;
}

//-----------------------------------【Game_Paint( )函数】--------------------------------------
//	描述：绘制函数，在此函数中进行绘制操作
//--------------------------------------------------------------------------------------------------
VOID Game_Paint(HWND hwnd)
{

}

//-----------------------------------【Game_CleanUp( )函数】--------------------------------
//	描述：资源清理函数，在此函数中进行程序退出前资源的清理工作
//---------------------------------------------------------------------------------------------------
BOOL Game_CleanUp(HWND hwnd)
{
	ReleaseDC(hwnd, g_hdc);
	return TRUE;
}