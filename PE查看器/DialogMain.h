/****************************************************************************************************
	
				PE�鿴��:
	
				�汾:1.0

				����:�ȷ��೤1997

����ʱ��:2023-01-04 11:04

*****************************************************************************************************/
#ifndef DIALOGMAIN_H
#define DIALOGMAIN_H

#include "Base.h"

extern HWND      ghwndListProcess;
extern HWND      ghwndListModule;

BOOL GetMainModuleInfo(DWORD dwPid, MODULEINFO* moduleInfo);

BOOL GetAllModules_(DWORD dwPid, HMODULE* pModuel, int nSize, DWORD* dwNumOfModules);

void ShowAllProcessInfo(HWND hwndList);

void ShowProcessAllModule();

void SetIcon(HWND hDlg);

void InitListViewProcess(HWND hDlg);

void InitListViewModules(HWND hDlg);

BOOL CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif
