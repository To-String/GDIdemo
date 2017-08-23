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
#define WINDOW_TITLE L"������������Ϩ�����Ϸ�������롿��Ϸ������ʾ����֮ ������ͼ"	//Ϊ���ڱ��ⶨ��ĺ�
#define SPRITE_NUMBER 30 //�����SPRITE_NUMBER����ʾ������Ҫ���ֵ�������Ŀ���ڴ��趨Ϊ30��

//������ȫ�ֽṹ�嶨��
struct Sprites  //����sprite�ṹ���������ϵ������ṹ��Աx��yΪ��ͼ���꣬directionΪĿǰ������ƶ�����
{
	int x, y; //x��yΪ��ͼ����
	int direction;   // directionΪĿǰ������ƶ�����
};



//������ȫ�ֱ���������

HDC g_hdc = nullptr, g_mdc = nullptr, g_bufdc = nullptr; //ȫ���豸�������������ȫ���ڴ�DC���

HBITMAP g_hSprite[4], g_hBackGround; //����λͼ�������������������λͼ

DWORD g_tPre = 0, g_tNow = 0;//����l������������¼ʱ��,g_tPre��¼��һ�λ�ͼ��ʱ�䣬g_tNow��¼�˴�׼����ͼ��ʱ��

int g_iPicNum = 0, g_iX = 0, g_iY = 0; //g_iPicNum������¼ͼ��,g_iX,g_iY�ֱ��¼��ͼ�ĺ�������	

Sprites Sprite[SPRITE_NUMBER]; //����SPRITE_NUMBER��ֵ��������Sprite[]



