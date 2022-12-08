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