#include<Windows.h>
#include"stdafx.h"
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	PlaySound(L"FirstBlood.wav", NULL, SND_FILENAME | SND_ASYNC);   //������Ч
	MessageBox(NULL, L"First blood! ��ã���Ϸ���������磬�������������ˣ�", L"First blood! ��Ϣ����", 0);//��ʾһ����Ϣ��
	return 0;
}
