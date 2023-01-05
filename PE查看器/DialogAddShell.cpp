/****************************************************************************************************
	
				PE�鿴��:
	
				�汾:1.0

				����:�ȷ��೤1997

����ʱ��:2023-01-04 11:04

*****************************************************************************************************/
#include "DialogAddShell.h"
#include "PETools.h"
#include "GHelp.h"

HWND gAddShellDialog = NULL;
TCHAR wstrShellFileName[256] = { 0 };
TCHAR wstrProcessFileName[256] = { 0 };


BOOL CALLBACK AddShellDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		memset(wstrShellFileName, 0, sizeof(wstrShellFileName) - 1);
		memset(wstrProcessFileName, 0, sizeof(wstrProcessFileName) - 1);
		gAddShellDialog = hDlg;
		return TRUE;
	}
	case WM_CLOSE:
	{
		ClearAddShellBuffer();
		EndDialog(hDlg, 0);
		return TRUE;
	}
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_SHELLFILE:
			AddShellFile();
			return TRUE;
		case IDC_BUTTON_PROCESSFILE:
			AddProcessFile();
			return TRUE;
		case IDC_BUTTON_STARTADDSHELL:
			StartAddShell();
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


void CreateDialogAddShell()
{
	DialogBox(gInstance, MAKEINTRESOURCE(IDD_DIALOG_ADDSHELL), ghDlg, AddShellDialogProc);
}


void AddShellFile()
{
	// �����ļ�ѡ����
	TCHAR strFeFileExt[128] = TEXT("EXE File(*.exe)\0*.exe\0") \
		TEXT("All File(*.*)\0*.*\0\0");
	OPENFILENAMEW st = { 0 };
	ZeroMemory(&st, sizeof(OPENFILENAME));
	st.lStructSize = sizeof(OPENFILENAME);
	st.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	st.hwndOwner = gAddShellDialog;
	st.lpstrFilter = strFeFileExt;
	st.lpstrFile = wstrShellFileName;
	st.nMaxFile = MAX_PATH;

	if (GetOpenFileName(&st) == FALSE)
	{
		return;
	}

	HWND hwndEdit = GetDlgItem(gAddShellDialog, IDC_EDIT_SHELLFILE);
	SetWindowText(hwndEdit, wstrShellFileName);
}


void AddProcessFile()
{
	// �����ļ�ѡ����
	TCHAR strFeFileExt[128] = TEXT("EXE File(*.exe)\0*.exe\0") \
		TEXT("All File(*.*)\0*.*\0\0");
	OPENFILENAMEW st = { 0 };
	ZeroMemory(&st, sizeof(OPENFILENAME));
	st.lStructSize = sizeof(OPENFILENAME);
	st.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	st.hwndOwner = gAddShellDialog;
	st.lpstrFilter = strFeFileExt;
	st.lpstrFile = wstrProcessFileName;
	st.nMaxFile = MAX_PATH;

	if (GetOpenFileName(&st) == FALSE)
	{
		return;
	}

	HWND hwndEdit = GetDlgItem(gAddShellDialog, IDC_EDIT_PROCESSFILE);
	SetWindowText(hwndEdit, wstrProcessFileName);
}


void StartAddShell()
{
	if (wcslen(wstrShellFileName) == 0 || wcslen(wstrProcessFileName) == 0)
	{
		MessageBox(gAddShellDialog, L"δ��д��Ŀ¼�����Ŀ¼", L"[ WARNING ]", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	TCHAR wstrNewFileName[256] = { 0 };
	GetCurrentDirectoryW(256, wstrNewFileName);
	wcscat(wstrNewFileName, L"\\�ӿǺ�ĳ���.exe");
	
	TCHAR strFeFileExt[128] = TEXT("EXE File(*.exe)\0*.exe\0") \
		TEXT("All File(*.*)\0*.*\0\0");
	OPENFILENAMEW st = { 0 };
	ZeroMemory(&st, sizeof(OPENFILENAME));
	st.lStructSize = sizeof(OPENFILENAME);
	st.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_NOCHANGEDIR | OFN_EXTENSIONDIFFERENT;
	st.hwndOwner = gAddShellDialog;
	st.lpstrFilter = strFeFileExt;
	st.lpstrFile = wstrNewFileName;
	st.nMaxFile = MAX_PATH;

	if (GetSaveFileName(&st) == FALSE)
	{
		return;
	}

	ClearAddShellBuffer();
	if (!ProgramShelling((LPSTR)WCHAR_TO_CHAR(wstrShellFileName), (LPSTR)WCHAR_TO_CHAR(wstrProcessFileName), (LPSTR)WCHAR_TO_CHAR(wstrNewFileName)))
	{
		MessageBox(gAddShellDialog, L"�ӿ�ʧ��", L"[ WARNING ]", MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	MessageBox(gAddShellDialog, L"�ӿǳɹ�!", L"[ SUCCESS ]", MB_OK);
	return;
}


void ClearAddShellBuffer()
{
	ClearAllBuffer();
}