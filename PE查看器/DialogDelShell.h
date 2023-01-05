/****************************************************************************************************
	
				PE查看器:
	
				版本:1.0

				作者:热饭班长1997

创建时间:2023-01-04 11:04

*****************************************************************************************************/
#ifndef DIALOGDELSHELL_H
#define DIALOGDELSHELL_H

#include "Base.h"

extern HWND gDelShellDialog;

void CreateDialogDelShell();

void AddDelShellFile();

void DelShellAndSave();

void DelShellAndRun();

void ClearDelShellBuffer();

BOOL CALLBACK DelShellDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // !DIALOGDELSHELL_H

