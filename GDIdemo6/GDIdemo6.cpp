#include "stdafx.h"
#include "GDIdemo6.h"
#include<tchar.h>
#include<Windows.h>
#include<mmsystem.h>

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"Msimg32.lib") //���ʹ��TransparentBlt��������Ŀ��ļ�

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"������������Ϩ�����Ϸ�������롿��Ϸ��������֮ ��ʱ��������ʾ ʾ������"

HDC g_hdc = nullptr, g_mdc = nullptr;   //ȫ���豸���������ȫ���ڴ�DC���
HBITMAP g_hSprite[12];   //����λͼ�������������������λͼ
int g_iNum = 0;  //"g_iNum"����������¼Ŀǰ��ʾ��ͼ��

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL		Game_Init(HWND hwnd);			//�ڴ˺����н�����Դ�ĳ�ʼ��
VOID		Game_Paint(HWND hwnd);		//�ڴ˺����н��л�ͼ�������д
BOOL		Game_CleanUp(HWND hwnd);	//�ڴ˺����н�����Դ������

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
	wndClass.lpszClassName = L"ForTheDreamOfGameDevelop";		//��һ���Կ���ֹ���ַ�����ָ������������֡�

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
		MessageBox(hwnd, L"��Դ��ʼ��ʧ��", L"��Ϣ����",0);
		return -1;
	}
	PlaySound(L"��֮ʫ.wav", nullptr, SND_FILENAME | SND_ASYNC | SND_LOOP); //ѭ�����ű������� 

	MSG msg = { 0 };
	while ( msg.message != WM_QUIT )
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);		//���������Ϣת��Ϊ�ַ���Ϣ
			DispatchMessage(&msg);			//�ַ�һ����Ϣ�����ڳ���
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
		Game_Paint(hwnd);  //���� Game_Paint()�������д��ڻ�ͼ
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

	g_mdc = CreateCompatibleDC(g_hdc); //���������豸�������ڴ�DC
	g_iNum = 0;              //���ó�ʼ����ʾͼ��Ϊ"0"
	SetTimer(hwnd, 1, 90, nullptr);   //������ʱ�������0.09�뷢����Ϣ
	Game_Paint(hwnd);

	return TRUE;
}

VOID Game_Paint(HWND hwnd)
{
	if (g_iNum == 11)
	{
		g_iNum = 0;
	}
	SelectObject(g_mdc, g_hSprite[g_iNum]);//����ͼ��ѡ���Ӧ��λͼ
	BitBlt(g_hdc,0,0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);         //��Ŀǰͼ�Ž��д�����ͼ)

																				  //ͼ������
	g_iNum++;                    //����g_iNum��ֵ��1��Ϊ��һ��Ҫ��ʾ��ͼ��
}

BOOL Game_CleanUp(HWND hwnd)
{
	KillTimer(hwnd, 1);   //ɾ���������Ķ�ʱ��  
						  //�ͷ���Դ����
	for (int i = 0; i < 12; ++i)
	{
		DeleteObject(g_hSprite[i]);
	}
	DeleteDC(g_mdc);
	ReleaseDC(hwnd, g_hdc);  //�ͷ��豸����
	return TRUE;
}