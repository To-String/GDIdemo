#include "stdafx.h"
#include "GDIdemo9.h"
#include<Windows.h>
#include<mmsystem.h>
#include<tchar.h>
#include<ctime>
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"Msimg32.lib")

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"【致我们永不熄灭的游戏开发梦想】游戏动画显示技术之 排序贴图"	//为窗口标题定义的宏
#define SPRITE_NUMBER 30 //定义宏SPRITE_NUMBER，表示画面上要出现的人物数目，在此设定为30个

//描述：全局结构体定义
struct Sprites  //定义sprite结构，代表画面上的人物，其结构成员x和y为贴图坐标，direction为目前人物的移动方向
{
	int x, y; //x和y为贴图坐标
	int direction;   // direction为目前人物的移动方向
};



//描述：全局变量的声明

HDC g_hdc = nullptr, g_mdc = nullptr, g_bufdc = nullptr; //全局设备环境句柄，两个全局内存DC句柄

HBITMAP g_hSprite[4], g_hBackGround; //声明位图数组用来储存各张人物位图

DWORD g_tPre = 0, g_tNow = 0;//声明l两个函数来记录时间,g_tPre记录上一次绘图的时间，g_tNow记录此次准备绘图的时间

int g_iPicNum = 0, g_iX = 0, g_iY = 0; //g_iPicNum用来记录图号,g_iX,g_iY分别记录贴图的横纵坐标	

Sprites Sprite[SPRITE_NUMBER]; //按照SPRITE_NUMBER的值建立数组Sprite[]



