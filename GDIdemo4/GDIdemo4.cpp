#include "stdafx.h"
#include "GDIdemo4.h"
#include<Windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

#define WINDOW_WIDTH 932
#define WINDOW_HEIGHT 700
#define WINDOW_TITLE L"������������Ϩ�����Ϸ�������롿͸����ͼ������ϵ֮��͸�����ַ�"	//Ϊ���ڱ��ⶨ��ĺ�

HDC	 g_hdc = NULL, g_mdc = NULL;       //ȫ���豸���������ȫ���ڴ�DC���
HBITMAP g_hBackGround, g_hCharacter1, g_hCharacter2;  //����3��λͼ���������3��ͼƬ�Ĵ��

													
LRESULT CALLBACK	WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);//���ڹ��̺���
BOOL		Game_Init(HWND hwnd);			//�ڴ˺����н�����Դ�ĳ�ʼ��
VOID		Game_Paint(HWND hwnd);		//�ڴ˺����н��л�ͼ�������д
BOOL		Game_CleanUp(HWND hwnd);	//�ڴ˺����н�����Դ������

													
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//��1�����ڴ����Ĳ���֮һ����ʼ���һ�������Ĵ�����
	WNDCLASSEX wndClass = { 0 };							//��WINDCLASSEX������һ��������
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
	if (!RegisterClassEx(&wndClass))				//����괰�ں���Ҫ�Դ��������ע�ᣬ�������ܴ��������͵Ĵ���
		return -1;

	//��3�����ڴ����Ĳ���֮������ʽ��������
	HWND hwnd = CreateWindow(L"ForTheDreamOfGameDevelop", WINDOW_TITLE,		//ϲ���ּ��Ĵ������ں���CreateWindow
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,
		WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

	//��4�����ڴ����Ĳ���֮�ģ����ڵ��ƶ�����ʾ�����
	MoveWindow(hwnd, 200, 20, WINDOW_WIDTH, WINDOW_HEIGHT, true);		//����������ʾʱ��λ�ã�ʹ�������Ͻ�λ�ڣ�200,20����
	ShowWindow(hwnd, nShowCmd);    //����ShowWindow��������ʾ����
	UpdateWindow(hwnd);						//�Դ��ڽ��и��£��������������·���Ҫװ��һ��

											//��Ϸ��Դ�ĳ�ʼ��������ʼ��ʧ�ܣ�����һ����Ϣ�򣬲�����FALSE
	if (!Game_Init(hwnd))
	{
		MessageBox(hwnd, L"��Դ��ʼ��ʧ��", L"��Ϣ����", 0); //ʹ��MessageBox����������һ����Ϣ����
		return FALSE;
	}
	PlaySound(L"��ƿʱ��1.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP); //ѭ�����ű������� 

																		//��5����Ϣѭ������
	MSG msg = { 0 };				//���岢��ʼ��msg
	while (msg.message != WM_QUIT)		//ʹ��whileѭ���������Ϣ����WM_QUIT��Ϣ���ͼ���ѭ��
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))   //�鿴Ӧ�ó�����Ϣ���У�����Ϣʱ�������е���Ϣ�ɷ���ȥ��
		{
			TranslateMessage(&msg);		//���������Ϣת��Ϊ�ַ���Ϣ
			DispatchMessage(&msg);			//�ַ�һ����Ϣ�����ڳ���
		}
	}

	//��6���������ע��
	UnregisterClass(L"ForTheDreamOfGameDevelop", wndClass.hInstance);  //����׼��������ע��������
	return 0;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT paintStruct;			//����һ��PAINTSTRUCT�ṹ������¼һЩ������Ϣ

	switch (message)						//switch��俪ʼ
	{
	case WM_PAINT:						// ���ǿͻ����ػ���Ϣ
		g_hdc = BeginPaint(hwnd, &paintStruct);  //ָ�����ڽ��л�ͼ������׼�������ý��ͻ�ͼ�йص���Ϣ��䵽paintStruct�ṹ���С�
		Game_Paint(hwnd);
		EndPaint(hwnd, &paintStruct);			//EndPaint�������ָ�����ڵĻ滭���̽���
		ValidateRect(hwnd, NULL);		// ���¿ͻ�������ʾ
		break;									//������switch���

	case WM_KEYDOWN:					// ���Ǽ��̰�����Ϣ
		if (wParam == VK_ESCAPE)    // ��������µļ���ESC
			DestroyWindow(hwnd);		// ���ٴ���, ������һ��WM_DESTROY��Ϣ
		break;									//������switch���

	case WM_DESTROY:					//���Ǵ���������Ϣ
		Game_CleanUp(hwnd);			//�����Զ������Դ������Game_CleanUp���������˳�ǰ����Դ����
		PostQuitMessage(0);			//��ϵͳ�����и��߳�����ֹ����������ӦWM_DESTROY��Ϣ
		break;									//������switch���

	default:										//������case�����������ϣ���ִ�и�default���
		return DefWindowProc(hwnd, message, wParam, lParam);		//����ȱʡ�Ĵ��ڹ���
	}

	return 0;									//�����˳�
}


