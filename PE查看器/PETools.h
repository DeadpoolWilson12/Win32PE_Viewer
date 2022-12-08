#ifndef PETOOLS_H
#define PETOOLS_H

#include "Base.h"

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

bool ReadPEFile(const char* FileName);

bool InitPEBuffer();

LPSTR GetNameByOrdinal(DWORD Ordinal);

#endif
