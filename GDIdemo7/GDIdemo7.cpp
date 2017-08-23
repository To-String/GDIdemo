#include "stdafx.h"
#include "GDIdemo7.h"
#include<tchar.h> //ʹ��swprintf ϵ���ַ���ʽ������Ҫ���� tchar.h  ͷ�ļ�
#include<Windows.h>
#include<mmsystem.h>

#pragma comment(lib,"winmm.lib")

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"������������Ϩ�����Ϸ�������롿��Ϸ��������֮ ��Ϸѭ��������ʾ ʾ������"	//Ϊ���ڱ��ⶨ��ĺ�


//	������ȫ�ֱ���������
HDC	 g_hdc = nullptr, g_mdc = nullptr;     //ȫ���豸���������ȫ���ڴ�DC���
HBITMAP	 g_hSprite[12];						//����λͼ�������������������λͼ
DWORD	g_tPre = 0, g_tNow = 0;          //����l������������¼ʱ��,g_tPre��¼��һ�λ�ͼ��ʱ�䣬g_tNow��¼�˴�׼����ͼ��ʱ��
int	g_iNum = 0;					//g_iNum����������¼Ŀǰ��ʾ��ͼ��

					//	������ȫ�ֺ�����������ֹ��δ�����ı�ʶ��ϵ�д���
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
	wndClass.lpszClassName = L"ForTheDreamOfGameDevelop";		//��һ���Կ���ֹ���ַ�����ָ������������֡�
	
	if (!RegisterClassEx(&wndClass))
	{
		return -1;
	}

	//��3�����ڴ����Ĳ���֮������ʽ��������
	HWND hwnd = CreateWindow(L"ForTheDreamOfGameDevelop", WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT, nullptr, nullptr, hInstance, nullptr);

	MoveWindow(hwnd, 250, 80, WINDOW_WIDTH, WINDOW_HEIGHT, true);

	ShowWindow(hwnd, nShowCmd);

	UpdateWindow(hwnd);

	//��Ϸ��Դ�ĳ�ʼ��������ʼ��ʧ�ܣ�����һ����Ϣ�򣬲�����FALSE
	if (!Game_Init(hwnd))
	{
		MessageBox(hwnd, L"��Դ��ʼ��ʧ��", L"��Ϣ����", 0);
		return FALSE;
	}

	PlaySound(L"�ɽ�����¥.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_LOOP);

	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0,0, PM_REMOVE))
		{
			TranslateMessage(&msg);		//���������Ϣת��Ϊ�ַ���Ϣ
			DispatchMessage(&msg);			//�ַ�һ����Ϣ�����ڳ���
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
	g_hdc = GetDC(hwnd); //��ȡ�豸�������

	wchar_t filename[20];

	//�����������λͼ

	for (int i = 0; i < 12; ++i)
	{
		memset(filename, 0,sizeof(filename));
		swprintf_s(filename, L"%d.bmp", i);
		g_hSprite[i] = (HBITMAP)LoadImage(nullptr, filename, IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);
	}

	g_mdc = CreateCompatibleDC(g_hdc);   //���������豸�������ڴ�DC
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