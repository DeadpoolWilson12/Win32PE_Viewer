/****************************************************************************************************
	
				PE查看器:
	
				版本:1.0

				作者:热饭班长1997

创建时间:2023-01-04 11:04

*****************************************************************************************************/
#include "Base.h"
#include "DialogMain.h"

HINSTANCE gInstance;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hpre, PWSTR pCmdLine, int nCmdShow)
{
    gInstance = hInstance;
    // 创建对话框
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, DialogProc);

    return 0;
}