BOOL Game_Init(HWND hwnd)
{
	g_hdc = GetDC(hwnd);  //��ȡ�豸�������

						  //-----��λͼ�����Ĳ���֮һ������λͼ��-----
						  //���ļ�����3��λͼ
	g_hBackGround = (HBITMAP)LoadImage(NULL, L"bg.bmp", IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);
	g_hCharacter1 = (HBITMAP)LoadImage(NULL, L"character1.bmp", IMAGE_BITMAP, 640, 579, LR_LOADFROMFILE);
	g_hCharacter2 = (HBITMAP)LoadImage(NULL, L"character2.bmp", IMAGE_BITMAP, 800, 584, LR_LOADFROMFILE);

	//-----��λͼ�����Ĳ���֮������������DC��-----
	g_mdc = CreateCompatibleDC(g_hdc);    //���������豸�������ڴ�DC

	Game_Paint(hwnd);
	ReleaseDC(hwnd, g_hdc);  //�ͷ��豸����
	return TRUE;
}


VOID Game_Paint(HWND hwnd)
{
	//�����ϱ���ͼ
	SelectObject(g_mdc, g_hBackGround);
	BitBlt(g_hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, g_mdc, 0, 0, SRCCOPY);    //����BitBlt������g_hdc�������ϱ���ͼ

																			   //��͸�����ַ����Ƴ���һ������
	SelectObject(g_mdc, g_hCharacter1);
	BitBlt(g_hdc, 50, WINDOW_HEIGHT - 579, 320, 640, g_mdc, 320, 0, SRCAND);//͸�����ַ���һ������������ͼ�뱳��ͼ��"AND"���� 
	BitBlt(g_hdc, 50, WINDOW_HEIGHT - 579, 320, 640, g_mdc, 0, 0, SRCPAINT);//͸�����ַ��ڶ���������ǰ��ͼ�뱳��ͼ��"OR"����

																			//��͸�����ַ����Ƴ��ڶ�������
	SelectObject(g_mdc, g_hCharacter2);
	BitBlt(g_hdc, 450, WINDOW_HEIGHT - 584, 400, 584, g_mdc, 400, 0, SRCAND);//͸�����ַ���һ������������ͼ�뱳��ͼ��"AND"����
	BitBlt(g_hdc, 450, WINDOW_HEIGHT - 584, 400, 584, g_mdc, 0, 0, SRCPAINT);//͸�����ַ��ڶ���������ǰ��ͼ�뱳��ͼ��"OR"����
}

BOOL Game_CleanUp(HWND hwnd)
{
	//�ͷ���Դ����
	DeleteObject(g_hBackGround);
	DeleteObject(g_hCharacter2);
	DeleteObject(g_hCharacter1);
	DeleteDC(g_mdc);
	return TRUE;
}