/****************************************************************************************************
	
				PE�鿴��:
	
				�汾:1.0

				����:�ȷ��೤1997

����ʱ��:2023-01-04 11:04

*****************************************************************************************************/
#ifndef DIALOGDETAIL_H
#define DIALOGDETAIL_H

#include "Base.h"

void CreateDialogDetail(int Index);

void InitDetailDialogView();

void ExportInfoView();

void ImportInfoView();

void ResourceInfoView();

void RelocalInfoView();

void BoundImportInfoView();

void IATInfoView();

BOOL CALLBACK DetailDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // !DIALOGDETAIL_H

