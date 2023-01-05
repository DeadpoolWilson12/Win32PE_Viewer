/****************************************************************************************************
	
				PE查看器:
	
				版本:1.0

				作者:热饭班长1997

创建时间:2023-01-04 11:04

*****************************************************************************************************/
#ifndef DIALOGDIRECTORY_H
#define DIALOGDIRECTORY_H

#include "Base.h"

extern HWND gDirDialog;

void CreateDialogDir();

void InitDirDialogView();

BOOL CALLBACK DirectoryDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // !DIALOGDIRECTORY_H

