/****************************************************************************************************
	
				PE�鿴��:
	
				�汾:1.0

				����:�ȷ��೤1997

����ʱ��:2023-01-04 11:04

*****************************************************************************************************/
#ifndef DIALOGSECTION_H
#define DIALOGSECTION_H

#include "Base.h"

void CreateDialogSection();

void InitSectionsListView();

void ShowSectionListInfo();

BOOL CALLBACK SectionDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // !DIALOGSECTION_H

