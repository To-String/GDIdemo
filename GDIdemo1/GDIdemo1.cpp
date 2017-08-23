#include "stdafx.h"
#include "GDIdemo1.h"
#include<Windows.h>
#include <mmsystem.h>
#include<ctime>  //ʹ�û�ȡϵͳʱ��time������Ҫ������ͷ�ļ�

#pragma comment(lib,"winmm.lib")  //����PlaySound����������ļ�

#define WINDOW_WIDTH	800							//Ϊ���ڿ�ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڿ��
#define WINDOW_HEIGHT	600							//Ϊ���ڸ߶ȶ���ĺ꣬�Է����ڴ˴��޸Ĵ��ڸ߶�
#define WINDOW_TITLE	L"������������Ϩ�����Ϸ�������롿GDI�������λ�ͼ"	//Ϊ���ڱ��ⶨ��ĺ�

HDC g_hdc = NULL;  //ȫ���豸�������
HPEN g_hPen[7] = { 0 };
HBRUSH g_hBrush[7] = { 0 };
int g_iPenStyle[7] = { PS_SOLID,PS_DASH,PS_DOT,PS_DASHDOT,PS_DASHDOTDOT,PS_NULL,PS_INSIDEFRAME };
int g_iBrushStyle[6]= { HS_VERTICAL,HS_HORIZONTAL,HS_CROSS,HS_DIAGCROSS,HS_FDIAGONAL,HS_BDIAGONAL };  //���廭ˢ��ʽ���鲢��ʼ��


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);  //���ڹ��̺���������

BOOL Game_Init(HWND hwnd); //�ڴ˺����н�����Դ�ĳ�ʼ��
VOID Game_Paint(HWND hwnd); //�ڴ˺����н��л�ͼ�������д
BOOL Game_CleanUp(HWND hwnd); //�ڴ˺����н�����Դ������

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
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//ΪhbrBackground��Աָ��һ����ɫ��ˢ���
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

	PlaySound(L"AIR - ��Ӱ.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

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

	UnregisterClass(L"ForTheDreamOfGameDevelop", wndClass.hInstance); //����׼��������ע��������

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
	return 0;
}

BOOL Game_Init(HWND hwnd)
{
	g_hdc = GetDC(hwnd);
	srand((unsigned)time(NULL));

	for (int i = 0; i <= 6; ++i)
	{
		g_hPen[i] = CreatePen(g_iPenStyle[i], 1, RGB(rand() % 256, rand() % 256, rand() % 256));
		if (i == 6)
		{
			g_hBrush[i] = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
		}
		else
		{
			g_hBrush[i] = CreateHatchBrush(g_iBrushStyle[i], RGB(rand() % 256, rand() % 256, rand() % 256));
		}
	}
	Game_Paint(hwnd);
	ReleaseDC(hwnd, g_hdc);
	return TRUE;
}

//-----------------------------------��Game_Paint( )������--------------------------------------
//	���������ƺ������ڴ˺����н��л��Ʋ���
//--------------------------------------------------------------------------------------------------
VOID Game_Paint(HWND hwnd)
{
	int y = 0;  //����һ��Y����ֵ


	for (int i = 0; i <= 6; ++i)
	{
		y = (i + 1) * 70;
		SelectObject(g_hdc, g_hPen[i]); //����Ӧ�Ļ���ѡ��
		MoveToEx(g_hdc, 30, y, NULL);
		LineTo(g_hdc, 100, y);
	}

	int x1 = 120;
	int x2 = 190;

	for (int i = 0; i <= 6; ++i)
	{
		SelectObject(g_hdc, g_hBrush[i]);
		Rectangle(g_hdc, x1, 70, x2, y);

		x1 += 90;
		x2 += 90;
	}
}

//-----------------------------------��Game_CleanUp( )������--------------------------------
//	��������Դ���������ڴ˺����н��г����˳�ǰ��Դ��������
//---------------------------------------------------------------------------------------------------
BOOL Game_CleanUp(HWND hwnd)
{
	for (int i = 0; i <= 6; ++i)
	{
		DeleteObject(g_hPen[i]);
		DeleteObject(g_hBrush[i]);
	}
	return TRUE;
}