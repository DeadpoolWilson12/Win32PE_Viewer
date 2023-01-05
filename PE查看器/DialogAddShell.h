/****************************************************************************************************
	
				PE查看器:
	
				版本:1.0

				作者:热饭班长1997

创建时间:2023-01-04 11:04

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

