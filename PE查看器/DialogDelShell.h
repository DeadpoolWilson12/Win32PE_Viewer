/****************************************************************************************************
	
				PE�鿴��:
	
				�汾:1.0

				����:�ȷ��೤1997

����ʱ��:2023-01-04 11:04

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

