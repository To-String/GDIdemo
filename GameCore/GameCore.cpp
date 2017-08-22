
//  用代码勾勒出游戏开发所需的程序框架

#include "stdafx.h"
#include "GameCore.h"
#include<Windows.h>  //包含的是Windows函数（API函数）
#include<iostream>
using namespace std;
#define WINDOW_WIDTH 800 //为窗口宽度定义的宏， 以方便以后在此处修改窗口的宽度
#define WINDOW_HEIGHT 600  //为窗口高度定义的宏， 以方便以后在此处修改窗口的高度
#define WINDOW_TITLE L"【致我们永不熄灭的游戏开发梦想】程序核心框架"  // 为窗口标题定义的宏

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam); //窗口过程函数的声明


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//【1】 窗口创建四部曲之一，开始设计一个完整的窗口类

	WNDCLASSEX wndClass = { 0 }; //用WNDCLASSEX 定义了一个窗口类
	wndClass.cbSize = sizeof(WNDCLASSEX);  //设置结构体的字节数大小
	wndClass.style = CS_HREDRAW | CS_VREDRAW;  //设置窗口的样式
	wndClass.lpfnWndProc = WndProc; //设置指向窗口过程函数的函数指针，与窗口过程函数的类型必须与WNDPROC相同，即"LRESULT"
	wndClass.cbClsExtra = 0; //窗口类的附加内存，取0就可以了
	wndClass.cbWndExtra = 0;//窗口类的附加内存，依然取0就可以了
	wndClass.hInstance = hInstance;//指定包含窗口过程的程序的实例句柄，填写WinMain函数的第一个参数，也就是该程序当前运行的实例句柄传递给它
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);  //用于指定窗口类的图标句柄，这个成员变量必须是个图标资源的句柄
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //指定窗口类的光标句柄,一般这个参数设置为默认的箭头光标
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);//为hbrBackground成员指定一个灰色画刷句柄
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName= L"ForTheDreamOfGameDevelop"; //用一个以空终止的字符串，指定窗口类的名字，名字是自定义的，后面会用到，用来标识我们正在定义的这个窗口类
	

	//【2】 窗口创建四部曲之二，  注册窗口类
	if (!RegisterClassEx(&wndClass)) //设计窗口类之后，需要对窗口类进行注册，这样才能创建该类型的窗口，填的就是我们窗口类的对象（实例）
	{
		return -1;
	}

	//【3】  窗口创建四部之三，正式创建窗口
	
	HWND hwnd = CreateWindow
	(L"ForTheDreamOfGameDevelop", WINDOW_TITLE,  //喜闻乐见的创建窗口函数CreateWindow
		WS_OVERLAPPEDWINDOW,CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
		NULL,NULL,hInstance,NULL
	);

	//【4】 窗口创建四部曲之四：窗口的移动、显示与更新

	MoveWindow(hwnd, 250, 80, WINDOW_WIDTH, WINDOW_HEIGHT, true);//调整窗口显示时的位置，使窗口左上角位于（250,80）处
	ShowWindow(hwnd, nShowCmd); //因为这个函数是在WinMain 函数内部调用的，直接取WinMain 函数的参数就可以了，调用ShowWindow函数来显示窗口
	UpdateWindow(hwnd);//填的是创建成功后的窗口的句柄

	//【5】 消息循环过程/

	MSG msg = { 0 };   //定义并初始化msg
	while (msg.message != WM_QUIT)  //使用while循环，如果消息不是WM_QUIT消息，就继续循环
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))  //查看应用程序消息队列，有消息时将队列中的消息派发出去。
		{
			TranslateMessage(&msg);		//将虚拟键消息转换为字符消息
			DispatchMessage(&msg);   //分发一个消息给窗口程序。
		}

		
	}

	//【6】  窗口类的注销

	UnregisterClass(L"ForTheDreamOfGameDevelop", wndClass.hInstance);

	
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)  //窗口过程函数的定义
{
	switch (message) //switch语句开始
	{
	case WM_PAINT:    // 若是客户区重绘消息
		ValidateRect(hwnd, NULL);		// 更新客户区的显示
		break;		//跳出该switch语句	

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			DestroyWindow(hwnd);
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}
