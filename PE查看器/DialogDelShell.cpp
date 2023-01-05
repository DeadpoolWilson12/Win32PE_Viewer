/****************************************************************************************************
	
				PE查看器:
	
				版本:1.0

				作者:热饭班长1997

创建时间:2023-01-04 11:04

*****************************************************************************************************/
#include "DialogDelShell.h"
#include "PETools.h"
#include "GHelp.h"


HWND gDelShellDialog = NULL;
TCHAR wstrProcessFileName_[256] = { 0 };

BOOL CALLBACK DelShellDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		memset(wstrProcessFileName_, 0, sizeof(wstrProcessFileName_) - 1);
		gDelShellDialog = hDlg;
		return TRUE;
	}
	case WM_CLOSE:
	{
		ClearDelShellBuffer();
		EndDialog(hDlg, 0);
		return TRUE;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_DELSHELLFILE:
			AddDelShellFile();
			return TRUE;
		case IDC_BUTTON_DELSHELLSAVE:
			DelShellAndSave();
			return TRUE;
		case IDC_BUTTON_DELSHELLRUN:
			DelShellAndRun();
			return TRUE;
		default:
			break;
		}
		break;
	}
	default:
		break;
	}

	return FALSE;
}


void CreateDialogDelShell()
{
	DialogBox(gInstance, MAKEINTRESOURCE(IDD_DIALOG_DELSHELL), ghDlg, DelShellDialogProc);
}


void AddDelShellFile()
{
	// 弹出文件选择器
	TCHAR strFeFileExt[128] = TEXT("EXE File(*.exe)\0*.exe\0") \
		TEXT("All File(*.*)\0*.*\0\0");
	OPENFILENAMEW st = { 0 };
	ZeroMemory(&st, sizeof(OPENFILENAME));
	st.lStructSize = sizeof(OPENFILENAME);
	st.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	st.hwndOwner = gDelShellDialog;
	st.lpstrFilter = strFeFileExt;
	st.lpstrFile = wstrProcessFileName_;
	st.nMaxFile = MAX_PATH;

	if (GetOpenFileName(&st) == FALSE)
	{
		return;
	}

	HWND hwndEdit = GetDlgItem(gDelShellDialog, IDC_EDIT_DELSHELLFILE);
	SetWindowText(hwndEdit, wstrProcessFileName_);
}


void DelShellAndSave()
{
	if (wcslen(wstrProcessFileName_) == 0)
	{
		MessageBox(gDelShellDialog, L"未填写程序目录", L"[ WARNING ]", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	TCHAR wstrNewFileName[256] = { 0 };
	GetCurrentDirectoryW(256, wstrNewFileName);
	wcscat(wstrNewFileName, L"\\解壳后的程序.exe");

	TCHAR strFeFileExt[128] = TEXT("EXE File(*.exe)\0*.exe\0") \
		TEXT("All File(*.*)\0*.*\0\0");
	OPENFILENAMEW st = { 0 };
	ZeroMemory(&st, sizeof(OPENFILENAME));
	st.lStructSize = sizeof(OPENFILENAME);
	st.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_NOCHANGEDIR | OFN_EXTENSIONDIFFERENT;
	st.hwndOwner = gDelShellDialog;
	st.lpstrFilter = strFeFileExt;
	st.lpstrFile = wstrNewFileName;
	st.nMaxFile = MAX_PATH;

	if (GetSaveFileName(&st) == FALSE)
	{
		return;
	}

	ClearDelShellBuffer();
	if (!DeleteShellAndSave((char*)WCHAR_TO_CHAR(wstrProcessFileName_), (char*)WCHAR_TO_CHAR(wstrNewFileName)))
	{
		MessageBox(gDelShellDialog, L"脱壳或保存文件失败", L"[ WARNING ]", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	MessageBox(gDelShellDialog, L"成功脱壳并保存文件!", L"[ SUCCESS ]", MB_OK);
	return;
}


void DelShellAndRun()
{
	if (wcslen(wstrProcessFileName_) == 0)
	{
		MessageBox(gDelShellDialog, L"未填写程序目录", L"[ WARNING ]", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	ClearDelShellBuffer();
	if (!DeleteShellAndRun((char*)WCHAR_TO_CHAR(wstrProcessFileName_)))
	{
		if (ProcessHandle != NULL)
		{
			TerminateThread(ProcessHandle, 0);
			ProcessHandle = NULL;
		}
		MessageBox(gDelShellDialog, L"脱壳或运行失败", L"[ WARNING ]", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	MessageBox(gDelShellDialog, L"成功脱壳并成功运行!", L"[ SUCCESS ]", MB_OK);
	return;
}


void ClearDelShellBuffer()
{
	ClearAllBuffer();
	if (ProcessHandle != NULL)
	{
		TerminateThread(ProcessHandle, 0);
		ProcessHandle = NULL;
	}
}