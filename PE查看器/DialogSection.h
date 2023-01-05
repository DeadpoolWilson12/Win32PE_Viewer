/****************************************************************************************************
	
				PE查看器:
	
				版本:1.0

				作者:热饭班长1997

创建时间:2023-01-04 11:04

*****************************************************************************************************/
#ifndef DIALOGSECTION_H
#define DIALOGSECTION_H

#include "Base.h"

void CreateDialogSection();

void InitSectionsListView();

void ShowSectionListInfo();

BOOL CALLBACK SectionDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // !DIALOGSECTION_H

