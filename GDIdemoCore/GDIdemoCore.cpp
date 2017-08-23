#include<iostream>
#include "stdafx.h"
#include "GDIdemoCore.h"
#include<Windows.h>
using namespace std;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE L"������������Ϩ�����Ϸ�������롿GDI������Ŀ��" // Ϊ���ڱ��ⶨ��ĺ�

//ȫ�ֱ�������
HDC g_hdc = NULL;  //ȫ�ֻ������

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);  //���ڹ��̺���������

BOOL Game_Init(HWND hwnd); //�ڴ˺����н�����Դ�ĳ�ʼ��
VOID Game_Paint(HWND hwnd); //�ڴ˺����н��л�ͼ�������д
BOOL Game_CleanUp(HWND hwnd); //�ڴ˺����н�����Դ������


//-----------------------------------��WinMain( )������--------------------------------------

//	������WindowsӦ�ó������ں��������ǵĳ�������￪ʼ

//------------------------------------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//��1�����ڴ����Ĳ���֮һ����ʼ���һ�������Ĵ�����

	WNDCLASSEX wndClass = { 0 };  //��WNDCLASSEX ������һ��������
	wndClass.cbSize = sizeof(WNDCLASSEX);   //���ýṹ����ֽ�����С
	wndClass.style = CS_HREDRAW | CS_VREDRAW;  //���ô��ڵ���ʽ
	wndClass.lpfnWndProc = WndProc;//����ָ�򴰿ڹ��̺�����ָ��
	wndClass.cbClsExtra = 0; //������ĸ����ڴ棬ȡ0�Ϳ�����
	wndClass.cbWndExtra = 0;//������ĸ����ڴ棬��Ȼȡ0�Ϳ�����
	wndClass.hInstance = hInstance;//ָ���������ڹ��̵ĳ����ʵ���������дWinMain�����ĵ�һ��������Ҳ���Ǹó���ǰ���е�ʵ��������ݸ���
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);  //����ָ���������ͼ�����������Ա���������Ǹ�ͼ����Դ�ľ��
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //ָ��������Ĺ����,һ�������������ΪĬ�ϵļ�ͷ���
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);//ΪhbrBackground��Աָ��һ����ɫ��ˢ���
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"ForTheDreamOfGameDevelop"; //��һ���Կ���ֹ���ַ�����ָ������������֣��������Զ���ģ�������õ���������ʶ�������ڶ�������������


														  //��2�� ���ڴ����Ĳ���֮����  ע�ᴰ����
	if (!RegisterClassEx(&wndClass)) //��ƴ�����֮����Ҫ�Դ��������ע�ᣬ�������ܴ��������͵Ĵ��ڣ���ľ������Ǵ�����Ķ���ʵ����
	{
		return -1;
	}

	//��3��  ���ڴ����Ĳ�֮������ʽ��������

	//hwnd ���Ǵ����ô��ڵľ��
	HWND hwnd = CreateWindow
	(L"ForTheDreamOfGameDevelop", WINDOW_TITLE,  //ϲ���ּ��Ĵ������ں���CreateWindow
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
		NULL, NULL, hInstance, NULL
	);

	//��4�� ���ڴ����Ĳ���֮�ģ����ڵ��ƶ�����ʾ�����
	//��true��ʾ���ػ�����
	MoveWindow(hwnd, 250, 80, WINDOW_WIDTH, WINDOW_HEIGHT, true);//����������ʾʱ��λ�ã�ʹ�������Ͻ�λ�ڣ�250,80����

	ShowWindow(hwnd, nShowCmd); //��Ϊ�����������WinMain �����ڲ����õģ�ֱ��ȡWinMain �����Ĳ����Ϳ����ˣ�����ShowWindow��������ʾ����

	UpdateWindow(hwnd); UpdateWindow(hwnd);//����Ǵ����ɹ���Ĵ��ڵľ��

										   //��Ϸ��Դ�ĳ�ʼ��������ʼ��ʧ�ܣ�����һ����Ϣ�򣬲�����FALSE

	if (!Game_Init(hwnd))
	{
		MessageBox(hwnd, L"��Դ��ʼ��ʧ��", L"��Ϣ����", 0); //ʹ��MessageBox������ ����һ����Ϣ����
		return FALSE;
	}

	//��5����Ϣѭ������

	MSG msg = { 0 };   //���岢��ʼ��msg

	while (msg.message != WM_QUIT)  //ʹ��whileѭ���������Ϣ����WM_QUIT��Ϣ���ͼ���ѭ��
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))  //�鿴Ӧ�ó�����Ϣ���У�����Ϣʱ�������е���Ϣ�ɷ���ȥ��
		{
			TranslateMessage(&msg);		//���������Ϣת��Ϊ�ַ���Ϣ
			DispatchMessage(&msg);   //�ַ�һ����Ϣ�����ڳ���
		}

	}

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

//-----------------------------------��Game_Init( )������--------------------------------------
//	��������ʼ������������һЩ�򵥵ĳ�ʼ��
//------------------------------------------------------------------------------------------------
BOOL Game_Init(HWND hwnd)
{
	g_hdc = GetDC(hwnd);
	Game_Paint(hwnd);
	return TRUE;
}

//-----------------------------------��Game_Paint( )������--------------------------------------
//	���������ƺ������ڴ˺����н��л��Ʋ���
//--------------------------------------------------------------------------------------------------
VOID Game_Paint(HWND hwnd)
{

}

//-----------------------------------��Game_CleanUp( )������--------------------------------
//	��������Դ���������ڴ˺����н��г����˳�ǰ��Դ��������
//---------------------------------------------------------------------------------------------------
BOOL Game_CleanUp(HWND hwnd)
{
	ReleaseDC(hwnd, g_hdc);
	return TRUE;
}