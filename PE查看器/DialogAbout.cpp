/****************************************************************************************************
	
				PE查看器:
	
				版本:1.0

				作者:热饭班长1997

创建时间:2023-01-04 11:04

*****************************************************************************************************/
#include "DialogAbout.h"

HBITMAP hBitmap = NULL;
HWND    gAboutDlg = NULL;


BOOL CALLBACK AboutDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        gAboutDlg = hDlg;
        InitDialogAbout();
        return TRUE;
    }
    case WM_CLOSE:
    {
        DeleteObject(hBitmap);
        EndDialog(hDlg, 0);
        return TRUE;
    }
    default:
        break;
    }
    return FALSE;
}


void CreateDialogAbout()
{
    DialogBox(gInstance, MAKEINTRESOURCE(IDD_DIALOG_ABOUT), ghDlg, AboutDialogProc);
}


void InitDialogAbout()
{
    hBitmap = LoadBitmap(gInstance, MAKEINTRESOURCE(IDB_BITMAP_DOGE));
    HWND hStatic = CreateWindow(TEXT("STATIC"), NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, 0, 0, 120, 150, gAboutDlg, (HMENU)10000, gInstance, NULL);
    SendMessage(hStatic, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);
}