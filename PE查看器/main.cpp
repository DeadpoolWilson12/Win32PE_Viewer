/****************************************************************************************************
	
				PE�鿴��:
	
				�汾:1.0

				����:�ȷ��೤1997

����ʱ��:2023-01-04 11:04

*****************************************************************************************************/
#include "Base.h"
#include "DialogMain.h"

HINSTANCE gInstance;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hpre, PWSTR pCmdLine, int nCmdShow)
{
    gInstance = hInstance;
    // �����Ի���
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL, DialogProc);

    return 0;
}