#ifndef DIALOGPECHEAK_H
#define DIALOGPECHEAK_H

#include "Base.h"

extern HWND gPECheakDialog;

void CreateDialogPECheak();

void TipStaticInfo(LPCTSTR str);

void PEBaseInfoView();

BOOL CALLBACK PECheakDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // !DIALOGPECHEAK_H

