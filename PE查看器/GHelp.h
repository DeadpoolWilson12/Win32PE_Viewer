/****************************************************************************************************
	
				PE�鿴��:
	
				�汾:1.0

				����:�ȷ��೤1997

����ʱ��:2023-01-04 11:04

*****************************************************************************************************/
#ifndef GHELP_H
#define GHELP_H

#include "Base.h"
#include <time.h>

LPCSTR WCHAR_TO_CHAR(LPWSTR lpWideCharStr);

LPWSTR CHAR_TO_WCHAR(LPSTR lpMultiCharStr);

LPWSTR TimeStampToTime(time_t TimeDateStamp);

#endif
