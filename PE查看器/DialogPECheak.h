/****************************************************************************************************
	
				PE�鿴��:
	
				�汾:1.0

				����:�ȷ��೤1997

����ʱ��:2023-01-04 11:04

*****************************************************************************************************/
#ifndef DIALOGPECHEAK_H
#define DIALOGPECHEAK_H

#include "Base.h"

extern HWND gPECheakDialog;

void CreateDialogPECheak();

void TipStaticInfo(LPCTSTR str);

void PEBaseInfoView();

void ClearPECheakBuffer();

BOOL CALLBACK PECheakDialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif // !DIALOGPECHEAK_H