//描述：全局函数声明，防止“未声明的标识”系列错误

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL Game_Init(HWND hwnd);//在此函数中进行资源的初始化
VOID Game_Paint(HWND hwnd);//在此函数中进行绘图代码的书写
BOOL Game_CleanUp(HWND hwnd);//在此函数中进行资源的清理

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSEX wndClass = { 0 };		//用WINDCLASSEX定义了一个窗口类
	wndClass.cbSize = sizeof(WNDCLASSEX);			//设置结构体的字节数大小
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	//设置窗口的样式
	wndClass.lpfnWndProc = WndProc;					//设置指向窗口过程函数的指针
	wndClass.cbClsExtra = 0;								//窗口类的附加内存，取0就可以了
	wndClass.cbWndExtra = 0;							//窗口的附加内存，依然取0就行了
	wndClass.hInstance = hInstance;						//指定包含窗口过程的程序的实例句柄。
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);  //本地加载自定义ico图标
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);    //指定窗口类的光标句柄。
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);  //为hbrBackground成员指定一个白色画刷句柄	
	wndClass.lpszMenuName = NULL;						//用一个以空终止的字符串，指定菜单资源的名字。
	wndClass.lpszClassName = L"ForTheDreamOfGameDevelop";		//用一个以空终止的字符串，指定窗口类的名字。
	
	
     //【2】窗口创建四步曲之二：注册窗口类
	if (!RegisterClassEx(&wndClass))
	{
		return -1;
	}



	//【3】窗口创建四步曲之三：正式创建窗口
	HWND hwnd = CreateWindow(L"ForTheDreamOfGameDevelop", WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,nullptr, nullptr, hInstance, nullptr);

	//【4】窗口创建四步曲之四：窗口的移动、显示与更新

	MoveWindow(hwnd, 250, 80, WINDOW_WIDTH, WINDOW_HEIGHT, true);

	ShowWindow(hwnd, nShowCmd);

	UpdateWindow(hwnd);

	//游戏资源的初始化，若初始化失败，弹出一个消息框，并返回FALSE
	if (!Game_Init(hwnd))
	{
		MessageBox(nullptr, L"资源初始化失败", L"消息窗口", 0);
		return FALSE;
	}
	PlaySound(L"OrcTheme.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_LOOP);


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
			g_tNow = GetTickCount();  //获取当前系统时间
			if (g_tNow - g_tPre>=100) //当此次循环运行与上次绘图时间相差0.1秒时再进行重绘操作
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
	case WM_KEYDOWN:					// 若是键盘按下消息
		if (wParam == VK_ESCAPE)    // 如果被按下的键是ESC
		{
			DestroyWindow(hwnd);  // 销毁窗口, 并发送一条WM_DESTROY消息
		}
		break;

	case WM_DESTROY:					//若是窗口销毁消息
		Game_CleanUp(hwnd);			//调用自定义的资源清理函数Game_CleanUp（）进行退出前的资源清理
		PostQuitMessage(0);			//向系统表明有个线程有终止请求。用来响应WM_DESTROY消息
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

VOID BubSort(int n)
{
	bool f = false;
	for (int i = 0; i < n - 1; ++i)
	{
		
		for (int j = 0; j < n - i - 1; ++j)
		{
			if (Sprite[j + 1].y < Sprite[j].y)
			{
				Sprites tmp = Sprite[j + 1];
				Sprite[j + 1] = Sprite[j];
				Sprite[j] = tmp;
				f = true;
			}
		}
		if (!f)
		{
			break;
		}
	}
}

BOOL Game_Init(HWND hwnd)
{
	srand((unsigned)time(nullptr));//用系统时间初始化随机种子

	g_hdc = GetDC(hwnd);
	g_mdc = CreateCompatibleDC(g_hdc);
	g_bufdc = CreateCompatibleDC(g_hdc);

	HBITMAP bmp= CreateCompatibleBitmap(g_hdc, WINDOW_WIDTH, WINDOW_HEIGHT); //建一个和窗口兼容的空的位图对象
	
 /*返回值：如果函数执行成功，那么返回值是位图的句柄；如果函数执行失败，那么返回值为NULL。*/
	/* 函数原型： HBITMAP CreateCompatibleBitmap(HDC hdc,int nWidth,int nHeight)；
	
	   hdc： 设备环境句柄。
      nWidth：指定位图的宽度，单位为像素。
       nHeight：指定位图的高度，单位为像素。
	   
	   指的是该函数创建与指定的设备环境相关的设备兼容的位图*/

	/*当你不再需要这个位图的时候，调用DeleteObject删除它。
	
	需要与CreateCompatibleDC配合使用*/

	SelectObject(g_mdc, bmp);

	//加载各张跑动图及背景图
	g_hBackGround = (HBITMAP)LoadImage(nullptr, L"bg.bmp", IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);
	g_hSprite[0] = (HBITMAP)LoadImage(nullptr, L"11.bmp", IMAGE_BITMAP, 384, 96, LR_LOADFROMFILE);
	g_hSprite[1] = (HBITMAP)LoadImage(nullptr, L"22.bmp", IMAGE_BITMAP, 384, 96, LR_LOADFROMFILE);
	g_hSprite[2] = (HBITMAP)LoadImage(nullptr, L"33.bmp", IMAGE_BITMAP, 384, 96, LR_LOADFROMFILE);
	g_hSprite[3] = (HBITMAP)LoadImage(nullptr, L"44.bmp", IMAGE_BITMAP, 384, 96, LR_LOADFROMFILE);

	//设定初始的贴图坐标都为窗口内的任意坐标，初始的移动方向都为向左。

	for (int i = 0; i < SPRITE_NUMBER; ++i)
	{
		Sprite[i].direction = 3;
		Sprite[i].x = rand() % WINDOW_WIDTH;
		Sprite[i].y = rand() % WINDOW_HEIGHT;
	}
	Game_Paint(hwnd);
	return TRUE;
}

VOID Game_Paint(HWND hwnd)
{
	if (g_iPicNum == 4)		//判断是否超过最大图号，若超过最大图号“3”，则将显示图号重设为"0"。
		g_iPicNum = 0;

	//在mdc中贴上背景图
	SelectObject(g_bufdc, g_hBackGround);
	BitBlt(g_mdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_bufdc, 0, 0, SRCCOPY);

	BubSort(SPRITE_NUMBER);    //贴上人物图之前调用BubSort（）函数进行排序

	for (int i = 0; i<SPRITE_NUMBER; i++)
	{
		SelectObject(g_bufdc, g_hSprite[Sprite[i].direction]);
		TransparentBlt(g_mdc, Sprite[i].x, Sprite[i].y, 96, 96, g_bufdc, g_iPicNum * 96, 0, 96, 96, RGB(0, 0, 0));//采用TransparentBlt透明色彩法
	}

	//将最后的画面显示在窗口中
	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);


	g_tPre = GetTickCount();     //记录此次绘图时间
	++g_iPicNum;


	//下面这个for循环，决定每一只精灵下一次的移动方向及贴图坐标
	for (int i = 0; i<SPRITE_NUMBER; i++)
	{
		switch (rand() % 4)          //随机数除以4的余数来决定下次移动方向，余数0,1,2,3分别代表上，下，左，右
		{
		case 0:					     //上
			Sprite[i].y -= 20;
			//在计算出新的贴图坐标之后，还需判断此新的坐标会不会使得人物贴图超出窗口边界，若超出，则将该方向上的坐标设定为刚好等于临界值
			if (Sprite[i].y < 0)
			{
				Sprite[i].y = 0;
			}
			Sprite[i].direction = 0;
			break;
			//其他方向按照和上面相同的方法计算
		case 1:		//下
			Sprite[i].y += 20;
			if (Sprite[i].y > WINDOW_HEIGHT - 100)
			{
				Sprite[i].y = WINDOW_HEIGHT - 100;
			}
			Sprite[i].direction = 1;
			break;
		case 2:				    	//左
			Sprite[i].x -= 20;
			if (Sprite[i].x < 0)
			{
				Sprite[i].x = 0;
			}
			Sprite[i].direction = 2;
			break;
		case 3:				    	//右
			Sprite[i].x += 20;

			if (Sprite[i].x > WINDOW_WIDTH - 100)
			{
				Sprite[i].x = WINDOW_WIDTH - 100;
			}
			Sprite[i].direction = 3;
			break;
		}
	}
}
BOOL Game_CleanUp(HWND hwnd)
{
	//释放资源对象
	DeleteObject(g_hBackGround);
	for (int i = 0; i < 4; ++i)
	{
		DeleteObject(g_hSprite[i]);
	}
	DeleteDC(g_bufdc);
	DeleteDC(g_mdc);
	ReleaseDC(hwnd, g_hdc);
	return TRUE;
}