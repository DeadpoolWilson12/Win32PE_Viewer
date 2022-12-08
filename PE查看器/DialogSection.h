#ifndef DIALOGSECTION_H
#define DIALOGSECTION_H

#include "Base.h"

void CreateDialogSection();

void InitSectionsListView();

void ShowSectionListInfo();

BOOL CALLBACK SectionDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // !DIALOGSECTION_H

