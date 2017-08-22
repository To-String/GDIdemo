
//  �ô��빴�ճ���Ϸ��������ĳ�����

#include "stdafx.h"
#include "GameCore.h"
#include<Windows.h>  //��������Windows������API������
#include<iostream>
using namespace std;
#define WINDOW_WIDTH 800 //Ϊ���ڿ�ȶ���ĺ꣬ �Է����Ժ��ڴ˴��޸Ĵ��ڵĿ��
#define WINDOW_HEIGHT 600  //Ϊ���ڸ߶ȶ���ĺ꣬ �Է����Ժ��ڴ˴��޸Ĵ��ڵĸ߶�
#define WINDOW_TITLE L"������������Ϩ�����Ϸ�������롿������Ŀ��"  // Ϊ���ڱ��ⶨ��ĺ�

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam); //���ڹ��̺���������


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//��1�� ���ڴ����Ĳ���֮һ����ʼ���һ�������Ĵ�����

	WNDCLASSEX wndClass = { 0 }; //��WNDCLASSEX ������һ��������
	wndClass.cbSize = sizeof(WNDCLASSEX);  //���ýṹ����ֽ�����С
	wndClass.style = CS_HREDRAW | CS_VREDRAW;  //���ô��ڵ���ʽ
	wndClass.lpfnWndProc = WndProc; //����ָ�򴰿ڹ��̺����ĺ���ָ�룬�봰�ڹ��̺��������ͱ�����WNDPROC��ͬ����"LRESULT"
	wndClass.cbClsExtra = 0; //������ĸ����ڴ棬ȡ0�Ϳ�����
	wndClass.cbWndExtra = 0;//������ĸ����ڴ棬��Ȼȡ0�Ϳ�����
	wndClass.hInstance = hInstance;//ָ���������ڹ��̵ĳ����ʵ���������дWinMain�����ĵ�һ��������Ҳ���Ǹó���ǰ���е�ʵ��������ݸ���
	wndClass.hIcon = (HICON)::LoadImage(NULL, L"icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);  //����ָ���������ͼ�����������Ա���������Ǹ�ͼ����Դ�ľ��
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); //ָ��������Ĺ����,һ�������������ΪĬ�ϵļ�ͷ���
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);//ΪhbrBackground��Աָ��һ����ɫ��ˢ���
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName= L"ForTheDreamOfGameDevelop"; //��һ���Կ���ֹ���ַ�����ָ������������֣��������Զ���ģ�������õ���������ʶ�������ڶ�������������
	

	//��2�� ���ڴ����Ĳ���֮����  ע�ᴰ����
	if (!RegisterClassEx(&wndClass)) //��ƴ�����֮����Ҫ�Դ��������ע�ᣬ�������ܴ��������͵Ĵ��ڣ���ľ������Ǵ�����Ķ���ʵ����
	{
		return -1;
	}

	//��3��  ���ڴ����Ĳ�֮������ʽ��������
	
	HWND hwnd = CreateWindow
	(L"ForTheDreamOfGameDevelop", WINDOW_TITLE,  //ϲ���ּ��Ĵ������ں���CreateWindow
		WS_OVERLAPPEDWINDOW,CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
		NULL,NULL,hInstance,NULL
	);

	//��4�� ���ڴ����Ĳ���֮�ģ����ڵ��ƶ�����ʾ�����

	MoveWindow(hwnd, 250, 80, WINDOW_WIDTH, WINDOW_HEIGHT, true);//����������ʾʱ��λ�ã�ʹ�������Ͻ�λ�ڣ�250,80����
	ShowWindow(hwnd, nShowCmd); //��Ϊ�����������WinMain �����ڲ����õģ�ֱ��ȡWinMain �����Ĳ����Ϳ����ˣ�����ShowWindow��������ʾ����
	UpdateWindow(hwnd);//����Ǵ����ɹ���Ĵ��ڵľ��

	//��5�� ��Ϣѭ������/

	MSG msg = { 0 };   //���岢��ʼ��msg
	while (msg.message != WM_QUIT)  //ʹ��whileѭ���������Ϣ����WM_QUIT��Ϣ���ͼ���ѭ��
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))  //�鿴Ӧ�ó�����Ϣ���У�����Ϣʱ�������е���Ϣ�ɷ���ȥ��
		{
			TranslateMessage(&msg);		//���������Ϣת��Ϊ�ַ���Ϣ
			DispatchMessage(&msg);   //�ַ�һ����Ϣ�����ڳ���
		}

		
	}

	//��6��  �������ע��

	UnregisterClass(L"ForTheDreamOfGameDevelop", wndClass.hInstance);

	
	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)  //���ڹ��̺����Ķ���
{
	switch (message) //switch��俪ʼ
	{
	case WM_PAINT:    // ���ǿͻ����ػ���Ϣ
		ValidateRect(hwnd, NULL);		// ���¿ͻ�������ʾ
		break;		//������switch���	

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
