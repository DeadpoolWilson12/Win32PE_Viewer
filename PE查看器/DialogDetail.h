/****************************************************************************************************
	
				PE查看器:
	
				版本:1.0

				作者:热饭班长1997

创建时间:2023-01-04 11:04

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

