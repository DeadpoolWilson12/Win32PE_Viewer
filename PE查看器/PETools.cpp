#include "PETools.h"


DWORD							FileSize = 0;
LPVOID							pFileBuffer = NULL;
PIMAGE_DOS_HEADER				pDosHeader = NULL;
PIMAGE_NT_HEADERS				pNTHeader = NULL;
PIMAGE_FILE_HEADER				pFileHeader = NULL;
PIMAGE_OPTIONAL_HEADER32		pOptionalHeader = NULL;
PIMAGE_SECTION_HEADER			pSectionHeader = NULL;
PIMAGE_EXPORT_DIRECTORY			pExportHeader = NULL;
PIMAGE_IMPORT_DESCRIPTOR		pImportDescriptor = NULL;
PIMAGE_RESOURCE_DIRECTORY	    pResourceHeader = NULL;
PIMAGE_BASE_RELOCATION			pRelocationHeader = NULL;
PIMAGE_BOUND_IMPORT_DESCRIPTOR	pBoundImportDescriptor = NULL;
PIMAGE_BOUND_FORWARDER_REF		pBoundForwarderRef = NULL;
PIMAGE_THUNK_DATA				pIATDirectory = NULL;


DWORD Align(int x, DWORD Alignment)
{
	if (x % Alignment == 0)
	{
		return x;
	}
	else
	{
		return (1 + (x / Alignment)) * Alignment;
	}
}


bool ReadPEFile(const char* FileName)
{
	FILE* pFile = NULL;
	LPVOID FileBuffer = NULL;

	pFile = fopen(FileName, "rb");
	if (!pFile)
	{
		return false;
	}

	fseek(pFile, 0, SEEK_END);
	FileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	FileBuffer = malloc(FileSize);

	if (!FileBuffer)
	{
		fclose(pFile);
		return false;
	}

	size_t ReadSize = fread(FileBuffer, FileSize, 1, pFile);
	if (!ReadSize)
	{
		free(FileBuffer);
		fclose(pFile);
		return false;
	}

	pFileBuffer = FileBuffer;
	fclose(pFile);
	return true;
}


LPSTR GetNameByOrdinal(DWORD Ordinal)
{
	if (!pExportHeader) return NULL;

	LPSTR pMultiChar = (LPSTR)malloc(256);
	memset(pMultiChar, 0, 256);
	int Index = -1;

	PWORD pAddressOfNameOrdinals = (PWORD)((DWORD)pDosHeader + RVA_TO_FOA(pDosHeader, pExportHeader->AddressOfNameOrdinals));
	for (int i = 0; i < pExportHeader->NumberOfNames; i++)
	{
		WORD TempNameIndex = *pAddressOfNameOrdinals;
		pAddressOfNameOrdinals++;
		if ((TempNameIndex + pExportHeader->Base) == Ordinal)
		{
			Index = i;
		}
	}
	if (Index == -1) return NULL;


	PDWORD pAddressOfNames = (PDWORD)((DWORD)pDosHeader + RVA_TO_FOA(pDosHeader, pExportHeader->AddressOfNames));
	for (int i = 0; i < pExportHeader->NumberOfNames; i++)
	{
		DWORD TempNameAddr = *pAddressOfNames;
		pAddressOfNames++;
		if (TempNameAddr == 0) continue;
		if (i == Index)
		{
			pMultiChar = (LPSTR)((DWORD)pDosHeader + RVA_TO_FOA(pDosHeader, TempNameAddr));
		}
	}

	return pMultiChar;
}


DWORD RVA_TO_FOA(PIMAGE_DOS_HEADER pDosHeader, DWORD dwRva)
{
	DWORD dwFov = 0;

	pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pDosHeader + pDosHeader->e_lfanew);
	pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pNTHeader + 4);
	pOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHeader + IMAGE_SIZEOF_FILE_HEADER);

	if (dwRva < pOptionalHeader->SizeOfHeaders && dwRva != 0)
	{
		dwFov = dwRva;
		// printf("FOV位SizeOfHeaders内\n");
		// printf("RVA转FOV成功\n");
		return dwFov;
	}

	PIMAGE_SECTION_HEADER TempSectionHeader = pSectionHeader;
	for (int i = 0; i < pFileHeader->NumberOfSections; i++)
	{
		if (dwRva >= TempSectionHeader->VirtualAddress && dwRva <= (TempSectionHeader->VirtualAddress + TempSectionHeader->SizeOfRawData))
		{
			dwFov = TempSectionHeader->PointerToRawData + dwRva - TempSectionHeader->VirtualAddress;
			// printf("FOV位于第%d个节\n", i + 1);
			// printf("RVA转FOV成功\n");
			return dwFov;
		}
		TempSectionHeader++;
	}


	printf("RVA转FOV失败\n");
	return 0;
}