//������ȫ�ֺ�����������ֹ��δ�����ı�ʶ��ϵ�д���

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL Game_Init(HWND hwnd);//�ڴ˺����н�����Դ�ĳ�ʼ��
VOID Game_Paint(HWND hwnd);//�ڴ˺����н��л�ͼ�������д
BOOL Game_CleanUp(HWND hwnd);//�ڴ˺����н�����Դ������

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSEX wndClass = { 0 };		//��WINDCLASSEX������һ��������
	wndClass.cbSize = sizeof(WNDCLASSEX);			//���ýṹ����ֽ�����С
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	//���ô��ڵ���ʽ
	wndClass.lpfnWndProc = WndProc;					//����ָ�򴰿ڹ��̺�����ָ��
	wndClass.cbClsExtra = 0;								//������ĸ����ڴ棬ȡ0�Ϳ�����
	wndClass.cbWndExtra = 0;							//���ڵĸ����ڴ棬��Ȼȡ0������
	wndClass.hInstance = hInstance;						//ָ���������ڹ��̵ĳ����ʵ�������
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);  //���ؼ����Զ���icoͼ��
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);    //ָ��������Ĺ������
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);  //ΪhbrBackground��Աָ��һ����ɫ��ˢ���	
	wndClass.lpszMenuName = NULL;						//��һ���Կ���ֹ���ַ�����ָ���˵���Դ�����֡�
	wndClass.lpszClassName = L"ForTheDreamOfGameDevelop";		//��һ���Կ���ֹ���ַ�����ָ������������֡�
	
	
     //��2�����ڴ����Ĳ���֮����ע�ᴰ����
	if (!RegisterClassEx(&wndClass))
	{
		return -1;
	}



	//��3�����ڴ����Ĳ���֮������ʽ��������
	HWND hwnd = CreateWindow(L"ForTheDreamOfGameDevelop", WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,nullptr, nullptr, hInstance, nullptr);

	//��4�����ڴ����Ĳ���֮�ģ����ڵ��ƶ�����ʾ�����

	MoveWindow(hwnd, 250, 80, WINDOW_WIDTH, WINDOW_HEIGHT, true);

	ShowWindow(hwnd, nShowCmd);

	UpdateWindow(hwnd);

	//��Ϸ��Դ�ĳ�ʼ��������ʼ��ʧ�ܣ�����һ����Ϣ�򣬲�����FALSE
	if (!Game_Init(hwnd))
	{
		MessageBox(nullptr, L"��Դ��ʼ��ʧ��", L"��Ϣ����", 0);
		return FALSE;
	}
	PlaySound(L"OrcTheme.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_LOOP);


	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);		//���������Ϣת��Ϊ�ַ���Ϣ
			DispatchMessage(&msg);			//�ַ�һ����Ϣ�����ڳ���
		}
		else
		{
			g_tNow = GetTickCount();  //��ȡ��ǰϵͳʱ��
			if (g_tNow - g_tPre>=100) //���˴�ѭ���������ϴλ�ͼʱ�����0.1��ʱ�ٽ����ػ����
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
	case WM_KEYDOWN:					// ���Ǽ��̰�����Ϣ
		if (wParam == VK_ESCAPE)    // ��������µļ���ESC
		{
			DestroyWindow(hwnd);  // ���ٴ���, ������һ��WM_DESTROY��Ϣ
		}
		break;

	case WM_DESTROY:					//���Ǵ���������Ϣ
		Game_CleanUp(hwnd);			//�����Զ������Դ������Game_CleanUp���������˳�ǰ����Դ����
		PostQuitMessage(0);			//��ϵͳ�����и��߳�����ֹ����������ӦWM_DESTROY��Ϣ
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
	srand((unsigned)time(nullptr));//��ϵͳʱ���ʼ���������

	g_hdc = GetDC(hwnd);
	g_mdc = CreateCompatibleDC(g_hdc);
	g_bufdc = CreateCompatibleDC(g_hdc);

	HBITMAP bmp= CreateCompatibleBitmap(g_hdc, WINDOW_WIDTH, WINDOW_HEIGHT); //��һ���ʹ��ڼ��ݵĿյ�λͼ����
	
 /*����ֵ���������ִ�гɹ�����ô����ֵ��λͼ�ľ�����������ִ��ʧ�ܣ���ô����ֵΪNULL��*/
	/* ����ԭ�ͣ� HBITMAP CreateCompatibleBitmap(HDC hdc,int nWidth,int nHeight)��
	
	   hdc�� �豸���������
      nWidth��ָ��λͼ�Ŀ�ȣ���λΪ���ء�
       nHeight��ָ��λͼ�ĸ߶ȣ���λΪ���ء�
	   
	   ָ���Ǹú���������ָ�����豸������ص��豸���ݵ�λͼ*/

	/*���㲻����Ҫ���λͼ��ʱ�򣬵���DeleteObjectɾ������
	
	��Ҫ��CreateCompatibleDC���ʹ��*/

	SelectObject(g_mdc, bmp);

	//���ظ����ܶ�ͼ������ͼ
	g_hBackGround = (HBITMAP)LoadImage(nullptr, L"bg.bmp", IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);
	g_hSprite[0] = (HBITMAP)LoadImage(nullptr, L"11.bmp", IMAGE_BITMAP, 384, 96, LR_LOADFROMFILE);
	g_hSprite[1] = (HBITMAP)LoadImage(nullptr, L"22.bmp", IMAGE_BITMAP, 384, 96, LR_LOADFROMFILE);
	g_hSprite[2] = (HBITMAP)LoadImage(nullptr, L"33.bmp", IMAGE_BITMAP, 384, 96, LR_LOADFROMFILE);
	g_hSprite[3] = (HBITMAP)LoadImage(nullptr, L"44.bmp", IMAGE_BITMAP, 384, 96, LR_LOADFROMFILE);

	//�趨��ʼ����ͼ���궼Ϊ�����ڵ��������꣬��ʼ���ƶ�����Ϊ����

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
	if (g_iPicNum == 4)		//�ж��Ƿ񳬹����ͼ�ţ����������ͼ�š�3��������ʾͼ������Ϊ"0"��
		g_iPicNum = 0;

	//��mdc�����ϱ���ͼ
	SelectObject(g_bufdc, g_hBackGround);
	BitBlt(g_mdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_bufdc, 0, 0, SRCCOPY);

	BubSort(SPRITE_NUMBER);    //��������ͼ֮ǰ����BubSort����������������

	for (int i = 0; i<SPRITE_NUMBER; i++)
	{
		SelectObject(g_bufdc, g_hSprite[Sprite[i].direction]);
		TransparentBlt(g_mdc, Sprite[i].x, Sprite[i].y, 96, 96, g_bufdc, g_iPicNum * 96, 0, 96, 96, RGB(0, 0, 0));//����TransparentBlt͸��ɫ�ʷ�
	}

	//�����Ļ�����ʾ�ڴ�����
	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);


	g_tPre = GetTickCount();     //��¼�˴λ�ͼʱ��
	++g_iPicNum;


	//�������forѭ��������ÿһֻ������һ�ε��ƶ�������ͼ����
	for (int i = 0; i<SPRITE_NUMBER; i++)
	{
		switch (rand() % 4)          //���������4�������������´��ƶ���������0,1,2,3�ֱ�����ϣ��£�����
		{
		case 0:					     //��
			Sprite[i].y -= 20;
			//�ڼ�����µ���ͼ����֮�󣬻����жϴ��µ�����᲻��ʹ��������ͼ�������ڱ߽磬���������򽫸÷����ϵ������趨Ϊ�պõ����ٽ�ֵ
			if (Sprite[i].y < 0)
			{
				Sprite[i].y = 0;
			}
			Sprite[i].direction = 0;
			break;
			//���������պ�������ͬ�ķ�������
		case 1:		//��
			Sprite[i].y += 20;
			if (Sprite[i].y > WINDOW_HEIGHT - 100)
			{
				Sprite[i].y = WINDOW_HEIGHT - 100;
			}
			Sprite[i].direction = 1;
			break;
		case 2:				    	//��
			Sprite[i].x -= 20;
			if (Sprite[i].x < 0)
			{
				Sprite[i].x = 0;
			}
			Sprite[i].direction = 2;
			break;
		case 3:				    	//��
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
	//�ͷ���Դ����
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