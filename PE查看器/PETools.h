/****************************************************************************************************
	
				PE查看器:
	
				版本:1.0

				作者:热饭班长1997

创建时间:2023-01-04 11:04

*****************************************************************************************************/
#ifndef PETOOLS_H
#define PETOOLS_H

#include "Base.h"

extern CONTEXT		context;
extern HANDLE		ProcessHandle;

extern LPVOID						pShellFile;
extern LPVOID						pNewShellFile;
extern LPVOID						pOriginalFile;
extern DWORD						ShellSize;
extern DWORD						NewShellSize;
extern DWORD						FileSize;
extern LPVOID						pFileBuffer;

extern PIMAGE_DOS_HEADER				pDosHeader;
extern PIMAGE_NT_HEADERS				pNTHeader;
extern PIMAGE_FILE_HEADER				pFileHeader;
extern PIMAGE_OPTIONAL_HEADER32			pOptionalHeader;
extern PIMAGE_SECTION_HEADER			pSectionHeader;
extern PIMAGE_EXPORT_DIRECTORY			pExportHeader;
extern PIMAGE_IMPORT_DESCRIPTOR			pImportDescriptor;
extern PIMAGE_RESOURCE_DIRECTORY		pResourceHeader;
extern PIMAGE_BASE_RELOCATION			pRelocationHeader;
extern PIMAGE_BOUND_IMPORT_DESCRIPTOR	pBoundImportDescriptor;
extern PIMAGE_BOUND_FORWARDER_REF		pBoundForwarderRef;
extern PIMAGE_THUNK_DATA				pIATDirectory;


DWORD RVA_TO_FOA(PIMAGE_DOS_HEADER pDosHeader, DWORD dwRva);

DWORD FOA_TO_RVA(PIMAGE_DOS_HEADER pDosHeader, DWORD dwFoa);

DWORD RVA_TO_FOA(LPVOID FileBuffer, DWORD dwRva);

bool ReadPEFile(const char* FileName);

bool ReadPEFile(_In_ const char* FileName, _Out_ LPVOID& FileBuffer_, _Out_ DWORD& OutFileSize);

bool MemeryToFile(IN LPVOID pMemBuffer, LPCSTR FileName, IN size_t size);

bool InitPEBuffer();

bool InitShellPEInfo(char* ShellFile);

LPSTR GetNameByOrdinal(DWORD Ordinal);

bool ExpendNewSection(char* ProcessFile);

bool WriteFileToNewSection(char* ProcessFile, char* NewFileName);

bool ProgramShelling(char* ShellFile, char* ProcessFile, char* NewFileName);

bool FixRelocationDirectory(DWORD NewImageBase);

bool CopyFileBufferToImageBuffer(_In_ LPVOID pFilebuffer_, _Out_ LPVOID& pImageBuffer_);

bool InitPEAllHeaderInfo(char* FilePath);

bool UpDateGlobalPEInfo(LPVOID pFileBuffer_);

bool DeleteShellAndSave(char* FilePath, char* SaveFilePath);

bool DeleteShellAndRun(char* FilePath);

void ClearAllBuffer();

#endif
