/****************************************************************************************************
	
				PE�鿴��:
	
				�汾:1.0

				����:�ȷ��೤1997

����ʱ��:2023-01-04 11:04

*****************************************************************************************************/
#ifndef DIALOGADDSHELL_H
#define DIALOGADDSHELL_H

#include "Base.h"

extern HWND gAddShellDialog;

void CreateDialogAddShell();

void AddShellFile();

void AddProcessFile();

void StartAddShell();

void ClearAddShellBuffer();

BOOL CALLBACK AddShellDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // !DIALOGADDSHELL_H

