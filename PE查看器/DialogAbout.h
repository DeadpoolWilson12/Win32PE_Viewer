/****************************************************************************************************
	
				PE查看器:
	
				版本:1.0

				作者:热饭班长1997

创建时间:2023-01-04 11:04

*****************************************************************************************************/
#ifndef DIALOGABOUT_H
#define DIALOGABOUT_H

#include "Base.h"

void CreateDialogAbout();

void InitDialogAbout();

BOOL CALLBACK AboutDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif
