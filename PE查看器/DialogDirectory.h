/****************************************************************************************************
	
				PE�鿴��:
	
				�汾:1.0

				����:�ȷ��೤1997

����ʱ��:2023-01-04 11:04

*****************************************************************************************************/
#ifndef DIALOGDIRECTORY_H
#define DIALOGDIRECTORY_H

#include "Base.h"

extern HWND gDirDialog;

void CreateDialogDir();

void InitDirDialogView();

BOOL CALLBACK DirectoryDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // !DIALOGDIRECTORY_H

