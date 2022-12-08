#ifndef GHELP_H
#define GHELP_H

#include "Base.h"
#include <time.h>

LPCSTR WCHAR_TO_CHAR(LPWSTR lpWideCharStr);

LPWSTR CHAR_TO_WCHAR(LPSTR lpMultiCharStr);

LPWSTR TimeStampToTime(time_t TimeDateStamp);

#endif