DWORD FOA_TO_RVA(PIMAGE_DOS_HEADER pDosHeader, DWORD dwFoa)
{
	DWORD dwRva = 0;

	PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pDosHeader + pDosHeader->e_lfanew);
	PIMAGE_FILE_HEADER pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pNTHeader + 4);
	PIMAGE_OPTIONAL_HEADER pOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHeader + IMAGE_SIZEOF_FILE_HEADER);

	if (dwFoa < pOptionalHeader->SizeOfHeaders)
	{
		dwRva = dwFoa;
		return dwRva;
	}

	PIMAGE_SECTION_HEADER TempSectionHeader = pSectionHeader;
	for (int i = 0; i < pFileHeader->NumberOfSections; i++)
	{
		if (dwFoa >= TempSectionHeader->PointerToRawData && dwFoa <= (TempSectionHeader->PointerToRawData + TempSectionHeader->SizeOfRawData))
		{
			dwRva = TempSectionHeader->VirtualAddress + dwFoa - TempSectionHeader->PointerToRawData;
			return dwRva;
		}
		TempSectionHeader++;
	}

	printf("FOA转RVA失败\n");
	return 0;
}


bool InitPEBuffer()
{
	pDosHeader = (PIMAGE_DOS_HEADER)pFileBuffer;
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		free(pFileBuffer);
		printf("该文件不是PE文件\n");
		return false;
	}

	pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pFileBuffer + pDosHeader->e_lfanew);
	if (pNTHeader->Signature != IMAGE_NT_SIGNATURE)
	{
		free(pFileBuffer);
		printf("该文件不是PE文件\n");
		return false;
	}

	pFileHeader = (PIMAGE_FILE_HEADER)((DWORD)pNTHeader + 4);
	pOptionalHeader = (PIMAGE_OPTIONAL_HEADER)((DWORD)pFileHeader + IMAGE_SIZEOF_FILE_HEADER);
	pSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD)pOptionalHeader + pFileHeader->SizeOfOptionalHeader);

	DWORD FOA_addr = RVA_TO_FOA(pDosHeader, pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	if(FOA_addr) pExportHeader = (PIMAGE_EXPORT_DIRECTORY)((DWORD)pDosHeader + FOA_addr);

	FOA_addr = RVA_TO_FOA(pDosHeader, pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
	if (FOA_addr) pImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)pDosHeader + FOA_addr);

	FOA_addr = RVA_TO_FOA(pDosHeader, pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress);
	if (FOA_addr) pResourceHeader = (PIMAGE_RESOURCE_DIRECTORY)((DWORD)pDosHeader + FOA_addr);

	FOA_addr = RVA_TO_FOA(pDosHeader, pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
	if (FOA_addr) pRelocationHeader = (PIMAGE_BASE_RELOCATION)((DWORD)pDosHeader + FOA_addr);

	FOA_addr = RVA_TO_FOA(pDosHeader, pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress);
	if (FOA_addr)
	{
		pBoundImportDescriptor = (PIMAGE_BOUND_IMPORT_DESCRIPTOR)((DWORD)pDosHeader + FOA_addr);
		pBoundForwarderRef = (PIMAGE_BOUND_FORWARDER_REF)((DWORD)pBoundImportDescriptor + sizeof(IMAGE_BOUND_IMPORT_DESCRIPTOR));
	}

	FOA_addr = RVA_TO_FOA(pDosHeader, pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress);
	if (FOA_addr) pIATDirectory = (PIMAGE_THUNK_DATA)((DWORD)pDosHeader + FOA_addr);

	return true;
}