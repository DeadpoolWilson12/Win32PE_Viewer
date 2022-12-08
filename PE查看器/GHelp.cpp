#include "GHelp.h"


LPCSTR WCHAR_TO_CHAR(LPWSTR lpWideCharStr)
{
	if (!lpWideCharStr) return NULL;

	CHAR str[512] = { 0 };
	int iSize = 0;
	
	// ��һ�ε���ȡ��С
	iSize = WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, -1, NULL, 0, NULL, NULL);
	// �ڶ��ε�����ʽת��
	WideCharToMultiByte(CP_ACP, 0, lpWideCharStr, -1, str, iSize, NULL, NULL);

	return str;
}


LPWSTR CHAR_TO_WCHAR(LPSTR lpMultiCharStr)
{
	if (!lpMultiCharStr) return NULL;
	WCHAR wstr[512] = { 0 };
	int iSize = 0;

	iSize = MultiByteToWideChar(CP_ACP, 0, lpMultiCharStr, -1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, lpMultiCharStr, -1, wstr, iSize);

	return wstr;
}


LPWSTR TimeStampToTime(time_t TimeDateStamp)
{
	tm TimeIns = { 0 };
	WCHAR TimeBuffer[100] = { 0 };
	TimeIns = *localtime(&TimeDateStamp);
	wcsftime(TimeBuffer, sizeof(TimeBuffer), L"%Y-%m-%d %H:%M:%S", &TimeIns);

	return TimeBuffer;
}