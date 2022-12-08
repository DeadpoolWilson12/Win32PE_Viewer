#ifndef DIALOGABOUT_H
#define DIALOGABOUT_H

#include "Base.h"

void CreateDialogAbout();

void InitDialogAbout();

BOOL CALLBACK AboutDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif
