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

