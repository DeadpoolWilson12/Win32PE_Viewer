#ifndef DIALOGDIRECTORY_H
#define DIALOGDIRECTORY_H

#include "Base.h"

extern HWND gDirDialog;

void CreateDialogDir();

void InitDirDialogView();

BOOL CALLBACK DirectoryDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // !DIALOGDIRECTORY_H

