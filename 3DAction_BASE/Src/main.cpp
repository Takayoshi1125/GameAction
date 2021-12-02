//------------------------------------------
// 氏名      ：氏名
//------------------------------------------
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"
#include <DxLib.h>
#include "Application.h"

// WinMain関数
//---------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	Application application;	//インスタンスの生成

	if (!application.Init())
	{
		return -1;
	}

	application.Run();

	if (!application.Release())
	{
		return -1;
	}

	return 0;

}
