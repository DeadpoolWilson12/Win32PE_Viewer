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