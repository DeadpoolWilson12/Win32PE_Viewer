/****************************************************************************************************
	
				PE查看器:
	
				版本:1.0

				作者:热饭班长1997

创建时间:2023-01-04 11:04

*****************************************************************************************************/
#ifndef GHELP_H
#define GHELP_H

#include "Base.h"
#include <time.h>

LPCSTR WCHAR_TO_CHAR(LPWSTR lpWideCharStr);

LPWSTR CHAR_TO_WCHAR(LPSTR lpMultiCharStr);

LPWSTR TimeStampToTime(time_t TimeDateStamp);

#endif
